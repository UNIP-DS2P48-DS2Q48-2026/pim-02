/* Hash de senhas PBKDF2-HMAC-SHA256, sal aleatorio por usuario.
   Novos cadastros usam formato pbkdf2_sha256$iteracoes$salHex$hashHex.
   Usuarios antigos (hash decimal legado) continuam entrando e sao atualizados no login.
   SHA-256 implementado aqui mesmo, sem OpenSSL: compila no Windows (MinGW) e no Linux.
   O sal aleatorio vem do CryptGenRandom no Windows (-ladvapi32) e de /dev/urandom no Linux. */
#ifdef _WIN32
#include <wincrypt.h>
#endif

#define SENHA_ITERACOES 600000

/* ---------- SHA-256 (FIPS 180-4) ---------- */
typedef struct
{
    unsigned int estado[8];
    unsigned char bloco[64];
    unsigned int usados;
    unsigned long long total;
} Sha256;

static const unsigned int sha256K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define SHA_ROTR(x,n) (((x)>>(n))|((x)<<(32-(n))))

static void sha256Bloco(Sha256 *s, const unsigned char *p)
{
    unsigned int w[64],a,b,c,d,e,f,g,h,t1,t2;
    int i;
    for(i=0;i<16;i++)
        w[i]=((unsigned int)p[i*4]<<24)|((unsigned int)p[i*4+1]<<16)|((unsigned int)p[i*4+2]<<8)|p[i*4+3];
    for(i=16;i<64;i++)
        w[i]=(SHA_ROTR(w[i-2],17)^SHA_ROTR(w[i-2],19)^(w[i-2]>>10))+w[i-7]
            +(SHA_ROTR(w[i-15],7)^SHA_ROTR(w[i-15],18)^(w[i-15]>>3))+w[i-16];
    a=s->estado[0];b=s->estado[1];c=s->estado[2];d=s->estado[3];
    e=s->estado[4];f=s->estado[5];g=s->estado[6];h=s->estado[7];
    for(i=0;i<64;i++){
        t1=h+(SHA_ROTR(e,6)^SHA_ROTR(e,11)^SHA_ROTR(e,25))+((e&f)^(~e&g))+sha256K[i]+w[i];
        t2=(SHA_ROTR(a,2)^SHA_ROTR(a,13)^SHA_ROTR(a,22))+((a&b)^(a&c)^(b&c));
        h=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;
    }
    s->estado[0]+=a;s->estado[1]+=b;s->estado[2]+=c;s->estado[3]+=d;
    s->estado[4]+=e;s->estado[5]+=f;s->estado[6]+=g;s->estado[7]+=h;
}
static void sha256Inicio(Sha256 *s)
{
    static const unsigned int inicial[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
                                          0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    memcpy(s->estado,inicial,sizeof(inicial));
    s->usados=0;s->total=0;
}
static void sha256Somar(Sha256 *s, const unsigned char *dados, size_t n)
{
    size_t i;
    for(i=0;i<n;i++){
        s->bloco[s->usados++]=dados[i];
        if(s->usados==64){sha256Bloco(s,s->bloco);s->usados=0;}
    }
    s->total+=n;
}
static void sha256Fim(Sha256 *s, unsigned char saida[32])
{
    unsigned long long bits=s->total*8;
    unsigned char um=0x80,zero=0,tamanho[8];
    int i;
    sha256Somar(s,&um,1);
    while(s->usados!=56)sha256Somar(s,&zero,1);
    for(i=0;i<8;i++)tamanho[i]=(unsigned char)(bits>>(56-8*i));
    sha256Somar(s,tamanho,8);
    for(i=0;i<8;i++){
        saida[i*4]=(unsigned char)(s->estado[i]>>24);saida[i*4+1]=(unsigned char)(s->estado[i]>>16);
        saida[i*4+2]=(unsigned char)(s->estado[i]>>8);saida[i*4+3]=(unsigned char)s->estado[i];
    }
}

/* ---------- PBKDF2-HMAC-SHA256 (RFC 8018) ---------- */
/* As chaves interna/externa do HMAC sao calculadas uma vez e reaproveitadas em cada iteracao. */
static void senhaPbkdf2(const char *senha, const unsigned char *sal, size_t tamSal,
                        long iteracoes, unsigned char saida[32])
{
    unsigned char chave[64],ipad[64],opad[64],u[32],contador[4]={0,0,0,1};
    Sha256 interno,externo,s;
    size_t tamSenha=strlen(senha);
    long i;
    int j;
    memset(chave,0,sizeof(chave));
    if(tamSenha>64){sha256Inicio(&s);sha256Somar(&s,(const unsigned char *)senha,tamSenha);sha256Fim(&s,chave);}
    else memcpy(chave,senha,tamSenha);
    for(j=0;j<64;j++){ipad[j]=chave[j]^0x36;opad[j]=chave[j]^0x5c;}
    sha256Inicio(&interno);sha256Somar(&interno,ipad,64);
    sha256Inicio(&externo);sha256Somar(&externo,opad,64);

    /* U1 = HMAC(senha, sal || 00000001) */
    s=interno;sha256Somar(&s,sal,tamSal);sha256Somar(&s,contador,4);sha256Fim(&s,u);
    s=externo;sha256Somar(&s,u,32);sha256Fim(&s,u);
    memcpy(saida,u,32);
    for(i=1;i<iteracoes;i++){
        s=interno;sha256Somar(&s,u,32);sha256Fim(&s,u);
        s=externo;sha256Somar(&s,u,32);sha256Fim(&s,u);
        for(j=0;j<32;j++)saida[j]^=u[j];
    }
    memset(chave,0,sizeof(chave));memset(u,0,sizeof(u));
}

/* Preenche destino com bytes aleatorios do sistema; devolve 1 se conseguiu */
static int senhaAleatorio(unsigned char *destino, size_t n)
{
#ifdef _WIN32
    HCRYPTPROV provedor;
    int ok;
    if(!CryptAcquireContext(&provedor,NULL,NULL,PROV_RSA_FULL,CRYPT_VERIFYCONTEXT))return 0;
    ok=CryptGenRandom(provedor,(DWORD)n,destino)!=0;
    CryptReleaseContext(provedor,0);
    return ok;
#else
    FILE *f=fopen("/dev/urandom","rb");
    size_t lidos;
    if(!f)return 0;
    lidos=fread(destino,1,n,f);
    fclose(f);
    return lidos==n;
#endif
}

/* Compara em tempo constante, para nao vazar onde a senha difere */
static int senhaIguais(const unsigned char *a, const unsigned char *b, size_t n)
{
    unsigned char diferenca=0;
    size_t i;
    for(i=0;i<n;i++)diferenca|=a[i]^b[i];
    return diferenca==0;
}

static void senhaHex(const unsigned char *origem, size_t n, char *destino)
{
    static const char *digitos="0123456789abcdef";
    size_t i;
    for (i=0;i<n;i++) {
        destino[i*2]=digitos[origem[i]>>4];
        destino[i*2+1]=digitos[origem[i]&15];
    }
    destino[n*2]='\0';
}
static int senhaNibble(char c)
{
    if(c>='0'&&c<='9')return c-'0';
    if(c>='a'&&c<='f')return c-'a'+10;
    if(c>='A'&&c<='F')return c-'A'+10;
    return -1;
}
static int senhaDeshex(const char *origem, size_t n, unsigned char *destino)
{
    size_t i;
    for(i=0;i<n;i++){
        int a=senhaNibble(origem[2*i]),b=senhaNibble(origem[2*i+1]);
        if(a<0||b<0)return 0;
        destino[i]=(unsigned char)((a<<4)|b);
    }
    return 1;
}
static unsigned int senhaLegada(const char *texto)
{
    unsigned int h=5381;
    const unsigned char *p=(const unsigned char *)texto;
    while(*p)h=h*33+*p++;
    return h;
}
void gerarHash(char senha[], char destino[])
{
    unsigned char sal[16],hash[32];char salHex[33],hashHex[65];
    destino[0]='\0';
    if(!senha || !*senha || !senhaAleatorio(sal,sizeof(sal)))return;
    senhaPbkdf2(senha,sal,sizeof(sal),SENHA_ITERACOES,hash);
    senhaHex(sal,sizeof(sal),salHex);senhaHex(hash,sizeof(hash),hashHex);
    snprintf(destino,TAM_HASH,"pbkdf2_sha256$%d$%s$%s",
             SENHA_ITERACOES,salHex,hashHex);
    memset(hash,0,sizeof(hash));
}
int verificarSenha(const char *digitada, const char *armazenada)
{
    const char *prefixo="pbkdf2_sha256$";
    const char *p,*fim;
    char tmp[12];char *endptr;
    long iteracoes;
    unsigned char sal[16],calc[32],esperado[32];
    int ok;
    if(!digitada || !armazenada)return 0;
    if(strncmp(armazenada,prefixo,strlen(prefixo))!=0){
        char legado[17];
        snprintf(legado,sizeof(legado),"%u",senhaLegada(digitada));
        return strcmp(legado,armazenada)==0;
    }
    p=armazenada+strlen(prefixo);
    fim=strchr(p,'$');
    if(!fim || (size_t)(fim-p)>=sizeof(tmp))return 0;
    memcpy(tmp,p,(size_t)(fim-p));tmp[fim-p]='\0';
    iteracoes=strtol(tmp,&endptr,10);
    if(*endptr || iteracoes<100000 || iteracoes>2000000)return 0;
    p=fim+1;
    if(strlen(p)!=32+1+64 || p[32]!='$')return 0;
    if(!senhaDeshex(p,16,sal) || !senhaDeshex(p+33,32,esperado))return 0;
    senhaPbkdf2(digitada,sal,sizeof(sal),iteracoes,calc);
    ok=senhaIguais(calc,esperado,sizeof(calc));
    memset(calc,0,sizeof(calc));
    return ok;
}

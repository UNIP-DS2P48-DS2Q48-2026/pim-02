/* Hash de senhas PBKDF2-HMAC-SHA256, sal aleatorio por usuario.
   Novos cadastros usam formato pbkdf2_sha256$iteracoes$salHex$hashHex.
   Usuarios antigos (hash decimal legado) continuam entrando e sao atualizados no login.
   Necessita OpenSSL: gcc ... -lcrypto. */
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>

#define SENHA_ITERACOES 600000

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
    if(!senha || !*senha || RAND_bytes(sal,sizeof(sal))!=1)return;
    if(PKCS5_PBKDF2_HMAC(senha,(int)strlen(senha),sal,sizeof(sal),
        SENHA_ITERACOES,EVP_sha256(),sizeof(hash),hash)!=1)return;
    senhaHex(sal,sizeof(sal),salHex);senhaHex(hash,sizeof(hash),hashHex);
    snprintf(destino,TAM_HASH,"pbkdf2_sha256$%d$%s$%s",
             SENHA_ITERACOES,salHex,hashHex);
    OPENSSL_cleanse(hash,sizeof(hash));
}
int verificarSenha(const char *digitada, const char *armazenada)
{
    const char *prefixo="pbkdf2_sha256$";
    if(!digitada || !armazenada)return 0;
    if(strncmp(armazenada,prefixo,strlen(prefixo))!=0){
        char legado[17];
        snprintf(legado,sizeof(legado),"%u",senhaLegada(digitada));
        return strcmp(legado,armazenada)==0;
    }
    const char *p=armazenada+strlen(prefixo),*fim;
    char tmp[12];char *endptr;
    long iteracoes;
    unsigned char sal[16],calc[32],esperado[32];
    fim=strchr(p,'$');
    if(!fim || (size_t)(fim-p)>=sizeof(tmp))return 0;
    memcpy(tmp,p,(size_t)(fim-p));tmp[fim-p]='\0';
    iteracoes=strtol(tmp,&endptr,10);
    if(*endptr || iteracoes<100000 || iteracoes>2000000)return 0;
    p=fim+1;
    if(strlen(p)!=32+1+64 || p[32]!='$')return 0;
    if(!senhaDeshex(p,16,sal) || !senhaDeshex(p+33,32,esperado))return 0;
    if(PKCS5_PBKDF2_HMAC(digitada,(int)strlen(digitada),sal,sizeof(sal),
       (int)iteracoes,EVP_sha256(),sizeof(calc),calc)!=1)return 0;
    int ok=CRYPTO_memcmp(calc,esperado,sizeof(calc))==0;
    OPENSSL_cleanse(calc,sizeof(calc));
    return ok;
}

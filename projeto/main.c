#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USAR_SQLSERVER 0

#if USAR_SQLSERVER
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#endif

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

/* Arquivos .txt: um por tabela, campos separados por ';' na ordem do schema_sqlserver.sql */
#define PASTA_DADOS "dados"
#define ARQ_USUARIO "dados/usuario.txt"
#define ARQ_EQUIPAMENTO "dados/equipamento.txt"
#define ARQ_OS "dados/os.txt"
#define ARQ_TECNICO_OS "dados/tecnico_os.txt"

#define MAX_USUARIOS 200
#define MAX_EQUIPAMENTOS 300
#define MAX_OS 500
#define MAX_TECNICO_OS 1000

/* Tamanhos de texto comuns */
#define TAM_DATA 11      /* AAAA-MM-DD */
#define TAM_DATAHORA 20  /* AAAA-MM-DD HH:MM:SS */
#define TAM_HASH 17
#define TAM_LINHA 1200

/* Tabela usuario: todos os perfis (admin, recepcionista, tecnico, cliente) */
typedef struct
{
    int id_usuario;
    char nome[60];
    char email[60];
    char telefone[20];
    char senha[TAM_HASH]; /* hash, nunca o texto da senha (RNF10) */
    char perfil[15];
    char cpf[15];           /* só cliente */
    char especialidade[40]; /* só tecnico */
    char status[8];         /* ativo | inativo */
    char criado_em[TAM_DATAHORA];
    char atualizado_em[TAM_DATAHORA];
} Usuario;

/* Tabela equipamento: hardware ou software de um cliente */
typedef struct
{
    int id_equipamento;
    int id_cliente;
    char categoria[10]; /* hardware | software */
    char tipo[40];
    char marca[40];
    char modelo_numero_serie[60]; /* ou chave de licença */
    char status[8];
    char criado_em[TAM_DATAHORA];
    char atualizado_em[TAM_DATAHORA];
} Equipamento;

/* Tabela os: ordem de serviço. Valores em dinheiro < 0 significam "ainda não informado" (NULL no banco) */
typedef struct
{
    int id_os;
    int id_equipamento;
    int id_atendente;
    char data_abertura[TAM_DATA];
    char problema_relatado[200];
    char diagnostico[200];
    double orcamento;
    char status[15]; /* aberta | em_andamento | finalizada | cancelada */
    char resultado_resposta_cliente[10]; /* aprovado | recusado */
    char data_entrega[TAM_DATA];
    char resultado_teste[10]; /* aprovado | reprovado */
    double valor_pago;
    char feedback_cliente[200];
    char satisfeito[4]; /* sim | nao */
    char criado_em[TAM_DATAHORA];
    char atualizado_em[TAM_DATAHORA];
} OrdemServico;

/* Tabela tecnico_os: técnicos atribuídos a cada OS */
typedef struct
{
    int id_tecnico_os;
    int id_tecnico;
    int id_os;
    char data_atribuicao[TAM_DATA];
    char observacoes_tecnicas[300];
    char criado_em[TAM_DATAHORA];
    char atualizado_em[TAM_DATAHORA];
} TecnicoOS;

Usuario usuarios[MAX_USUARIOS];
int totalUsuarios = 0;
Equipamento equipamentos[MAX_EQUIPAMENTOS];
int totalEquipamentos = 0;
OrdemServico ordens[MAX_OS];
int totalOrdens = 0;
TecnicoOS tecnicosOS[MAX_TECNICO_OS];
int totalTecnicosOS = 0;

/* Posição do usuário logado no vetor usuarios (-1 = ninguém logado) */
int usuarioLogado = -1;

void limparBuffer();
void imprimirCabecalho(const char *subtitulo);
void lerTexto(const char *rotulo, char *destino, int tamanho);
int lerInteiro(const char *rotulo);
int confirmar(const char *pergunta);
void pausar(void);
void gerarHash(const char *senha, char *destino);
void dataHoje(char *destino);
void dataHoraAgora(char *destino);
void lerCampo(char **cursor, char *destino, int tamanho);
int lerCampoInt(char **cursor);
double lerCampoValor(char **cursor);
void gravarValor(FILE *arquivo, double valor);

#if USAR_SQLSERVER
#include "banco.c"
#endif

#include "geral.c"
#include "admin.c"
#include "recepcionista.c"
#include "tecnico.c"
#include "cliente.c"
#include "equipamento.c"
#include "ordem_servico.c"
#include "manutencao.c"
#include "orcamento.c"
#include "relatorio.c"

/* Limpa o buffer do teclado para evitar loops infinitos em inputs inválidos */
void limparBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* Lê uma linha inteira (aceita espaços) e tira o ENTER do final */
void lerTexto(const char *rotulo, char *destino, int tamanho)
{
    printf("%s", rotulo);
    if (fgets(destino, tamanho, stdin) == NULL)
    {
        destino[0] = '\0';
        return;
    }
    if (strchr(destino, '\n') == NULL)
    {
        limparBuffer(); /* texto maior que o campo: descarta o resto da linha */
    }
    destino[strcspn(destino, "\n")] = '\0';

    /* ';' separa os campos nos arquivos .txt, então não pode entrar no texto */
    for (int i = 0; destino[i] != '\0'; i++)
    {
        if (destino[i] == ';')
        {
            destino[i] = ',';
        }
    }
}

/* Lê um número inteiro; devolve -1 se o que foi digitado não for número */
int lerInteiro(const char *rotulo)
{
    char texto[20];
    char *fim;
    long valor;

    lerTexto(rotulo, texto, sizeof(texto));
    valor = strtol(texto, &fim, 10);
    if (fim == texto || *fim != '\0')
    {
        return -1;
    }
    return (int)valor;
}

/* Pergunta S/N; devolve 1 para S e 0 para qualquer outra resposta */
int confirmar(const char *pergunta)
{
    char resposta[5];

    printf("%s (S/N): ", pergunta);
    lerTexto("", resposta, sizeof(resposta));
    return resposta[0] == 'S' || resposta[0] == 's';
}

void pausar(void)
{
    char tecla[5];
    lerTexto("\nPressione ENTER para continuar...", tecla, sizeof(tecla));
}

/* Hash da senha (FNV-1a, 64 bits) em 16 caracteres hexadecimais - RNF10. destino precisa de 17 posições */
void gerarHash(const char *senha, char *destino)
{
    unsigned long long hash = 14695981039346656037ULL;
    int i;

    for (i = 0; senha[i] != '\0'; i++)
    {
        hash ^= (unsigned char)senha[i];
        hash *= 1099511628211ULL;
    }
    /* duas metades de 32 bits: o MinGW antigo não aceita %llx */
    sprintf(destino, "%08lx%08lx", (unsigned long)(hash >> 32), (unsigned long)(hash & 0xFFFFFFFFUL));
}

/* Data de hoje no formato do banco (AAAA-MM-DD). destino precisa de 11 posições */
void dataHoje(char *destino)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(destino, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}

/* Data e hora atuais para criado_em/atualizado_em. destino precisa de 20 posições */
void dataHoraAgora(char *destino)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(destino, "%04d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/* Copia o próximo campo de uma linha do .txt (até o ';') e avança o cursor */
void lerCampo(char **cursor, char *destino, int tamanho)
{
    int i = 0;

    while (**cursor != '\0' && **cursor != ';' && **cursor != '\n' && **cursor != '\r')
    {
        if (i < tamanho - 1)
        {
            destino[i++] = **cursor;
        }
        (*cursor)++;
    }
    destino[i] = '\0';
    if (**cursor == ';')
    {
        (*cursor)++;
    }
}

int lerCampoInt(char **cursor)
{
    char texto[20];
    lerCampo(cursor, texto, sizeof(texto));
    return atoi(texto);
}

/* Campo de dinheiro: vazio vira -1 (ainda não informado) */
double lerCampoValor(char **cursor)
{
    char texto[30];
    lerCampo(cursor, texto, sizeof(texto));
    return texto[0] == '\0' ? -1 : atof(texto);
}

/* Grava um campo de dinheiro; valor < 0 fica vazio. Não escreve o ';' */
void gravarValor(FILE *arquivo, double valor)
{
    if (valor >= 0)
    {
        fprintf(arquivo, "%.2f", valor);
    }
}

void imprimirCabecalho(const char *subtitulo)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    printf("================================================================================\n");
    printf("BYTETECH  Sistema de Gestao de Manutencao                             %02d/%02d/%04d\n",
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    printf("================================================================================\n");

    if (subtitulo != NULL && strlen(subtitulo) > 0)
    {
        printf("%s\n", subtitulo);
        printf("--------------------------------------------------------------------------------\n");
    }
}

void menuPerfis(void)
{
    int opcao = -1;

    while (opcao != 0)
    {
        Usuario *u = &usuarios[usuarioLogado];

        imprimirCabecalho(nomePerfil(u->perfil));

        printf("\n  Bem-vindo(a), %s!\n\n", u->nome);
        printf("  1 - Meus Dados\n");
        printf("  2 - Alterar Senha\n");
        printf("  0 - Sair (Fazer Logout)\n");

        /* Espaçamento para o prompt */
        for (int i = 0; i < 9; i++)
            printf("\n");

        opcao = lerInteiro("Opcao: ");

        if (opcao == 1)
        {
            consultarMeusDados();
        }
        else if (opcao == 2)
        {
            alterarSenha();
        }
        else if (opcao == 0)
        {
            fazerLogout();
        }
        else
        {
            printf("\n[!] Opcao invalida!\n");
            pausar();
        }
    }
}

void menuPrincipal()
{
    int opcao = -1;

    while (opcao != 0)
    {
        imprimirCabecalho("");

        printf("\n  Controle de clientes, equipamentos, tecnicos e ordens de servico\n");
        printf("  Hardware e software para pessoas fisicas\n\n");

        printf("  1 - Entrar\n");
        printf("  0 - Encerrar o programa\n");

        /* Espaçamento vertical para o input da opção */
        for (int i = 0; i < 11; i++)
        {
            printf("\n");
        }

        opcao = lerInteiro("Opcao: ");

        if (opcao == 1)
        {
            usuarioLogado = realizarLogin();

            if (usuarioLogado >= 0)
            {
                /* Sucesso: Encaminha para o submenu específico */
                menuPerfis();
            }
        }
        else if (opcao == 0)
        {
            printf("\nA encerrar o sistema BYTETECH. Ate logo!\n");
        }
        else
        {
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    }
}

int main()
{
    /* Cria a pasta dos arquivos .txt se ela ainda não existir */
#ifdef _WIN32
    _mkdir(PASTA_DADOS);
#else
    mkdir(PASTA_DADOS, 0777);
#endif

    carregarUsuarios();
    carregarEquipamentos();
    carregarOrdens();
    carregarTecnicosOS();

    menuPrincipal();
    return 0;
}
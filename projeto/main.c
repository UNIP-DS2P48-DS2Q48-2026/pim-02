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

#define NUM_USUARIOS 4

/* Estrutura base de dados simulada para os utilizadores */
typedef struct
{
    int id_usuario;
    char nome[50];
    char email[50];
    char senha[50];
    char perfil[20];
} Usuario;

/* Dados fornecidos na imagem de requisitos */
Usuario usuarios[NUM_USUARIOS] = {
    {1, "Ana", "ana@bytetech.com", "byte123", "Admin"},
    {2, "Carla", "carla@bytetech.com", "byte123", "Recepcionista"},
    {3, "Rafael", "rafael@bytetech.com", "byte123", "Técnico"},
    {4, "João Pereira", "joao.pereira@email.com", "byte123", "Cliente"}};

void limparEcra();
void limparBuffer();
void imprimirCabecalho(const char *subtitulo);

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

void menuPerfis(Usuario u)
{
    int opcao = -1;

    while (opcao != 0)
    {
        imprimirCabecalho(u.perfil);

        printf("\n  Bem-vindo(a), %s!\n", u.nome);
        printf("  Este e o espaco restrito para as funcoes de: %s\n\n", u.perfil);
        printf("  0 - Sair (Fazer Logout)\n");

        /* Espaçamento para o prompt */
        for (int i = 0; i < 10; i++)
            printf("\n");

        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1)
        {
            limparBuffer();
            opcao = -1;
        }

        if (opcao == 0)
        {
            fazerLogout();
        }
        else
        {
            printf("\nOpcao invalida!\n");
            limparBuffer();
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

        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1)
        {
            limparBuffer();
            opcao = -1;
            continue;
        }

        if (opcao == 1)
        {
            int indiceAtivo = realizarLogin();

            if (indiceAtivo >= 0)
            {
                /* Sucesso: Encaminha para o submenu específico */
                menuPerfis(usuarios[indiceAtivo]);
            }
            else
            {
                /* Falha na autenticação */
                printf("\n[!] E-mail ou senha incorretos. Pressione ENTER para voltar.");
                getchar();
            }
        }
        else if (opcao == 0)
        {
            printf("\nA encerrar o sistema BYTETECH. Ate logo!\n");
        }
        else
        {
            printf("\n[!] Opcao invalida! Pressione ENTER para voltar.");
            limparBuffer();
            getchar();
        }
    }
}

int main()
{

    menuPrincipal();
    return 0;
}
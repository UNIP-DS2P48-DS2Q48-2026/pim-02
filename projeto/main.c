#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NUM_USUARIOS 4

/* Estrutura base de dados simulada para os utilizadores */
typedef struct {
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
    {4, "João Pereira", "joao.pereira@email.com", "byte123", "Cliente"}
};

/* Função multiplataforma para limpar o ecrã */
void limparEcra() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/* Limpa o buffer do teclado para evitar loops infinitos em inputs inválidos */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Função genérica para imprimir o cabeçalho idêntico às imagens */
void imprimirCabecalho(const char* subtitulo) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    limparEcra();
    printf("================================================================================\n");
    
    /* 41 caracteres ocupados pelo título, restam 29 espaços para empurrar a data para a direita */
    printf("BYTETECH  Sistema de Gestao de Manutencao                             %02d/%02d/%04d\n", 
           tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
           
    printf("================================================================================\n");
    
    /* Subtítulo dinâmico (Ex: "Entrar") */
    if (subtitulo != NULL && strlen(subtitulo) > 0) {
        printf("%s\n", subtitulo);
        printf("--------------------------------------------------------------------------------\n");
    }
}

/* Função que gere o ecrã de Autenticação */
int realizarLogin() {
    char email[50];
    char senha[50];
    int i;
    
    /* --- PASSO 1: ECRÃ DO E-MAIL --- */
    imprimirCabecalho("Entrar");
    
    /* Espaçamento para colocar o input na zona inferior, como na imagem */
    for(i = 0; i < 15; i++) {
        printf("\n");
    }
    
    printf("E-mail: ");
    scanf("%49s", email);
    limparBuffer();
    
    /* --- PASSO 2: ECRÃ DA SENHA --- */
    imprimirCabecalho("Entrar");
    
    /* O e-mail fica agora fixo no topo, logo abaixo do cabeçalho */
    printf("\nE-mail: %s\n", email);
    
    /* Espaçamento para o campo de senha na zona inferior */
    for(i = 0; i < 13; i++) {
        printf("\n");
    }
    
    printf("Senha: ");
    scanf("%49s", senha);
    limparBuffer();
    
    /* --- VALIDAÇÃO --- */
    for (i = 0; i < NUM_USUARIOS; i++) {
        if (strcmp(email, usuarios[i].email) == 0 && strcmp(senha, usuarios[i].senha) == 0) {
            return i; /* Retorna o índice do utilizador autenticado */
        }
    }
    return -1; /* Retorna -1 se as credenciais falharem */
}

/* Stub: Simula a entrada nos menus específicos sem os aprofundar */
void menuPerfis(Usuario u) {
    int opcao = -1;
    
    while (opcao != 0) {
        imprimirCabecalho(u.perfil);
        
        printf("\n  Bem-vindo(a), %s!\n", u.nome);
        printf("  Este e o espaco restrito para as funcoes de: %s\n\n", u.perfil);
        printf("  0 - Sair (Fazer Logout)\n");
        
        /* Espaçamento para o prompt */
        for(int i = 0; i < 10; i++) printf("\n");
        
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        }
        
        if (opcao == 0) {
            printf("\nA terminar a sessao...\n");
        } else {
            printf("\nOpcao invalida!\n");
            limparBuffer();
        }
    }
}

/* Ponto de partida da aplicação (Ecrã Principal) */
void menuPrincipal() {
    int opcao = -1;
    
    while (opcao != 0) {
        imprimirCabecalho("");
        
        printf("\n  Controle de clientes, equipamentos, tecnicos e ordens de servico\n");
        printf("  Hardware e software para pessoas fisicas\n\n");
        
        printf("  1 - Entrar\n");
        printf("  0 - Encerrar o programa\n");
        
        /* Espaçamento vertical para o input da opção */
        for(int i = 0; i < 11; i++) {
            printf("\n");
        }
        
        printf("Opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
            continue;
        }
        
        if (opcao == 1) {
            int indiceAtivo = realizarLogin();
            
            if (indiceAtivo >= 0) {
                /* Sucesso: Encaminha para o submenu específico */
                menuPerfis(usuarios[indiceAtivo]);
            } else {
                /* Falha na autenticação */
                printf("\n[!] E-mail ou senha incorretos. Prima ENTER para voltar.");
                getchar();
            }
        } else if (opcao == 0) {
            printf("\nA encerrar o sistema BYTETECH. Ate logo!\n");
        } else {
            printf("\n[!] Opcao invalida! Prima ENTER para voltar.");
            limparBuffer();
            getchar();
        }
    }
}

int main() {
    /* Chamada inicial para a estrutura do programa */
    menuPrincipal();
    return 0;
}
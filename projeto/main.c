#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Conexão com o SQL Server (banco bytetech). Compilar com: gcc main.c -o output/main.exe -lodbc32 */
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#define MAX_USUARIOS 200
#define MAX_EQUIPAMENTOS 300
#define MAX_OS 500
#define MAX_TECNICO_OS 1000

/* Tamanhos de texto comuns */
#define TAM_DATA 11      /* AAAA-MM-DD */
#define TAM_DATAHORA 20  /* AAAA-MM-DD HH:MM:SS */
#define TAM_HASH 17

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

/* Tabela os: ordem de serviço. Dinheiro -1 significa "ainda não informado" (NULL no banco) */
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

/* Vetores globais com os dados carregados */
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

/* Protótipos: funções do main.c usadas pelos outros arquivos */
void imprimirCabecalho(char titulo[]);
void lerTexto(char rotulo[], char destino[], int tamanho);
int lerInteiro(char rotulo[]);
double lerValor(char rotulo[]);
int confirmar(char pergunta[]);
void pausar();
void gerarHash(char senha[], char destino[]);
void dataHoje(char destino[]);
void dataHoraAgora(char destino[]);

#include "banco.c"
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

/* Lê uma linha inteira (aceita espaços) e tira o ENTER do final */
void lerTexto(char rotulo[], char destino[], int tamanho)
{
    char linha[500];

    printf("%s", rotulo);
    if (fgets(linha, sizeof(linha), stdin) == NULL)
    {
        /* Acabou a entrada (Ctrl+Z): encerra, senão o menu repetiria para sempre */
        printf("\n\n[!] Fim da entrada. Encerrando o programa.\n");
        exit(0);
    }
    linha[strcspn(linha, "\n")] = '\0'; /* tira o ENTER */
    linha[tamanho - 1] = '\0';          /* corta o que não cabe no destino */
    strcpy(destino, linha);
}

/* Lê um número inteiro; devolve -1 se não for número */
int lerInteiro(char rotulo[])
{
    char texto[20];
    int valor;

    lerTexto(rotulo, texto, sizeof(texto));
    if (sscanf(texto, "%d", &valor) != 1)
        return -1;
    return valor;
}

/* Lê um valor em reais (aceita vírgula ou ponto); devolve -1 se ficou vazio ou não é valor */
double lerValor(char rotulo[])
{
    char texto[30];
    double valor;
    int i;

    lerTexto(rotulo, texto, sizeof(texto));
    for (i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] == ',')
            texto[i] = '.';
    }
    if (sscanf(texto, "%lf", &valor) != 1 || valor < 0)
        return -1;
    return valor;
}

/* Pergunta S/N; devolve 1 para S e 0 para o resto */
int confirmar(char pergunta[])
{
    char resposta[5];

    printf("%s (S/N): ", pergunta);
    lerTexto("", resposta, sizeof(resposta));
    if (resposta[0] == 'S' || resposta[0] == 's')
        return 1;
    return 0;
}

void pausar()
{
    char tecla[5];
    lerTexto("\nPressione ENTER para continuar...", tecla, sizeof(tecla));
}

/* Transforma a senha num número (hash) para não gravar a senha (RNF10) */
void gerarHash(char senha[], char destino[])
{
    unsigned long hash = 5381;
    int i;

    for (i = 0; senha[i] != '\0'; i++)
    {
        hash = hash * 33 + senha[i];
    }
    sprintf(destino, "%lu", hash);
}

/* Data de hoje: AAAA-MM-DD */
void dataHoje(char destino[])
{
    time_t agora = time(NULL);
    strftime(destino, TAM_DATA, "%Y-%m-%d", localtime(&agora));
}

/* Data e hora de agora: AAAA-MM-DD HH:MM:SS */
void dataHoraAgora(char destino[])
{
    time_t agora = time(NULL);
    strftime(destino, TAM_DATAHORA, "%Y-%m-%d %H:%M:%S", localtime(&agora));
}

void imprimirCabecalho(char titulo[])
{
    printf("\n========== BYTETECH - Sistema de Gestao de Manutencao ==========\n");
    printf("%s\n", titulo);
}

/* ---------- Submenus (só Admin e Recepcionista chegam aqui) ---------- */

void menuAdministradores()
{
    int opcao;

    do
    {
        imprimirCabecalho("Administradores");
        printf("1 - Cadastrar\n");
        printf("2 - Consultar\n");
        printf("3 - Editar\n");
        printf("4 - Inativar\n");
        printf("5 - Reativar\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: cadastrarAdmin(); break;
        case 2: consultarAdmin(); break;
        case 3: editarAdmin(); break;
        case 4: inativarAdmin(); break;
        case 5: reativarAdmin(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuRecepcionistas()
{
    int opcao;

    do
    {
        imprimirCabecalho("Recepcionistas");
        printf("1 - Cadastrar\n");
        printf("2 - Consultar\n");
        printf("3 - Editar\n");
        printf("4 - Inativar\n");
        printf("5 - Reativar\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: cadastrarRecepcionista(); break;
        case 2: consultarRecepcionista(); break;
        case 3: editarRecepcionista(); break;
        case 4: inativarRecepcionista(); break;
        case 5: reativarRecepcionista(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuTecnicos()
{
    int opcao;

    do
    {
        imprimirCabecalho("Tecnicos");
        printf("1 - Cadastrar\n");
        printf("2 - Consultar\n");
        printf("3 - Editar\n");
        printf("4 - Inativar\n");
        printf("5 - Reativar\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: cadastrarTecnico(); break;
        case 2: consultarTecnico(); break;
        case 3: editarTecnico(); break;
        case 4: inativarTecnico(); break;
        case 5: reativarTecnico(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuClientes()
{
    int opcao;

    do
    {
        imprimirCabecalho("Clientes");
        printf("1 - Cadastrar\n");
        printf("2 - Consultar\n");
        printf("3 - Editar\n");
        printf("4 - Inativar\n");
        printf("5 - Reativar\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: cadastrarCliente(); break;
        case 2: consultarCliente(); break;
        case 3: editarCliente(); break;
        case 4: inativarCliente(); break;
        case 5: reativarCliente(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuEquipamentos()
{
    int opcao;

    do
    {
        imprimirCabecalho("Equipamentos");
        printf("1 - Cadastrar\n");
        printf("2 - Consultar\n");
        printf("3 - Editar\n");
        printf("4 - Inativar\n");
        printf("5 - Reativar\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: cadastrarEquipamento(); break;
        case 2: consultarEquipamento(); break;
        case 3: editarEquipamento(); break;
        case 4: inativarEquipamento(); break;
        case 5: reativarEquipamento(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuOrdensServico()
{
    int opcao;

    do
    {
        imprimirCabecalho("Ordens de Servico");
        printf("1 - Criar\n");
        printf("2 - Consultar\n");
        printf("3 - Editar\n");
        printf("4 - Cancelar\n");
        printf("5 - Atribuir Tecnico\n");
        printf("6 - Registrar Entrega\n");
        printf("7 - Registrar Feedback\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: criarOrdemServico(); break;
        case 2: consultarOS(); break;
        case 3: editarOS(); break;
        case 4: cancelarOS(); break;
        case 5: atribuirTecnico(); break;
        case 6: registrarEntrega(); break;
        case 7: registrarFeedback(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuOrcamentos()
{
    int opcao;

    do
    {
        imprimirCabecalho("Orcamentos");
        printf("1 - Registrar\n");
        printf("2 - Editar\n");
        printf("3 - Consultar\n");
        printf("4 - Aprovar\n");
        printf("5 - Recusar\n");
        printf("6 - Arquivar\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: registrarOrcamento(); break;
        case 2: editarOrcamento(); break;
        case 3: consultarOrcamento(); break;
        case 4: aprovarOrcamento(); break;
        case 5: recusarOrcamento(); break;
        case 6: arquivarOrcamento(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

/* Manutenção do Admin: as funções do técnico (RN11) e as OS prontas para entrega */
void menuManutencao()
{
    int opcao;

    do
    {
        imprimirCabecalho("Manutencao");
        printf("1 - Registrar Observacao Tecnica\n");
        printf("2 - Registrar Diagnostico\n");
        printf("3 - Executar Manutencao\n");
        printf("4 - Registrar Teste\n");
        printf("5 - OS Prontas para Entrega\n");
        printf("0 - Voltar\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: registrarObservacao(); break;
        case 2: registrarDiagnostico(); break;
        case 3: executarManutencao(); break;
        case 4: registrarTeste(); break;
        case 5: listarProntasParaEntrega(); break;
        case 0: break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

/* ---------- Um menu principal por perfil (Matriz de Permissões, RN10) ---------- */

void menuDoAdmin()
{
    int opcao;

    do
    {
        imprimirCabecalho("Menu do Administrador");
        printf("Bem-vindo(a), %s!\n", usuarios[usuarioLogado].nome);
        printf("1 - Administradores\n");
        printf("2 - Recepcionistas\n");
        printf("3 - Tecnicos\n");
        printf("4 - Clientes\n");
        printf("5 - Equipamentos\n");
        printf("6 - Ordens de Servico\n");
        printf("7 - Manutencao\n");
        printf("8 - Orcamentos\n");
        printf("9 - Historico de Equipamento\n");
        printf("10 - Relatorios\n");
        printf("11 - Meus Dados\n");
        printf("12 - Alterar Senha\n");
        printf("0 - Sair (Fazer Logout)\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: menuAdministradores(); break;
        case 2: menuRecepcionistas(); break;
        case 3: menuTecnicos(); break;
        case 4: menuClientes(); break;
        case 5: menuEquipamentos(); break;
        case 6: menuOrdensServico(); break;
        case 7: menuManutencao(); break;
        case 8: menuOrcamentos(); break;
        case 9: consultarHistorico(); break;
        case 10: gerarRelatorio(); break;
        case 11: consultarMeusDados(); break;
        case 12: alterarSenha(); break;
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuDaRecepcionista()
{
    int opcao;

    do
    {
        imprimirCabecalho("Menu da Recepcionista");
        printf("Bem-vindo(a), %s!\n", usuarios[usuarioLogado].nome);
        printf("1 - Clientes\n");
        printf("2 - Equipamentos\n");
        printf("3 - Ordens de Servico\n");
        printf("4 - OS Prontas para Entrega\n");
        printf("5 - Orcamentos\n");
        printf("6 - Historico de Equipamento\n");
        printf("7 - Meus Dados\n");
        printf("8 - Alterar Senha\n");
        printf("0 - Sair (Fazer Logout)\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: menuClientes(); break;
        case 2: menuEquipamentos(); break;
        case 3: menuOrdensServico(); break;
        case 4: listarProntasParaEntrega(); break;
        case 5: menuOrcamentos(); break;
        case 6: consultarHistorico(); break;
        case 7: consultarMeusDados(); break;
        case 8: alterarSenha(); break;
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

/* O técnico só consulta clientes, equipamentos e OS; o resto é a manutenção */
void menuDoTecnico()
{
    int opcao;

    do
    {
        imprimirCabecalho("Menu do Tecnico");
        printf("Bem-vindo(a), %s!\n", usuarios[usuarioLogado].nome);
        printf("1 - Consultar Clientes\n");
        printf("2 - Consultar Equipamentos\n");
        printf("3 - Consultar Ordens de Servico\n");
        printf("4 - Registrar Observacao Tecnica\n");
        printf("5 - Registrar Diagnostico\n");
        printf("6 - Executar Manutencao\n");
        printf("7 - Registrar Teste\n");
        printf("8 - Historico de Equipamento\n");
        printf("9 - Meus Dados\n");
        printf("10 - Alterar Senha\n");
        printf("0 - Sair (Fazer Logout)\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: consultarCliente(); break;
        case 2: consultarEquipamento(); break;
        case 3: consultarOS(); break;
        case 4: registrarObservacao(); break;
        case 5: registrarDiagnostico(); break;
        case 6: executarManutencao(); break;
        case 7: registrarTeste(); break;
        case 8: consultarHistorico(); break;
        case 9: consultarMeusDados(); break;
        case 10: alterarSenha(); break;
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

/* O cliente vê só o que é dele: equipamentos, OS e orçamentos */
void menuDoCliente()
{
    int opcao;

    do
    {
        imprimirCabecalho("Menu do Cliente");
        printf("Bem-vindo(a), %s!\n", usuarios[usuarioLogado].nome);
        printf("1 - Meus Equipamentos\n");
        printf("2 - Minhas Ordens de Servico\n");
        printf("3 - Registrar Feedback\n");
        printf("4 - Consultar Orcamento\n");
        printf("5 - Aprovar Orcamento\n");
        printf("6 - Recusar Orcamento\n");
        printf("7 - Historico de Equipamento\n");
        printf("8 - Meus Dados\n");
        printf("9 - Alterar Senha\n");
        printf("0 - Sair (Fazer Logout)\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1: consultarEquipamento(); break;
        case 2: consultarOS(); break;
        case 3: registrarFeedback(); break;
        case 4: consultarOrcamento(); break;
        case 5: aprovarOrcamento(); break;
        case 6: recusarOrcamento(); break;
        case 7: consultarHistorico(); break;
        case 8: consultarMeusDados(); break;
        case 9: alterarSenha(); break;
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

void menuPrincipal()
{
    char perfil[15];
    int opcao;

    do
    {
        imprimirCabecalho("Tela inicial");
        printf("1 - Entrar\n");
        printf("0 - Encerrar o programa\n");
        opcao = lerInteiro("Opcao: ");

        switch (opcao)
        {
        case 1:
            usuarioLogado = realizarLogin();
            if (usuarioLogado < 0)
                break; /* login falhou: volta para a tela inicial */

            /* guarda o perfil antes, porque o logout apaga o usuarioLogado */
            strcpy(perfil, usuarios[usuarioLogado].perfil);
            if (strcmp(perfil, "admin") == 0)
                menuDoAdmin();
            if (strcmp(perfil, "recepcionista") == 0)
                menuDaRecepcionista();
            if (strcmp(perfil, "tecnico") == 0)
                menuDoTecnico();
            if (strcmp(perfil, "cliente") == 0)
                menuDoCliente();
            break;
        case 0:
            printf("\nEncerrando o sistema BYTETECH. Ate logo!\n");
            break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (opcao != 0);
}

int main()
{
    if (!bdConectar())
    {
        printf("[!] Nao foi possivel conectar ao SQL Server (banco bytetech).\n");
        printf("    Confira se o SQL Server esta rodando e se o banco foi criado:\n");
        printf("    sqlcmd -S localhost -E -i docs\\Modelagem_DB\\schema_sqlserver.sql\n");
        return 1;
    }

    carregarUsuarios();
    carregarEquipamentos();
    carregarOrdens();
    carregarTecnicosOS();

    /* Primeira vez (sem nenhum usuário): cria os usuários de teste do grupo, com a senha byte123 */
    if (totalUsuarios == 0)
    {
        adicionarUsuarioInicial("Ana", "ana@bytetech.com", "", "admin", "", "");
        adicionarUsuarioInicial("Carla", "carla@bytetech.com", "", "recepcionista", "", "");
        adicionarUsuarioInicial("Rafael", "rafael@bytetech.com", "", "tecnico", "", "Hardware");
        adicionarUsuarioInicial("Joao Pereira", "joao.pereira@email.com", "(11) 90000-0000", "cliente", "111.111.111-11", "");
        salvarUsuarios();
    }

    menuPrincipal();

    bdDesconectar();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/* Os outros arquivos entram aqui, nesta ordem */
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

/* Mostra a pergunta e lê a resposta até o ENTER */
void lerTexto(char rotulo[], char destino[], int tamanho)
{
    char linha[500];

    printf("%s", rotulo);
    if (fgets(linha, sizeof(linha), stdin) == NULL)
    {
        printf("\n\n[!] Fim da entrada. Encerrando o programa.\n");
        exit(0);
    }
    linha[strcspn(linha, "\n")] = '\0'; /* tira o ENTER */
    linha[tamanho - 1] = '\0';          /* corta no tamanho */
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

/* Lê um valor em reais (ex: 95.50); devolve -1 se vazio ou inválido */
double lerValor(char rotulo[])
{
    char texto[30];
    double valor;

    lerTexto(rotulo, texto, sizeof(texto));
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

/* Espera o ENTER para dar tempo de ler a mensagem */
void pausar()
{
    char tecla[5];
    lerTexto("\nPressione ENTER para continuar...", tecla, sizeof(tecla));
}

/* Transforma a senha num número (hash) para não gravar a senha no banco (RNF10) */
void gerarHash(char senha[], char destino[])
{
    unsigned long hash = 5381;
    int posicaoLetra;

    for (posicaoLetra = 0; senha[posicaoLetra] != '\0'; posicaoLetra++)
    {
        hash = hash * 33 + senha[posicaoLetra];
    }
    sprintf(destino, "%lu", hash);
}

/* Data de hoje (AAAA-MM-DD), pegando a data do SQL Server */
void dataHoje(char destino[])
{
    strcpy(destino, "");
    /* bdExecutar está em banco.c, linha 61 */
    /* bdProximaLinha está em banco.c, linha 80 */
    /* bdLerTexto está em banco.c, linha 88 */
    if (bdExecutar("SELECT CONVERT(VARCHAR(10), GETDATE(), 120)") && bdProximaLinha())
        bdLerTexto(1, destino, TAM_DATA);
}

/* Data e hora de agora (AAAA-MM-DD HH:MM:SS), pegando do SQL Server */
void dataHoraAgora(char destino[])
{
    strcpy(destino, "");
    /* bdExecutar está em banco.c, linha 61 */
    /* bdProximaLinha está em banco.c, linha 80 */
    /* bdLerTexto está em banco.c, linha 88 */
    if (bdExecutar("SELECT CONVERT(VARCHAR(19), GETDATE(), 120)") && bdProximaLinha())
        bdLerTexto(1, destino, TAM_DATAHORA);
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
        /* cadastrarAdmin está em admin.c, linha 2 */
        case 1: cadastrarAdmin(); break;
        /* consultarAdmin está em admin.c, linha 9 */
        case 2: consultarAdmin(); break;
        /* editarAdmin está em admin.c, linha 16 */
        case 3: editarAdmin(); break;
        /* inativarAdmin está em admin.c, linha 23 */
        case 4: inativarAdmin(); break;
        /* reativarAdmin está em admin.c, linha 30 */
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
        /* cadastrarRecepcionista está em recepcionista.c, linha 2 */
        case 1: cadastrarRecepcionista(); break;
        /* consultarRecepcionista está em recepcionista.c, linha 9 */
        case 2: consultarRecepcionista(); break;
        /* editarRecepcionista está em recepcionista.c, linha 16 */
        case 3: editarRecepcionista(); break;
        /* inativarRecepcionista está em recepcionista.c, linha 23 */
        case 4: inativarRecepcionista(); break;
        /* reativarRecepcionista está em recepcionista.c, linha 30 */
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
        /* cadastrarTecnico está em tecnico.c, linha 2 */
        case 1: cadastrarTecnico(); break;
        /* consultarTecnico está em tecnico.c, linha 9 */
        case 2: consultarTecnico(); break;
        /* editarTecnico está em tecnico.c, linha 16 */
        case 3: editarTecnico(); break;
        /* inativarTecnico está em tecnico.c, linha 23 */
        case 4: inativarTecnico(); break;
        /* reativarTecnico está em tecnico.c, linha 30 */
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
        /* cadastrarCliente está em cliente.c, linha 2 */
        case 1: cadastrarCliente(); break;
        /* consultarCliente está em cliente.c, linha 9 */
        case 2: consultarCliente(); break;
        /* editarCliente está em cliente.c, linha 16 */
        case 3: editarCliente(); break;
        /* inativarCliente está em cliente.c, linha 23 */
        case 4: inativarCliente(); break;
        /* reativarCliente está em cliente.c, linha 30 */
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
        /* cadastrarEquipamento está em equipamento.c, linha 235 */
        case 1: cadastrarEquipamento(); break;
        /* consultarEquipamento está em equipamento.c, linha 312 */
        case 2: consultarEquipamento(); break;
        /* editarEquipamento está em equipamento.c, linha 335 */
        case 3: editarEquipamento(); break;
        /* inativarEquipamento está em equipamento.c, linha 367 */
        case 4: inativarEquipamento(); break;
        /* reativarEquipamento está em equipamento.c, linha 405 */
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
        /* criarOrdemServico está em ordem_servico.c, linha 431 */
        case 1: criarOrdemServico(); break;
        /* consultarOS está em ordem_servico.c, linha 547 */
        case 2: consultarOS(); break;
        /* editarOS está em ordem_servico.c, linha 613 */
        case 3: editarOS(); break;
        /* cancelarOS está em ordem_servico.c, linha 639 */
        case 4: cancelarOS(); break;
        /* atribuirTecnico está em ordem_servico.c, linha 666 */
        case 5: atribuirTecnico(); break;
        /* registrarEntrega está em ordem_servico.c, linha 744 */
        case 6: registrarEntrega(); break;
        /* registrarFeedback está em ordem_servico.c, linha 801 */
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
        /* registrarOrcamento está em orcamento.c, linha 5 */
        case 1: registrarOrcamento(); break;
        /* editarOrcamento está em orcamento.c, linha 37 */
        case 2: editarOrcamento(); break;
        /* consultarOrcamento está em orcamento.c, linha 69 */
        case 3: consultarOrcamento(); break;
        /* aprovarOrcamento está em orcamento.c, linha 133 */
        case 4: aprovarOrcamento(); break;
        /* recusarOrcamento está em orcamento.c, linha 140 */
        case 5: recusarOrcamento(); break;
        /* arquivarOrcamento está em orcamento.c, linha 147 */
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
        /* registrarObservacao está em manutencao.c, linha 5 */
        case 1: registrarObservacao(); break;
        /* registrarDiagnostico está em manutencao.c, linha 68 */
        case 2: registrarDiagnostico(); break;
        /* executarManutencao está em manutencao.c, linha 147 */
        case 3: executarManutencao(); break;
        /* registrarTeste está em manutencao.c, linha 236 */
        case 4: registrarTeste(); break;
        /* listarProntasParaEntrega está em manutencao.c, linha 303 */
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
        /* menuAdministradores está em main.c, linha 222 */
        case 1: menuAdministradores(); break;
        /* menuRecepcionistas está em main.c, linha 257 */
        case 2: menuRecepcionistas(); break;
        /* menuTecnicos está em main.c, linha 292 */
        case 3: menuTecnicos(); break;
        /* menuClientes está em main.c, linha 327 */
        case 4: menuClientes(); break;
        /* menuEquipamentos está em main.c, linha 362 */
        case 5: menuEquipamentos(); break;
        /* menuOrdensServico está em main.c, linha 397 */
        case 6: menuOrdensServico(); break;
        /* menuManutencao está em main.c, linha 477 */
        case 7: menuManutencao(); break;
        /* menuOrcamentos está em main.c, linha 438 */
        case 8: menuOrcamentos(); break;
        /* consultarHistorico está em relatorio.c, linha 5 */
        case 9: consultarHistorico(); break;
        /* gerarRelatorio está em relatorio.c, linha 42 */
        case 10: gerarRelatorio(); break;
        /* consultarMeusDados está em geral.c, linha 621 */
        case 11: consultarMeusDados(); break;
        /* alterarSenha está em geral.c, linha 638 */
        case 12: alterarSenha(); break;
        /* fazerLogout está em geral.c, linha 611 */
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (usuarioLogado >= 0); /* sai só depois do logout */
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
        /* menuClientes está em main.c, linha 327 */
        case 1: menuClientes(); break;
        /* menuEquipamentos está em main.c, linha 362 */
        case 2: menuEquipamentos(); break;
        /* menuOrdensServico está em main.c, linha 397 */
        case 3: menuOrdensServico(); break;
        /* listarProntasParaEntrega está em manutencao.c, linha 303 */
        case 4: listarProntasParaEntrega(); break;
        /* menuOrcamentos está em main.c, linha 438 */
        case 5: menuOrcamentos(); break;
        /* consultarHistorico está em relatorio.c, linha 5 */
        case 6: consultarHistorico(); break;
        /* consultarMeusDados está em geral.c, linha 621 */
        case 7: consultarMeusDados(); break;
        /* alterarSenha está em geral.c, linha 638 */
        case 8: alterarSenha(); break;
        /* fazerLogout está em geral.c, linha 611 */
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (usuarioLogado >= 0); /* sai só depois do logout */
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
        /* consultarCliente está em cliente.c, linha 9 */
        case 1: consultarCliente(); break;
        /* consultarEquipamento está em equipamento.c, linha 312 */
        case 2: consultarEquipamento(); break;
        /* consultarOS está em ordem_servico.c, linha 547 */
        case 3: consultarOS(); break;
        /* registrarObservacao está em manutencao.c, linha 5 */
        case 4: registrarObservacao(); break;
        /* registrarDiagnostico está em manutencao.c, linha 68 */
        case 5: registrarDiagnostico(); break;
        /* executarManutencao está em manutencao.c, linha 147 */
        case 6: executarManutencao(); break;
        /* registrarTeste está em manutencao.c, linha 236 */
        case 7: registrarTeste(); break;
        /* consultarHistorico está em relatorio.c, linha 5 */
        case 8: consultarHistorico(); break;
        /* consultarMeusDados está em geral.c, linha 621 */
        case 9: consultarMeusDados(); break;
        /* alterarSenha está em geral.c, linha 638 */
        case 10: alterarSenha(); break;
        /* fazerLogout está em geral.c, linha 611 */
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (usuarioLogado >= 0); /* sai só depois do logout */
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
        /* consultarEquipamento está em equipamento.c, linha 312 */
        case 1: consultarEquipamento(); break;
        /* consultarOS está em ordem_servico.c, linha 547 */
        case 2: consultarOS(); break;
        /* registrarFeedback está em ordem_servico.c, linha 801 */
        case 3: registrarFeedback(); break;
        /* consultarOrcamento está em orcamento.c, linha 69 */
        case 4: consultarOrcamento(); break;
        /* aprovarOrcamento está em orcamento.c, linha 133 */
        case 5: aprovarOrcamento(); break;
        /* recusarOrcamento está em orcamento.c, linha 140 */
        case 6: recusarOrcamento(); break;
        /* consultarHistorico está em relatorio.c, linha 5 */
        case 7: consultarHistorico(); break;
        /* consultarMeusDados está em geral.c, linha 621 */
        case 8: consultarMeusDados(); break;
        /* alterarSenha está em geral.c, linha 638 */
        case 9: alterarSenha(); break;
        /* fazerLogout está em geral.c, linha 611 */
        case 0: fazerLogout(); break;
        default:
            printf("\n[!] Opcao invalida!");
            pausar();
        }
    } while (usuarioLogado >= 0); /* sai só depois do logout */
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
            /* realizarLogin está em geral.c, linha 577 */
            usuarioLogado = realizarLogin();
            if (usuarioLogado < 0)
                break; /* login falhou: volta para a tela inicial */

            /* guarda o perfil antes, porque o logout apaga o usuarioLogado */
            strcpy(perfil, usuarios[usuarioLogado].perfil);
            /* menuDoAdmin está em main.c, linha 514 */
            if (strcmp(perfil, "admin") == 0)
                menuDoAdmin();
            /* menuDaRecepcionista está em main.c, linha 572 */
            if (strcmp(perfil, "recepcionista") == 0)
                menuDaRecepcionista();
            /* menuDoTecnico está em main.c, linha 619 */
            if (strcmp(perfil, "tecnico") == 0)
                menuDoTecnico();
            /* menuDoCliente está em main.c, linha 672 */
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
    /* bdConectar está em banco.c, linha 29 */
    if (!bdConectar())
    {
        printf("[!] Nao foi possivel conectar ao SQL Server (banco bytetech).\n");
        printf("    Confira se o SQL Server esta rodando e se o banco foi criado:\n");
        printf("    sqlcmd -S localhost -E -i docs\\Modelagem_DB\\schema_sqlserver.sql\n");
        return 1;
    }

    /* carregarUsuarios está em geral.c, linha 63 */
    carregarUsuarios();
    /* carregarEquipamentos está em equipamento.c, linha 51 */
    carregarEquipamentos();
    /* carregarOrdens está em ordem_servico.c, linha 66 */
    carregarOrdens();
    /* carregarTecnicosOS está em ordem_servico.c, linha 161 */
    carregarTecnicosOS();

    /* Primeira vez (sem nenhum usuário): cria os usuários de teste do grupo, com a senha byte123 */
    if (totalUsuarios == 0)
    {
        /* adicionarUsuarioInicial está em geral.c, linha 101 */
        adicionarUsuarioInicial("Ana", "ana@bytetech.com", "", "admin", "", "");
        adicionarUsuarioInicial("Carla", "carla@bytetech.com", "", "recepcionista", "", "");
        adicionarUsuarioInicial("Rafael", "rafael@bytetech.com", "", "tecnico", "", "Hardware");
        adicionarUsuarioInicial("Joao Pereira", "joao.pereira@email.com", "(11) 90000-0000", "cliente", "111.111.111-11", "");
        /* salvarUsuarios está em geral.c, linha 16 */
        salvarUsuarios();
    }

    /* menuPrincipal está em main.c, linha 721 */
    menuPrincipal();

    /* bdDesconectar está em banco.c, linha 50 */
    bdDesconectar();
    return 0;
}

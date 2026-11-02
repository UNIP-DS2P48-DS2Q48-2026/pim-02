#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "banco.h"
#include <termios.h>
#include <unistd.h>

#define MAX_USUARIOS 200
#define MAX_EQUIPAMENTOS 300
#define MAX_OS 500
#define MAX_TECNICO_OS 1000

/* Tamanhos de texto comuns */

#define TAM_DATA 11      /* AAAA-MM-DD */
#define TAM_DATAHORA 20  /* AAAA-MM-DD HH:MM:SS */
#define TAM_HASH 128
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

/* Vetores globais com os dados carregados dos arquivos */

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

/* Conexao usada pelas funcoes de cadastro durante a execucao. */
Banco *bancoAtivo = NULL;

/* Prototipo: a implementacao fica em usuarios_sql.c. */
int inserirUsuarioSQL(Banco *banco, Usuario *usuario);
int atualizarUsuarioSQL(Banco *banco, Usuario *usuario);
int inativarUsuarioSQL(Banco *banco, int idUsuario);
int alterarSenhaUsuarioSQL(Banco *banco, int idUsuario, const char *hashAtual, const char *hashNovo);

int podeVerOS(int i);
void nomeDoEquipamento(int idEquipamento, char destino[]);
int buscarOSPorId(int id);
double textoParaValor(char texto[]);

/* Protótipos: funções do main.c usadas pelos outros arquivos */

void limparBuffer();

void imprimirCabecalho(char subtitulo[]);

void lerTexto(char rotulo[], char destino[], int tamanho);

void lerSenha(char rotulo[], char destino[], int tamanho);

int lerInteiro(char rotulo[]);

int confirmar(char pergunta[]);

void pausar();

void gerarHash(char senha[], char destino[]);
int verificarSenha(const char *digitada, const char *armazenada);

void dataHoje(char destino[]);

void dataHoraAgora(char destino[]);

void pegarCampo(char linha[], int posicao, char destino[], int tamanho);

int contemTexto(char texto[], char filtro[]);




#include "senhas.c"
#include "geral.c"

#include "usuarios_sql.c"
#include "persistencia_sql.c"

#include "admin.c"

#include "recepcionista.c"

#include "tecnico.c"

#include "cliente.c"

#include "equipamento.c"

#include "ordem_servico.c"

#include "manutencao.c"

#include "orcamento.c"

#include "relatorio.c"


/* Descarta o que sobrou digitado no teclado até o ENTER */

void limparBuffer()

{

    int c = getchar();

    while (c != '\n' && c != EOF)

    {

        c = getchar();

    }

}

/* Lê uma linha inteira (aceita espaços) e tira o ENTER do final */

void lerTexto(char rotulo[], char destino[], int tamanho)

{

    int i, tam;

    printf("%s", rotulo);

    if (fgets(destino, tamanho, stdin) == NULL)

    {

        destino[0] = '\0'; /* não leu nada (fim da entrada) */

        return;

    }

    tam = strlen(destino);

    if (tam > 0 && destino[tam - 1] == '\n')

    {

        destino[tam - 1] = '\0'; /* tira o ENTER */

    }

    else

    {

        limparBuffer(); /* digitou mais do que cabe: descarta o resto */

    }

    /* ';' separa os campos nos arquivos .txt, então não pode entrar no texto */

    for (i = 0; destino[i] != '\0'; i++)

    {

        if (destino[i] == ';')

        {

            destino[i] = ',';

        }

    }

}

/* Lê um número inteiro; devolve -1 se o que foi digitado não for número */

int lerInteiro(char rotulo[])

{

    char texto[20];

    int valor;

    lerTexto(rotulo, texto, sizeof(texto));

    if (sscanf(texto, "%d", &valor) != 1)

    {

        return -1;

    }

    return valor;

}

/* Pergunta S/N; devolve 1 para S e 0 para qualquer outra resposta */

int confirmar(char pergunta[])

{

    char resposta[5];

    printf("%s (S/N): ", pergunta);

    lerTexto("", resposta, sizeof(resposta));

    if (resposta[0] == 'S' || resposta[0] == 's')

    {

        return 1;

    }

    return 0;

}

void pausar()

{

    char tecla[5];

    lerTexto("\nPressione ENTER para continuar...", tecla, sizeof(tecla));

}

/* Funcoes de hash e verificacao de senhas implementadas em senhas.c. */
/* Data de hoje no formato do banco: AAAA-MM-DD */

void dataHoje(char destino[])

{

    time_t agora = time(NULL);

    strftime(destino, TAM_DATA, "%Y-%m-%d", localtime(&agora));

}

/* Data e hora atuais para criado_em/atualizado_em: AAAA-MM-DD HH:MM:SS */

void dataHoraAgora(char destino[])

{

    time_t agora = time(NULL);

    strftime(destino, TAM_DATAHORA, "%Y-%m-%d %H:%M:%S", localtime(&agora));

}

/* Copia para destino o campo de número 'posicao' (0, 1, 2...) de uma linha do .txt */

void pegarCampo(char linha[], int posicao, char destino[], int tamanho)

{

    int i = 0, j = 0, campo = 0;

    while (linha[i] != '\0' && linha[i] != '\n')

    {

        if (linha[i] == ';')

        {

            campo++;

        }

        else if (campo == posicao && j < tamanho - 1)

        {

            destino[j] = linha[i];

            j++;

        }

        i++;

    }

    destino[j] = '\0';

}

/* O filtro aparece dentro do texto? Não diferencia maiúsculas. Filtro vazio aceita tudo */

int contemTexto(char texto[], char filtro[])

{

    char t[300], f[100];

    int i;

    /* copia os dois textos em minúsculo e procura um dentro do outro com strstr */

    for (i = 0; texto[i] != '\0'; i++)

    {

        t[i] = texto[i];

        if (t[i] >= 'A' && t[i] <= 'Z')

            t[i] = t[i] + 32;

    }

    t[i] = '\0';

    for (i = 0; filtro[i] != '\0'; i++)

    {

        f[i] = filtro[i];

        if (f[i] >= 'A' && f[i] <= 'Z')

            f[i] = f[i] + 32;

    }

    f[i] = '\0';

    if (strstr(t, f) != NULL)

    {

        return 1;

    }

    return 0;

}


/* Le uma senha sem mostrar os caracteres no terminal. */
void lerSenha(char rotulo[], char destino[], int tamanho)
{
    struct termios original;
    struct termios oculto;

    if (tamanho <= 0)
        return;

    destino[0] = '\0';

    // Guarda as configuracoes atuais do terminal.
    if (tcgetattr(STDIN_FILENO, &original) != 0)
    {
        printf("\n[!] Nao foi possivel proteger a entrada da senha.\n");
        return;
    }

    // Copia as configuracoes e desativa a exibicao.
    oculto = original;
    oculto.c_lflag &= ~ECHO;

    // Aplica a configuracao temporaria.
    if (tcsetattr(STDIN_FILENO, TCSANOW, &oculto) != 0)
    {
        printf("\n[!] Nao foi possivel ocultar a senha.\n");
        return;
    }

    // Aproveita nossa funcao de leitura existente.
    lerTexto(rotulo, destino, tamanho);

    // Restaura o comportamento normal do terminal.
    if (tcsetattr(STDIN_FILENO, TCSANOW, &original) != 0)
    {
        fprintf(stderr, "\n[!] Erro ao restaurar o terminal.\n");
    }

    printf("\n");
}


void imprimirCabecalho(char subtitulo[])

{

    char hoje[TAM_DATA];

    time_t agora = time(NULL);

    strftime(hoje, sizeof(hoje), "%d/%m/%Y", localtime(&agora));

    printf("================================================================================\n");

    printf("BYTETECH - Sistema de Gestão                                            %s\n", hoje);

    printf("================================================================================\n");

    if (strlen(subtitulo) > 0)

    {

        printf("%s\n", subtitulo);

        printf("--------------------------------------------------------------------------------\n");

    }

}

/* ---------- Submenus (só Admin e Recepcionista chegam aqui) ---------- */

void menuAdministradores()

{

    int opcao;

    do

    {

        imprimirCabecalho("Administradores");

        printf("\n  1 - Cadastrar\n");

        printf("  2 - Consultar\n");

        printf("  3 - Editar\n");

        printf("  4 - Inativar\n");

        printf("  0 - Voltar\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: cadastrarAdmin(); break;

        case 2: consultarAdmin(); break;

        case 3: editarAdmin(); break;

        case 4: inativarAdmin(); break;

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

        printf("\n  1 - Cadastrar\n");

        printf("  2 - Consultar\n");

        printf("  3 - Editar\n");

        printf("  4 - Inativar\n");

        printf("  0 - Voltar\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: cadastrarRecepcionista(); break;

        case 2: consultarRecepcionista(); break;

        case 3: editarRecepcionista(); break;

        case 4: inativarRecepcionista(); break;

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

        printf("\n  1 - Cadastrar\n");

        printf("  2 - Consultar\n");

        printf("  3 - Editar\n");

        printf("  4 - Inativar\n");

        printf("  0 - Voltar\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: cadastrarTecnico(); break;

        case 2: consultarTecnico(); break;

        case 3: editarTecnico(); break;

        case 4: inativarTecnico(); break;

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

        printf("\n  1 - Cadastrar\n");

        printf("  2 - Consultar\n");

        printf("  3 - Editar\n");

        printf("  4 - Inativar\n");

        printf("  0 - Voltar\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: cadastrarCliente(); break;

        case 2: consultarCliente(); break;

        case 3: editarCliente(); break;

        case 4: inativarCliente(); break;

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

        printf("\n  1 - Cadastrar\n");

        printf("  2 - Consultar\n");

        printf("  3 - Editar\n");

        printf("  4 - Inativar\n");

        printf("  0 - Voltar\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: cadastrarEquipamento(); break;

        case 2: consultarEquipamento(); break;

        case 3: editarEquipamento(); break;

        case 4: inativarEquipamento(); break;

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

        printf("\n  1 - Criar\n");

        printf("  2 - Consultar\n");

        printf("  3 - Editar\n");

        printf("  4 - Cancelar\n");

        printf("  5 - Atribuir Tecnico\n");

        printf("  6 - Registrar Entrega\n");

        printf("  7 - Registrar Feedback\n");

        printf("  0 - Voltar\n\n");

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

        printf("\n  1 - Registrar\n");

        printf("  2 - Editar\n");

        printf("  3 - Consultar\n");

        printf("  4 - Aprovar\n");

        printf("  5 - Recusar\n");

        printf("  6 - Arquivar\n");

        printf("  0 - Voltar\n\n");

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

        printf("\n  1 - Registrar Observacao Tecnica\n");

        printf("  2 - Registrar Diagnostico\n");

        printf("  3 - Executar Manutencao\n");

        printf("  4 - Registrar Teste\n");

        printf("  5 - OS Prontas para Entrega\n");

        printf("  0 - Voltar\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: registrarObservacao(); break;

        case 2: registrarDiagnostico(); break;

        case 3: executarManutencao(); break;

        case 4: registrarTeste(); break;

        case 5: osProntasEntrega(); break;

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

    char titulo[100];

    int opcao;

    sprintf(titulo, "Administrador - Bem-vindo(a), %s!", usuarios[usuarioLogado].nome);

    do

    {

        imprimirCabecalho(titulo);

        printf("\n  1 - Administradores\n");

        printf("  2 - Recepcionistas\n");

        printf("  3 - Tecnicos\n");

        printf("  4 - Clientes\n");

        printf("  5 - Equipamentos\n");

        printf("  6 - Ordens de Servico\n");

        printf("  7 - Manutencao\n");

        printf("  8 - Orcamentos\n");

        printf("  9 - Historico de Equipamento\n");

        printf("  10 - Relatorios\n");

        printf("  11 - Meus Dados\n");

        printf("  12 - Alterar Senha\n");


        printf("  0 - Sair (Fazer Logout)\n\n");

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

        case 9: historicoEquipamento(); break;

        case 10: consultarRelatorios(); break;

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

    char titulo[100];

    int opcao;

    sprintf(titulo, "Recepcionista - Bem-vindo(a), %s!", usuarios[usuarioLogado].nome);

    do

    {

        imprimirCabecalho(titulo);

        printf("\n  1 - Clientes\n");

        printf("  2 - Equipamentos\n");

        printf("  3 - Ordens de Servico\n");

        printf("  4 - OS Prontas para Entrega\n");

        printf("  5 - Orcamentos\n");

        printf("  6 - Historico de Equipamento\n");

        printf("  7 - Meus Dados\n");

        printf("  8 - Alterar Senha\n");


        printf("  0 - Sair (Fazer Logout)\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: menuClientes(); break;

        case 2: menuEquipamentos(); break;

        case 3: menuOrdensServico(); break;

        case 4: osProntasEntrega(); break;

        case 5: menuOrcamentos(); break;

        case 6: historicoEquipamento(); break;

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

    char titulo[100];

    int opcao;

    sprintf(titulo, "Tecnico - Bem-vindo(a), %s!", usuarios[usuarioLogado].nome);

    do

    {

        imprimirCabecalho(titulo);

        printf("\n  1 - Consultar Clientes\n");

        printf("  2 - Consultar Equipamentos\n");

        printf("  3 - Consultar Ordens de Servico\n");

        printf("  4 - Registrar Observacao Tecnica\n");

        printf("  5 - Registrar Diagnostico\n");

        printf("  6 - Executar Manutencao\n");

        printf("  7 - Registrar Teste\n");

        printf("  8 - Historico de Equipamento\n");

        printf("  9 - Meus Dados\n");

        printf("  10 - Alterar Senha\n");


        printf("  0 - Sair (Fazer Logout)\n\n");

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

        case 8: historicoEquipamento(); break;

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

    char titulo[100];

    int opcao;

    sprintf(titulo, "Cliente - Bem-vindo(a), %s!", usuarios[usuarioLogado].nome);

    do

    {

        imprimirCabecalho(titulo);

        printf("\n  1 - Meus Equipamentos\n");

        printf("  2 - Minhas Ordens de Servico\n");

        printf("  3 - Registrar Feedback\n");

        printf("  4 - Consultar Orcamento\n");

        printf("  5 - Aprovar Orcamento\n");

        printf("  6 - Recusar Orcamento\n");

        printf("  7 - Historico de Equipamento\n");

        printf("  8 - Meus Dados\n");

        printf("  9 - Alterar Senha\n");

        printf("  0 - Sair (Fazer Logout)\n\n");

        opcao = lerInteiro("Opcao: ");

        switch (opcao)

        {

        case 1: consultarEquipamento(); break;

        case 2: consultarOS(); break;

        case 3: registrarFeedback(); break;

        case 4: consultarOrcamento(); break;

        case 5: aprovarOrcamento(); break;

        case 6: recusarOrcamento(); break;

        case 7: historicoEquipamento(); break;

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

            /* Sucesso: abre o menu do perfil de quem entrou */

            if (usuarioLogado >= 0)

            {

                if (strcmp(usuarios[usuarioLogado].perfil, "admin") == 0)

                    menuDoAdmin();

                else if (strcmp(usuarios[usuarioLogado].perfil, "recepcionista") == 0)

                    menuDaRecepcionista();

                else if (strcmp(usuarios[usuarioLogado].perfil, "tecnico") == 0)

                    menuDoTecnico();

                else

                    menuDoCliente();

            }

        }

        else if (opcao == 0)

        {

            printf("\nSitema encerrado. Ate logo!\n");

        }

        else

        {

            printf("\n[!] Opcao invalida!");

            pausar();

        }

    }

}

int main(void)

{

    // Inicializa os identificadores ODBC.

    Banco banco = {

        SQL_NULL_HENV,

        SQL_NULL_HDBC,

        SQL_NULL_HSTMT

    };

    // Estabelece a conexao com o SQL Server.

    if (!bdConectar(&banco))

    {

        printf("Erro ao conectar no banco Pim02.\n");

        return 1;

    }

    // Compartilha a conexao ativa com os menus de cadastro.
    bancoAtivo = &banco;

    // Carrega os usuarios do SQL Server.

    if (!carregarUsuariosSQL(&banco))

    {

        bdDesconectar(&banco);

        return 1;

    }

    // Se nao houver usuarios, cria o administrador inicial.

    if (totalUsuarios == 0)

    {

        if (!criarAdministradorInicialSQL(&banco))

        {

            bdDesconectar(&banco);

            return 1;

        }

        // Recarrega os usuarios apos cadastrar o admin.

        if (!carregarUsuariosSQL(&banco))

        {

            bdDesconectar(&banco);

            return 1;

        }

    }

    // Recupera os demais modulos do SQL Server, sem consultar arquivos .txt.
    if (!carregarEquipamentosSQL() || !carregarOrdensSQL() || !carregarTecnicosOSSQL())
    {
        fprintf(stderr, "Falha ao carregar dados operacionais do SQL Server.\n");
        bdDesconectar(&banco);
        return 1;
    }

    // Abre o menu principal.

    menuPrincipal();

    // Encerra a conexao ao sair.

    bdDesconectar(&banco);

    return 0;

}

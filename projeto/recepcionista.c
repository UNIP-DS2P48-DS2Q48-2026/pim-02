#include <stdio.h>
#include <string.h>

extern void limparBuffer(void);
extern void imprimirCabecalho(const char *subtitulo);

/* UC08 - Cadastrar Recepcionista: cadastra uma nova recepcionista */
void cadastrarRecepcionista(void)
{
    char nome[50], email[50], senha[50];

    imprimirCabecalho("Recepcionistas > Cadastrar");

    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("E-mail: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    /* Validação de campos obrigatórios (RNF09) */
    if (strlen(nome) == 0 || strlen(email) == 0 || strlen(senha) == 0)
    {
        printf("[!] Erro: Todos os campos sao obrigatorios.\n");
        return;
    }

    /* Simulação de verificação de e-mail único (RN13) e inserção no banco de dados */
    printf("\n[+] Validando e-mail (RN13)...\n");
    printf("[+] Gravando no sistema com perfil 'recepcionista', status 'ativo' e senha em hash...\n");

    printf("\n[+] Cadastro da recepcionista realizado com sucesso!\n");
}

/* UC09 - Consultar Recepcionista: lista e busca as recepcionistas */
void consultarRecepcionista(void)
{
    char filtro[50];

    imprimirCabecalho("Recepcionistas > Consultar");

    printf("Informe um filtro (nome/e-mail) ou pressione ENTER para listar todos: ");
    fgets(filtro, sizeof(filtro), stdin);
    filtro[strcspn(filtro, "\n")] = '\0';

    /* Simulação de busca no banco incluindo registros inativos (RN12) */
    printf("\n--- Resultados da Busca ---\n");
    if (strlen(filtro) > 0)
    {
        printf("Buscando recepcionistas (ativos e inativos) para o filtro: '%s'...\n", filtro);
    }
    else
    {
        printf("Listando todos os recepcionistas (ativos e inativos)...\n");
    }

    /* Aqui entraria o loop com a cláusula SELECT do banco ou varredura do array */
    printf("[+] Consulta finalizada.\n");
}

/* UC10 - Editar Recepcionista: altera os dados de uma recepcionista */
void editarRecepcionista(void)
{
    int id_alvo;

    imprimirCabecalho("Recepcionistas > Editar");

    printf("Digite o ID da recepcionista que deseja editar (ou 0 para voltar): ");
    if (scanf("%d", &id_alvo) != 1 || id_alvo == 0)
    {
        limparBuffer();
        return;
    }
    limparBuffer();

    /* Simulação de atualização de dados e validação de unicidade do novo e-mail (RN13) */
    printf("\nEditando os dados da Recepcionista ID: %d\n", id_alvo);
    printf("[+] Validando as alteracoes e checando se o e-mail ja existe...\n");

    printf("\n[+] Cadastro atualizado e 'atualizado_em' modificado com sucesso!\n");
}

/* UC11 - Inativar Recepcionista: inativa uma recepcionista sem OS em andamento */
void inativarRecepcionista(void)
{
    int id_alvo;
    char confirmacao;

    imprimirCabecalho("Recepcionistas > Inativar");

    printf("Digite o ID da recepcionista para inativar (ou 0 para voltar): ");
    if (scanf("%d", &id_alvo) != 1 || id_alvo == 0)
    {
        limparBuffer();
        return;
    }
    limparBuffer();

    /* Validação de bloqueio caso a recepcionista tenha OS "aberta" ou "em andamento" (RN14) */
    printf("\n[+] Verificando se existem Ordens de Servico em andamento para o ID %d...\n", id_alvo);

    printf("Tem certeza que deseja inativar a recepcionista ID %d? (S/N): ", id_alvo);
    scanf(" %c", &confirmacao);
    limparBuffer();

    if (confirmacao == 'S' || confirmacao == 's')
    {
        /* Simulação de inativação preservando histórico (RN12) */
        printf("\n[+] Recepcionista inativada com sucesso. (Historico preservado).\n");
    }
    else
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.\n");
    }
}
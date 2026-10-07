#include <stdio.h>
#include <string.h>

/* Funções utilitárias externas já declaradas no main.c */
extern void limparBuffer(void);
extern void imprimirCabecalho(const char *subtitulo);

/* UC16 - Cadastrar Cliente: cadastra um novo cliente com CPF e telefone */
void cadastrarCliente(void)
{
    char nome[50], email[50], telefone[20], cpf[20], senha[50];

    imprimirCabecalho("Clientes > Cadastrar");

    printf("Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("E-mail: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    printf("Telefone: ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    printf("CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    printf("Senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    /* Validação de campos obrigatórios (RNF09) */
    if (strlen(nome) == 0 || strlen(email) == 0 || strlen(cpf) == 0 || strlen(senha) == 0)
    {
        printf("\n[!] Erro: Nome, E-mail, CPF e Senha sao campos obrigatorios.\n");
        return;
    }

    /* Simulação de regras de negócio antes da inserção */
    printf("\n[+] Validando e-mail e CPF...\n");
    printf("[+] Gravando no sistema com perfil 'cliente', status 'ativo' e senha em hash...\n");

    printf("\n[+] Cliente cadastrado com sucesso e apto a ter equipamentos vinculados!\n");
}

/* UC17 - Consultar Cliente: lista e busca os clientes por nome, CPF ou e-mail */
void consultarCliente(void)
{
    char filtro[50];

    imprimirCabecalho("Clientes > Consultar");

    printf("Informe um filtro (nome, CPF ou e-mail) ou pressione ENTER para listar todos: ");
    fgets(filtro, sizeof(filtro), stdin);
    filtro[strcspn(filtro, "\n")] = '\0';

    printf("\n--- Resultados da Busca ---\n");

    /* Simulação de restrição de escopo por perfil (RN10) */
    printf("[+] Verificando escopo do perfil logado...\n");
    printf("    - Recepcionista/Admin: visualizam todos os clientes.\n");
    printf("    - Tecnico: visualizam apenas os clientes de suas OS atribuidas.\n\n");

    if (strlen(filtro) > 0)
    {
        printf("Buscando clientes para o filtro: '%s'...\n", filtro);
    }
    else
    {
        printf("Listando os clientes cadastrados...\n");
    }

    /* Aqui ocorreria a exibição iterativa dos clientes */
    printf("\n[+] Consulta finalizada.\n");
}

/* UC18 - Editar Cliente: altera os dados de um cliente */
void editarCliente(void)
{
    char busca[50];
    char nome[50], email[50], telefone[20], cpf[20], senha[50];

    imprimirCabecalho("Clientes > Editar");

    printf("Digite o Nome ou CPF do cliente que deseja editar (ou ENTER para voltar): ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';

    if (strlen(busca) == 0)
    {
        return;
    }

    /* Simulação de busca e edição */
    printf("\n[+] Cliente localizado. Insira os novos dados (ou pressione ENTER para manter o atual).\n\n");

    printf("Novo Nome: ");
    fgets(nome, sizeof(nome), stdin);
    nome[strcspn(nome, "\n")] = '\0';

    printf("Novo E-mail: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    printf("Novo Telefone: ");
    fgets(telefone, sizeof(telefone), stdin);
    telefone[strcspn(telefone, "\n")] = '\0';

    printf("Novo CPF: ");
    fgets(cpf, sizeof(cpf), stdin);
    cpf[strcspn(cpf, "\n")] = '\0';

    printf("Nova Senha: ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    printf("\n[+] Validando se o novo E-mail ou CPF ja estao em uso por outro cadastro...\n");
    printf("[+] Verificando preenchimento de campos obrigatorios (RNF09)...\n");

    printf("\n[+] Cadastro atualizado com sucesso (data de 'atualizado_em' modificada)!\n");
}

/* UC19 - Inativar Cliente: inativa um cliente sem OS em andamento */
void inativarCliente(void)
{
    char busca[50];
    char confirmacao;

    imprimirCabecalho("Clientes > Inativar");

    printf("Digite o Nome ou CPF do cliente para inativar (ou ENTER para voltar): ");
    fgets(busca, sizeof(busca), stdin);
    busca[strcspn(busca, "\n")] = '\0';

    if (strlen(busca) == 0)
    {
        return;
    }

    /* Simulação da verificação de bloqueio por OS ativa (RN14) */
    printf("\n[+] Buscando cliente '%s'...\n", busca);
    printf("[+] Validando se o cliente possui OS 'aberta' ou 'em_andamento' vinculada aos seus equipamentos...\n");

    printf("\nTem certeza que deseja inativar este cliente? (S/N): ");
    scanf(" %c", &confirmacao);
    limparBuffer();

    if (confirmacao == 'S' || confirmacao == 's')
    {
        /* Simulação de inativação lógica preservando o histórico (RN12) */
        printf("\n[+] Cliente inativado com sucesso (status = 'inativo').\n");
        printf("[+] Historico preservado. Este cliente nao podera abrir novas OS.\n");
    }
    else
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.\n");
    }
}
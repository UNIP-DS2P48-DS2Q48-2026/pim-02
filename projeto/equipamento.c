#include <stdio.h>
#include <string.h>

/* Funções utilitárias externas declaradas no main.c */
extern void limparBuffer(void);
extern void imprimirCabecalho(const char *subtitulo);

/* UC20 - Cadastrar Equipamento: cadastra um hardware ou software de um cliente */
void cadastrarEquipamento(void)
{
    char cliente_busca[50];
    int categoria_opcao;
    char tipo[50], marca[50], modelo_serie[100];

    imprimirCabecalho("Equipamentos > Cadastrar");

    /* Vinculação do equipamento a um cliente (RN01) */
    printf("Informe o CPF ou Nome do cliente proprietario: ");
    fgets(cliente_busca, sizeof(cliente_busca), stdin);
    cliente_busca[strcspn(cliente_busca, "\n")] = '\0';

    printf("\n[+] Buscando cliente e validando status...\n");
    printf("[+] (Verificando se o cliente possui status 'ativo')\n");
    printf("[+] (Verificando se o usuario tem perfil 'cliente')\n\n");

    printf("Selecione a Categoria do Equipamento:\n");
    printf("1 - Hardware\n");
    printf("2 - Software\n");
    printf("Opcao: ");
    if (scanf("%d", &categoria_opcao) != 1)
    {
        limparBuffer();
        printf("[!] Erro na leitura da categoria.\n");
        return;
    }
    limparBuffer();

    printf("Tipo (ex: Notebook, Sistema Operacional): ");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = '\0';

    printf("Marca: ");
    fgets(marca, sizeof(marca), stdin);
    marca[strcspn(marca, "\n")] = '\0';

    if (categoria_opcao == 2)
    {
        printf("Chave de Licenca: ");
    }
    else
    {
        printf("Modelo / Numero de Serie: ");
    }
    fgets(modelo_serie, sizeof(modelo_serie), stdin);
    modelo_serie[strcspn(modelo_serie, "\n")] = '\0';

    /* Validação de campos obrigatórios (RNF09) */
    if (strlen(tipo) == 0 || strlen(marca) == 0 || strlen(modelo_serie) == 0)
    {
        printf("\n[!] Erro: Todos os campos (tipo, marca, modelo/serie) sao obrigatorios (RNF09).\n");
        return;
    }

    printf("\n[+] Gravando o cadastro no sistema com status 'ativo'...\n");
    printf("[+] Equipamento cadastrado com sucesso e vinculado ao cliente!\n");
}

/* UC21 - Consultar Equipamento: lista e busca os equipamentos que o perfil pode ver */
void consultarEquipamento(void)
{
    char filtro[50];

    imprimirCabecalho("Equipamentos > Consultar");

    printf("Informe um filtro (cliente, tipo ou numero de serie) ou ENTER para listar todos: ");
    fgets(filtro, sizeof(filtro), stdin);
    filtro[strcspn(filtro, "\n")] = '\0';

    printf("\n--- Resultados da Busca ---\n");

    /* Regra de restrição de visibilidade (RN10, RI10) */
    printf("[+] Verificando escopo do perfil logado...\n");
    printf("    - Cliente: so ve os proprios equipamentos.\n");
    printf("    - Tecnico: so ve os equipamentos das OS atribuidas a ele.\n");
    printf("    - Recepcionista/Admin: ve todos.\n\n");

    if (strlen(filtro) > 0)
    {
        printf("Buscando equipamentos ativos e inativos para o filtro: '%s'...\n", filtro);
    }
    else
    {
        printf("Listando todos os equipamentos permitidos ao seu perfil...\n");
    }

    /* Simulação de exibição dos registros (incluindo status) */
    printf("\n[+] Consulta finalizada.\n");
}

/* UC22 - Editar Equipamento: altera tipo, marca e modelo/numero de serie */
void editarEquipamento(void)
{
    int id_alvo;
    char tipo[50], marca[50], modelo_serie[100];

    imprimirCabecalho("Equipamentos > Editar");

    printf("Digite o ID do equipamento que deseja alterar (ou 0 para voltar): ");
    if (scanf("%d", &id_alvo) != 1 || id_alvo
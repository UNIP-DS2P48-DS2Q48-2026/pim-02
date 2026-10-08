/* Regrava o os.txt inteiro com o que está no vetor. Dinheiro -1.00 = ainda não informado */
int salvarOrdens()
{
    FILE *arquivo;
    int i;

    arquivo = fopen(ARQ_OS, "w");
    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_OS);
        return 0;
    }
    fprintf(arquivo, "id_os;id_equipamento;id_atendente;data_abertura;problema_relatado;diagnostico;orcamento;status;resultado_resposta_cliente;data_entrega;resultado_teste;valor_pago;feedback_cliente;satisfeito;criado_em;atualizado_em\n");
    for (i = 0; i < totalOrdens; i++)
    {
        fprintf(arquivo, "%d;%d;%d;%s;%s;%s;%.2f;%s;%s;%s;%s;%.2f;%s;%s;%s;%s\n",
                ordens[i].id_os, ordens[i].id_equipamento, ordens[i].id_atendente, ordens[i].data_abertura,
                ordens[i].problema_relatado, ordens[i].diagnostico, ordens[i].orcamento, ordens[i].status,
                ordens[i].resultado_resposta_cliente, ordens[i].data_entrega, ordens[i].resultado_teste,
                ordens[i].valor_pago, ordens[i].feedback_cliente, ordens[i].satisfeito,
                ordens[i].criado_em, ordens[i].atualizado_em);
    }
    fclose(arquivo);
    return 1;
}

/* Lê o os.txt para o vetor (sem arquivo = nenhuma OS ainda) */
void carregarOrdens()
{
    FILE *arquivo;
    char linha[TAM_LINHA];
    char numero[30];
    int n;

    totalOrdens = 0;
    arquivo = fopen(ARQ_OS, "r");
    if (arquivo == NULL)
    {
        return;
    }
    while (totalOrdens < MAX_OS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        /* Pula o cabeçalho e linhas vazias: toda linha de dados começa com o id (um número) */
        if (linha[0] < '0' || linha[0] > '9')
            continue;

        n = totalOrdens;
        pegarCampo(linha, 0, numero, sizeof(numero));
        ordens[n].id_os = atoi(numero);
        pegarCampo(linha, 1, numero, sizeof(numero));
        ordens[n].id_equipamento = atoi(numero);
        pegarCampo(linha, 2, numero, sizeof(numero));
        ordens[n].id_atendente = atoi(numero);
        pegarCampo(linha, 3, ordens[n].data_abertura, sizeof(ordens[n].data_abertura));
        pegarCampo(linha, 4, ordens[n].problema_relatado, sizeof(ordens[n].problema_relatado));
        pegarCampo(linha, 5, ordens[n].diagnostico, sizeof(ordens[n].diagnostico));
        pegarCampo(linha, 6, numero, sizeof(numero));
        ordens[n].orcamento = atof(numero);
        pegarCampo(linha, 7, ordens[n].status, sizeof(ordens[n].status));
        pegarCampo(linha, 8, ordens[n].resultado_resposta_cliente, sizeof(ordens[n].resultado_resposta_cliente));
        pegarCampo(linha, 9, ordens[n].data_entrega, sizeof(ordens[n].data_entrega));
        pegarCampo(linha, 10, ordens[n].resultado_teste, sizeof(ordens[n].resultado_teste));
        pegarCampo(linha, 11, numero, sizeof(numero));
        ordens[n].valor_pago = atof(numero);
        pegarCampo(linha, 12, ordens[n].feedback_cliente, sizeof(ordens[n].feedback_cliente));
        pegarCampo(linha, 13, ordens[n].satisfeito, sizeof(ordens[n].satisfeito));
        pegarCampo(linha, 14, ordens[n].criado_em, sizeof(ordens[n].criado_em));
        pegarCampo(linha, 15, ordens[n].atualizado_em, sizeof(ordens[n].atualizado_em));
        totalOrdens++;
    }
    fclose(arquivo);
}

/* Regrava o tecnico_os.txt inteiro com o que está no vetor */
int salvarTecnicosOS()
{
    FILE *arquivo;
    int i;

    arquivo = fopen(ARQ_TECNICO_OS, "w");
    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_TECNICO_OS);
        return 0;
    }
    fprintf(arquivo, "id_tecnico_os;id_tecnico;id_os;data_atribuicao;observacoes_tecnicas;criado_em;atualizado_em\n");
    for (i = 0; i < totalTecnicosOS; i++)
    {
        fprintf(arquivo, "%d;%d;%d;%s;%s;%s;%s\n", tecnicosOS[i].id_tecnico_os, tecnicosOS[i].id_tecnico,
                tecnicosOS[i].id_os, tecnicosOS[i].data_atribuicao, tecnicosOS[i].observacoes_tecnicas,
                tecnicosOS[i].criado_em, tecnicosOS[i].atualizado_em);
    }
    fclose(arquivo);
    return 1;
}

/* Lê o tecnico_os.txt para o vetor (sem arquivo = nenhuma atribuição ainda) */
void carregarTecnicosOS()
{
    FILE *arquivo;
    char linha[TAM_LINHA];
    char numero[20];
    int n;

    totalTecnicosOS = 0;
    arquivo = fopen(ARQ_TECNICO_OS, "r");
    if (arquivo == NULL)
    {
        return;
    }
    while (totalTecnicosOS < MAX_TECNICO_OS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        /* Pula o cabeçalho e linhas vazias: toda linha de dados começa com o id (um número) */
        if (linha[0] < '0' || linha[0] > '9')
            continue;

        n = totalTecnicosOS;
        pegarCampo(linha, 0, numero, sizeof(numero));
        tecnicosOS[n].id_tecnico_os = atoi(numero);
        pegarCampo(linha, 1, numero, sizeof(numero));
        tecnicosOS[n].id_tecnico = atoi(numero);
        pegarCampo(linha, 2, numero, sizeof(numero));
        tecnicosOS[n].id_os = atoi(numero);
        pegarCampo(linha, 3, tecnicosOS[n].data_atribuicao, sizeof(tecnicosOS[n].data_atribuicao));
        pegarCampo(linha, 4, tecnicosOS[n].observacoes_tecnicas, sizeof(tecnicosOS[n].observacoes_tecnicas));
        pegarCampo(linha, 5, tecnicosOS[n].criado_em, sizeof(tecnicosOS[n].criado_em));
        pegarCampo(linha, 6, tecnicosOS[n].atualizado_em, sizeof(tecnicosOS[n].atualizado_em));
        totalTecnicosOS++;
    }
    fclose(arquivo);
}

/* UC24 - Criar Ordem de Servico: abre uma OS para um equipamento do cliente */
void criarOrdemServico() {
}

/* UC25 - Consultar Ordem de Servico: lista e busca as OS por numero, CPF, status ou periodo */
void consultarOS() {
}

/* UC26 - Editar Ordem de Servico: altera o problema relatado da OS */
void editarOS() {
}

/* UC27 - Cancelar Ordem de Servico: cancela a OS sem apagar */
void cancelarOS() {
}

/* UC28 - Atribuir Tecnico a Ordem de Servico: vincula um ou mais tecnicos a OS */
void atribuirTecnico() {
}

/* UC29 - Registrar Entrega e Pagamento: registra a data de entrega e o valor pago */
void registrarEntrega() {
}

/* UC30 - Registrar Feedback do Cliente: finaliza a OS ou devolve para novo reparo */
void registrarFeedback() {
}

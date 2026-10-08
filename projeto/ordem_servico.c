/* Regrava o os.txt inteiro com o que está no vetor */
void salvarOrdens(void)
{
    FILE *arquivo = fopen(ARQ_OS, "w");
    int i;

    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_OS);
        return;
    }
    for (i = 0; i < totalOrdens; i++)
    {
        OrdemServico *o = &ordens[i];
        fprintf(arquivo, "%d;%d;%d;%s;%s;%s;", o->id_os, o->id_equipamento, o->id_atendente,
                o->data_abertura, o->problema_relatado, o->diagnostico);
        gravarValor(arquivo, o->orcamento);
        fprintf(arquivo, ";%s;%s;%s;%s;", o->status, o->resultado_resposta_cliente,
                o->data_entrega, o->resultado_teste);
        gravarValor(arquivo, o->valor_pago);
        fprintf(arquivo, ";%s;%s;%s;%s\n", o->feedback_cliente, o->satisfeito,
                o->criado_em, o->atualizado_em);
    }
    fclose(arquivo);
}

/* Lê o os.txt para o vetor (sem arquivo = nenhuma OS ainda) */
void carregarOrdens(void)
{
    FILE *arquivo = fopen(ARQ_OS, "r");
    char linha[TAM_LINHA];

    totalOrdens = 0;
    if (arquivo == NULL)
    {
        return;
    }
    while (totalOrdens < MAX_OS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        OrdemServico *o = &ordens[totalOrdens];
        char *cursor = linha;

        if (linha[0] == '\n' || linha[0] == '\r')
            continue;
        o->id_os = lerCampoInt(&cursor);
        o->id_equipamento = lerCampoInt(&cursor);
        o->id_atendente = lerCampoInt(&cursor);
        lerCampo(&cursor, o->data_abertura, sizeof(o->data_abertura));
        lerCampo(&cursor, o->problema_relatado, sizeof(o->problema_relatado));
        lerCampo(&cursor, o->diagnostico, sizeof(o->diagnostico));
        o->orcamento = lerCampoValor(&cursor);
        lerCampo(&cursor, o->status, sizeof(o->status));
        lerCampo(&cursor, o->resultado_resposta_cliente, sizeof(o->resultado_resposta_cliente));
        lerCampo(&cursor, o->data_entrega, sizeof(o->data_entrega));
        lerCampo(&cursor, o->resultado_teste, sizeof(o->resultado_teste));
        o->valor_pago = lerCampoValor(&cursor);
        lerCampo(&cursor, o->feedback_cliente, sizeof(o->feedback_cliente));
        lerCampo(&cursor, o->satisfeito, sizeof(o->satisfeito));
        lerCampo(&cursor, o->criado_em, sizeof(o->criado_em));
        lerCampo(&cursor, o->atualizado_em, sizeof(o->atualizado_em));
        totalOrdens++;
    }
    fclose(arquivo);
}

/* Regrava o tecnico_os.txt inteiro com o que está no vetor */
void salvarTecnicosOS(void)
{
    FILE *arquivo = fopen(ARQ_TECNICO_OS, "w");
    int i;

    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_TECNICO_OS);
        return;
    }
    for (i = 0; i < totalTecnicosOS; i++)
    {
        TecnicoOS *t = &tecnicosOS[i];
        fprintf(arquivo, "%d;%d;%d;%s;%s;%s;%s\n", t->id_tecnico_os, t->id_tecnico, t->id_os,
                t->data_atribuicao, t->observacoes_tecnicas, t->criado_em, t->atualizado_em);
    }
    fclose(arquivo);
}

/* Lê o tecnico_os.txt para o vetor (sem arquivo = nenhuma atribuição ainda) */
void carregarTecnicosOS(void)
{
    FILE *arquivo = fopen(ARQ_TECNICO_OS, "r");
    char linha[TAM_LINHA];

    totalTecnicosOS = 0;
    if (arquivo == NULL)
    {
        return;
    }
    while (totalTecnicosOS < MAX_TECNICO_OS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        TecnicoOS *t = &tecnicosOS[totalTecnicosOS];
        char *cursor = linha;

        if (linha[0] == '\n' || linha[0] == '\r')
            continue;
        t->id_tecnico_os = lerCampoInt(&cursor);
        t->id_tecnico = lerCampoInt(&cursor);
        t->id_os = lerCampoInt(&cursor);
        lerCampo(&cursor, t->data_atribuicao, sizeof(t->data_atribuicao));
        lerCampo(&cursor, t->observacoes_tecnicas, sizeof(t->observacoes_tecnicas));
        lerCampo(&cursor, t->criado_em, sizeof(t->criado_em));
        lerCampo(&cursor, t->atualizado_em, sizeof(t->atualizado_em));
        totalTecnicosOS++;
    }
    fclose(arquivo);
}

/* UC24 - Criar Ordem de Servico: abre uma OS para um equipamento do cliente */
void criarOrdemServico(void) {
}

/* UC25 - Consultar Ordem de Servico: lista e busca as OS por numero, CPF, status ou periodo */
void consultarOS(void) {
}

/* UC26 - Editar Ordem de Servico: altera o problema relatado da OS */
void editarOS(void) {
}

/* UC27 - Cancelar Ordem de Servico: cancela a OS sem apagar */
void cancelarOS(void) {
}

/* UC28 - Atribuir Tecnico a Ordem de Servico: vincula um ou mais tecnicos a OS */
void atribuirTecnico(void) {
}

/* UC29 - Registrar Entrega e Pagamento: registra a data de entrega e o valor pago */
void registrarEntrega(void) {
}

/* UC30 - Registrar Feedback do Cliente: finaliza a OS ou devolve para novo reparo */
void registrarFeedback(void) {
}

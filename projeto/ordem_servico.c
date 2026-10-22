#if USAR_SQLSERVER
/* Quantas posições de cada vetor já estão gravadas no banco. Registro novo entra sempre no fim do vetor */
int ordensNoBanco = 0;
int tecnicosOSNoBanco = 0;

/* Grava o vetor na tabela os: UPDATE do que já está no banco, INSERT do que é novo.
   Texto vazio e dinheiro -1 (ainda não informado) viram NULL */
int salvarOrdens()
{
    char sql[4000];
    char abertura[30], problema[410], diagnostico[410], orcamento[30], status[40], resposta[30];
    char entrega[30], teste[30], pago[30], feedback[410], satisfeito[20], criado[50], atualizado[50];
    int i, ok = 1;

    /* deixa gravar o id que o programa escolheu, em vez de o banco gerar outro */
    bdExecutar("SET IDENTITY_INSERT os ON");
    for (i = 0; i < totalOrdens && ok; i++)
    {
        sqlTexto(abertura, ordens[i].data_abertura);
        sqlTexto(problema, ordens[i].problema_relatado);
        sqlTexto(diagnostico, ordens[i].diagnostico);
        sqlDinheiro(orcamento, ordens[i].orcamento);
        sqlTexto(status, ordens[i].status);
        sqlTexto(resposta, ordens[i].resultado_resposta_cliente);
        sqlTexto(entrega, ordens[i].data_entrega);
        sqlTexto(teste, ordens[i].resultado_teste);
        sqlDinheiro(pago, ordens[i].valor_pago);
        sqlTexto(feedback, ordens[i].feedback_cliente);
        sqlTexto(satisfeito, ordens[i].satisfeito);
        sqlTexto(criado, ordens[i].criado_em);
        sqlTexto(atualizado, ordens[i].atualizado_em);

        if (i < ordensNoBanco)
        {
            sprintf(sql, "UPDATE os SET id_equipamento = %d, id_atendente = %d, data_abertura = %s, problema_relatado = %s, "
                         "diagnostico = %s, orcamento = %s, status = %s, resultado_resposta_cliente = %s, data_entrega = %s, "
                         "resultado_teste = %s, valor_pago = %s, feedback_cliente = %s, satisfeito = %s, criado_em = %s, "
                         "atualizado_em = %s WHERE id_os = %d",
                    ordens[i].id_equipamento, ordens[i].id_atendente, abertura, problema, diagnostico, orcamento,
                    status, resposta, entrega, teste, pago, feedback, satisfeito, criado, atualizado, ordens[i].id_os);
        }
        else
        {
            sprintf(sql, "INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, "
                         "orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, "
                         "feedback_cliente, satisfeito, criado_em, atualizado_em) "
                         "VALUES (%d, %d, %d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                    ordens[i].id_os, ordens[i].id_equipamento, ordens[i].id_atendente, abertura, problema, diagnostico,
                    orcamento, status, resposta, entrega, teste, pago, feedback, satisfeito, criado, atualizado);
        }
        ok = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT os OFF");

    if (ok)
        ordensNoBanco = totalOrdens;
    return ok;
}

/* Lê a tabela os para o vetor. Dinheiro NULL vira -1 (ainda não informado) */
void carregarOrdens()
{
    char numero[30];
    int n;

    totalOrdens = 0;
    ordensNoBanco = 0;
    /* CONVERT 120 devolve AAAA-MM-DD (date) e AAAA-MM-DD HH:MM:SS (datetime2), igual aos .txt */
    if (!bdConsultar("SELECT id_os, id_equipamento, id_atendente, CONVERT(VARCHAR(10), data_abertura, 120), "
                     "problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, "
                     "CONVERT(VARCHAR(10), data_entrega, 120), resultado_teste, valor_pago, feedback_cliente, satisfeito, "
                     "CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                     "FROM os ORDER BY id_os"))
    {
        return;
    }
    while (totalOrdens < MAX_OS && bdProximaLinha())
    {
        n = totalOrdens;
        bdLerTexto(1, numero, sizeof(numero));
        ordens[n].id_os = atoi(numero);
        bdLerTexto(2, numero, sizeof(numero));
        ordens[n].id_equipamento = atoi(numero);
        bdLerTexto(3, numero, sizeof(numero));
        ordens[n].id_atendente = atoi(numero);
        bdLerTexto(4, ordens[n].data_abertura, sizeof(ordens[n].data_abertura));
        bdLerTexto(5, ordens[n].problema_relatado, sizeof(ordens[n].problema_relatado));
        bdLerTexto(6, ordens[n].diagnostico, sizeof(ordens[n].diagnostico));
        bdLerTexto(7, numero, sizeof(numero));
        if (numero[0] == '\0')
            ordens[n].orcamento = -1;
        else
            ordens[n].orcamento = atof(numero);
        bdLerTexto(8, ordens[n].status, sizeof(ordens[n].status));
        bdLerTexto(9, ordens[n].resultado_resposta_cliente, sizeof(ordens[n].resultado_resposta_cliente));
        bdLerTexto(10, ordens[n].data_entrega, sizeof(ordens[n].data_entrega));
        bdLerTexto(11, ordens[n].resultado_teste, sizeof(ordens[n].resultado_teste));
        bdLerTexto(12, numero, sizeof(numero));
        if (numero[0] == '\0')
            ordens[n].valor_pago = -1;
        else
            ordens[n].valor_pago = atof(numero);
        bdLerTexto(13, ordens[n].feedback_cliente, sizeof(ordens[n].feedback_cliente));
        bdLerTexto(14, ordens[n].satisfeito, sizeof(ordens[n].satisfeito));
        bdLerTexto(15, ordens[n].criado_em, sizeof(ordens[n].criado_em));
        bdLerTexto(16, ordens[n].atualizado_em, sizeof(ordens[n].atualizado_em));
        totalOrdens++;
    }
    ordensNoBanco = totalOrdens;
}

/* Grava o vetor na tabela tecnico_os: UPDATE do que já está no banco, INSERT do que é novo */
int salvarTecnicosOS()
{
    char sql[1500];
    char atribuicao[30], observacoes[610], criado[50], atualizado[50];
    int i, ok = 1;

    /* deixa gravar o id que o programa escolheu, em vez de o banco gerar outro */
    bdExecutar("SET IDENTITY_INSERT tecnico_os ON");
    for (i = 0; i < totalTecnicosOS && ok; i++)
    {
        sqlTexto(atribuicao, tecnicosOS[i].data_atribuicao);
        sqlTexto(observacoes, tecnicosOS[i].observacoes_tecnicas);
        sqlTexto(criado, tecnicosOS[i].criado_em);
        sqlTexto(atualizado, tecnicosOS[i].atualizado_em);

        if (i < tecnicosOSNoBanco)
        {
            sprintf(sql, "UPDATE tecnico_os SET id_tecnico = %d, id_os = %d, data_atribuicao = %s, observacoes_tecnicas = %s, "
                         "criado_em = %s, atualizado_em = %s WHERE id_tecnico_os = %d",
                    tecnicosOS[i].id_tecnico, tecnicosOS[i].id_os, atribuicao, observacoes, criado, atualizado,
                    tecnicosOS[i].id_tecnico_os);
        }
        else
        {
            sprintf(sql, "INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, "
                         "criado_em, atualizado_em) VALUES (%d, %d, %d, %s, %s, %s, %s)",
                    tecnicosOS[i].id_tecnico_os, tecnicosOS[i].id_tecnico, tecnicosOS[i].id_os, atribuicao,
                    observacoes, criado, atualizado);
        }
        ok = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT tecnico_os OFF");

    if (ok)
        tecnicosOSNoBanco = totalTecnicosOS;
    return ok;
}

/* Lê a tabela tecnico_os para o vetor */
void carregarTecnicosOS()
{
    char numero[20];
    int n;

    totalTecnicosOS = 0;
    tecnicosOSNoBanco = 0;
    if (!bdConsultar("SELECT id_tecnico_os, id_tecnico, id_os, CONVERT(VARCHAR(10), data_atribuicao, 120), "
                     "observacoes_tecnicas, CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                     "FROM tecnico_os ORDER BY id_tecnico_os"))
    {
        return;
    }
    while (totalTecnicosOS < MAX_TECNICO_OS && bdProximaLinha())
    {
        n = totalTecnicosOS;
        bdLerTexto(1, numero, sizeof(numero));
        tecnicosOS[n].id_tecnico_os = atoi(numero);
        bdLerTexto(2, numero, sizeof(numero));
        tecnicosOS[n].id_tecnico = atoi(numero);
        bdLerTexto(3, numero, sizeof(numero));
        tecnicosOS[n].id_os = atoi(numero);
        bdLerTexto(4, tecnicosOS[n].data_atribuicao, sizeof(tecnicosOS[n].data_atribuicao));
        bdLerTexto(5, tecnicosOS[n].observacoes_tecnicas, sizeof(tecnicosOS[n].observacoes_tecnicas));
        bdLerTexto(6, tecnicosOS[n].criado_em, sizeof(tecnicosOS[n].criado_em));
        bdLerTexto(7, tecnicosOS[n].atualizado_em, sizeof(tecnicosOS[n].atualizado_em));
        totalTecnicosOS++;
    }
    tecnicosOSNoBanco = totalTecnicosOS;
}
#else
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
#endif

/* ---------- Funções de apoio às OS (usadas também no manutencao.c, orcamento.c e relatorio.c) ---------- */

/* Posição no vetor da OS com este id (-1 se não existir) */
int buscarOSPorId(int id)
{
    int i;

    for (i = 0; i < totalOrdens; i++)
    {
        if (ordens[i].id_os == id)
            return i;
    }
    return -1;
}

/* RN10: cliente só vê as OS dos próprios equipamentos; técnico só as OS atribuídas a ele */
int podeVerOS(int i)
{
    int idLogado = usuarios[usuarioLogado].id_usuario;

    if (perfilLogado("cliente"))
    {
        if (donoDoEquipamento(ordens[i].id_equipamento) == idLogado)
            return 1;
        return 0;
    }
    if (perfilLogado("tecnico"))
    {
        if (buscarTecnicoNaOS(idLogado, ordens[i].id_os) >= 0)
            return 1;
        return 0;
    }
    return 1;
}

/* Copia para destino a etapa em que a OS está, olhando quais campos já foram preenchidos.
   Confere da etapa mais adiantada (entrega) para a mais antiga (vistoria): a primeira que bater é a atual */
void etapaDaOS(int i, char destino[])
{
    if (strcmp(ordens[i].status, "aberta") == 0)
        strcpy(destino, "aguardando tecnico");          /* falta atribuir o técnico (UC28) */
    else if (strcmp(ordens[i].status, "em_andamento") != 0)
        strcpy(destino, ordens[i].status);              /* finalizada ou cancelada */
    else if (strlen(ordens[i].data_entrega) > 0)
        strcpy(destino, "aguardando feedback");         /* entregue (UC29), falta o feedback (UC30) */
    else if (strcmp(ordens[i].resultado_teste, "aprovado") == 0)
        strcpy(destino, "pronta para entrega");         /* teste aprovado (UC34) */
    else if (strcmp(ordens[i].resultado_resposta_cliente, "aprovado") == 0)
        strcpy(destino, "em reparo");                   /* orçamento aprovado (UC39): reparo e teste */
    else if (strcmp(ordens[i].resultado_resposta_cliente, "recusado") == 0)
        strcpy(destino, "orcamento recusado");          /* recusado (UC40): renegociar ou arquivar */
    else if (ordens[i].orcamento >= 0)
        strcpy(destino, "aguardando resposta");         /* orçamento registrado (UC36) */
    else if (strlen(ordens[i].diagnostico) > 0)
        strcpy(destino, "aguardando orcamento");        /* diagnóstico registrado (UC32) */
    else
        strcpy(destino, "em vistoria");                 /* o técnico está olhando o equipamento (UC31) */
}

/* Mostra uma linha da lista de OS: número, abertura, cliente, equipamento e etapa */
void imprimirLinhaOS(int i)
{
    char dono[60], etapa[25];
    int e = buscarEquipamentoPorId(ordens[i].id_equipamento);

    nomeDoCliente(equipamentos[e].id_cliente, dono);
    etapaDaOS(i, etapa);
    printf("OS %d - %s - %s - %s %s - %s\n", ordens[i].id_os, ordens[i].data_abertura, dono, equipamentos[e].tipo,
           equipamentos[e].marca, etapa);
}

/* A OS entra na lista?
   etapa = "" : as OS em aberto (e as finalizadas, se comFinalizadas for 1)
   etapa = "em reparo", "aguardando resposta"...: só as OS que estão nessa etapa (ver etapaDaOS) */
int osNaLista(int i, char etapa[], int comFinalizadas)
{
    char etapaAtual[25];

    if (strlen(etapa) > 0)
    {
        etapaDaOS(i, etapaAtual);
        if (strcmp(etapaAtual, etapa) == 0)
            return 1;
        return 0;
    }
    if (strcmp(ordens[i].status, "aberta") == 0 || strcmp(ordens[i].status, "em_andamento") == 0)
        return 1;
    if (comFinalizadas == 1 && strcmp(ordens[i].status, "finalizada") == 0)
        return 1;
    return 0;
}

/* Lista as OS que o usuário pode ver e pede o número. Devolve a posição no vetor ou -1.
   comFinalizadas = 1 inclui as finalizadas (só o Admin, ao editar: RN08) */
int selecionarOS(char etapa[], int comFinalizadas)
{
    int i, id, mostrados = 0;

    printf("\n");
    for (i = 0; i < totalOrdens; i++)
    {
        if (podeVerOS(i) && osNaLista(i, etapa, comFinalizadas))
        {
            imprimirLinhaOS(i);
            mostrados++;
        }
    }

    if (mostrados == 0)
    {
        if (strlen(etapa) > 0)
            printf("\n[!] Nenhuma ordem de servico na etapa \"%s\".", etapa);
        else
            printf("\n[!] Nenhuma ordem de servico em aberto.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nNumero da OS (0 para voltar): ");
    if (id <= 0)
        return -1;

    i = buscarOSPorId(id);
    if (i < 0 || !podeVerOS(i) || !osNaLista(i, etapa, comFinalizadas))
    {
        printf("\n[!] OS %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return i;
}

/* Cria a linha deste técnico nesta OS na tecnico_os (UC28, e o Admin que cobre o técnico: RN11/RN15). Devolve a posição ou -1 */
int novaLinhaTecnicoOS(int idTecnico, int idOS)
{
    int n = totalTecnicosOS;

    if (n >= MAX_TECNICO_OS)
    {
        printf("\n[!] Limite de %d atribuicoes atingido.", MAX_TECNICO_OS);
        pausar();
        return -1;
    }

    /* O vetor está em ordem de id: o novo id é o id do último + 1 */
    tecnicosOS[n].id_tecnico_os = 1;
    if (n > 0)
        tecnicosOS[n].id_tecnico_os = tecnicosOS[n - 1].id_tecnico_os + 1;
    tecnicosOS[n].id_tecnico = idTecnico;
    tecnicosOS[n].id_os = idOS;
    dataHoje(tecnicosOS[n].data_atribuicao);
    strcpy(tecnicosOS[n].observacoes_tecnicas, "");
    dataHoraAgora(tecnicosOS[n].criado_em);
    strcpy(tecnicosOS[n].atualizado_em, tecnicosOS[n].criado_em);
    totalTecnicosOS++;
    return n;
}

/* Mostra todos os dados da OS: cliente, equipamento, andamento e técnicos */
void mostrarOS(int i)
{
    char etapa[25];
    int c, e, k, t;

    etapaDaOS(i, etapa);
    e = buscarEquipamentoPorId(ordens[i].id_equipamento);
    c = buscarUsuarioPorId(equipamentos[e].id_cliente);

    printf("\nOS %d - %s\n", ordens[i].id_os, etapa);
    printf("Cliente: %s - CPF %s - %s\n", usuarios[c].nome, usuarios[c].cpf, usuarios[c].telefone);
    printf("Equipamento: %s %s\n", equipamentos[e].tipo, equipamentos[e].marca);
    printf("Abertura: %s\n", ordens[i].data_abertura);
    printf("Problema: %s\n", ordens[i].problema_relatado);
    printf("Diagnostico: %s\n", ordens[i].diagnostico);
    if (ordens[i].orcamento >= 0)
        printf("Orcamento: R$ %.2f %s\n", ordens[i].orcamento, ordens[i].resultado_resposta_cliente);
    printf("Teste: %s\n", ordens[i].resultado_teste);
    if (strlen(ordens[i].data_entrega) > 0)
        printf("Entrega: %s - pago R$ %.2f\n", ordens[i].data_entrega, ordens[i].valor_pago);
    printf("Feedback: %s %s\n", ordens[i].satisfeito, ordens[i].feedback_cliente);

    for (k = 0; k < totalTecnicosOS; k++)
    {
        if (tecnicosOS[k].id_os == ordens[i].id_os)
        {
            t = buscarUsuarioPorId(tecnicosOS[k].id_tecnico);
            printf("Tecnico: %s - %s\n", usuarios[t].nome, tecnicosOS[k].observacoes_tecnicas);
        }
    }
}

/* ---------- Casos de uso da ordem de servico ---------- */

/* UC24 - Criar Ordem de Servico: abre uma OS para um equipamento do cliente */
void criarOrdemServico()
{
    char cpf[20], problema[200];
    int i, c = -1, e, n, id, mostrados = 0;

    imprimirCabecalho("Ordens de Servico > Criar");

    if (totalOrdens >= MAX_OS)
    {
        printf("\n[!] Limite de %d ordens de servico atingido.", MAX_OS);
        pausar();
        return;
    }

    lerTexto("\nCPF do cliente, como foi cadastrado (ENTER para voltar): ", cpf, sizeof(cpf));
    if (strlen(cpf) == 0)
        return;

    for (i = 0; i < totalUsuarios; i++)
    {
        if (strcmp(usuarios[i].perfil, "cliente") == 0 && strcmp(usuarios[i].cpf, cpf) == 0)
            c = i;
    }

    /* RN01: a OS só é aberta para cliente já cadastrado e ativo (RN14) */
    if (c < 0)
    {
        printf("\n[!] Nenhum cliente com o CPF %s. Cadastre o cliente e o equipamento antes de abrir a OS.", cpf);
        pausar();
        return;
    }
    if (strcmp(usuarios[c].status, "ativo") != 0)
    {
        printf("\n[!] O cliente %s esta inativo. Nao e possivel abrir OS (RN14).", usuarios[c].nome);
        pausar();
        return;
    }

    /* Equipamentos ativos deste cliente */
    printf("\nCliente: %s\n", usuarios[c].nome);
    for (e = 0; e < totalEquipamentos; e++)
    {
        if (equipamentos[e].id_cliente == usuarios[c].id_usuario && strcmp(equipamentos[e].status, "ativo") == 0)
        {
            printf("ID %d - %s %s - %s\n", equipamentos[e].id_equipamento, equipamentos[e].tipo, equipamentos[e].marca,
                   equipamentos[e].modelo_numero_serie);
            mostrados++;
        }
    }
    if (mostrados == 0)
    {
        printf("\n[!] O cliente nao tem equipamento ativo. Cadastre em Equipamentos > Cadastrar (RN02).");
        pausar();
        return;
    }

    id = lerInteiro("\nID do equipamento (0 para voltar): ");
    if (id <= 0)
        return;
    e = buscarEquipamentoPorId(id);
    if (e < 0 || equipamentos[e].id_cliente != usuarios[c].id_usuario || strcmp(equipamentos[e].status, "ativo") != 0)
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return;
    }

    lerTexto("\nProblema relatado pelo cliente: ", problema, sizeof(problema));
    if (!campoPreenchido(problema, "problema relatado"))
        return;

    /* Preenche a próxima posição do vetor; o novo id é o id da última OS + 1; dinheiro -1 = ainda não informado */
    n = totalOrdens;
    ordens[n].id_os = 1;
    if (n > 0)
        ordens[n].id_os = ordens[n - 1].id_os + 1;
    ordens[n].id_equipamento = id;
    ordens[n].id_atendente = usuarios[usuarioLogado].id_usuario; /* RN03 */
    dataHoje(ordens[n].data_abertura);
    strcpy(ordens[n].problema_relatado, problema);
    strcpy(ordens[n].diagnostico, "");
    ordens[n].orcamento = -1;
    strcpy(ordens[n].status, "aberta");
    strcpy(ordens[n].resultado_resposta_cliente, "");
    strcpy(ordens[n].data_entrega, "");
    strcpy(ordens[n].resultado_teste, "");
    ordens[n].valor_pago = -1;
    strcpy(ordens[n].feedback_cliente, "");
    strcpy(ordens[n].satisfeito, "");
    dataHoraAgora(ordens[n].criado_em);
    strcpy(ordens[n].atualizado_em, ordens[n].criado_em);

    totalOrdens++;
    if (salvarOrdens())
        printf("\n[+] OS %d criada! Agora atribua um tecnico em Ordens de Servico > Atribuir Tecnico.", ordens[n].id_os);
    else
        totalOrdens--; /* não gravou: desfaz */
    pausar();
}

/* UC25 - Consultar Ordem de Servico: lista as OS (inclusive finalizadas e canceladas) com um filtro */
void consultarOS()
{
    char filtro[60], etapa[25];
    int i, c, id, mostrados;

    imprimirCabecalho("Ordens de Servico > Consultar");

    /* Pede o filtro de novo enquanto nenhuma OS for encontrada (exceção 4a) */
    do
    {
        lerTexto("\nFiltro (cliente, CPF ou etapa), ENTER para listar todas ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;

        mostrados = 0;
        for (i = 0; i < totalOrdens; i++)
        {
            if (!podeVerOS(i)) /* RN10, RI10 */
                continue;

            /* O filtro pode estar no nome do cliente, no CPF ou na etapa da OS */
            c = buscarUsuarioPorId(donoDoEquipamento(ordens[i].id_equipamento));
            etapaDaOS(i, etapa);
            if (strstr(usuarios[c].nome, filtro) != NULL || strstr(usuarios[c].cpf, filtro) != NULL ||
                strstr(etapa, filtro) != NULL)
            {
                imprimirLinhaOS(i);
                mostrados++;
            }
        }

        if (mostrados == 0)
            printf("\n[!] Nenhuma OS encontrada para o filtro informado. Tente outro filtro.\n");
    } while (mostrados == 0);

    id = lerInteiro("\nNumero da OS para ver os detalhes (0 para voltar): ");
    if (id <= 0)
        return;
    i = buscarOSPorId(id);
    if (i < 0 || !podeVerOS(i))
    {
        printf("\n[!] OS %d nao esta na lista.", id);
        pausar();
        return;
    }
    mostrarOS(i);
    pausar();
}

/* UC26 - Editar Ordem de Servico: altera o problema relatado da OS */
void editarOS()
{
    char problema[200];
    int i;

    imprimirCabecalho("Ordens de Servico > Editar");

    /* RN08 e UC26 (exceção 4a): só o Admin edita também a OS finalizada */
    i = selecionarOS("", perfilLogado("admin"));
    if (i < 0)
        return;

    /* RN07: a recepção altera só o dado administrativo; diagnóstico e teste são do técnico */
    printf("\nSo o problema relatado pode ser alterado (RN07). ENTER mantem o valor atual.\n");
    lerOuManter("Problema relatado", ordens[i].problema_relatado, problema, sizeof(problema));

    strcpy(ordens[i].problema_relatado, problema);
    dataHoraAgora(ordens[i].atualizado_em);
    if (salvarOrdens())
        printf("\n[+] OS %d atualizada!", ordens[i].id_os);
    pausar();
}

/* UC27 - Cancelar Ordem de Servico: cancela a OS sem apagar */
void cancelarOS()
{
    int i;

    imprimirCabecalho("Ordens de Servico > Cancelar");

    i = selecionarOS("", 0);
    if (i < 0)
        return;

    if (!confirmar("\nCancelar esta OS? Ela continua no historico (RN09)."))
    {
        printf("\nOperacao cancelada. A OS nao foi alterada.");
        pausar();
        return;
    }

    strcpy(ordens[i].status, "cancelada");
    dataHoraAgora(ordens[i].atualizado_em);
    if (salvarOrdens())
        printf("\n[+] OS %d cancelada. O registro foi mantido no historico.", ordens[i].id_os);
    pausar();
}

/* UC28 - Atribuir Tecnico a Ordem de Servico: vincula um técnico à OS (pode repetir para mais de um, RN04) */
void atribuirTecnico()
{
    int i, t, id, mostrados = 0;

    imprimirCabecalho("Ordens de Servico > Atribuir Tecnico");

    i = selecionarOS("", 0);
    if (i < 0)
        return;

    /* RN15: só técnico ou, na falta dele, admin (RN11); RN14: só ativos */
    printf("\nTecnicos disponiveis:\n");
    for (t = 0; t < totalUsuarios; t++)
    {
        if (strcmp(usuarios[t].status, "ativo") == 0 &&
            (strcmp(usuarios[t].perfil, "tecnico") == 0 || strcmp(usuarios[t].perfil, "admin") == 0))
        {
            printf("ID %d - %s - %s %s\n", usuarios[t].id_usuario, usuarios[t].nome, usuarios[t].perfil,
                   usuarios[t].especialidade);
            mostrados++;
        }
    }
    if (mostrados == 0)
    {
        printf("\n[!] Nenhum tecnico ativo cadastrado.");
        pausar();
        return;
    }

    id = lerInteiro("\nID do tecnico (0 para voltar): ");
    if (id <= 0)
        return;

    t = buscarUsuarioPorId(id);
    if (t < 0 || strcmp(usuarios[t].status, "ativo") != 0 ||
        (strcmp(usuarios[t].perfil, "tecnico") != 0 && strcmp(usuarios[t].perfil, "admin") != 0))
    {
        printf("\n[!] O usuario %d nao e um tecnico ativo (RN14, RN15).", id);
        pausar();
        return;
    }
    if (buscarTecnicoNaOS(id, ordens[i].id_os) >= 0)
    {
        printf("\n[!] %s ja esta nesta OS.", usuarios[t].nome);
        pausar();
        return;
    }

    if (novaLinhaTecnicoOS(id, ordens[i].id_os) < 0)
        return;
    if (!salvarTecnicosOS())
    {
        totalTecnicosOS--; /* não gravou: desfaz */
        pausar();
        return;
    }

    /* Com técnico atribuído, a OS sai de "aberta" e vai para a vistoria */
    strcpy(ordens[i].status, "em_andamento");
    dataHoraAgora(ordens[i].atualizado_em);
    if (salvarOrdens())
        printf("\n[+] %s atribuido(a) a OS %d!", usuarios[t].nome, ordens[i].id_os);
    pausar();
}

/* UC29 - Registrar Entrega e Pagamento: registra a data de entrega e o valor pago */
void registrarEntrega()
{
    double valor;
    int i, confirmado;

    imprimirCabecalho("Ordens de Servico > Registrar Entrega");

    i = selecionarOS("", 0);
    if (i < 0)
        return;

    /* RF08: só entrega depois do teste aprovado */
    if (strcmp(ordens[i].resultado_teste, "aprovado") != 0 || strlen(ordens[i].data_entrega) > 0)
    {
        printf("\n[!] A OS %d nao esta pronta para entrega.", ordens[i].id_os);
        pausar();
        return;
    }

    printf("\nOrcamento aprovado: R$ %.2f\n", ordens[i].orcamento);

    /* Exceção 7a: valor diferente do orçamento aprovado. O atendente confirma ou digita o valor de novo */
    do
    {
        valor = lerValor("Valor recebido em R$ (ENTER para voltar): ");
        if (valor < 0)
        {
            printf("\nNada foi gravado.");
            pausar();
            return;
        }

        confirmado = 1;
        if (valor != ordens[i].orcamento)
        {
            confirmado = confirmar("\n[!] Valor diferente do orcamento. Confirmar mesmo assim?");
            if (!confirmado)
                printf("Digite o valor correto.\n");
        }
    } while (!confirmado);

    dataHoje(ordens[i].data_entrega);
    ordens[i].valor_pago = valor;
    dataHoraAgora(ordens[i].atualizado_em);
    if (salvarOrdens())
        printf("\n[+] Entrega registrada! A OS %d aguarda o feedback do cliente.", ordens[i].id_os);
    pausar();
}

/* UC30 - Registrar Feedback do Cliente: finaliza a OS ou devolve para novo reparo */
void registrarFeedback()
{
    int i;

    imprimirCabecalho("Ordens de Servico > Registrar Feedback");

    i = selecionarOS("", 0);
    if (i < 0)
        return;

    if (strlen(ordens[i].data_entrega) == 0)
    {
        printf("\n[!] A OS %d ainda nao foi entregue.", ordens[i].id_os);
        pausar();
        return;
    }

    lerTexto("\nComentario do cliente: ", ordens[i].feedback_cliente, sizeof(ordens[i].feedback_cliente));

    if (confirmar("O cliente ficou satisfeito?"))
    {
        strcpy(ordens[i].satisfeito, "sim");
        strcpy(ordens[i].status, "finalizada");
    }
    else
    {
        /* Não satisfeito: volta para Executar Manutenção (limpa teste, entrega e pagamento) */
        strcpy(ordens[i].satisfeito, "nao");
        strcpy(ordens[i].resultado_teste, "");
        strcpy(ordens[i].data_entrega, "");
        ordens[i].valor_pago = -1;
    }

    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    if (strcmp(ordens[i].status, "finalizada") == 0)
        printf("\n[+] Feedback registrado. A OS %d foi finalizada!", ordens[i].id_os);
    else
        printf("\n[+] Feedback registrado. A OS %d voltou para Executar Manutencao.", ordens[i].id_os);
    pausar();
}

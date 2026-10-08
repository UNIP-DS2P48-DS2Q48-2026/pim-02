/* Regrava o equipamento.txt inteiro com o que está no vetor */
void salvarEquipamentos(void)
{
    FILE *arquivo = fopen(ARQ_EQUIPAMENTO, "w");
    int i;

    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_EQUIPAMENTO);
        return;
    }
    for (i = 0; i < totalEquipamentos; i++)
    {
        Equipamento *e = &equipamentos[i];
        fprintf(arquivo, "%d;%d;%s;%s;%s;%s;%s;%s;%s\n",
                e->id_equipamento, e->id_cliente, e->categoria, e->tipo, e->marca,
                e->modelo_numero_serie, e->status, e->criado_em, e->atualizado_em);
    }
    fclose(arquivo);
}

/* Lê o equipamento.txt para o vetor (sem arquivo = nenhum equipamento ainda) */
void carregarEquipamentos(void)
{
    FILE *arquivo = fopen(ARQ_EQUIPAMENTO, "r");
    char linha[TAM_LINHA];

    totalEquipamentos = 0;
    if (arquivo == NULL)
    {
        return;
    }
    while (totalEquipamentos < MAX_EQUIPAMENTOS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        Equipamento *e = &equipamentos[totalEquipamentos];
        char *cursor = linha;

        if (linha[0] == '\n' || linha[0] == '\r')
            continue;
        e->id_equipamento = lerCampoInt(&cursor);
        e->id_cliente = lerCampoInt(&cursor);
        lerCampo(&cursor, e->categoria, sizeof(e->categoria));
        lerCampo(&cursor, e->tipo, sizeof(e->tipo));
        lerCampo(&cursor, e->marca, sizeof(e->marca));
        lerCampo(&cursor, e->modelo_numero_serie, sizeof(e->modelo_numero_serie));
        lerCampo(&cursor, e->status, sizeof(e->status));
        lerCampo(&cursor, e->criado_em, sizeof(e->criado_em));
        lerCampo(&cursor, e->atualizado_em, sizeof(e->atualizado_em));
        totalEquipamentos++;
    }
    fclose(arquivo);
}

/* Posição no vetor do equipamento com este id (-1 se não existir) */
int buscarEquipamentoPorId(int id)
{
    for (int i = 0; i < totalEquipamentos; i++)
    {
        if (equipamentos[i].id_equipamento == id)
            return i;
    }
    return -1;
}

/* RN14: o equipamento tem alguma OS aberta ou em andamento? */
int equipamentoTemOSEmAndamento(int idEquipamento)
{
    for (int i = 0; i < totalOrdens; i++)
    {
        if (ordens[i].id_equipamento == idEquipamento &&
            (strcmp(ordens[i].status, "aberta") == 0 || strcmp(ordens[i].status, "em_andamento") == 0))
            return 1;
    }
    return 0;
}

/* RN10/RI10: cliente só vê os próprios; técnico só os das OS atribuídas a ele; atendente vê todos */
int podeVerEquipamento(int i)
{
    int idLogado = usuarios[usuarioLogado].id_usuario;

    if (podeAtender())
        return 1;
    if (perfilLogado("cliente"))
        return equipamentos[i].id_cliente == idLogado;
    if (perfilLogado("tecnico"))
    {
        for (int j = 0; j < totalOrdens; j++)
        {
            if (ordens[j].id_equipamento != equipamentos[i].id_equipamento)
                continue;
            for (int k = 0; k < totalTecnicosOS; k++)
            {
                if (tecnicosOS[k].id_os == ordens[j].id_os && tecnicosOS[k].id_tecnico == idLogado)
                    return 1;
            }
        }
    }
    return 0;
}

/* Nome do dono do equipamento, para as listagens */
const char *nomeCliente(int idCliente)
{
    int i = buscarUsuarioPorId(idCliente);
    return i >= 0 ? usuarios[i].nome : "?";
}

/* Lista os equipamentos visíveis que batem com o filtro (cliente, tipo, marca ou número de série); devolve quantos mostrou */
int listarEquipamentos(const char *filtro, int soAtivos)
{
    int mostrados = 0;

    for (int i = 0; i < totalEquipamentos; i++)
    {
        Equipamento *e = &equipamentos[i];
        const char *dono = nomeCliente(e->id_cliente);

        if (!podeVerEquipamento(i) || (soAtivos && strcmp(e->status, "ativo") != 0))
            continue;
        if (!contemTexto(dono, filtro) && !contemTexto(e->tipo, filtro) && !contemTexto(e->marca, filtro) &&
            !contemTexto(e->modelo_numero_serie, filtro))
            continue;

        if (mostrados == 0)
        {
            printf("\n  %-4s %-14s %-8s %-14s %-10s %-15s %s\n", "ID", "Cliente", "Categ.", "Tipo", "Marca",
                   "Modelo/Serie", "Status");
            printf("  ----------------------------------------------------------------------------\n");
        }
        printf("  %-4d %-14.14s %-8s %-14.14s %-10.10s %-15.15s %s\n", e->id_equipamento, dono, e->categoria,
               e->tipo, e->marca, e->modelo_numero_serie, e->status);
        mostrados++;
    }
    return mostrados;
}

/* Pede um filtro, lista e pede o ID. Devolve a posição no vetor ou -1 (voltou ou ID inválido) */
int selecionarEquipamento(int soAtivos)
{
    char filtro[60];
    int id, i;

    lerTexto("\nFiltro (cliente, tipo, marca ou numero de serie), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return -1;

    if (listarEquipamentos(filtro, soAtivos) == 0)
    {
        printf("\n[!] Nenhum equipamento encontrado para o filtro informado.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nID do equipamento (0 para voltar): ");
    if (id <= 0)
        return -1;
    i = buscarEquipamentoPorId(id);
    if (i < 0 || !podeVerEquipamento(i) || (soAtivos && strcmp(equipamentos[i].status, "ativo") != 0))
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return i;
}

/* UC20 - Cadastrar Equipamento: cadastra um hardware ou software de um cliente */
void cadastrarEquipamento(void)
{
    Equipamento novo;
    int iCliente, categoria, maiorId = 0;

    memset(&novo, 0, sizeof(novo));
    imprimirCabecalho("Equipamentos > Cadastrar");

    if (!podeAtender())
    {
        printf("\n[!] Seu perfil nao pode cadastrar equipamentos (RN10).");
        pausar();
        return;
    }

    /* Vinculação do equipamento a um cliente (RN01) */
    printf("\nCliente proprietario:");
    iCliente = selecionarUsuario("cliente", 0);
    if (iCliente < 0)
        return;
    if (strcmp(usuarios[iCliente].status, "ativo") != 0)
    {
        printf("\n[!] O cliente %s esta inativo e nao pode receber equipamentos (RN14).", usuarios[iCliente].nome);
        pausar();
        return;
    }

    printf("\nSelecione a Categoria do Equipamento:\n");
    printf("1 - Hardware\n");
    printf("2 - Software\n");
    categoria = lerInteiro("Opcao: ");
    if (categoria != 1 && categoria != 2)
    {
        printf("\n[!] Categoria invalida. Nada foi gravado.");
        pausar();
        return;
    }
    strcpy(novo.categoria, categoria == 1 ? "hardware" : "software");

    lerTexto("Tipo (ex: Notebook, Sistema Operacional): ", novo.tipo, sizeof(novo.tipo));
    lerTexto("Marca: ", novo.marca, sizeof(novo.marca));
    lerTexto(categoria == 2 ? "Chave de Licenca: " : "Modelo / Numero de Serie: ",
             novo.modelo_numero_serie, sizeof(novo.modelo_numero_serie));

    /* Validação de campos obrigatórios (RNF09) */
    if (!campoPreenchido(novo.tipo, "Tipo") || !campoPreenchido(novo.marca, "Marca") ||
        !campoPreenchido(novo.modelo_numero_serie, categoria == 2 ? "Chave de Licenca" : "Modelo / Numero de Serie"))
        return;
    if (totalEquipamentos >= MAX_EQUIPAMENTOS)
    {
        printf("\n[!] Limite de %d equipamentos atingido. Nada foi gravado.", MAX_EQUIPAMENTOS);
        pausar();
        return;
    }

    for (int i = 0; i < totalEquipamentos; i++)
    {
        if (equipamentos[i].id_equipamento > maiorId)
            maiorId = equipamentos[i].id_equipamento;
    }
    novo.id_equipamento = maiorId + 1;
    novo.id_cliente = usuarios[iCliente].id_usuario;
    strcpy(novo.status, "ativo");
    dataHoraAgora(novo.criado_em);
    strcpy(novo.atualizado_em, novo.criado_em);

    equipamentos[totalEquipamentos++] = novo;
    salvarEquipamentos();

    printf("\n[+] Equipamento cadastrado e vinculado a %s! ID: %d", usuarios[iCliente].nome, novo.id_equipamento);
    pausar();
}

/* UC21 - Consultar Equipamento: lista e busca os equipamentos que o perfil pode ver */
void consultarEquipamento(void)
{
    char filtro[60];

    imprimirCabecalho("Equipamentos > Consultar");

    while (1)
    {
        lerTexto("\nFiltro (cliente, tipo, marca ou numero de serie), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;
        /* Ativos e inativos aparecem, com o status, para preservar o histórico */
        if (listarEquipamentos(filtro, 0) > 0)
            break;
        printf("\n[!] Nenhum equipamento encontrado para o filtro informado.\n");
        if (strlen(filtro) == 0)
        {
            pausar();
            return;
        }
    }
    pausar();
}

/* UC22 - Editar Equipamento: altera tipo, marca e modelo/numero de serie */
void editarEquipamento(void)
{
    Equipamento editado;
    int i;

    imprimirCabecalho("Equipamentos > Editar");

    if (!podeAtender())
    {
        printf("\n[!] Seu perfil nao pode editar equipamentos (RN10).");
        pausar();
        return;
    }

    i = selecionarEquipamento(0);
    if (i < 0)
        return;

    editado = equipamentos[i]; /* trabalha numa cópia: se algo falhar, nada muda */
    printf("\nEquipamento de %s (%s). Deixe em branco para manter o valor atual.\n\n",
           nomeCliente(editado.id_cliente), editado.categoria);
    lerOuManter("Tipo", editado.tipo, sizeof(editado.tipo));
    lerOuManter("Marca", editado.marca, sizeof(editado.marca));
    lerOuManter(strcmp(editado.categoria, "software") == 0 ? "Chave de Licenca" : "Modelo / Numero de Serie",
                editado.modelo_numero_serie, sizeof(editado.modelo_numero_serie));

    dataHoraAgora(editado.atualizado_em);
    equipamentos[i] = editado;
    salvarEquipamentos();

    printf("\n[+] Equipamento ID %d atualizado com sucesso!", editado.id_equipamento);
    pausar();
}

/* UC23 - Inativar Equipamento: inativa um equipamento sem OS em andamento */
void inativarEquipamento(void)
{
    char pergunta[150];
    int i;

    imprimirCabecalho("Equipamentos > Inativar");

    if (!podeAtender())
    {
        printf("\n[!] Seu perfil nao pode inativar equipamentos (RN10).");
        pausar();
        return;
    }

    i = selecionarEquipamento(1);
    if (i < 0)
        return;

    /* Bloqueio caso o equipamento tenha OS "aberta" ou "em andamento" (RN14) */
    if (equipamentoTemOSEmAndamento(equipamentos[i].id_equipamento))
    {
        printf("\n[!] O equipamento ID %d tem ordem de servico aberta ou em andamento. Inativacao bloqueada (RN14).",
               equipamentos[i].id_equipamento);
        pausar();
        return;
    }

    sprintf(pergunta, "\nTem certeza que deseja inativar o equipamento ID %d (%s %s)?", equipamentos[i].id_equipamento,
            equipamentos[i].tipo, equipamentos[i].marca);
    if (!confirmar(pergunta))
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.");
        pausar();
        return;
    }

    /* Inativação preservando o histórico (RN12) */
    strcpy(equipamentos[i].status, "inativo");
    dataHoraAgora(equipamentos[i].atualizado_em);
    salvarEquipamentos();

    printf("\n[+] Equipamento inativado. O historico de OS foi preservado e ele nao recebe novas OS.");
    pausar();
}


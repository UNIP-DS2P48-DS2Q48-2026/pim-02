/* Regrava o equipamento.txt inteiro com o que está no vetor */
int salvarEquipamentos()
{
    FILE *arquivo;
    int i;

    arquivo = fopen(ARQ_EQUIPAMENTO, "w");
    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_EQUIPAMENTO);
        return 0;
    }
    fprintf(arquivo, "id_equipamento;id_cliente;categoria;tipo;marca;modelo_numero_serie;status;criado_em;atualizado_em\n");
    for (i = 0; i < totalEquipamentos; i++)
    {
        fprintf(arquivo, "%d;%d;%s;%s;%s;%s;%s;%s;%s\n",
                equipamentos[i].id_equipamento, equipamentos[i].id_cliente, equipamentos[i].categoria,
                equipamentos[i].tipo, equipamentos[i].marca, equipamentos[i].modelo_numero_serie,
                equipamentos[i].status, equipamentos[i].criado_em, equipamentos[i].atualizado_em);
    }
    fclose(arquivo);
    return 1;
}

/* Lê o equipamento.txt para o vetor (sem arquivo = nenhum equipamento ainda) */
void carregarEquipamentos()
{
    FILE *arquivo;
    char linha[TAM_LINHA];
    char numero[20];
    int n;

    totalEquipamentos = 0;
    arquivo = fopen(ARQ_EQUIPAMENTO, "r");
    if (arquivo == NULL)
    {
        return;
    }
    while (totalEquipamentos < MAX_EQUIPAMENTOS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        /* Pula o cabeçalho e linhas vazias: toda linha de dados começa com o id (um número) */
        if (linha[0] < '0' || linha[0] > '9')
            continue;

        n = totalEquipamentos;
        pegarCampo(linha, 0, numero, sizeof(numero));
        equipamentos[n].id_equipamento = atoi(numero);
        pegarCampo(linha, 1, numero, sizeof(numero));
        equipamentos[n].id_cliente = atoi(numero);
        pegarCampo(linha, 2, equipamentos[n].categoria, sizeof(equipamentos[n].categoria));
        pegarCampo(linha, 3, equipamentos[n].tipo, sizeof(equipamentos[n].tipo));
        pegarCampo(linha, 4, equipamentos[n].marca, sizeof(equipamentos[n].marca));
        pegarCampo(linha, 5, equipamentos[n].modelo_numero_serie, sizeof(equipamentos[n].modelo_numero_serie));
        pegarCampo(linha, 6, equipamentos[n].status, sizeof(equipamentos[n].status));
        pegarCampo(linha, 7, equipamentos[n].criado_em, sizeof(equipamentos[n].criado_em));
        pegarCampo(linha, 8, equipamentos[n].atualizado_em, sizeof(equipamentos[n].atualizado_em));
        totalEquipamentos++;
    }
    fclose(arquivo);
}

/* Posição no vetor do equipamento com este id (-1 se não existir) */
int buscarEquipamentoPorId(int id)
{
    int i;

    for (i = 0; i < totalEquipamentos; i++)
    {
        if (equipamentos[i].id_equipamento == id)
            return i;
    }
    return -1;
}

/* RN14: o equipamento tem alguma OS aberta ou em andamento? */
int equipamentoTemOSEmAndamento(int idEquipamento)
{
    int i;

    for (i = 0; i < totalOrdens; i++)
    {
        if (ordens[i].id_equipamento != idEquipamento)
            continue;
        if (strcmp(ordens[i].status, "aberta") == 0 || strcmp(ordens[i].status, "em_andamento") == 0)
            return 1;
    }
    return 0;
}

/* RN10/RI10: cliente só vê os próprios; técnico só os das OS atribuídas a ele; atendente vê todos */
int podeVerEquipamento(int i)
{
    int j, idLogado = usuarios[usuarioLogado].id_usuario;

    if (perfilLogado("cliente"))
    {
        if (equipamentos[i].id_cliente == idLogado)
            return 1;
        return 0;
    }
    if (perfilLogado("tecnico"))
    {
        for (j = 0; j < totalOrdens; j++)
        {
            if (ordens[j].id_equipamento == equipamentos[i].id_equipamento && tecnicoEstaNaOS(idLogado, ordens[j].id_os))
                return 1;
        }
        return 0;
    }
    return 1;
}

/* Copia para destino o nome do cliente com este id, para as listagens */
void nomeDoCliente(int idCliente, char destino[])
{
    int i = buscarUsuarioPorId(idCliente);

    if (i >= 0)
        strcpy(destino, usuarios[i].nome);
    else
        strcpy(destino, "?");
}

/* Lista os equipamentos visíveis que batem com o filtro (cliente, tipo, marca ou número de série); devolve quantos mostrou */
int listarEquipamentos(char filtro[], int soAtivos)
{
    char dono[60];
    int i, mostrados = 0;

    for (i = 0; i < totalEquipamentos; i++)
    {
        if (!podeVerEquipamento(i))
            continue;
        if (soAtivos == 1 && strcmp(equipamentos[i].status, "ativo") != 0)
            continue;

        nomeDoCliente(equipamentos[i].id_cliente, dono);
        if (!contemTexto(dono, filtro) && !contemTexto(equipamentos[i].tipo, filtro) &&
            !contemTexto(equipamentos[i].marca, filtro) && !contemTexto(equipamentos[i].modelo_numero_serie, filtro))
            continue;

        /* Cabeçalho da tabela antes do primeiro da lista */
        if (mostrados == 0)
        {
            printf("\n  %-4s %-14s %-8s %-14s %-10s %-15s %s\n", "ID", "Cliente", "Categ.", "Tipo", "Marca",
                   "Modelo/Serie", "Status");
            printf("  ----------------------------------------------------------------------------\n");
        }
        printf("  %-4d %-14.14s %-8s %-14.14s %-10.10s %-15.15s %s\n", equipamentos[i].id_equipamento, dono,
               equipamentos[i].categoria, equipamentos[i].tipo, equipamentos[i].marca,
               equipamentos[i].modelo_numero_serie, equipamentos[i].status);
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
    if (i < 0 || !podeVerEquipamento(i) || (soAtivos == 1 && strcmp(equipamentos[i].status, "ativo") != 0))
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return i;
}

/* UC20 - Cadastrar Equipamento: cadastra um hardware ou software de um cliente */
void cadastrarEquipamento()
{
    int i, n, iCliente, categoria, maiorId = 0;

    imprimirCabecalho("Equipamentos > Cadastrar");

    if (totalEquipamentos >= MAX_EQUIPAMENTOS)
    {
        printf("\n[!] Limite de %d equipamentos atingido.", MAX_EQUIPAMENTOS);
        pausar();
        return;
    }

    /* Vinculação do equipamento a um cliente ativo (RN01, RN14) */
    printf("\nCliente proprietario:");
    iCliente = selecionarUsuario("cliente", 1);
    if (iCliente < 0)
        return;

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

    /* Preenche a próxima posição livre; só conta o equipamento se tudo der certo */
    n = totalEquipamentos;
    lerTexto("Tipo (ex: Notebook, Sistema Operacional): ", equipamentos[n].tipo, sizeof(equipamentos[n].tipo));
    lerTexto("Marca: ", equipamentos[n].marca, sizeof(equipamentos[n].marca));
    if (categoria == 1)
    {
        strcpy(equipamentos[n].categoria, "hardware");
        lerTexto("Modelo / Numero de Serie: ", equipamentos[n].modelo_numero_serie, sizeof(equipamentos[n].modelo_numero_serie));
    }
    else
    {
        strcpy(equipamentos[n].categoria, "software");
        lerTexto("Chave de Licenca: ", equipamentos[n].modelo_numero_serie, sizeof(equipamentos[n].modelo_numero_serie));
    }

    /* Validação de campos obrigatórios (RNF09) */
    if (!campoPreenchido(equipamentos[n].tipo, "Tipo") || !campoPreenchido(equipamentos[n].marca, "Marca") ||
        !campoPreenchido(equipamentos[n].modelo_numero_serie, "Modelo / Numero de Serie / Chave"))
        return;

    for (i = 0; i < totalEquipamentos; i++)
    {
        if (equipamentos[i].id_equipamento > maiorId)
            maiorId = equipamentos[i].id_equipamento;
    }
    equipamentos[n].id_equipamento = maiorId + 1;
    equipamentos[n].id_cliente = usuarios[iCliente].id_usuario;
    strcpy(equipamentos[n].status, "ativo");
    dataHoraAgora(equipamentos[n].criado_em);
    strcpy(equipamentos[n].atualizado_em, equipamentos[n].criado_em);

    totalEquipamentos++;
    if (!salvarEquipamentos())
    {
        totalEquipamentos--; /* não gravou: desfaz o cadastro */
        pausar();
        return;
    }

    printf("\n[+] Equipamento cadastrado e vinculado a %s! ID: %d", usuarios[iCliente].nome, equipamentos[n].id_equipamento);
    pausar();
}

/* UC21 - Consultar Equipamento: lista e busca os equipamentos que o perfil pode ver */
void consultarEquipamento()
{
    char filtro[60];

    imprimirCabecalho("Equipamentos > Consultar");

    lerTexto("\nFiltro (cliente, tipo, marca ou numero de serie), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return;

    /* Ativos e inativos aparecem, com o status, para preservar o histórico */
    if (listarEquipamentos(filtro, 0) == 0)
    {
        printf("\n[!] Nenhum equipamento encontrado para o filtro informado.\n");
    }
    pausar();
}

/* UC22 - Editar Equipamento: altera tipo, marca e modelo/numero de serie */
void editarEquipamento()
{
    char tipo[40], marca[40], serie[60], dono[60];
    int i;

    imprimirCabecalho("Equipamentos > Editar");

    i = selecionarEquipamento(0);
    if (i < 0)
        return;

    nomeDoCliente(equipamentos[i].id_cliente, dono);
    printf("\nEquipamento de %s (%s). Deixe em branco para manter o valor atual.\n\n", dono, equipamentos[i].categoria);
    lerOuManter("Tipo", equipamentos[i].tipo, tipo, sizeof(tipo));
    lerOuManter("Marca", equipamentos[i].marca, marca, sizeof(marca));
    if (strcmp(equipamentos[i].categoria, "software") == 0)
        lerOuManter("Chave de Licenca", equipamentos[i].modelo_numero_serie, serie, sizeof(serie));
    else
        lerOuManter("Modelo / Numero de Serie", equipamentos[i].modelo_numero_serie, serie, sizeof(serie));

    strcpy(equipamentos[i].tipo, tipo);
    strcpy(equipamentos[i].marca, marca);
    strcpy(equipamentos[i].modelo_numero_serie, serie);
    dataHoraAgora(equipamentos[i].atualizado_em);
    if (!salvarEquipamentos())
    {
        pausar();
        return;
    }

    printf("\n[+] Equipamento ID %d atualizado com sucesso!", equipamentos[i].id_equipamento);
    pausar();
}

/* UC23 - Inativar Equipamento: inativa um equipamento sem OS em andamento */
void inativarEquipamento()
{
    int i;

    imprimirCabecalho("Equipamentos > Inativar");

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

    printf("\nInativar o equipamento ID %d (%s %s).", equipamentos[i].id_equipamento, equipamentos[i].tipo,
           equipamentos[i].marca);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.");
        pausar();
        return;
    }

    /* Inativação preservando o histórico (RN12) */
    strcpy(equipamentos[i].status, "inativo");
    dataHoraAgora(equipamentos[i].atualizado_em);
    if (!salvarEquipamentos())
    {
        pausar();
        return;
    }

    printf("\n[+] Equipamento inativado. O historico de OS foi preservado e ele nao recebe novas OS.");
    pausar();
}

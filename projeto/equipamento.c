/* Quantas posições do vetor já estão gravadas no banco. Cadastro novo entra sempre no fim do vetor */
int equipamentosNoBanco = 0;

/* Grava o vetor na tabela equipamento: UPDATE do que já está no banco, INSERT do que é novo */
int salvarEquipamentos()
{
    char sql[1500];
    char categoria[30], tipo[90], marca[90], modelo[130], status[20], criado[50], atualizado[50];
    int i, ok = 1;

    /* deixa gravar o id que o programa escolheu, em vez de o banco gerar outro */
    bdExecutar("SET IDENTITY_INSERT equipamento ON");
    for (i = 0; i < totalEquipamentos && ok; i++)
    {
        sqlTexto(categoria, equipamentos[i].categoria);
        sqlTexto(tipo, equipamentos[i].tipo);
        sqlTexto(marca, equipamentos[i].marca);
        sqlTexto(modelo, equipamentos[i].modelo_numero_serie);
        sqlTexto(status, equipamentos[i].status);
        sqlTexto(criado, equipamentos[i].criado_em);
        sqlTexto(atualizado, equipamentos[i].atualizado_em);

        if (i < equipamentosNoBanco)
        {
            sprintf(sql, "UPDATE equipamento SET id_cliente = %d, categoria = %s, tipo = %s, marca = %s, "
                         "modelo_numero_serie = %s, status = %s, criado_em = %s, atualizado_em = %s WHERE id_equipamento = %d",
                    equipamentos[i].id_cliente, categoria, tipo, marca, modelo, status, criado, atualizado,
                    equipamentos[i].id_equipamento);
        }
        else
        {
            sprintf(sql, "INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, "
                         "status, criado_em, atualizado_em) VALUES (%d, %d, %s, %s, %s, %s, %s, %s, %s)",
                    equipamentos[i].id_equipamento, equipamentos[i].id_cliente, categoria, tipo, marca, modelo,
                    status, criado, atualizado);
        }
        ok = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT equipamento OFF");

    if (ok)
        equipamentosNoBanco = totalEquipamentos;
    return ok;
}

/* Lê a tabela equipamento para o vetor */
void carregarEquipamentos()
{
    char numero[20];
    int n;

    totalEquipamentos = 0;
    equipamentosNoBanco = 0;
    if (!bdConsultar("SELECT id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, "
                     "CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                     "FROM equipamento ORDER BY id_equipamento"))
    {
        return;
    }
    while (totalEquipamentos < MAX_EQUIPAMENTOS && bdProximaLinha())
    {
        n = totalEquipamentos;
        bdLerTexto(1, numero, sizeof(numero));
        equipamentos[n].id_equipamento = atoi(numero);
        bdLerTexto(2, numero, sizeof(numero));
        equipamentos[n].id_cliente = atoi(numero);
        bdLerTexto(3, equipamentos[n].categoria, sizeof(equipamentos[n].categoria));
        bdLerTexto(4, equipamentos[n].tipo, sizeof(equipamentos[n].tipo));
        bdLerTexto(5, equipamentos[n].marca, sizeof(equipamentos[n].marca));
        bdLerTexto(6, equipamentos[n].modelo_numero_serie, sizeof(equipamentos[n].modelo_numero_serie));
        bdLerTexto(7, equipamentos[n].status, sizeof(equipamentos[n].status));
        bdLerTexto(8, equipamentos[n].criado_em, sizeof(equipamentos[n].criado_em));
        bdLerTexto(9, equipamentos[n].atualizado_em, sizeof(equipamentos[n].atualizado_em));
        totalEquipamentos++;
    }
    equipamentosNoBanco = totalEquipamentos;
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
            if (ordens[j].id_equipamento == equipamentos[i].id_equipamento && buscarTecnicoNaOS(idLogado, ordens[j].id_os) >= 0)
                return 1;
        }
        return 0;
    }
    return 1;
}

/* Copia para destino o nome do cliente com este id */
void nomeDoCliente(int idCliente, char destino[])
{
    int i = buscarUsuarioPorId(idCliente);

    if (i >= 0)
        strcpy(destino, usuarios[i].nome);
    else
        strcpy(destino, "?");
}

/* Lista os equipamentos visíveis cujo cliente, tipo, marca ou número de série contém o filtro; devolve quantos mostrou.
   status: "ativo" ou "inativo" para listar só esses, ou "" para listar todos */
int listarEquipamentos(char filtro[], char status[])
{
    char dono[60];
    int i, mostrados = 0;

    for (i = 0; i < totalEquipamentos; i++)
    {
        if (!podeVerEquipamento(i))
            continue;
        if (strlen(status) > 0 && strcmp(equipamentos[i].status, status) != 0)
            continue;

        nomeDoCliente(equipamentos[i].id_cliente, dono);
        if (strstr(dono, filtro) == NULL && strstr(equipamentos[i].tipo, filtro) == NULL &&
            strstr(equipamentos[i].marca, filtro) == NULL && strstr(equipamentos[i].modelo_numero_serie, filtro) == NULL)
            continue;

        printf("ID %d - %s - %s - %s %s - %s - %s\n", equipamentos[i].id_equipamento, dono, equipamentos[i].categoria,
               equipamentos[i].tipo, equipamentos[i].marca, equipamentos[i].modelo_numero_serie, equipamentos[i].status);
        mostrados++;
    }
    return mostrados;
}

/* Pede um filtro, lista e pede o ID. Devolve a posição no vetor ou -1 (voltou ou ID inválido) */
int selecionarEquipamento(char status[])
{
    char filtro[60];
    int id, i;

    lerTexto("\nFiltro (cliente, tipo, marca ou numero de serie), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return -1;

    if (listarEquipamentos(filtro, status) == 0)
    {
        printf("\n[!] Nenhum equipamento encontrado para o filtro informado.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nID do equipamento (0 para voltar): ");
    if (id <= 0)
        return -1;

    i = buscarEquipamentoPorId(id);
    if (i < 0 || !podeVerEquipamento(i) || (strlen(status) > 0 && strcmp(equipamentos[i].status, status) != 0))
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
    int n, iCliente, categoria;

    imprimirCabecalho("Equipamentos > Cadastrar");

    if (totalEquipamentos >= MAX_EQUIPAMENTOS)
    {
        printf("\n[!] Limite de %d equipamentos atingido.", MAX_EQUIPAMENTOS);
        pausar();
        return;
    }

    /* O equipamento é de um cliente ativo (RN01, RN14) */
    printf("\nCliente proprietario:");
    iCliente = selecionarUsuario("cliente", "ativo");
    if (iCliente < 0)
        return;

    printf("\nCategoria do equipamento:\n");
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

    /* Campos obrigatórios (RNF09) */
    if (!campoPreenchido(equipamentos[n].tipo, "Tipo") || !campoPreenchido(equipamentos[n].marca, "Marca") ||
        !campoPreenchido(equipamentos[n].modelo_numero_serie, "Modelo / Numero de Serie / Chave"))
        return;

    /* O vetor está em ordem de id: o novo id é o id do último + 1 */
    equipamentos[n].id_equipamento = 1;
    if (n > 0)
        equipamentos[n].id_equipamento = equipamentos[n - 1].id_equipamento + 1;
    equipamentos[n].id_cliente = usuarios[iCliente].id_usuario;
    strcpy(equipamentos[n].status, "ativo");
    dataHoraAgora(equipamentos[n].criado_em);
    strcpy(equipamentos[n].atualizado_em, equipamentos[n].criado_em);

    totalEquipamentos++;
    if (salvarEquipamentos())
        printf("\n[+] Equipamento cadastrado e vinculado a %s! ID: %d", usuarios[iCliente].nome, equipamentos[n].id_equipamento);
    else
        totalEquipamentos--; /* não gravou: desfaz o cadastro */
    pausar();
}

/* UC21 - Consultar Equipamento: lista e busca os equipamentos que o perfil pode ver */
void consultarEquipamento()
{
    char filtro[60];
    int mostrados;

    imprimirCabecalho("Equipamentos > Consultar");

    /* Pede o filtro de novo enquanto nenhum equipamento for encontrado (exceção 4a) */
    do
    {
        lerTexto("\nFiltro (cliente, tipo, marca ou numero de serie), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;

        /* Ativos e inativos aparecem, com o status, para preservar o histórico */
        mostrados = listarEquipamentos(filtro, "");
        if (mostrados == 0)
            printf("\n[!] Nenhum equipamento encontrado para o filtro informado. Tente outro filtro.\n");
    } while (mostrados == 0);
    pausar();
}

/* UC22 - Editar Equipamento: altera tipo, marca e modelo/numero de serie */
void editarEquipamento()
{
    char tipo[40], marca[40], serie[60];
    int i;

    imprimirCabecalho("Equipamentos > Editar");

    i = selecionarEquipamento("");
    if (i < 0)
        return;

    printf("\nDeixe em branco para manter o valor atual.\n");
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
    if (salvarEquipamentos())
        printf("\n[+] Equipamento ID %d atualizado com sucesso!", equipamentos[i].id_equipamento);
    pausar();
}

/* UC23 - Inativar Equipamento: inativa um equipamento sem OS em andamento */
void inativarEquipamento()
{
    int i;

    imprimirCabecalho("Equipamentos > Inativar");

    /* Só os equipamentos ativos podem ser inativados */
    i = selecionarEquipamento("ativo");
    if (i < 0)
        return;

    /* RN14: bloqueia se o equipamento tem OS aberta ou em andamento */
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

    /* Inativa sem apagar, preservando o histórico (RN12) */
    strcpy(equipamentos[i].status, "inativo");
    dataHoraAgora(equipamentos[i].atualizado_em);
    if (salvarEquipamentos())
        printf("\n[+] Equipamento inativado. O historico de OS foi preservado e ele nao recebe novas OS.");
    pausar();
}

/* UC49 - Reativar Equipamento: volta um equipamento inativo para ativo (RF03) */
void reativarEquipamento()
{
    int i, c, inativos = 0;

    imprimirCabecalho("Equipamentos > Reativar");

    /* Exceção 2a: nenhum equipamento inativo */
    for (i = 0; i < totalEquipamentos; i++)
    {
        if (strcmp(equipamentos[i].status, "inativo") == 0)
            inativos++;
    }
    if (inativos == 0)
    {
        printf("\n[!] Nenhum equipamento inativo. Nao ha cadastro para reativar.");
        pausar();
        return;
    }

    /* Só os equipamentos inativos aparecem na lista */
    i = selecionarEquipamento("inativo");
    if (i < 0)
        return;

    /* RN01 e RN14: o equipamento só volta se o cliente dono estiver ativo */
    c = buscarUsuarioPorId(equipamentos[i].id_cliente);
    if (strcmp(usuarios[c].status, "ativo") != 0)
    {
        printf("\n[!] O cliente %s esta inativo. Reative o cliente antes do equipamento (RN14).", usuarios[c].nome);
        pausar();
        return;
    }

    printf("\nReativar o equipamento ID %d (%s %s).", equipamentos[i].id_equipamento, equipamentos[i].tipo,
           equipamentos[i].marca);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao cancelada. O equipamento continua inativo.");
        pausar();
        return;
    }

    strcpy(equipamentos[i].status, "ativo");
    dataHoraAgora(equipamentos[i].atualizado_em);
    if (salvarEquipamentos())
        printf("\n[+] Equipamento reativado. Ele volta a poder receber novas OS.");
    pausar();
}

/* equipamento.c - equipamentos dos clientes: cadastrar, consultar, editar, inativar e reativar */
/* Funções do main.c usadas aqui: lerTexto (linha 121), lerInteiro (linha 137), confirmar (linha 161), pausar (linha 173), imprimirCabecalho (linha 215), dataHoraAgora (linha 204) */

/* Quantos equipamentos do vetor já estão no banco (os novos ficam no fim do vetor) */
int equipamentosNoBanco = 0;

/* Grava o vetor na tabela equipamento: UPDATE do que já existe, INSERT do que é novo; devolve 1 se deu certo */
int salvarEquipamentos()
{
    char sql[1500];
    char categoria[30], tipo[90], marca[90], modelo[130], status[20], criado[50], atualizado[50];
    int posicaoEquipamento, deuCerto = 1;

    /* bdExecutar está em banco.c, linha 61 */
    bdExecutar("SET IDENTITY_INSERT equipamento ON"); /* deixa o programa escolher o id */
    for (posicaoEquipamento = 0; posicaoEquipamento < totalEquipamentos && deuCerto; posicaoEquipamento++)
    {
        /* sqlTexto está em banco.c, linha 100 */
        sqlTexto(categoria, equipamentos[posicaoEquipamento].categoria);
        sqlTexto(tipo, equipamentos[posicaoEquipamento].tipo);
        sqlTexto(marca, equipamentos[posicaoEquipamento].marca);
        sqlTexto(modelo, equipamentos[posicaoEquipamento].modelo_numero_serie);
        sqlTexto(status, equipamentos[posicaoEquipamento].status);
        sqlTexto(criado, equipamentos[posicaoEquipamento].criado_em);
        sqlTexto(atualizado, equipamentos[posicaoEquipamento].atualizado_em);

        if (posicaoEquipamento < equipamentosNoBanco)
        {
            sprintf(sql, "UPDATE equipamento SET id_cliente = %d, categoria = %s, tipo = %s, marca = %s, "
                         "modelo_numero_serie = %s, status = %s, criado_em = %s, atualizado_em = %s WHERE id_equipamento = %d",
                    equipamentos[posicaoEquipamento].id_cliente, categoria, tipo, marca, modelo, status, criado, atualizado,
                    equipamentos[posicaoEquipamento].id_equipamento);
        }
        else
        {
            sprintf(sql, "INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, "
                         "status, criado_em, atualizado_em) VALUES (%d, %d, %s, %s, %s, %s, %s, %s, %s)",
                    equipamentos[posicaoEquipamento].id_equipamento, equipamentos[posicaoEquipamento].id_cliente, categoria,
                    tipo, marca, modelo, status, criado, atualizado);
        }
        deuCerto = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT equipamento OFF");

    if (deuCerto)
        equipamentosNoBanco = totalEquipamentos;
    return deuCerto;
}

/* Lê a tabela equipamento para o vetor */
void carregarEquipamentos()
{
    char numero[20];
    int novaPosicao;

    totalEquipamentos = 0;
    equipamentosNoBanco = 0;
    /* bdExecutar está em banco.c, linha 61 */
    if (!bdExecutar("SELECT id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, "
                    "CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                    "FROM equipamento ORDER BY id_equipamento"))
    {
        return;
    }

    /* Cada linha do SELECT vai para a próxima posição do vetor */
    /* bdProximaLinha está em banco.c, linha 80 */
    while (totalEquipamentos < MAX_EQUIPAMENTOS && bdProximaLinha())
    {
        novaPosicao = totalEquipamentos;
        /* bdLerTexto está em banco.c, linha 88 */
        bdLerTexto(1, numero, sizeof(numero));
        equipamentos[novaPosicao].id_equipamento = atoi(numero);
        bdLerTexto(2, numero, sizeof(numero));
        equipamentos[novaPosicao].id_cliente = atoi(numero);
        bdLerTexto(3, equipamentos[novaPosicao].categoria, sizeof(equipamentos[novaPosicao].categoria));
        bdLerTexto(4, equipamentos[novaPosicao].tipo, sizeof(equipamentos[novaPosicao].tipo));
        bdLerTexto(5, equipamentos[novaPosicao].marca, sizeof(equipamentos[novaPosicao].marca));
        bdLerTexto(6, equipamentos[novaPosicao].modelo_numero_serie, sizeof(equipamentos[novaPosicao].modelo_numero_serie));
        bdLerTexto(7, equipamentos[novaPosicao].status, sizeof(equipamentos[novaPosicao].status));
        bdLerTexto(8, equipamentos[novaPosicao].criado_em, sizeof(equipamentos[novaPosicao].criado_em));
        bdLerTexto(9, equipamentos[novaPosicao].atualizado_em, sizeof(equipamentos[novaPosicao].atualizado_em));
        totalEquipamentos++;
    }
    equipamentosNoBanco = totalEquipamentos;
}

/* Devolve a posição no vetor do equipamento com este id, ou -1 se não existir */
int buscarEquipamentoPorId(int id)
{
    int posicaoEquipamento;

    for (posicaoEquipamento = 0; posicaoEquipamento < totalEquipamentos; posicaoEquipamento++)
    {
        if (equipamentos[posicaoEquipamento].id_equipamento == id)
            return posicaoEquipamento;
    }
    return -1;
}

/* RN14: devolve 1 se o equipamento tem OS aberta ou em andamento */
int equipamentoTemOSEmAndamento(int idEquipamento)
{
    int posicaoOS;

    for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
    {
        if (ordens[posicaoOS].id_equipamento != idEquipamento)
            continue;
        if (strcmp(ordens[posicaoOS].status, "aberta") == 0 || strcmp(ordens[posicaoOS].status, "em_andamento") == 0)
            return 1;
    }
    return 0;
}

/* RN10: cliente vê só os dele; técnico só os das OS dele; admin e recepcionista veem todos */
int podeVerEquipamento(int posicaoEquipamento)
{
    int posicaoOS, idLogado = usuarios[usuarioLogado].id_usuario;

    /* perfilLogado está em geral.c, linha 5 */
    if (perfilLogado("cliente"))
    {
        if (equipamentos[posicaoEquipamento].id_cliente == idLogado)
            return 1;
        return 0;
    }
    if (perfilLogado("tecnico"))
    {
        for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
        {
            /* buscarTecnicoNaOS está em geral.c, linha 187 */
            if (ordens[posicaoOS].id_equipamento == equipamentos[posicaoEquipamento].id_equipamento &&
                buscarTecnicoNaOS(idLogado, ordens[posicaoOS].id_os) >= 0)
                return 1;
        }
        return 0;
    }
    return 1;
}

/* Copia para destino o nome do cliente com este id ("?" se não existir) */
void nomeDoCliente(int idCliente, char destino[])
{
    /* buscarUsuarioPorId está em geral.c, linha 123 */
    int posicaoCliente = buscarUsuarioPorId(idCliente);

    if (posicaoCliente >= 0)
        strcpy(destino, usuarios[posicaoCliente].nome);
    else
        strcpy(destino, "?");
}

/* Devolve 1 se o equipamento entra na lista: o logado pode ver e o status bate ("" aceita todos) */
int equipamentoEntraNaLista(int posicaoEquipamento, char status[])
{
    /* podeVerEquipamento está em equipamento.c, linha 117 */
    if (!podeVerEquipamento(posicaoEquipamento))
        return 0;
    if (strlen(status) > 0 && strcmp(equipamentos[posicaoEquipamento].status, status) != 0)
        return 0;
    return 1;
}

/* Lista os equipamentos que têm o filtro no cliente, tipo, marca ou série; devolve quantos mostrou */
int listarEquipamentos(char filtro[], char status[])
{
    char dono[60];
    int posicaoEquipamento, mostrados = 0;

    for (posicaoEquipamento = 0; posicaoEquipamento < totalEquipamentos; posicaoEquipamento++)
    {
        /* equipamentoEntraNaLista está em equipamento.c, linha 155 */
        if (!equipamentoEntraNaLista(posicaoEquipamento, status))
            continue;

        /* nomeDoCliente está em equipamento.c, linha 143 */
        nomeDoCliente(equipamentos[posicaoEquipamento].id_cliente, dono);
        if (strstr(dono, filtro) == NULL &&
            strstr(equipamentos[posicaoEquipamento].tipo, filtro) == NULL &&
            strstr(equipamentos[posicaoEquipamento].marca, filtro) == NULL &&
            strstr(equipamentos[posicaoEquipamento].modelo_numero_serie, filtro) == NULL)
            continue;

        printf("ID %d - %s - %s - %s %s - %s - %s\n", equipamentos[posicaoEquipamento].id_equipamento, dono,
               equipamentos[posicaoEquipamento].categoria, equipamentos[posicaoEquipamento].tipo,
               equipamentos[posicaoEquipamento].marca, equipamentos[posicaoEquipamento].modelo_numero_serie,
               equipamentos[posicaoEquipamento].status);
        mostrados++;
    }
    return mostrados;
}

/* Pede um filtro, lista e pede o ID; devolve a posição do escolhido ou -1 */
int selecionarEquipamento(char status[])
{
    char filtro[60];
    int id, posicaoEquipamento;

    lerTexto("\nFiltro (cliente, tipo, marca ou numero de serie), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return -1;

    /* listarEquipamentos está em equipamento.c, linha 166 */
    if (listarEquipamentos(filtro, status) == 0)
    {
        printf("\n[!] Nenhum equipamento encontrado para o filtro informado.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nID do equipamento (0 para voltar): ");
    if (id <= 0)
        return -1;

    /* buscarEquipamentoPorId está em equipamento.c, linha 89 */
    posicaoEquipamento = buscarEquipamentoPorId(id);
    if (posicaoEquipamento < 0)
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    /* equipamentoEntraNaLista está em equipamento.c, linha 155 */
    if (!equipamentoEntraNaLista(posicaoEquipamento, status))
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return posicaoEquipamento;
}

/* UC20 - Cadastrar Equipamento: hardware ou software de um cliente ativo */
void cadastrarEquipamento()
{
    int novaPosicao, posicaoCliente, categoria;

    imprimirCabecalho("Equipamentos > Cadastrar");

    if (totalEquipamentos >= MAX_EQUIPAMENTOS)
    {
        printf("\n[!] Limite de %d equipamentos atingido.", MAX_EQUIPAMENTOS);
        pausar();
        return;
    }

    /* O dono precisa ser um cliente ativo (RN01, RN14) */
    printf("\nCliente proprietario:");
    /* selecionarUsuario está em geral.c, linha 283 */
    posicaoCliente = selecionarUsuario("cliente", "ativo");
    if (posicaoCliente < 0)
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

    /* Preenche a próxima posição livre; o total só aumenta se tudo der certo */
    novaPosicao = totalEquipamentos;
    lerTexto("Tipo (ex: Notebook, Sistema Operacional): ", equipamentos[novaPosicao].tipo, sizeof(equipamentos[novaPosicao].tipo));
    lerTexto("Marca: ", equipamentos[novaPosicao].marca, sizeof(equipamentos[novaPosicao].marca));
    if (categoria == 1)
    {
        strcpy(equipamentos[novaPosicao].categoria, "hardware");
        lerTexto("Modelo / Numero de Serie: ", equipamentos[novaPosicao].modelo_numero_serie,
                 sizeof(equipamentos[novaPosicao].modelo_numero_serie));
    }
    else
    {
        strcpy(equipamentos[novaPosicao].categoria, "software");
        lerTexto("Chave de Licenca: ", equipamentos[novaPosicao].modelo_numero_serie,
                 sizeof(equipamentos[novaPosicao].modelo_numero_serie));
    }

    /* Campos obrigatórios (RNF09) */
    /* campoPreenchido está em geral.c, linha 162 */
    if (!campoPreenchido(equipamentos[novaPosicao].tipo, "Tipo"))
        return;
    if (!campoPreenchido(equipamentos[novaPosicao].marca, "Marca"))
        return;
    if (!campoPreenchido(equipamentos[novaPosicao].modelo_numero_serie, "Modelo / Numero de Serie / Chave"))
        return;

    /* O novo id é o id do último + 1 (ou 1 se for o primeiro) */
    equipamentos[novaPosicao].id_equipamento = 1;
    if (novaPosicao > 0)
        equipamentos[novaPosicao].id_equipamento = equipamentos[novaPosicao - 1].id_equipamento + 1;
    equipamentos[novaPosicao].id_cliente = usuarios[posicaoCliente].id_usuario;
    strcpy(equipamentos[novaPosicao].status, "ativo");
    dataHoraAgora(equipamentos[novaPosicao].criado_em);
    strcpy(equipamentos[novaPosicao].atualizado_em, equipamentos[novaPosicao].criado_em);

    totalEquipamentos++;
    /* salvarEquipamentos está em equipamento.c, linha 8 */
    if (salvarEquipamentos())
        printf("\n[+] Equipamento cadastrado e vinculado a %s! ID: %d", usuarios[posicaoCliente].nome,
               equipamentos[novaPosicao].id_equipamento);
    else
        totalEquipamentos--; /* não gravou: desfaz */
    pausar();
}

/* UC21 - Consultar Equipamento: lista os equipamentos que o perfil pode ver */
void consultarEquipamento()
{
    char filtro[60];
    int mostrados;

    imprimirCabecalho("Equipamentos > Consultar");

    /* Pede outro filtro enquanto nada for encontrado (exceção 4a) */
    do
    {
        lerTexto("\nFiltro (cliente, tipo, marca ou numero de serie), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;

        /* listarEquipamentos está em equipamento.c, linha 166 */
        mostrados = listarEquipamentos(filtro, "");
        if (mostrados == 0)
            printf("\n[!] Nenhum equipamento encontrado para o filtro informado. Tente outro filtro.\n");
    } while (mostrados == 0);
    pausar();
}

/* UC22 - Editar Equipamento: altera tipo, marca e modelo/número de série */
void editarEquipamento()
{
    char tipo[40], marca[40], serie[60];
    int posicaoEquipamento;

    imprimirCabecalho("Equipamentos > Editar");

    /* selecionarEquipamento está em equipamento.c, linha 195 */
    posicaoEquipamento = selecionarEquipamento("");
    if (posicaoEquipamento < 0)
        return;

    printf("\nDeixe em branco para manter o valor atual.\n");
    /* lerOuManter está em geral.c, linha 323 */
    lerOuManter("Tipo", equipamentos[posicaoEquipamento].tipo, tipo, sizeof(tipo));
    lerOuManter("Marca", equipamentos[posicaoEquipamento].marca, marca, sizeof(marca));
    if (strcmp(equipamentos[posicaoEquipamento].categoria, "software") == 0)
        lerOuManter("Chave de Licenca", equipamentos[posicaoEquipamento].modelo_numero_serie, serie, sizeof(serie));
    else
        lerOuManter("Modelo / Numero de Serie", equipamentos[posicaoEquipamento].modelo_numero_serie, serie, sizeof(serie));

    strcpy(equipamentos[posicaoEquipamento].tipo, tipo);
    strcpy(equipamentos[posicaoEquipamento].marca, marca);
    strcpy(equipamentos[posicaoEquipamento].modelo_numero_serie, serie);
    dataHoraAgora(equipamentos[posicaoEquipamento].atualizado_em);
    /* salvarEquipamentos está em equipamento.c, linha 8 */
    if (salvarEquipamentos())
        printf("\n[+] Equipamento ID %d atualizado com sucesso!", equipamentos[posicaoEquipamento].id_equipamento);
    pausar();
}

/* UC23 - Inativar Equipamento: não apaga (RN12) e bloqueia se tiver OS aberta (RN14) */
void inativarEquipamento()
{
    int posicaoEquipamento;

    imprimirCabecalho("Equipamentos > Inativar");

    /* selecionarEquipamento está em equipamento.c, linha 195 */
    posicaoEquipamento = selecionarEquipamento("ativo");
    if (posicaoEquipamento < 0)
        return;

    /* equipamentoTemOSEmAndamento está em equipamento.c, linha 102 */
    if (equipamentoTemOSEmAndamento(equipamentos[posicaoEquipamento].id_equipamento))
    {
        printf("\n[!] O equipamento ID %d tem ordem de servico aberta ou em andamento. Inativacao bloqueada (RN14).",
               equipamentos[posicaoEquipamento].id_equipamento);
        pausar();
        return;
    }

    printf("\nInativar o equipamento ID %d (%s %s).", equipamentos[posicaoEquipamento].id_equipamento,
           equipamentos[posicaoEquipamento].tipo, equipamentos[posicaoEquipamento].marca);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.");
        pausar();
        return;
    }

    strcpy(equipamentos[posicaoEquipamento].status, "inativo");
    dataHoraAgora(equipamentos[posicaoEquipamento].atualizado_em);
    /* salvarEquipamentos está em equipamento.c, linha 8 */
    if (salvarEquipamentos())
        printf("\n[+] Equipamento inativado. O historico de OS foi preservado e ele nao recebe novas OS.");
    pausar();
}

/* UC49 - Reativar Equipamento: só se o cliente dono estiver ativo */
void reativarEquipamento()
{
    int posicaoEquipamento, posicaoCliente, inativos = 0;

    imprimirCabecalho("Equipamentos > Reativar");

    /* Conta os equipamentos inativos (exceção 2a: nenhum) */
    for (posicaoEquipamento = 0; posicaoEquipamento < totalEquipamentos; posicaoEquipamento++)
    {
        if (strcmp(equipamentos[posicaoEquipamento].status, "inativo") == 0)
            inativos++;
    }
    if (inativos == 0)
    {
        printf("\n[!] Nenhum equipamento inativo. Nao ha cadastro para reativar.");
        pausar();
        return;
    }

    /* selecionarEquipamento está em equipamento.c, linha 195 */
    posicaoEquipamento = selecionarEquipamento("inativo");
    if (posicaoEquipamento < 0)
        return;

    /* RN01 e RN14: o dono precisa estar ativo */
    /* buscarUsuarioPorId está em geral.c, linha 123 */
    posicaoCliente = buscarUsuarioPorId(equipamentos[posicaoEquipamento].id_cliente);
    if (strcmp(usuarios[posicaoCliente].status, "ativo") != 0)
    {
        printf("\n[!] O cliente %s esta inativo. Reative o cliente antes do equipamento (RN14).", usuarios[posicaoCliente].nome);
        pausar();
        return;
    }

    printf("\nReativar o equipamento ID %d (%s %s).", equipamentos[posicaoEquipamento].id_equipamento,
           equipamentos[posicaoEquipamento].tipo, equipamentos[posicaoEquipamento].marca);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao cancelada. O equipamento continua inativo.");
        pausar();
        return;
    }

    strcpy(equipamentos[posicaoEquipamento].status, "ativo");
    dataHoraAgora(equipamentos[posicaoEquipamento].atualizado_em);
    /* salvarEquipamentos está em equipamento.c, linha 8 */
    if (salvarEquipamentos())
        printf("\n[+] Equipamento reativado. Ele volta a poder receber novas OS.");
    pausar();
}

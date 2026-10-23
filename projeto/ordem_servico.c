/* Funções do main.c usadas aqui: lerTexto (linha 121), lerInteiro (linha 137), lerValor (linha 149), confirmar (linha 161), pausar (linha 173), imprimirCabecalho (linha 214), dataHoje (linha 193), dataHoraAgora (linha 204) */
/* ordem_servico.c - ordens de serviço (tabela os) e técnicos de cada OS (tabela tecnico_os) */

/* Quantas OS e atribuições do vetor já estão no banco (as novas ficam no fim do vetor) */
int ordensNoBanco = 0;
int tecnicosOSNoBanco = 0;

/* Grava o vetor na tabela os: UPDATE do que já existe, INSERT do que é novo; devolve 1 se deu certo */
int salvarOrdens()
{
    char sql[4000];
    char abertura[30], problema[410], diagnostico[410], orcamento[30], status[40], resposta[30];
    char entrega[30], teste[30], pago[30], feedback[410], satisfeito[20], criado[50], atualizado[50];
    int posicaoOS, deuCerto = 1;

    /* bdExecutar está em banco.c, linha 61 */
    bdExecutar("SET IDENTITY_INSERT os ON"); /* deixa o programa escolher o id */
    for (posicaoOS = 0; posicaoOS < totalOrdens && deuCerto; posicaoOS++)
    {
        /* sqlTexto está em banco.c, linha 100 */
        sqlTexto(abertura, ordens[posicaoOS].data_abertura);
        sqlTexto(problema, ordens[posicaoOS].problema_relatado);
        sqlTexto(diagnostico, ordens[posicaoOS].diagnostico);
        /* sqlDinheiro está em banco.c, linha 127 */
        sqlDinheiro(orcamento, ordens[posicaoOS].orcamento);
        sqlTexto(status, ordens[posicaoOS].status);
        sqlTexto(resposta, ordens[posicaoOS].resultado_resposta_cliente);
        sqlTexto(entrega, ordens[posicaoOS].data_entrega);
        sqlTexto(teste, ordens[posicaoOS].resultado_teste);
        sqlDinheiro(pago, ordens[posicaoOS].valor_pago);
        sqlTexto(feedback, ordens[posicaoOS].feedback_cliente);
        sqlTexto(satisfeito, ordens[posicaoOS].satisfeito);
        sqlTexto(criado, ordens[posicaoOS].criado_em);
        sqlTexto(atualizado, ordens[posicaoOS].atualizado_em);

        if (posicaoOS < ordensNoBanco)
        {
            sprintf(sql, "UPDATE os SET id_equipamento = %d, id_atendente = %d, data_abertura = %s, problema_relatado = %s, "
                         "diagnostico = %s, orcamento = %s, status = %s, resultado_resposta_cliente = %s, data_entrega = %s, "
                         "resultado_teste = %s, valor_pago = %s, feedback_cliente = %s, satisfeito = %s, criado_em = %s, "
                         "atualizado_em = %s WHERE id_os = %d",
                    ordens[posicaoOS].id_equipamento, ordens[posicaoOS].id_atendente, abertura, problema, diagnostico,
                    orcamento, status, resposta, entrega, teste, pago, feedback, satisfeito, criado, atualizado,
                    ordens[posicaoOS].id_os);
        }
        else
        {
            sprintf(sql, "INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, "
                         "orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, "
                         "feedback_cliente, satisfeito, criado_em, atualizado_em) "
                         "VALUES (%d, %d, %d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                    ordens[posicaoOS].id_os, ordens[posicaoOS].id_equipamento, ordens[posicaoOS].id_atendente, abertura,
                    problema, diagnostico, orcamento, status, resposta, entrega, teste, pago, feedback, satisfeito, criado,
                    atualizado);
        }
        deuCerto = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT os OFF");

    if (deuCerto)
        ordensNoBanco = totalOrdens;
    return deuCerto;
}

/* Lê a tabela os para o vetor; dinheiro NULL vira -1 (ainda não informado) */
void carregarOrdens()
{
    char numero[30];
    int novaPosicao;

    totalOrdens = 0;
    ordensNoBanco = 0;
    /* bdExecutar está em banco.c, linha 61 */
    if (!bdExecutar("SELECT id_os, id_equipamento, id_atendente, CONVERT(VARCHAR(10), data_abertura, 120), "
                    "problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, "
                    "CONVERT(VARCHAR(10), data_entrega, 120), resultado_teste, valor_pago, feedback_cliente, satisfeito, "
                    "CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                    "FROM os ORDER BY id_os"))
    {
        return;
    }

    /* bdProximaLinha está em banco.c, linha 80 */
    while (totalOrdens < MAX_OS && bdProximaLinha())
    {
        novaPosicao = totalOrdens;
        /* bdLerTexto está em banco.c, linha 88 */
        bdLerTexto(1, numero, sizeof(numero));
        ordens[novaPosicao].id_os = atoi(numero);
        bdLerTexto(2, numero, sizeof(numero));
        ordens[novaPosicao].id_equipamento = atoi(numero);
        bdLerTexto(3, numero, sizeof(numero));
        ordens[novaPosicao].id_atendente = atoi(numero);
        bdLerTexto(4, ordens[novaPosicao].data_abertura, sizeof(ordens[novaPosicao].data_abertura));
        bdLerTexto(5, ordens[novaPosicao].problema_relatado, sizeof(ordens[novaPosicao].problema_relatado));
        bdLerTexto(6, ordens[novaPosicao].diagnostico, sizeof(ordens[novaPosicao].diagnostico));
        bdLerTexto(7, numero, sizeof(numero));
        if (numero[0] == '\0')
            ordens[novaPosicao].orcamento = -1;
        else
            ordens[novaPosicao].orcamento = atof(numero);
        bdLerTexto(8, ordens[novaPosicao].status, sizeof(ordens[novaPosicao].status));
        bdLerTexto(9, ordens[novaPosicao].resultado_resposta_cliente, sizeof(ordens[novaPosicao].resultado_resposta_cliente));
        bdLerTexto(10, ordens[novaPosicao].data_entrega, sizeof(ordens[novaPosicao].data_entrega));
        bdLerTexto(11, ordens[novaPosicao].resultado_teste, sizeof(ordens[novaPosicao].resultado_teste));
        bdLerTexto(12, numero, sizeof(numero));
        if (numero[0] == '\0')
            ordens[novaPosicao].valor_pago = -1;
        else
            ordens[novaPosicao].valor_pago = atof(numero);
        bdLerTexto(13, ordens[novaPosicao].feedback_cliente, sizeof(ordens[novaPosicao].feedback_cliente));
        bdLerTexto(14, ordens[novaPosicao].satisfeito, sizeof(ordens[novaPosicao].satisfeito));
        bdLerTexto(15, ordens[novaPosicao].criado_em, sizeof(ordens[novaPosicao].criado_em));
        bdLerTexto(16, ordens[novaPosicao].atualizado_em, sizeof(ordens[novaPosicao].atualizado_em));
        totalOrdens++;
    }
    ordensNoBanco = totalOrdens;
}

/* Grava o vetor na tabela tecnico_os: UPDATE do que já existe, INSERT do que é novo; devolve 1 se deu certo */
int salvarTecnicosOS()
{
    char sql[1500];
    char atribuicao[30], observacoes[610], criado[50], atualizado[50];
    int posicaoAtribuicao, deuCerto = 1;

    /* bdExecutar está em banco.c, linha 61 */
    bdExecutar("SET IDENTITY_INSERT tecnico_os ON");
    for (posicaoAtribuicao = 0; posicaoAtribuicao < totalTecnicosOS && deuCerto; posicaoAtribuicao++)
    {
        /* sqlTexto está em banco.c, linha 100 */
        sqlTexto(atribuicao, tecnicosOS[posicaoAtribuicao].data_atribuicao);
        sqlTexto(observacoes, tecnicosOS[posicaoAtribuicao].observacoes_tecnicas);
        sqlTexto(criado, tecnicosOS[posicaoAtribuicao].criado_em);
        sqlTexto(atualizado, tecnicosOS[posicaoAtribuicao].atualizado_em);

        if (posicaoAtribuicao < tecnicosOSNoBanco)
        {
            sprintf(sql, "UPDATE tecnico_os SET id_tecnico = %d, id_os = %d, data_atribuicao = %s, observacoes_tecnicas = %s, "
                         "criado_em = %s, atualizado_em = %s WHERE id_tecnico_os = %d",
                    tecnicosOS[posicaoAtribuicao].id_tecnico, tecnicosOS[posicaoAtribuicao].id_os, atribuicao, observacoes,
                    criado, atualizado, tecnicosOS[posicaoAtribuicao].id_tecnico_os);
        }
        else
        {
            sprintf(sql, "INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, "
                         "criado_em, atualizado_em) VALUES (%d, %d, %d, %s, %s, %s, %s)",
                    tecnicosOS[posicaoAtribuicao].id_tecnico_os, tecnicosOS[posicaoAtribuicao].id_tecnico,
                    tecnicosOS[posicaoAtribuicao].id_os, atribuicao, observacoes, criado, atualizado);
        }
        deuCerto = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT tecnico_os OFF");

    if (deuCerto)
        tecnicosOSNoBanco = totalTecnicosOS;
    return deuCerto;
}

/* Lê a tabela tecnico_os para o vetor */
void carregarTecnicosOS()
{
    char numero[20];
    int novaPosicao;

    totalTecnicosOS = 0;
    tecnicosOSNoBanco = 0;
    /* bdExecutar está em banco.c, linha 61 */
    if (!bdExecutar("SELECT id_tecnico_os, id_tecnico, id_os, CONVERT(VARCHAR(10), data_atribuicao, 120), "
                    "observacoes_tecnicas, CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                    "FROM tecnico_os ORDER BY id_tecnico_os"))
    {
        return;
    }

    /* bdProximaLinha está em banco.c, linha 80 */
    while (totalTecnicosOS < MAX_TECNICO_OS && bdProximaLinha())
    {
        novaPosicao = totalTecnicosOS;
        /* bdLerTexto está em banco.c, linha 88 */
        bdLerTexto(1, numero, sizeof(numero));
        tecnicosOS[novaPosicao].id_tecnico_os = atoi(numero);
        bdLerTexto(2, numero, sizeof(numero));
        tecnicosOS[novaPosicao].id_tecnico = atoi(numero);
        bdLerTexto(3, numero, sizeof(numero));
        tecnicosOS[novaPosicao].id_os = atoi(numero);
        bdLerTexto(4, tecnicosOS[novaPosicao].data_atribuicao, sizeof(tecnicosOS[novaPosicao].data_atribuicao));
        bdLerTexto(5, tecnicosOS[novaPosicao].observacoes_tecnicas, sizeof(tecnicosOS[novaPosicao].observacoes_tecnicas));
        bdLerTexto(6, tecnicosOS[novaPosicao].criado_em, sizeof(tecnicosOS[novaPosicao].criado_em));
        bdLerTexto(7, tecnicosOS[novaPosicao].atualizado_em, sizeof(tecnicosOS[novaPosicao].atualizado_em));
        totalTecnicosOS++;
    }
    tecnicosOSNoBanco = totalTecnicosOS;
}

/* ---------- Funções de apoio às OS (também usadas em manutencao.c, orcamento.c e relatorio.c) ---------- */

/* Devolve a posição no vetor da OS com este número, ou -1 se não existir */
int buscarOSPorId(int id)
{
    int posicaoOS;

    for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
    {
        if (ordens[posicaoOS].id_os == id)
            return posicaoOS;
    }
    return -1;
}

/* RN10: cliente vê as OS dos equipamentos dele; técnico as OS em que está; admin e recepcionista veem todas */
int podeVerOS(int posicaoOS)
{
    int idLogado = usuarios[usuarioLogado].id_usuario;

    /* perfilLogado está em geral.c, linha 5 */
    if (perfilLogado("cliente"))
    {
        /* donoDoEquipamento está em geral.c, linha 174 */
        if (donoDoEquipamento(ordens[posicaoOS].id_equipamento) == idLogado)
            return 1;
        return 0;
    }
    if (perfilLogado("tecnico"))
    {
        /* buscarTecnicoNaOS está em geral.c, linha 187 */
        if (buscarTecnicoNaOS(idLogado, ordens[posicaoOS].id_os) >= 0)
            return 1;
        return 0;
    }
    return 1;
}

/* Copia para destino a etapa da OS, olhando da etapa mais adiantada para a mais antiga */
void etapaDaOS(int posicaoOS, char destino[])
{
    if (strcmp(ordens[posicaoOS].status, "aberta") == 0)
    {
        strcpy(destino, "aguardando tecnico");
        return;
    }
    if (strcmp(ordens[posicaoOS].status, "em_andamento") != 0)
    {
        strcpy(destino, ordens[posicaoOS].status); /* finalizada ou cancelada */
        return;
    }
    if (strlen(ordens[posicaoOS].data_entrega) > 0)
    {
        strcpy(destino, "aguardando feedback");
        return;
    }
    if (strcmp(ordens[posicaoOS].resultado_teste, "aprovado") == 0)
    {
        strcpy(destino, "pronta para entrega");
        return;
    }
    if (strcmp(ordens[posicaoOS].resultado_resposta_cliente, "aprovado") == 0)
    {
        strcpy(destino, "em reparo");
        return;
    }
    if (strcmp(ordens[posicaoOS].resultado_resposta_cliente, "recusado") == 0)
    {
        strcpy(destino, "orcamento recusado");
        return;
    }
    if (ordens[posicaoOS].orcamento >= 0)
    {
        strcpy(destino, "aguardando resposta");
        return;
    }
    if (strlen(ordens[posicaoOS].diagnostico) > 0)
    {
        strcpy(destino, "aguardando orcamento");
        return;
    }
    strcpy(destino, "em vistoria");
}

/* Mostra uma linha da lista de OS: número, abertura, cliente, equipamento e etapa */
void imprimirLinhaOS(int posicaoOS)
{
    char dono[60], etapa[25];
    /* buscarEquipamentoPorId está em equipamento.c, linha 89 */
    int posicaoEquipamento = buscarEquipamentoPorId(ordens[posicaoOS].id_equipamento);

    /* nomeDoCliente está em equipamento.c, linha 143 */
    nomeDoCliente(equipamentos[posicaoEquipamento].id_cliente, dono);
    /* etapaDaOS está em ordem_servico.c, linha 235 */
    etapaDaOS(posicaoOS, etapa);
    printf("OS %d - %s - %s - %s %s - %s\n", ordens[posicaoOS].id_os, ordens[posicaoOS].data_abertura, dono,
           equipamentos[posicaoEquipamento].tipo, equipamentos[posicaoEquipamento].marca, etapa);
}

/* Devolve 1 se a OS entra na lista.
   etapa "" = OS em aberto (e finalizadas se comFinalizadas for 1); outra etapa = só as OS nessa etapa */
int osNaLista(int posicaoOS, char etapa[], int comFinalizadas)
{
    char etapaAtual[25];

    if (strlen(etapa) > 0)
    {
        /* etapaDaOS está em ordem_servico.c, linha 235 */
        etapaDaOS(posicaoOS, etapaAtual);
        if (strcmp(etapaAtual, etapa) == 0)
            return 1;
        return 0;
    }
    if (strcmp(ordens[posicaoOS].status, "aberta") == 0 || strcmp(ordens[posicaoOS].status, "em_andamento") == 0)
        return 1;
    if (comFinalizadas == 1 && strcmp(ordens[posicaoOS].status, "finalizada") == 0)
        return 1;
    return 0;
}

/* Lista as OS e pede o número; devolve a posição da escolhida ou -1 */
int selecionarOS(char etapa[], int comFinalizadas)
{
    int posicaoOS, id, mostrados = 0;

    printf("\n");
    for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
    {
        /* podeVerOS está em ordem_servico.c, linha 212 */
        /* osNaLista está em ordem_servico.c, linha 297 */
        if (podeVerOS(posicaoOS) && osNaLista(posicaoOS, etapa, comFinalizadas))
        {
            /* imprimirLinhaOS está em ordem_servico.c, linha 281 */
            imprimirLinhaOS(posicaoOS);
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

    /* buscarOSPorId está em ordem_servico.c, linha 199 */
    posicaoOS = buscarOSPorId(id);
    if (posicaoOS < 0)
    {
        printf("\n[!] OS %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    if (!podeVerOS(posicaoOS) || !osNaLista(posicaoOS, etapa, comFinalizadas))
    {
        printf("\n[!] OS %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return posicaoOS;
}

/* Coloca o técnico na OS (nova linha em tecnicosOS); devolve a posição ou -1 se o vetor estiver cheio */
int novaLinhaTecnicoOS(int idTecnico, int idOS)
{
    int novaPosicao = totalTecnicosOS;

    if (novaPosicao >= MAX_TECNICO_OS)
    {
        printf("\n[!] Limite de %d atribuicoes atingido.", MAX_TECNICO_OS);
        pausar();
        return -1;
    }

    tecnicosOS[novaPosicao].id_tecnico_os = 1;
    if (novaPosicao > 0)
        tecnicosOS[novaPosicao].id_tecnico_os = tecnicosOS[novaPosicao - 1].id_tecnico_os + 1;
    tecnicosOS[novaPosicao].id_tecnico = idTecnico;
    tecnicosOS[novaPosicao].id_os = idOS;
    dataHoje(tecnicosOS[novaPosicao].data_atribuicao);
    strcpy(tecnicosOS[novaPosicao].observacoes_tecnicas, "");
    dataHoraAgora(tecnicosOS[novaPosicao].criado_em);
    strcpy(tecnicosOS[novaPosicao].atualizado_em, tecnicosOS[novaPosicao].criado_em);
    totalTecnicosOS++;
    return novaPosicao;
}

/* Mostra todos os dados da OS: cliente, equipamento, andamento e técnicos */
void mostrarOS(int posicaoOS)
{
    char etapa[25];
    int posicaoCliente, posicaoEquipamento, posicaoAtribuicao, posicaoTecnico;

    /* etapaDaOS está em ordem_servico.c, linha 235 */
    etapaDaOS(posicaoOS, etapa);
    /* buscarEquipamentoPorId está em equipamento.c, linha 89 */
    posicaoEquipamento = buscarEquipamentoPorId(ordens[posicaoOS].id_equipamento);
    /* buscarUsuarioPorId está em geral.c, linha 123 */
    posicaoCliente = buscarUsuarioPorId(equipamentos[posicaoEquipamento].id_cliente);

    printf("\nOS %d - %s\n", ordens[posicaoOS].id_os, etapa);
    printf("Cliente: %s - CPF %s - %s\n", usuarios[posicaoCliente].nome, usuarios[posicaoCliente].cpf,
           usuarios[posicaoCliente].telefone);
    printf("Equipamento: %s %s\n", equipamentos[posicaoEquipamento].tipo, equipamentos[posicaoEquipamento].marca);
    printf("Abertura: %s\n", ordens[posicaoOS].data_abertura);
    printf("Problema: %s\n", ordens[posicaoOS].problema_relatado);
    printf("Diagnostico: %s\n", ordens[posicaoOS].diagnostico);
    if (ordens[posicaoOS].orcamento >= 0)
        printf("Orcamento: R$ %.2f %s\n", ordens[posicaoOS].orcamento, ordens[posicaoOS].resultado_resposta_cliente);
    printf("Teste: %s\n", ordens[posicaoOS].resultado_teste);
    if (strlen(ordens[posicaoOS].data_entrega) > 0)
        printf("Entrega: %s - pago R$ %.2f\n", ordens[posicaoOS].data_entrega, ordens[posicaoOS].valor_pago);
    printf("Feedback: %s %s\n", ordens[posicaoOS].satisfeito, ordens[posicaoOS].feedback_cliente);

    /* Os técnicos desta OS, com as observações de cada um */
    for (posicaoAtribuicao = 0; posicaoAtribuicao < totalTecnicosOS; posicaoAtribuicao++)
    {
        if (tecnicosOS[posicaoAtribuicao].id_os == ordens[posicaoOS].id_os)
        {
            posicaoTecnico = buscarUsuarioPorId(tecnicosOS[posicaoAtribuicao].id_tecnico);
            printf("Tecnico: %s - %s\n", usuarios[posicaoTecnico].nome, tecnicosOS[posicaoAtribuicao].observacoes_tecnicas);
        }
    }
}

/* ---------- Casos de uso da ordem de servico ---------- */

/* UC24 - Criar Ordem de Servico: abre uma OS para um equipamento ativo de um cliente ativo */
void criarOrdemServico()
{
    char cpf[20], problema[200];
    int posicaoUsuario, posicaoCliente = -1, posicaoEquipamento, novaPosicao, id, mostrados = 0;

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

    /* Procura o cliente com este CPF */
    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        if (strcmp(usuarios[posicaoUsuario].perfil, "cliente") == 0 && strcmp(usuarios[posicaoUsuario].cpf, cpf) == 0)
            posicaoCliente = posicaoUsuario;
    }

    /* RN01 e RN14: o cliente precisa existir e estar ativo */
    if (posicaoCliente < 0)
    {
        printf("\n[!] Nenhum cliente com o CPF %s. Cadastre o cliente e o equipamento antes de abrir a OS.", cpf);
        pausar();
        return;
    }
    if (strcmp(usuarios[posicaoCliente].status, "ativo") != 0)
    {
        printf("\n[!] O cliente %s esta inativo. Nao e possivel abrir OS (RN14).", usuarios[posicaoCliente].nome);
        pausar();
        return;
    }

    /* Lista os equipamentos ativos do cliente */
    printf("\nCliente: %s\n", usuarios[posicaoCliente].nome);
    for (posicaoEquipamento = 0; posicaoEquipamento < totalEquipamentos; posicaoEquipamento++)
    {
        if (equipamentos[posicaoEquipamento].id_cliente == usuarios[posicaoCliente].id_usuario &&
            strcmp(equipamentos[posicaoEquipamento].status, "ativo") == 0)
        {
            printf("ID %d - %s %s - %s\n", equipamentos[posicaoEquipamento].id_equipamento,
                   equipamentos[posicaoEquipamento].tipo, equipamentos[posicaoEquipamento].marca,
                   equipamentos[posicaoEquipamento].modelo_numero_serie);
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

    /* buscarEquipamentoPorId está em equipamento.c, linha 89 */
    posicaoEquipamento = buscarEquipamentoPorId(id);
    if (posicaoEquipamento < 0)
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return;
    }
    if (equipamentos[posicaoEquipamento].id_cliente != usuarios[posicaoCliente].id_usuario ||
        strcmp(equipamentos[posicaoEquipamento].status, "ativo") != 0)
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return;
    }

    lerTexto("\nProblema relatado pelo cliente: ", problema, sizeof(problema));
    /* campoPreenchido está em geral.c, linha 162 */
    if (!campoPreenchido(problema, "problema relatado"))
        return;

    /* Preenche a próxima posição; o novo id é o da última OS + 1; dinheiro -1 = ainda não informado */
    novaPosicao = totalOrdens;
    ordens[novaPosicao].id_os = 1;
    if (novaPosicao > 0)
        ordens[novaPosicao].id_os = ordens[novaPosicao - 1].id_os + 1;
    ordens[novaPosicao].id_equipamento = id;
    ordens[novaPosicao].id_atendente = usuarios[usuarioLogado].id_usuario; /* RN03 */
    dataHoje(ordens[novaPosicao].data_abertura);
    strcpy(ordens[novaPosicao].problema_relatado, problema);
    strcpy(ordens[novaPosicao].diagnostico, "");
    ordens[novaPosicao].orcamento = -1;
    strcpy(ordens[novaPosicao].status, "aberta");
    strcpy(ordens[novaPosicao].resultado_resposta_cliente, "");
    strcpy(ordens[novaPosicao].data_entrega, "");
    strcpy(ordens[novaPosicao].resultado_teste, "");
    ordens[novaPosicao].valor_pago = -1;
    strcpy(ordens[novaPosicao].feedback_cliente, "");
    strcpy(ordens[novaPosicao].satisfeito, "");
    dataHoraAgora(ordens[novaPosicao].criado_em);
    strcpy(ordens[novaPosicao].atualizado_em, ordens[novaPosicao].criado_em);

    totalOrdens++;
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] OS %d criada! Agora atribua um tecnico em Ordens de Servico > Atribuir Tecnico.",
               ordens[novaPosicao].id_os);
    else
        totalOrdens--; /* não gravou: desfaz */
    pausar();
}

/* UC25 - Consultar Ordem de Servico: lista as OS com um filtro e mostra os detalhes da escolhida */
void consultarOS()
{
    char filtro[60], etapa[25];
    int posicaoOS, idCliente, posicaoCliente, id, mostrados;

    imprimirCabecalho("Ordens de Servico > Consultar");

    /* Pede outro filtro enquanto nada for encontrado (exceção 4a) */
    do
    {
        lerTexto("\nFiltro (cliente, CPF ou etapa), ENTER para listar todas ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;

        mostrados = 0;
        for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
        {
            /* podeVerOS está em ordem_servico.c, linha 212 */
            if (!podeVerOS(posicaoOS))
                continue;

            /* O filtro pode estar no nome do cliente, no CPF ou na etapa */
            /* donoDoEquipamento está em geral.c, linha 174 */
            idCliente = donoDoEquipamento(ordens[posicaoOS].id_equipamento);
            /* buscarUsuarioPorId está em geral.c, linha 123 */
            posicaoCliente = buscarUsuarioPorId(idCliente);
            /* etapaDaOS está em ordem_servico.c, linha 235 */
            etapaDaOS(posicaoOS, etapa);
            if (strstr(usuarios[posicaoCliente].nome, filtro) != NULL ||
                strstr(usuarios[posicaoCliente].cpf, filtro) != NULL ||
                strstr(etapa, filtro) != NULL)
            {
                /* imprimirLinhaOS está em ordem_servico.c, linha 281 */
                imprimirLinhaOS(posicaoOS);
                mostrados++;
            }
        }

        if (mostrados == 0)
            printf("\n[!] Nenhuma OS encontrada para o filtro informado. Tente outro filtro.\n");
    } while (mostrados == 0);

    id = lerInteiro("\nNumero da OS para ver os detalhes (0 para voltar): ");
    if (id <= 0)
        return;

    /* buscarOSPorId está em ordem_servico.c, linha 199 */
    posicaoOS = buscarOSPorId(id);
    if (posicaoOS < 0)
    {
        printf("\n[!] OS %d nao esta na lista.", id);
        pausar();
        return;
    }
    if (!podeVerOS(posicaoOS))
    {
        printf("\n[!] OS %d nao esta na lista.", id);
        pausar();
        return;
    }
    /* mostrarOS está em ordem_servico.c, linha 391 */
    mostrarOS(posicaoOS);
    pausar();
}

/* UC26 - Editar Ordem de Servico: só o problema relatado (RN07); só o admin edita OS finalizada (RN08) */
void editarOS()
{
    char problema[200];
    int posicaoOS;

    imprimirCabecalho("Ordens de Servico > Editar");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    /* perfilLogado está em geral.c, linha 5 */
    posicaoOS = selecionarOS("", perfilLogado("admin"));
    if (posicaoOS < 0)
        return;

    printf("\nSo o problema relatado pode ser alterado (RN07). ENTER mantem o valor atual.\n");
    /* lerOuManter está em geral.c, linha 323 */
    lerOuManter("Problema relatado", ordens[posicaoOS].problema_relatado, problema, sizeof(problema));

    strcpy(ordens[posicaoOS].problema_relatado, problema);
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] OS %d atualizada!", ordens[posicaoOS].id_os);
    pausar();
}

/* UC27 - Cancelar Ordem de Servico: muda o status para cancelada; a OS continua no histórico (RN09) */
void cancelarOS()
{
    int posicaoOS;

    imprimirCabecalho("Ordens de Servico > Cancelar");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("", 0);
    if (posicaoOS < 0)
        return;

    if (!confirmar("\nCancelar esta OS? Ela continua no historico (RN09)."))
    {
        printf("\nOperacao cancelada. A OS nao foi alterada.");
        pausar();
        return;
    }

    strcpy(ordens[posicaoOS].status, "cancelada");
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] OS %d cancelada. O registro foi mantido no historico.", ordens[posicaoOS].id_os);
    pausar();
}

/* UC28 - Atribuir Tecnico: coloca um técnico (ou admin) ativo na OS; pode repetir para mais de um (RN04) */
void atribuirTecnico()
{
    int posicaoOS, posicaoUsuario, posicaoTecnico, id, mostrados = 0;

    imprimirCabecalho("Ordens de Servico > Atribuir Tecnico");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("", 0);
    if (posicaoOS < 0)
        return;

    /* RN15: técnico ou admin (RN11); RN14: só ativos */
    printf("\nTecnicos disponiveis:\n");
    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        if (strcmp(usuarios[posicaoUsuario].status, "ativo") == 0 &&
            (strcmp(usuarios[posicaoUsuario].perfil, "tecnico") == 0 || strcmp(usuarios[posicaoUsuario].perfil, "admin") == 0))
        {
            printf("ID %d - %s - %s %s\n", usuarios[posicaoUsuario].id_usuario, usuarios[posicaoUsuario].nome,
                   usuarios[posicaoUsuario].perfil, usuarios[posicaoUsuario].especialidade);
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

    /* buscarUsuarioPorId está em geral.c, linha 123 */
    posicaoTecnico = buscarUsuarioPorId(id);
    if (posicaoTecnico < 0)
    {
        printf("\n[!] O usuario %d nao e um tecnico ativo (RN14, RN15).", id);
        pausar();
        return;
    }
    if (strcmp(usuarios[posicaoTecnico].status, "ativo") != 0 ||
        (strcmp(usuarios[posicaoTecnico].perfil, "tecnico") != 0 && strcmp(usuarios[posicaoTecnico].perfil, "admin") != 0))
    {
        printf("\n[!] O usuario %d nao e um tecnico ativo (RN14, RN15).", id);
        pausar();
        return;
    }
    /* buscarTecnicoNaOS está em geral.c, linha 187 */
    if (buscarTecnicoNaOS(id, ordens[posicaoOS].id_os) >= 0)
    {
        printf("\n[!] %s ja esta nesta OS.", usuarios[posicaoTecnico].nome);
        pausar();
        return;
    }

    /* novaLinhaTecnicoOS está em ordem_servico.c, linha 366 */
    if (novaLinhaTecnicoOS(id, ordens[posicaoOS].id_os) < 0)
        return;
    /* salvarTecnicosOS está em ordem_servico.c, linha 121 */
    if (!salvarTecnicosOS())
    {
        totalTecnicosOS--; /* não gravou: desfaz */
        pausar();
        return;
    }

    /* Com técnico, a OS sai de "aberta" e vai para a vistoria */
    strcpy(ordens[posicaoOS].status, "em_andamento");
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] %s atribuido(a) a OS %d!", usuarios[posicaoTecnico].nome, ordens[posicaoOS].id_os);
    pausar();
}

/* UC29 - Registrar Entrega e Pagamento: só depois do teste aprovado (RF08) */
void registrarEntrega()
{
    double valor;
    int posicaoOS, confirmado;

    imprimirCabecalho("Ordens de Servico > Registrar Entrega");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("", 0);
    if (posicaoOS < 0)
        return;

    if (strcmp(ordens[posicaoOS].resultado_teste, "aprovado") != 0)
    {
        printf("\n[!] A OS %d nao esta pronta para entrega.", ordens[posicaoOS].id_os);
        pausar();
        return;
    }
    if (strlen(ordens[posicaoOS].data_entrega) > 0)
    {
        printf("\n[!] A OS %d nao esta pronta para entrega.", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    printf("\nOrcamento aprovado: R$ %.2f\n", ordens[posicaoOS].orcamento);

    /* Exceção 7a: valor diferente do orçamento precisa ser confirmado ou digitado de novo */
    do
    {
        valor = lerValor("Valor recebido em R$ (ex: 95.50, ENTER para voltar): ");
        if (valor < 0)
        {
            printf("\nNada foi gravado.");
            pausar();
            return;
        }

        confirmado = 1;
        if (valor != ordens[posicaoOS].orcamento)
        {
            confirmado = confirmar("\n[!] Valor diferente do orcamento. Confirmar mesmo assim?");
            if (!confirmado)
                printf("Digite o valor correto.\n");
        }
    } while (!confirmado);

    dataHoje(ordens[posicaoOS].data_entrega);
    ordens[posicaoOS].valor_pago = valor;
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] Entrega registrada! A OS %d aguarda o feedback do cliente.", ordens[posicaoOS].id_os);
    pausar();
}

/* UC30 - Registrar Feedback: satisfeito finaliza a OS; não satisfeito volta para o reparo */
void registrarFeedback()
{
    int posicaoOS;

    imprimirCabecalho("Ordens de Servico > Registrar Feedback");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("", 0);
    if (posicaoOS < 0)
        return;

    if (strlen(ordens[posicaoOS].data_entrega) == 0)
    {
        printf("\n[!] A OS %d ainda nao foi entregue.", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    lerTexto("\nComentario do cliente: ", ordens[posicaoOS].feedback_cliente, sizeof(ordens[posicaoOS].feedback_cliente));

    if (confirmar("O cliente ficou satisfeito?"))
    {
        strcpy(ordens[posicaoOS].satisfeito, "sim");
        strcpy(ordens[posicaoOS].status, "finalizada");
    }
    else
    {
        /* Limpa teste, entrega e pagamento para refazer o reparo */
        strcpy(ordens[posicaoOS].satisfeito, "nao");
        strcpy(ordens[posicaoOS].resultado_teste, "");
        strcpy(ordens[posicaoOS].data_entrega, "");
        ordens[posicaoOS].valor_pago = -1;
    }

    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    if (strcmp(ordens[posicaoOS].status, "finalizada") == 0)
        printf("\n[+] Feedback registrado. A OS %d foi finalizada!", ordens[posicaoOS].id_os);
    else
        printf("\n[+] Feedback registrado. A OS %d voltou para Executar Manutencao.", ordens[posicaoOS].id_os);
    pausar();
}

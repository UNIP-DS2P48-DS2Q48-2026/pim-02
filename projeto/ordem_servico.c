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

/* A OS ainda está em aberto (aberta ou em andamento)? Finalizada e cancelada não mudam mais (RN08) */
int osEmAberto(int i)
{
    if (strcmp(ordens[i].status, "aberta") == 0 || strcmp(ordens[i].status, "em_andamento") == 0)
        return 1;
    return 0;
}

/* Copia para destino o tipo e a marca do equipamento da OS, para as listagens */
void nomeDoEquipamento(int idEquipamento, char destino[])
{
    int e = buscarEquipamentoPorId(idEquipamento);

    if (e >= 0)
        sprintf(destino, "%s %s", equipamentos[e].tipo, equipamentos[e].marca);
    else
        strcpy(destino, "?");
}

/* Copia para destino a etapa em que a OS está, olhando quais campos já foram preenchidos.
   A OS em andamento é conferida da etapa mais adiantada (entrega) para a mais antiga (vistoria):
   a primeira que bater é a etapa atual. */
void etapaDaOS(int i, char destino[])
{
    /* Ainda sem técnico atribuído (UC28) */
    if (strcmp(ordens[i].status, "aberta") == 0)
    {
        strcpy(destino, "aguardando tecnico");
        return;
    }

    /* Finalizada ou cancelada: mostra o próprio status */
    if (strcmp(ordens[i].status, "finalizada") == 0 || strcmp(ordens[i].status, "cancelada") == 0)
    {
        strcpy(destino, ordens[i].status);
        return;
    }

    /* Daqui para baixo a OS está em andamento */

    /* Já foi entregue (UC29): falta o feedback do cliente (UC30) */
    if (strlen(ordens[i].data_entrega) > 0)
    {
        strcpy(destino, "aguardando feedback");
        return;
    }

    /* Teste aprovado (UC34): falta entregar */
    if (strcmp(ordens[i].resultado_teste, "aprovado") == 0)
    {
        strcpy(destino, "pronta para entrega");
        return;
    }

    /* Cliente aprovou o orçamento (UC39): o técnico faz o reparo e o teste */
    if (strcmp(ordens[i].resultado_resposta_cliente, "aprovado") == 0)
    {
        strcpy(destino, "em reparo");
        return;
    }

    /* Cliente recusou (UC40): renegociar (UC37) ou arquivar (UC41) */
    if (strcmp(ordens[i].resultado_resposta_cliente, "recusado") == 0)
    {
        strcpy(destino, "orcamento recusado");
        return;
    }

    /* Orçamento registrado (UC36), sem resposta do cliente ainda (-1 = sem orçamento) */
    if (ordens[i].orcamento >= 0)
    {
        strcpy(destino, "aguardando resposta");
        return;
    }

    /* Diagnóstico registrado (UC32): falta o orçamento */
    if (strlen(ordens[i].diagnostico) > 0)
    {
        strcpy(destino, "aguardando orcamento");
        return;
    }

    /* Nada preenchido ainda: o técnico está olhando o equipamento (UC31) */
    strcpy(destino, "em vistoria");
}

/* Mostra uma linha da tabela de OS (o cabeçalho é impresso por quem chama) */
void imprimirLinhaOS(int i)
{
    char dono[60], equipamento[90], etapa[25];

    nomeDoCliente(donoDoEquipamento(ordens[i].id_equipamento), dono);
    nomeDoEquipamento(ordens[i].id_equipamento, equipamento);
    etapaDaOS(i, etapa);
    printf("  %-4d %-11s %-18.18s %-20.20s %s\n", ordens[i].id_os, ordens[i].data_abertura, dono, equipamento, etapa);
}

void imprimirCabecalhoOS()
{
    printf("\n  %-4s %-11s %-18s %-20s %s\n", "OS", "Abertura", "Cliente", "Equipamento", "Etapa");
    printf("  ---------------------------------------------------------------------------\n");
}

/* A OS entra na lista?
   etapa = "" : todas as OS em aberto (e as finalizadas, se comFinalizadas for 1)
   etapa = "aguardando resposta", "em reparo"...: só as OS que estão nessa etapa (ver etapaDaOS) */
int osNaLista(int i, char etapa[], int comFinalizadas)
{
    char etapaAtual[25];

    if (strlen(etapa) > 0)
    {
        etapaDaOS(i, etapaAtual);
        if (strcmp(etapaAtual, etapa) == 0)
        {
            return 1;
        }
        return 0;
    }

    if (osEmAberto(i))
    {
        return 1;
    }

    if (comFinalizadas == 1 && strcmp(ordens[i].status, "finalizada") == 0)
    {
        return 1;
    }
    return 0;
}

/* Lista as OS que o usuário pode ver e pede o número. Devolve a posição no vetor ou -1.
   etapa: "" para todas em aberto, ou o nome de uma etapa para listar só as OS dela.
   comFinalizadas = 1 inclui as finalizadas (só o Admin, ao editar: RN08) */
int selecionarOS(char etapa[], int comFinalizadas)
{
    int i, id, mostrados = 0;

    for (i = 0; i < totalOrdens; i++)
    {
        if (!podeVerOS(i) || !osNaLista(i, etapa, comFinalizadas))
            continue;
        if (mostrados == 0)
            imprimirCabecalhoOS();
        imprimirLinhaOS(i);
        mostrados++;
    }

    if (mostrados == 0)
    {
        if (strlen(etapa) > 0)
        {
            printf("\n[!] Nenhuma ordem de servico na etapa \"%s\".", etapa);
        }
        else
        {
            printf("\n[!] Nenhuma ordem de servico em aberto.");
        }
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
    int i, n, maiorId = 0;

    if (totalTecnicosOS >= MAX_TECNICO_OS)
    {
        printf("\n[!] Limite de %d atribuicoes atingido.", MAX_TECNICO_OS);
        pausar();
        return -1;
    }

    for (i = 0; i < totalTecnicosOS; i++)
    {
        if (tecnicosOS[i].id_tecnico_os > maiorId)
            maiorId = tecnicosOS[i].id_tecnico_os;
    }

    n = totalTecnicosOS;
    tecnicosOS[n].id_tecnico_os = maiorId + 1;
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
    char etapa[25], equipamento[90];
    int c, k, t;

    etapaDaOS(i, etapa);
    nomeDoEquipamento(ordens[i].id_equipamento, equipamento);
    c = buscarUsuarioPorId(donoDoEquipamento(ordens[i].id_equipamento));

    printf("\nOS %d - %s\n", ordens[i].id_os, etapa);
    if (c >= 0)
        printf("  Cliente:     %s - CPF %s - %s\n", usuarios[c].nome, usuarios[c].cpf, usuarios[c].telefone);
    printf("  Equipamento: %s\n", equipamento);
    printf("  Abertura:    %s\n", ordens[i].data_abertura);
    printf("  Problema:    %s\n", ordens[i].problema_relatado);
    if (strlen(ordens[i].diagnostico) > 0)
        printf("  Diagnostico: %s\n", ordens[i].diagnostico);
    if (ordens[i].orcamento >= 0)
        printf("  Orcamento:   R$ %.2f %s\n", ordens[i].orcamento, ordens[i].resultado_resposta_cliente);
    if (strlen(ordens[i].resultado_teste) > 0)
        printf("  Teste:       %s\n", ordens[i].resultado_teste);
    if (strlen(ordens[i].data_entrega) > 0)
        printf("  Entrega:     %s - pago R$ %.2f\n", ordens[i].data_entrega, ordens[i].valor_pago);
    if (strlen(ordens[i].satisfeito) > 0)
        printf("  Feedback:    satisfeito: %s - %s\n", ordens[i].satisfeito, ordens[i].feedback_cliente);

    for (k = 0; k < totalTecnicosOS; k++)
    {
        if (tecnicosOS[k].id_os != ordens[i].id_os)
            continue;
        t = buscarUsuarioPorId(tecnicosOS[k].id_tecnico);
        if (t >= 0)
            printf("  Tecnico:     %s - %s\n", usuarios[t].nome, tecnicosOS[k].observacoes_tecnicas);
    }
}

/* ---------- Casos de uso da ordem de servico ---------- */

/* UC24 - Criar Ordem de Servico: abre uma OS para um equipamento do cliente */
void criarOrdemServico()
{
    char cpf[20], problema[200];
    int i, c = -1, e, n, id, mostrados = 0, maiorId = 0;

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
        if (equipamentos[e].id_cliente != usuarios[c].id_usuario || strcmp(equipamentos[e].status, "ativo") != 0)
            continue;
        printf("  %d - %s %s (%s)\n", equipamentos[e].id_equipamento, equipamentos[e].tipo, equipamentos[e].marca,
               equipamentos[e].modelo_numero_serie);
        mostrados++;
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

    for (i = 0; i < totalOrdens; i++)
    {
        if (ordens[i].id_os > maiorId)
            maiorId = ordens[i].id_os;
    }

    /* Preenche a próxima posição do vetor; dinheiro -1 = ainda não informado */
    n = totalOrdens;
    ordens[n].id_os = maiorId + 1;
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

    if (!salvarOrdens())
    {
        totalOrdens--; /* não gravou: desfaz */
        pausar();
        return;
    }

    printf("\n[+] OS %d criada! Agora atribua um tecnico em Ordens de Servico > Atribuir Tecnico.", ordens[n].id_os);
    pausar();
}

/* UC25 - Consultar Ordem de Servico: lista as OS (inclusive finalizadas e canceladas) com um filtro */
void consultarOS()
{
    char filtro[60], etapa[25];
    int i, c, id, mostrados = 0;

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
            if (c < 0 || (!contemTexto(usuarios[c].nome, filtro) && !contemTexto(usuarios[c].cpf, filtro) &&
                          !contemTexto(etapa, filtro)))
                continue;

            if (mostrados == 0)
                imprimirCabecalhoOS();
            imprimirLinhaOS(i);
            mostrados++;
        }

        if (mostrados == 0)
        {
            printf("\n[!] Nenhuma OS encontrada para o filtro informado. Tente outro filtro.\n");
        }
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

    /* RN07: a recepcao altera só o dado administrativo; diagnóstico e teste são do técnico */
    printf("\nSo o problema relatado pode ser alterado (RN07). ENTER mantem o valor atual.\n");
    lerOuManter("Problema relatado", ordens[i].problema_relatado, problema, sizeof(problema));

    strcpy(ordens[i].problema_relatado, problema);
    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }
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
    if (!salvarOrdens())
    {
        pausar();
        return;
    }
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
        if (strcmp(usuarios[t].status, "ativo") != 0)
            continue;
        if (strcmp(usuarios[t].perfil, "tecnico") != 0 && strcmp(usuarios[t].perfil, "admin") != 0)
            continue;
        printf("  %d - %s (%s %s)\n", usuarios[t].id_usuario, usuarios[t].nome, usuarios[t].perfil,
               usuarios[t].especialidade);
        mostrados++;
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
    if (!salvarOrdens())
    {
        pausar();
        return;
    }
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
            {
                printf("Digite o valor correto.\n");
            }
        }
    } while (!confirmado);

    dataHoje(ordens[i].data_entrega);
    ordens[i].valor_pago = valor;
    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }
    printf("\n[+] Entrega registrada! A OS %d aguarda o feedback do cliente.", ordens[i].id_os);
    pausar();
}

/* UC30 - Registrar Feedback do Cliente: finaliza a OS ou devolve para novo reparo */
void registrarFeedback()
{
    char comentario[200];
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

    lerTexto("\nComentario do cliente: ", comentario, sizeof(comentario));
    strcpy(ordens[i].feedback_cliente, comentario);

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

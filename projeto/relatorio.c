/* A data está no formato AAAA-MM-DD? */
int dataValida(char data[])
{
    int i;

    if (strlen(data) != 10 || data[4] != '-' || data[7] != '-')
        return 0;
    for (i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (data[i] < '0' || data[i] > '9')
            return 0;
    }
    return 1;
}

/* UC42 - Consultar Historico dos Equipamentos: mostra todas as OS de um equipamento */
void historicoEquipamento()
{
    char dono[60];
    int e, i, k, t, tecnicos, mostrados = 0;

    imprimirCabecalho("Historico de Equipamento");

    /* A escolha do equipamento já respeita o escopo do perfil (RN10, RI10) */
    e = selecionarEquipamento(0);
    if (e < 0)
        return;

    nomeDoCliente(equipamentos[e].id_cliente, dono);
    printf("\nEquipamento %d: %s %s (%s) - cliente %s\n", equipamentos[e].id_equipamento, equipamentos[e].tipo,
           equipamentos[e].marca, equipamentos[e].modelo_numero_serie, dono);

    /* Todas as OS do equipamento, inclusive finalizadas e canceladas (RN09) */
    for (i = 0; i < totalOrdens; i++)
    {
        if (ordens[i].id_equipamento != equipamentos[e].id_equipamento)
            continue;

        printf("\n  OS %d - aberta em %s - %s\n", ordens[i].id_os, ordens[i].data_abertura, ordens[i].status);
        printf("    Problema:    %s\n", ordens[i].problema_relatado);
        if (strlen(ordens[i].diagnostico) > 0)
            printf("    Diagnostico: %s\n", ordens[i].diagnostico);
        else
            printf("    Diagnostico: (ainda nao registrado)\n");

        /* Técnicos atribuídos à OS (tabela tecnico_os) */
        printf("    Tecnicos:    ");
        tecnicos = 0;
        for (k = 0; k < totalTecnicosOS; k++)
        {
            if (tecnicosOS[k].id_os != ordens[i].id_os)
                continue;
            t = buscarUsuarioPorId(tecnicosOS[k].id_tecnico);
            if (tecnicos > 0)
                printf(", ");
            if (t >= 0)
                printf("%s", usuarios[t].nome);
            else
                printf("?");
            tecnicos++;
        }
        if (tecnicos == 0)
            printf("(nenhum)");
        printf("\n");

        if (strlen(ordens[i].data_entrega) > 0)
            printf("    Entrega:     %s\n", ordens[i].data_entrega);
        mostrados++;
    }

    if (mostrados == 0)
    {
        printf("\n[!] Este equipamento ainda nao tem historico de atendimento.\n");
    }
    pausar();
}

/* UC43 - Consultar Relatorios: contagens de OS e equipamentos por periodo */
void consultarRelatorios()
{
    char inicio[TAM_DATA + 5], fim[TAM_DATA + 5];
    int i, j, repetido;
    int total = 0, abertas = 0, andamento = 0, finalizadas = 0, canceladas = 0;
    int equipamentosAtendidos = 0, comFeedback = 0, satisfeitos = 0;

    imprimirCabecalho("Relatorios");

    printf("\nInforme o periodo pela data de abertura das OS (formato AAAA-MM-DD).\n");
    lerTexto("Data inicial (ENTER para desde o inicio): ", inicio, sizeof(inicio));
    lerTexto("Data final (ENTER para ate hoje): ", fim, sizeof(fim));

    /* ENTER em branco: sem limite naquele lado do período */
    if (strlen(inicio) == 0)
        strcpy(inicio, "0000-01-01");
    if (strlen(fim) == 0)
        dataHoje(fim);

    if (!dataValida(inicio) || !dataValida(fim))
    {
        printf("\n[!] Data invalida. Use o formato AAAA-MM-DD (ex: 2026-10-01).");
        pausar();
        return;
    }

    /* Datas AAAA-MM-DD podem ser comparadas como texto com strcmp */
    for (i = 0; i < totalOrdens; i++)
    {
        if (strcmp(ordens[i].data_abertura, inicio) < 0 || strcmp(ordens[i].data_abertura, fim) > 0)
            continue;

        total++;
        if (strcmp(ordens[i].status, "aberta") == 0)
            abertas++;
        else if (strcmp(ordens[i].status, "em_andamento") == 0)
            andamento++;
        else if (strcmp(ordens[i].status, "finalizada") == 0)
            finalizadas++;
        else if (strcmp(ordens[i].status, "cancelada") == 0)
            canceladas++;

        if (strlen(ordens[i].satisfeito) > 0)
        {
            comFeedback++;
            if (strcmp(ordens[i].satisfeito, "sim") == 0)
                satisfeitos++;
        }

        /* Conta o equipamento só na primeira OS dele dentro do período */
        repetido = 0;
        for (j = 0; j < i; j++)
        {
            if (ordens[j].id_equipamento == ordens[i].id_equipamento &&
                strcmp(ordens[j].data_abertura, inicio) >= 0 && strcmp(ordens[j].data_abertura, fim) <= 0)
            {
                repetido = 1;
            }
        }
        if (repetido == 0)
            equipamentosAtendidos++;
    }

    if (total == 0)
    {
        printf("\n[!] Nenhuma OS aberta entre %s e %s.", inicio, fim);
        pausar();
        return;
    }

    printf("\n  Periodo: %s a %s\n", inicio, fim);
    printf("  ---------------------------------------------------------------------------\n");
    printf("  Ordens de servico abertas no periodo:      %d\n", total);
    printf("    Abertas (aguardando tecnico):            %d\n", abertas);
    printf("    Em andamento:                            %d\n", andamento);
    printf("    Finalizadas:                             %d\n", finalizadas);
    printf("    Canceladas / arquivadas:                 %d\n", canceladas);
    printf("  Equipamentos que passaram por manutencao:  %d\n", equipamentosAtendidos);
    printf("  Clientes satisfeitos (feedback):           %d de %d\n", satisfeitos, comFeedback);
    pausar();
}

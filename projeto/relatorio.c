/* UC42 - Consultar Historico dos Equipamentos: mostra as OS de um equipamento */
void consultarHistorico()
{
    int e, i, mostrados = 0;

    imprimirCabecalho("Historico de Equipamento");

    /* Escolhe outro equipamento enquanto o escolhido não tiver OS (exceção 4a) */
    do
    {
        /* A escolha do equipamento já respeita o que cada perfil pode ver (RN10, RI10) */
        e = selecionarEquipamento("");
        if (e < 0)
            return;

        printf("\nEquipamento %d: %s %s (%s)\n", equipamentos[e].id_equipamento, equipamentos[e].tipo,
               equipamentos[e].marca, equipamentos[e].modelo_numero_serie);

        /* As OS do equipamento, inclusive finalizadas e canceladas (RN09), dentro do escopo do perfil:
           o técnico só vê as OS em que trabalhou (RN10) */
        mostrados = 0;
        for (i = 0; i < totalOrdens; i++)
        {
            if (ordens[i].id_equipamento == equipamentos[e].id_equipamento && podeVerOS(i))
            {
                mostrarOS(i);
                mostrados++;
            }
        }

        if (mostrados == 0)
        {
            printf("\n[!] Este equipamento ainda nao tem historico de atendimento. Escolha outro.\n");
        }
    } while (mostrados == 0);
    pausar();
}

/* UC43 - Consultar Relatorios: quantidade de OS por mes e por status num ano */
void gerarRelatorio()
{
    /* Matriz: uma linha por mês (0 = janeiro ... 11 = dezembro), uma coluna por status */
    int contagem[12][4];
    char nomesMes[12][4] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
    char ano[10], anoDaOS[5], mesDaOS[3];
    int i, m, s, totalMes, satisfeitos = 0, comFeedback = 0;

    imprimirCabecalho("Relatorios");

    lerTexto("\nAno (AAAA, ENTER para o ano atual): ", ano, sizeof(ano));
    if (strlen(ano) == 0)
    {
        dataHoje(anoDaOS); /* AAAA-MM-DD: fica só com o AAAA */
        anoDaOS[4] = '\0';
        strcpy(ano, anoDaOS);
    }

    /* Zera a matriz */
    for (m = 0; m < 12; m++)
    {
        for (s = 0; s < 4; s++)
        {
            contagem[m][s] = 0;
        }
    }

    /* Conta cada OS na linha do mês e na coluna do status */
    for (i = 0; i < totalOrdens; i++)
    {
        /* data_abertura = AAAA-MM-DD: separa o ano (posições 0 a 3) e o mês (posições 5 e 6) */
        anoDaOS[0] = ordens[i].data_abertura[0];
        anoDaOS[1] = ordens[i].data_abertura[1];
        anoDaOS[2] = ordens[i].data_abertura[2];
        anoDaOS[3] = ordens[i].data_abertura[3];
        anoDaOS[4] = '\0';
        if (strcmp(anoDaOS, ano) != 0)
            continue;

        mesDaOS[0] = ordens[i].data_abertura[5];
        mesDaOS[1] = ordens[i].data_abertura[6];
        mesDaOS[2] = '\0';
        m = atoi(mesDaOS) - 1;
        if (m < 0 || m > 11)
            continue;

        /* Coluna da matriz: 0 = aberta, 1 = em andamento, 2 = finalizada, 3 = cancelada */
        s = 3; /* começa como cancelada (o arquivamento do orçamento também grava cancelada) */

        if (strcmp(ordens[i].status, "aberta") == 0)
        {
            s = 0;
        }

        if (strcmp(ordens[i].status, "em_andamento") == 0)
        {
            s = 1;
        }

        if (strcmp(ordens[i].status, "finalizada") == 0)
        {
            s = 2;
        }
        contagem[m][s]++;

        if (strlen(ordens[i].satisfeito) > 0)
        {
            comFeedback++;
            if (strcmp(ordens[i].satisfeito, "sim") == 0)
                satisfeitos++;
        }
    }

    /* Imprime a matriz como tabela, pulando os meses sem nenhuma OS */
    printf("\n  Ordens de servico abertas em %s\n\n", ano);
    printf("  %-5s %8s %12s %12s %11s %7s\n", "Mes", "Abertas", "Andamento", "Finalizadas", "Canceladas", "Total");
    printf("  ----------------------------------------------------------\n");
    for (m = 0; m < 12; m++)
    {
        totalMes = contagem[m][0] + contagem[m][1] + contagem[m][2] + contagem[m][3];
        if (totalMes == 0)
            continue;
        printf("  %-5s %8d %12d %12d %11d %7d\n", nomesMes[m], contagem[m][0], contagem[m][1], contagem[m][2],
               contagem[m][3], totalMes);
    }
    printf("\n  Clientes satisfeitos: %d de %d que deram feedback\n", satisfeitos, comFeedback);
    pausar();
}

/* Funções do main.c usadas aqui: lerTexto (linha 121), pausar (linha 173), imprimirCabecalho (linha 214), dataHoje (linha 193) */
/* relatorio.c - histórico do equipamento e relatório de OS por mês */

/* UC42 - Consultar Historico: todas as OS de um equipamento */
void consultarHistorico()
{
    int posicaoEquipamento, posicaoOS, mostrados = 0;

    imprimirCabecalho("Historico de Equipamento");

    /* Pede outro equipamento enquanto o escolhido não tiver OS (exceção 4a) */
    do
    {
        /* selecionarEquipamento está em equipamento.c, linha 195 */
        posicaoEquipamento = selecionarEquipamento("");
        if (posicaoEquipamento < 0)
            return;

        printf("\nEquipamento %d: %s %s - %s\n", equipamentos[posicaoEquipamento].id_equipamento, equipamentos[posicaoEquipamento].tipo,
               equipamentos[posicaoEquipamento].marca, equipamentos[posicaoEquipamento].modelo_numero_serie);

        /* Inclui finalizadas e canceladas (RN09); o técnico só vê as dele (RN10) */
        mostrados = 0;
        for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
        {
            /* podeVerOS está em ordem_servico.c, linha 212 */
            if (ordens[posicaoOS].id_equipamento == equipamentos[posicaoEquipamento].id_equipamento && podeVerOS(posicaoOS))
            {
                /* mostrarOS está em ordem_servico.c, linha 391 */
                mostrarOS(posicaoOS);
                mostrados++;
            }
        }

        if (mostrados == 0)
            printf("\n[!] Este equipamento ainda nao tem historico de atendimento. Escolha outro.\n");
    } while (mostrados == 0);
    pausar();
}

/* UC43 - Consultar Relatorios: quantidade de OS por mês e por status no ano */
void gerarRelatorio()
{
    /* Linha = mês (0 = janeiro), coluna = status */
    int contagem[12][4];
    char nomesMes[12][4] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez"};
    char ano[TAM_DATA], anoDaOS[5], mesDaOS[3];
    int posicaoOS, mes, coluna, satisfeitos = 0, comFeedback = 0;

    imprimirCabecalho("Relatorios");

    lerTexto("\nAno (AAAA, ENTER para o ano atual): ", ano, sizeof(ano));
    if (strlen(ano) == 0)
    {
        dataHoje(ano); /* AAAA-MM-DD: fica só com o AAAA */
        ano[4] = '\0';
    }

    /* Zera a matriz */
    for (mes = 0; mes < 12; mes++)
    {
        for (coluna = 0; coluna < 4; coluna++)
        {
            contagem[mes][coluna] = 0;
        }
    }

    /* Conta cada OS na linha do mês e na coluna do status */
    for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
    {
        /* data_abertura é AAAA-MM-DD: ano nas posições 0 a 3, mês nas posições 5 e 6 */
        anoDaOS[0] = ordens[posicaoOS].data_abertura[0];
        anoDaOS[1] = ordens[posicaoOS].data_abertura[1];
        anoDaOS[2] = ordens[posicaoOS].data_abertura[2];
        anoDaOS[3] = ordens[posicaoOS].data_abertura[3];
        anoDaOS[4] = '\0';
        if (strcmp(anoDaOS, ano) != 0)
            continue;

        mesDaOS[0] = ordens[posicaoOS].data_abertura[5];
        mesDaOS[1] = ordens[posicaoOS].data_abertura[6];
        mesDaOS[2] = '\0';
        mes = atoi(mesDaOS) - 1;

        /* Coluna: 0 = aberta, 1 = em andamento, 2 = finalizada, 3 = cancelada */
        if (strcmp(ordens[posicaoOS].status, "aberta") == 0)
            coluna = 0;
        else if (strcmp(ordens[posicaoOS].status, "em_andamento") == 0)
            coluna = 1;
        else if (strcmp(ordens[posicaoOS].status, "finalizada") == 0)
            coluna = 2;
        else
            coluna = 3;
        contagem[mes][coluna]++;

        if (strlen(ordens[posicaoOS].satisfeito) > 0)
        {
            comFeedback++;
            if (strcmp(ordens[posicaoOS].satisfeito, "sim") == 0)
                satisfeitos++;
        }
    }

    /* Mostra a matriz, pulando os meses sem nenhuma OS */
    printf("\nOrdens de servico abertas em %s\n", ano);
    for (mes = 0; mes < 12; mes++)
    {
        if (contagem[mes][0] + contagem[mes][1] + contagem[mes][2] + contagem[mes][3] > 0)
        {
            printf("%s: abertas %d - em andamento %d - finalizadas %d - canceladas %d\n", nomesMes[mes], contagem[mes][0],
                   contagem[mes][1], contagem[mes][2], contagem[mes][3]);
        }
    }
    printf("\nClientes satisfeitos: %d de %d que deram feedback\n", satisfeitos, comFeedback);
    pausar();
}

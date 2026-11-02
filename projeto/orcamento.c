/* Converte o texto digitado em valor (aceita vírgula ou ponto); devolve -1 se não for um valor válido */
double textoParaValor(char texto[])
{
    double valor;
    int i;

    for (i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] == ',')
            texto[i] = '.';
    }
    if (sscanf(texto, "%lf", &valor) != 1 || valor < 0)
    {
        return -1;
    }
    return valor;
}

/* Copia para destino a situação do orçamento da OS */
void situacaoOrcamento(int i, char destino[])
{
    if (ordens[i].orcamento < 0)
        strcpy(destino, "sem orcamento");
    else if (strlen(ordens[i].resultado_resposta_cliente) == 0)
        strcpy(destino, "aguardando resposta");
    else
        strcpy(destino, ordens[i].resultado_resposta_cliente);
}

/* Mostra o diagnóstico, o valor e a situação do orçamento da OS */
void mostrarOrcamento(int i)
{
    char situacao[25];

    situacaoOrcamento(i, situacao);
    printf("\nOS %d\n", ordens[i].id_os);
    printf("  Problema:    %s\n", ordens[i].problema_relatado);
    printf("  Diagnostico: %s\n", ordens[i].diagnostico);
    if (ordens[i].orcamento >= 0)
        printf("  Valor:       R$ %.2f\n", ordens[i].orcamento);
    printf("  Situacao:    %s\n", situacao);
}

/* UC36 - Registrar Orcamento: registra o valor da proposta a partir do diagnostico */
void registrarOrcamento()
{
    char texto[30];
    double valor;
    int i;

    imprimirCabecalho("Orcamentos > Registrar");

    /* Só aparecem as OS com diagnóstico registrado e sem orçamento (RF05) */
    i = selecionarOS(ETAPA_SEM_ORCAMENTO);
    if (i < 0)
        return;

    mostrarOrcamento(i);
    printf("\nSe o diagnostico nao encontrou defeito, informe 0 (RN18).\n");
    lerTexto("Valor da proposta em R$ (ENTER para voltar): ", texto, sizeof(texto));
    if (strlen(texto) == 0)
        return;

    valor = textoParaValor(texto);
    if (valor < 0)
    {
        printf("\n[!] Valor invalido. Nada foi gravado.");
        pausar();
        return;
    }

    ordens[i].orcamento = valor;
    strcpy(ordens[i].resultado_resposta_cliente, "");
    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    printf("\n[+] Orcamento de R$ %.2f enviado! A OS %d aguarda a resposta do cliente.", valor, ordens[i].id_os);
    pausar();
}

/* UC37 - Editar Orcamento: renegocia o valor depois de uma recusa */
void editarOrcamento()
{
    char texto[30];
    double valor;
    int i;

    imprimirCabecalho("Orcamentos > Editar");

    /* Só renegocia orçamento recusado pelo cliente */
    i = selecionarOS(ETAPA_RECUSADO);
    if (i < 0)
        return;

    mostrarOrcamento(i);
    lerTexto("\nNovo valor da proposta em R$ (ENTER para voltar): ", texto, sizeof(texto));
    if (strlen(texto) == 0)
        return;

    valor = textoParaValor(texto);
    if (valor < 0)
    {
        printf("\n[!] Valor invalido. Nada foi alterado.");
        pausar();
        return;
    }

    /* O novo valor substitui o anterior e a resposta volta a ficar vazia (aguardando o cliente) */
    ordens[i].orcamento = valor;
    strcpy(ordens[i].resultado_resposta_cliente, "");
    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    printf("\n[+] Orcamento reenviado com o valor de R$ %.2f! A OS %d aguarda nova resposta.", valor, ordens[i].id_os);
    pausar();
}

/* UC38 - Consultar Orcamento: mostra o valor e a situacao do orcamento */
void consultarOrcamento()
{
    char dono[60], situacao[25];
    int i, mostrados = 0;

    imprimirCabecalho("Orcamentos > Consultar");

    /* O cliente só vê os orçamentos das próprias OS (RI10) */
    for (i = 0; i < totalOrdens; i++)
    {
        if (!osNaEtapa(i, ETAPA_COM_ORCAMENTO))
            continue;

        if (mostrados == 0)
        {
            printf("\n  %-4s %-20s %-12s %-20s %s\n", "OS", "Cliente", "Valor (R$)", "Situacao", "Status da OS");
            printf("  ---------------------------------------------------------------------------\n");
        }

        nomeDoCliente(donoDoEquipamento(ordens[i].id_equipamento), dono);
        situacaoOrcamento(i, situacao);
        printf("  %-4d %-20.20s %-12.2f %-20s %s\n", ordens[i].id_os, dono, ordens[i].orcamento, situacao,
               ordens[i].status);
        mostrados++;
    }

    if (mostrados == 0)
    {
        printf("\n[!] Nenhuma OS com orcamento registrado.\n");
    }
    pausar();
}

/* Aprovar e Recusar fazem a mesma coisa, só muda a resposta gravada ("aprovado" ou "recusado") */
void responderOrcamento(char resposta[], char titulo[])
{
    char pergunta[60];
    int i;

    imprimirCabecalho(titulo);

    /* Só os orçamentos aguardando resposta, dentro do escopo do perfil (RN10) */
    i = selecionarOS(ETAPA_AGUARDANDO);
    if (i < 0)
        return;

    mostrarOrcamento(i);
    sprintf(pergunta, "\nConfirma o orcamento como %s?", resposta);
    if (!confirmar(pergunta))
    {
        printf("\nOperacao cancelada. O orcamento continua aguardando resposta.");
        pausar();
        return;
    }

    strcpy(ordens[i].resultado_resposta_cliente, resposta);
    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    if (strcmp(resposta, "aprovado") == 0)
        printf("\n[+] Orcamento aprovado! A OS %d esta liberada para a manutencao.", ordens[i].id_os);
    else
        printf("\n[+] Orcamento recusado. A OS %d pode ser renegociada ou arquivada.", ordens[i].id_os);
    pausar();
}

/* UC39 - Registrar Aprovacao do Orcamento: registra que o cliente aprovou */
void aprovarOrcamento()
{
    responderOrcamento("aprovado", "Orcamentos > Aprovar");
}

/* UC40 - Registrar Recusa do Orcamento: registra que o cliente recusou */
void recusarOrcamento()
{
    responderOrcamento("recusado", "Orcamentos > Recusar");
}

/* UC41 - Arquivar Orcamento: encerra a OS quando nao ha acordo */
void arquivarOrcamento()
{
    int i;

    imprimirCabecalho("Orcamentos > Arquivar");

    /* Só arquiva orçamento recusado */
    i = selecionarOS(ETAPA_RECUSADO);
    if (i < 0)
        return;

    mostrarOrcamento(i);
    if (!confirmar("\nArquivar esta OS? Ela sera encerrada sem reparo."))
    {
        printf("\nOperacao cancelada. A OS nao foi alterada.");
        pausar();
        return;
    }

    /* "Arquivada" é a OS cancelada por recusa: o registro continua no histórico (RN09) */
    strcpy(ordens[i].status, "cancelada");
    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    printf("\n[+] OS %d arquivada. O historico foi preservado.", ordens[i].id_os);
    pausar();
}

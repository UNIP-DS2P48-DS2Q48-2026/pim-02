/* UC36 - Registrar Orcamento: registra o valor da proposta a partir do diagnostico */
void registrarOrcamento()
{
    double valor;
    int i;

    imprimirCabecalho("Orcamentos > Registrar");

    i = selecionarOS();
    if (i < 0)
        return;

    /* RF05: o orçamento vem depois do diagnóstico, e só uma vez (para mudar o valor use Editar) */
    if (strlen(ordens[i].diagnostico) == 0)
    {
        printf("\n[!] A OS %d ainda nao tem diagnostico.", ordens[i].id_os);
        pausar();
        return;
    }
    if (ordens[i].orcamento >= 0)
    {
        printf("\n[!] A OS %d ja tem orcamento.", ordens[i].id_os);
        pausar();
        return;
    }

    printf("\nDiagnostico: %s\n", ordens[i].diagnostico);
    printf("Se nao houve defeito, informe 0 (RN18).\n");
    valor = lerValor("Valor da proposta em R$: ");
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
    double valor;
    int i;

    imprimirCabecalho("Orcamentos > Editar");

    i = selecionarOS();
    if (i < 0)
        return;

    if (strcmp(ordens[i].resultado_resposta_cliente, "recusado") != 0)
    {
        printf("\n[!] So um orcamento recusado pode ser renegociado.");
        pausar();
        return;
    }

    printf("\nValor recusado: R$ %.2f\n", ordens[i].orcamento);
    valor = lerValor("Novo valor em R$: ");
    if (valor < 0)
    {
        printf("\n[!] Valor invalido. Nada foi alterado.");
        pausar();
        return;
    }

    /* O novo valor volta a aguardar a resposta do cliente */
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

/* UC38 - Consultar Orcamento: mostra o valor e a resposta de cada orcamento */
void consultarOrcamento()
{
    char dono[60];
    int i, mostrados = 0;

    imprimirCabecalho("Orcamentos > Consultar");

    for (i = 0; i < totalOrdens; i++)
    {
        /* O cliente só vê os orçamentos das próprias OS (RI10) */
        if (!podeVerOS(i) || ordens[i].orcamento < 0)
            continue;

        if (mostrados == 0)
        {
            printf("\n  %-4s %-20s %-12s %-12s %s\n", "OS", "Cliente", "Valor (R$)", "Resposta", "Status da OS");
            printf("  ---------------------------------------------------------------------------\n");
        }
        nomeDoCliente(donoDoEquipamento(ordens[i].id_equipamento), dono);
        printf("  %-4d %-20.20s %-12.2f %-12s %s\n", ordens[i].id_os, dono, ordens[i].orcamento,
               ordens[i].resultado_resposta_cliente, ordens[i].status);
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
    int i;

    imprimirCabecalho(titulo);

    i = selecionarOS();
    if (i < 0)
        return;

    if (ordens[i].orcamento < 0 || strlen(ordens[i].resultado_resposta_cliente) > 0)
    {
        printf("\n[!] A OS %d nao tem orcamento aguardando resposta.", ordens[i].id_os);
        pausar();
        return;
    }

    printf("\nDiagnostico: %s\n", ordens[i].diagnostico);
    printf("Valor:       R$ %.2f\n", ordens[i].orcamento);
    if (!confirmar("\nConfirma a resposta?"))
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

    printf("\n[+] Orcamento %s na OS %d.", resposta, ordens[i].id_os);
    pausar();
}

/* UC39 - Registrar Aprovacao do Orcamento: a OS fica liberada para a manutencao */
void aprovarOrcamento()
{
    responderOrcamento("aprovado", "Orcamentos > Aprovar");
}

/* UC40 - Registrar Recusa do Orcamento: a OS pode ser renegociada ou arquivada */
void recusarOrcamento()
{
    responderOrcamento("recusado", "Orcamentos > Recusar");
}

/* UC41 - Arquivar Orcamento: encerra a OS quando nao ha acordo */
void arquivarOrcamento()
{
    int i;

    imprimirCabecalho("Orcamentos > Arquivar");

    i = selecionarOS();
    if (i < 0)
        return;

    if (strcmp(ordens[i].resultado_resposta_cliente, "recusado") != 0)
    {
        printf("\n[!] So um orcamento recusado pode ser arquivado.");
        pausar();
        return;
    }
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

/* orcamento.c - orçamento da OS: registrar, editar, consultar, aprovar, recusar e arquivar */
/* Funções do main.c usadas aqui: lerValor (linha 149), confirmar (linha 161), pausar (linha 173), imprimirCabecalho (linha 215), dataHoraAgora (linha 204) */

/* UC36 - Registrar Orcamento: valor da proposta para uma OS já diagnosticada */
void registrarOrcamento()
{
    double valor;
    int posicaoOS;

    imprimirCabecalho("Orcamentos > Registrar");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("aguardando orcamento", 0);
    if (posicaoOS < 0)
        return;

    printf("\nDiagnostico: %s\n", ordens[posicaoOS].diagnostico);
    printf("Se nao houve defeito, informe 0 (RN18).\n");
    valor = lerValor("Valor da proposta em R$ (ex: 95.50): ");
    if (valor < 0)
    {
        printf("\n[!] Valor invalido. Nada foi gravado.");
        pausar();
        return;
    }

    ordens[posicaoOS].orcamento = valor;
    strcpy(ordens[posicaoOS].resultado_resposta_cliente, "");
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] Orcamento de R$ %.2f enviado! A OS %d aguarda a resposta do cliente.", valor, ordens[posicaoOS].id_os);
    pausar();
}

/* UC37 - Editar Orcamento: novo valor depois de uma recusa */
void editarOrcamento()
{
    double valor;
    int posicaoOS;

    imprimirCabecalho("Orcamentos > Editar");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("orcamento recusado", 0);
    if (posicaoOS < 0)
        return;

    printf("\nValor recusado: R$ %.2f\n", ordens[posicaoOS].orcamento);
    valor = lerValor("Novo valor em R$ (ex: 95.50): ");
    if (valor < 0)
    {
        printf("\n[!] Valor invalido. Nada foi alterado.");
        pausar();
        return;
    }

    /* O novo valor volta a aguardar a resposta do cliente */
    ordens[posicaoOS].orcamento = valor;
    strcpy(ordens[posicaoOS].resultado_resposta_cliente, "");
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] Orcamento reenviado com o valor de R$ %.2f! A OS %d aguarda nova resposta.", valor, ordens[posicaoOS].id_os);
    pausar();
}

/* UC38 - Consultar Orcamento: valor e resposta de cada orçamento */
void consultarOrcamento()
{
    char dono[60], resposta[25];
    int posicaoOS, idCliente, mostrados = 0;

    imprimirCabecalho("Orcamentos > Consultar");

    for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
    {
        /* RN10: o cliente só vê os dele; -1 = OS sem orçamento */
        /* podeVerOS está em ordem_servico.c, linha 212 */
        if (podeVerOS(posicaoOS) && ordens[posicaoOS].orcamento >= 0)
        {
            /* donoDoEquipamento está em geral.c, linha 174 */
            idCliente = donoDoEquipamento(ordens[posicaoOS].id_equipamento);
            /* nomeDoCliente está em equipamento.c, linha 143 */
            nomeDoCliente(idCliente, dono);

            /* Resposta vazia = cliente ainda não respondeu */
            strcpy(resposta, ordens[posicaoOS].resultado_resposta_cliente);
            if (strlen(resposta) == 0)
                strcpy(resposta, "aguardando");

            printf("OS %d - %s - R$ %.2f - %s - %s\n", ordens[posicaoOS].id_os, dono, ordens[posicaoOS].orcamento, resposta,
                   ordens[posicaoOS].status);
            mostrados++;
        }
    }

    if (mostrados == 0)
        printf("\n[!] Nenhuma OS com orcamento registrado.\n");
    pausar();
}

/* Grava a resposta do cliente ("aprovado" ou "recusado") */
void responderOrcamento(char resposta[], char titulo[])
{
    int posicaoOS;

    imprimirCabecalho(titulo);

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("aguardando resposta", 0);
    if (posicaoOS < 0)
        return;

    printf("\nDiagnostico: %s\n", ordens[posicaoOS].diagnostico);
    printf("Valor: R$ %.2f\n", ordens[posicaoOS].orcamento);
    if (!confirmar("\nConfirma a resposta?"))
    {
        printf("\nOperacao cancelada. O orcamento continua aguardando resposta.");
        pausar();
        return;
    }

    strcpy(ordens[posicaoOS].resultado_resposta_cliente, resposta);
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] Orcamento %s na OS %d.", resposta, ordens[posicaoOS].id_os);
    pausar();
}

/* UC39 - Aprovar Orcamento: libera a OS para o reparo */
void aprovarOrcamento()
{
    /* responderOrcamento está em orcamento.c, linha 104 */
    responderOrcamento("aprovado", "Orcamentos > Aprovar");
}

/* UC40 - Recusar Orcamento: a OS pode ser renegociada ou arquivada */
void recusarOrcamento()
{
    /* responderOrcamento está em orcamento.c, linha 104 */
    responderOrcamento("recusado", "Orcamentos > Recusar");
}

/* UC41 - Arquivar Orcamento: encerra a OS como cancelada (RN09) */
void arquivarOrcamento()
{
    int posicaoOS;

    imprimirCabecalho("Orcamentos > Arquivar");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("orcamento recusado", 0);
    if (posicaoOS < 0)
        return;

    if (!confirmar("\nArquivar esta OS? Ela sera encerrada sem reparo."))
    {
        printf("\nOperacao cancelada. A OS nao foi alterada.");
        pausar();
        return;
    }

    strcpy(ordens[posicaoOS].status, "cancelada");
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] OS %d arquivada. O historico foi preservado.", ordens[posicaoOS].id_os);
    pausar();
}

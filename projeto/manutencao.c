/* ---------- Casos de uso da manutenção ---------- */

/* UC31 - Registrar Observacao Tecnica sobre Equipamento: o tecnico anota a vistoria na OS */
void registrarObservacao()
{
    char observacao[300];
    int i, k;

    imprimirCabecalho("Manutencao > Registrar Observacao Tecnica");

    i = selecionarOS();
    if (i < 0)
        return;
    if (strcmp(ordens[i].status, "aberta") == 0)
    {
        printf("\n[!] A OS %d ainda nao tem tecnico. Atribua um tecnico primeiro (UC28).", ordens[i].id_os);
        pausar();
        return;
    }

    printf("\nProblema relatado: %s\n", ordens[i].problema_relatado);

    /* Cada técnico tem a sua observação na OS (tabela tecnico_os) */
    k = buscarTecnicoNaOS(usuarios[usuarioLogado].id_usuario, ordens[i].id_os);
    if (k >= 0 && strlen(tecnicosOS[k].observacoes_tecnicas) > 0)
    {
        printf("Sua observacao atual: %s\n", tecnicosOS[k].observacoes_tecnicas);
    }

    lerTexto("\nObservacao tecnica (ENTER para voltar): ", observacao, sizeof(observacao));
    if (strlen(observacao) == 0)
        return;

    if (k < 0)
    {
        k = novaLinhaTecnicoOS(usuarios[usuarioLogado].id_usuario, ordens[i].id_os);
        if (k < 0)
            return;
    }

    strcpy(tecnicosOS[k].observacoes_tecnicas, observacao);
    dataHoraAgora(tecnicosOS[k].atualizado_em);
    if (!salvarTecnicosOS())
    {
        pausar();
        return;
    }

    printf("\n[+] Observacao registrada na OS %d!", ordens[i].id_os);
    pausar();
}

/* UC32 - Registrar Diagnostico do Equipamento: registra o diagnostico que libera o orcamento */
void registrarDiagnostico()
{
    char diagnostico[200];
    int i, k, t, opcao, observacoes = 0;

    imprimirCabecalho("Manutencao > Registrar Diagnostico");

    i = selecionarOS();
    if (i < 0)
        return;
    if (strcmp(ordens[i].status, "aberta") == 0)
    {
        printf("\n[!] A OS %d ainda nao tem tecnico. Atribua um tecnico primeiro (UC28).", ordens[i].id_os);
        pausar();
        return;
    }

    /* Depois do orçamento o diagnóstico fica travado, para não mudar o que o cliente vai aprovar */
    if (ordens[i].orcamento >= 0)
    {
        printf("\n[!] A OS %d ja tem orcamento registrado. O diagnostico nao pode mais ser alterado.", ordens[i].id_os);
        pausar();
        return;
    }

    printf("\nProblema relatado: %s\n", ordens[i].problema_relatado);

    /* Mostra as observações de todos os técnicos da OS (UC31) */
    printf("\nObservacoes tecnicas:\n");
    for (k = 0; k < totalTecnicosOS; k++)
    {
        if (tecnicosOS[k].id_os == ordens[i].id_os && strlen(tecnicosOS[k].observacoes_tecnicas) > 0)
        {
            t = buscarUsuarioPorId(tecnicosOS[k].id_tecnico);
            if (t >= 0)
                printf("  - %s: %s\n", usuarios[t].nome, tecnicosOS[k].observacoes_tecnicas);
            else
                printf("  - ?: %s\n", tecnicosOS[k].observacoes_tecnicas);
            observacoes++;
        }
    }

    /* A observação é opcional (RF05): só avisa */
    if (observacoes == 0)
    {
        printf("  (nenhuma observacao registrada)\n");
        if (!confirmar("\nA observacao tecnica e opcional. Deseja continuar mesmo assim?"))
            return;
    }

    if (strlen(ordens[i].diagnostico) > 0)
    {
        printf("\nDiagnostico atual: %s\n", ordens[i].diagnostico);
    }

    printf("\n  1 - Defeito encontrado\n");
    printf("  2 - Nenhum defeito encontrado\n");
    printf("  0 - Voltar\n\n");
    opcao = lerInteiro("Opcao: ");

    if (opcao == 1)
    {
        lerTexto("\nDiagnostico (situacao e servico necessario, ENTER para voltar): ", diagnostico, sizeof(diagnostico));
        if (strlen(diagnostico) == 0)
            return;
    }
    else if (opcao == 2)
    {
        /* RN18: sem defeito, a OS segue o fluxo normal com orçamento zero */
        strcpy(diagnostico, "Nenhum defeito encontrado, nenhum servico necessario");
    }
    else
    {
        return;
    }

    strcpy(ordens[i].diagnostico, diagnostico);
    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    printf("\n[+] Diagnostico registrado! A OS %d esta liberada para o orcamento.", ordens[i].id_os);
    pausar();
}

/* UC33 - Executar Manutencao: registra as acoes do reparo */
void executarManutencao()
{
    char acoes[200], texto[600];
    int i, k;

    imprimirCabecalho("Manutencao > Executar Manutencao");

    i = selecionarOS();
    if (i < 0)
        return;

    /* RF06: só repara depois que o cliente aprovou o orçamento */
    if (strcmp(ordens[i].resultado_resposta_cliente, "aprovado") != 0)
    {
        printf("\n[!] O orcamento da OS %d ainda nao foi aprovado pelo cliente (RF06).", ordens[i].id_os);
        pausar();
        return;
    }
    if (strcmp(ordens[i].resultado_teste, "aprovado") == 0)
    {
        printf("\n[!] A OS %d ja passou no teste e esta pronta para entrega.", ordens[i].id_os);
        pausar();
        return;
    }

    printf("\nDiagnostico: %s\n", ordens[i].diagnostico);
    if (ordens[i].orcamento == 0)
    {
        printf("Orcamento de valor zero: registre que nenhum reparo foi necessario (RN18).\n");
    }
    if (strcmp(ordens[i].resultado_teste, "reprovado") == 0)
    {
        printf("O ultimo teste foi reprovado: registre o novo reparo.\n");
    }

    lerTexto("\nAcoes realizadas (ENTER para voltar): ", acoes, sizeof(acoes));
    if (strlen(acoes) == 0)
        return;

    /* As ações ficam no mesmo campo das observações, depois do que já foi anotado */
    k = buscarTecnicoNaOS(usuarios[usuarioLogado].id_usuario, ordens[i].id_os);
    if (k >= 0 && strlen(tecnicosOS[k].observacoes_tecnicas) > 0)
        sprintf(texto, "%s | Reparo: %s", tecnicosOS[k].observacoes_tecnicas, acoes);
    else
        sprintf(texto, "Reparo: %s", acoes);

    if (strlen(texto) >= sizeof(tecnicosOS[0].observacoes_tecnicas))
    {
        printf("\n[!] O texto ficou maior que o espaco das observacoes. Escreva as acoes de forma mais curta.");
        pausar();
        return;
    }

    if (k < 0)
    {
        k = novaLinhaTecnicoOS(usuarios[usuarioLogado].id_usuario, ordens[i].id_os);
        if (k < 0)
            return;
    }

    strcpy(tecnicosOS[k].observacoes_tecnicas, texto);
    dataHoraAgora(tecnicosOS[k].atualizado_em);
    if (!salvarTecnicosOS())
    {
        pausar();
        return;
    }

    printf("\n[+] Reparo registrado na OS %d! Quando terminar, registre o teste.", ordens[i].id_os);
    pausar();
}

/* UC34 - Registrar Teste de Funcionamento: aprova ou reprova o reparo antes da entrega */
void registrarTeste()
{
    int i, opcao;

    imprimirCabecalho("Manutencao > Registrar Teste");

    i = selecionarOS();
    if (i < 0)
        return;

    if (strcmp(ordens[i].resultado_resposta_cliente, "aprovado") != 0)
    {
        printf("\n[!] O orcamento da OS %d ainda nao foi aprovado pelo cliente (RF06).", ordens[i].id_os);
        pausar();
        return;
    }
    if (strcmp(ordens[i].resultado_teste, "aprovado") == 0)
    {
        printf("\n[!] O teste da OS %d ja foi aprovado. Ela esta pronta para entrega.", ordens[i].id_os);
        pausar();
        return;
    }

    printf("\nDiagnostico: %s\n", ordens[i].diagnostico);
    printf("\nResultado do teste:\n");
    printf("  1 - Aprovado (os testes funcionaram e o desempenho esta bom)\n");
    printf("  2 - Reprovado (o equipamento ainda tem problema)\n");
    printf("  0 - Voltar\n\n");
    opcao = lerInteiro("Opcao: ");

    if (opcao == 1)
        strcpy(ordens[i].resultado_teste, "aprovado");
    else if (opcao == 2)
        strcpy(ordens[i].resultado_teste, "reprovado");
    else
        return;

    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    if (opcao == 1)
        printf("\n[+] Teste aprovado! A OS %d esta pronta para entrega.", ordens[i].id_os);
    else
        printf("\n[+] Teste reprovado. A OS %d volta para Executar Manutencao.", ordens[i].id_os);
    pausar();
}

/* UC35 - Informar Conclusao ao Cliente: lista as OS prontas para entrega e o contato do cliente */
void osProntasEntrega()
{
    char equipamento[90];
    int i, c, mostrados = 0;

    imprimirCabecalho("OS Prontas para Entrega");

    for (i = 0; i < totalOrdens; i++)
    {
        /* Pronta para entrega: em andamento, teste aprovado e ainda não entregue */
        if (strcmp(ordens[i].status, "em_andamento") != 0 || strcmp(ordens[i].resultado_teste, "aprovado") != 0 ||
            strlen(ordens[i].data_entrega) > 0)
            continue;

        if (mostrados == 0)
        {
            printf("\n  %-4s %-18s %-17s %-26s %s\n", "OS", "Cliente", "Telefone", "E-mail", "Equipamento");
            printf("  ---------------------------------------------------------------------------\n");
        }

        c = buscarUsuarioPorId(donoDoEquipamento(ordens[i].id_equipamento));
        nomeDoEquipamento(ordens[i].id_equipamento, equipamento);
        if (c >= 0)
            printf("  %-4d %-18.18s %-17.17s %-26.26s %s\n", ordens[i].id_os, usuarios[c].nome, usuarios[c].telefone,
                   usuarios[c].email, equipamento);
        else
            printf("  %-4d %-18s %-17s %-26s %s\n", ordens[i].id_os, "?", "", "", equipamento);
        mostrados++;
    }

    if (mostrados == 0)
    {
        printf("\n[!] Nenhuma OS pronta para entrega no momento.\n");
    }
    pausar();
}

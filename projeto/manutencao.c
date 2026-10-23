/* Funções do main.c usadas aqui: lerTexto (linha 121), lerInteiro (linha 137), confirmar (linha 161), pausar (linha 173), imprimirCabecalho (linha 214), dataHoraAgora (linha 204) */
/* manutencao.c - trabalho do técnico na OS: observação, diagnóstico, reparo e teste */

/* UC31 - Registrar Observacao Tecnica: o técnico anota a vistoria na OS */
void registrarObservacao()
{
    char observacao[300], texto[610];
    int posicaoOS, posicaoAtribuicao;

    imprimirCabecalho("Manutencao > Registrar Observacao Tecnica");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("", 0);
    if (posicaoOS < 0)
        return;
    if (strcmp(ordens[posicaoOS].status, "aberta") == 0)
    {
        printf("\n[!] A OS %d ainda nao tem tecnico. Atribua um tecnico primeiro (UC28).", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    printf("\nProblema relatado: %s\n", ordens[posicaoOS].problema_relatado);

    /* Cada técnico tem a própria observação na OS (-1 = ainda não está na OS) */
    /* buscarTecnicoNaOS está em geral.c, linha 187 */
    posicaoAtribuicao = buscarTecnicoNaOS(usuarios[usuarioLogado].id_usuario, ordens[posicaoOS].id_os);
    if (posicaoAtribuicao >= 0)
        printf("Sua observacao atual: %s\n", tecnicosOS[posicaoAtribuicao].observacoes_tecnicas);

    lerTexto("\nObservacao tecnica (ENTER para voltar): ", observacao, sizeof(observacao));
    if (strlen(observacao) == 0)
        return;

    /* Se o reparo já foi registrado, a observação entra depois dele para não apagá-lo */
    strcpy(texto, observacao);
    if (posicaoAtribuicao >= 0 && strstr(tecnicosOS[posicaoAtribuicao].observacoes_tecnicas, "Reparo:") != NULL)
    {
        strcpy(texto, tecnicosOS[posicaoAtribuicao].observacoes_tecnicas);
        strcat(texto, " | ");
        strcat(texto, observacao);
    }
    if (strlen(texto) >= sizeof(tecnicosOS[0].observacoes_tecnicas))
    {
        printf("\n[!] O texto ficou maior que o espaco das observacoes. Escreva de forma mais curta.");
        pausar();
        return;
    }

    /* Admin cobrindo o técnico (RN11) entra na OS agora */
    if (posicaoAtribuicao < 0)
    {
        /* novaLinhaTecnicoOS está em ordem_servico.c, linha 366 */
        posicaoAtribuicao = novaLinhaTecnicoOS(usuarios[usuarioLogado].id_usuario, ordens[posicaoOS].id_os);
        if (posicaoAtribuicao < 0)
            return;
    }

    strcpy(tecnicosOS[posicaoAtribuicao].observacoes_tecnicas, texto);
    dataHoraAgora(tecnicosOS[posicaoAtribuicao].atualizado_em);
    /* salvarTecnicosOS está em ordem_servico.c, linha 121 */
    if (salvarTecnicosOS())
        printf("\n[+] Observacao registrada na OS %d!", ordens[posicaoOS].id_os);
    pausar();
}

/* UC32 - Registrar Diagnostico: libera a OS para o orçamento */
void registrarDiagnostico()
{
    char diagnostico[200];
    int posicaoOS, posicaoAtribuicao, posicaoTecnico, opcao, observacoes = 0;

    imprimirCabecalho("Manutencao > Registrar Diagnostico");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("", 0);
    if (posicaoOS < 0)
        return;
    if (strcmp(ordens[posicaoOS].status, "aberta") == 0)
    {
        printf("\n[!] A OS %d ainda nao tem tecnico. Atribua um tecnico primeiro (UC28).", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    /* Depois do orçamento o diagnóstico não pode mais mudar */
    if (ordens[posicaoOS].orcamento >= 0)
    {
        printf("\n[!] A OS %d ja tem orcamento registrado. O diagnostico nao pode mais ser alterado.", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    printf("\nProblema relatado: %s\n", ordens[posicaoOS].problema_relatado);

    /* Mostra as observações de todos os técnicos da OS */
    printf("\nObservacoes tecnicas:\n");
    for (posicaoAtribuicao = 0; posicaoAtribuicao < totalTecnicosOS; posicaoAtribuicao++)
    {
        if (tecnicosOS[posicaoAtribuicao].id_os == ordens[posicaoOS].id_os && strlen(tecnicosOS[posicaoAtribuicao].observacoes_tecnicas) > 0)
        {
            /* buscarUsuarioPorId está em geral.c, linha 123 */
            posicaoTecnico = buscarUsuarioPorId(tecnicosOS[posicaoAtribuicao].id_tecnico);
            printf("- %s: %s\n", usuarios[posicaoTecnico].nome, tecnicosOS[posicaoAtribuicao].observacoes_tecnicas);
            observacoes++;
        }
    }

    /* A observação é opcional (RF05) */
    if (observacoes == 0)
    {
        printf("(nenhuma observacao registrada)\n");
        if (!confirmar("\nA observacao tecnica e opcional. Deseja continuar mesmo assim?"))
            return;
    }

    printf("\nDiagnostico atual: %s\n", ordens[posicaoOS].diagnostico);
    printf("1 - Defeito encontrado\n");
    printf("2 - Nenhum defeito encontrado\n");
    printf("0 - Voltar\n");
    opcao = lerInteiro("Opcao: ");

    switch (opcao)
    {
    case 1:
        lerTexto("\nDiagnostico (situacao e servico necessario, ENTER para voltar): ", diagnostico, sizeof(diagnostico));
        if (strlen(diagnostico) == 0)
            return;
        break;
    case 2:
        /* RN18: sem defeito, segue com orçamento zero */
        strcpy(diagnostico, "Nenhum defeito encontrado, nenhum servico necessario");
        break;
    default:
        return; /* 0 ou opção inválida: volta sem gravar */
    }

    strcpy(ordens[posicaoOS].diagnostico, diagnostico);
    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (salvarOrdens())
        printf("\n[+] Diagnostico registrado! A OS %d esta liberada para o orcamento.", ordens[posicaoOS].id_os);
    pausar();
}

/* UC33 - Executar Manutencao: registra o que foi feito no reparo */
void executarManutencao()
{
    char acoes[200], texto[600];
    int posicaoOS, posicaoAtribuicao;

    imprimirCabecalho("Manutencao > Executar Manutencao");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("", 0);
    if (posicaoOS < 0)
        return;

    /* RF06: só repara com o orçamento aprovado */
    if (strcmp(ordens[posicaoOS].resultado_resposta_cliente, "aprovado") != 0)
    {
        printf("\n[!] O orcamento da OS %d ainda nao foi aprovado pelo cliente (RF06).", ordens[posicaoOS].id_os);
        pausar();
        return;
    }
    if (strcmp(ordens[posicaoOS].resultado_teste, "aprovado") == 0)
    {
        printf("\n[!] A OS %d ja passou no teste e esta pronta para entrega.", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    printf("\nDiagnostico: %s\n", ordens[posicaoOS].diagnostico);
    if (ordens[posicaoOS].orcamento == 0)
        printf("Orcamento de valor zero: registre que nenhum reparo foi necessario (RN18).\n");
    if (strcmp(ordens[posicaoOS].resultado_teste, "reprovado") == 0)
        printf("O ultimo teste foi reprovado: registre o novo reparo.\n");

    lerTexto("\nAcoes realizadas (ENTER para voltar): ", acoes, sizeof(acoes));
    if (strlen(acoes) == 0)
        return;

    /* O reparo é gravado no fim das observações do técnico */
    /* buscarTecnicoNaOS está em geral.c, linha 187 */
    posicaoAtribuicao = buscarTecnicoNaOS(usuarios[usuarioLogado].id_usuario, ordens[posicaoOS].id_os);
    strcpy(texto, "");
    if (posicaoAtribuicao >= 0 && strlen(tecnicosOS[posicaoAtribuicao].observacoes_tecnicas) > 0)
    {
        strcpy(texto, tecnicosOS[posicaoAtribuicao].observacoes_tecnicas);
        strcat(texto, " | ");
    }
    strcat(texto, "Reparo: ");
    strcat(texto, acoes);
    if (strlen(texto) >= sizeof(tecnicosOS[0].observacoes_tecnicas))
    {
        printf("\n[!] O texto ficou maior que o espaco das observacoes. Escreva as acoes de forma mais curta.");
        pausar();
        return;
    }

    if (posicaoAtribuicao < 0)
    {
        /* novaLinhaTecnicoOS está em ordem_servico.c, linha 366 */
        posicaoAtribuicao = novaLinhaTecnicoOS(usuarios[usuarioLogado].id_usuario, ordens[posicaoOS].id_os);
        if (posicaoAtribuicao < 0)
            return;
    }

    strcpy(tecnicosOS[posicaoAtribuicao].observacoes_tecnicas, texto);
    dataHoraAgora(tecnicosOS[posicaoAtribuicao].atualizado_em);
    /* salvarTecnicosOS está em ordem_servico.c, linha 121 */
    if (!salvarTecnicosOS())
    {
        pausar();
        return;
    }

    /* Depois de um teste reprovado, o novo reparo libera outro teste */
    if (strcmp(ordens[posicaoOS].resultado_teste, "reprovado") == 0)
    {
        strcpy(ordens[posicaoOS].resultado_teste, "");
        dataHoraAgora(ordens[posicaoOS].atualizado_em);
        /* salvarOrdens está em ordem_servico.c, linha 9 */
        if (!salvarOrdens())
        {
            pausar();
            return;
        }
    }

    printf("\n[+] Reparo registrado na OS %d! Quando terminar, registre o teste.", ordens[posicaoOS].id_os);
    pausar();
}

/* UC34 - Registrar Teste: aprova ou reprova o reparo */
void registrarTeste()
{
    int posicaoOS, posicaoAtribuicao, temReparo = 0, opcao;

    imprimirCabecalho("Manutencao > Registrar Teste");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("em reparo", 0);
    if (posicaoOS < 0)
        return;

    /* Só testa se já existe um reparo registrado */
    for (posicaoAtribuicao = 0; posicaoAtribuicao < totalTecnicosOS; posicaoAtribuicao++)
    {
        if (tecnicosOS[posicaoAtribuicao].id_os == ordens[posicaoOS].id_os && strstr(tecnicosOS[posicaoAtribuicao].observacoes_tecnicas, "Reparo:") != NULL)
            temReparo = 1;
    }
    if (!temReparo)
    {
        printf("\n[!] Nenhum reparo registrado na OS %d. Use Executar Manutencao antes do teste.", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    /* Exceção 6a/7a: teste reprovado exige um novo reparo antes de outro teste */
    if (strcmp(ordens[posicaoOS].resultado_teste, "reprovado") == 0)
    {
        printf("\n[!] O ultimo teste da OS %d foi reprovado. Registre o novo reparo em Executar Manutencao.", ordens[posicaoOS].id_os);
        pausar();
        return;
    }

    printf("\nDiagnostico: %s\n", ordens[posicaoOS].diagnostico);
    printf("Resultado do teste:\n");
    printf("1 - Aprovado (os testes funcionaram e o desempenho esta bom)\n");
    printf("2 - Reprovado (o equipamento ainda tem problema)\n");
    printf("0 - Voltar\n");
    opcao = lerInteiro("Opcao: ");

    switch (opcao)
    {
    case 1:
        strcpy(ordens[posicaoOS].resultado_teste, "aprovado");
        break;
    case 2:
        strcpy(ordens[posicaoOS].resultado_teste, "reprovado");
        break;
    default:
        return; /* 0 ou opção inválida: volta sem gravar */
    }

    dataHoraAgora(ordens[posicaoOS].atualizado_em);
    /* salvarOrdens está em ordem_servico.c, linha 9 */
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    if (opcao == 1)
        printf("\n[+] Teste aprovado! A OS %d esta pronta para entrega.", ordens[posicaoOS].id_os);
    else
        printf("\n[+] Teste reprovado. A OS %d volta para Executar Manutencao.", ordens[posicaoOS].id_os);
    pausar();
}

/* UC35 - Informar Conclusao ao Cliente: mostra o contato do cliente das OS prontas */
void listarProntasParaEntrega()
{
    int posicaoOS, idCliente, posicaoCliente;

    imprimirCabecalho("OS Prontas para Entrega");

    /* selecionarOS está em ordem_servico.c, linha 317 */
    posicaoOS = selecionarOS("pronta para entrega", 0);
    if (posicaoOS < 0)
        return;

    /* Contato do dono do equipamento */
    /* donoDoEquipamento está em geral.c, linha 174 */
    idCliente = donoDoEquipamento(ordens[posicaoOS].id_equipamento);
    /* buscarUsuarioPorId está em geral.c, linha 123 */
    posicaoCliente = buscarUsuarioPorId(idCliente);
    printf("\nContato para combinar a retirada da OS %d:\n", ordens[posicaoOS].id_os);
    printf("Cliente: %s\n", usuarios[posicaoCliente].nome);
    printf("Telefone: %s\n", usuarios[posicaoCliente].telefone);
    printf("E-mail: %s\n", usuarios[posicaoCliente].email);
    pausar();
}

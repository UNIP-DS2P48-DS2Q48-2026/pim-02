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

/* Funções que ficam em arquivos incluídos depois deste */
double textoParaValor(char texto[]); /* orcamento.c */
int dataValida(char data[]);         /* relatorio.c */

/* Etapas usadas para escolher quais OS aparecem em cada lista */
#define ETAPA_TODAS 0          /* qualquer OS que o usuário logado pode ver */
#define ETAPA_MANUTENCAO 1     /* OS em andamento que o usuário logado pode atender */
#define ETAPA_PRONTA_ENTREGA 2 /* teste aprovado e ainda não entregue */
#define ETAPA_SEM_ORCAMENTO 3  /* diagnóstico registrado e sem orçamento */
#define ETAPA_AGUARDANDO 4     /* orçamento enviado, sem resposta do cliente */
#define ETAPA_RECUSADO 5       /* orçamento recusado pelo cliente */
#define ETAPA_COM_ORCAMENTO 6  /* qualquer OS que já tem orçamento */
#define ETAPA_EM_ABERTO 7      /* aberta ou em andamento: pode cancelar e atribuir técnico */
#define ETAPA_EDITAVEL 8       /* em aberto; o Admin edita também a finalizada (RN08) */
#define ETAPA_FEEDBACK 9       /* entregue e esperando a resposta do cliente */

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

/* Posição no vetor tecnicosOS da linha deste técnico nesta OS (-1 se não existir) */
int buscarTecnicoNaOS(int idTecnico, int idOS)
{
    int i;

    for (i = 0; i < totalTecnicosOS; i++)
    {
        if (tecnicosOS[i].id_tecnico == idTecnico && tecnicosOS[i].id_os == idOS)
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
        return tecnicoEstaNaOS(idLogado, ordens[i].id_os);
    }
    return 1;
}

/* A OS está na etapa pedida? (e o usuário logado pode vê-la?) */
int osNaEtapa(int i, int etapa)
{
    int emAndamento = 0;

    if (!podeVerOS(i))
        return 0;

    /* RN08: OS finalizada ou cancelada não volta para as etapas de trabalho */
    if (strcmp(ordens[i].status, "em_andamento") == 0)
        emAndamento = 1;

    if (etapa == ETAPA_TODAS)
    {
        return 1;
    }
    if (etapa == ETAPA_EM_ABERTO)
    {
        if (emAndamento || strcmp(ordens[i].status, "aberta") == 0)
            return 1;
        return 0;
    }
    if (etapa == ETAPA_EDITAVEL)
    {
        if (emAndamento || strcmp(ordens[i].status, "aberta") == 0)
            return 1;
        if (perfilLogado("admin") && strcmp(ordens[i].status, "finalizada") == 0)
            return 1;
        return 0;
    }
    if (etapa == ETAPA_FEEDBACK)
    {
        if (emAndamento && strlen(ordens[i].data_entrega) > 0)
            return 1;
        return 0;
    }
    if (etapa == ETAPA_MANUTENCAO)
    {
        return emAndamento;
    }
    if (etapa == ETAPA_PRONTA_ENTREGA)
    {
        if (emAndamento && strcmp(ordens[i].resultado_teste, "aprovado") == 0 && strlen(ordens[i].data_entrega) == 0)
            return 1;
        return 0;
    }
    if (etapa == ETAPA_SEM_ORCAMENTO)
    {
        if (emAndamento && strlen(ordens[i].diagnostico) > 0 && ordens[i].orcamento < 0)
            return 1;
        return 0;
    }
    if (etapa == ETAPA_AGUARDANDO)
    {
        if (emAndamento && ordens[i].orcamento >= 0 && strlen(ordens[i].resultado_resposta_cliente) == 0)
            return 1;
        return 0;
    }
    if (etapa == ETAPA_RECUSADO)
    {
        if (emAndamento && strcmp(ordens[i].resultado_resposta_cliente, "recusado") == 0)
            return 1;
        return 0;
    }
    if (etapa == ETAPA_COM_ORCAMENTO)
    {
        if (ordens[i].orcamento >= 0)
            return 1;
        return 0;
    }
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

/* Lista as OS que estão na etapa pedida; devolve quantas mostrou */
int listarOS(int etapa)
{
    char dono[60], equipamento[90];
    int i, mostrados = 0;

    for (i = 0; i < totalOrdens; i++)
    {
        if (!osNaEtapa(i, etapa))
            continue;

        /* Cabeçalho da tabela antes da primeira da lista */
        if (mostrados == 0)
        {
            printf("\n  %-4s %-11s %-18s %-22s %s\n", "OS", "Abertura", "Cliente", "Equipamento", "Status");
            printf("  ---------------------------------------------------------------------------\n");
        }

        nomeDoCliente(donoDoEquipamento(ordens[i].id_equipamento), dono);
        nomeDoEquipamento(ordens[i].id_equipamento, equipamento);
        printf("  %-4d %-11s %-18.18s %-22.22s %s\n", ordens[i].id_os, ordens[i].data_abertura, dono, equipamento,
               ordens[i].status);
        mostrados++;
    }
    return mostrados;
}

/* Lista as OS da etapa e pede o número. Devolve a posição no vetor ou -1 (voltou ou número inválido) */
int selecionarOS(int etapa)
{
    int id, i;

    if (listarOS(etapa) == 0)
    {
        printf("\n[!] Nenhuma ordem de servico disponivel para esta operacao.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nNumero da OS (0 para voltar): ");
    if (id <= 0)
        return -1;

    i = buscarOSPorId(id);
    if (i < 0 || !osNaEtapa(i, etapa))
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

/* ---------- Casos de uso da ordem de servico ---------- */

/* Copia para destino só os números do texto (CPF com ou sem ponto e traço) */
void soNumeros(char texto[], char destino[])
{
    int i, j = 0;

    for (i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] >= '0' && texto[i] <= '9')
        {
            destino[j] = texto[i];
            j++;
        }
    }
    destino[j] = '\0';
}

/* UC25: copia para destino a etapa da OS, deduzida dos campos preenchidos */
void etapaDaOS(int i, char destino[])
{
    if (strcmp(ordens[i].status, "aberta") == 0)
        strcpy(destino, "aguardando tecnico");
    else if (strcmp(ordens[i].status, "em_andamento") != 0)
        strcpy(destino, ordens[i].status); /* finalizada ou cancelada */
    else if (strlen(ordens[i].data_entrega) > 0)
        strcpy(destino, "aguardando feedback");
    else if (strcmp(ordens[i].resultado_teste, "aprovado") == 0)
        strcpy(destino, "pronta para entrega");
    else if (strcmp(ordens[i].resultado_resposta_cliente, "aprovado") == 0)
        strcpy(destino, "em reparo");
    else if (strcmp(ordens[i].resultado_resposta_cliente, "recusado") == 0)
        strcpy(destino, "orcamento recusado");
    else if (ordens[i].orcamento >= 0)
        strcpy(destino, "aguardando resposta");
    else if (strlen(ordens[i].diagnostico) > 0)
        strcpy(destino, "aguardando orcamento");
    else
        strcpy(destino, "em vistoria");
}

/* Mostra todos os dados da OS: cliente, equipamento, andamento e técnicos */
void mostrarOS(int i)
{
    char etapa[25], equipamento[90];
    int c, a, k, t, e, tecnicos = 0;

    etapaDaOS(i, etapa);
    nomeDoEquipamento(ordens[i].id_equipamento, equipamento);
    c = buscarUsuarioPorId(donoDoEquipamento(ordens[i].id_equipamento));
    a = buscarUsuarioPorId(ordens[i].id_atendente);
    e = buscarEquipamentoPorId(ordens[i].id_equipamento);

    if (strcmp(etapa, ordens[i].status) == 0)
        printf("\nOS %d - %s\n", ordens[i].id_os, ordens[i].status);
    else
        printf("\nOS %d - %s (%s)\n", ordens[i].id_os, ordens[i].status, etapa);
    if (c >= 0)
        printf("  Cliente:     %s - CPF %s - %s\n", usuarios[c].nome, usuarios[c].cpf, usuarios[c].telefone);
    if (e >= 0)
        printf("  Equipamento: %s (%s)\n", equipamento, equipamentos[e].modelo_numero_serie);
    if (a >= 0)
        printf("  Abertura:    %s, por %s\n", ordens[i].data_abertura, usuarios[a].nome);
    else
        printf("  Abertura:    %s\n", ordens[i].data_abertura);
    printf("  Problema:    %s\n", ordens[i].problema_relatado);

    if (strlen(ordens[i].diagnostico) > 0)
        printf("  Diagnostico: %s\n", ordens[i].diagnostico);
    else
        printf("  Diagnostico: (ainda nao registrado)\n");

    if (ordens[i].orcamento >= 0)
    {
        if (strlen(ordens[i].resultado_resposta_cliente) > 0)
            printf("  Orcamento:   R$ %.2f (%s)\n", ordens[i].orcamento, ordens[i].resultado_resposta_cliente);
        else
            printf("  Orcamento:   R$ %.2f (aguardando resposta)\n", ordens[i].orcamento);
    }
    if (strlen(ordens[i].resultado_teste) > 0)
        printf("  Teste:       %s\n", ordens[i].resultado_teste);
    if (strlen(ordens[i].data_entrega) > 0)
        printf("  Entrega:     %s - pago R$ %.2f\n", ordens[i].data_entrega, ordens[i].valor_pago);
    if (strlen(ordens[i].satisfeito) > 0)
        printf("  Feedback:    satisfeito: %s - %s\n", ordens[i].satisfeito, ordens[i].feedback_cliente);

    printf("  Tecnicos:\n");
    for (k = 0; k < totalTecnicosOS; k++)
    {
        if (tecnicosOS[k].id_os != ordens[i].id_os)
            continue;
        t = buscarUsuarioPorId(tecnicosOS[k].id_tecnico);
        if (t >= 0)
            printf("    - %s (desde %s): %s\n", usuarios[t].nome, tecnicosOS[k].data_atribuicao,
                   tecnicosOS[k].observacoes_tecnicas);
        tecnicos++;
    }
    if (tecnicos == 0)
        printf("    (nenhum)\n");
}

/* UC25: a OS bate com o filtro escolhido? (1 todas, 2 número, 3 cliente, 4 status/etapa, 5 período) */
int osBateFiltro(int i, int tipo, char filtro[], char fim[])
{
    char etapa[25], cpf[20], numeros[20];
    int c;

    if (tipo == 2)
    {
        if (ordens[i].id_os == atoi(filtro))
            return 1;
        return 0;
    }
    if (tipo == 3)
    {
        c = buscarUsuarioPorId(donoDoEquipamento(ordens[i].id_equipamento));
        if (c < 0)
            return 0;
        if (contemTexto(usuarios[c].nome, filtro))
            return 1;
        /* CPF com ou sem pontuação */
        soNumeros(usuarios[c].cpf, cpf);
        soNumeros(filtro, numeros);
        if (strlen(numeros) > 0 && strstr(cpf, numeros) != NULL)
            return 1;
        return 0;
    }
    if (tipo == 4)
    {
        etapaDaOS(i, etapa);
        if (contemTexto(ordens[i].status, filtro) || contemTexto(etapa, filtro))
            return 1;
        return 0;
    }
    if (tipo == 5)
    {
        /* Datas AAAA-MM-DD podem ser comparadas como texto com strcmp */
        if (strcmp(ordens[i].data_abertura, filtro) >= 0 && strcmp(ordens[i].data_abertura, fim) <= 0)
            return 1;
        return 0;
    }
    return 1;
}

/* Lista os técnicos e admins ativos que ainda não estão na OS e atribui um ou mais (UC28, RN04) */
void atribuirTecnicosNaOS(int i)
{
    int k, t, id, mostrados;

    do
    {
        printf("\nTecnicos ja atribuidos a OS %d: ", ordens[i].id_os);
        mostrados = 0;
        for (k = 0; k < totalTecnicosOS; k++)
        {
            if (tecnicosOS[k].id_os != ordens[i].id_os)
                continue;
            t = buscarUsuarioPorId(tecnicosOS[k].id_tecnico);
            if (mostrados > 0)
                printf(", ");
            if (t >= 0)
                printf("%s", usuarios[t].nome);
            mostrados++;
        }
        if (mostrados == 0)
            printf("(nenhum)");
        printf("\n");

        /* RN15: só técnico ou, na falta dele, admin (RN11); RN14: só ativos */
        mostrados = 0;
        for (t = 0; t < totalUsuarios; t++)
        {
            if (strcmp(usuarios[t].status, "ativo") != 0)
                continue;
            if (strcmp(usuarios[t].perfil, "tecnico") != 0 && strcmp(usuarios[t].perfil, "admin") != 0)
                continue;
            if (tecnicoEstaNaOS(usuarios[t].id_usuario, ordens[i].id_os))
                continue;

            if (mostrados == 0)
            {
                printf("\n  %-4s %-25s %-10s %s\n", "ID", "Nome", "Perfil", "Especialidade");
                printf("  ---------------------------------------------------------------------------\n");
            }
            printf("  %-4d %-25.25s %-10s %s\n", usuarios[t].id_usuario, usuarios[t].nome, usuarios[t].perfil,
                   usuarios[t].especialidade);
            mostrados++;
        }
        if (mostrados == 0)
        {
            printf("\n[!] Nao ha outro tecnico ativo para atribuir.");
            pausar();
            return;
        }

        id = lerInteiro("\nID do tecnico (0 para voltar): ");
        if (id <= 0)
            return;

        t = buscarUsuarioPorId(id);
        if (t < 0 || (strcmp(usuarios[t].perfil, "tecnico") != 0 && strcmp(usuarios[t].perfil, "admin") != 0))
        {
            printf("\n[!] O usuario %d nao e tecnico nem admin (RN15).", id);
            pausar();
            return;
        }
        if (strcmp(usuarios[t].status, "ativo") != 0)
        {
            printf("\n[!] %s esta inativo e nao pode ser atribuido (RN14).", usuarios[t].nome);
            pausar();
            return;
        }
        if (tecnicoEstaNaOS(id, ordens[i].id_os))
        {
            printf("\n[!] %s ja esta nesta OS.", usuarios[t].nome);
            pausar();
            return;
        }

        if (novaLinhaTecnicoOS(id, ordens[i].id_os) < 0)
            return;
        if (!salvarTecnicosOS())
        {
            totalTecnicosOS--; /* desfaz a linha que não foi gravada */
            pausar();
            return;
        }

        /* Com técnico atribuído, a OS sai de "aberta" e vai para a vistoria (UC31/UC32) */
        if (strcmp(ordens[i].status, "aberta") == 0)
        {
            strcpy(ordens[i].status, "em_andamento");
            dataHoraAgora(ordens[i].atualizado_em);
            if (!salvarOrdens())
            {
                pausar();
                return;
            }
        }

        printf("\n[+] %s atribuido(a) a OS %d!", usuarios[t].nome, ordens[i].id_os);
    } while (confirmar("\n\nAtribuir outro tecnico a esta OS?"));
}

/* UC24 - Criar Ordem de Servico: abre uma OS para um equipamento do cliente */
void criarOrdemServico()
{
    char cpf[20], numeros[20], cpfCliente[20], problema[200];
    int i, c = -1, e, n, id, mostrados = 0, maiorId = 0;

    imprimirCabecalho("Ordens de Servico > Criar");

    if (totalOrdens >= MAX_OS)
    {
        printf("\n[!] Limite de %d ordens de servico atingido.", MAX_OS);
        pausar();
        return;
    }

    lerTexto("\nCPF do cliente, com ou sem pontuacao (ENTER para voltar): ", cpf, sizeof(cpf));
    soNumeros(cpf, numeros);
    if (strlen(numeros) == 0)
        return;

    for (i = 0; i < totalUsuarios; i++)
    {
        soNumeros(usuarios[i].cpf, cpfCliente);
        if (strcmp(usuarios[i].perfil, "cliente") == 0 && strcmp(cpfCliente, numeros) == 0)
            c = i;
    }

    /* RN01/RI02: a OS só é aberta para cliente e equipamento já cadastrados */
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

    printf("\nCliente: %s\n", usuarios[c].nome);
    for (e = 0; e < totalEquipamentos; e++)
    {
        if (equipamentos[e].id_cliente != usuarios[c].id_usuario)
            continue;

        if (mostrados == 0)
        {
            printf("\n  %-4s %-8s %-14s %-12s %-22s %s\n", "ID", "Categ.", "Tipo", "Marca", "Modelo/Serie", "Status");
            printf("  ---------------------------------------------------------------------------\n");
        }
        printf("  %-4d %-8s %-14.14s %-12.12s %-22.22s %s\n", equipamentos[e].id_equipamento,
               equipamentos[e].categoria, equipamentos[e].tipo, equipamentos[e].marca,
               equipamentos[e].modelo_numero_serie, equipamentos[e].status);
        mostrados++;
    }
    if (mostrados == 0)
    {
        printf("\n[!] O cliente nao tem equipamento cadastrado. Cadastre em Equipamentos > Cadastrar (RN02).");
        pausar();
        return;
    }

    id = lerInteiro("\nID do equipamento (0 para voltar): ");
    if (id <= 0)
        return;

    e = buscarEquipamentoPorId(id);
    if (e < 0 || equipamentos[e].id_cliente != usuarios[c].id_usuario)
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return;
    }
    if (strcmp(equipamentos[e].status, "ativo") != 0)
    {
        printf("\n[!] O equipamento %d esta inativo. Nao e possivel abrir OS (RN14).", id);
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

    /* Preenche a próxima posição e só faz total++ no fim */
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
        totalOrdens--; /* desfaz o registro que não foi gravado */
        pausar();
        return;
    }

    printf("\n[+] OS %d criada com status \"aberta\"!", ordens[n].id_os);
    if (confirmar("\n\nDeseja atribuir um tecnico agora?"))
        atribuirTecnicosNaOS(n);
}

/* UC25 - Consultar Ordem de Servico: lista e busca as OS por numero, CPF, status ou periodo */
void consultarOS()
{
    char filtro[60], fim[TAM_DATA + 5], dono[60], equipamento[90], etapa[25];
    int tipo, i, id, mostrados = 0;

    imprimirCabecalho("Ordens de Servico > Consultar");

    printf("\n  1 - Listar todas\n");
    printf("  2 - Por numero da OS\n");
    printf("  3 - Por cliente (nome ou CPF)\n");
    printf("  4 - Por status ou etapa\n");
    printf("  5 - Por periodo de abertura\n");
    printf("  0 - Voltar\n\n");
    tipo = lerInteiro("Opcao: ");

    strcpy(filtro, "");
    strcpy(fim, "");
    if (tipo == 0)
    {
        return;
    }
    else if (tipo == 2)
    {
        lerTexto("\nNumero da OS: ", filtro, sizeof(filtro));
    }
    else if (tipo == 3)
    {
        lerTexto("\nNome ou CPF do cliente: ", filtro, sizeof(filtro));
    }
    else if (tipo == 4)
    {
        printf("\nStatus: aberta, em_andamento, finalizada, cancelada\n");
        printf("Etapas: aguardando tecnico, em vistoria, aguardando orcamento, aguardando resposta,\n");
        printf("        orcamento recusado, em reparo, pronta para entrega, aguardando feedback\n");
        lerTexto("Status ou etapa: ", filtro, sizeof(filtro));
    }
    else if (tipo == 5)
    {
        lerTexto("\nData inicial AAAA-MM-DD (ENTER para desde o inicio): ", filtro, sizeof(filtro));
        lerTexto("Data final AAAA-MM-DD (ENTER para ate hoje): ", fim, sizeof(fim));
        if (strlen(filtro) == 0)
            strcpy(filtro, "0000-01-01");
        if (strlen(fim) == 0)
            dataHoje(fim);
        if (!dataValida(filtro) || !dataValida(fim))
        {
            printf("\n[!] Data invalida. Use o formato AAAA-MM-DD (ex: 2026-10-01).");
            pausar();
            return;
        }
    }
    else if (tipo != 1)
    {
        printf("\n[!] Opcao invalida!");
        pausar();
        return;
    }

    /* RN10/RI10: só as OS que o perfil pode ver, inclusive finalizadas e canceladas (RN09) */
    for (i = 0; i < totalOrdens; i++)
    {
        if (!osNaEtapa(i, ETAPA_TODAS) || !osBateFiltro(i, tipo, filtro, fim))
            continue;

        if (mostrados == 0)
        {
            printf("\n  %-4s %-11s %-18s %-20s %s\n", "OS", "Abertura", "Cliente", "Equipamento", "Etapa");
            printf("  ---------------------------------------------------------------------------\n");
        }
        nomeDoCliente(donoDoEquipamento(ordens[i].id_equipamento), dono);
        nomeDoEquipamento(ordens[i].id_equipamento, equipamento);
        etapaDaOS(i, etapa);
        printf("  %-4d %-11s %-18.18s %-20.20s %s\n", ordens[i].id_os, ordens[i].data_abertura, dono, equipamento,
               etapa);
        mostrados++;
    }

    if (mostrados == 0)
    {
        printf("\n[!] Nenhuma OS encontrada para o filtro informado.");
        pausar();
        return;
    }

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

    /* RN08: finalizada e cancelada não são editadas (finalizada só pelo Admin) */
    i = selecionarOS(ETAPA_EDITAVEL);
    if (i < 0)
        return;

    mostrarOS(i);

    /* RN07: o atendente altera só o dado administrativo; diagnóstico e teste são do técnico */
    printf("\nSo o problema relatado pode ser alterado aqui (RN07). ENTER mantem o valor atual.\n");
    lerOuManter("Problema relatado", ordens[i].problema_relatado, problema, sizeof(problema));

    if (strcmp(problema, ordens[i].problema_relatado) == 0)
    {
        printf("\nNada foi alterado.");
        pausar();
        return;
    }

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

    /* RN08: OS finalizada não pode ser cancelada */
    i = selecionarOS(ETAPA_EM_ABERTO);
    if (i < 0)
        return;

    mostrarOS(i);
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

/* UC28 - Atribuir Tecnico a Ordem de Servico: vincula um ou mais tecnicos a OS */
void atribuirTecnico()
{
    int i;

    imprimirCabecalho("Ordens de Servico > Atribuir Tecnico");

    i = selecionarOS(ETAPA_EM_ABERTO);
    if (i < 0)
        return;

    atribuirTecnicosNaOS(i);
}

/* UC29 - Registrar Entrega e Pagamento: registra a data de entrega e o valor pago */
void registrarEntrega()
{
    char texto[30];
    double valor, diferenca;
    int i;

    imprimirCabecalho("Ordens de Servico > Registrar Entrega");

    /* RF08: só OS com teste aprovado e ainda não entregue */
    i = selecionarOS(ETAPA_PRONTA_ENTREGA);
    if (i < 0)
        return;

    mostrarOS(i);
    lerTexto("\nValor recebido em R$ (ENTER para voltar): ", texto, sizeof(texto));
    if (strlen(texto) == 0)
        return;

    valor = textoParaValor(texto);
    if (valor < 0)
    {
        printf("\n[!] Valor invalido. Nada foi gravado.");
        pausar();
        return;
    }

    /* 7a: o valor recebido deve ser o do orçamento aprovado */
    diferenca = valor - ordens[i].orcamento;
    if (diferenca > 0.005 || diferenca < -0.005)
    {
        printf("\n[!] O valor recebido (R$ %.2f) e diferente do orcamento aprovado (R$ %.2f).", valor,
               ordens[i].orcamento);
        if (!confirmar("\nConfirmar este valor mesmo assim?"))
        {
            printf("\nNada foi gravado.");
            pausar();
            return;
        }
    }

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
    int i, opcao;

    imprimirCabecalho("Ordens de Servico > Registrar Feedback");

    /* Só OS já entregues (UC29); o cliente vê só as dele (RI10) */
    i = selecionarOS(ETAPA_FEEDBACK);
    if (i < 0)
        return;

    mostrarOS(i);
    printf("\nO cliente ficou satisfeito com o servico?\n");
    printf("  1 - Sim (finaliza a OS)\n");
    printf("  2 - Nao (a OS volta para reparo)\n");
    printf("  0 - Voltar\n\n");
    opcao = lerInteiro("Opcao: ");

    if (opcao == 1)
    {
        lerTexto("\nComentario (opcional): ", comentario, sizeof(comentario));
        strcpy(ordens[i].satisfeito, "sim");
        strcpy(ordens[i].feedback_cliente, comentario);
        strcpy(ordens[i].status, "finalizada");
    }
    else if (opcao == 2)
    {
        lerTexto("\nO que ainda esta com problema? ", comentario, sizeof(comentario));
        if (!campoPreenchido(comentario, "problema"))
            return;
        strcpy(ordens[i].satisfeito, "nao");
        strcpy(ordens[i].feedback_cliente, comentario);
        /* Volta para Executar Manutenção (UC33): limpa o teste, a entrega e o pagamento */
        strcpy(ordens[i].resultado_teste, "");
        strcpy(ordens[i].data_entrega, "");
        ordens[i].valor_pago = -1;
    }
    else
    {
        return;
    }

    dataHoraAgora(ordens[i].atualizado_em);
    if (!salvarOrdens())
    {
        pausar();
        return;
    }

    if (opcao == 1)
        printf("\n[+] Feedback registrado. A OS %d foi finalizada!", ordens[i].id_os);
    else
        printf("\n[+] Feedback registrado. A OS %d voltou para Executar Manutencao.", ordens[i].id_os);
    pausar();
}

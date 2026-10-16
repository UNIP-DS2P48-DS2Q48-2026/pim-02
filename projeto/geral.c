/* Matriz de Permissões (RN10): o perfil do usuário logado é este? */
int perfilLogado(char perfil[])
{
    if (usuarioLogado >= 0 && strcmp(usuarios[usuarioLogado].perfil, perfil) == 0)
    {
        return 1;
    }
    return 0;
}

/* Regrava o usuario.txt inteiro com o que está no vetor */
int salvarUsuarios()
{
    FILE *arquivo;
    int i;

    arquivo = fopen(ARQ_USUARIO, "w");
    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_USUARIO);
        return 0;
    }
    fprintf(arquivo, "id_usuario;nome;email;telefone;senha;perfil;cpf;especialidade;status;criado_em;atualizado_em\n");
    for (i = 0; i < totalUsuarios; i++)
    {
        fprintf(arquivo, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
                usuarios[i].id_usuario, usuarios[i].nome, usuarios[i].email, usuarios[i].telefone,
                usuarios[i].senha, usuarios[i].perfil, usuarios[i].cpf, usuarios[i].especialidade,
                usuarios[i].status, usuarios[i].criado_em, usuarios[i].atualizado_em);
    }
    fclose(arquivo);
    return 1;
}

/* Coloca um usuário de teste no fim do vetor, com a senha byte123 */
void adicionarUsuarioInicial(char nome[], char email[], char telefone[], char perfil[], char cpf[], char especialidade[])
{
    int n = totalUsuarios;

    usuarios[n].id_usuario = n + 1;
    strcpy(usuarios[n].nome, nome);
    strcpy(usuarios[n].email, email);
    strcpy(usuarios[n].telefone, telefone);
    gerarHash("byte123", usuarios[n].senha);
    strcpy(usuarios[n].perfil, perfil);
    strcpy(usuarios[n].cpf, cpf);
    strcpy(usuarios[n].especialidade, especialidade);
    strcpy(usuarios[n].status, "ativo");
    dataHoraAgora(usuarios[n].criado_em);
    strcpy(usuarios[n].atualizado_em, usuarios[n].criado_em);
    totalUsuarios++;
}

/* Lê o usuario.txt para o vetor; se o arquivo não existir, cria os usuários de teste do grupo */
void carregarUsuarios()
{
    FILE *arquivo;
    char linha[TAM_LINHA];
    char numero[20];
    int n;

    totalUsuarios = 0;
    arquivo = fopen(ARQ_USUARIO, "r");
    if (arquivo == NULL)
    {
        adicionarUsuarioInicial("Ana", "ana@bytetech.com", "", "admin", "", "");
        adicionarUsuarioInicial("Carla", "carla@bytetech.com", "", "recepcionista", "", "");
        adicionarUsuarioInicial("Rafael", "rafael@bytetech.com", "", "tecnico", "", "Hardware");
        adicionarUsuarioInicial("Joao Pereira", "joao.pereira@email.com", "(11) 90000-0000", "cliente", "111.111.111-11", "");
        salvarUsuarios();
        return;
    }

    /* Uma linha por usuário, campos na mesma ordem do salvarUsuarios */
    while (totalUsuarios < MAX_USUARIOS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        /* Pula o cabeçalho e linhas vazias: toda linha de dados começa com o id (um número) */
        if (linha[0] < '0' || linha[0] > '9')
            continue;

        n = totalUsuarios;
        pegarCampo(linha, 0, numero, sizeof(numero));
        usuarios[n].id_usuario = atoi(numero);
        pegarCampo(linha, 1, usuarios[n].nome, sizeof(usuarios[n].nome));
        pegarCampo(linha, 2, usuarios[n].email, sizeof(usuarios[n].email));
        pegarCampo(linha, 3, usuarios[n].telefone, sizeof(usuarios[n].telefone));
        pegarCampo(linha, 4, usuarios[n].senha, sizeof(usuarios[n].senha));
        pegarCampo(linha, 5, usuarios[n].perfil, sizeof(usuarios[n].perfil));
        pegarCampo(linha, 6, usuarios[n].cpf, sizeof(usuarios[n].cpf));
        pegarCampo(linha, 7, usuarios[n].especialidade, sizeof(usuarios[n].especialidade));
        pegarCampo(linha, 8, usuarios[n].status, sizeof(usuarios[n].status));
        pegarCampo(linha, 9, usuarios[n].criado_em, sizeof(usuarios[n].criado_em));
        pegarCampo(linha, 10, usuarios[n].atualizado_em, sizeof(usuarios[n].atualizado_em));
        totalUsuarios++;
    }
    fclose(arquivo);
}

/* ---------- Funções comuns aos cadastros de usuário (UC04 a UC19) ---------- */

/* Posição no vetor do usuário com este id (-1 se não existir) */
int buscarUsuarioPorId(int id)
{
    int i;

    for (i = 0; i < totalUsuarios; i++)
    {
        if (usuarios[i].id_usuario == id)
            return i;
    }
    return -1;
}

/* RN13: e-mail já usado por outro usuário? ignorar = posição que está sendo editada (-1 no cadastro) */
int emailEmUso(char email[], int ignorar)
{
    int i;

    for (i = 0; i < totalUsuarios; i++)
    {
        if (i != ignorar && strcmp(usuarios[i].email, email) == 0)
            return 1;
    }
    return 0;
}

/* RN13: CPF já usado por outro cliente? */
int cpfEmUso(char cpf[], int ignorar)
{
    int i;

    for (i = 0; i < totalUsuarios; i++)
    {
        if (i != ignorar && strcmp(usuarios[i].cpf, cpf) == 0)
            return 1;
    }
    return 0;
}

/* E-mail válido: não começa com @, tem um @ e um ponto depois dele */
int emailValido(char email[])
{
    int i, temArroba = 0, temPonto = 0;

    if (email[0] == '@')
        return 0;
    for (i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
            temArroba = 1;
        else if (email[i] == '.' && temArroba == 1)
            temPonto = 1;
    }
    if (temArroba == 1 && temPonto == 1)
        return 1;
    return 0;
}

/* RNF09: avisa qual campo obrigatório ficou vazio */
int campoPreenchido(char valor[], char nomeCampo[])
{
    if (strlen(valor) == 0)
    {
        printf("\n[!] O campo %s e obrigatorio (RNF09). Nada foi gravado.", nomeCampo);
        pausar();
        return 0;
    }
    return 1;
}

/* Id do cliente dono do equipamento (-1 se não existir) */
int donoDoEquipamento(int idEquipamento)
{
    int i;

    for (i = 0; i < totalEquipamentos; i++)
    {
        if (equipamentos[i].id_equipamento == idEquipamento)
            return equipamentos[i].id_cliente;
    }
    return -1;
}

/* Posição na tecnico_os da linha deste técnico nesta OS (-1 = o técnico não está na OS) */
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

/* RN14: o usuário está em alguma OS aberta ou em andamento, como atendente, cliente ou técnico? */
int temOSEmAndamento(int idUsuario)
{
    int i;

    for (i = 0; i < totalOrdens; i++)
    {
        if (strcmp(ordens[i].status, "aberta") != 0 && strcmp(ordens[i].status, "em_andamento") != 0)
            continue; /* OS encerrada não bloqueia */

        if (ordens[i].id_atendente == idUsuario)
            return 1;
        if (donoDoEquipamento(ordens[i].id_equipamento) == idUsuario)
            return 1;
        if (buscarTecnicoNaOS(idUsuario, ordens[i].id_os) >= 0)
            return 1;
    }
    return 0;
}

/* RN10: o técnico tem alguma OS atribuída a ele com equipamento deste cliente? */
int tecnicoAtendeCliente(int idTecnico, int idCliente)
{
    int i;

    for (i = 0; i < totalOrdens; i++)
    {
        if (donoDoEquipamento(ordens[i].id_equipamento) == idCliente && buscarTecnicoNaOS(idTecnico, ordens[i].id_os) >= 0)
            return 1;
    }
    return 0;
}

/* RN10: o usuário logado pode ver este cadastro? (técnico só vê clientes das OS dele) */
int podeVerUsuario(int i, char perfil[])
{
    if (strcmp(usuarios[i].perfil, perfil) != 0)
        return 0;
    if (perfilLogado("tecnico"))
        return tecnicoAtendeCliente(usuarios[usuarioLogado].id_usuario, usuarios[i].id_usuario);
    return 1;
}

/* Lista os usuários do perfil que batem com o filtro (nome, e-mail ou CPF); devolve quantos mostrou */
int listarUsuarios(char perfil[], char filtro[], int soAtivos)
{
    int i, mostrados = 0;

    for (i = 0; i < totalUsuarios; i++)
    {
        if (!podeVerUsuario(i, perfil))
            continue;
        if (soAtivos == 1 && strcmp(usuarios[i].status, "ativo") != 0)
            continue;
        if (!contemTexto(usuarios[i].nome, filtro) && !contemTexto(usuarios[i].email, filtro) &&
            !contemTexto(usuarios[i].cpf, filtro))
            continue;

        /* Cabeçalho da tabela antes do primeiro da lista */
        if (mostrados == 0)
        {
            printf("\n  %-4s %-22s %-26s %-15s %s\n", "ID", "Nome", "E-mail", "CPF/Especial.", "Status");
            printf("  ---------------------------------------------------------------------------\n");
        }

        printf("  %-4d %-22.22s %-26.26s ", usuarios[i].id_usuario, usuarios[i].nome, usuarios[i].email);
        if (strcmp(perfil, "cliente") == 0)
            printf("%-15.15s %s\n", usuarios[i].cpf, usuarios[i].status);
        else
            printf("%-15.15s %s\n", usuarios[i].especialidade, usuarios[i].status);

        if (strlen(usuarios[i].telefone) > 0)
            printf("       Telefone: %s\n", usuarios[i].telefone);
        mostrados++;
    }
    return mostrados;
}

/* Pede um filtro, lista e pede o ID. Devolve a posição no vetor ou -1 (voltou ou ID inválido) */
int selecionarUsuario(char perfil[], int soAtivos)
{
    char filtro[60];
    int id, i;

    lerTexto("\nFiltro (nome, e-mail ou CPF), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return -1;

    if (listarUsuarios(perfil, filtro, soAtivos) == 0)
    {
        printf("\n[!] Nenhum cadastro encontrado para o filtro informado.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nID (0 para voltar): ");
    if (id <= 0)
        return -1;

    i = buscarUsuarioPorId(id);
    if (i < 0 || !podeVerUsuario(i, perfil) || (soAtivos == 1 && strcmp(usuarios[i].status, "ativo") != 0))
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return i;
}

/* Mostra o valor atual entre colchetes e lê o novo em destino; ENTER mantém o atual */
void lerOuManter(char rotulo[], char atual[], char destino[], int tamanho)
{
    printf("%s [%s]: ", rotulo, atual);
    lerTexto("", destino, tamanho);
    if (strlen(destino) == 0)
    {
        strcpy(destino, atual);
    }
}

/* Cadastrar (UC04, UC08, UC12, UC16): nome, e-mail e senha; cliente com telefone e CPF; técnico com especialidade */
void cadastrarUsuario(char perfil[], char titulo[])
{
    char senha[50];
    int i, n, maiorId = 0;

    imprimirCabecalho(titulo);

    if (totalUsuarios >= MAX_USUARIOS)
    {
        printf("\n[!] Limite de %d usuarios atingido.", MAX_USUARIOS);
        pausar();
        return;
    }

    /* Preenche a próxima posição livre do vetor; só conta o usuário (totalUsuarios++) se tudo der certo */
    n = totalUsuarios;
    strcpy(usuarios[n].telefone, "");
    strcpy(usuarios[n].cpf, "");
    strcpy(usuarios[n].especialidade, "");

    lerTexto("\nNome (ENTER para voltar): ", usuarios[n].nome, sizeof(usuarios[n].nome));
    if (strlen(usuarios[n].nome) == 0)
        return;
    lerTexto("E-mail: ", usuarios[n].email, sizeof(usuarios[n].email));
    if (strcmp(perfil, "cliente") == 0)
    {
        lerTexto("Telefone: ", usuarios[n].telefone, sizeof(usuarios[n].telefone));
        lerTexto("CPF: ", usuarios[n].cpf, sizeof(usuarios[n].cpf));
    }
    if (strcmp(perfil, "tecnico") == 0)
    {
        lerTexto("Especialidade: ", usuarios[n].especialidade, sizeof(usuarios[n].especialidade));
    }
    lerTexto("Senha: ", senha, sizeof(senha));

    /* Validações antes de gravar (RNF09, RN13) */
    if (!campoPreenchido(usuarios[n].email, "E-mail") || !campoPreenchido(senha, "Senha"))
        return;
    if (strcmp(perfil, "cliente") == 0 && !campoPreenchido(usuarios[n].cpf, "CPF"))
        return;
    if (strcmp(perfil, "tecnico") == 0 && !campoPreenchido(usuarios[n].especialidade, "Especialidade"))
        return;
    if (!emailValido(usuarios[n].email))
    {
        printf("\n[!] E-mail invalido. Nada foi gravado.");
        pausar();
        return;
    }
    if (emailEmUso(usuarios[n].email, n))
    {
        printf("\n[!] Este e-mail ja pertence a outro usuario (RN13). Nada foi gravado.");
        pausar();
        return;
    }
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(usuarios[n].cpf, n))
    {
        printf("\n[!] Este CPF ja esta cadastrado (RN13). Nada foi gravado.");
        pausar();
        return;
    }

    /* O novo id é o maior id existente + 1 */
    for (i = 0; i < totalUsuarios; i++)
    {
        if (usuarios[i].id_usuario > maiorId)
            maiorId = usuarios[i].id_usuario;
    }
    usuarios[n].id_usuario = maiorId + 1;
    strcpy(usuarios[n].perfil, perfil);
    strcpy(usuarios[n].status, "ativo");
    gerarHash(senha, usuarios[n].senha);
    dataHoraAgora(usuarios[n].criado_em);
    strcpy(usuarios[n].atualizado_em, usuarios[n].criado_em);

    totalUsuarios++;
    if (!salvarUsuarios())
    {
        totalUsuarios--; /* não gravou: desfaz o cadastro */
        pausar();
        return;
    }

    printf("\n[+] Cadastro realizado com sucesso! ID: %d", usuarios[n].id_usuario);
    pausar();
}

/* Consultar (UC05, UC09, UC13, UC17): lista com o status; o técnico só vê clientes das OS dele (RN10) */
void consultarUsuarios(char perfil[], char titulo[])
{
    char filtro[60];

    imprimirCabecalho(titulo);

    lerTexto("\nFiltro (nome, e-mail ou CPF), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return;

    if (listarUsuarios(perfil, filtro, 0) == 0)
    {
        printf("\n[!] Nenhum cadastro encontrado para o filtro informado.\n");
    }
    pausar();
}

/* Editar (UC06, UC10, UC14, UC18): ENTER mantém o valor atual; o status só muda pelo Inativar */
void editarUsuario(char perfil[], char titulo[])
{
    char nome[60], email[60], telefone[20], cpf[15], especialidade[40], senha[50];
    int i;

    imprimirCabecalho(titulo);

    i = selecionarUsuario(perfil, 0);
    if (i < 0)
        return;

    /* Lê tudo em variáveis locais; o cadastro só muda depois das validações */
    printf("\nDeixe em branco para manter o valor atual.\n\n");
    lerOuManter("Nome", usuarios[i].nome, nome, sizeof(nome));
    lerOuManter("E-mail", usuarios[i].email, email, sizeof(email));
    strcpy(telefone, usuarios[i].telefone);
    strcpy(cpf, usuarios[i].cpf);
    strcpy(especialidade, usuarios[i].especialidade);
    if (strcmp(perfil, "cliente") == 0)
    {
        lerOuManter("Telefone", usuarios[i].telefone, telefone, sizeof(telefone));
        lerOuManter("CPF", usuarios[i].cpf, cpf, sizeof(cpf));
    }
    if (strcmp(perfil, "tecnico") == 0)
    {
        lerOuManter("Especialidade", usuarios[i].especialidade, especialidade, sizeof(especialidade));
    }
    lerTexto("Nova senha [manter]: ", senha, sizeof(senha));

    if (!emailValido(email))
    {
        printf("\n[!] E-mail invalido. Nada foi alterado.");
        pausar();
        return;
    }
    if (emailEmUso(email, i))
    {
        printf("\n[!] Este e-mail ja pertence a outro usuario (RN13). Nada foi alterado.");
        pausar();
        return;
    }
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(cpf, i))
    {
        printf("\n[!] Este CPF ja pertence a outro cliente (RN13). Nada foi alterado.");
        pausar();
        return;
    }

    strcpy(usuarios[i].nome, nome);
    strcpy(usuarios[i].email, email);
    strcpy(usuarios[i].telefone, telefone);
    strcpy(usuarios[i].cpf, cpf);
    strcpy(usuarios[i].especialidade, especialidade);
    if (strlen(senha) > 0)
    {
        gerarHash(senha, usuarios[i].senha);
    }
    dataHoraAgora(usuarios[i].atualizado_em);
    if (!salvarUsuarios())
    {
        pausar();
        return;
    }

    printf("\n[+] Cadastro de %s atualizado com sucesso!", usuarios[i].nome);
    pausar();
}

/* Inativar (UC07, UC11, UC15, UC19): nunca apaga (RN12); bloqueia a si mesmo (RN17) e quem tem OS em andamento (RN14) */
void inativarUsuario(char perfil[], char titulo[])
{
    int i;

    imprimirCabecalho(titulo);

    i = selecionarUsuario(perfil, 1);
    if (i < 0)
        return;

    if (i == usuarioLogado)
    {
        printf("\n[!] Voce nao pode inativar o proprio cadastro (RN17).");
        pausar();
        return;
    }
    if (temOSEmAndamento(usuarios[i].id_usuario))
    {
        printf("\n[!] %s tem ordem de servico aberta ou em andamento. Inativacao bloqueada (RN14).", usuarios[i].nome);
        pausar();
        return;
    }

    printf("\nInativar %s.", usuarios[i].nome);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.");
        pausar();
        return;
    }

    strcpy(usuarios[i].status, "inativo");
    dataHoraAgora(usuarios[i].atualizado_em);
    if (!salvarUsuarios())
    {
        pausar();
        return;
    }

    printf("\n[+] %s inativado(a). O historico foi preservado e o acesso esta bloqueado (RN12).", usuarios[i].nome);
    pausar();
}

/* UC01 - Realizar Login: autentica por e-mail e senha e abre o menu do perfil */
int realizarLogin()
{
    char email[60];
    char senha[50];
    char hash[TAM_HASH];
    int i;

    /* --- PASSO 1: E-MAIL --- */
    imprimirCabecalho("Entrar");

    for (i = 0; i < 15; i++)
    {
        printf("\n");
    }

    lerTexto("E-mail (ENTER para voltar): ", email, sizeof(email));
    if (strlen(email) == 0)
    {
        return -1;
    }

    /* --- PASSO 2: SENHA --- */
    imprimirCabecalho("Entrar");

    /* O e-mail fica agora fixo no topo, logo abaixo do cabeçalho */
    printf("\nE-mail: %s\n", email);

    /* Espaçamento para o campo de senha na zona inferior */
    for (i = 0; i < 13; i++)
    {
        printf("\n");
    }

    lerTexto("Senha: ", senha, sizeof(senha));

    /* --- VALIDAÇÃO --- */
    gerarHash(senha, hash);
    for (i = 0; i < totalUsuarios; i++)
    {
        if (strcmp(email, usuarios[i].email) == 0 && strcmp(hash, usuarios[i].senha) == 0)
        {
            if (strcmp(usuarios[i].status, "ativo") != 0)
            {
                printf("\n[!] Usuario inativo. O acesso esta bloqueado (RN12).");
                pausar();
                return -1;
            }
            return i; /* Retorna o índice do utilizador autenticado */
        }
    }

    /* Mesma mensagem para e-mail inexistente e senha errada */
    printf("\n[!] E-mail ou senha incorretos.");
    pausar();
    return -1;
}

/* UC02 - Fazer Logout: encerra a sessao e volta para a tela inicial */
void fazerLogout()
{
    usuarioLogado = -1;
    printf("\nA terminar a sessao...\n");
}

/* UC03 - Consultar Meus Dados: mostra o cadastro do usuario logado */
void consultarMeusDados()
{
    int i = usuarioLogado;

    imprimirCabecalho("Meus Dados");

    printf("\n  Nome:     %s\n", usuarios[i].nome);
    printf("  E-mail:   %s\n", usuarios[i].email);
    if (strlen(usuarios[i].telefone) > 0)
        printf("  Telefone: %s\n", usuarios[i].telefone);
    printf("  Perfil:   %s\n", usuarios[i].perfil);
    if (strlen(usuarios[i].cpf) > 0)
        printf("  CPF:      %s\n", usuarios[i].cpf);
    if (strlen(usuarios[i].especialidade) > 0)
        printf("  Especialidade: %s\n", usuarios[i].especialidade);
    pausar();
}

/* UC44 - Alterar Senha: troca a propria senha, confirmando a senha atual */
void alterarSenha()
{
    char atual[50], nova[50], confirmacao[50];
    char hash[TAM_HASH];

    imprimirCabecalho("Alterar Senha");

    lerTexto("\nSenha atual (ENTER para voltar): ", atual, sizeof(atual));
    if (strlen(atual) == 0)
    {
        return;
    }
    gerarHash(atual, hash);
    if (strcmp(hash, usuarios[usuarioLogado].senha) != 0)
    {
        printf("\n[!] A senha atual nao confere. A senha nao foi alterada.");
        pausar();
        return;
    }

    /* Repete até a nova senha e a confirmação serem iguais */
    do
    {
        lerTexto("Nova senha: ", nova, sizeof(nova));
        if (strlen(nova) == 0)
        {
            printf("\n[!] A nova senha e obrigatoria. A senha nao foi alterada.");
            pausar();
            return;
        }
        lerTexto("Confirme a nova senha: ", confirmacao, sizeof(confirmacao));
        if (strcmp(nova, confirmacao) != 0)
        {
            printf("\n[!] As senhas nao conferem. Digite novamente.\n\n");
        }
    } while (strcmp(nova, confirmacao) != 0);

    gerarHash(nova, usuarios[usuarioLogado].senha);
    dataHoraAgora(usuarios[usuarioLogado].atualizado_em);
    if (!salvarUsuarios())
    {
        pausar();
        return;
    }

    printf("\n[+] Senha alterada com sucesso!");
    pausar();
}

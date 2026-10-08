/* Nome do perfil para mostrar na tela (no arquivo fica em minúsculo, como no banco) */
const char *nomePerfil(const char *perfil)
{
    if (strcmp(perfil, "admin") == 0)
        return "Administrador";
    if (strcmp(perfil, "recepcionista") == 0)
        return "Recepcionista";
    if (strcmp(perfil, "tecnico") == 0)
        return "Tecnico";
    if (strcmp(perfil, "cliente") == 0)
        return "Cliente";
    return perfil;
}

/* Regrava o usuario.txt inteiro com o que está no vetor */
void salvarUsuarios(void)
{
    FILE *arquivo = fopen(ARQ_USUARIO, "w");
    int i;

    if (arquivo == NULL)
    {
        printf("\n[!] Erro: nao foi possivel gravar %s.\n", ARQ_USUARIO);
        return;
    }
    for (i = 0; i < totalUsuarios; i++)
    {
        Usuario *u = &usuarios[i];
        fprintf(arquivo, "%d;%s;%s;%s;%s;%s;%s;%s;%s;%s;%s\n",
                u->id_usuario, u->nome, u->email, u->telefone, u->senha, u->perfil,
                u->cpf, u->especialidade, u->status, u->criado_em, u->atualizado_em);
    }
    fclose(arquivo);
}

/* Usuários de teste da primeira execução (os mesmos que o grupo usava), todos com a senha byte123 */
void criarUsuariosIniciais(void)
{
    const char *dados[4][6] = {
        /* nome, email, telefone, perfil, cpf, especialidade */
        {"Ana", "ana@bytetech.com", "", "admin", "", ""},
        {"Carla", "carla@bytetech.com", "", "recepcionista", "", ""},
        {"Rafael", "rafael@bytetech.com", "", "tecnico", "", "Hardware"},
        {"Joao Pereira", "joao.pereira@email.com", "(11) 90000-0000", "cliente", "111.111.111-11", ""}};
    char agora[TAM_DATAHORA];
    int i;

    dataHoraAgora(agora);
    for (i = 0; i < 4; i++)
    {
        Usuario *u = &usuarios[i];
        u->id_usuario = i + 1;
        strcpy(u->nome, dados[i][0]);
        strcpy(u->email, dados[i][1]);
        strcpy(u->telefone, dados[i][2]);
        gerarHash("byte123", u->senha);
        strcpy(u->perfil, dados[i][3]);
        strcpy(u->cpf, dados[i][4]);
        strcpy(u->especialidade, dados[i][5]);
        strcpy(u->status, "ativo");
        strcpy(u->criado_em, agora);
        strcpy(u->atualizado_em, agora);
    }
    totalUsuarios = 4;
    salvarUsuarios();
}

/* Lê o usuario.txt para o vetor; se o arquivo não existir, cria os usuários iniciais */
void carregarUsuarios(void)
{
    FILE *arquivo = fopen(ARQ_USUARIO, "r");
    char linha[TAM_LINHA];

    totalUsuarios = 0;
    if (arquivo == NULL)
    {
        criarUsuariosIniciais();
        return;
    }
    while (totalUsuarios < MAX_USUARIOS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        Usuario *u = &usuarios[totalUsuarios];
        char *cursor = linha;

        if (linha[0] == '\n' || linha[0] == '\r')
            continue;
        u->id_usuario = lerCampoInt(&cursor);
        lerCampo(&cursor, u->nome, sizeof(u->nome));
        lerCampo(&cursor, u->email, sizeof(u->email));
        lerCampo(&cursor, u->telefone, sizeof(u->telefone));
        lerCampo(&cursor, u->senha, sizeof(u->senha));
        lerCampo(&cursor, u->perfil, sizeof(u->perfil));
        lerCampo(&cursor, u->cpf, sizeof(u->cpf));
        lerCampo(&cursor, u->especialidade, sizeof(u->especialidade));
        lerCampo(&cursor, u->status, sizeof(u->status));
        lerCampo(&cursor, u->criado_em, sizeof(u->criado_em));
        lerCampo(&cursor, u->atualizado_em, sizeof(u->atualizado_em));
        totalUsuarios++;
    }
    fclose(arquivo);
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
void fazerLogout(void)
{
    usuarioLogado = -1;
    printf("\nA terminar a sessao...\n");
}

/* UC03 - Consultar Meus Dados: mostra o cadastro do usuario logado */
void consultarMeusDados(void)
{
    Usuario *u = &usuarios[usuarioLogado];

    imprimirCabecalho("Meus Dados");

    printf("\n  Nome:     %s\n", u->nome);
    printf("  E-mail:   %s\n", u->email);
    printf("  Telefone: %s\n", strlen(u->telefone) > 0 ? u->telefone : "-");
    printf("  Perfil:   %s\n", nomePerfil(u->perfil));
    if (strcmp(u->perfil, "cliente") == 0)
    {
        printf("  CPF:      %s\n", u->cpf);
    }
    if (strlen(u->especialidade) > 0)
    {
        printf("  Especialidade: %s\n", u->especialidade);
    }
    pausar();
}

/* UC44 - Alterar Senha: troca a propria senha, confirmando a senha atual */
void alterarSenha(void)
{
    Usuario *u = &usuarios[usuarioLogado];
    char atual[50], nova[50], confirmacao[50];
    char hash[TAM_HASH];

    imprimirCabecalho("Alterar Senha");

    lerTexto("\nSenha atual (ENTER para voltar): ", atual, sizeof(atual));
    if (strlen(atual) == 0)
    {
        return;
    }
    gerarHash(atual, hash);
    if (strcmp(hash, u->senha) != 0)
    {
        printf("\n[!] A senha atual nao confere. A senha nao foi alterada.");
        pausar();
        return;
    }

    while (1)
    {
        lerTexto("Nova senha: ", nova, sizeof(nova));
        if (strlen(nova) == 0)
        {
            printf("\n[!] A nova senha e obrigatoria. A senha nao foi alterada.");
            pausar();
            return;
        }
        lerTexto("Confirme a nova senha: ", confirmacao, sizeof(confirmacao));
        if (strcmp(nova, confirmacao) == 0)
        {
            break;
        }
        printf("\n[!] As senhas nao conferem. Digite novamente.\n\n");
    }

    gerarHash(nova, u->senha);
    dataHoraAgora(u->atualizado_em);
    salvarUsuarios();

    printf("\n[+] Senha alterada com sucesso!");
    pausar();
}

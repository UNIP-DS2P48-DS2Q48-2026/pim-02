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

/* Matriz de Permissões (RN10): o perfil do usuário logado é este? */
int perfilLogado(const char *perfil)
{
    return usuarioLogado >= 0 && strcmp(usuarios[usuarioLogado].perfil, perfil) == 0;
}

/* Funções de recepcionista: o Admin também pode, na ausência dela (RN11) */
int podeAtender(void)
{
    return perfilLogado("recepcionista") || perfilLogado("admin");
}

/* Funções de técnico: o Admin também pode, na ausência dele (RN11) */
int podeAtuarComoTecnico(void)
{
    return perfilLogado("tecnico") || perfilLogado("admin");
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

/* ---------- Funções comuns aos cadastros de usuário (UC04 a UC19) ---------- */

/* Nome do perfil no plural, para os títulos das telas */
const char *pluralPerfil(const char *perfil)
{
    if (strcmp(perfil, "admin") == 0)
        return "Administradores";
    if (strcmp(perfil, "recepcionista") == 0)
        return "Recepcionistas";
    if (strcmp(perfil, "tecnico") == 0)
        return "Tecnicos";
    return "Clientes";
}

/* Posição no vetor do usuário com este id (-1 se não existir) */
int buscarUsuarioPorId(int id)
{
    for (int i = 0; i < totalUsuarios; i++)
    {
        if (usuarios[i].id_usuario == id)
            return i;
    }
    return -1;
}

/* RN13: e-mail já usado por outro usuário? ignorar = posição que está sendo editada (-1 no cadastro) */
int emailEmUso(const char *email, int ignorar)
{
    for (int i = 0; i < totalUsuarios; i++)
    {
        if (i != ignorar && contemTexto(usuarios[i].email, email) && strlen(usuarios[i].email) == strlen(email))
            return 1;
    }
    return 0;
}

/* RN13: CPF já usado por outro cliente? */
int cpfEmUso(const char *cpf, int ignorar)
{
    for (int i = 0; i < totalUsuarios; i++)
    {
        if (i != ignorar && strcmp(usuarios[i].cpf, cpf) == 0)
            return 1;
    }
    return 0;
}

int emailValido(const char *email)
{
    const char *arroba = strchr(email, '@');
    return arroba != NULL && arroba != email && strchr(arroba, '.') != NULL;
}

/* RNF09: avisa qual campo obrigatório ficou vazio */
int campoPreenchido(const char *valor, const char *nomeCampo)
{
    if (strlen(valor) == 0)
    {
        printf("\n[!] O campo %s e obrigatorio (RNF09). Nada foi gravado.", nomeCampo);
        pausar();
        return 0;
    }
    return 1;
}

/* RN14: o usuário está em alguma OS aberta ou em andamento, como atendente, técnico ou cliente? */
int temOSEmAndamento(int idUsuario)
{
    for (int i = 0; i < totalOrdens; i++)
    {
        OrdemServico *o = &ordens[i];

        if (strcmp(o->status, "aberta") != 0 && strcmp(o->status, "em_andamento") != 0)
            continue;
        if (o->id_atendente == idUsuario)
            return 1;
        for (int j = 0; j < totalEquipamentos; j++)
        {
            if (equipamentos[j].id_equipamento == o->id_equipamento && equipamentos[j].id_cliente == idUsuario)
                return 1;
        }
        for (int j = 0; j < totalTecnicosOS; j++)
        {
            if (tecnicosOS[j].id_os == o->id_os && tecnicosOS[j].id_tecnico == idUsuario)
                return 1;
        }
    }
    return 0;
}

/* RN10: o técnico tem alguma OS atribuída a ele com equipamento deste cliente? */
int tecnicoAtendeCliente(int idTecnico, int idCliente)
{
    for (int i = 0; i < totalTecnicosOS; i++)
    {
        if (tecnicosOS[i].id_tecnico != idTecnico)
            continue;
        for (int j = 0; j < totalOrdens; j++)
        {
            if (ordens[j].id_os != tecnicosOS[i].id_os)
                continue;
            for (int k = 0; k < totalEquipamentos; k++)
            {
                if (equipamentos[k].id_equipamento == ordens[j].id_equipamento && equipamentos[k].id_cliente == idCliente)
                    return 1;
            }
        }
    }
    return 0;
}

/* RN05: só o Admin cadastra, edita e inativa a equipe; clientes também pela Recepcionista */
int podeGerenciar(const char *perfil)
{
    if (strcmp(perfil, "cliente") == 0)
        return podeAtender();
    return perfilLogado("admin");
}

/* RN10: o usuário logado pode ver este cadastro? (técnico só vê clientes das OS dele) */
int podeVerUsuario(int i, const char *perfil)
{
    if (strcmp(usuarios[i].perfil, perfil) != 0)
        return 0;
    if (podeGerenciar(perfil))
        return 1;
    if (strcmp(perfil, "cliente") == 0 && perfilLogado("tecnico"))
        return tecnicoAtendeCliente(usuarios[usuarioLogado].id_usuario, usuarios[i].id_usuario);
    return 0;
}

/* Lista os usuários do perfil que batem com o filtro (nome, e-mail ou CPF); devolve quantos mostrou */
int listarUsuarios(const char *perfil, const char *filtro, int soAtivos)
{
    int mostrados = 0;
    int temExtra = strcmp(perfil, "cliente") == 0 || strcmp(perfil, "tecnico") == 0;

    for (int i = 0; i < totalUsuarios; i++)
    {
        Usuario *u = &usuarios[i];

        if (!podeVerUsuario(i, perfil) || (soAtivos && strcmp(u->status, "ativo") != 0))
            continue;
        if (!contemTexto(u->nome, filtro) && !contemTexto(u->email, filtro) && !contemTexto(u->cpf, filtro))
            continue;

        if (mostrados == 0)
        {
            printf("\n  %-4s %-22s %-26s %-15s %s\n", "ID", "Nome", "E-mail",
                   strcmp(perfil, "cliente") == 0 ? "CPF" : (temExtra ? "Especialidade" : ""), "Status");
            printf("  ---------------------------------------------------------------------------\n");
        }
        printf("  %-4d %-22.22s %-26.26s %-15.15s %s\n", u->id_usuario, u->nome, u->email,
               strcmp(perfil, "cliente") == 0 ? u->cpf : u->especialidade, u->status);
        if (strcmp(perfil, "cliente") == 0 && strlen(u->telefone) > 0)
        {
            printf("       Telefone: %s\n", u->telefone);
        }
        mostrados++;
    }
    return mostrados;
}

/* Pede um filtro, lista e pede o ID. Devolve a posição no vetor ou -1 (voltou ou ID inválido) */
int selecionarUsuario(const char *perfil, int soAtivos)
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
    if (i < 0 || !podeVerUsuario(i, perfil) || (soAtivos && strcmp(usuarios[i].status, "ativo") != 0))
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return i;
}

/* Mostra o valor atual entre colchetes; ENTER mantém, qualquer texto substitui */
void lerOuManter(const char *rotulo, char *campo, int tamanho)
{
    char texto[200];
    char pergunta[300];

    sprintf(pergunta, "%s [%s]: ", rotulo, campo);
    lerTexto(pergunta, texto, tamanho < (int)sizeof(texto) ? tamanho : (int)sizeof(texto));
    if (strlen(texto) > 0)
    {
        strcpy(campo, texto);
    }
}

/* Cadastrar (UC04, UC08, UC12, UC16): nome, e-mail e senha; cliente com telefone e CPF; técnico com especialidade */
void cadastrarUsuario(const char *perfil)
{
    Usuario novo;
    char senha[50], titulo[60];
    int maiorId = 0;

    memset(&novo, 0, sizeof(novo));
    sprintf(titulo, "%s > Cadastrar", pluralPerfil(perfil));
    imprimirCabecalho(titulo);

    if (!podeGerenciar(perfil))
    {
        printf("\n[!] Seu perfil nao pode cadastrar %s (RN05).", pluralPerfil(perfil));
        pausar();
        return;
    }

    lerTexto("\nNome (ENTER para voltar): ", novo.nome, sizeof(novo.nome));
    if (strlen(novo.nome) == 0)
        return;
    lerTexto("E-mail: ", novo.email, sizeof(novo.email));
    if (strcmp(perfil, "cliente") == 0)
    {
        lerTexto("Telefone: ", novo.telefone, sizeof(novo.telefone));
        lerTexto("CPF: ", novo.cpf, sizeof(novo.cpf));
    }
    if (strcmp(perfil, "tecnico") == 0)
    {
        lerTexto("Especialidade: ", novo.especialidade, sizeof(novo.especialidade));
    }
    lerTexto("Senha: ", senha, sizeof(senha));

    /* Validações antes de gravar (RNF09, RN13) */
    if (!campoPreenchido(novo.email, "E-mail") || !campoPreenchido(senha, "Senha"))
        return;
    if (strcmp(perfil, "cliente") == 0 && !campoPreenchido(novo.cpf, "CPF"))
        return;
    if (strcmp(perfil, "tecnico") == 0 && !campoPreenchido(novo.especialidade, "Especialidade"))
        return;
    if (!emailValido(novo.email))
    {
        printf("\n[!] E-mail invalido. Nada foi gravado.");
        pausar();
        return;
    }
    if (emailEmUso(novo.email, -1))
    {
        printf("\n[!] Este e-mail ja pertence a outro usuario (RN13). Nada foi gravado.");
        pausar();
        return;
    }
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(novo.cpf, -1))
    {
        printf("\n[!] Este CPF ja esta cadastrado (RN13). Nada foi gravado.");
        pausar();
        return;
    }
    if (totalUsuarios >= MAX_USUARIOS)
    {
        printf("\n[!] Limite de %d usuarios atingido. Nada foi gravado.", MAX_USUARIOS);
        pausar();
        return;
    }

    for (int i = 0; i < totalUsuarios; i++)
    {
        if (usuarios[i].id_usuario > maiorId)
            maiorId = usuarios[i].id_usuario;
    }
    novo.id_usuario = maiorId + 1;
    strcpy(novo.perfil, perfil);
    strcpy(novo.status, "ativo");
    gerarHash(senha, novo.senha);
    dataHoraAgora(novo.criado_em);
    strcpy(novo.atualizado_em, novo.criado_em);

    usuarios[totalUsuarios++] = novo;
    salvarUsuarios();

    printf("\n[+] %s cadastrado(a) com sucesso! ID: %d", nomePerfil(perfil), novo.id_usuario);
    pausar();
}

/* Consultar (UC05, UC09, UC13, UC17): lista com o status; o técnico só vê clientes das OS dele (RN10) */
void consultarUsuarios(const char *perfil)
{
    char filtro[60], titulo[60];

    sprintf(titulo, "%s > Consultar", pluralPerfil(perfil));
    imprimirCabecalho(titulo);

    if (!podeGerenciar(perfil) && !(strcmp(perfil, "cliente") == 0 && perfilLogado("tecnico")))
    {
        printf("\n[!] Seu perfil nao pode consultar %s (RN10).", pluralPerfil(perfil));
        pausar();
        return;
    }

    while (1)
    {
        lerTexto("\nFiltro (nome, e-mail ou CPF), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;
        if (listarUsuarios(perfil, filtro, 0) > 0)
            break;
        printf("\n[!] Nenhum cadastro encontrado para o filtro informado.\n");
        if (strlen(filtro) == 0)
        {
            pausar();
            return;
        }
    }
    pausar();
}

/* Editar (UC06, UC10, UC14, UC18): ENTER mantém o valor atual; o status só muda pelo Inativar */
void editarUsuario(const char *perfil)
{
    Usuario editado;
    char senha[50], titulo[60];
    int i;

    sprintf(titulo, "%s > Editar", pluralPerfil(perfil));
    imprimirCabecalho(titulo);

    if (!podeGerenciar(perfil))
    {
        printf("\n[!] Seu perfil nao pode editar %s (RN05).", pluralPerfil(perfil));
        pausar();
        return;
    }

    i = selecionarUsuario(perfil, 0);
    if (i < 0)
        return;

    editado = usuarios[i]; /* trabalha numa cópia: se algo falhar, nada muda */
    printf("\nDeixe em branco para manter o valor atual.\n\n");
    lerOuManter("Nome", editado.nome, sizeof(editado.nome));
    lerOuManter("E-mail", editado.email, sizeof(editado.email));
    if (strcmp(perfil, "cliente") == 0)
    {
        lerOuManter("Telefone", editado.telefone, sizeof(editado.telefone));
        lerOuManter("CPF", editado.cpf, sizeof(editado.cpf));
    }
    if (strcmp(perfil, "tecnico") == 0)
    {
        lerOuManter("Especialidade", editado.especialidade, sizeof(editado.especialidade));
    }
    lerTexto("Nova senha [manter]: ", senha, sizeof(senha));

    if (!emailValido(editado.email))
    {
        printf("\n[!] E-mail invalido. Nada foi alterado.");
        pausar();
        return;
    }
    if (emailEmUso(editado.email, i))
    {
        printf("\n[!] Este e-mail ja pertence a outro usuario (RN13). Nada foi alterado.");
        pausar();
        return;
    }
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(editado.cpf, i))
    {
        printf("\n[!] Este CPF ja pertence a outro cliente (RN13). Nada foi alterado.");
        pausar();
        return;
    }

    if (strlen(senha) > 0)
    {
        gerarHash(senha, editado.senha);
    }
    dataHoraAgora(editado.atualizado_em);
    usuarios[i] = editado;
    salvarUsuarios();

    printf("\n[+] Cadastro de %s atualizado com sucesso!", editado.nome);
    pausar();
}

/* Inativar (UC07, UC11, UC15, UC19): nunca apaga (RN12); bloqueia a si mesmo (RN17) e quem tem OS em andamento (RN14) */
void inativarUsuario(const char *perfil)
{
    char titulo[60], pergunta[120];
    int i;

    sprintf(titulo, "%s > Inativar", pluralPerfil(perfil));
    imprimirCabecalho(titulo);

    if (!podeGerenciar(perfil))
    {
        printf("\n[!] Seu perfil nao pode inativar %s (RN05).", pluralPerfil(perfil));
        pausar();
        return;
    }

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

    sprintf(pergunta, "\nTem certeza que deseja inativar %s?", usuarios[i].nome);
    if (!confirmar(pergunta))
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.");
        pausar();
        return;
    }

    strcpy(usuarios[i].status, "inativo");
    dataHoraAgora(usuarios[i].atualizado_em);
    salvarUsuarios();

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

/* Matriz de Permissões (RN10): o perfil do usuário logado é este? */
int perfilLogado(char perfil[])
{
    if (usuarioLogado >= 0 && strcmp(usuarios[usuarioLogado].perfil, perfil) == 0)
    {
        return 1;
    }
    return 0;
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
        printf("\n[!] O campo %s e obrigatorio. Nada foi gravado.", nomeCampo);
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

/* O técnico está atribuído a esta OS? (tabela tecnico_os) */
int tecnicoEstaNaOS(int idTecnico, int idOS)
{
    int i;

    for (i = 0; i < totalTecnicosOS; i++)
    {
        if (tecnicosOS[i].id_tecnico == idTecnico && tecnicosOS[i].id_os == idOS)
            return 1;
    }
    return 0;
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
        if (tecnicoEstaNaOS(idUsuario, ordens[i].id_os))
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
        if (donoDoEquipamento(ordens[i].id_equipamento) == idCliente && tecnicoEstaNaOS(idTecnico, ordens[i].id_os))
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
    int n;

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
    lerSenha("Senha: ", senha, sizeof(senha));

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
        printf("\n[!] Este e-mail ja pertence a outro usuario. Nada foi gravado.");
        pausar();
        return;
    }
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(usuarios[n].cpf, n))
    {
        printf("\n[!] Este CPF ja esta cadastrado. Nada foi gravado.");
        pausar();
        return;
    }

    /* A geracao do ID agora e responsabilidade do SQL Server (IDENTITY). */
    strcpy(usuarios[n].perfil, perfil);
    strcpy(usuarios[n].status, "ativo");
    gerarHash(senha, usuarios[n].senha);
    if (!usuarios[n].senha[0]) { printf("Erro ao proteger senha.\n"); pausar(); return; }
    dataHoraAgora(usuarios[n].criado_em);
    strcpy(usuarios[n].atualizado_em, usuarios[n].criado_em);

    /* So adiciona ao vetor se o INSERT realmente funcionar. */
    if (!inserirUsuarioSQL(bancoAtivo, &usuarios[n]))
    {
        pausar();
        return;
    }
    totalUsuarios++;

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
    lerSenha("Nova senha [manter]: ", senha, sizeof(senha));

    if (!emailValido(email))
    {
        printf("\n[!] E-mail invalido. Nada foi alterado.");
        pausar();
        return;
    }
    if (emailEmUso(email, i))
    {
        printf("\n[!] Este e-mail ja pertence a outro usuario. Nada foi alterado.");
        pausar();
        return;
    }
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(cpf, i))
    {
        printf("\n[!] Este CPF ja pertence a outro cliente. Nada foi alterado.");
        pausar();
        return;
    }

    /* Prepara uma copia: nao altera o vetor se o UPDATE falhar. */
    Usuario alterado = usuarios[i];
    strcpy(alterado.nome, nome);
    strcpy(alterado.email, email);
    strcpy(alterado.telefone, telefone);
    strcpy(alterado.cpf, cpf);
    strcpy(alterado.especialidade, especialidade);
    if (strlen(senha) > 0)
    {
        gerarHash(senha, alterado.senha);
    }

    /* O SQL Server e a fonte de verdade dos dados. */
    if (!atualizarUsuarioSQL(bancoAtivo, &alterado))
    {
        printf("\n[!] Nao foi possivel salvar a edicao no SQL Server.");
        pausar();
        return;
    }

    dataHoraAgora(alterado.atualizado_em);
    usuarios[i] = alterado;
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
        printf("\n[!] Voce nao pode inativar o proprio cadastro.");
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

    /* O SQL verifica as OS abertas/em andamento antes de inativar (RN14).
       Atualizamos o vetor somente depois que o banco confirmar. */
    if (!inativarUsuarioSQL(bancoAtivo, usuarios[i].id_usuario))
    {
        printf("\n[!] Inativacao nao realizada. Verifique se o usuario esta ativo "
               "e se nao possui OS aberta ou em andamento.");
        pausar();
        return;
    }

    strcpy(usuarios[i].status, "inativo");
    dataHoraAgora(usuarios[i].atualizado_em);

    printf("\n[+] %s inativado(a). O historico foi preservado e o acesso esta bloqueado.", usuarios[i].nome);
    pausar();
}

/* UC01 - Realizar Login: autentica por e-mail e senha e abre o menu do perfil */
int realizarLogin()
{
    char email[60];
    char senha[50];
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

    lerSenha("Senha: ", senha, sizeof(senha));

    /* --- VALIDAÇÃO --- */
    for (i = 0; i < totalUsuarios; i++)
    {
        if (strcmp(email, usuarios[i].email) == 0 && verificarSenha(senha, usuarios[i].senha))
        {
            if (strcmp(usuarios[i].status, "ativo") != 0)
            {
                printf("\n[!] Usuario inativo. O acesso esta bloqueado.");
                pausar();
                return -1;
            }
            /* Migra automaticamente o hash legado ao efetuar login correto. */
            if (strncmp(usuarios[i].senha, "pbkdf2_sha256$", 14) != 0)
            {
                char novo[TAM_HASH];
                gerarHash(senha, novo);
                if (*novo && alterarSenhaUsuarioSQL(bancoAtivo, usuarios[i].id_usuario,
                                                    usuarios[i].senha, novo))
                    strcpy(usuarios[i].senha, novo);
                else
                    fprintf(stderr, "[!] Falha ao atualizar hash antigo da senha.\n");
            }
            return i; /* Indice do usuario autenticado */
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
    imprimirCabecalho("Alterar Senha");

    lerSenha("\nSenha atual (ENTER para voltar): ", atual, sizeof(atual));
    if (strlen(atual) == 0)
    {
        return;
    }
    if (!verificarSenha(atual, usuarios[usuarioLogado].senha))
    {
        printf("\n[!] A senha atual nao confere. A senha nao foi alterada.");
        pausar();
        return;
    }

    /* Repete até a nova senha e a confirmação serem iguais */
    do
    {
        lerSenha("Nova senha: ", nova, sizeof(nova));
        if (strlen(nova) == 0)
        {
            printf("\n[!] A nova senha e obrigatoria. A senha nao foi alterada.");
            pausar();
            return;
        }
        lerSenha("Confirme a nova senha: ", confirmacao, sizeof(confirmacao));
        if (strcmp(nova, confirmacao) != 0)
        {
            printf("\n[!] As senhas nao conferem. Digite novamente.\n\n");
        }
    } while (strcmp(nova, confirmacao) != 0);

    /* Calcula o hash sem alterar a memoria antes do SQL confirmar. */
    char hashNovo[TAM_HASH];
    gerarHash(nova, hashNovo);
    if (!*hashNovo) { printf("Erro ao gerar hash seguro.\n"); pausar(); return; }

    if (!alterarSenhaUsuarioSQL(bancoAtivo,
                                usuarios[usuarioLogado].id_usuario,
                                usuarios[usuarioLogado].senha,
                                hashNovo))
    {
        printf("\n[!] A senha nao foi alterada no banco de dados.");
        pausar();
        return;
    }

    /* Somente depois do UPDATE atualizamos a memoria. */
    strcpy(usuarios[usuarioLogado].senha, hashNovo);
    dataHoraAgora(usuarios[usuarioLogado].atualizado_em);

    printf("\n[+] Senha alterada com sucesso!");
    pausar();
}

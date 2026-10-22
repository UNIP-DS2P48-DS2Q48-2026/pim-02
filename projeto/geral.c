/* Matriz de Permissões (RN10): o perfil do usuário logado é este? */
int perfilLogado(char perfil[])
{
    if (usuarioLogado >= 0 && strcmp(usuarios[usuarioLogado].perfil, perfil) == 0)
        return 1;
    return 0;
}

#if USAR_SQLSERVER
/* Quantas posições do vetor já estão gravadas no banco. Cadastro novo entra sempre no fim do vetor */
int usuariosNoBanco = 0;

/* Grava o vetor na tabela usuario: UPDATE de quem já está no banco, INSERT de quem é novo */
int salvarUsuarios()
{
    char sql[2500];
    char nome[130], email[130], telefone[50], senha[40], perfil[40], cpf[40];
    char especialidade[90], status[20], criado[50], atualizado[50];
    int i, ok = 1;

    /* deixa gravar o id que o programa escolheu, em vez de o banco gerar outro */
    bdExecutar("SET IDENTITY_INSERT usuario ON");
    for (i = 0; i < totalUsuarios && ok; i++)
    {
        sqlTexto(nome, usuarios[i].nome);
        sqlTexto(email, usuarios[i].email);
        sqlTexto(telefone, usuarios[i].telefone);
        sqlTexto(senha, usuarios[i].senha);
        sqlTexto(perfil, usuarios[i].perfil);
        sqlTexto(cpf, usuarios[i].cpf);
        sqlTexto(especialidade, usuarios[i].especialidade);
        sqlTexto(status, usuarios[i].status);
        sqlTexto(criado, usuarios[i].criado_em);
        sqlTexto(atualizado, usuarios[i].atualizado_em);

        if (i < usuariosNoBanco)
        {
            sprintf(sql, "UPDATE usuario SET nome = %s, email = %s, telefone = %s, senha = %s, perfil = %s, cpf = %s, "
                         "especialidade = %s, status = %s, criado_em = %s, atualizado_em = %s WHERE id_usuario = %d",
                    nome, email, telefone, senha, perfil, cpf, especialidade, status, criado, atualizado,
                    usuarios[i].id_usuario);
        }
        else
        {
            sprintf(sql, "INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, "
                         "status, criado_em, atualizado_em) VALUES (%d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                    usuarios[i].id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status,
                    criado, atualizado);
        }
        ok = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT usuario OFF");

    if (ok)
        usuariosNoBanco = totalUsuarios;
    return ok;
}

/* Lê a tabela usuario para o vetor */
void carregarUsuarios()
{
    char numero[20];
    int n;

    totalUsuarios = 0;
    usuariosNoBanco = 0;
    /* CONVERT 120 devolve a data como AAAA-MM-DD HH:MM:SS, igual ao dataHoraAgora */
    if (!bdConsultar("SELECT id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, "
                     "CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                     "FROM usuario ORDER BY id_usuario"))
        return;

    while (totalUsuarios < MAX_USUARIOS && bdProximaLinha())
    {
        n = totalUsuarios;
        bdLerTexto(1, numero, sizeof(numero));
        usuarios[n].id_usuario = atoi(numero);
        bdLerTexto(2, usuarios[n].nome, sizeof(usuarios[n].nome));
        bdLerTexto(3, usuarios[n].email, sizeof(usuarios[n].email));
        bdLerTexto(4, usuarios[n].telefone, sizeof(usuarios[n].telefone));
        bdLerTexto(5, usuarios[n].senha, sizeof(usuarios[n].senha));
        bdLerTexto(6, usuarios[n].perfil, sizeof(usuarios[n].perfil));
        bdLerTexto(7, usuarios[n].cpf, sizeof(usuarios[n].cpf));
        bdLerTexto(8, usuarios[n].especialidade, sizeof(usuarios[n].especialidade));
        bdLerTexto(9, usuarios[n].status, sizeof(usuarios[n].status));
        bdLerTexto(10, usuarios[n].criado_em, sizeof(usuarios[n].criado_em));
        bdLerTexto(11, usuarios[n].atualizado_em, sizeof(usuarios[n].atualizado_em));
        totalUsuarios++;
    }
    usuariosNoBanco = totalUsuarios;
}
#else
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

/* Lê o usuario.txt para o vetor (sem arquivo = nenhum usuário ainda) */
void carregarUsuarios()
{
    FILE *arquivo;
    char linha[TAM_LINHA];
    char numero[20];
    int n;

    totalUsuarios = 0;
    arquivo = fopen(ARQ_USUARIO, "r");
    if (arquivo == NULL)
        return;

    while (totalUsuarios < MAX_USUARIOS && fgets(linha, sizeof(linha), arquivo) != NULL)
    {
        /* Pula o cabeçalho: toda linha de dados começa com o id (um número) */
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
#endif

/* Coloca um usuário de teste no fim do vetor, com a senha byte123 (usado no main na primeira vez) */
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

/* ---------- Funções comuns aos cadastros de usuário (UC04 a UC19, UC45 a UC48) ---------- */

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

/* RN13: e-mail já usado por outro usuário? ignorar = posição que está sendo cadastrada ou editada */
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

/* RN10: o usuário logado pode ver este cadastro? O técnico só vê os clientes das OS dele */
int podeVerUsuario(int i, char perfil[])
{
    int k;

    if (strcmp(usuarios[i].perfil, perfil) != 0)
        return 0;
    if (!perfilLogado("tecnico"))
        return 1;

    for (k = 0; k < totalOrdens; k++)
    {
        if (donoDoEquipamento(ordens[k].id_equipamento) == usuarios[i].id_usuario &&
            buscarTecnicoNaOS(usuarios[usuarioLogado].id_usuario, ordens[k].id_os) >= 0)
            return 1;
    }
    return 0;
}

/* Lista os usuários do perfil cujo nome, e-mail ou CPF contém o filtro; devolve quantos mostrou.
   status: "ativo" ou "inativo" para listar só esses, ou "" para listar todos */
int listarUsuarios(char perfil[], char filtro[], char status[])
{
    int i, mostrados = 0;

    for (i = 0; i < totalUsuarios; i++)
    {
        if (!podeVerUsuario(i, perfil))
            continue;
        if (strlen(status) > 0 && strcmp(usuarios[i].status, status) != 0)
            continue;
        /* strstr procura o filtro dentro do texto (filtro vazio aparece em qualquer texto) */
        if (strstr(usuarios[i].nome, filtro) == NULL && strstr(usuarios[i].email, filtro) == NULL &&
            strstr(usuarios[i].cpf, filtro) == NULL)
            continue;

        printf("ID %d - %s - %s - %s", usuarios[i].id_usuario, usuarios[i].nome, usuarios[i].email, usuarios[i].status);
        if (strcmp(perfil, "cliente") == 0)
            printf(" - CPF %s - Telefone %s", usuarios[i].cpf, usuarios[i].telefone);
        if (strcmp(perfil, "tecnico") == 0)
            printf(" - %s", usuarios[i].especialidade);
        printf("\n");
        mostrados++;
    }
    return mostrados;
}

/* Pede um filtro, lista e pede o ID. Devolve a posição no vetor ou -1 (voltou ou ID inválido) */
int selecionarUsuario(char perfil[], char status[])
{
    char filtro[60];
    int id, i;

    lerTexto("\nFiltro (nome, e-mail ou CPF), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return -1;

    if (listarUsuarios(perfil, filtro, status) == 0)
    {
        printf("\n[!] Nenhum cadastro encontrado para o filtro informado.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nID (0 para voltar): ");
    if (id <= 0)
        return -1;

    i = buscarUsuarioPorId(id);
    if (i < 0 || !podeVerUsuario(i, perfil) || (strlen(status) > 0 && strcmp(usuarios[i].status, status) != 0))
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return i;
}

/* Mostra o valor atual entre colchetes e lê o novo; ENTER mantém o atual */
void lerOuManter(char rotulo[], char atual[], char destino[], int tamanho)
{
    printf("%s [%s]: ", rotulo, atual);
    lerTexto("", destino, tamanho);
    if (strlen(destino) == 0)
        strcpy(destino, atual);
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
        lerTexto("Especialidade: ", usuarios[n].especialidade, sizeof(usuarios[n].especialidade));
    lerTexto("Senha: ", senha, sizeof(senha));

    /* Campos obrigatórios (RNF09) e e-mail/CPF únicos (RN13) */
    if (!campoPreenchido(usuarios[n].email, "E-mail") || !campoPreenchido(senha, "Senha"))
        return;
    if (strcmp(perfil, "cliente") == 0 && !campoPreenchido(usuarios[n].cpf, "CPF"))
        return;
    if (strcmp(perfil, "tecnico") == 0 && !campoPreenchido(usuarios[n].especialidade, "Especialidade"))
        return;
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

    /* O vetor está em ordem de id: o novo id é o id do último + 1 */
    usuarios[n].id_usuario = usuarios[n - 1].id_usuario + 1;
    strcpy(usuarios[n].perfil, perfil);
    strcpy(usuarios[n].status, "ativo");
    gerarHash(senha, usuarios[n].senha);
    dataHoraAgora(usuarios[n].criado_em);
    strcpy(usuarios[n].atualizado_em, usuarios[n].criado_em);

    totalUsuarios++;
    if (salvarUsuarios())
        printf("\n[+] Cadastro realizado com sucesso! ID: %d", usuarios[n].id_usuario);
    else
        totalUsuarios--; /* não gravou: desfaz o cadastro */
    pausar();
}

/* Consultar (UC05, UC09, UC13, UC17): lista com o status; o técnico só vê clientes das OS dele (RN10) */
void consultarUsuarios(char perfil[], char titulo[])
{
    char filtro[60];
    int mostrados;

    imprimirCabecalho(titulo);

    /* Pede o filtro de novo enquanto nenhum cadastro for encontrado (exceção 4a) */
    do
    {
        lerTexto("\nFiltro (nome, e-mail ou CPF), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;

        mostrados = listarUsuarios(perfil, filtro, "");
        if (mostrados == 0)
            printf("\n[!] Nenhum cadastro encontrado para o filtro informado. Tente outro filtro.\n");
    } while (mostrados == 0);
    pausar();
}

/* Editar (UC06, UC10, UC14, UC18): ENTER mantém o valor atual; o status só muda pelo Inativar e pelo Reativar */
void editarUsuario(char perfil[], char titulo[])
{
    char nome[60], email[60], telefone[20], cpf[15], especialidade[40], senha[50];
    int i;

    imprimirCabecalho(titulo);

    i = selecionarUsuario(perfil, "");
    if (i < 0)
        return;

    /* Lê tudo em variáveis locais; o cadastro só muda depois das validações */
    printf("\nDeixe em branco para manter o valor atual.\n");
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
        lerOuManter("Especialidade", usuarios[i].especialidade, especialidade, sizeof(especialidade));
    lerTexto("Nova senha [manter]: ", senha, sizeof(senha));

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
        gerarHash(senha, usuarios[i].senha);
    dataHoraAgora(usuarios[i].atualizado_em);
    if (salvarUsuarios())
        printf("\n[+] Cadastro de %s atualizado com sucesso!", usuarios[i].nome);
    pausar();
}

/* Inativar (UC07, UC11, UC15, UC19): nunca apaga (RN12); bloqueia a si mesmo (RN17) e quem tem OS em andamento (RN14) */
void inativarUsuario(char perfil[], char titulo[])
{
    int i;

    imprimirCabecalho(titulo);

    /* Só os cadastros ativos podem ser inativados */
    i = selecionarUsuario(perfil, "ativo");
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
    if (salvarUsuarios())
        printf("\n[+] %s inativado(a). O historico foi preservado e o acesso esta bloqueado (RN12).", usuarios[i].nome);
    pausar();
}

/* Reativar (UC45, UC46, UC47, UC48): volta um cadastro inativo para ativo (RF02) */
void reativarUsuario(char perfil[], char titulo[])
{
    int i, inativos = 0;

    imprimirCabecalho(titulo);

    /* Exceção 2a: nenhum cadastro inativo deste perfil */
    for (i = 0; i < totalUsuarios; i++)
    {
        if (strcmp(usuarios[i].perfil, perfil) == 0 && strcmp(usuarios[i].status, "inativo") == 0)
            inativos++;
    }
    if (inativos == 0)
    {
        printf("\n[!] Nenhum cadastro inativo. Nao ha cadastro para reativar.");
        pausar();
        return;
    }

    /* Só os cadastros inativos aparecem na lista */
    i = selecionarUsuario(perfil, "inativo");
    if (i < 0)
        return;

    printf("\nReativar %s.", usuarios[i].nome);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao cancelada. O cadastro continua inativo.");
        pausar();
        return;
    }

    strcpy(usuarios[i].status, "ativo");
    dataHoraAgora(usuarios[i].atualizado_em);
    if (salvarUsuarios())
        printf("\n[+] %s reativado(a). O acesso ao sistema foi liberado.", usuarios[i].nome);
    pausar();
}

/* UC01 - Realizar Login: autentica por e-mail e senha; devolve a posição do usuário ou -1 */
int realizarLogin()
{
    char email[60], senha[50], hash[TAM_HASH];
    int i;

    imprimirCabecalho("Entrar");
    lerTexto("E-mail (ENTER para voltar): ", email, sizeof(email));
    if (strlen(email) == 0)
        return -1;
    lerTexto("Senha: ", senha, sizeof(senha));

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
            return i;
        }
    }

    /* Mesma mensagem para e-mail inexistente e senha errada */
    printf("\n[!] E-mail ou senha incorretos.");
    pausar();
    return -1;
}

/* UC02 - Fazer Logout: encerra a sessão e volta para a tela inicial */
void fazerLogout()
{
    usuarioLogado = -1;
    printf("\nEncerrando a sessao...\n");
}

/* UC03 - Consultar Meus Dados: mostra o cadastro do usuário logado */
void consultarMeusDados()
{
    int i = usuarioLogado;

    imprimirCabecalho("Meus Dados");
    printf("Nome: %s\n", usuarios[i].nome);
    printf("E-mail: %s\n", usuarios[i].email);
    printf("Perfil: %s\n", usuarios[i].perfil);
    if (strcmp(usuarios[i].perfil, "cliente") == 0)
    {
        printf("Telefone: %s\n", usuarios[i].telefone);
        printf("CPF: %s\n", usuarios[i].cpf);
    }
    if (strcmp(usuarios[i].perfil, "tecnico") == 0)
        printf("Especialidade: %s\n", usuarios[i].especialidade);
    pausar();
}

/* UC44 - Alterar Senha: troca a própria senha, confirmando a senha atual */
void alterarSenha()
{
    char atual[50], nova[50], confirmacao[50], hash[TAM_HASH];

    imprimirCabecalho("Alterar Senha");

    lerTexto("\nSenha atual (ENTER para voltar): ", atual, sizeof(atual));
    if (strlen(atual) == 0)
        return;
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
            printf("\n[!] As senhas nao conferem. Digite novamente.\n");
    } while (strcmp(nova, confirmacao) != 0);

    gerarHash(nova, usuarios[usuarioLogado].senha);
    dataHoraAgora(usuarios[usuarioLogado].atualizado_em);
    if (salvarUsuarios())
        printf("\n[+] Senha alterada com sucesso!");
    pausar();
}

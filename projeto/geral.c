/* geral.c - usuários: login, logout, meus dados, senha e os cadastros dos 4 perfis */
/* Funções do main.c usadas aqui: lerTexto (linha 121), lerInteiro (linha 137), confirmar (linha 161), pausar (linha 173), imprimirCabecalho (linha 215), dataHoraAgora (linha 204) */

/* Devolve 1 se o usuário logado tem este perfil (RN10) */
int perfilLogado(char perfil[])
{
    if (usuarioLogado >= 0 && strcmp(usuarios[usuarioLogado].perfil, perfil) == 0)
        return 1;
    return 0;
}

/* Quantos usuários do vetor já estão no banco (os novos ficam no fim do vetor) */
int usuariosNoBanco = 0;

/* Grava o vetor na tabela usuario: UPDATE de quem já existe, INSERT de quem é novo; devolve 1 se deu certo */
int salvarUsuarios()
{
    char sql[2500];
    char nome[130], email[130], telefone[50], senha[40], perfil[40], cpf[40];
    char especialidade[90], status[20], criado[50], atualizado[50];
    int posicaoUsuario, deuCerto = 1;

    /* bdExecutar está em banco.c, linha 61 */
    bdExecutar("SET IDENTITY_INSERT usuario ON"); /* deixa o programa escolher o id */
    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios && deuCerto; posicaoUsuario++)
    {
        /* sqlTexto está em banco.c, linha 100 */
        sqlTexto(nome, usuarios[posicaoUsuario].nome);
        sqlTexto(email, usuarios[posicaoUsuario].email);
        sqlTexto(telefone, usuarios[posicaoUsuario].telefone);
        sqlTexto(senha, usuarios[posicaoUsuario].senha);
        sqlTexto(perfil, usuarios[posicaoUsuario].perfil);
        sqlTexto(cpf, usuarios[posicaoUsuario].cpf);
        sqlTexto(especialidade, usuarios[posicaoUsuario].especialidade);
        sqlTexto(status, usuarios[posicaoUsuario].status);
        sqlTexto(criado, usuarios[posicaoUsuario].criado_em);
        sqlTexto(atualizado, usuarios[posicaoUsuario].atualizado_em);

        if (posicaoUsuario < usuariosNoBanco)
        {
            sprintf(sql, "UPDATE usuario SET nome = %s, email = %s, telefone = %s, senha = %s, perfil = %s, cpf = %s, "
                         "especialidade = %s, status = %s, criado_em = %s, atualizado_em = %s WHERE id_usuario = %d",
                    nome, email, telefone, senha, perfil, cpf, especialidade, status, criado, atualizado,
                    usuarios[posicaoUsuario].id_usuario);
        }
        else
        {
            sprintf(sql, "INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, "
                         "status, criado_em, atualizado_em) VALUES (%d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)",
                    usuarios[posicaoUsuario].id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status,
                    criado, atualizado);
        }
        deuCerto = bdExecutar(sql);
    }
    bdExecutar("SET IDENTITY_INSERT usuario OFF");

    if (deuCerto)
        usuariosNoBanco = totalUsuarios;
    return deuCerto;
}

/* Lê a tabela usuario para o vetor */
void carregarUsuarios()
{
    char numero[20];
    int novaPosicao;

    totalUsuarios = 0;
    usuariosNoBanco = 0;
    /* CONVERT 120 devolve a data como AAAA-MM-DD HH:MM:SS */
    /* bdExecutar está em banco.c, linha 61 */
    if (!bdExecutar("SELECT id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, "
                    "CONVERT(VARCHAR(19), criado_em, 120), CONVERT(VARCHAR(19), atualizado_em, 120) "
                    "FROM usuario ORDER BY id_usuario"))
        return;

    /* Cada linha do SELECT vai para a próxima posição do vetor; as colunas seguem a ordem do SELECT */
    /* bdProximaLinha está em banco.c, linha 80 */
    while (totalUsuarios < MAX_USUARIOS && bdProximaLinha())
    {
        novaPosicao = totalUsuarios;
        /* bdLerTexto está em banco.c, linha 88 */
        bdLerTexto(1, numero, sizeof(numero));
        usuarios[novaPosicao].id_usuario = atoi(numero);
        bdLerTexto(2, usuarios[novaPosicao].nome, sizeof(usuarios[novaPosicao].nome));
        bdLerTexto(3, usuarios[novaPosicao].email, sizeof(usuarios[novaPosicao].email));
        bdLerTexto(4, usuarios[novaPosicao].telefone, sizeof(usuarios[novaPosicao].telefone));
        bdLerTexto(5, usuarios[novaPosicao].senha, sizeof(usuarios[novaPosicao].senha));
        bdLerTexto(6, usuarios[novaPosicao].perfil, sizeof(usuarios[novaPosicao].perfil));
        bdLerTexto(7, usuarios[novaPosicao].cpf, sizeof(usuarios[novaPosicao].cpf));
        bdLerTexto(8, usuarios[novaPosicao].especialidade, sizeof(usuarios[novaPosicao].especialidade));
        bdLerTexto(9, usuarios[novaPosicao].status, sizeof(usuarios[novaPosicao].status));
        bdLerTexto(10, usuarios[novaPosicao].criado_em, sizeof(usuarios[novaPosicao].criado_em));
        bdLerTexto(11, usuarios[novaPosicao].atualizado_em, sizeof(usuarios[novaPosicao].atualizado_em));
        totalUsuarios++;
    }
    usuariosNoBanco = totalUsuarios;
}

/* Coloca um usuário de teste no fim do vetor, com a senha byte123 (usado no main quando o banco está vazio) */
void adicionarUsuarioInicial(char nome[], char email[], char telefone[], char perfil[], char cpf[], char especialidade[])
{
    int novaPosicao = totalUsuarios;

    usuarios[novaPosicao].id_usuario = novaPosicao + 1;
    strcpy(usuarios[novaPosicao].nome, nome);
    strcpy(usuarios[novaPosicao].email, email);
    strcpy(usuarios[novaPosicao].telefone, telefone);
    /* gerarHash está em main.c, linha 180 */
    gerarHash("byte123", usuarios[novaPosicao].senha);
    strcpy(usuarios[novaPosicao].perfil, perfil);
    strcpy(usuarios[novaPosicao].cpf, cpf);
    strcpy(usuarios[novaPosicao].especialidade, especialidade);
    strcpy(usuarios[novaPosicao].status, "ativo");
    dataHoraAgora(usuarios[novaPosicao].criado_em);
    strcpy(usuarios[novaPosicao].atualizado_em, usuarios[novaPosicao].criado_em);
    totalUsuarios++;
}

/* ---------- Funções comuns aos cadastros de usuário ---------- */

/* Devolve a posição no vetor do usuário com este id, ou -1 se não existir */
int buscarUsuarioPorId(int id)
{
    int posicaoUsuario;

    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        if (usuarios[posicaoUsuario].id_usuario == id)
            return posicaoUsuario;
    }
    return -1;
}

/* RN13: devolve 1 se outro usuário já usa este e-mail (ignorar = posição do próprio cadastro) */
int emailEmUso(char email[], int ignorar)
{
    int posicaoUsuario;

    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        if (posicaoUsuario != ignorar && strcmp(usuarios[posicaoUsuario].email, email) == 0)
            return 1;
    }
    return 0;
}

/* RN13: devolve 1 se outro cliente já usa este CPF */
int cpfEmUso(char cpf[], int ignorar)
{
    int posicaoUsuario;

    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        if (posicaoUsuario != ignorar && strcmp(usuarios[posicaoUsuario].cpf, cpf) == 0)
            return 1;
    }
    return 0;
}

/* RNF09: devolve 0 e avisa se o campo obrigatório ficou vazio */
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

/* Devolve o id do cliente dono do equipamento, ou -1 se o equipamento não existir */
int donoDoEquipamento(int idEquipamento)
{
    int posicaoEquipamento;

    for (posicaoEquipamento = 0; posicaoEquipamento < totalEquipamentos; posicaoEquipamento++)
    {
        if (equipamentos[posicaoEquipamento].id_equipamento == idEquipamento)
            return equipamentos[posicaoEquipamento].id_cliente;
    }
    return -1;
}

/* Devolve a posição em tecnicosOS da atribuição deste técnico nesta OS, ou -1 se ele não está na OS */
int buscarTecnicoNaOS(int idTecnico, int idOS)
{
    int posicaoAtribuicao;

    for (posicaoAtribuicao = 0; posicaoAtribuicao < totalTecnicosOS; posicaoAtribuicao++)
    {
        if (tecnicosOS[posicaoAtribuicao].id_tecnico == idTecnico && tecnicosOS[posicaoAtribuicao].id_os == idOS)
            return posicaoAtribuicao;
    }
    return -1;
}

/* RN14: devolve 1 se o usuário está numa OS aberta ou em andamento (como atendente, cliente ou técnico) */
int temOSEmAndamento(int idUsuario)
{
    int posicaoOS;

    for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
    {
        if (strcmp(ordens[posicaoOS].status, "aberta") != 0 && strcmp(ordens[posicaoOS].status, "em_andamento") != 0)
            continue; /* OS encerrada não conta */

        if (ordens[posicaoOS].id_atendente == idUsuario)
            return 1;
        /* donoDoEquipamento está em geral.c, linha 174 */
        if (donoDoEquipamento(ordens[posicaoOS].id_equipamento) == idUsuario)
            return 1;
        /* buscarTecnicoNaOS está em geral.c, linha 187 */
        if (buscarTecnicoNaOS(idUsuario, ordens[posicaoOS].id_os) >= 0)
            return 1;
    }
    return 0;
}

/* RN10: devolve 1 se o logado pode ver este cadastro; o técnico só vê os clientes das OS dele */
int podeVerUsuario(int posicaoUsuario, char perfil[])
{
    int posicaoOS;

    if (strcmp(usuarios[posicaoUsuario].perfil, perfil) != 0)
        return 0;
    /* perfilLogado está em geral.c, linha 5 */
    if (!perfilLogado("tecnico"))
        return 1;

    for (posicaoOS = 0; posicaoOS < totalOrdens; posicaoOS++)
    {
        /* donoDoEquipamento está em geral.c, linha 174 */
        /* buscarTecnicoNaOS está em geral.c, linha 187 */
        if (donoDoEquipamento(ordens[posicaoOS].id_equipamento) == usuarios[posicaoUsuario].id_usuario &&
            buscarTecnicoNaOS(usuarios[usuarioLogado].id_usuario, ordens[posicaoOS].id_os) >= 0)
            return 1;
    }
    return 0;
}

/* Devolve 1 se o cadastro entra na lista: o logado pode ver e o status bate ("" aceita todos) */
int usuarioEntraNaLista(int posicaoUsuario, char perfil[], char status[])
{
    /* podeVerUsuario está em geral.c, linha 222 */
    if (!podeVerUsuario(posicaoUsuario, perfil))
        return 0;
    if (strlen(status) > 0 && strcmp(usuarios[posicaoUsuario].status, status) != 0)
        return 0;
    return 1;
}

/* Lista os usuários do perfil que têm o filtro no nome, e-mail ou CPF; devolve quantos mostrou */
int listarUsuarios(char perfil[], char filtro[], char status[])
{
    int posicaoUsuario, mostrados = 0;

    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        /* usuarioEntraNaLista está em geral.c, linha 244 */
        if (!usuarioEntraNaLista(posicaoUsuario, perfil, status))
            continue;
        /* strstr devolve NULL quando o filtro não aparece no texto */
        if (strstr(usuarios[posicaoUsuario].nome, filtro) == NULL &&
            strstr(usuarios[posicaoUsuario].email, filtro) == NULL &&
            strstr(usuarios[posicaoUsuario].cpf, filtro) == NULL)
            continue;

        printf("ID %d - %s - %s - %s", usuarios[posicaoUsuario].id_usuario, usuarios[posicaoUsuario].nome,
               usuarios[posicaoUsuario].email, usuarios[posicaoUsuario].status);
        if (strcmp(perfil, "cliente") == 0)
            printf(" - CPF %s - Telefone %s", usuarios[posicaoUsuario].cpf, usuarios[posicaoUsuario].telefone);
        if (strcmp(perfil, "tecnico") == 0)
            printf(" - %s", usuarios[posicaoUsuario].especialidade);
        printf("\n");
        mostrados++;
    }
    return mostrados;
}

/* Pede um filtro, lista e pede o ID; devolve a posição do escolhido ou -1 */
int selecionarUsuario(char perfil[], char status[])
{
    char filtro[60];
    int id, posicaoUsuario;

    lerTexto("\nFiltro (nome, e-mail ou CPF), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
    if (strcmp(filtro, "0") == 0)
        return -1;

    /* listarUsuarios está em geral.c, linha 255 */
    if (listarUsuarios(perfil, filtro, status) == 0)
    {
        printf("\n[!] Nenhum cadastro encontrado para o filtro informado.");
        pausar();
        return -1;
    }

    id = lerInteiro("\nID (0 para voltar): ");
    if (id <= 0)
        return -1;

    /* buscarUsuarioPorId está em geral.c, linha 123 */
    posicaoUsuario = buscarUsuarioPorId(id);
    if (posicaoUsuario < 0)
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    /* usuarioEntraNaLista está em geral.c, linha 244 */
    if (!usuarioEntraNaLista(posicaoUsuario, perfil, status))
    {
        printf("\n[!] ID %d nao esta na lista.", id);
        pausar();
        return -1;
    }
    return posicaoUsuario;
}

/* Mostra o valor atual e lê o novo; ENTER mantém o atual */
void lerOuManter(char rotulo[], char atual[], char destino[], int tamanho)
{
    printf("%s [%s]: ", rotulo, atual);
    lerTexto("", destino, tamanho);
    if (strlen(destino) == 0)
        strcpy(destino, atual);
}

/* Cadastrar (UC04, UC08, UC12, UC16): cliente também tem telefone e CPF; técnico tem especialidade */
void cadastrarUsuario(char perfil[], char titulo[])
{
    char senha[50];
    int novaPosicao;

    imprimirCabecalho(titulo);

    if (totalUsuarios >= MAX_USUARIOS)
    {
        printf("\n[!] Limite de %d usuarios atingido.", MAX_USUARIOS);
        pausar();
        return;
    }

    /* Preenche a próxima posição livre; o total só aumenta se tudo der certo */
    novaPosicao = totalUsuarios;
    strcpy(usuarios[novaPosicao].telefone, "");
    strcpy(usuarios[novaPosicao].cpf, "");
    strcpy(usuarios[novaPosicao].especialidade, "");

    lerTexto("\nNome (ENTER para voltar): ", usuarios[novaPosicao].nome, sizeof(usuarios[novaPosicao].nome));
    if (strlen(usuarios[novaPosicao].nome) == 0)
        return;
    lerTexto("E-mail: ", usuarios[novaPosicao].email, sizeof(usuarios[novaPosicao].email));
    if (strcmp(perfil, "cliente") == 0)
    {
        lerTexto("Telefone: ", usuarios[novaPosicao].telefone, sizeof(usuarios[novaPosicao].telefone));
        lerTexto("CPF: ", usuarios[novaPosicao].cpf, sizeof(usuarios[novaPosicao].cpf));
    }
    if (strcmp(perfil, "tecnico") == 0)
        lerTexto("Especialidade: ", usuarios[novaPosicao].especialidade, sizeof(usuarios[novaPosicao].especialidade));
    lerTexto("Senha: ", senha, sizeof(senha));

    /* Campos obrigatórios (RNF09) */
    /* campoPreenchido está em geral.c, linha 162 */
    if (!campoPreenchido(usuarios[novaPosicao].email, "E-mail"))
        return;
    if (!campoPreenchido(senha, "Senha"))
        return;
    if (strcmp(perfil, "cliente") == 0 && !campoPreenchido(usuarios[novaPosicao].cpf, "CPF"))
        return;
    if (strcmp(perfil, "tecnico") == 0 && !campoPreenchido(usuarios[novaPosicao].especialidade, "Especialidade"))
        return;

    /* E-mail e CPF não podem repetir (RN13) */
    /* emailEmUso está em geral.c, linha 136 */
    if (emailEmUso(usuarios[novaPosicao].email, novaPosicao))
    {
        printf("\n[!] Este e-mail ja pertence a outro usuario (RN13). Nada foi gravado.");
        pausar();
        return;
    }
    /* cpfEmUso está em geral.c, linha 149 */
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(usuarios[novaPosicao].cpf, novaPosicao))
    {
        printf("\n[!] Este CPF ja esta cadastrado (RN13). Nada foi gravado.");
        pausar();
        return;
    }

    /* O novo id é o id do último + 1 */
    usuarios[novaPosicao].id_usuario = usuarios[novaPosicao - 1].id_usuario + 1;
    strcpy(usuarios[novaPosicao].perfil, perfil);
    strcpy(usuarios[novaPosicao].status, "ativo");
    /* gerarHash está em main.c, linha 180 */
    gerarHash(senha, usuarios[novaPosicao].senha); /* RNF10: grava o hash, não a senha */
    dataHoraAgora(usuarios[novaPosicao].criado_em);
    strcpy(usuarios[novaPosicao].atualizado_em, usuarios[novaPosicao].criado_em);

    totalUsuarios++;
    /* salvarUsuarios está em geral.c, linha 16 */
    if (salvarUsuarios())
        printf("\n[+] Cadastro realizado com sucesso! ID: %d", usuarios[novaPosicao].id_usuario);
    else
        totalUsuarios--; /* não gravou: desfaz */
    pausar();
}

/* Consultar (UC05, UC09, UC13, UC17): lista os cadastros com o status */
void consultarUsuarios(char perfil[], char titulo[])
{
    char filtro[60];
    int mostrados;

    imprimirCabecalho(titulo);

    /* Pede outro filtro enquanto nada for encontrado (exceção 4a) */
    do
    {
        lerTexto("\nFiltro (nome, e-mail ou CPF), ENTER para listar todos ou 0 para voltar: ", filtro, sizeof(filtro));
        if (strcmp(filtro, "0") == 0)
            return;

        /* listarUsuarios está em geral.c, linha 255 */
        mostrados = listarUsuarios(perfil, filtro, "");
        if (mostrados == 0)
            printf("\n[!] Nenhum cadastro encontrado para o filtro informado. Tente outro filtro.\n");
    } while (mostrados == 0);
    pausar();
}

/* Editar (UC06, UC10, UC14, UC18): ENTER mantém o valor atual */
void editarUsuario(char perfil[], char titulo[])
{
    char nome[60], email[60], telefone[20], cpf[15], especialidade[40], senha[50];
    int posicaoUsuario;

    imprimirCabecalho(titulo);

    /* selecionarUsuario está em geral.c, linha 283 */
    posicaoUsuario = selecionarUsuario(perfil, "");
    if (posicaoUsuario < 0)
        return;

    /* Lê em variáveis locais; o cadastro só muda depois das validações */
    printf("\nDeixe em branco para manter o valor atual.\n");
    /* lerOuManter está em geral.c, linha 323 */
    lerOuManter("Nome", usuarios[posicaoUsuario].nome, nome, sizeof(nome));
    lerOuManter("E-mail", usuarios[posicaoUsuario].email, email, sizeof(email));
    strcpy(telefone, usuarios[posicaoUsuario].telefone);
    strcpy(cpf, usuarios[posicaoUsuario].cpf);
    strcpy(especialidade, usuarios[posicaoUsuario].especialidade);
    if (strcmp(perfil, "cliente") == 0)
    {
        lerOuManter("Telefone", usuarios[posicaoUsuario].telefone, telefone, sizeof(telefone));
        lerOuManter("CPF", usuarios[posicaoUsuario].cpf, cpf, sizeof(cpf));
    }
    if (strcmp(perfil, "tecnico") == 0)
        lerOuManter("Especialidade", usuarios[posicaoUsuario].especialidade, especialidade, sizeof(especialidade));
    lerTexto("Nova senha [manter]: ", senha, sizeof(senha));

    /* emailEmUso está em geral.c, linha 136 */
    if (emailEmUso(email, posicaoUsuario))
    {
        printf("\n[!] Este e-mail ja pertence a outro usuario (RN13). Nada foi alterado.");
        pausar();
        return;
    }
    /* cpfEmUso está em geral.c, linha 149 */
    if (strcmp(perfil, "cliente") == 0 && cpfEmUso(cpf, posicaoUsuario))
    {
        printf("\n[!] Este CPF ja pertence a outro cliente (RN13). Nada foi alterado.");
        pausar();
        return;
    }

    strcpy(usuarios[posicaoUsuario].nome, nome);
    strcpy(usuarios[posicaoUsuario].email, email);
    strcpy(usuarios[posicaoUsuario].telefone, telefone);
    strcpy(usuarios[posicaoUsuario].cpf, cpf);
    strcpy(usuarios[posicaoUsuario].especialidade, especialidade);
    /* gerarHash está em main.c, linha 180 */
    if (strlen(senha) > 0)
        gerarHash(senha, usuarios[posicaoUsuario].senha);
    dataHoraAgora(usuarios[posicaoUsuario].atualizado_em);
    /* salvarUsuarios está em geral.c, linha 16 */
    if (salvarUsuarios())
        printf("\n[+] Cadastro de %s atualizado com sucesso!", usuarios[posicaoUsuario].nome);
    pausar();
}

/* Inativar (UC07, UC11, UC15, UC19): não apaga (RN12); bloqueia a si mesmo (RN17) e quem tem OS aberta (RN14) */
void inativarUsuario(char perfil[], char titulo[])
{
    int posicaoUsuario;

    imprimirCabecalho(titulo);

    /* selecionarUsuario está em geral.c, linha 283 */
    posicaoUsuario = selecionarUsuario(perfil, "ativo");
    if (posicaoUsuario < 0)
        return;

    if (posicaoUsuario == usuarioLogado)
    {
        printf("\n[!] Voce nao pode inativar o proprio cadastro (RN17).");
        pausar();
        return;
    }
    /* temOSEmAndamento está em geral.c, linha 200 */
    if (temOSEmAndamento(usuarios[posicaoUsuario].id_usuario))
    {
        printf("\n[!] %s tem ordem de servico aberta ou em andamento. Inativacao bloqueada (RN14).", usuarios[posicaoUsuario].nome);
        pausar();
        return;
    }

    printf("\nInativar %s.", usuarios[posicaoUsuario].nome);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao de inativacao cancelada pelo usuario.");
        pausar();
        return;
    }

    strcpy(usuarios[posicaoUsuario].status, "inativo");
    dataHoraAgora(usuarios[posicaoUsuario].atualizado_em);
    /* salvarUsuarios está em geral.c, linha 16 */
    if (salvarUsuarios())
        printf("\n[+] %s inativado(a). O historico foi preservado e o acesso esta bloqueado (RN12).", usuarios[posicaoUsuario].nome);
    pausar();
}

/* Reativar (UC45, UC46, UC47, UC48): volta um cadastro inativo para ativo */
void reativarUsuario(char perfil[], char titulo[])
{
    int posicaoUsuario, inativos = 0;

    imprimirCabecalho(titulo);

    /* Conta os inativos deste perfil (exceção 2a: nenhum) */
    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        if (strcmp(usuarios[posicaoUsuario].perfil, perfil) == 0 && strcmp(usuarios[posicaoUsuario].status, "inativo") == 0)
            inativos++;
    }
    if (inativos == 0)
    {
        printf("\n[!] Nenhum cadastro inativo. Nao ha cadastro para reativar.");
        pausar();
        return;
    }

    /* selecionarUsuario está em geral.c, linha 283 */
    posicaoUsuario = selecionarUsuario(perfil, "inativo");
    if (posicaoUsuario < 0)
        return;

    printf("\nReativar %s.", usuarios[posicaoUsuario].nome);
    if (!confirmar("\nTem certeza?"))
    {
        printf("\nOperacao cancelada. O cadastro continua inativo.");
        pausar();
        return;
    }

    strcpy(usuarios[posicaoUsuario].status, "ativo");
    dataHoraAgora(usuarios[posicaoUsuario].atualizado_em);
    /* salvarUsuarios está em geral.c, linha 16 */
    if (salvarUsuarios())
        printf("\n[+] %s reativado(a). O acesso ao sistema foi liberado.", usuarios[posicaoUsuario].nome);
    pausar();
}

/* UC01 - Realizar Login: devolve a posição do usuário ou -1 */
int realizarLogin()
{
    char email[60], senha[50], hash[TAM_HASH];
    int posicaoUsuario;

    imprimirCabecalho("Entrar");
    lerTexto("E-mail (ENTER para voltar): ", email, sizeof(email));
    if (strlen(email) == 0)
        return -1;
    lerTexto("Senha: ", senha, sizeof(senha));

    /* O banco guarda o hash da senha, então compara o hash do que foi digitado */
    /* gerarHash está em main.c, linha 180 */
    gerarHash(senha, hash);
    for (posicaoUsuario = 0; posicaoUsuario < totalUsuarios; posicaoUsuario++)
    {
        if (strcmp(email, usuarios[posicaoUsuario].email) == 0 && strcmp(hash, usuarios[posicaoUsuario].senha) == 0)
        {
            if (strcmp(usuarios[posicaoUsuario].status, "ativo") != 0)
            {
                printf("\n[!] Usuario inativo. O acesso esta bloqueado (RN12).");
                pausar();
                return -1;
            }
            return posicaoUsuario;
        }
    }

    printf("\n[!] E-mail ou senha incorretos.");
    pausar();
    return -1;
}

/* UC02 - Fazer Logout: pergunta se quer sair; se sim, ninguém fica logado e volta para a tela inicial */
void fazerLogout()
{
    if (!confirmar("\nTem certeza que deseja sair?"))
        return;

    usuarioLogado = -1;
    printf("\nEncerrando a sessao...\n");
}

/* UC03 - Consultar Meus Dados: mostra o cadastro do usuário logado */
void consultarMeusDados()
{
    imprimirCabecalho("Meus Dados");
    printf("Nome: %s\n", usuarios[usuarioLogado].nome);
    printf("E-mail: %s\n", usuarios[usuarioLogado].email);
    printf("Perfil: %s\n", usuarios[usuarioLogado].perfil);
    if (strcmp(usuarios[usuarioLogado].perfil, "cliente") == 0)
    {
        printf("Telefone: %s\n", usuarios[usuarioLogado].telefone);
        printf("CPF: %s\n", usuarios[usuarioLogado].cpf);
    }
    if (strcmp(usuarios[usuarioLogado].perfil, "tecnico") == 0)
        printf("Especialidade: %s\n", usuarios[usuarioLogado].especialidade);
    pausar();
}

/* UC44 - Alterar Senha: confere a senha atual e pede a nova duas vezes */
void alterarSenha()
{
    char atual[50], nova[50], confirmacao[50], hash[TAM_HASH];

    imprimirCabecalho("Alterar Senha");

    lerTexto("\nSenha atual (ENTER para voltar): ", atual, sizeof(atual));
    if (strlen(atual) == 0)
        return;
    /* gerarHash está em main.c, linha 180 */
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
    /* salvarUsuarios está em geral.c, linha 16 */
    if (salvarUsuarios())
        printf("\n[+] Senha alterada com sucesso!");
    pausar();
}

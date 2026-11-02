/*
 * Ponte temporaria entre o vetor usuarios[] e a tabela dbo.usuario.
 * Este arquivo e incluido por main.c APOS geral.c.
 * Depois, o projeto pode ser reorganizado em modulos .h/.c separados.
 */

/* Le um campo de texto de uma linha retornada pelo SQL Server. */
static int usuarioSQLLerTexto(SQLHSTMT consulta, SQLUSMALLINT coluna,
                             char *destino, size_t tamanho)
{
    SQLLEN indicador = 0;
    SQLRETURN retorno;

    destino[0] = '\0';
    retorno = SQLGetData(consulta, coluna, SQL_C_CHAR,
                         destino, (SQLLEN)tamanho, &indicador);

    if (indicador == SQL_NULL_DATA)
    {
        destino[0] = '\0';
        return SQL_SUCCEEDED(retorno);
    }

    /* Avisos de truncamento nao podem ser ignorados: evitamos dados incompletos. */
    if (retorno != SQL_SUCCESS)
        return 0;

    return 1;
}

/* Mostra o primeiro diagnostico do comando ODBC que falhou. */
static void usuarioSQLMostrarErro(SQLHSTMT comando)
{
    SQLCHAR estado[6], mensagem[512];
    SQLINTEGER codigo;
    SQLSMALLINT tamanho;

    if (SQL_SUCCEEDED(SQLGetDiagRec(SQL_HANDLE_STMT, comando, 1,
                                     estado, &codigo, mensagem,
                                     sizeof(mensagem), &tamanho)))
    {
        fprintf(stderr, "SQLSTATE %s (%ld): %s\n",
                (char *)estado, (long)codigo, (char *)mensagem);
    }
}

/* Carrega a tabela usuario do banco para o vetor usado pelos menus. */
int carregarUsuariosSQL(Banco *banco)
{
    const char *consulta =
        "SELECT id_usuario, nome, email, ISNULL(telefone, ''), senha, perfil, "
        "ISNULL(cpf, ''), ISNULL(especialidade, ''), status, "
        "CONVERT(varchar(19), criado_em, 120), "
        "CONVERT(varchar(19), atualizado_em, 120) "
        "FROM dbo.usuario ORDER BY id_usuario";
    SQLRETURN retorno;

    totalUsuarios = 0;

    if (!bdConsultar(banco, consulta))
        return 0;

    while ((retorno = SQLFetch(banco->hstmt)) != SQL_NO_DATA)
    {
        SQLINTEGER id;
        SQLLEN indicador;
        Usuario *u;

        if (!SQL_SUCCEEDED(retorno) || totalUsuarios >= MAX_USUARIOS)
        {
            printf("Erro de leitura ou limite de %d usuarios excedido.\n", MAX_USUARIOS);
            goto erro;
        }

        u = &usuarios[totalUsuarios];
        memset(u, 0, sizeof(*u));

        retorno = SQLGetData(banco->hstmt, 1, SQL_C_SLONG,
                             &id, sizeof(id), &indicador);
        if (!SQL_SUCCEEDED(retorno) || indicador == SQL_NULL_DATA)
            goto erro;
        u->id_usuario = (int)id;

        if (!usuarioSQLLerTexto(banco->hstmt, 2, u->nome, sizeof(u->nome)) ||
            !usuarioSQLLerTexto(banco->hstmt, 3, u->email, sizeof(u->email)) ||
            !usuarioSQLLerTexto(banco->hstmt, 4, u->telefone, sizeof(u->telefone)) ||
            !usuarioSQLLerTexto(banco->hstmt, 5, u->senha, sizeof(u->senha)) ||
            !usuarioSQLLerTexto(banco->hstmt, 6, u->perfil, sizeof(u->perfil)) ||
            !usuarioSQLLerTexto(banco->hstmt, 7, u->cpf, sizeof(u->cpf)) ||
            !usuarioSQLLerTexto(banco->hstmt, 8, u->especialidade, sizeof(u->especialidade)) ||
            !usuarioSQLLerTexto(banco->hstmt, 9, u->status, sizeof(u->status)) ||
            !usuarioSQLLerTexto(banco->hstmt, 10, u->criado_em, sizeof(u->criado_em)) ||
            !usuarioSQLLerTexto(banco->hstmt, 11, u->atualizado_em, sizeof(u->atualizado_em)))
        {
            printf("Erro ao ler usuario ID %d (talvez campo maior que o vetor).\n", u->id_usuario);
            goto erro;
        }

        totalUsuarios++;
    }

    bdEncerrarConsulta(banco);
    printf("Usuarios carregados do SQL Server: %d\n", totalUsuarios);
    return 1;

erro:
    usuarioSQLMostrarErro(banco->hstmt);
    bdEncerrarConsulta(banco);
    totalUsuarios = 0;
    return 0;
}

/*
 * Faz o PRIMEIRO cadastro administrativo apenas quando a tabela esta vazia.
 * Os valores do usuario sao enviados como parametros (sem concatenar SQL).
 */
int criarAdministradorInicialSQL(Banco *banco)
{
    char nome[60], email[60], senha[50], confirmacao[50], hash[TAM_HASH];
    SQLHSTMT comando = SQL_NULL_HSTMT;
    SQLRETURN retorno;
    SQLLEN comprimentos[3] = {SQL_NTS, SQL_NTS, SQL_NTS};
    const char *sql =
        "INSERT INTO dbo.usuario (nome, email, senha, perfil) "
        "SELECT ?, ?, ?, 'admin' "
        "WHERE NOT EXISTS (SELECT 1 FROM dbo.usuario)";

    if (totalUsuarios != 0)
    {
        printf("A inicializacao administrativa so funciona com o banco vazio.\n");
        return 0;
    }

    printf("\n=== CONFIGURACAO DO PRIMEIRO ADMINISTRADOR ===\n");
    lerTexto("Nome: ", nome, sizeof(nome));
    lerTexto("E-mail: ", email, sizeof(email));
    lerSenha("Senha: ", senha, sizeof(senha));
    lerSenha("Confirme a senha: ", confirmacao, sizeof(confirmacao));

    if (!nome[0] || !emailValido(email) || !senha[0] ||
        strcmp(senha, confirmacao) != 0)
    {
        printf("Dados invalidos ou senhas diferentes. Cadastro cancelado.\n");
        return 0;
    }

    /* Compatibilidade TEMPORARIA com gerarHash() do login do ByteTech.
       Este hash simples NAO e seguro para uso real. */
    gerarHash(senha, hash);
    if (!*hash) { printf("Erro ao proteger senha.\n"); return 0; }

    retorno = SQLAllocHandle(SQL_HANDLE_STMT, banco->hdbc, &comando);
    if (!SQL_SUCCEEDED(retorno))
        return 0;

    retorno = SQLPrepare(comando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 1, SQL_PARAM_INPUT,
                               SQL_C_CHAR, SQL_VARCHAR, 120, 0,
                               nome, sizeof(nome), &comprimentos[0]);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 2, SQL_PARAM_INPUT,
                               SQL_C_CHAR, SQL_VARCHAR, 120, 0,
                               email, sizeof(email), &comprimentos[1]);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 3, SQL_PARAM_INPUT,
                               SQL_C_CHAR, SQL_VARCHAR, 255, 0,
                               hash, sizeof(hash), &comprimentos[2]);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLExecute(comando);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    printf("Administrador inicial cadastrado no SQL Server!\n");
    return 1;

erro:
    printf("Erro ao cadastrar administrador inicial.\n");
    usuarioSQLMostrarErro(comando);
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 0;
}

/* Insere um usuario no SQL Server e recupera o ID gerado pelo IDENTITY. */
int inserirUsuarioSQL(Banco *banco, Usuario *usuario)
{
    const char *sql =
        "INSERT INTO dbo.usuario "
        "(nome, email, telefone, senha, perfil, cpf, especialidade) "
        "OUTPUT INSERTED.id_usuario "
        "VALUES (?, ?, ?, ?, ?, ?, ?)";
    SQLHSTMT comando = SQL_NULL_HSTMT;
    SQLRETURN retorno;
    SQLLEN indicadores[7];
    SQLINTEGER novoId = 0;
    SQLLEN indicadorId = 0;
    char *valores[7];
    SQLULEN tamanhosSql[7] = {120, 120, 20, 255, 20, 14, 80};
    SQLLEN tamanhosBuffer[7] = {
        sizeof(usuario->nome), sizeof(usuario->email),
        sizeof(usuario->telefone), sizeof(usuario->senha),
        sizeof(usuario->perfil), sizeof(usuario->cpf),
        sizeof(usuario->especialidade)
    };

    if (banco == NULL || usuario == NULL || banco->hdbc == SQL_NULL_HDBC)
        return 0;

    valores[0] = usuario->nome;
    valores[1] = usuario->email;
    valores[2] = usuario->telefone;
    valores[3] = usuario->senha;
    valores[4] = usuario->perfil;
    valores[5] = usuario->cpf;
    valores[6] = usuario->especialidade;

    retorno = SQLAllocHandle(SQL_HANDLE_STMT, banco->hdbc, &comando);
    if (!SQL_SUCCEEDED(retorno))
        return 0;

    retorno = SQLPrepare(comando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    for (int i = 0; i < 7; i++)
    {
        /* Telefone, CPF e especialidade ficam como NULL quando vazios. */
        indicadores[i] = (i == 2 || i == 5 || i == 6) &&
                          valores[i][0] == '\0' ? SQL_NULL_DATA : SQL_NTS;

        retorno = SQLBindParameter(comando, (SQLUSMALLINT)(i + 1),
                                  SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                                  tamanhosSql[i], 0, valores[i],
                                  tamanhosBuffer[i], &indicadores[i]);
        if (!SQL_SUCCEEDED(retorno))
            goto erro;
    }

    retorno = SQLExecute(comando);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLFetch(comando);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLGetData(comando, 1, SQL_C_SLONG,
                         &novoId, sizeof(novoId), &indicadorId);
    if (!SQL_SUCCEEDED(retorno) || indicadorId == SQL_NULL_DATA)
        goto erro;

    usuario->id_usuario = (int)novoId;
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 1;

erro:
    printf("[!] Falha ao cadastrar usuario no SQL Server.\n");
    usuarioSQLMostrarErro(comando);
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 0;
}


/* Atualiza os dados de um usuario existente, sem alterar seu perfil/status. */
int atualizarUsuarioSQL(Banco *banco, Usuario *usuario)
{
    const char *sql =
        "UPDATE dbo.usuario SET nome=?, email=?, telefone=?, senha=?, "
        "cpf=?, especialidade=?, atualizado_em=SYSDATETIME() "
        "WHERE id_usuario=?";
    SQLHSTMT comando = SQL_NULL_HSTMT;
    SQLRETURN retorno;
    SQLLEN indicadores[6];
    SQLLEN indicadorId = 0;
    SQLLEN linhasAfetadas = 0;
    SQLINTEGER id;
    char *valores[6];
    SQLULEN tamanhosSql[6] = {120, 120, 20, 255, 14, 80};
    SQLLEN tamanhosBuffer[6] = {
        sizeof(usuario->nome), sizeof(usuario->email),
        sizeof(usuario->telefone), sizeof(usuario->senha),
        sizeof(usuario->cpf), sizeof(usuario->especialidade)
    };

    if (banco == NULL || usuario == NULL || banco->hdbc == SQL_NULL_HDBC)
        return 0;

    valores[0] = usuario->nome;
    valores[1] = usuario->email;
    valores[2] = usuario->telefone;
    valores[3] = usuario->senha;
    valores[4] = usuario->cpf;
    valores[5] = usuario->especialidade;
    id = (SQLINTEGER)usuario->id_usuario;

    retorno = SQLAllocHandle(SQL_HANDLE_STMT, banco->hdbc, &comando);
    if (!SQL_SUCCEEDED(retorno))
        return 0;

    retorno = SQLPrepare(comando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    for (int i = 0; i < 6; i++)
    {
        /* Os campos opcionais vazios viram NULL no banco. */
        indicadores[i] = (i == 2 || i == 4 || i == 5) &&
                          valores[i][0] == '\0' ? SQL_NULL_DATA : SQL_NTS;

        retorno = SQLBindParameter(comando, (SQLUSMALLINT)(i + 1),
                                  SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR,
                                  tamanhosSql[i], 0, valores[i],
                                  tamanhosBuffer[i], &indicadores[i]);
        if (!SQL_SUCCEEDED(retorno))
            goto erro;
    }

    retorno = SQLBindParameter(comando, 7, SQL_PARAM_INPUT,
                               SQL_C_SLONG, SQL_INTEGER, 0, 0,
                               &id, sizeof(id), &indicadorId);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLExecute(comando);
    if (!SQL_SUCCEEDED(retorno) && retorno != SQL_NO_DATA)
        goto erro;

    retorno = SQLRowCount(comando, &linhasAfetadas);
    if (!SQL_SUCCEEDED(retorno) || linhasAfetadas == 0)
        goto erro;

    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 1;

erro:
    printf("[!] Falha ao editar usuario no SQL Server.\n");
    usuarioSQLMostrarErro(comando);
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 0;
}


/* Inativa um usuario sem exclui-lo. O banco impede a alteracao
   quando ele participa de OS aberta ou em andamento (RN12/RN14). */
int inativarUsuarioSQL(Banco *banco, int idUsuario)
{
    const char *sql =
        "UPDATE u SET status='inativo', atualizado_em=SYSDATETIME() "
        "FROM dbo.usuario AS u "
        "WHERE u.id_usuario=? AND u.status='ativo' "
        "AND NOT EXISTS ( "
        " SELECT 1 FROM dbo.os AS o "
        " LEFT JOIN dbo.equipamento AS e "
        " ON e.id_equipamento=o.id_equipamento "
        " WHERE o.status IN ('aberta', 'em_andamento') "
        " AND (o.id_atendente=u.id_usuario "
        " OR e.id_cliente=u.id_usuario "
        " OR EXISTS (SELECT 1 FROM dbo.tecnico_os AS tos "
        " WHERE tos.id_os=o.id_os AND tos.id_tecnico=u.id_usuario)) "
        ")";
    SQLHSTMT comando = SQL_NULL_HSTMT;
    SQLRETURN retorno;
    SQLINTEGER id = (SQLINTEGER)idUsuario;
    SQLLEN indicadorId = 0;
    SQLLEN linhasAfetadas = 0;

    if (banco == NULL || banco->hdbc == SQL_NULL_HDBC || idUsuario <= 0)
        return 0;

    retorno = SQLAllocHandle(SQL_HANDLE_STMT, banco->hdbc, &comando);
    if (!SQL_SUCCEEDED(retorno))
        return 0;

    retorno = SQLPrepare(comando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 1, SQL_PARAM_INPUT,
                               SQL_C_SLONG, SQL_INTEGER, 0, 0,
                               &id, sizeof(id), &indicadorId);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    // SQL_NO_DATA: o UPDATE rodou mas nao alterou linha (bloqueado pela regra).
    retorno = SQLExecute(comando);
    if (!SQL_SUCCEEDED(retorno) && retorno != SQL_NO_DATA)
        goto erro;

    retorno = SQLRowCount(comando, &linhasAfetadas);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return linhasAfetadas == 1;

erro:
    printf("[!] Erro ao tentar inativar o usuario no SQL Server.\n");
    usuarioSQLMostrarErro(comando);
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 0;
}


/* Reativa um usuario inativo, liberando de novo o acesso (UC45 a UC48). */
int reativarUsuarioSQL(Banco *banco, int idUsuario)
{
    const char *sql =
        "UPDATE dbo.usuario SET status='ativo', atualizado_em=SYSDATETIME() "
        "WHERE id_usuario=? AND status='inativo'";
    SQLHSTMT comando = SQL_NULL_HSTMT;
    SQLRETURN retorno;
    SQLINTEGER id = (SQLINTEGER)idUsuario;
    SQLLEN indicadorId = 0;
    SQLLEN linhasAfetadas = 0;

    if (banco == NULL || banco->hdbc == SQL_NULL_HDBC || idUsuario <= 0)
        return 0;

    retorno = SQLAllocHandle(SQL_HANDLE_STMT, banco->hdbc, &comando);
    if (!SQL_SUCCEEDED(retorno))
        return 0;

    retorno = SQLPrepare(comando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 1, SQL_PARAM_INPUT,
                               SQL_C_SLONG, SQL_INTEGER, 0, 0,
                               &id, sizeof(id), &indicadorId);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLExecute(comando);
    if (!SQL_SUCCEEDED(retorno) && retorno != SQL_NO_DATA)
        goto erro;

    retorno = SQLRowCount(comando, &linhasAfetadas);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return linhasAfetadas == 1;

erro:
    printf("[!] Erro ao tentar reativar o usuario no SQL Server.\n");
    usuarioSQLMostrarErro(comando);
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 0;
}


/* Altera apenas a senha do usuario logado, preservando todos os outros dados.
   Confere o hash anterior para evitar sobrescrever uma senha alterada em paralelo.
   Hash atual e temporariamente o mesmo usado por gerarHash() no login. */
int alterarSenhaUsuarioSQL(Banco *banco, int idUsuario,
                          const char *hashAtual, const char *hashNovo)
{
    const char *sql =
        "UPDATE dbo.usuario SET senha=?, atualizado_em=SYSDATETIME() "
        "WHERE id_usuario=? AND senha=? AND status='ativo'";
    SQLHSTMT comando = SQL_NULL_HSTMT;
    SQLRETURN retorno;
    SQLINTEGER id = (SQLINTEGER)idUsuario;
    SQLLEN indicadorNovo = SQL_NTS;
    SQLLEN indicadorAtual = SQL_NTS;
    SQLLEN indicadorId = 0;
    SQLLEN linhasAfetadas = 0;

    if (banco == NULL || banco->hdbc == SQL_NULL_HDBC ||
        idUsuario <= 0 || hashAtual == NULL || hashNovo == NULL ||
        hashAtual[0] == '\0' || hashNovo[0] == '\0')
        return 0;

    retorno = SQLAllocHandle(SQL_HANDLE_STMT, banco->hdbc, &comando);
    if (!SQL_SUCCEEDED(retorno))
        return 0;

    retorno = SQLPrepare(comando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 1, SQL_PARAM_INPUT,
                               SQL_C_CHAR, SQL_VARCHAR, 255, 0,
                               (SQLPOINTER)hashNovo, (SQLLEN)(strlen(hashNovo) + 1),
                               &indicadorNovo);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 2, SQL_PARAM_INPUT,
                               SQL_C_SLONG, SQL_INTEGER, 0, 0,
                               &id, sizeof(id), &indicadorId);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLBindParameter(comando, 3, SQL_PARAM_INPUT,
                               SQL_C_CHAR, SQL_VARCHAR, 255, 0,
                               (SQLPOINTER)hashAtual, (SQLLEN)(strlen(hashAtual) + 1),
                               &indicadorAtual);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    retorno = SQLExecute(comando);
    if (!SQL_SUCCEEDED(retorno) && retorno != SQL_NO_DATA)
        goto erro;

    retorno = SQLRowCount(comando, &linhasAfetadas);
    if (!SQL_SUCCEEDED(retorno))
        goto erro;

    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    if (linhasAfetadas == 1)
        return 1;

    printf("[!] Usuario nao encontrado, inativo ou senha alterada em outra sessao.\n");
    return 0;

erro:
    printf("[!] Falha ao alterar senha no SQL Server.\n");
    usuarioSQLMostrarErro(comando);
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 0;
}

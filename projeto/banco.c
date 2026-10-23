/* banco.c - conversa com o SQL Server (ODBC). Compilar com: gcc main.c -o output/main.exe -lodbc32 */

/* Banco bytetech nesta máquina, com o login do Windows. Se só tiver o driver 17, troque 18 por 17 */
#define CONEXAO_SQLSERVER "DRIVER={ODBC Driver 18 for SQL Server};" \
                          "SERVER=localhost;"                        \
                          "DATABASE=bytetech;"                       \
                          "Trusted_Connection=yes;"                  \
                          "Encrypt=yes;"                             \
                          "TrustServerCertificate=yes;"

/* Ligações com o ODBC: o ambiente, a conexão e o último comando executado */
SQLHENV  bdAmbiente = SQL_NULL_HENV;
SQLHDBC  bdConexao = SQL_NULL_HDBC;
SQLHSTMT bdComando = SQL_NULL_HSTMT;

/* Mostra a mensagem de erro que o SQL Server devolveu */
void bdMostrarErro(SQLSMALLINT tipo, SQLHANDLE handle)
{
    SQLCHAR estado[7];
    SQLCHAR mensagem[512];
    SQLINTEGER erroNativo;
    SQLSMALLINT tamanho;

    if (SQLGetDiagRec(tipo, handle, 1, estado, &erroNativo, mensagem, sizeof(mensagem), &tamanho) == SQL_SUCCESS)
        printf("\n[!] Banco de dados: %s\n", mensagem);
}

/* Abre a conexão com o banco; devolve 1 se conectou */
int bdConectar()
{
    SQLRETURN resultado;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &bdAmbiente);
    SQLSetEnvAttr(bdAmbiente, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, bdAmbiente, &bdConexao);
    SQLSetConnectAttr(bdConexao, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0); /* espera até 5 segundos */

    resultado = SQLDriverConnect(bdConexao, NULL, (SQLCHAR *)CONEXAO_SQLSERVER, SQL_NTS,
                                 NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    if (!SQL_SUCCEEDED(resultado))
    {
        /* bdMostrarErro está em banco.c, linha 17 */
        bdMostrarErro(SQL_HANDLE_DBC, bdConexao);
        return 0;
    }
    return 1;
}

/* Fecha a conexão com o banco */
void bdDesconectar()
{
    if (bdComando != SQL_NULL_HSTMT)
        SQLFreeHandle(SQL_HANDLE_STMT, bdComando);
    SQLDisconnect(bdConexao);
    SQLFreeHandle(SQL_HANDLE_DBC, bdConexao);
    SQLFreeHandle(SQL_HANDLE_ENV, bdAmbiente);
}

/* Roda um comando no banco (SELECT, INSERT, UPDATE ou SET); devolve 1 se deu certo.
   As linhas de um SELECT são lidas depois com bdProximaLinha e bdLerTexto */
int bdExecutar(char sql[])
{
    SQLRETURN resultado;

    if (bdComando != SQL_NULL_HSTMT)
        SQLFreeHandle(SQL_HANDLE_STMT, bdComando); /* fecha o comando anterior */
    SQLAllocHandle(SQL_HANDLE_STMT, bdConexao, &bdComando);

    resultado = SQLExecDirect(bdComando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(resultado) && resultado != SQL_NO_DATA) /* NO_DATA: rodou, mas não mudou nenhuma linha */
    {
        /* bdMostrarErro está em banco.c, linha 17 */
        bdMostrarErro(SQL_HANDLE_STMT, bdComando);
        return 0;
    }
    return 1;
}

/* Passa para a próxima linha do SELECT; devolve 0 quando as linhas acabaram */
int bdProximaLinha()
{
    if (SQL_SUCCEEDED(SQLFetch(bdComando)))
        return 1;
    return 0;
}

/* Copia a coluna (1, 2, 3...) da linha atual para destino; NULL no banco vira texto vazio */
void bdLerTexto(int coluna, char destino[], int tamanho)
{
    SQLLEN indicador;

    destino[0] = '\0';
    SQLGetData(bdComando, coluna, SQL_C_CHAR, destino, tamanho, &indicador);
    if (indicador == SQL_NULL_DATA)
        destino[0] = '\0';
}

/* Prepara um texto para o SQL: vazio vira NULL, o resto vira N'texto'.
   O apóstrofo é repetido (D'Avila vira D''Avila) para o SQL Server não achar que o texto acabou */
void sqlTexto(char destino[], char texto[])
{
    int posicaoTexto, posicaoDestino, tamanho = strlen(texto);

    if (tamanho == 0)
    {
        strcpy(destino, "NULL");
        return;
    }

    strcpy(destino, "N'");
    posicaoDestino = 2; /* depois do N' */
    for (posicaoTexto = 0; posicaoTexto < tamanho; posicaoTexto++)
    {
        destino[posicaoDestino] = texto[posicaoTexto];
        posicaoDestino++;
        if (texto[posicaoTexto] == '\'')
        {
            destino[posicaoDestino] = '\''; /* repete o apóstrofo */
            posicaoDestino++;
        }
    }
    destino[posicaoDestino] = '\0';
    strcat(destino, "'");
}

/* Prepara um valor em dinheiro para o SQL: -1 (ainda não informado) vira NULL */
void sqlDinheiro(char destino[], double valor)
{
    if (valor < 0)
        strcpy(destino, "NULL");
    else
        sprintf(destino, "%.2f", valor);
}

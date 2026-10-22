/* Conexão com o SQL Server pela API ODBC (sql.h / sqlext.h).
   Compilar com: gcc main.c -o output/main.exe -lodbc32
   Antes de usar: rodar docs/Modelagem_DB/schema_sqlserver.sql e dados_exemplo_sqlserver.sql */

/* Banco bytetech na instância padrão da máquina, com o login do Windows (sem senha no código).
   Se a máquina só tiver o driver 17, troque "ODBC Driver 18" por "ODBC Driver 17" */
#define CONEXAO_SQLSERVER "DRIVER={ODBC Driver 18 for SQL Server};" \
                          "SERVER=localhost;"                        \
                          "DATABASE=bytetech;"                       \
                          "Trusted_Connection=yes;"                  \
                          "Encrypt=yes;"                             \
                          "TrustServerCertificate=yes;"

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
    SQLSMALLINT i = 1;

    while (SQLGetDiagRec(tipo, handle, i, estado, &erroNativo, mensagem, sizeof(mensagem), &tamanho) == SQL_SUCCESS)
    {
        printf("\n[!] Banco de dados: %s\n", mensagem);
        i++;
    }
}

/* Abre a conexão com o banco; devolve 1 se conectou */
int bdConectar()
{
    SQLRETURN ret;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &bdAmbiente);
    SQLSetEnvAttr(bdAmbiente, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, bdAmbiente, &bdConexao);
    SQLSetConnectAttr(bdConexao, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

    ret = SQLDriverConnect(bdConexao, NULL, (SQLCHAR *)CONEXAO_SQLSERVER, SQL_NTS,
                           NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    if (!SQL_SUCCEEDED(ret))
    {
        bdMostrarErro(SQL_HANDLE_DBC, bdConexao);
        return 0;
    }
    return 1;
}

/* Fecha a conexão e libera os handles */
void bdDesconectar()
{
    if (bdComando != SQL_NULL_HSTMT)
        SQLFreeHandle(SQL_HANDLE_STMT, bdComando);
    SQLDisconnect(bdConexao);
    SQLFreeHandle(SQL_HANDLE_DBC, bdConexao);
    SQLFreeHandle(SQL_HANDLE_ENV, bdAmbiente);
}

/* Roda um comando que não devolve linhas (INSERT, UPDATE, SET); devolve 1 se deu certo */
int bdExecutar(char sql[])
{
    SQLHSTMT comando;
    SQLRETURN ret;

    /* fecha o SELECT anterior: o SQL Server não roda outro comando com um SELECT aberto */
    if (bdComando != SQL_NULL_HSTMT)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, bdComando);
        bdComando = SQL_NULL_HSTMT;
    }
    SQLAllocHandle(SQL_HANDLE_STMT, bdConexao, &comando);
    ret = SQLExecDirect(comando, (SQLCHAR *)sql, SQL_NTS);
    /* SQL_NO_DATA: o comando rodou, mas não mudou nenhuma linha */
    if (!SQL_SUCCEEDED(ret) && ret != SQL_NO_DATA)
    {
        bdMostrarErro(SQL_HANDLE_STMT, comando);
        SQLFreeHandle(SQL_HANDLE_STMT, comando);
        return 0;
    }
    SQLFreeHandle(SQL_HANDLE_STMT, comando);
    return 1;
}

/* Roda um SELECT; as linhas são lidas depois com bdProximaLinha e bdLerTexto */
int bdConsultar(char sql[])
{
    SQLRETURN ret;

    if (bdComando != SQL_NULL_HSTMT)
        SQLFreeHandle(SQL_HANDLE_STMT, bdComando);
    SQLAllocHandle(SQL_HANDLE_STMT, bdConexao, &bdComando);
    ret = SQLExecDirect(bdComando, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        bdMostrarErro(SQL_HANDLE_STMT, bdComando);
        return 0;
    }
    return 1;
}

/* Avança para a próxima linha do SELECT; devolve 0 quando acabou */
int bdProximaLinha()
{
    if (SQL_SUCCEEDED(SQLFetch(bdComando)))
        return 1;
    return 0;
}

/* Copia a coluna (1, 2, 3...) da linha atual para destino. NULL no banco vira texto vazio */
void bdLerTexto(int coluna, char destino[], int tamanho)
{
    SQLLEN indicador;

    destino[0] = '\0';
    SQLGetData(bdComando, coluna, SQL_C_CHAR, destino, tamanho, &indicador);
    if (indicador == SQL_NULL_DATA)
        destino[0] = '\0';
}

/* Prepara um texto para entrar no SQL: vazio vira NULL; o resto vira N'texto'.
   O apóstrofo é escrito duas vezes (D'Avila vira D''Avila): assim o SQL Server
   entende que ele faz parte do texto e não que o texto acabou */
void sqlTexto(char destino[], char texto[])
{
    int i, j, tamanho = strlen(texto);

    if (tamanho == 0)
    {
        strcpy(destino, "NULL");
        return;
    }

    strcpy(destino, "N'");
    j = 2; /* próxima posição livre do destino */
    for (i = 0; i < tamanho; i++)
    {
        destino[j] = texto[i];
        j++;
        if (texto[i] == '\'')
        {
            destino[j] = '\''; /* repete o apóstrofo */
            j++;
        }
    }
    destino[j] = '\0';
    strcat(destino, "'");
}

/* Prepara um valor em dinheiro: -1 (ainda não informado) vira NULL */
void sqlDinheiro(char destino[], double valor)
{
    if (valor < 0)
        strcpy(destino, "NULL");
    else
        sprintf(destino, "%.2f", valor);
}

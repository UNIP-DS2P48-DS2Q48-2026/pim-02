
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "banco.h"

// Exibe os erros retornados pelo ODBC.
static void bdMostrarErro(SQLSMALLINT tipo, SQLHANDLE identificador)
{
    SQLSMALLINT numero = 1;
    SQLCHAR estado[6];
    SQLCHAR mensagem[512];
    SQLINTEGER codigo;
    SQLSMALLINT tamanho;

    while (SQLGetDiagRec(tipo, identificador, numero,
                         estado, &codigo, mensagem,
                         sizeof(mensagem), &tamanho) == SQL_SUCCESS)
    {
        fprintf(stderr, "ODBC [%s] (%ld): %s\n",
                (char *)estado, (long)codigo, (char *)mensagem);
        numero++;
    }
}

// Estabelece conexao com o banco bytetech.
int bdConectar(Banco *banco)
{
    SQLRETURN retorno;
    SQLCHAR saida[1024];
    SQLSMALLINT tamanhoSaida;
    char conexao[1024];
    const char *senha = getenv("BYTETECH_DB_PASSWORD");
    if (senha == NULL || senha[0] == '\0')
        senha = getenv("PIM02_DB_PASSWORD");  // compatibilidade com a configuracao antiga
    const char *servidor = getenv("BYTETECH_DB_SERVER");
    if (servidor == NULL || servidor[0] == '\0')
        servidor = "localhost";
    // Se so tiver o driver 17: BYTETECH_DB_DRIVER=ODBC Driver 17 for SQL Server
    const char *driver = getenv("BYTETECH_DB_DRIVER");
    if (driver == NULL || driver[0] == '\0')
        driver = "ODBC Driver 18 for SQL Server";

    if (banco == NULL)
        return 0;

    banco->henv = SQL_NULL_HENV;
    banco->hdbc = SQL_NULL_HDBC;
    banco->hstmt = SQL_NULL_HSTMT;

    int tamanho;

    if (senha != NULL && senha[0] != '\0')
    {
        // Com senha definida, entra com o usuario sa (Linux/Docker ou login SQL).
        tamanho = snprintf(conexao, sizeof(conexao),
            "DRIVER={%s};"
            "SERVER=%s;DATABASE=bytetech;UID=sa;PWD=%s;"
            "Encrypt=yes;TrustServerCertificate=yes;", driver, servidor, senha);
    }
    else
    {
#ifdef _WIN32
        // Sem senha no Windows, entra com o login do proprio Windows.
        tamanho = snprintf(conexao, sizeof(conexao),
            "DRIVER={%s};"
            "SERVER=%s;DATABASE=bytetech;Trusted_Connection=yes;"
            "Encrypt=yes;TrustServerCertificate=yes;", driver, servidor);
#else
        printf("Defina a variavel BYTETECH_DB_PASSWORD.\n");
        return 0;
#endif
    }

    if (tamanho < 0 || (size_t)tamanho >= sizeof(conexao))
    {
        printf("String de conexao muito longa.\n");
        return 0;
    }

    // Cria o ambiente ODBC.
    retorno = SQLAllocHandle(
        SQL_HANDLE_ENV, SQL_NULL_HANDLE, &banco->henv
    );

    if (!SQL_SUCCEEDED(retorno))
        return 0;

    // Configura a versao do ODBC.
    retorno = SQLSetEnvAttr(
        banco->henv,
        SQL_ATTR_ODBC_VERSION,
        (SQLPOINTER)SQL_OV_ODBC3,
        0
    );

    if (!SQL_SUCCEEDED(retorno))
    {
        bdMostrarErro(SQL_HANDLE_ENV, banco->henv);
        bdDesconectar(banco);
        return 0;
    }

    // Cria o identificador da conexao.
    retorno = SQLAllocHandle(
        SQL_HANDLE_DBC, banco->henv, &banco->hdbc
    );

    if (!SQL_SUCCEEDED(retorno))
    {
        bdDesconectar(banco);
        return 0;
    }

    // Define tempo limite de cinco segundos.
    retorno = SQLSetConnectAttr(
        banco->hdbc, SQL_LOGIN_TIMEOUT,
        (SQLPOINTER)(intptr_t)5, 0
    );

    if (!SQL_SUCCEEDED(retorno))
    {
        bdMostrarErro(SQL_HANDLE_DBC, banco->hdbc);
        bdDesconectar(banco);
        return 0;
    }

    // Estabelece a conexao com SQL Server.
    retorno = SQLDriverConnect(
        banco->hdbc,
        NULL,
        (SQLCHAR *)conexao,
        SQL_NTS,
        saida,
        sizeof(saida),
        &tamanhoSaida,
        SQL_DRIVER_NOPROMPT
    );

    if (!SQL_SUCCEEDED(retorno))
    {
        printf("Erro ao conectar no SQL Server.\n");
        bdMostrarErro(SQL_HANDLE_DBC, banco->hdbc);
        bdDesconectar(banco);
        return 0;
    }

    printf("Conexao com SQL Server estabelecida!\n");
    return 1;
}

// Libera o identificador da consulta.
void bdEncerrarConsulta(Banco *banco)
{
    if (banco != NULL &&
        banco->hstmt != SQL_NULL_HSTMT)
    {
        SQLFreeHandle(SQL_HANDLE_STMT, banco->hstmt);
        banco->hstmt = SQL_NULL_HSTMT;
    }
}

// Encerra a conexao com o SQL Server.
void bdDesconectar(Banco *banco)
{
    if (banco == NULL)
        return;

    bdEncerrarConsulta(banco);

    if (banco->hdbc != SQL_NULL_HDBC)
    {
        SQLDisconnect(banco->hdbc);
        SQLFreeHandle(SQL_HANDLE_DBC, banco->hdbc);
        banco->hdbc = SQL_NULL_HDBC;
    }

    if (banco->henv != SQL_NULL_HENV)
    {
        SQLFreeHandle(SQL_HANDLE_ENV, banco->henv);
        banco->henv = SQL_NULL_HENV;
    }
}

// Executa uma consulta e mantem o resultado aberto.
int bdConsultar(Banco *banco, const char *sql)
{
    SQLRETURN retorno;

    if (banco == NULL || sql == NULL ||
        banco->hdbc == SQL_NULL_HDBC ||
        banco->hstmt != SQL_NULL_HSTMT)
    {
        printf("Conexao invalida ou consulta anterior aberta.\n");
        return 0;
    }

    // Cria o identificador SQL.
    retorno = SQLAllocHandle(
        SQL_HANDLE_STMT, banco->hdbc, &banco->hstmt
    );

    if (!SQL_SUCCEEDED(retorno))
    {
        banco->hstmt = SQL_NULL_HSTMT;
        return 0;
    }

    // Executa o comando SQL.
    retorno = SQLExecDirect(
        banco->hstmt, (SQLCHAR *)sql, SQL_NTS
    );

    if (!SQL_SUCCEEDED(retorno))
    {
        printf("Erro ao executar consulta SQL.\n");
        bdMostrarErro(SQL_HANDLE_STMT, banco->hstmt);
        bdEncerrarConsulta(banco);
        return 0;
    }

    return 1;
}

// Executa um comando e libera o identificador.
int bdExecutar(Banco *banco, const char *sql)
{
    if (!bdConsultar(banco, sql))
        return 0;

    bdEncerrarConsulta(banco);
    return 1;
}

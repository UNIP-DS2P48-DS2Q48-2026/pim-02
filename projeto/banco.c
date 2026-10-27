#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "banco.h"

#define CONNECTION_STRING "DRIVER={ODBC Driver 18 for SQL Server};" \
                          "SERVER=localhost;"                        \
                          "DATABASE=Pim02;"                          \
                          "UID=sa;"                                  \
                          "PWD=8501Math&&;"                          \
                          "Encrypt=yes;"                             \
                          "TrustServerCertificate=yes;"
           
                          

#define RESULT_LEN  256
//commit
//prepara e estabelece a conexão com o banco.
int bdConectar(Banco *banco)
{
	SQLRETURN retcode;
	SQLCHAR outstr[1024];
	SQLSMALLINT outstrlen;
//estamos criando o ambiente ODBC
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &banco->henv);

	if (!SQL_SUCCEEDED(retcode))
	{
		printf("Erro ao criar ambiente ODBC. \n");
		return 0;
	}
//estamos configurando o ambiente ODBC
	retcode = SQLSetEnvAttr(banco->henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

	if (!SQL_SUCCEEDED(retcode))
	{
		printf("Erro ao configurar ambiente ODBC.\n");
		SQLFreeHandle(SQL_HANDLE_ENV, banco->henv);
		banco->henv = SQL_NULL_HENV;
		return 0;
	}
//estamos criando o identificador da conexão do ambiente ODBC		
	retcode = SQLAllocHandle(SQL_HANDLE_DBC, banco->henv, &banco->hdbc);
	if (!SQL_SUCCEEDED(retcode))
	{
		printf("Erro ao criar identificador de conexao.\n");
		SQLFreeHandle(SQL_HANDLE_ENV, banco->henv);
		banco->henv = SQL_NULL_HENV;
		return 0;
	}

//estamos setando o tempo limite de tentativa de conexão com o ODBC
	retcode = SQLSetConnectAttr(banco->hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5,0);
	
	if (!SQL_SUCCEEDED(retcode))
	{
    printf("Erro ao configurar tempo limite de conexao.\n");

    SQLFreeHandle(SQL_HANDLE_DBC, banco->hdbc);
//libera o identificador da conexão.
    banco->hdbc = SQL_NULL_HDBC;
//indica que o identificador nao está mais em uso

    SQLFreeHandle(SQL_HANDLE_ENV, banco->henv);
//libera o ambiente ODBC.
    banco->henv = SQL_NULL_HENV;
//indica que o identificador nao está mais em uso
    return 0;
	}

	retcode = SQLDriverConnect(
		banco->hdbc,
		NULL,
		(SQLCHAR*)CONNECTION_STRING,
		SQL_NTS,
		outstr,
		sizeof(outstr),
		&outstrlen,
		SQL_DRIVER_NOPROMPT
	);
	if (!SQL_SUCCEEDED(retcode))
	{
    printf("Erro ao conectar no SQL Server.\n");

    SQLFreeHandle(SQL_HANDLE_DBC, banco->hdbc);
//libera o identificador da conexão.
    banco->hdbc = SQL_NULL_HDBC;
//indica que o identificador nao está mais em uso

    SQLFreeHandle(SQL_HANDLE_ENV, banco->henv);
//libera o ambiente ODBC.
    banco->henv = SQL_NULL_HENV;
//indica que o identificador nao está mais em uso
    return 0;
	}
	printf("Conexao com SQL Server estabelecida!\n");
return 1;
}
//utiliza a conexão existente para executar um comando SQL.


void bdDesconectar(Banco *banco)
{
if (banco->hstmt != SQL_NULL_HSTMT)
{
    SQLFreeHandle(SQL_HANDLE_STMT, banco->hstmt);
    banco->hstmt = SQL_NULL_HSTMT;
}
//identificador da conexão existe e ainda não foi marcado como nulo?
	if (banco->hdbc != SQL_NULL_HDBC)
	{
		SQLDisconnect(banco->hdbc);
//encerra a conexão
		SQLFreeHandle(SQL_HANDLE_DBC, banco->hdbc);
//liberamos o identificador
		banco->hdbc = SQL_NULL_HDBC;
//marcamos ele como nulo 
	}

	if (banco->henv != SQL_NULL_HENV)
	{
		SQLFreeHandle(SQL_HANDLE_ENV, banco->henv);
//liberamos o identificador
		banco->henv = SQL_NULL_HENV;
//marcamos ele como nulo 
	}

	printf("Conexao encerrada!\n");
}
//encerra a conexão depois que terminarmos.

int bdExecutar(Banco *banco, const char *sql)
{
	SQLRETURN retcode;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, banco->hdbc, &banco->hstmt);
	if (!SQL_SUCCEEDED(retcode))
	{
		printf("Erro ao criar identificador sql. \n");
		return 0;
	}
retcode = SQLExecDirect(banco->hstmt, (SQLCHAR*)sql, SQL_NTS);
if (!SQL_SUCCEEDED(retcode))
{
	printf("Erro ao executar comando SQL. \n");

	SQLFreeHandle(SQL_HANDLE_STMT, banco->hstmt);
	banco->hstmt = SQL_NULL_HSTMT;
	
	return 0;
}


//libera o identificador apos executar o comando;
SQLFreeHandle(SQL_HANDLE_STMT, banco->hstmt);
banco->hstmt = SQL_NULL_HSTMT;

//marca o identificador como nulo.
banco->hstmt = SQL_NULL_HSTMT;

	return 1;
}
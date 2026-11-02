
#ifndef BANCO_H
#define BANCO_H

#ifdef _WIN32
#include <windows.h> /* o sql.h do Windows precisa dele antes */
#endif
#include <sql.h>
#include <sqlext.h>

// Estrutura que guarda os identificadores ODBC.
typedef struct
{
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt;
} Banco;

// Funcoes de conexao.
int bdConectar(Banco *banco);
void bdDesconectar(Banco *banco);

// Funcoes para executar comandos SQL.
int bdExecutar(Banco *banco, const char *sql);
int bdConsultar(Banco *banco, const char *sql);
void bdEncerrarConsulta(Banco *banco);

#endif

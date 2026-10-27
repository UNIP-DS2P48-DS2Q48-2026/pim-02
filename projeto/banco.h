#ifndef BANCO_H
#define BANCO_H

#include <sql.h>
#include <sqlext.h>
//commit
typedef struct
{
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLHSTMT hstmt;
} Banco;

int bdConectar(Banco *banco);

void bdDesconectar(Banco *banco);

int bdExecutar(Banco *banco, const char *sql);

#endif
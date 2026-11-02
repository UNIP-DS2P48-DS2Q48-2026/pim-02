
#include <stdio.h>
#include "banco.h"

int main(void)
{
    // Inicializa a estrutura Banco.
    Banco banco = {
        SQL_NULL_HENV,
        SQL_NULL_HDBC,
        SQL_NULL_HSTMT
    };

    SQLINTEGER quantidade = 0;
    SQLLEN indicador = 0;

    // Tenta estabelecer a conexao.
    if (!bdConectar(&banco))
    {
        printf("Falha na conexao.\n");
        return 1;
    }

    // Consulta a quantidade de usuarios.
    if (bdConsultar(&banco, "SELECT COUNT(*) FROM dbo.usuario"))
    {
        // Busca a linha e recupera a primeira coluna.
        if (SQL_SUCCEEDED(SQLFetch(banco.hstmt)) &&
            SQL_SUCCEEDED(SQLGetData(
                banco.hstmt,
                1,
                SQL_C_SLONG,
                &quantidade,
                sizeof(quantidade),
                &indicador
            )) &&
            indicador != SQL_NULL_DATA)
        {
            printf("Usuarios no Pim02: %d\n", (int)quantidade);
        }
        else
        {
            printf("Erro ao ler resultado da consulta.\n");
        }

        bdEncerrarConsulta(&banco);
    }
    else
    {
        printf("Falha ao consultar a tabela usuario.\n");
    }

    // Encerra a conexao.
    bdDesconectar(&banco);
    printf("Conexao encerrada!\n");

    return 0;
}

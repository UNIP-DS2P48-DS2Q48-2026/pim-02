#include <stdio.h>
#include "banco.h"
//commit
int bdConectar(Banco *banco);
void bdDesconectar(Banco *banco);

int main(void)
{
    Banco banco = {
        SQL_NULL_HENV,
        SQL_NULL_HDBC,
        SQL_NULL_HSTMT    
};

if (banco.hdbc == SQL_NULL_HDBC)
    {
        printf("Estrutura Banco inicializada!\n");
    }

    if (bdConectar(&banco))
    {
        printf("Teste realizado com sucesso!\n");

        bdDesconectar(&banco);
    }
    else
    {
        printf("Falha no teste de conexao.\n");
    }

    return 0;
}
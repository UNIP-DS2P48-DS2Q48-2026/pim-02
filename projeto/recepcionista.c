#include "banco.c"

/* UC08 - Cadastrar Recepcionista: cadastra uma nova recepcionista */
void cadastrarRecepcionista()
{
    cadastrarUsuario("recepcionista", "Recepcionistas > Cadastrar");
}

/* UC09 - Consultar Recepcionista: lista e busca as recepcionistas */
void consultarRecepcionista()
{
    int id;
    scanf("%d", &id);
    consul_user("Recepcionistas", id);
    getchar();

}
/* UC10 - Editar Recepcionista: altera os dados de uma recepcionista */
void editarRecepcionista()
{
    editarUsuario("recepcionista", "Recepcionistas > Editar");
}

/* UC11 - Inativar Recepcionista: inativa uma recepcionista sem OS em andamento */
void inativarRecepcionista()
{
    inativarUsuario("recepcionista", "Recepcionistas > Inativar");
}
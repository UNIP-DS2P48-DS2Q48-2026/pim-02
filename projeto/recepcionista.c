/* UC08 - Cadastrar Recepcionista: cadastra uma nova recepcionista */
void cadastrarRecepcionista()
{
    /* cadastrarUsuario está em geral.c, linha 332 */
    cadastrarUsuario("recepcionista", "Recepcionistas > Cadastrar");
}

/* UC09 - Consultar Recepcionista: lista e busca as recepcionistas */
void consultarRecepcionista()
{
    /* consultarUsuarios está em geral.c, linha 411 */
    consultarUsuarios("recepcionista", "Recepcionistas > Consultar");
}

/* UC10 - Editar Recepcionista: altera os dados de uma recepcionista */
void editarRecepcionista()
{
    /* editarUsuario está em geral.c, linha 434 */
    editarUsuario("recepcionista", "Recepcionistas > Editar");
}

/* UC11 - Inativar Recepcionista: inativa uma recepcionista sem OS em andamento */
void inativarRecepcionista()
{
    /* inativarUsuario está em geral.c, linha 494 */
    inativarUsuario("recepcionista", "Recepcionistas > Inativar");
}

/* UC46 - Reativar Recepcionista: reativa uma recepcionista inativa */
void reativarRecepcionista()
{
    /* reativarUsuario está em geral.c, linha 536 */
    reativarUsuario("recepcionista", "Recepcionistas > Reativar");
}

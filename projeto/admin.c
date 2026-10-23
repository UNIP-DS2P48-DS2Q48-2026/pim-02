/* admin.c - casos de uso dos administradores; o trabalho é feito pelas funções de usuário do geral.c */

/* UC04 - Cadastrar Admin: cadastra um novo administrador */
void cadastrarAdmin()
{
    /* cadastrarUsuario está em geral.c, linha 332 */
    cadastrarUsuario("admin", "Administradores > Cadastrar");
}

/* UC05 - Consultar Admin: lista e busca os administradores */
void consultarAdmin()
{
    /* consultarUsuarios está em geral.c, linha 411 */
    consultarUsuarios("admin", "Administradores > Consultar");
}

/* UC06 - Editar Admin: altera os dados de um administrador */
void editarAdmin()
{
    /* editarUsuario está em geral.c, linha 434 */
    editarUsuario("admin", "Administradores > Editar");
}

/* UC07 - Inativar Admin: inativa um administrador (nunca a si mesmo) */
void inativarAdmin()
{
    /* inativarUsuario está em geral.c, linha 494 */
    inativarUsuario("admin", "Administradores > Inativar");
}

/* UC45 - Reativar Admin: reativa um administrador inativo */
void reativarAdmin()
{
    /* reativarUsuario está em geral.c, linha 536 */
    reativarUsuario("admin", "Administradores > Reativar");
}

/* UC04 - Cadastrar Admin: cadastra um novo administrador */
void cadastrarAdmin()
{
    cadastrarUsuario("admin", "Administradores > Cadastrar");
}

/* UC05 - Consultar Admin: lista e busca os administradores */
void consultarAdmin()
{
    consultarUsuarios("admin", "Administradores > Consultar");
}

/* UC06 - Editar Admin: altera os dados de um administrador */
void editarAdmin()
{
    editarUsuario("admin", "Administradores > Editar");
}

/* UC07 - Inativar Admin: inativa um administrador (nunca a si mesmo) */
void inativarAdmin()
{
    inativarUsuario("admin", "Administradores > Inativar");
}

/* UC45 - Reativar Admin: reativa um administrador inativo */
void reativarAdmin()
{
    reativarUsuario("admin", "Administradores > Reativar");
}

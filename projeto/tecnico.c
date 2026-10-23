/* UC12 - Cadastrar Tecnico: cadastra um novo tecnico com a especialidade */
void cadastrarTecnico()
{
    /* cadastrarUsuario está em geral.c, linha 332 */
    cadastrarUsuario("tecnico", "Tecnicos > Cadastrar");
}

/* UC13 - Consultar Tecnico: lista e busca os tecnicos */
void consultarTecnico()
{
    /* consultarUsuarios está em geral.c, linha 411 */
    consultarUsuarios("tecnico", "Tecnicos > Consultar");
}

/* UC14 - Editar Tecnico: altera os dados de um tecnico */
void editarTecnico()
{
    /* editarUsuario está em geral.c, linha 434 */
    editarUsuario("tecnico", "Tecnicos > Editar");
}

/* UC15 - Inativar Tecnico: inativa um tecnico sem OS em andamento */
void inativarTecnico()
{
    /* inativarUsuario está em geral.c, linha 494 */
    inativarUsuario("tecnico", "Tecnicos > Inativar");
}

/* UC47 - Reativar Tecnico: reativa um tecnico inativo */
void reativarTecnico()
{
    /* reativarUsuario está em geral.c, linha 536 */
    reativarUsuario("tecnico", "Tecnicos > Reativar");
}

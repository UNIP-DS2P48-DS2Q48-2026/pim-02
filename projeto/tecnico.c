/* UC12 - Cadastrar Tecnico: cadastra um novo tecnico com a especialidade */
void cadastrarTecnico(void)
{
    cadastrarUsuario("tecnico");
}

/* UC13 - Consultar Tecnico: lista e busca os tecnicos */
void consultarTecnico(void)
{
    consultarUsuarios("tecnico");
}

/* UC14 - Editar Tecnico: altera os dados de um tecnico */
void editarTecnico(void)
{
    editarUsuario("tecnico");
}

/* UC15 - Inativar Tecnico: inativa um tecnico sem OS em andamento */
void inativarTecnico(void)
{
    inativarUsuario("tecnico");
}

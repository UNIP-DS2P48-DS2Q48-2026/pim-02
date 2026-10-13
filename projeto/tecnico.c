/* UC12 - Cadastrar Tecnico: cadastra um novo tecnico com a especialidade */
void cadastrarTecnico()
{
    cadastrarUsuario("tecnico", "Tecnicos > Cadastrar");
}

/* UC13 - Consultar Tecnico: lista e busca os tecnicos */
void consultarTecnico()
{
    consultarUsuarios("tecnico", "Tecnicos > Consultar");
}

/* UC14 - Editar Tecnico: altera os dados de um tecnico */
void editarTecnico()
{
    editarUsuario("tecnico", "Tecnicos > Editar");
}

/* UC15 - Inativar Tecnico: inativa um tecnico sem OS em andamento */
void inativarTecnico()
{
    inativarUsuario("tecnico", "Tecnicos > Inativar");
}

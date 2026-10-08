/* UC16 - Cadastrar Cliente: cadastra um novo cliente com CPF e telefone */
void cadastrarCliente()
{
    cadastrarUsuario("cliente", "Clientes > Cadastrar");
}

/* UC17 - Consultar Cliente: lista e busca os clientes por nome, CPF ou e-mail */
void consultarCliente()
{
    consultarUsuarios("cliente", "Clientes > Consultar");
}

/* UC18 - Editar Cliente: altera os dados de um cliente */
void editarCliente()
{
    editarUsuario("cliente", "Clientes > Editar");
}

/* UC19 - Inativar Cliente: inativa um cliente sem OS em andamento */
void inativarCliente()
{
    inativarUsuario("cliente", "Clientes > Inativar");
}

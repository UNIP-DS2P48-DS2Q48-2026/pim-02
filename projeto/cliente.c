/* UC16 - Cadastrar Cliente: cadastra um novo cliente com CPF e telefone */
void cadastrarCliente(void)
{
    cadastrarUsuario("cliente");
}

/* UC17 - Consultar Cliente: lista e busca os clientes por nome, CPF ou e-mail */
void consultarCliente(void)
{
    consultarUsuarios("cliente");
}

/* UC18 - Editar Cliente: altera os dados de um cliente */
void editarCliente(void)
{
    editarUsuario("cliente");
}

/* UC19 - Inativar Cliente: inativa um cliente sem OS em andamento */
void inativarCliente(void)
{
    inativarUsuario("cliente");
}

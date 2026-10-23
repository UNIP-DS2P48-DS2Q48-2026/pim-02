/* cliente.c - casos de uso dos clientes; o trabalho é feito pelas funções de usuário do geral.c */

/* UC16 - Cadastrar Cliente: cadastra um novo cliente com CPF e telefone */
void cadastrarCliente()
{
    /* cadastrarUsuario está em geral.c, linha 332 */
    cadastrarUsuario("cliente", "Clientes > Cadastrar");
}

/* UC17 - Consultar Cliente: lista e busca os clientes por nome, CPF ou e-mail */
void consultarCliente()
{
    /* consultarUsuarios está em geral.c, linha 411 */
    consultarUsuarios("cliente", "Clientes > Consultar");
}

/* UC18 - Editar Cliente: altera os dados de um cliente */
void editarCliente()
{
    /* editarUsuario está em geral.c, linha 434 */
    editarUsuario("cliente", "Clientes > Editar");
}

/* UC19 - Inativar Cliente: inativa um cliente sem OS em andamento */
void inativarCliente()
{
    /* inativarUsuario está em geral.c, linha 494 */
    inativarUsuario("cliente", "Clientes > Inativar");
}

/* UC48 - Reativar Cliente: reativa um cliente inativo */
void reativarCliente()
{
    /* reativarUsuario está em geral.c, linha 536 */
    reativarUsuario("cliente", "Clientes > Reativar");
}

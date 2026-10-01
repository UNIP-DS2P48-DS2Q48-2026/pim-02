/* UC01 - Realizar Login: autentica por e-mail e senha e abre o menu do perfil */
int realizarLogin() {
    char email[50];
    char senha[50];
    int i;
    
    /* --- PASSO 1: ECRÃ DO E-MAIL --- */
    imprimirCabecalho("Entrar");
    
    /* Espaçamento para colocar o input na zona inferior, como na imagem */
    for(i = 0; i < 15; i++) {
        printf("\n");
    }
    
    printf("E-mail: ");
    scanf("%49s", email);
    limparBuffer();
    
    /* --- PASSO 2: ECRÃ DA SENHA --- */
    imprimirCabecalho("Entrar");
    
    /* O e-mail fica agora fixo no topo, logo abaixo do cabeçalho */
    printf("\nE-mail: %s\n", email);
    
    /* Espaçamento para o campo de senha na zona inferior */
    for(i = 0; i < 13; i++) {
        printf("\n");
    }
    
    printf("Senha: ");
    scanf("%49s", senha);
    limparBuffer();
    
    /* --- VALIDAÇÃO --- */
    for (i = 0; i < NUM_USUARIOS; i++) {
        if (strcmp(email, usuarios[i].email) == 0 && strcmp(senha, usuarios[i].senha) == 0) {
            return i; /* Retorna o índice do utilizador autenticado */
        }
    }
    return -1; /* Retorna -1 se as credenciais falharem */
}

/* UC02 - Fazer Logout: encerra a sessao e volta para a tela inicial */
void fazerLogout(void) {
    printf("\nA terminar a sessao...\n");
}

/* UC03 - Consultar Meus Dados: mostra o cadastro do usuario logado */
void consultarMeusDados(void) {
}

/* UC44 - Alterar Senha: troca a propria senha, confirmando a senha atual */
void alterarSenha(void) {
}

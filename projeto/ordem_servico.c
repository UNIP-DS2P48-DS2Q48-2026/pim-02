/* Persistencia de OS e atribuicoes ocorre no SQL Server. */
int salvarOrdens(void) { return salvarOrdensSQL(); }
void carregarOrdens(void) { if (!carregarOrdensSQL()) fprintf(stderr,"Falha ao carregar OS.\n"); }
int salvarTecnicosOS(void) { return salvarTecnicosOSSQL(); }
void carregarTecnicosOS(void) { if (!carregarTecnicosOSSQL()) fprintf(stderr,"Falha ao carregar atribuicoes.\n"); }

/* UC24-UC30: implementacao dos fluxos de OS. */
static int osSelecionarVisivel(const char *titulo)
{
    int i,id,mostrados=0;char dono[60],equip[90];
    imprimirCabecalho((char *)titulo);
    for (i=0;i<totalOrdens;i++) {
        if (!podeVerOS(i)) continue;
        nomeDoCliente(donoDoEquipamento(ordens[i].id_equipamento),dono);
        nomeDoEquipamento(ordens[i].id_equipamento,equip);
        printf("  OS %d | %s | %-14.14s | %-20.20s | %s\n",ordens[i].id_os,
               ordens[i].data_abertura,dono,equip,ordens[i].status);
        mostrados++;
    }
    if (!mostrados) {printf("\nNenhuma OS disponivel.\n");pausar();return -1;}
    id=lerInteiro("\nNumero da OS (0 para voltar): ");
    if (id<=0) return -1;
    i=buscarOSPorId(id);
    if (i<0 || !podeVerOS(i)) {printf("\n[!] OS nao encontrada ou sem permissao.\n");pausar();return -1;}
    return i;
}

void criarOrdemServico(void)
{
    int e,n,maior=0,j,cliente,id,mostrados=0;char problema[200],cpf[20];
    imprimirCabecalho("Ordens de Servico > Criar");
    if (!perfilLogado("admin") && !perfilLogado("recepcionista")) {printf("Sem permissao.\n");pausar();return;}
    if (totalOrdens>=MAX_OS) {printf("Limite de OS atingido.\n");pausar();return;}
    /* UC24: o atendente informa o CPF e escolhe um equipamento ativo desse cliente */
    lerTexto("\nCPF do cliente, como foi cadastrado (ENTER para voltar): ",cpf,sizeof(cpf));
    if (!*cpf) return;
    cliente=-1;
    for (j=0;j<totalUsuarios;j++)
        if (!strcmp(usuarios[j].perfil,"cliente") && !strcmp(usuarios[j].cpf,cpf)) cliente=j;
    if (cliente<0) {
        printf("\n[!] Nenhum cliente com o CPF %s. Cadastre o cliente e o equipamento antes de abrir a OS.\n",cpf);
        pausar();return;
    }
    if (strcmp(usuarios[cliente].status,"ativo")!=0) {
        printf("\n[!] O cliente %s esta inativo. Nao e possivel abrir OS.\n",usuarios[cliente].nome);
        pausar();return;
    }
    printf("\nCliente: %s\n",usuarios[cliente].nome);
    for (j=0;j<totalEquipamentos;j++) {
        if (equipamentos[j].id_cliente!=usuarios[cliente].id_usuario || strcmp(equipamentos[j].status,"ativo")) continue;
        printf("ID %d - %s %s - %s\n",equipamentos[j].id_equipamento,equipamentos[j].tipo,
               equipamentos[j].marca,equipamentos[j].modelo_numero_serie);
        mostrados++;
    }
    if (!mostrados) {
        printf("\n[!] O cliente nao tem equipamento ativo. Cadastre em Equipamentos > Cadastrar.\n");
        pausar();return;
    }
    id=lerInteiro("\nID do equipamento (0 para voltar): ");
    if (id<=0) return;
    e=buscarEquipamentoPorId(id);
    if (e<0 || equipamentos[e].id_cliente!=usuarios[cliente].id_usuario || strcmp(equipamentos[e].status,"ativo")) {
        printf("\n[!] ID %d nao esta na lista.\n",id);pausar();return;
    }
    lerTexto("Problema relatado: ",problema,sizeof(problema));
    if (!campoPreenchido(problema,"Problema"))return;
    for (j=0;j<totalOrdens;j++)if(ordens[j].id_os>maior)maior=ordens[j].id_os;
    n=totalOrdens;memset(&ordens[n],0,sizeof(ordens[n]));
    ordens[n].id_os=maior+1;
    ordens[n].id_equipamento=equipamentos[e].id_equipamento;
    ordens[n].id_atendente=usuarios[usuarioLogado].id_usuario;
    strcpy(ordens[n].problema_relatado,problema);
    strcpy(ordens[n].status,"aberta");
    ordens[n].orcamento=-1;ordens[n].valor_pago=-1;
    dataHoje(ordens[n].data_abertura);
    dataHoraAgora(ordens[n].criado_em);
    strcpy(ordens[n].atualizado_em,ordens[n].criado_em);
    totalOrdens++;
    if (!salvarOrdens()) {printf("\n[!] Nao foi possivel salvar a OS.\n");pausar();return;}
    printf("\n[+] OS %d criada! Agora atribua um tecnico em Ordens de Servico > Atribuir Tecnico.\n",ordens[n].id_os);pausar();
}

void consultarOS(void)
{
    int i=osSelecionarVisivel("Ordens de Servico > Consultar");
    if (i<0)return;
    OrdemServico *o=&ordens[i];
    printf("\nOS: %d | Status: %s\nAbertura: %s\nProblema: %s\nDiagnostico: %s\n",
        o->id_os,o->status,o->data_abertura,o->problema_relatado,
        *o->diagnostico?o->diagnostico:"(pendente)");
    if(o->orcamento>=0)printf("Orcamento: R$ %.2f | Resposta: %s\n",o->orcamento,
        *o->resultado_resposta_cliente?o->resultado_resposta_cliente:"aguardando");
    if (*o->resultado_teste)printf("Teste: %s\n",o->resultado_teste);
    if (*o->data_entrega)printf("Entrega: %s | Pago: R$ %.2f\n",o->data_entrega,o->valor_pago);
    if (*o->feedback_cliente)printf("Feedback: %s | Satisfeito: %s\n",o->feedback_cliente,o->satisfeito);
    pausar();
}

void editarOS(void)
{
    int i;char texto[200];
    if(!perfilLogado("admin")&&!perfilLogado("recepcionista")){printf("Sem permissao.\n");pausar();return;}
    i=osSelecionarVisivel("Ordens de Servico > Editar");if(i<0)return;
    if(strcmp(ordens[i].status,"aberta")!=0) {
        printf("[!] So e possivel editar o problema de OS aberta.\n");pausar();return;
    }
    lerTexto("Novo problema (ENTER para voltar): ",texto,sizeof(texto));if(!*texto)return;
    strcpy(ordens[i].problema_relatado,texto);
    if(!salvarOrdens()){printf("Falha ao editar OS.\n");pausar();return;}
    printf("[+] OS atualizada.\n");pausar();
}

void cancelarOS(void)
{
    int i;
    if(!perfilLogado("admin")&&!perfilLogado("recepcionista")){printf("Sem permissao.\n");pausar();return;}
    i=osSelecionarVisivel("Ordens de Servico > Cancelar");if(i<0)return;
    if(!strcmp(ordens[i].status,"finalizada")||!strcmp(ordens[i].status,"cancelada")) {
        printf("[!] OS encerrada nao pode ser cancelada.\n");pausar();return;
    }
    if(!confirmar("Confirmar cancelamento da OS?"))return;
    strcpy(ordens[i].status,"cancelada");
    if(!salvarOrdens()){printf("Falha ao cancelar.\n");pausar();return;}
    printf("[+] OS cancelada; historico preservado.\n");pausar();
}

void atribuirTecnico(void)
{
    int i,t;char idOS[24],idTec[24];
    if(!perfilLogado("admin")&&!perfilLogado("recepcionista")){printf("Sem permissao.\n");pausar();return;}
    i=osSelecionarVisivel("Ordens de Servico > Atribuir Tecnico");if(i<0)return;
    if(strcmp(ordens[i].status,"aberta")&&strcmp(ordens[i].status,"em_andamento")) {
        printf("[!] OS encerrada nao aceita tecnico.\n");pausar();return;
    }
    t=selecionarUsuario("tecnico",1);if(t<0)return;
    if(tecnicoEstaNaOS(usuarios[t].id_usuario,ordens[i].id_os)) {
        printf("[!] Tecnico ja atribuido a OS.\n");pausar();return;
    }
    dadosNumero(ordens[i].id_os,idOS,sizeof(idOS));
    dadosNumero(usuarios[t].id_usuario,idTec,sizeof(idTec));
    /* SQL Server gera a identidade. O BEGIN TRAN mantem vinculo e status juntos. */
    const char *sql=
      "SET XACT_ABORT ON; BEGIN TRY BEGIN TRAN; "
      "IF NOT EXISTS (SELECT 1 FROM dbo.os WHERE id_os=? AND status IN ('aberta','em_andamento')) "
      "THROW 50001,'OS indisponivel',1; "
      "IF EXISTS(SELECT 1 FROM dbo.tecnico_os WHERE id_os=? AND id_tecnico=?) "
      "THROW 50002,'Tecnico ja vinculado',1; "
      "INSERT dbo.tecnico_os(id_os,id_tecnico,data_atribuicao) VALUES(?,?,CONVERT(date,GETDATE())); "
      "UPDATE dbo.os SET status='em_andamento',atualizado_em=SYSDATETIME() WHERE id_os=?; "
      "COMMIT; END TRY BEGIN CATCH IF @@TRANCOUNT>0 ROLLBACK; THROW; END CATCH";
    const char *p[]={idOS,idOS,idTec,idOS,idTec,idOS};
    if(!dadosExecutar(sql,6,p,NULL)) {printf("Falha ao atribuir tecnico.\n");pausar();return;}
    if(!carregarOrdensSQL() || !carregarTecnicosOSSQL()) {printf("Erro ao atualizar dados da memoria.\n");pausar();return;}
    printf("[+] Tecnico atribuido e OS em andamento.\n");pausar();
}

void registrarEntrega(void)
{
    int i,confirmado;char texto[32];double pago;
    if(!perfilLogado("admin")&&!perfilLogado("recepcionista")){printf("Sem permissao.\n");pausar();return;}
    i=osSelecionarVisivel("Ordens de Servico > Registrar Entrega");if(i<0)return;
    if(strcmp(ordens[i].status,"em_andamento") ||
       strcmp(ordens[i].resultado_teste,"aprovado") ||
       strcmp(ordens[i].resultado_resposta_cliente,"aprovado") ||
       *ordens[i].data_entrega) {
        printf("[!] A OS %d nao esta pronta para entrega.\n",ordens[i].id_os);pausar();return;
    }
    printf("\nOrcamento aprovado: R$ %.2f\n",ordens[i].orcamento);
    /* UC29 7a: valor diferente do orcamento precisa ser confirmado ou digitado de novo */
    do {
        lerTexto("Valor recebido em R$ (ENTER para voltar): ",texto,sizeof(texto));
        if(!*texto) return;
        pago=textoParaValor(texto);
        if(pago<0 || pago>99999999) {printf("Valor invalido.\n");pausar();return;}
        confirmado=1;
        if(pago!=ordens[i].orcamento) {
            confirmado=confirmar("\n[!] Valor diferente do orcamento. Confirmar mesmo assim?");
            if(!confirmado) printf("Digite o valor correto.\n");
        }
    } while(!confirmado);
    /* A OS continua em andamento: so e finalizada pelo feedback do cliente (UC30) */
    ordens[i].valor_pago=pago;
    dataHoje(ordens[i].data_entrega);
    if(!salvarOrdens()){printf("Falha ao registrar entrega.\n");pausar();return;}
    printf("[+] Entrega registrada! A OS %d aguarda o feedback do cliente.\n",ordens[i].id_os);pausar();
}

void registrarFeedback(void)
{
    int i,resposta;char feedback[200];
    i=osSelecionarVisivel("Ordens de Servico > Registrar Feedback");if(i<0)return;
    if(!*ordens[i].data_entrega) {
        printf("[!] A OS %d ainda nao foi entregue.\n",ordens[i].id_os);pausar();return;
    }
    if(strcmp(ordens[i].status,"em_andamento")) {
        printf("[!] A OS %d ja foi encerrada.\n",ordens[i].id_os);pausar();return;
    }
    lerTexto("Comentario do cliente (opcional): ",feedback,sizeof(feedback));
    resposta=lerInteiro("Satisfeito? 1-Sim, 2-Nao: ");
    if(resposta!=1 && resposta!=2) {printf("Opcao invalida.\n");pausar();return;}
    strcpy(ordens[i].feedback_cliente,feedback);
    if(resposta==1) {
        strcpy(ordens[i].satisfeito,"sim");
        strcpy(ordens[i].status,"finalizada");
    } else {
        /* UC30 5a: limpa teste, entrega e pagamento para refazer o reparo */
        strcpy(ordens[i].satisfeito,"nao");
        ordens[i].resultado_teste[0]='\0';
        ordens[i].data_entrega[0]='\0';
        ordens[i].valor_pago=-1;
    }
    if(!salvarOrdens()){printf("Falha ao registrar feedback.\n");pausar();return;}
    if(resposta==1) printf("[+] Feedback registrado. A OS %d foi finalizada!\n",ordens[i].id_os);
    else printf("[+] Feedback registrado. A OS %d voltou para Executar Manutencao.\n",ordens[i].id_os);
    pausar();
}

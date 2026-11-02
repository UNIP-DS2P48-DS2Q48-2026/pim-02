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
    int e,n,maior=0,j;char problema[200];
    imprimirCabecalho("Ordens de Servico > Criar");
    if (!perfilLogado("admin") && !perfilLogado("recepcionista")) {printf("Sem permissao.\n");pausar();return;}
    if (totalOrdens>=MAX_OS) {printf("Limite de OS atingido.\n");pausar();return;}
    e=selecionarEquipamento(1);
    if (e<0) return;
    j=buscarUsuarioPorId(equipamentos[e].id_cliente);
    if (j<0 || strcmp(usuarios[j].status,"ativo")!=0) {
        printf("\n[!] Cliente inativo ou nao encontrado.\n");pausar();return;
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
    printf("\n[+] Ordem de servico %d criada.\n",ordens[n].id_os);pausar();
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
    int i;char texto[32];double pago;
    if(!perfilLogado("admin")&&!perfilLogado("recepcionista")){printf("Sem permissao.\n");pausar();return;}
    i=osSelecionarVisivel("Ordens de Servico > Registrar Entrega");if(i<0)return;
    if(strcmp(ordens[i].status,"em_andamento") ||
       strcmp(ordens[i].resultado_teste,"aprovado") ||
       strcmp(ordens[i].resultado_resposta_cliente,"aprovado")) {
        printf("[!] Entrega exige orcamento aprovado, manutencao e teste aprovado.\n");pausar();return;
    }
    lerTexto("Valor recebido (R$): ",texto,sizeof(texto));
    pago=textoParaValor(texto);
    if(!*texto || pago<0 || pago>99999999) {printf("Valor invalido.\n");pausar();return;}
    if(!confirmar("Confirmar entrega e pagamento?"))return;
    ordens[i].valor_pago=pago;
    dataHoje(ordens[i].data_entrega);
    strcpy(ordens[i].status,"finalizada");
    if(!salvarOrdens()){printf("Falha ao registrar entrega.\n");pausar();return;}
    printf("[+] Entrega e pagamento registrados.\n");pausar();
}

void registrarFeedback(void)
{
    int i,resposta;char feedback[200];
    i=osSelecionarVisivel("Ordens de Servico > Registrar Feedback");if(i<0)return;
    if(strcmp(ordens[i].status,"finalizada") || !*ordens[i].data_entrega) {
        printf("[!] Feedback so e registrado apos a entrega.\n");pausar();return;
    }
    lerTexto("Comentario (ENTER para voltar): ",feedback,sizeof(feedback));if(!*feedback)return;
    resposta=lerInteiro("Satisfeito? 1-Sim, 2-Nao: ");
    if(resposta!=1 && resposta!=2) {printf("Opcao invalida.\n");pausar();return;}
    strcpy(ordens[i].feedback_cliente,feedback);
    strcpy(ordens[i].satisfeito,resposta==1?"sim":"nao");
    if(!salvarOrdens()){printf("Falha ao registrar feedback.\n");pausar();return;}
    printf("[+] Feedback registrado.\n");pausar();
}

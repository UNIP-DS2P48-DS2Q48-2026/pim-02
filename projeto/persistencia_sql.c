/* Persistencia SQL: tabelas equipamento, os e tecnico_os.
   Os vetores continuam sendo usados pelos menus, mas o SQL Server e a fonte dos dados. */

/* Executa comandos parametrizados. NULL em parametros representa SQL NULL.
   Devolve 1 em sucesso; o numero de linhas alteradas sai em afetadas, quando informado. */
static int dadosExecutar(const char *sql, int numero, const char *const valores[], SQLLEN *afetadas)
{
    SQLHSTMT stmt = SQL_NULL_HSTMT;
    SQLRETURN r;
    SQLLEN indicadores[24];
    int i, ok = 0;
    if (!bancoAtivo || bancoAtivo->hstmt != SQL_NULL_HSTMT || numero > 24) return 0;
    r = SQLAllocHandle(SQL_HANDLE_STMT, bancoAtivo->hdbc, &stmt);
    if (!SQL_SUCCEEDED(r)) return 0;
    /* UPDATE deve sempre informar o numero de linhas alteradas. */
    r = SQLExecDirect(stmt, (SQLCHAR *)"SET NOCOUNT OFF", SQL_NTS);
    if (!SQL_SUCCEEDED(r)) goto fim;
    SQLFreeStmt(stmt, SQL_CLOSE);
    for (i = 0; i < numero; i++) {
        indicadores[i] = valores[i] ? SQL_NTS : SQL_NULL_DATA;
        r = SQLBindParameter(stmt, (SQLUSMALLINT)(i + 1), SQL_PARAM_INPUT,
             SQL_C_CHAR, SQL_VARCHAR, 4000, 0, (SQLPOINTER)valores[i],
             valores[i] ? (SQLLEN)(strlen(valores[i]) + 1) : 0, &indicadores[i]);
        if (!SQL_SUCCEEDED(r)) goto fim;
    }
    /* ODBC aceita parametros vinculados com SQLExecDirect. Assim SET
       IDENTITY_INSERT nao fica restrito ao contexto de um comando preparado. */
    r = SQLExecDirect(stmt, (SQLCHAR *)sql, SQL_NTS);
    if (!SQL_SUCCEEDED(r) && !(r == SQL_NO_DATA && afetadas)) goto fim;
    if (afetadas) {
        *afetadas = -1;
        SQLRowCount(stmt, afetadas);
    }
    ok = 1;
fim:
    if (!ok) {
        SQLCHAR estado[6], mensagem[512]; SQLINTEGER codigo; SQLSMALLINT tam;
        if (SQL_SUCCEEDED(SQLGetDiagRec(SQL_HANDLE_STMT, stmt, 1, estado,
                                       &codigo, mensagem, sizeof(mensagem), &tam)))
            fprintf(stderr, "SQLSTATE %s: %s\n", (char *)estado, (char *)mensagem);
        else fprintf(stderr, "Erro SQL ao executar comando.\n");
    }
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    return ok;
}

static int dadosTransacaoInicio(void)
{
    return bancoAtivo && SQL_SUCCEEDED(SQLSetConnectAttr(bancoAtivo->hdbc,
          SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_OFF, 0));
}
static int dadosTransacaoFim(int sucesso)
{
    SQLRETURN r = SQLEndTran(SQL_HANDLE_DBC, bancoAtivo->hdbc,
                              sucesso ? SQL_COMMIT : SQL_ROLLBACK);
    SQLRETURN a = SQLSetConnectAttr(bancoAtivo->hdbc, SQL_ATTR_AUTOCOMMIT,
                                    (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
    return sucesso && SQL_SUCCEEDED(r) && SQL_SUCCEEDED(a);
}
static int dadosCampo(SQLHSTMT stmt, int coluna, char *destino, size_t capacidade)
{
    SQLLEN indicador = 0;
    SQLRETURN r;
    destino[0] = '\0';
    r = SQLGetData(stmt, (SQLUSMALLINT)coluna, SQL_C_CHAR, destino,
                   (SQLLEN)capacidade, &indicador);
    if (indicador == SQL_NULL_DATA) { destino[0] = '\0'; return SQL_SUCCEEDED(r); }
    /* Um dado maior que a struct nao pode ser truncado silenciosamente. */
    return r == SQL_SUCCESS;
}
static void dadosNumero(int valor, char *destino, size_t tam) {
    snprintf(destino, tam, "%d", valor);
}
static void dadosValor(double valor, char *destino, size_t tam) {
    snprintf(destino, tam, "%.2f", valor);
}
static const char *dadosNuloSeVazio(const char *s) { return s && *s ? s : NULL; }

int carregarEquipamentosSQL(void)
{
    SQLRETURN r;
    const char *sql = "SELECT CONVERT(varchar(20),id_equipamento), CONVERT(varchar(20),id_cliente), "
      "categoria, ISNULL(tipo,''), ISNULL(marca,''), ISNULL(modelo_numero_serie,''), status, "
      "CONVERT(varchar(19),criado_em,120), CONVERT(varchar(19),atualizado_em,120) "
      "FROM dbo.equipamento ORDER BY id_equipamento";
    totalEquipamentos = 0;
    if (!bdConsultar(bancoAtivo, sql)) return 0;
    while ((r = SQLFetch(bancoAtivo->hstmt)) != SQL_NO_DATA) {
        char id[32], cliente[32]; Equipamento *e;
        if (!SQL_SUCCEEDED(r) || totalEquipamentos >= MAX_EQUIPAMENTOS) goto erro;
        e = &equipamentos[totalEquipamentos]; memset(e, 0, sizeof(*e));
        if (!dadosCampo(bancoAtivo->hstmt,1,id,sizeof(id)) ||
            !dadosCampo(bancoAtivo->hstmt,2,cliente,sizeof(cliente)) ||
            !dadosCampo(bancoAtivo->hstmt,3,e->categoria,sizeof(e->categoria)) ||
            !dadosCampo(bancoAtivo->hstmt,4,e->tipo,sizeof(e->tipo)) ||
            !dadosCampo(bancoAtivo->hstmt,5,e->marca,sizeof(e->marca)) ||
            !dadosCampo(bancoAtivo->hstmt,6,e->modelo_numero_serie,sizeof(e->modelo_numero_serie)) ||
            !dadosCampo(bancoAtivo->hstmt,7,e->status,sizeof(e->status)) ||
            !dadosCampo(bancoAtivo->hstmt,8,e->criado_em,sizeof(e->criado_em)) ||
            !dadosCampo(bancoAtivo->hstmt,9,e->atualizado_em,sizeof(e->atualizado_em))) goto erro;
        e->id_equipamento = atoi(id); e->id_cliente = atoi(cliente);
        totalEquipamentos++;
    }
    bdEncerrarConsulta(bancoAtivo); return 1;
erro:
    fprintf(stderr,"Falha ao carregar equipamentos (limite ou tamanho de campo).\n");
    bdEncerrarConsulta(bancoAtivo); totalEquipamentos = 0; return 0;
}

int carregarOrdensSQL(void)
{
    SQLRETURN r;
    const char *sql = "SELECT CONVERT(varchar(20),id_os),CONVERT(varchar(20),id_equipamento),"
      "CONVERT(varchar(20),id_atendente),CONVERT(varchar(10),data_abertura,23),"
      "ISNULL(problema_relatado,''),ISNULL(diagnostico,''),CONVERT(varchar(40),orcamento),"
      "status,ISNULL(resultado_resposta_cliente,''),ISNULL(CONVERT(varchar(10),data_entrega,23),''),"
      "ISNULL(resultado_teste,''),CONVERT(varchar(40),valor_pago),ISNULL(feedback_cliente,''),"
      "ISNULL(satisfeito,''),CONVERT(varchar(19),criado_em,120),CONVERT(varchar(19),atualizado_em,120) "
      "FROM dbo.os ORDER BY id_os";
    totalOrdens = 0;
    if (!bdConsultar(bancoAtivo, sql)) return 0;
    while ((r = SQLFetch(bancoAtivo->hstmt)) != SQL_NO_DATA) {
        char a[32],b[32],c[32],orc[48],pag[48]; OrdemServico *o;
        if (!SQL_SUCCEEDED(r) || totalOrdens >= MAX_OS) goto erro;
        o=&ordens[totalOrdens]; memset(o,0,sizeof(*o));
        if (!dadosCampo(bancoAtivo->hstmt,1,a,sizeof(a)) ||
            !dadosCampo(bancoAtivo->hstmt,2,b,sizeof(b)) ||
            !dadosCampo(bancoAtivo->hstmt,3,c,sizeof(c)) ||
            !dadosCampo(bancoAtivo->hstmt,4,o->data_abertura,sizeof(o->data_abertura)) ||
            !dadosCampo(bancoAtivo->hstmt,5,o->problema_relatado,sizeof(o->problema_relatado)) ||
            !dadosCampo(bancoAtivo->hstmt,6,o->diagnostico,sizeof(o->diagnostico)) ||
            !dadosCampo(bancoAtivo->hstmt,7,orc,sizeof(orc)) ||
            !dadosCampo(bancoAtivo->hstmt,8,o->status,sizeof(o->status)) ||
            !dadosCampo(bancoAtivo->hstmt,9,o->resultado_resposta_cliente,sizeof(o->resultado_resposta_cliente)) ||
            !dadosCampo(bancoAtivo->hstmt,10,o->data_entrega,sizeof(o->data_entrega)) ||
            !dadosCampo(bancoAtivo->hstmt,11,o->resultado_teste,sizeof(o->resultado_teste)) ||
            !dadosCampo(bancoAtivo->hstmt,12,pag,sizeof(pag)) ||
            !dadosCampo(bancoAtivo->hstmt,13,o->feedback_cliente,sizeof(o->feedback_cliente)) ||
            !dadosCampo(bancoAtivo->hstmt,14,o->satisfeito,sizeof(o->satisfeito)) ||
            !dadosCampo(bancoAtivo->hstmt,15,o->criado_em,sizeof(o->criado_em)) ||
            !dadosCampo(bancoAtivo->hstmt,16,o->atualizado_em,sizeof(o->atualizado_em))) goto erro;
        o->id_os=atoi(a); o->id_equipamento=atoi(b); o->id_atendente=atoi(c);
        o->orcamento = *orc ? strtod(orc,NULL) : -1;
        o->valor_pago = *pag ? strtod(pag,NULL) : -1;
        totalOrdens++;
    }
    bdEncerrarConsulta(bancoAtivo);return 1;
erro:
    fprintf(stderr,"Falha ao carregar OS (limite ou tamanho de campo).\n");
    bdEncerrarConsulta(bancoAtivo);totalOrdens=0;return 0;
}

int carregarTecnicosOSSQL(void)
{
    SQLRETURN r;
    const char *sql = "SELECT CONVERT(varchar(20),id_tecnico_os),CONVERT(varchar(20),id_tecnico),"
      "CONVERT(varchar(20),id_os),CONVERT(varchar(10),data_atribuicao,23),"
      "ISNULL(observacoes_tecnicas,''),CONVERT(varchar(19),criado_em,120),"
      "CONVERT(varchar(19),atualizado_em,120) FROM dbo.tecnico_os ORDER BY id_tecnico_os";
    totalTecnicosOS=0;
    if (!bdConsultar(bancoAtivo,sql)) return 0;
    while ((r=SQLFetch(bancoAtivo->hstmt))!=SQL_NO_DATA) {
        char a[32],b[32],c[32]; TecnicoOS *t;
        if (!SQL_SUCCEEDED(r) || totalTecnicosOS>=MAX_TECNICO_OS) goto erro;
        t=&tecnicosOS[totalTecnicosOS]; memset(t,0,sizeof(*t));
        if (!dadosCampo(bancoAtivo->hstmt,1,a,sizeof(a)) ||
            !dadosCampo(bancoAtivo->hstmt,2,b,sizeof(b)) ||
            !dadosCampo(bancoAtivo->hstmt,3,c,sizeof(c)) ||
            !dadosCampo(bancoAtivo->hstmt,4,t->data_atribuicao,sizeof(t->data_atribuicao)) ||
            !dadosCampo(bancoAtivo->hstmt,5,t->observacoes_tecnicas,sizeof(t->observacoes_tecnicas)) ||
            !dadosCampo(bancoAtivo->hstmt,6,t->criado_em,sizeof(t->criado_em)) ||
            !dadosCampo(bancoAtivo->hstmt,7,t->atualizado_em,sizeof(t->atualizado_em))) goto erro;
        t->id_tecnico_os=atoi(a);t->id_tecnico=atoi(b);t->id_os=atoi(c); totalTecnicosOS++;
    }
    bdEncerrarConsulta(bancoAtivo);return 1;
erro:
    fprintf(stderr,"Falha ao carregar tecnico_os (limite ou campo).\n");
    bdEncerrarConsulta(bancoAtivo);totalTecnicosOS=0;return 0;
}

/* A sincronizacao e transacional e nao apaga nenhum registro existente.
   Quando a UI gera um ID, IDENTITY_INSERT o preserva. Para uso concorrente
   por multiplos processos, migrar futuramente para INSERT ... OUTPUT INSERTED.id. */
int salvarEquipamentosSQL(void)
{
    int i, ok = 1, identity = 0;
    const char *upd = "UPDATE dbo.equipamento SET tipo=?, marca=?, modelo_numero_serie=?, "
       "status=?, atualizado_em=SYSDATETIME() WHERE id_equipamento=?";
    const char *ins = "INSERT INTO dbo.equipamento "
       "(id_equipamento,id_cliente,categoria,tipo,marca,modelo_numero_serie,status) "
       "VALUES (?,?,?,?,?,?,?)";
    if (!dadosTransacaoInicio()) return 0;
    for (i=0;i<totalEquipamentos && ok;i++) {
        char id[32]; SQLLEN rows = -1; Equipamento *e=&equipamentos[i];
        dadosNumero(e->id_equipamento,id,sizeof(id));
        const char *p[]={e->tipo,e->marca,e->modelo_numero_serie,e->status,id};
        ok=dadosExecutar(upd,5,p,&rows);
        if (ok && rows==0) {
            char cli[32]; dadosNumero(e->id_cliente,cli,sizeof(cli));
            const char *q[]={id,cli,e->categoria,e->tipo,e->marca,e->modelo_numero_serie,e->status};
            if (!identity) {
                ok=dadosExecutar("SET IDENTITY_INSERT dbo.equipamento ON",0,NULL,NULL);
                identity = ok;
            }
            if (ok) ok=dadosExecutar(ins,7,q,NULL);
        }
        if (ok && rows<0) { fprintf(stderr,"Nao foi possivel confirmar UPDATE equipamento.\n");ok=0; }
    }
    if (identity && !dadosExecutar("SET IDENTITY_INSERT dbo.equipamento OFF",0,NULL,NULL)) ok=0;
    ok=dadosTransacaoFim(ok);
    if (!ok) carregarEquipamentosSQL();
    return ok;
}

int salvarOrdensSQL(void)
{
    int i,ok=1,identity=0;
    const char *upd="UPDATE dbo.os SET problema_relatado=?,diagnostico=?,orcamento=?,status=?,"
      "resultado_resposta_cliente=?,data_entrega=?,resultado_teste=?,valor_pago=?,feedback_cliente=?,"
      "satisfeito=?,atualizado_em=SYSDATETIME() WHERE id_os=?";
    const char *ins="INSERT INTO dbo.os (id_os,id_equipamento,id_atendente,data_abertura,"
      "problema_relatado,diagnostico,orcamento,status,resultado_resposta_cliente,data_entrega,"
      "resultado_teste,valor_pago,feedback_cliente,satisfeito) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    if (!dadosTransacaoInicio()) return 0;
    for (i=0;i<totalOrdens && ok;i++) {
        char id[32],eq[32],at[32],orc[48],pag[48]; SQLLEN rows=-1;
        OrdemServico *o=&ordens[i];
        dadosNumero(o->id_os,id,sizeof(id));dadosNumero(o->id_equipamento,eq,sizeof(eq));
        dadosNumero(o->id_atendente,at,sizeof(at));
        dadosValor(o->orcamento,orc,sizeof(orc));dadosValor(o->valor_pago,pag,sizeof(pag));
        const char *v[]={dadosNuloSeVazio(o->problema_relatado),dadosNuloSeVazio(o->diagnostico),
          o->orcamento<0?NULL:orc,o->status,dadosNuloSeVazio(o->resultado_resposta_cliente),
          dadosNuloSeVazio(o->data_entrega),dadosNuloSeVazio(o->resultado_teste),
          o->valor_pago<0?NULL:pag,dadosNuloSeVazio(o->feedback_cliente),
          dadosNuloSeVazio(o->satisfeito),id};
        ok=dadosExecutar(upd,11,v,&rows);
        if (ok && rows==0) {
            const char *q[]={id,eq,at,o->data_abertura,v[0],v[1],v[2],o->status,
               v[4],v[5],v[6],v[7],v[8],v[9]};
            if (!identity) {ok=dadosExecutar("SET IDENTITY_INSERT dbo.os ON",0,NULL,NULL);identity=ok;}
            if (ok) ok=dadosExecutar(ins,14,q,NULL);
        }
        if (ok && rows<0) {fprintf(stderr,"Nao foi possivel confirmar UPDATE da OS.\n");ok=0;}
    }
    if (identity && !dadosExecutar("SET IDENTITY_INSERT dbo.os OFF",0,NULL,NULL)) ok=0;
    ok=dadosTransacaoFim(ok);
    if (!ok) carregarOrdensSQL();
    return ok;
}

int salvarTecnicosOSSQL(void)
{
    int i,ok=1,identity=0;
    const char *upd="UPDATE dbo.tecnico_os SET observacoes_tecnicas=?,atualizado_em=SYSDATETIME() "
      "WHERE id_tecnico_os=?";
    const char *ins="INSERT INTO dbo.tecnico_os "
      "(id_tecnico_os,id_tecnico,id_os,data_atribuicao,observacoes_tecnicas) VALUES (?,?,?,?,?)";
    if (!dadosTransacaoInicio()) return 0;
    for (i=0;i<totalTecnicosOS && ok;i++) {
        char id[32],tec[32],os[32];SQLLEN rows=-1;TecnicoOS *t=&tecnicosOS[i];
        dadosNumero(t->id_tecnico_os,id,sizeof(id));dadosNumero(t->id_tecnico,tec,sizeof(tec));
        dadosNumero(t->id_os,os,sizeof(os));
        const char *v[]={dadosNuloSeVazio(t->observacoes_tecnicas),id};
        ok=dadosExecutar(upd,2,v,&rows);
        if (ok && rows==0) {
            const char *q[]={id,tec,os,t->data_atribuicao,v[0]};
            if (!identity) {ok=dadosExecutar("SET IDENTITY_INSERT dbo.tecnico_os ON",0,NULL,NULL);identity=ok;}
            if (ok) ok=dadosExecutar(ins,5,q,NULL);
        }
        if (ok && rows<0) {fprintf(stderr,"Nao foi possivel confirmar UPDATE tecnico_os.\n");ok=0;}
    }
    if (identity && !dadosExecutar("SET IDENTITY_INSERT dbo.tecnico_os OFF",0,NULL,NULL)) ok=0;
    ok=dadosTransacaoFim(ok);
    if (!ok) carregarTecnicosOSSQL();
    return ok;
}

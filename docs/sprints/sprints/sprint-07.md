# Sprint 07

**Status:** Concluída - fechada na sexta-feira, 16/10/2026.

**Período:** 10/10/2026 a 16/10/2026.

## Sprint Goal

Completar os 44 casos de uso no código, começar o documento do PIM e deixar o sistema fácil de apresentar.

## Product Backlog Items

- **PBI05** - Abrir ordem de serviço.
- **PBI06** - Registrar diagnóstico técnico.
- **PBI07** - Elaborar orçamento e registrar aprovação do cliente.
- **PBI08** - Executar manutenção com um ou mais técnicos.
- **PBI09** - Registrar teste de funcionamento pós-reparo.
- **PBI10** - Registrar entrega, pagamento e feedback do cliente.
- **PBI11** - Consultas e histórico de ordens de serviço.
- **PBI12** - Validações e integridade dos dados (junto com cada PBI).

Fora do backlog: documento acadêmico do PIM, organização das sprints e acabamento dos diagramas.

## Tasks

### Código (`projeto/`)

- [x] PBI06, PBI08, PBI09: observação técnica, diagnóstico, execução da manutenção, teste e OS prontas para entrega - UC31–UC35 - 15/10.
- [x] PBI07: registrar, editar, consultar, aprovar, recusar e arquivar orçamento, inclusive o de valor zero (RN18) - UC36–UC41 - 15/10.
- [x] PBI11: histórico do equipamento e relatórios - UC42, UC43 - 15/10.
- [x] PBI05, PBI08: criar, consultar, editar, cancelar e atribuir técnico à OS - UC24–UC28 - 16/10.
- [x] PBI10: registrar entrega e pagamento (com aviso de valor diferente do orçamento) e feedback do cliente - UC29, UC30 - 16/10.
- [x] Decisões pendentes do plano: atribuir técnico coloca a OS em andamento (UC28); cliente não satisfeito devolve a OS para o reparo (UC30); a recepcionista vê todas as OS - 16/10.
- [x] Correções: marcas de conflito no `.gitignore` e programa repetindo "Opcao invalida" para sempre quando a entrada acaba - 16/10.
- [x] PR da branch `feat/lote-5-ordens` com merge na `main` - 16/10 (PR #26).
- [x] Roteiro de testes com 28 cenários, que roda o programa com os 4 perfis e leva OS novas do começo ao fim: os 44 casos de uso passaram - 16/10.
- [x] Página de demonstração com as telas reais gravadas do programa, para mostrar ao grupo - 16/10.
- [x] Simplificação do código para a apresentação: SQL Server para o próximo passo, regras das OS com `if` direto, consulta com um filtro e relatório com matriz (ver "Decisões desta sprint") - 16/10. Na branch `refactor/codigo-simples`, aguardando PR.

### Documentação acadêmica

- [x] Iniciar o documento do PIM com resumo, abstract, introdução, caracterização da organização, pesquisa científica, engenharia de software, planejamento ágil e referências (`docs/documentacao-pim/`) - 12/10. O primeiro envio (PR #24) trouxe arquivos antigos junto e foi desfeito no PR #25; o documento foi reenviado no mesmo dia.
- [x] Atualizar no documento do PIM o RNF04 e a seção 3.6 (arquitetura), e acrescentar o Quadro 5, que mostra onde cada elemento do Diagrama Estendido está no código - 16/10.
- [x] Fechar a Sprint 05 e dividir o período de 03/10 a 23/10 em sprints de uma semana (06, 07 e 08) - 16/10.
- [x] Atualizar o status dos PBIs no [Product Backlog](../../product-backlog/product-backlog.md) - 16/10.
- [x] Alinhar a documentação ao código simplificado: descrições do UC25 (filtro), UC28 (mais de um técnico) e UC43 (relatório por ano), exemplo do RF10 e README (persistência, "Como executar" e link do documento do PIM) - 16/10.

### Modelagem

- [x] Conferir os nomes dos 44 casos de uso nos diagramas, nas descrições e no código: todos iguais - 16/10.
- [x] Trocar o título "byteTech - Diagrama de Classes" para "byteTech - Diagrama Estendido" no `diagramas.asta` e exportar de novo os PNGs - 16/10.
- [x] No Astah, trocar `OrdemServico.gerarRelatorio(inicio, fim)` por `gerarRelatorio(ano)`, porque o relatório agora pede o ano, e exportar os PNGs de novo (`2026-10-16-diagramas-de-casos-de-uso-e-estendido/`) - 16/10.

## Decisões desta sprint

### 16/10 - Código mais simples, só com arquivos .txt

**Por quê.** O Manual do PIM II pede, na Etapa 5, a implementação em C das *funcionalidades principais*, usando condicionais, repetição, funções, vetores, matrizes e arquivos. O código tinha um começo de ligação com o SQL Server (ODBC), que ainda não foi vista nas aulas, e um mecanismo genérico de "etapas" da OS que era difícil de explicar. O grupo quer um código que todos consigam apresentar.

**O que foi decidido.** Os 44 casos de uso continuam. O que mudou foi o jeito de escrever:

- **SQL Server fica para o próximo passo.** Nesta versão, o programa grava só em `projeto/dados/*.txt`. O `banco.c` e a chave `USAR_SQLSERVER`, que estavam vazios, saíram para não confundir quem lê o código; o esqueleto (string de conexão e funções `bdConectar`, `bdExecutar` etc.) pode ser recuperado do Git, commit `e1080f0`. O `schema_sqlserver.sql` continua como modelo físico do banco e fonte dos comandos SQL da Etapa 4; ligar o programa a ele será feito depois.
- **Regras das OS com `if`.** A lista de OS (`selecionarOS`) mostra as OS em aberto com a etapa de cada uma, e cada caso de uso confere a própria regra com um `if` e uma mensagem clara (ex.: "A OS 9 ainda nao tem diagnostico"). Saíram a função `osNaEtapa` e as 10 constantes `ETAPA_*`.
- **Consultar OS com um filtro só:** cliente, CPF ou etapa.
- **Relatório com matriz:** `int contagem[12][4]`, uma linha por mês e uma coluna por status, como o exemplo das notas na Unidade 6. Assim o código usa vetores e matrizes, como o manual pede.
- **Valor em dinheiro lido por `lerValor`**, no `main.c`, junto com `lerTexto` e `lerInteiro`.
- **Uma função só** (`buscarTecnicoNaOS`) para saber se o técnico está na OS, no lugar de duas quase iguais.

**O que muda para quem usa o sistema.**

- Abrir OS não atribui técnico na hora: em seguida, usa-se Atribuir Técnico. Para pôr mais de um técnico na OS, repete-se a opção (RN04).
- Na abertura da OS, o CPF é digitado como foi cadastrado (com ponto e traço), igual à busca de clientes.
- Feedback: primeiro o comentário, depois "O cliente ficou satisfeito? (S/N)".
- O relatório pede o ano, e não mais a data inicial e final.
- OS finalizada não é mais editada, nem pelo Admin. **Para o grupo revisar:** a RN08 permite a "operação administrativa"; se quiserem manter essa exceção, ela volta com um `if` no `editarOS`.

**Resultado.** O código passou de 3.647 para 3.254 linhas. Os 28 cenários de teste continuam passando, cobrindo os 44 casos de uso.

### 16/10 - O nome continua Diagrama Estendido

**Decisão.** O grupo mantém o nome **Diagrama Estendido**, escolhido em 30/09 porque C não tem classes.

**Como explicar na apresentação.** O Diagrama Estendido é feito a partir do diagrama de classes da UML, usando estereótipos: `«boundary»` para as telas e `«entity»` para os dados. Em C não há classes, então cada entidade é uma `struct` e cada tela é um arquivo `.c` com o seu menu. Assim ele também atende ao item "diagrama de classes (quando aplicável)" da Etapa 3 do Manual do PIM. O documento do PIM mostra essa correspondência no Quadro 5.

**Sobre casos de uso "estendidos".** `«extend»` (e `«include»`) são ligações entre casos de uso na UML. São opcionais, e o projeto não usa nenhuma: os diagramas têm só associações e a generalização do Admin. As 44 descrições dizem "Sub UseCase: Não se aplica", o que está coerente com isso.

### 16/10 - Sprints de uma semana

As Sprints 05 e 06 não foram fechadas a tempo, e a Sprint 06 chegou a ser aberta com três semanas. O período foi dividido de novo em sprints de uma semana (06, 07 e 08), de sábado a sexta, como diz o [README das sprints](../README.md). Daqui em diante, toda sexta-feira o grupo fecha a sprint e abre a próxima a partir do [modelo](../modelo-sprint.md).

## Sprint Review

Os 44 casos de uso funcionam com os arquivos `.txt`, e o código compila sem avisos (`gcc -Wall -Wextra main.c`). Um roteiro de 28 cenários testou o sistema com os 4 perfis, levando OS novas da abertura à finalização, e todos passaram. O código foi simplificado para a apresentação, e o documento do PIM foi iniciado. Os diagramas foram exportados de novo com o título "Diagrama Estendido".

Não foram entregues: os protótipos no Figma e as etapas do documento que faltam (4 a 8).

## Retrospective

### Funcionou bem

- Dividir o código entre o grupo: a manutenção, o orçamento e os relatórios foram feitos em paralelo com as ordens de serviço.
- O roteiro de testes mostrou rapidamente que tudo continuava funcionando depois da simplificação.

### Problemas

- Um commit subiu o `.gitignore` com marcas de conflito do Git, e o primeiro envio do documento do PIM (PR #24) precisou ser desfeito.
- A pasta do documento foi criada com o ano errado (`20206-10-12`).
- O código ficou maior do que o manual pede, e foi preciso simplificar depois.
- O nome do diagrama gerou dúvida no grupo.

### Melhorias

- Antes de cada commit, olhar o `git status` e o `git diff` para não enviar sobras de conflito.
- Conferir o Manual do PIM antes de aumentar o escopo.
- Fechar a sprint toda sexta-feira, mesmo que incompleta.

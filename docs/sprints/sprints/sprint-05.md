# Sprint 05

**Status:** Concluída - fechada com atraso, em 16/10/2026, a partir do histórico do Git.

**Período:** 26/09/2026 a 02/10/2026.

## Sprint Goal

Agilizar a implementação em C com persistência em banco de dados SQL Server ou em arquivos de texto: pesquisar as duas formas de gravar os dados e avançar a estrutura do código (projeto, persistência e login). A documentação acadêmica e o acabamento da modelagem seguem em paralelo, com prioridade menor.

## Product Backlog Items

- **PBI01** - Estrutura inicial do projeto e persistência (SQL Server ou `.txt`).
- **PBI02** - Autenticação e controle de acesso por perfil.

Fora do backlog: pesquisa de persistência em C (SQL Server e `.txt`), documentação acadêmica do PIM e acabamento da modelagem.

## Tasks

### Prioridade da semana: persistência (SQL Server ou .txt) e estrutura do código

- [x] Esqueleto do sistema em C: cabeçalho, tela de login e menu por perfil (`projeto/main.c`) - 28/09.
- [x] Organizar o código na pasta `projeto/` - 29/09.
- [x] Plano de implementação: um `.c` por pacote do Astah e uma função por caso de uso (`ucNN_verboObjeto`) - 29/09.
- [x] Definir as opções de persistência: banco de dados SQL Server (`docs/Modelagem_DB/schema_sqlserver.sql`) ou arquivos de texto - 30/09.
- [x] Criar os arquivos `.c` de cada pacote com as funções dos casos de uso ainda vazias, o `banco.c` e a chave `USAR_SQLSERVER` no `main.c` para escolher entre SQL Server e `.txt` - 01/10.
- [ ] Pesquisar a conexão do C com o SQL Server via ODBC (`sql.h`/`sqlext.h`, compilação com `-lodbc32`, driver "ODBC Driver for SQL Server" e string de conexão). → Fica no backlog como opcional: o grupo decidiu fazer primeiro a versão em `.txt`.
- [ ] Instalar e configurar o SQL Server (Express ou Developer) em cada máquina do grupo e executar o `schema_sqlserver.sql`. → Opcional, junto com a tarefa anterior.
- [ ] Fazer um teste mínimo em C com SQL Server: conectar ao banco, inserir e consultar um usuário. → Opcional, junto com a tarefa anterior.
- [ ] Definir o formato dos arquivos `.txt` (um arquivo por tabela, campos separados por `;`, mesmos campos do `schema_sqlserver.sql`). → Feito na Sprint 06 (07/10).
- [ ] Fazer um teste mínimo em C com `.txt`: gravar, ler e consultar um usuário. → Feito na Sprint 06 (07/10).
- [ ] Avançar a estrutura do código: structs das 4 entidades (`usuario`, `equipamento`, `os`, `tecnico_os`) e os arquivos `.c` de cada pacote, conforme o plano. → Os arquivos `.c` foram criados nesta sprint (01/10); as structs, na Sprint 06 (07/10).
- [ ] PBI01: funções de persistência (abrir, ler, gravar e tratar erros) para as duas opções, começando pelos usuários. → Versão `.txt` feita na Sprint 06 (07/10); SQL Server opcional.
- [ ] PBI02: login consultando os usuários salvos (tabela `usuario` ou `usuario.txt`), senha com hash (RNF10) e bloqueio de usuário inativo (RN12) - UC01. → Feito na Sprint 06 (07/10).
- [ ] PBI02: Fazer Logout, Consultar Meus Dados e Alterar Senha - UC02, UC03, UC44. → Feito na Sprint 06 (07/10).
- [ ] Validar com o grupo as decisões pendentes do plano (UC28, UC30 e escopo da recepcionista). → Decididas na Sprint 07, durante a implementação (16/10).

### Modelagem

- [x] Criar o diagrama estendido (antes chamado de diagrama de classes) com as telas (`«boundary»`) e as entidades - 26/09.
- [x] Revisão geral e correções dos casos de uso - 26/09.
- [x] Remover a planilha `requisitos.xlsx`: o `requisitos.md` passa a ser a única fonte dos requisitos - 01/10.
- [x] Atualizar os atributos da tabela `os` - 27/09.
- [x] Exportar os modelos conceitual e lógico do banco em PNG - 28/09.
- [x] Renomear o diagrama para "Diagrama Estendido" no Astah (C não tem classes) e exportar os PNGs - 30/09.
- [x] Exportar o Diagrama Estendido completo e uma versão melhorada em PNG - 01/10.

### Documentação acadêmica

- [ ] Iniciar o documento do PIM no formato do manual (`docs/manual-pim/`), reaproveitando requisitos, casos de uso e modelagem já prontos. → Iniciado na Sprint 07 (12/10).

## Sprint Review

A estrutura do código ficou pronta: pasta `projeto/` com o `main.c`, um `.c` por pacote do Astah (geral, admin, recepcionista, técnico, cliente, equipamento, ordem de serviço, manutenção, orçamento e relatório) e o `banco.c`. Cada caso de uso tem a sua função, ainda vazia, com o comentário `/* UCnn */`. A persistência foi definida: arquivos `.txt` primeiro e SQL Server como opção, escolhida pela chave `USAR_SQLSERVER`. Na modelagem, o diagrama de classes virou o Diagrama Estendido (telas e entidades), exportado em PNG junto com os modelos do banco.

Não foram entregues nesta sprint: a gravação de dados, o login real e o início do documento do PIM. A gravação e o login foram feitos na Sprint 06, e o documento começou na Sprint 07.

## Retrospective

### Funcionou bem

- Um `.c` por pacote do Astah e uma função por caso de uso deixaram claro onde cada parte do sistema vai ficar e facilitaram dividir o trabalho.
- Ter as duas opções de persistência numa chave (`USAR_SQLSERVER`) permitiu começar pelo `.txt` sem descartar o SQL Server.

### Problemas

- A sprint não foi fechada na sexta-feira (02/10): a review e a retrospectiva só foram escritas em 16/10.
- Parte da semana foi gasta reorganizando a pasta do código (código na raiz e depois em `projeto/`, em 29/09), e a gravação de dados ficou para a semana seguinte.
- A pesquisa do SQL Server (conexão ODBC e instalação do banco) não foi feita.
- O executável `projeto/output/main.exe` foi versionado no Git.

### Melhorias

- Fechar a sprint toda sexta-feira, mesmo que incompleta, e levar o que faltar para a próxima.
- Fazer a versão `.txt` funcionar de ponta a ponta antes de voltar ao SQL Server.
- Colocar `projeto/output/` no `.gitignore`.

# Sprint 05

**Status:** Em andamento - fecha na sexta-feira, 02/10/2026.

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
- [ ] Pesquisar a conexão do C com o SQL Server via ODBC (`sql.h`/`sqlext.h`, compilação com `-lodbc32`, driver "ODBC Driver for SQL Server" e string de conexão).
- [ ] Instalar e configurar o SQL Server (Express ou Developer) em cada máquina do grupo e executar o `schema_sqlserver.sql`.
- [ ] Fazer um teste mínimo em C com SQL Server: conectar ao banco, inserir e consultar um usuário.
- [ ] Definir o formato dos arquivos `.txt` (um arquivo por tabela, campos separados por `;`, mesmos campos do `schema_sqlserver.sql`).
- [ ] Fazer um teste mínimo em C com `.txt`: gravar, ler e consultar um usuário.
- [ ] Avançar a estrutura do código: structs das 4 entidades (`usuario`, `equipamento`, `os`, `tecnico_os`) e os arquivos `.c` de cada pacote, conforme o plano.
- [ ] PBI01: funções de persistência (abrir, ler, gravar e tratar erros) para as duas opções, começando pelos usuários.
- [ ] PBI02: login consultando os usuários salvos (tabela `usuario` ou `usuario.txt`), senha com hash (RNF10) e bloqueio de usuário inativo (RN12) - UC01.
- [ ] PBI02: Fazer Logout, Consultar Meus Dados e Alterar Senha - UC02, UC03, UC44.
- [ ] Validar com o grupo as decisões pendentes do plano (UC28, UC30 e escopo da recepcionista).

### Modelagem

- [x] Criar o diagrama estendido (antes chamado de diagrama de classes) com as telas (`«boundary»`) e as entidades - 26/09.
- [x] Revisão geral e correções dos casos de uso - 26/09.
- [x] Remover a planilha `requisitos.xlsx`: o `requisitos.md` passa a ser a única fonte dos requisitos - 01/10.
- [x] Atualizar os atributos da tabela `os` - 27/09.
- [x] Exportar os modelos conceitual e lógico do banco em PNG - 28/09.
- [x] Renomear o diagrama para "Diagrama Estendido" no Astah (C não tem classes) e exportar os PNGs - 30/09.

### Documentação acadêmica

- [ ] Iniciar o documento do PIM no formato do manual (`docs/manual-pim/`), reaproveitando requisitos, casos de uso e modelagem já prontos.

## Sprint Review

*A ser preenchido na sexta-feira, 02/10.*

## Retrospective

### Funcionou bem

*A ser preenchido na sexta-feira, 02/10.*

### Problemas

*A ser preenchido na sexta-feira, 02/10.*

### Melhorias

*A ser preenchido na sexta-feira, 02/10.*

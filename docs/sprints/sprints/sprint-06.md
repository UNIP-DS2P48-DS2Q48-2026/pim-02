# Sprint 06

**Status:** Concluída - fechada em 16/10/2026, a partir do histórico do Git.

**Período:** 03/10/2026 a 09/10/2026.

## Sprint Goal

Fazer o programa gravar dados de verdade em arquivos `.txt`: login real, cadastros de usuários e de equipamentos, com o código no nível das aulas.

## Product Backlog Items

- **PBI01** - Estrutura inicial do projeto e persistência (versão `.txt`).
- **PBI02** - Autenticação e controle de acesso por perfil.
- **PBI03** - Gerenciar usuários (admin, recepcionista, técnico, cliente).
- **PBI04** - Gerenciar equipamentos.
- **PBI12** - Validações e integridade dos dados (junto com cada PBI).

## Tasks

### Código (`projeto/`)

- [x] Primeira versão das telas de cliente, recepcionista e equipamento (perguntas e sequência das telas, ainda sem gravar dados) - 07/10.
- [x] Lote 0: funções de leitura (`lerTexto`, `lerInteiro`, `confirmar`, `pausar`), hash da senha e data de hoje - 07/10.
- [x] Lote 1 - PBI01, PBI02: structs das 4 tabelas, um arquivo `.txt` por tabela (campos separados por `;`), login com hash da senha (RNF10) e bloqueio de usuário inativo (RN12), Meus Dados e Alterar Senha - UC01–UC03, UC44 - 07/10.
- [x] Lote 2 - PBI02: um menu para cada perfil, conforme a Matriz de Permissões (RN10, RN11) - 07/10.
- [x] Lote 3 - PBI03: cadastrar, consultar, editar e inativar administradores, recepcionistas, técnicos e clientes, com e-mail e CPF únicos (RN13) e bloqueio por OS em andamento (RN14) - UC04–UC19 - 07/10.
- [x] Lote 4 - PBI04: cadastrar, consultar, editar e inativar equipamentos de hardware e software - UC20–UC23 - 07/10.
- [x] Revisão do código para usar só o que foi visto nas aulas (Unidades 1 a 5), cabeçalho com o nome dos campos nos `.txt`, aviso quando o programa é aberto fora da pasta `projeto/` e dados de exemplo (9 usuários, 7 equipamentos, 8 OS) - 08/10 (PR #23).

### Documentação acadêmica

- [ ] Iniciar o documento do PIM. → Feito na Sprint 07 (12/10).

## Sprint Review

O sistema passou a gravar de verdade: login com senha em hash, menus por perfil, cadastro, consulta, edição e inativação dos quatro perfis de usuário e dos equipamentos. São 24 dos 44 casos de uso funcionando com os arquivos `.txt`, com dados de exemplo para testar. O código foi revisado para usar só o que foi visto nas aulas.

Não foram entregues: as ordens de serviço, a manutenção, o orçamento e o documento do PIM.

## Retrospective

### Funcionou bem

- Dividir o código em lotes pequenos, com um commit cada, deixou fácil ver o que tinha mudado.
- A revisão "no nível das aulas" deixou o código mais fácil de explicar na apresentação.

### Problemas

- As primeiras telas de cliente, recepcionista e equipamento só imprimiam mensagens de sucesso, sem gravar nada, e precisaram ser escritas de novo.
- Quase todo o trabalho ficou concentrado em dois dias (07 e 08/10).
- A Sprint 05 ainda estava aberta, e esta sprint também não foi fechada na sexta-feira.

### Melhorias

- Combinar no começo da semana quem faz qual parte, para o trabalho não ficar todo no fim.
- Testar no terminal antes de enviar: a tela tem que gravar no `.txt`, e não só mostrar a mensagem.

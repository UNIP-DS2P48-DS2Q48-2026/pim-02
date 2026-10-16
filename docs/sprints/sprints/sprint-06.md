# Sprint 06

**Status:** Em andamento - fecha na sexta-feira, 23/10/2026.

**Período:** 03/10/2026 a 23/10/2026.

Esta sprint durou três semanas, e não uma. As semanas de 03/10 a 16/10 não tiveram sprint aberta, porque a Sprint 05 não foi fechada a tempo. O trabalho desse período (registrado no Git) entrou aqui, junto com o planejamento da semana de 17/10 a 23/10.

## Sprint Goal

Fazer o sistema em C funcionar de ponta a ponta com arquivos `.txt`, cobrindo os 44 casos de uso, e avançar a documentação do PIM, incluindo os protótipos das telas no Figma.

## Product Backlog Items

- **PBI01** - Estrutura inicial do projeto e persistência (versão `.txt`).
- **PBI02** - Autenticação e controle de acesso por perfil.
- **PBI03** - Gerenciar usuários (admin, recepcionista, técnico, cliente).
- **PBI04** - Gerenciar equipamentos.
- **PBI05** - Abrir ordem de serviço.
- **PBI06** - Registrar diagnóstico técnico.
- **PBI07** - Elaborar orçamento e registrar aprovação do cliente.
- **PBI08** - Executar manutenção com um ou mais técnicos.
- **PBI09** - Registrar teste de funcionamento pós-reparo.
- **PBI10** - Registrar entrega, pagamento e feedback do cliente.
- **PBI11** - Consultas e histórico de ordens de serviço.
- **PBI12** - Validações e integridade dos dados (junto com cada PBI).

Fora do backlog: documento acadêmico do PIM, protótipos no Figma e acabamento da modelagem.

## Tasks

### Código (`projeto/`)

- [x] Primeira versão das telas de cliente, recepcionista e equipamento (perguntas e sequência das telas, ainda sem gravar dados) - 07/10.
- [x] Lote 0: funções de leitura (`lerTexto`, `lerInteiro`, `confirmar`, `pausar`), hash da senha e data de hoje - 07/10.
- [x] Lote 1 - PBI01, PBI02: structs das 4 tabelas, um arquivo `.txt` por tabela (campos separados por `;`), login com hash da senha (RNF10) e bloqueio de usuário inativo (RN12), Meus Dados e Alterar Senha - UC01–UC03, UC44 - 07/10.
- [x] Lote 2 - PBI02: um menu para cada perfil, conforme a Matriz de Permissões (RN10, RN11) - 07/10.
- [x] Lote 3 - PBI03: cadastrar, consultar, editar e inativar administradores, recepcionistas, técnicos e clientes, com e-mail e CPF únicos (RN13) e bloqueio por OS em andamento (RN14) - UC04–UC19 - 07/10.
- [x] Lote 4 - PBI04: cadastrar, consultar, editar e inativar equipamentos de hardware e software - UC20–UC23 - 07/10.
- [x] Revisão do código para usar só o que foi visto nas aulas (Unidades 1 a 5), cabeçalho com o nome dos campos nos `.txt`, aviso quando o programa é aberto fora da pasta `projeto/` e dados de exemplo (9 usuários, 7 equipamentos, 8 OS) - 08/10 (PR #23).
- [x] PBI06, PBI08, PBI09: observação técnica, diagnóstico, execução da manutenção, teste e OS prontas para entrega - UC31–UC35 - 15/10.
- [x] PBI07: registrar, editar, consultar, aprovar, recusar e arquivar orçamento, inclusive o de valor zero (RN18) - UC36–UC41 - 15/10.
- [x] PBI11: histórico do equipamento e relatórios por período - UC42, UC43 - 15/10.
- [x] PBI05, PBI08: criar, consultar (por número, cliente, status/etapa ou período), editar, cancelar e atribuir um ou mais técnicos à OS - UC24–UC28 - 16/10.
- [x] PBI10: registrar entrega e pagamento (com aviso de valor diferente do orçamento) e feedback do cliente - UC29, UC30 - 16/10.
- [x] Decisões pendentes do plano: atribuir técnico coloca a OS em andamento (UC28); cliente não satisfeito devolve a OS para o reparo (UC30); a recepcionista vê todas as OS - 16/10.
- [x] Correções: marcas de conflito no `.gitignore` e programa repetindo "Opcao invalida" para sempre quando a entrada acaba - 16/10.
- [x] Teste do ciclo completo de uma OS nova com os 4 perfis, da abertura ao feedback - 16/10.
- [ ] Abrir o PR da branch `feat/lote-5-ordens` e fazer o merge na `main`.
- [ ] Atualizar o status dos PBIs no [Product Backlog](../../product-backlog/product-backlog.md).
- [ ] Opcional: não apagar o texto do reparo quando o técnico registra uma nova observação; exigir o reparo registrado antes do teste.
- [ ] Opcional (Lote 8): versão com SQL Server no `banco.c`.

### Documentação acadêmica

- [x] Iniciar o documento do PIM com resumo, abstract, introdução, caracterização da organização, pesquisa científica, engenharia de software, planejamento ágil e referências (`docs/documentacao-pim/`) - 12/10. O primeiro envio (PR #24) trouxe arquivos antigos junto e foi desfeito no PR #25; o documento foi reenviado no mesmo dia.
- [ ] Atualizar a seção 4 (Planejamento ágil) com as Sprints 05 e 06.
- [ ] Escrever a seção sobre a implementação em C: organização dos arquivos, persistência em `.txt`, menus por perfil e como compilar e executar.
- [ ] Incluir no documento os prints das telas do terminal e os protótipos do Figma.
- [ ] Revisar o documento com o grupo e passar para o formato do manual (`docs/manual-pim/`).
- [ ] Corrigir o nome da pasta `docs/documentacao-pim/20206-10-12/` para `2026-10-12`.

### Protótipos (Figma)

- [ ] Criar o arquivo do projeto no Figma e definir o padrão das telas, igual ao terminal: cabeçalho BYTETECH com a data, 80 colunas e menus numerados.
- [ ] Prototipar o Login e os 4 menus principais (Administrador, Recepcionista, Técnico e Cliente).
- [ ] Prototipar as telas de cada pacote do Diagrama Estendido: Usuário, Equipamento, Ordem de Serviço, Manutenção, Orçamento, Histórico e Relatório, Meus Dados.
- [ ] Ligar as telas no modo protótipo do Figma, seguindo o caminho de uma OS: abertura, diagnóstico, orçamento, reparo, teste, entrega e feedback.
- [ ] Exportar os protótipos em PNG para `docs/` e colocar o link do Figma no README.

### Modelagem

- [ ] Trocar o título "byteTech - Diagrama de Classes" para "byteTech - Diagrama Estendido" no `diagramas.asta` e exportar de novo os PNGs.

## Sprint Review

*A ser preenchido na sexta-feira, 23/10.*

Andamento até 16/10: os 44 casos de uso funcionam com os arquivos `.txt`, e o código compila sem avisos (`gcc -Wall -Wextra main.c`). O documento do PIM está iniciado. Os protótipos no Figma ainda não começaram.

## Retrospective

### Funcionou bem

*A ser preenchido na sexta-feira, 23/10.*

### Problemas

*A ser preenchido na sexta-feira, 23/10.*

### Melhorias

*A ser preenchido na sexta-feira, 23/10.*

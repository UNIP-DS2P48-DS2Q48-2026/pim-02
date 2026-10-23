# Sprints

Cadência: **1 semana = 1 sprint**, de sábado a sexta-feira, fechando toda **sexta-feira**. No fechamento de cada sprint, na mesma reunião:

1. **Sprint Review** da sprint que está fechando - o que foi entregue, à vista de todos.
2. **Retrospective** - o que funcionou, o que atrapalhou, o que muda na próxima.
3. **Sprint Planning** da sprint seguinte - meta e PBIs da próxima semana, já registrados no arquivo `sprint-XX.md` correspondente.

As Sprints 01 a 04 foram reconstruídas a partir do histórico do Git, pois não haviam sido documentadas em tempo real. Uma sprint pode conter, além de PBIs do [Product Backlog](../product-backlog/product-backlog.md), tarefas de documentação e modelagem exigidas pelo PIM (diagramas, banco de dados, documento acadêmico).

| Sprint | Período | Foco | Status |
|---|---|---|---|
| [Sprint 01](./sprints/sprint-01.md) | 27/08 – 04/09/2026 | Estrutura da documentação, modelo de dados inicial e casos de uso v1 | Concluída (reconstruída) |
| [Sprint 02](./sprints/sprint-02.md) | 05/09 – 11/09/2026 | Casos de uso por módulo e ajuste do modelo de usuário | Concluída (reconstruída) |
| [Sprint 03](./sprints/sprint-03.md) | 12/09 – 18/09/2026 | Reescrita de requisitos, escopo e modelo de dados (4 tabelas) | Concluída (reconstruída) |
| [Sprint 04](./sprints/sprint-04.md) | 19/09 – 25/09/2026 | Fluxogramas, casos de uso no Astah, modelo lógico e descrição dos 44 UCs | Concluída (reconstruída) |
| [Sprint 05](./sprints/sprint-05.md) | 26/09 – 02/10/2026 | Persistência em SQL Server ou .txt e avanço da estrutura do código (prioridade); diagrama estendido e documentação acadêmica | Concluída (fechada em 16/10) |
| [Sprint 06](./sprints/sprint-06.md) | 03/10 – 09/10/2026 | Login real, cadastros de usuários e equipamentos gravando em `.txt` e revisão do código no nível das aulas | Concluída (fechada em 16/10) |
| [Sprint 07](./sprints/sprint-07.md) | 10/10 – 16/10/2026 | Os 44 casos de uso funcionando, testes, código simplificado para a apresentação e início do documento do PIM | Concluída |
| [Sprint 08](./sprints/sprint-08.md) | 17/10 – 23/10/2026 | Etapas 3 a 8 do documento do PIM, protótipos no Figma e roteiro da apresentação | Em andamento |
| [Sprint 09](./sprints/sprint-09.md) | 24/10 – 30/10/2026 | Documento do PIM finalizado, documento do código, manual de uso do sistema e ajustes finais | Planejada |

A Sprint 01 tem 9 dias porque inclui a organização inicial do projeto; a partir da Sprint 02, todas vão de sábado a sexta-feira.

## Como manter o controle

Toda **sexta-feira**, na reunião de fechamento:

1. No arquivo da sprint que fecha, marcar `[x]` nas tarefas feitas (com a data), escrever a **Sprint Review** e a **Retrospective** e trocar o **Status** para `Concluída`.
2. O que não terminou ganha a nota `→ passou para a Sprint NN` e é copiado para a próxima.
3. Criar o arquivo da próxima sprint a partir do [modelo](./modelo-sprint.md), com a meta e as tarefas da semana (e quem faz cada uma).
4. Atualizar a tabela acima e o status dos PBIs no [Product Backlog](../product-backlog/product-backlog.md).

## Definition of Done

Um item só é considerado concluído quando:

- **Documentação/modelagem:** está no repositório (merge na `main`), com o arquivo-fonte (`.asta`, `.brM3`, `.md`) e, quando for diagrama, a exportação em PNG.
- **Código:** compila sem erros (`gcc main.c -o output/main.exe -lodbc32`), foi testado manualmente no terminal pelo fluxo principal e pelas exceções do caso de uso, respeita a Matriz de Permissões e os dados persistem após fechar o programa.
- **Rastreabilidade:** o item está ligado ao seu PBI/requisito/caso de uso, e o arquivo da sprint foi atualizado.

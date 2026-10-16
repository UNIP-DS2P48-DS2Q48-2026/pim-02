# Product Backlog

Cada PBI corresponde a uma User Story do [Levantamento de Dados](../levantamento-de-dados/levantamento-de-dados.md) (PBI01 ↔ US01, PBI02 ↔ US02...) e está ligado aos [requisitos](../requisitos/requisitos.md) e aos [casos de uso](../Modalagem_casos_de_uso/descricoes-astah.md) que ele implementa. A ordem da tabela é a ordem sugerida de implementação.

Prioridade: `Alta` / `Média` / `Baixa`. Status: `A fazer` / `Em andamento` / `Concluído` (conforme a [Definition of Done](../sprints/README.md#definition-of-done)).

| ID    | Item                                                        | Requisito(s)                              | Casos de uso        | Prioridade | Status       |
| ----- | ----------------------------------------------------------- | ----------------------------------------- | ------------------- | ---------- | ------------ |
| PBI01 | Estrutura inicial do projeto e persistência (.txt; SQL Server no próximo passo) | RNF01, RNF02, RNF03, RNF04, RNF06, RNF07  | -                   | Alta       | Concluído (S06) |
| PBI02 | Autenticação e controle de acesso por perfil                | RF01, RNF10, RN10, RN12, RN19             | UC01–UC03, UC44     | Alta       | Concluído (S06) |
| PBI03 | Gerenciar usuários (admin, recepcionista, técnico, cliente) | RF02, RN05, RN12, RN13, RN17              | UC04–UC19           | Alta       | Concluído (S06) |
| PBI04 | Gerenciar equipamentos (hardware e software)                | RF03, RN01, RN12, RN14, RN16              | UC20–UC23           | Alta       | Concluído (S06) |
| PBI05 | Abrir ordem de serviço                                      | RF04, RN02, RN03, RN09, RN11, RN14        | UC24–UC27           | Alta       | Concluído (S07) |
| PBI06 | Registrar diagnóstico técnico                               | RF05, RN18                                | UC31, UC32          | Alta       | Concluído (S07) |
| PBI07 | Elaborar orçamento e registrar aprovação do cliente         | RF06, RN09, RN18                          | UC36–UC41           | Alta       | Concluído (S07) |
| PBI08 | Executar manutenção com um ou mais técnicos                 | RF07, RN04, RN07, RN15                    | UC28, UC33          | Alta       | Concluído (S07) |
| PBI09 | Registrar teste de funcionamento pós-reparo                 | RF08                                      | UC34                | Média      | Concluído (S07) |
| PBI10 | Registrar entrega, pagamento e feedback do cliente          | RF09                                      | UC29, UC30, UC35    | Média      | Concluído (S07) |
| PBI11 | Consultas e histórico de ordens de serviço                  | RF10, RN08, RN10                          | UC42, UC43          | Média      | Concluído (S07) |
| PBI12 | Validações e integridade dos dados do sistema               | RNF08, RNF09                              | todos (transversal) | Alta       | Concluído (S07) |
PBI12 não é implementado sozinho: as validações de cada operação entram junto com o PBI correspondente, e o PBI12 só é concluído quando todos os outros estiverem.

O número entre parênteses é a sprint em que o PBI foi concluído (S06 = [Sprint 06](../sprints/sprints/sprint-06.md), S07 = [Sprint 07](../sprints/sprints/sprint-07.md)). No PBI01, a versão concluída grava em `.txt`; ligar o programa ao SQL Server é o próximo passo.

Os 44 casos de uso estão distribuídos entre os PBIs acima, sem repetição.

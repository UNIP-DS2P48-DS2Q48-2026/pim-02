# Sprint 04

**Status:** Concluída - reconstruída a partir do histórico do Git.

**Período:** 19/09/2026 a 25/09/2026.

## Sprint Goal

Transformar os requisitos reescritos em modelagem: fluxogramas do processo, casos de uso no Astah com descrição detalhada e modelo lógico do banco de dados.

## Product Backlog Items

Nenhum PBI de código - modelagem (Etapas de Engenharia de Software e Banco de Dados do PIM) que cobre os PBI01 a PBI12.

## Tasks

- [x] Publicar os requisitos reescritos, a estrutura e as planilhas (Sprint 03) - 19/09.
- [x] Publicar os 8 fluxogramas do processo (`docs/Fluxograma/`) - 19/09.
- [x] Atualizar os diagramas de casos de uso no Astah e corrigir o fluxo, incluindo o cancelamento de OS - 19/09 e 20/09.
- [x] Atualizar o modelo conceitual do banco de dados - 20/09.
- [x] Criar o modelo lógico do banco de dados - 21/09.
- [x] Realizar testes de estresse dos triggers (`2026-09-22-testes-estresse-triggers/testes.sql`) - 22/09.
- [x] Revisar UML, RN e casos de uso - 25/09.
- [x] Escrever as descrições dos 44 casos de uso no formato do Astah (`descricoes-astah.md`) e exportar um PNG por caso de uso - 25/09.

## Sprint Review

Fluxogramas, diagramas de casos de uso (44 UCs, com descrição de fluxo principal, alternativo e de exceção), modelo lógico do banco e scripts de teste dos triggers publicados no repositório.

## Retrospective

### Funcionou bem

- As descrições dos casos de uso ficaram correlacionadas com os fluxogramas, o que vai guiar a implementação função por função.

### Problemas

- Cada revisão dos diagramas virou uma nova pasta datada (`v2`, `v2` do dia seguinte...), deixando `docs/Modalagem_casos_de_uso/` com várias cópias quase iguais - o Git já guarda o histórico.

### Melhorias

- Manter só a versão atual dos diagramas na pasta e deixar o histórico para o Git.
- Iniciar a implementação em C na próxima sprint.

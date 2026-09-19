# Sprint 04

**Status:** Concluída — fechada em 18/09/2026.

**Período:** 12/09/2026 a 18/09/2026 (mesma semana da Sprint 03, que segue em andamento à parte).

## Sprint Goal

Confrontar os requisitos existentes com o recado do professor e com o fluxo real do processo (Miro), simplificar o modelo de dados e fechar as decisões de escopo pendentes.

## Product Backlog Items

PBI01 a PBI12 (reescritos nesta sprint) — ver [product-backlog.md](../../product-backlog/product-backlog.md).

## Tasks

- Ler e transcrever os 8 fluxogramas do board Miro (Atendimento → Cadastro → Diagnóstico → Orçamento → Aprovação → Execução → Teste → Entrega).
- Comparar os requisitos atuais com o recado do professor (Minimundo, regras de negócio, RF/RNF sobrepostos, CRUD, "muitas tabelas").
- Levantar o backlog mestre das 9 etapas do PIM II, cruzando com o Trello e a orientação recebida.
- Decidir remover o controle de estoque do escopo.
- Decidir o modelo de dados final: 4 tabelas (`users` com `role`, `equipamentos`, `os`, `tecnicos_rel`).
- Decidir que uma OS pode ter um ou mais técnicos, e que o Administrador cobre a ausência de Recepcionista/Técnico (RN11).
- Reescrever `requisitos.md`: RF, RNF, RN e Matriz de Permissões.
- Reclassificar a validação de integridade dos dados de RF para RNF (RNF09), por ser uma preocupação transversal a todas as operações.
- Separar os Casos de Uso Detalhados em arquivo próprio (`casos-de-uso.md`), com exceções específicas por etapa do fluxo (podendo haver mais de uma por caso de uso).
- Criar o DER simplificado (`Modelagem_DB/der-simplificado.md`).
- Sincronizar `product-backlog.md` e `levantamento-de-dados.md` com os novos requisitos.
- Criar a planilha de requisitos (`requisitos.xlsx`), separada por módulo.

## Sprint Review

Requisitos, backlog e modelo de dados totalmente reescritos e sincronizados entre si. Escopo de estoque fechado. Matriz de permissões única criada, eliminando as repetições identificadas. Planilha de requisitos publicada para facilitar a leitura pela equipe.

## Retrospective

### Funcionou bem

- Usar o fluxo real do Miro como base para os RF evitou requisitos genéricos que não batiam com o processo de verdade.
- Fechar decisões (estoque, tabelas, múltiplos técnicos) antes de reescrever o texto evitou retrabalho.

### Problemas

- Passaram-se sprints de trabalho sem nenhuma documentada em tempo real — a defasagem só foi percebida nesta sprint.

### Melhorias

- A partir de agora, fechar Review + Retrospective + Planning toda sexta-feira, sem acumular.

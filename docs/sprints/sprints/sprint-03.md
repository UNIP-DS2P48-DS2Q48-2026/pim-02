# Sprint 03

**Status:** Concluída - reconstruída (o trabalho foi feito na semana e publicado no repositório em 19/09).

**Período:** 12/09/2026 a 18/09/2026.

## Sprint Goal

Confrontar os requisitos existentes com o recado do professor e com o fluxo real do processo (Miro), simplificar o modelo de dados e fechar as decisões de escopo pendentes.

## Product Backlog Items

PBI01 a PBI12 (reescritos nesta sprint) - ver [Product Backlog](../../product-backlog/product-backlog.md).

## Tasks

- [x] Ler e transcrever os 8 fluxogramas do board Miro (Atendimento → Cadastro → Diagnóstico → Orçamento → Aprovação → Execução → Teste → Entrega).
- [x] Comparar os requisitos atuais com o recado do professor (Minimundo, regras de negócio, RF/RNF sobrepostos, CRUD, "muitas tabelas").
- [x] Levantar o backlog das 9 etapas do PIM II, cruzando com o Trello e a orientação recebida.
- [x] Decidir remover o controle de estoque do escopo.
- [x] Decidir o modelo de dados final: 4 tabelas (`usuario` com `perfil`, `equipamento`, `os`, `tecnico_os`).
- [x] Decidir que uma OS pode ter um ou mais técnicos, e que o Administrador cobre a ausência de Recepcionista/Técnico (RN11).
- [x] Reescrever `requisitos.md`: RF, RNF, RN e Matriz de Permissões.
- [x] Reclassificar a validação de integridade dos dados de RF para RNF (RNF09), por ser uma preocupação transversal a todas as operações.
- [x] Criar o DER simplificado (`Modelagem_DB/der-simplificado.md`).
- [x] Sincronizar `product-backlog.md` e `levantamento-de-dados.md` com os novos requisitos.
- [x] Criar a planilha de requisitos (`requisitos.xlsx`), separada por módulo.

## Sprint Review

Requisitos, backlog e modelo de dados reescritos e sincronizados entre si. Escopo de estoque fechado. Matriz de permissões única criada, eliminando as repetições identificadas. Planilha de requisitos publicada para facilitar a leitura pela equipe.

## Retrospective

### Funcionou bem

- Usar o fluxo real do Miro como base para os RF evitou requisitos genéricos que não batiam com o processo de verdade.
- Fechar decisões (estoque, tabelas, múltiplos técnicos) antes de reescrever o texto evitou retrabalho.

### Problemas

- Passaram-se sprints de trabalho sem nenhuma documentada em tempo real - a defasagem só foi percebida nesta sprint.
- A implementação em C, prevista para começar nesta semana, foi adiada: era preciso fechar os requisitos primeiro.

### Melhorias

- Fechar Review + Retrospective + Planning toda sexta-feira, sem acumular.

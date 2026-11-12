# Sprint 09

**Status:** Concluída - fechada em 11/11/2026, a partir do histórico do Git. O período planejado terminava na sexta-feira, 30/10, mas a integração com o SQL Server e o documento final do PIM só foram concluídos depois, e a sprint foi estendida até a entrega.

**Período:** 24/10/2026 a 30/10/2026 (planejado), estendida até 11/11/2026.

## Sprint Goal

Fechar a documentação: o documento do PIM completo no formato do manual e os ajustes finais para a entrega e a apresentação.

## Product Backlog Items

Todos os PBIs do código estão concluídos (ver o [Product Backlog](../../product-backlog/product-backlog.md)). Esta sprint é de documentação, integração final com o banco e acabamento.

## Tasks

As tarefas marcadas com *(Sprint 08)* vieram da sprint anterior (ver a [Sprint Review da Sprint 08](./sprint-08.md#sprint-review)).

### Documento do PIM

- [x] Juntar a parte inicial (12/10) e a continuação (18/10) num texto só, sem seções repetidas - documento final (11/11).
- [x] Alinhar o texto com o SQL Server: a linha do RNF04, o quadro de persistência e o parágrafo sobre os arquivos de texto. *(Sprint 08)* - documento final (11/11).
- [x] Etapa 3: diagrama de sequência da criação da OS (Figura 2 do documento final). *(Sprint 08)*
- [x] Etapa 4: normalização, comandos SQL e NoSQL (seção 2.6 do documento final). *(Sprint 08)*
- [x] Etapa 5: implementação em C com SQL Server, pseudocódigo e fluxograma (seção 2.7 do documento final). *(Sprint 08)*
- [x] Etapas 6 a 8: redes, direitos humanos, integração e Conclusão revisadas, com o SQL Server descrito como parte do sistema e não como evolução futura (seções 2.8 a 2.10 e 3). *(Sprint 08)*
- [x] Planejamento ágil com as Sprints 01 a 09 (Quadro 6 do documento final). *(Sprint 08)*
- [x] Desenvolvimento entre 20 e 30 páginas: o documento final tem 29. *(Sprint 08)*
- [x] Conferir as referências e as citações no texto - 11/11.
- [x] Passar o texto para o formato do manual (`docs/manual-pim/`) e gerar o PDF final - 11/11.
- [x] Revisar o documento inteiro com o grupo antes da entrega - 11/11.

### Código e banco de dados

Trabalho que não estava no planejamento da sprint, feito para fechar a integração com o SQL Server.

- [x] Refatoração da conexão do programa com o SQL Server - 27/10.
- [x] Integração com o SQL Server finalizada: senhas protegidas em `senhas.c`, gravação e leitura em `usuarios_sql.c` e `persistencia_sql.c`, programa de teste da conexão (`teste_banco.c`) e instalador - 02/11.
- [x] Merge da integração na `main`, adaptado para Windows: Reativar (UC45 a UC49) de volta, mensagens sem os códigos de regra (RN/RF), README com a conexão e o uso no Linux, `schema_sqlserver.sql` da `docs` como script oficial do banco e OS seguindo as regras dos casos de uso - 02/11 (PR #41).
- [x] Guia de modelagem (`docs/guia-modelagem/`) e limpeza dos arquivos que não eram mais usados (`schema.sql`, `schema_mysql.sql`, testes de estresse dos triggers e o `Kit_ByteTech_Git`) - 07/11 (PR #42).

### Ajustes finais e apresentação

- [x] Protótipos das telas no Figma exportados em PNG para `docs/telas-figma/`: login, página inicial, menus e telas de cada perfil (Admin, Recepcionista, Técnico e Cliente) e o mapa de navegação, 34 imagens - 26/10 (PR #39). *(Sprint 08)*
- [x] Decidir se os PDFs das aulas (Unidades 1 a 6) entram no repositório: ficaram fora. *(Sprint 08)*
- [x] Roteiro da apresentação: quem fala de cada parte e o caminho de uma OS para mostrar no terminal. *(Sprint 08)*
- [x] Ensaio da apresentação com o programa rodando e o banco com os dados de exemplo.
- [x] Tirar a integrante que saiu do grupo da lista do README - 11/11.

## Decisões desta sprint

### 02/11 - Relatório por período

Na integração com o SQL Server, o relatório do UC43 mudou. A versão de 20/10 contava as OS de um ano numa matriz de 12 meses por 4 situações (`int contagem[12][4]`, em `relatorio.c`). A versão integrada em 02/11 pede uma data inicial e uma final e mostra o total de OS do período, quantas estão em cada situação, quantos equipamentos passaram por manutenção e quantos clientes ficaram satisfeitos. Com isso, a matriz saiu do código. A descrição do UC43 em `descricoes-astah.md` foi atualizada para o relatório por período.

## Sprint Review

O documento do PIM foi concluído no formato do manual, com as Etapas 1 a 8, o Desenvolvimento em 29 páginas e o planejamento das Sprints 01 a 09, e revisado pelo grupo em 11/11. A integração com o SQL Server, que tinha começado na Sprint 08, foi finalizada e entrou na `main` em 02/11 (PR #41), com as senhas protegidas por hash e o instalador. O repositório ganhou o guia de modelagem e perdeu os scripts antigos (PR #42). As telas do Figma entraram em 26/10 (PR #39), e o roteiro e o ensaio da apresentação foram feitos.

A sprint não fechou na sexta-feira, 30/10: a integração e o documento final só terminaram em novembro, e a sprint foi estendida até 11/11.

## Retrospective

### Funcionou bem

- Dividir o documento do PIM pelas etapas do manual deixou fácil conferir se nenhuma etapa ficou de fora.
- A integração com o SQL Server foi feita numa branch própria e só entrou na `main` depois de adaptada para Windows.
- A revisão do documento final contra o manual, antes da entrega, encontrou problemas de formatação, de ABNT e de texto a tempo de corrigir.

### Problemas

- A sprint passou da data prevista: o fim da integração com o banco e o documento final ficaram para novembro.
- No merge da integração (02/11), o relatório com a matriz foi trocado pelo relatório por período, e o documento final passou a dizer que a matriz estava pendente.
- Parte dos registros foi feita depois, a partir do histórico do Git, e não na reunião de sexta-feira.

### Melhorias

- Em um próximo projeto, conferir depois de cada merge grande se os documentos (casos de uso, sprints e texto acadêmico) continuam descrevendo o código.
- Fechar a sprint na data, mesmo incompleta, e abrir uma nova para o que sobrar.

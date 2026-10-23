# Sprint 09

**Status:** Planejada - começa no sábado, 24/10/2026, e fecha na sexta-feira, 30/10/2026.

**Período:** 24/10/2026 a 30/10/2026 (sábado a sexta-feira).

## Sprint Goal

Fechar a documentação: o documento do PIM completo no formato do manual, o documento do código, o manual de uso do sistema e os ajustes finais para a entrega e a apresentação.

## Product Backlog Items

Todos os PBIs do código estão concluídos (ver o [Product Backlog](../../product-backlog/product-backlog.md)). Esta sprint é de documentação e acabamento.

## Tasks

Os responsáveis são definidos na Sprint Planning. As tarefas marcadas com *(Sprint 08)* vêm da sprint anterior e só entram aqui se não forem concluídas até o fechamento dela.

### Documento do PIM (`docs/documentacao-pim/`)

- [ ] Juntar a parte inicial (12/10) e a continuação (18/10) num texto só, sem seções repetidas.
- [ ] Alinhar o texto com o SQL Server: a linha do RNF04, o Quadro 5 ("gravadas em usuario.txt...") e o parágrafo sobre os arquivos de texto. *(Sprint 08)*
- [ ] Etapa 3: diagrama de atividades ou de sequência. A figura da seção 3.5 precisa ser colocada no repositório (hoje aponta para `C:/Users/math-/...`). *(Sprint 08)*
- [ ] Etapa 4: revisar a seção 5 (normalização, comandos SQL e NoSQL). *(Sprint 08)*
- [ ] Etapa 5: escrever a 6.4 (o banco já está pronto) e atualizar a 6.7, que ainda fala em casos de uso vazios e gravação em texto. *(Sprint 08)*
- [ ] Etapas 6 a 8: revisar as seções 7 a 9 e a Conclusão; o SQL Server já não é evolução futura. *(Sprint 08)*
- [ ] Atualizar a seção 4 (Planejamento ágil) com as Sprints 05 a 09; o Quadro 6 vai só até a Sprint 05. *(Sprint 08)*
- [ ] Manter o Desenvolvimento entre 20 e 30 páginas: levar detalhes (as 49 descrições de casos de uso, as regras de integridade) para um apêndice. *(Sprint 08)*
- [ ] Conferir as referências e as citações no texto.
- [ ] Passar o texto para o formato do manual (`docs/manual-pim/`) e gerar o Word e o PDF finais.
- [ ] Revisar o documento inteiro com o grupo antes da entrega.

### Documento do código (novo, em `docs/documentacao-codigo/`)

- [ ] Organização do programa: um `.c` por pacote do Astah, as 4 structs, os vetores globais e o `banco.c`.
- [ ] Caminho do `main` até um caso de uso: conectar, carregar as tabelas, login, menu do perfil, função do caso de uso, gravar no banco.
- [ ] Funções de cada arquivo: o que recebem, o que devolvem e em que linha estão. Base: o [Guia do Código](https://claude.ai/artifact/577C2waDhk3jP7QM4gquXn) e os comentários `/* função está em arquivo.c, linha N */`.
- [ ] Onde cada regra de negócio (RN) está no código.
- [ ] Um caso de uso explicado passo a passo com trecho do código e pseudocódigo (sugestão: UC24 Criar Ordem de Serviço).
- [ ] Como os testes foram feitos (os cenários e o que cada um confere).

### Manual de uso do sistema (novo, em `docs/manual-de-uso/`)

- [ ] Instalação: SQL Server, driver ODBC 18, criação do banco com os dois scripts, compilação e execução.
- [ ] Primeiro acesso: os usuários de exemplo, a senha `byte123` e como trocar a senha.
- [ ] Uma seção por perfil (Administrador, Recepcionista, Técnico e Cliente), com o menu e o passo a passo de cada tela.
- [ ] O caminho completo de uma OS: abertura, atribuição do técnico, observação, diagnóstico, orçamento, aprovação, reparo, teste, entrega e feedback.
- [ ] Prints das telas do terminal.
- [ ] Mensagens de aviso mais comuns (`[!]`) e o que fazer em cada uma.

### Ajustes finais e organização

- [ ] Protótipos das telas no Figma, exportados em PNG, e o link no README. *(Sprint 08)*
- [ ] Colocar no README os links do documento do código e do manual de uso.
- [ ] Atualizar ou arquivar os materiais fora do repositório que descrevem a versão antiga do código (a documentação do código no Claude Docs, de 22/10, e o guia do banco ODBC, de 21/10).
- [ ] Apagar as pastas antigas de diagramas em `docs/Modalagem_casos_de_uso/` e manter só a mais nova. *(Sprint 08)*
- [ ] Colocar `*.asta.lock` no `.gitignore` e tirar do repositório o `diagrama_de_classes.asta.lock`. *(Sprint 08)*
- [ ] Apagar as branches já mescladas e decidir o que fazer com a `projeto+banco`. *(Sprint 08)*
- [ ] Decidir se os PDFs das aulas (Unidades 1 a 6) entram no repositório ou no `.gitignore`. *(Sprint 08)*
- [ ] Roteiro da apresentação: quem fala de cada parte e o caminho de uma OS para mostrar no terminal. *(Sprint 08)*
- [ ] Ensaio da apresentação com o programa rodando e o banco com os dados de exemplo.

Ao concluir uma tarefa, marque `[x]` e escreva a data no fim (ex.: `- 27/10`). O que não terminar até sexta passa para a próxima sprint com a nota `→ passou para a Sprint 10`.

## Sprint Review

*A ser preenchido na sexta-feira, 30/10.*

## Retrospective

### Funcionou bem

*A ser preenchido na sexta-feira, 30/10.*

### Problemas

*A ser preenchido na sexta-feira, 30/10.*

### Melhorias

*A ser preenchido na sexta-feira, 30/10.*

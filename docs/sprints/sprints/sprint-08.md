# Sprint 08

**Status:** Planejada - começa no sábado, 17/10/2026, e fecha na sexta-feira, 23/10/2026.

**Período:** 17/10/2026 a 23/10/2026.

## Sprint Goal

Escrever as etapas que faltam no documento do PIM, fazer os protótipos das telas no Figma e deixar a apresentação pronta.

## Product Backlog Items

Todos os PBIs do código estão concluídos (ver o [Product Backlog](../../product-backlog/product-backlog.md)). Esta sprint é de documentação, protótipos e acabamento.

## Tasks

### Código (`projeto/`)

- [ ] Abrir o PR da branch `refactor/codigo-simples` e fazer o merge na `main`.
- [ ] Grupo decidir: o Admin volta a poder editar OS finalizada (exceção da RN08)? Hoje o código não deixa, mas o UC26 (exceção 4a) e a RN08 ainda dizem que o Admin pode: ou volta o `if` no `editarOS`, ou muda o texto do UC26 e da RN08.
- [ ] Opcional: não apagar o texto do reparo quando o técnico registra uma nova observação; exigir o reparo registrado antes do teste.
- [ ] Próximo passo (planejar): ligar o programa ao SQL Server (`schema_sqlserver.sql`). Definir se entra antes ou depois da entrega do PIM.

### Documentação acadêmica

- [ ] Etapa 3: diagrama de atividades ou de sequência (os fluxogramas de `docs/Fluxograma/` podem servir de base).
- [ ] Etapa 4: normalização, principais comandos SQL e discussão sobre bancos NoSQL (os modelos e o `schema_sqlserver.sql` já existem).
- [ ] Etapa 5: seção sobre a implementação em C (arquivos, `.txt`, menus por perfil, como compilar e executar), com algoritmos, pseudocódigo e prints das telas do terminal.
- [ ] Etapa 6: redes e sistemas distribuídos (arquitetura, topologia, equipamentos, endereçamento IP, VLANs, nuvem, segurança).
- [ ] Etapa 7: direitos humanos, inclusão e segurança (acessibilidade, privacidade, proteção de dados, ética).
- [ ] Etapa 8: integração da solução, limitações, evolução futura (entre elas, o SQL Server) e conclusão.
- [ ] Atualizar a seção 4 (Planejamento ágil) com as Sprints 05 a 08.
- [ ] Manter o Desenvolvimento entre 20 e 30 páginas: levar detalhes (as 44 descrições de casos de uso, as regras de integridade) para um apêndice.
- [ ] Revisar o documento com o grupo e passar para o formato do manual (`docs/manual-pim/`).
- [ ] Corrigir o nome da pasta `docs/documentacao-pim/20206-10-12/` para `2026-10-12`.

### Protótipos (Figma)

- [ ] Criar o arquivo do projeto no Figma e definir o padrão das telas, igual ao terminal: cabeçalho BYTETECH com a data, 80 colunas e menus numerados.
- [ ] Prototipar o Login e os 4 menus principais (Administrador, Recepcionista, Técnico e Cliente).
- [ ] Prototipar as telas de cada pacote do Diagrama Estendido: Usuário, Equipamento, Ordem de Serviço, Manutenção, Orçamento, Histórico e Relatório, Meus Dados.
- [ ] Ligar as telas no modo protótipo do Figma, seguindo o caminho de uma OS: abertura, diagnóstico, orçamento, reparo, teste, entrega e feedback.
- [ ] Exportar os protótipos em PNG para `docs/` e colocar o link do Figma no README.

### Organização e apresentação

- [ ] Apagar as pastas antigas de diagramas em `docs/Modalagem_casos_de_uso/` e manter só a de 16/10 (o histórico fica no Git, como combinado na Sprint 04).
- [ ] Decidir se os PDFs das aulas (Unidades 1 a 6) entram no repositório ou no `.gitignore`.
- [ ] Montar o roteiro da apresentação: quem fala de cada parte e o caminho de uma OS para mostrar no terminal.

## Sprint Review

*A ser preenchido na sexta-feira, 23/10.*

## Retrospective

### Funcionou bem

*A ser preenchido na sexta-feira, 23/10.*

### Problemas

*A ser preenchido na sexta-feira, 23/10.*

### Melhorias

*A ser preenchido na sexta-feira, 23/10.*

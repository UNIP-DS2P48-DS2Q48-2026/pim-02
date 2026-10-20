# Sprint 08

**Status:** Planejada - começa no sábado, 17/10/2026, e fecha na sexta-feira, 23/10/2026.

**Período:** 17/10/2026 a 23/10/2026.

## Sprint Goal

Escrever as etapas que faltam no documento do PIM, fazer os protótipos das telas no Figma e deixar a apresentação pronta.

## Product Backlog Items

Todos os PBIs do código estão concluídos (ver o [Product Backlog](../../product-backlog/product-backlog.md)). Esta sprint é de documentação, protótipos e acabamento.

## Tasks

### Código (`projeto/`)

- [x] UC24 a UC30 de volta, depois do PR #29, e código simplificado de novo, seguindo os PDFs das aulas (Unidades 1 a 6): branch `fix/volta-uc24-30` - 20/10.
- [ ] Commit e PR da branch `fix/volta-uc24-30`.
- [x] Admin edita OS finalizada, como no diagrama de casos de uso (o Admin generaliza Recepcionista e Técnico, RN11) e no UC26 (exceção 4a, RN08, RI09): `selecionarOS(1)` inclui as finalizadas só no `editarOS` do Admin - 20/10.
- [x] Desfazer as cadeias de `else if` grudadas: `switch` nas opções numéricas (diagnóstico, teste, menu inicial; Unidade 3) e `if` com chaves, um por caso e com comentário, onde a comparação é de texto (menu pelo perfil, coluna do relatório, `etapaDaOS`) - 20/10.
- [x] Revisão do projeto inteiro contra os diagramas, as regras e as descrições dos casos de uso (ver "Decisões desta sprint") - 20/10.
- [x] Reativar usuário e equipamento (RF02, RF03): 5 casos de uso novos, UC45 a UC49 ("Reativar Admin", "Reativar Recepcionista", "Reativar Técnico", "Reativar Cliente" e "Reativar Equipamento"), no código, nas descrições e no Product Backlog - 20/10.
- [ ] Acrescentar no Astah as 5 elipses novas (uma em cada pacote, ao lado do Inativar), as operações no Diagrama Estendido e exportar de novo os PNGs.
- [x] Não apagar o texto do reparo quando o técnico registra uma nova observação; exigir o reparo registrado antes do teste (`osTemReparo` no `manutencao.c`) - 20/10.
- [ ] Próximo passo (planejar): ligar o programa ao SQL Server (`schema_sqlserver.sql`). Definir se entra antes ou depois da entrega do PIM.

### Documentação acadêmica

- [ ] Etapa 3: diagrama de atividades ou de sequência (os fluxogramas de `docs/Fluxograma/` podem servir de base).
- [ ] Etapa 4: normalização, principais comandos SQL e discussão sobre bancos NoSQL (os modelos, o `schema_sqlserver.sql` e o dicionário de dados já existem).
- [ ] Etapa 5: seção sobre a implementação em C (arquivos, `.txt`, menus por perfil, como compilar e executar), com algoritmos, pseudocódigo e prints das telas do terminal.
- [ ] Etapa 6: redes e sistemas distribuídos (arquitetura, topologia, equipamentos, endereçamento IP, VLANs, nuvem, segurança).
- [ ] Etapa 7: direitos humanos, inclusão e segurança (acessibilidade, privacidade, proteção de dados, ética).
- [ ] Etapa 8: integração da solução, limitações, evolução futura (entre elas, o SQL Server) e conclusão.
- [ ] Atualizar a seção 4 (Planejamento ágil) com as Sprints 05 a 08.
- [ ] Manter o Desenvolvimento entre 20 e 30 páginas: levar detalhes (as 44 descrições de casos de uso, as regras de integridade) para um apêndice.
- [ ] Revisar o documento com o grupo e passar para o formato do manual (`docs/manual-pim/`).
- [x] Corrigir o nome da pasta `docs/documentacao-pim/20206-10-12/` para `2026-10-12` - 18/10.
- [x] Conferir o título do Diagrama Estendido no Astah: está "byteTech - Diagrama Estendido" - 20/10.

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

## Decisões desta sprint

### 20/10 - UC24 a UC30 de volta, com o código simplificado

**O que aconteceu.** Na noite de 16/10, o PR #29 trocou a pasta `projeto/` pela versão do commit `19d76d2` (15/10). Com isso, os UC24 a UC30 voltaram a ser funções vazias, e a simplificação da Sprint 07 saiu do código.

**O que foi feito.** Primeiro voltaram os UC24 a UC30, do commit `e1080f0`, que foi feito em cima do próprio `19d76d2`. Depois, como o grupo quer o código no nível das aulas, voltou a simplificação da Sprint 07 (commit `8f426a8`), que saiu desse mesmo código. Ficou assim:

- **Só arquivos `.txt`.** O programa lê `projeto/dados/*.txt` com `fopen`/`fgets` ao abrir e regrava o arquivo com `fprintf` a cada mudança (Unidade 4). O `banco.c` e a chave `USAR_SQLSERVER` saíram de novo; o SQL Server fica como evolução futura.
- **Regras das OS com `if`** em cada caso de uso, sem `osNaEtapa` e sem as constantes `ETAPA_*`.
- **Relatório com a matriz** `int contagem[12][4]` (Unidade 6).
- **Duas correções novas no `manutencao.c`:** a observação nova não apaga mais o reparo (entra no fim do texto), e o teste só é aceito depois do reparo registrado (`osTemReparo`, que usa o `contemTexto` que já existia).

- **Admin segue os diagramas:** ele herda tudo do Recepcionista e do Técnico (RN11), e também edita OS finalizada (UC26, exceção 4a).
- **Sem `else if` grudado.** Onde a escolha é por número, entrou `switch` (Unidade 3). Em C o `switch` não compara textos, então o menu pelo perfil, a coluna do relatório e a `etapaDaOS` ficaram com um `if` com chaves para cada caso, comparando a palavra inteira com `strcmp` e com um comentário dizendo qual caso de uso leva àquela etapa.

**O que muda para quem usa o sistema:** igual à Sprint 07 (CPF digitado como foi cadastrado, Atribuir Técnico em seguida à abertura, relatório por ano, feedback com o comentário primeiro), mais as correções acima.

**Resultado.** 3.357 linhas, compila sem avisos (`gcc -Wall -Wextra main.c`). Testado numa cópia de `dados/`: OS nova da abertura ao feedback (abertura, atribuição, observação, diagnóstico, orçamento, aprovação, teste bloqueado antes do reparo, reparo, teste, entrega, feedback), cancelar, editar, relatório, login de usuário inativo e inativação bloqueada pela RN14.

### 20/10 - Revisão do projeto contra os diagramas e as regras

O código foi conferido com a Matriz de Permissões, as RN, as RI, as 44 descrições de casos de uso e as operações do Diagrama Estendido. O que não batia e foi corrigido:

- **Listas de OS por etapa, como o passo 2 dos casos de uso.** O `selecionarOS` recebe o nome da etapa (a mesma da `etapaDaOS`) e mostra só as OS dela: Registrar Orçamento ("aguardando orcamento"), Aprovar e Recusar ("aguardando resposta", exceção 2a), Editar e Arquivar Orçamento ("orcamento recusado"), Registrar Teste ("em reparo") e OS Prontas para Entrega ("pronta para entrega"). Com isso, os `if` que conferiam a etapa depois da escolha saíram. As telas cujo passo 2 só pede o número da OS ou mostra "as OS atribuídas ao técnico" (UC26 a UC33) continuam listando todas em aberto, com o `if` da exceção 4a. Isso muda a decisão de 16/10 (Sprint 07), que listava todas em aberto em todas as telas.
- **Busca sem resultado pede o filtro de novo** (UC25 e UC42, exceção 4a), com `do...while`.
- **Histórico (UC42) dentro do escopo do perfil:** o técnico só vê as OS do equipamento em que trabalhou (RN10).
- **OS Prontas para Entrega (UC35):** lista as OS, o atendente escolhe uma e o sistema mostra o contato do cliente (passos 2 a 4).
- **Nomes iguais aos do Diagrama Estendido:** `consultarHistorico`, `gerarRelatorio` e `listarProntasParaEntrega`.
- **Matriz de Permissões:** a Recepcionista consulta todas as OS, como foi decidido em 16/10 (nota ² no `requisitos.md`).
- **Textos em português do Brasil** ("Encerrando a sessao", "Encerrando o sistema").

### 20/10 - Reativar como casos de uso novos

**Por quê.** O RF02 e o RF03 falam em "ativar/inativar" usuários e equipamentos, mas não havia como reativar um cadastro inativado por engano ou um cliente que voltou.

**O que foi decidido.** Reativar virou caso de uso próprio, um em cada pacote: UC45 Reativar Admin, UC46 Reativar Recepcionista, UC47 Reativar Técnico, UC48 Reativar Cliente e UC49 Reativar Equipamento. O sistema passa de 44 para 49 casos de uso. Juntar com o Inativar numa tela só ("Ativar/Inativar") foi testado e descartado: o caso de uso ficava com dois objetivos e o nome ficava estranho; separados, cada um tem um fluxo só. Quem reativa é quem já podia inativar: o Admin para a equipe (RN05), a Recepcionista e o Admin para cliente e equipamento.

**Como funciona.** "5 - Reativar" no menu de cada cadastro. A lista mostra só os inativos, o sistema pede a confirmação e volta o `status` para `ativo`. O equipamento só é reativado se o cliente dono estiver ativo (exceção 4a do UC49, RN01 e RN14). No código: `reativarUsuario` no `geral.c` e `reativarEquipamento` no `equipamento.c`; o `selecionarUsuario` e o `selecionarEquipamento` recebem o status que querem listar ("ativo", "inativo" ou "" para todos).

## Sprint Review

*A ser preenchido na sexta-feira, 23/10.*

## Retrospective

### Funcionou bem

*A ser preenchido na sexta-feira, 23/10.*

### Problemas

*A ser preenchido na sexta-feira, 23/10.*

### Melhorias

*A ser preenchido na sexta-feira, 23/10.*

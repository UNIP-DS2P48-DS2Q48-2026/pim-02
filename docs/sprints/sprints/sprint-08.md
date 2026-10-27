# Sprint 08

**Status:** Concluída - fechada em 26/10/2026, a partir do histórico do Git (a reunião de sexta-feira, 23/10, não registrou o fechamento).

**Período:** 17/10/2026 a 23/10/2026.

## Sprint Goal

Escrever as etapas que faltam no documento do PIM, fazer os protótipos das telas no Figma e deixar a apresentação pronta.

## Product Backlog Items

Todos os PBIs do código estão concluídos (ver o [Product Backlog](../../product-backlog/product-backlog.md)). Esta sprint é de documentação, protótipos e acabamento.

## Tasks

### Código (`projeto/`)

- [x] UC24 a UC30 de volta, depois do PR #29, e código simplificado de novo, seguindo os PDFs das aulas (Unidades 1 a 6): branch `fix/volta-uc24-30` - 20/10.
- [x] Commit e PR da branch `fix/volta-uc24-30` - 20/10 (PRs #30 e #31).
- [x] Admin edita OS finalizada, como no diagrama de casos de uso (o Admin generaliza Recepcionista e Técnico, RN11) e no UC26 (exceção 4a, RN08, RI09): `selecionarOS(1)` inclui as finalizadas só no `editarOS` do Admin - 20/10.
- [x] Desfazer as cadeias de `else if` grudadas: `switch` nas opções numéricas (diagnóstico, teste, menu inicial; Unidade 3) e `if` com chaves, um por caso e com comentário, onde a comparação é de texto (menu pelo perfil, coluna do relatório, `etapaDaOS`) - 20/10. A coluna do relatório e a `etapaDaOS` foram revistas de novo em 23/10.
- [x] Revisão do projeto inteiro contra os diagramas, as regras e as descrições dos casos de uso (ver "Decisões desta sprint") - 20/10.
- [x] Vistoria final do código seguindo as descrições dos casos de uso - 20/10 (PR #32).
- [x] Reativar usuário e equipamento (RF02, RF03): 5 casos de uso novos, UC45 a UC49 ("Reativar Admin", "Reativar Recepcionista", "Reativar Técnico", "Reativar Cliente" e "Reativar Equipamento"), no código, nas descrições e no Product Backlog - 20/10.
- [x] Acrescentar no Astah as 5 elipses novas (uma em cada pacote, ao lado do Inativar), as operações no Diagrama Estendido e exportar de novo os PNGs - 20/10 (pasta `2026-10-20-...`; nova exportação em 23/10, pasta `2026-10-23-...`).
- [x] Não apagar o texto do reparo quando o técnico registra uma nova observação; exigir o reparo registrado antes do teste - 20/10.
- [x] Ligar o programa ao SQL Server (`schema_sqlserver.sql`) pela API ODBC, no `banco.c` - 21/10 (PR #33).
- [x] Simplificar o código para a apresentação: menos funções de apoio, telas sem formatação e só SQL Server (o modo `.txt` e a pasta `projeto/dados` saíram) - 22/10.
- [x] Nomes claros nas variáveis (`posicaoUsuario`, `posicaoOS`, `posicaoCliente`... no lugar de `i`, `c`, `k`, `n`) e condições longas quebradas em vários `if` - 23/10 (PR #34).
- [x] Comentários curtos e diretos em cada função e, acima de cada chamada, onde a função está (`/* buscarUsuarioPorId está em geral.c, linha 147 */`) - 23/10 (PRs #34 e #36).
- [x] Data de hoje pedida ao SQL Server (`GETDATE()`), sem o `time.h`; `bdConsultar` unido ao `bdExecutar` no `banco.c` - 23/10 (PR #36).
- [x] Confirmação ao sair (UC02): "Tem certeza que deseja sair?"; com N, o usuário volta ao menu - 23/10 (PR #35).
- [x] Descrição do UC02 atualizada (passos 2 e 3, exceção 3a) no `descricoes-astah.md` e no Astah - 23/10.
- [x] Teste com 31 cenários antes e depois de cada mudança (telas e tabelas iguais) e compilação sem avisos (`gcc -Wall -Wextra main.c`) - 23/10.
- [x] Tirar dos documentos a frase "os dados ficam em `.txt`; o SQL Server é o próximo passo" (README, requisitos, DER, Product Backlog) e corrigir os links do README - 23/10 (PR #37).

### Documentação acadêmica

- [ ] Etapa 3: diagrama de atividades ou de sequência (os fluxogramas de `docs/Fluxograma/` podem servir de base). A continuação de 18/10 já tem a seção 3.5 (sequência da criação da OS), mas a imagem aponta para uma pasta do computador de um integrante (`C:/Users/math-/...`) e não aparece no repositório. → passou para a Sprint 09.
- [ ] Etapa 4: normalização, principais comandos SQL e discussão sobre bancos NoSQL. Rascunho na continuação de 18/10 (seção 5); falta revisar. → passou para a Sprint 09.
- [ ] Etapa 5: seção sobre a implementação em C (arquivos, SQL Server via ODBC, menus por perfil, como compilar e executar), com algoritmos, pseudocódigo e prints das telas do terminal. Rascunho na continuação de 18/10 (seção 6); a 6.4 ainda está "pendente terminar o banco" e a 6.7 descreve a versão antiga (casos de uso vazios e gravação em texto). → passou para a Sprint 09.
- [ ] Etapa 6: redes e sistemas distribuídos (arquitetura, topologia, equipamentos, endereçamento IP, VLANs, nuvem, segurança). Rascunho na continuação de 18/10 (seção 7); falta revisar. → passou para a Sprint 09.
- [ ] Etapa 7: direitos humanos, inclusão e segurança (acessibilidade, privacidade, proteção de dados, ética). Rascunho na continuação de 18/10 (seção 8); falta revisar. → passou para a Sprint 09.
- [ ] Etapa 8: integração da solução, limitações, evolução futura e conclusão. Rascunho na continuação de 18/10 (seção 9 e Conclusão); o SQL Server já não é evolução futura. → passou para a Sprint 09.
- [ ] Atualizar a seção 4 (Planejamento ágil) com as Sprints 05 a 08: o Quadro 6 da continuação vai só até a Sprint 05. → passou para a Sprint 09.
- [ ] Alinhar a parte inicial (`2026-10-12/documentacao_pim_2_inicio.md`) com o SQL Server: a linha do RNF04, o Quadro 5 ("gravadas em usuario.txt...") e o parágrafo sobre os arquivos de texto. → passou para a Sprint 09.
- [ ] Manter o Desenvolvimento entre 20 e 30 páginas: levar detalhes (as 49 descrições de casos de uso, as regras de integridade) para um apêndice. → passou para a Sprint 09.
- [ ] Revisar o documento com o grupo e passar para o formato do manual (`docs/manual-pim/`). → passou para a Sprint 09.
- [x] Corrigir o nome da pasta `docs/documentacao-pim/20206-10-12/` para `2026-10-12` - 18/10.
- [x] Conferir o título do Diagrama Estendido no Astah: está "byteTech - Diagrama Estendido" - 20/10.

### Protótipos (Figma)

- [ ] Criar o arquivo do projeto no Figma e definir o padrão das telas, igual ao terminal: cabeçalho BYTETECH, 80 colunas e menus numerados. → passou para a Sprint 09. As telas entraram no repositório em 26/10 (PR #39), num estilo de página web, e não no do terminal.
- [ ] Prototipar o Login e os 4 menus principais (Administrador, Recepcionista, Técnico e Cliente). → passou para a Sprint 09.
- [ ] Prototipar as telas de cada pacote do Diagrama Estendido: Usuário, Equipamento, Ordem de Serviço, Manutenção, Orçamento, Histórico e Relatório, Meus Dados. → passou para a Sprint 09.
- [ ] Ligar as telas no modo protótipo do Figma, seguindo o caminho de uma OS: abertura, diagnóstico, orçamento, reparo, teste, entrega e feedback. → passou para a Sprint 09.
- [ ] Exportar os protótipos em PNG para `docs/` e colocar o link do Figma no README. → passou para a Sprint 09.

### Organização e apresentação

- [ ] Apagar as pastas antigas de diagramas em `docs/Modalagem_casos_de_uso/` e manter só a mais nova, de 23/10 (o histórico fica no Git, como combinado na Sprint 04). → passou para a Sprint 09.
- [ ] Colocar `*.asta.lock` no `.gitignore` e tirar do repositório o `diagrama_de_classes.asta.lock`. → passou para a Sprint 09.
- [ ] Apagar as branches já mescladas (`refactor/nomes-claros`, `fix/logout`, `fix/comentarios`) e decidir o que fazer com a `projeto+banco` (feita sobre uma versão antiga do código). → passou para a Sprint 09.
- [ ] Decidir se os PDFs das aulas (Unidades 1 a 6) entram no repositório ou no `.gitignore`. → passou para a Sprint 09.
- [ ] Montar o roteiro da apresentação: quem fala de cada parte e o caminho de uma OS para mostrar no terminal. → passou para a Sprint 09.

## Decisões desta sprint

### 20/10 - UC24 a UC30 de volta, com o código simplificado

**O que aconteceu.** Na noite de 16/10, o PR #29 trocou a pasta `projeto/` pela versão do commit `19d76d2` (15/10). Com isso, os UC24 a UC30 voltaram a ser funções vazias, e a simplificação da Sprint 07 saiu do código.

**O que foi feito.** Primeiro voltaram os UC24 a UC30, do commit `e1080f0`, que foi feito em cima do próprio `19d76d2`. Depois, como o grupo quer o código no nível das aulas, voltou a simplificação da Sprint 07 (commit `8f426a8`), que saiu desse mesmo código. Ficou assim:

- **Só arquivos `.txt`.** O programa lê `projeto/dados/*.txt` com `fopen`/`fgets` ao abrir e regrava o arquivo com `fprintf` a cada mudança (Unidade 4). O `banco.c` e a chave `USAR_SQLSERVER` saíram de novo; o SQL Server fica como evolução futura. *(Mudou em 21/10: ver abaixo.)*
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

### 21/10 e 22/10 - Só SQL Server

**O que mudou.** O programa passou a gravar no SQL Server (banco `bytetech`) pela API ODBC, no `banco.c` (PR #33). Em 22/10 o modo `.txt` saiu de vez: a chave `USAR_SQLSERVER`, as funções de arquivo e a pasta `projeto/dados` foram removidas.

**Como funciona.** Ao abrir, o programa lê as 4 tabelas para os vetores. A cada mudança, grava o vetor com `UPDATE` (o que já existe) e `INSERT` (o que é novo). Sem o SQL Server rodando, o programa avisa e fecha.

### 22/10 e 23/10 - Código mais fácil de explicar na apresentação

**Por quê.** O grupo precisa explicar cada parte do código ao professor, e nomes como `i`, `c` e `k` e funções muito curtas atrapalhavam.

**O que foi feito.**

- **Nomes que dizem o que são:** `posicaoUsuario`, `posicaoOS`, `posicaoCliente`, `posicaoTecnico`, `novaPosicao`, `deuCerto`.
- **Comentários curtos e diretos**, um por função. Acima de cada chamada vai o arquivo e a linha onde a função está. Os números são recalculados quando o código muda.
- **Menos coisa fora das aulas:** a data vem do SQL Server (`GETDATE()`) em vez do `time.h`. O `banco.c` ficou com uma função só para rodar comandos (`bdExecutar`).
- **Funções novas para condições que se repetiam:** `usuarioEntraNaLista` e `equipamentoEntraNaLista`.

**Resultado.** Os mesmos 31 cenários de teste dão telas e tabelas iguais antes e depois de cada mudança. Compila sem avisos.

### 23/10 - Confirmação ao sair (UC02)

**O que mudou.** "0 - Sair" agora pergunta "Tem certeza que deseja sair?". Com S, a sessão é encerrada e o sistema volta à tela inicial; com N, o menu do perfil aparece de novo. A descrição do UC02 ganhou os passos 2 e 3 (pedir e dar a confirmação) e a exceção 3a (o usuário desiste). O diagrama de casos de uso não muda: a confirmação fica dentro do próprio UC02. O "0 - Encerrar o programa" da tela inicial continua sem confirmação.

## Sprint Review

A semana foi quase toda de código. Os UC24 a UC30 voltaram depois do PR #29, o código foi conferido com os diagramas, as regras e as descrições, e o sistema ganhou os casos de uso de reativar (UC45 a UC49), passando de 44 para 49. O programa passou a gravar no SQL Server pela API ODBC e o modo `.txt` saiu. O código ficou mais fácil de explicar: nomes claros, comentários com o arquivo e a linha de cada função, data vinda do banco e confirmação ao sair (UC02). Os 31 cenários de teste passam e o código compila sem avisos. Os diagramas foram exportados de novo (23/10) e os documentos deixaram de dizer que os dados ficam em `.txt`. Foram 8 PRs com merge na `main` (#30 a #37).

Não foram entregues e passaram para a [Sprint 09](./sprint-09.md): as etapas 3 a 8 do documento do PIM (a continuação de 18/10 não foi revisada nem alinhada ao SQL Server), os protótipos no Figma dentro da semana, a limpeza do repositório (pastas antigas de diagramas, `.asta.lock`, branches) e o roteiro da apresentação.

## Retrospective

### Funcionou bem

- Os 31 cenários de teste rodados antes e depois de cada mudança deram segurança para trocar o `.txt` pelo SQL Server e renomear variáveis sem quebrar nada.
- Uma branch e um PR para cada assunto (`fix/volta-uc24-30`, `feat/banco-sqlserver`, `refactor/nomes-claros`, `fix/logout`, `fix/comentarios`) deixou o histórico fácil de seguir.
- Registrar as decisões no arquivo da sprint, com o porquê, ajudou a explicar as mudanças ao grupo.

### Problemas

- O documento do PIM ficou parado desde 18/10: o código tomou a semana toda e a meta da sprint (documento, Figma e apresentação) não foi cumprida.
- O trabalho ficou concentrado nas noites de 20 a 23/10, e a sprint de novo não foi fechada na sexta-feira.
- O código mudou de base duas vezes na mesma semana (`.txt` e depois só SQL Server), e o documento do PIM e os materiais de fora do repositório ficaram descrevendo a versão antiga.
- O commit de 21/10 que faz o menu da recepcionista consultar direto no banco ficou só na branch `projeto+banco`, feita sobre uma versão antiga, e não chegou à `main`.

### Melhorias

- Proposta para a Sprint 09: só mexer no código para corrigir erro, e usar o tempo no documento do PIM, no manual de uso e na apresentação.
- Dividir as etapas do documento entre o grupo logo no sábado, com um responsável por etapa.
- Fechar a sprint na sexta-feira, mesmo incompleta, e enviar as mudanças pela branch antes de começar a próxima.

# Casos de Uso

Este documento detalha **cada elipse** do diagrama de casos de uso (`diagramas.asta`, nesta mesma pasta), organizado por pacote/diagrama, na mesma ordem em que os pacotes aparecem no Astah. Cada caso de uso é uma função independente do sistema, com seu próprio ator, pré-condição, fluxo e pós-condição - inclusive as pré-condições de sessão (usuário já autenticado) e de encadeamento com outros casos de uso (ex.: só é possível orçar depois que o diagnóstico foi registrado).

Quando o caso de uso tem uma etapa correspondente em algum dos [fluxogramas](../Fluxograma/), isso é indicado logo abaixo do título.

## Como ler os diagramas

O `diagramas.asta` é dividido em pacotes (um diagrama por área). Convenções usadas em todos eles:

- **Boneco (ator):** quem interage com o sistema - Admin, Recepcionista, Técnico ou Cliente.
- **Elipse:** um caso de uso - uma ação que o sistema oferece.
- **Seta simples (ator → elipse):** associação - esse ator pode executar esse caso de uso.
- **Seta com triângulo vazado (ator → ator):** generalização entre atores. Usada para representar a RN11 - o Administrador generaliza Recepcionista e/ou Técnico, ou seja, herda tudo o que esses perfis podem fazer, cobrindo a ausência de um deles.

## Convenções dos fluxos abaixo

- **Fluxo alternativo:** um caminho *recuperável* - o caso de uso continua e ainda pode terminar com sucesso (repetição, negociação).
- **Exceção:** uma condição que *bloqueia ou desvia* o caso de uso do seu término normal (pré-condição não satisfeita, dado obrigatório ausente, regra de negócio violada). Cada exceção indica a etapa exata do fluxo principal em que ocorre, e pode haver mais de uma por caso de uso.
- Toda pré-condição de autenticação remete ao **UC01 - Realizar Login**. Por padrão, cada caso de uso cita apenas o perfil exigido (ex.: "Usuário autenticado com perfil Admin (UC01)").

---

## 1. Geral (Generalizado)

Ações de sessão e autoatendimento, comuns a qualquer perfil autenticado (RF01). Não corresponde a nenhum fluxograma específico - é transversal, acontece antes/depois de qualquer um dos fluxos abaixo.

### UC01 - Realizar Login (RF01)

- **Ator(es):** Cliente, Técnico, Recepcionista, Admin.
- **Pré-condição:** Usuário previamente cadastrado no sistema, com `status` ativo (RN12).
- **Fluxo principal:**
  1. O usuário informa e-mail e senha.
  2. O sistema localiza o cadastro pelo e-mail (RN13, único).
  3. O sistema compara a senha informada com o hash armazenado (RNF10).
  4. O sistema libera as funcionalidades do perfil (`perfil`) conforme a Matriz de Permissões (RN10).
- **Fluxo alternativo:**
  - Na etapa 3: senha incorreta → o sistema informa o erro e permite nova tentativa, sem indicar se o e-mail existe ou não.
- **Exceções:**
  - Na etapa 2: e-mail não cadastrado → o sistema rejeita o login (mesma mensagem genérica da senha incorreta, para não expor quais e-mails existem).
  - Na etapa 4: usuário com `status` inativo → o sistema impede o login mesmo com credenciais corretas (RN14).
- **Pós-condição:** Sessão iniciada, com o perfil do usuário identificado.

### UC02 - Fazer Logout (RF01)

- **Ator(es):** Cliente, Técnico, Recepcionista, Admin.
- **Pré-condição:** Usuário autenticado (sessão ativa, UC01).
- **Fluxo principal:**
  1. O usuário solicita encerrar a sessão.
  2. O sistema encerra a sessão e retorna à tela de login.
- **Exceções:** nenhuma - operação sem regra de negócio associada.
- **Pós-condição:** Sessão encerrada.

### UC03 - Consultar Meus Dados (RF01, RF10)

- **Ator(es):** Cliente, Técnico, Recepcionista, Admin.
- **Pré-condição:** Usuário autenticado (UC01).
- **Fluxo principal:**
  1. O usuário solicita ver os próprios dados.
  2. O sistema exibe o cadastro do usuário autenticado (dados comuns e específicos do perfil, ex.: especialidade para o Técnico).
- **Exceções:** nenhuma - o usuário só acessa o próprio registro (RI10 impede ver dados de outro usuário por essa via).
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

---

## 2. Gerenciar Admin

Cadastro de administradores. Executado só pelo próprio Admin (RN05 - "Somente o Administrador poderá cadastrar, alterar e ativar/inativar usuários"). Não corresponde a um fluxograma - é cadastro puro, sem decisão de negócio.

### UC04 - Cadastrar Admin (RF02)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01).
- **Fluxo principal:**
  1. O Admin acessa o cadastro de administradores.
  2. O Admin informa nome, e-mail e senha do novo administrador.
  3. O sistema valida os campos obrigatórios e a unicidade do e-mail (RN13).
  4. O sistema grava o cadastro com `perfil = admin` e `status = ativo`.
- **Exceções:**
  - Na etapa 3: e-mail já cadastrado para outro usuário → o sistema rejeita o cadastro (RN13, RI13).
  - Na etapa 3: campo obrigatório vazio → o sistema impede o avanço (RNF09).
- **Pós-condição:** Novo administrador cadastrado, apto a autenticar-se (UC01).

### UC05 - Consultar Admin (RF02, RF10)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01).
- **Fluxo principal:**
  1. O Admin busca ou lista os administradores cadastrados.
  2. O sistema exibe os registros, incluindo os inativos (RN12 preserva o histórico).
- **Exceções:** nenhuma.
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC06 - Editar Admin (RF02)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01); administrador-alvo previamente cadastrado (UC04).
- **Fluxo principal:**
  1. O Admin seleciona um administrador cadastrado.
  2. O Admin altera os dados desejados (nome, e-mail, senha).
  3. O sistema valida os campos e, se o e-mail foi alterado, sua unicidade (RN13).
  4. O sistema grava a alteração e atualiza `atualizado_em`.
- **Exceções:**
  - Na etapa 3: novo e-mail já usado por outro usuário → o sistema rejeita a alteração (RN13, RI13).
- **Pós-condição:** Cadastro do administrador atualizado.

### UC07 - Inativar Admin (RF02, RN12)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01); administrador-alvo com `status` ativo.
- **Fluxo principal:**
  1. O Admin seleciona um administrador cadastrado.
  2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" em que esse administrador atue como atendente ou como técnico substituto (RN14).
  3. O sistema marca `status = inativo`, preservando o histórico das OS relacionadas (RN12).
- **Exceções:**
  - Na etapa 2: existe OS em andamento vinculada ao administrador → o sistema impede a inativação (RN14, RI14).
- **Pós-condição:** Administrador inativo - não pode mais autenticar-se (RN14) nem ser vinculado a uma nova OS.

---

## 3. Gerenciar Recepcionista

Mesmo padrão do pacote anterior, agora para `perfil = recepcionista`. Não corresponde a um fluxograma.

### UC08 - Cadastrar Recepcionista (RF02)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01).
- **Fluxo principal:**
  1. O Admin acessa o cadastro de recepcionistas.
  2. O Admin informa nome, e-mail e senha do novo recepcionista.
  3. O sistema valida os campos obrigatórios e a unicidade do e-mail (RN13).
  4. O sistema grava o cadastro com `perfil = recepcionista` e `status = ativo`.
- **Exceções:**
  - Na etapa 3: e-mail já cadastrado → o sistema rejeita o cadastro (RN13, RI13).
  - Na etapa 3: campo obrigatório vazio → o sistema impede o avanço (RNF09).
- **Pós-condição:** Novo recepcionista cadastrado, apto a autenticar-se (UC01) e a abrir OS (RF04).

### UC09 - Consultar Recepcionista (RF02, RF10)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01).
- **Fluxo principal:**
  1. O Admin busca ou lista os recepcionistas cadastrados.
  2. O sistema exibe os registros, incluindo os inativos.
- **Exceções:** nenhuma.
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC10 - Editar Recepcionista (RF02)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01); recepcionista-alvo previamente cadastrado (UC08).
- **Fluxo principal:**
  1. O Admin seleciona um recepcionista cadastrado.
  2. O Admin altera os dados desejados.
  3. O sistema valida os campos e, se o e-mail foi alterado, sua unicidade (RN13).
  4. O sistema grava a alteração e atualiza `atualizado_em`.
- **Exceções:**
  - Na etapa 3: novo e-mail já usado por outro usuário → o sistema rejeita a alteração (RN13, RI13).
- **Pós-condição:** Cadastro do recepcionista atualizado.

### UC11 - Inativar Recepcionista (RF02, RN12)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01); recepcionista-alvo com `status` ativo.
- **Fluxo principal:**
  1. O Admin seleciona um recepcionista cadastrado.
  2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" com esse recepcionista como atendente (`os.id_atendente`, RN14).
  3. O sistema marca `status = inativo` (RN12).
- **Exceções:**
  - Na etapa 2: existe OS em andamento atendida por ele → o sistema impede a inativação (RN14, RI14).
- **Pós-condição:** Recepcionista inativo - não pode mais autenticar-se nem abrir novas OS.

---

## 4. Gerenciar Técnico

Mesmo padrão, para `perfil = tecnico`, com o campo adicional `especialidade`. Não corresponde a um fluxograma.

### UC12 - Cadastrar Técnico (RF02)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01).
- **Fluxo principal:**
  1. O Admin acessa o cadastro de técnicos.
  2. O Admin informa nome, e-mail, senha e especialidade do novo técnico.
  3. O sistema valida os campos obrigatórios e a unicidade do e-mail (RN13).
  4. O sistema grava o cadastro com `perfil = tecnico` e `status = ativo`.
- **Exceções:**
  - Na etapa 3: e-mail já cadastrado → o sistema rejeita o cadastro (RN13, RI13).
  - Na etapa 3: campo obrigatório vazio (incluindo especialidade) → o sistema impede o avanço (RNF09).
- **Pós-condição:** Novo técnico cadastrado, apto a autenticar-se (UC01) e a ser atribuído a uma OS (RF07).

### UC13 - Consultar Técnico (RF02, RF10)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01).
- **Fluxo principal:**
  1. O Admin busca ou lista os técnicos cadastrados.
  2. O sistema exibe os registros, incluindo a especialidade e os inativos.
- **Exceções:** nenhuma.
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC14 - Editar Técnico (RF02)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01); técnico-alvo previamente cadastrado (UC12).
- **Fluxo principal:**
  1. O Admin seleciona um técnico cadastrado.
  2. O Admin altera os dados desejados, incluindo a especialidade.
  3. O sistema valida os campos e, se o e-mail foi alterado, sua unicidade (RN13).
  4. O sistema grava a alteração e atualiza `atualizado_em`.
- **Exceções:**
  - Na etapa 3: novo e-mail já usado por outro usuário → o sistema rejeita a alteração (RN13, RI13).
- **Pós-condição:** Cadastro do técnico atualizado.

### UC15 - Inativar Técnico (RF02, RN12)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01); técnico-alvo com `status` ativo.
- **Fluxo principal:**
  1. O Admin seleciona um técnico cadastrado.
  2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" em que esse técnico esteja atribuído (`tecnico_os`, RN14).
  3. O sistema marca `status = inativo` (RN12).
- **Exceções:**
  - Na etapa 2: existe OS em andamento atribuída a ele → o sistema impede a inativação (RN14, RI14).
- **Pós-condição:** Técnico inativo - não pode mais autenticar-se nem ser atribuído a novas OS.

---

## 5. Gerenciar Cliente (Generalizado)

Quem cadastra é o Recepcionista (Admin generaliza, RN11), já que o cliente costuma ser criado durante o próprio atendimento (RF04).

### UC16 - Cadastrar Cliente (RF02, RF04)

*Fluxograma: "Registro do Cliente", em Cadastro e Abertura de Serviço.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01).
- **Fluxo principal:**
  1. O atendente informa nome, e-mail, telefone, senha e CPF do cliente.
  2. O sistema valida os campos obrigatórios e a unicidade de e-mail e CPF (RN13).
  3. O sistema grava o cadastro com `perfil = cliente` e `status = ativo`.
- **Exceções:**
  - Na etapa 2: e-mail ou CPF já cadastrados → o sistema rejeita o cadastro (RN13, RI13).
  - Na etapa 2: campo obrigatório vazio → o sistema impede o avanço (RNF09).
- **Pós-condição:** Cliente cadastrado, apto a ter equipamentos vinculados (RF03) e OS abertas (RF04).

### UC17 - Consultar Cliente (RF10)

- **Ator(es):** Recepcionista (ou Admin, RN11); Técnico consulta apenas o cliente da OS que está executando.
- **Pré-condição:** Usuário autenticado com perfil Recepcionista, Admin ou Técnico (UC01).
- **Fluxo principal:**
  1. O ator busca ou lista clientes cadastrados.
  2. O sistema exibe os registros dentro do escopo do perfil (RN10).
- **Exceções:**
  - Na etapa 2: Técnico tenta consultar um cliente de uma OS não atribuída a ele → o sistema nega o acesso (RN10).
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC18 - Editar Cliente (RF02)

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); cliente-alvo previamente cadastrado (UC16).
- **Fluxo principal:**
  1. O atendente seleciona um cliente cadastrado.
  2. O atendente altera os dados desejados.
  3. O sistema valida os campos e, se e-mail/CPF foram alterados, sua unicidade (RN13).
  4. O sistema grava a alteração e atualiza `atualizado_em`.
- **Exceções:**
  - Na etapa 3: novo e-mail ou CPF já usados por outro usuário → o sistema rejeita a alteração (RN13, RI13).
- **Pós-condição:** Cadastro do cliente atualizado.

### UC19 - Inativar Cliente (RN12)

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); cliente-alvo com `status` ativo.
- **Fluxo principal:**
  1. O atendente seleciona um cliente cadastrado.
  2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" vinculada a um equipamento desse cliente (RN14).
  3. O sistema marca `status = inativo` (RN12).
- **Exceções:**
  - Na etapa 2: existe OS em andamento vinculada a um equipamento do cliente → o sistema impede a inativação (RN14, RI14).
- **Pós-condição:** Cliente inativo - não pode mais autenticar-se, ter novos equipamentos vinculados nem novas OS abertas.

---

## 6. Gerenciar Equipamento (Generalizado)

### UC20 - Cadastrar Equipamento (RF03, RN01)

*Fluxograma: "Registro do equipamento", em Cadastro e Abertura de Serviço.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); cliente proprietário previamente cadastrado e ativo (UC16).
- **Fluxo principal:**
  1. O atendente seleciona o cliente proprietário.
  2. O atendente informa categoria (hardware ou software), tipo, marca e modelo/número de série (ou chave de licença).
  3. O sistema valida os campos obrigatórios e vincula o equipamento ao cliente (RN01).
  4. O sistema grava o cadastro com `status = ativo`.
- **Exceções:**
  - Na etapa 1: cliente inativo → o sistema impede o cadastro do equipamento (RN14).
  - Na etapa 3: campo obrigatório vazio → o sistema impede o avanço (RNF09).
- **Pós-condição:** Equipamento cadastrado, apto a receber uma OS (RF04).

### UC21 - Consultar Equipamento (RF10)

*Fluxograma: "Tipo de equipamento", em Atendimento ao Cliente.*

- **Ator(es):** Recepcionista (ou Admin, RN11); Técnico consulta o equipamento da OS que está executando; Cliente consulta apenas os próprios.
- **Pré-condição:** Usuário autenticado (UC01).
- **Fluxo principal:**
  1. O ator busca ou lista equipamentos.
  2. O sistema exibe os registros dentro do escopo do perfil (RN10).
- **Exceções:**
  - Na etapa 2: Cliente tenta consultar equipamento de outro cliente → o sistema nega o acesso (RI10).
  - Na etapa 2: Técnico tenta consultar equipamento de uma OS não atribuída a ele → o sistema nega o acesso (RN10).
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC22 - Editar Equipamento (RF03)

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); equipamento-alvo previamente cadastrado (UC20).
- **Fluxo principal:**
  1. O atendente seleciona um equipamento cadastrado.
  2. O atendente altera os dados desejados (tipo, marca, modelo/número de série).
  3. O sistema valida os campos e grava a alteração, atualizando `atualizado_em`.
- **Exceções:**
  - Na etapa 3: campo obrigatório removido/vazio → o sistema impede o avanço (RNF09).
- **Pós-condição:** Cadastro do equipamento atualizado.

### UC23 - Inativar Equipamento (RN12)

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); equipamento-alvo com `status` ativo.
- **Fluxo principal:**
  1. O atendente seleciona um equipamento cadastrado.
  2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" vinculada a esse equipamento (RN14).
  3. O sistema marca `status = inativo` (RN12).
- **Exceções:**
  - Na etapa 2: existe OS em andamento vinculada ao equipamento → o sistema impede a inativação (RN14, RI14).
- **Pós-condição:** Equipamento inativo - não pode mais receber uma nova OS.

---

## 7. Gerenciar Ordem de Serviço

O núcleo do processo - liga o atendimento inicial à execução e ao encerramento da OS.

### UC24 - Criar Ordem de Serviço (RF04)

*Fluxograma: Cadastro e Abertura de Serviço.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); equipamento identificado (UC20).
- **Fluxo principal:**
  1. O atendente coleta os dados do cliente e do equipamento.
  2. O sistema verifica se o cliente já possui cadastro.
  3. O atendente registra o problema relatado.
  4. O sistema abre a OS com status "Aberta" e a encaminha para diagnóstico.
- **Fluxo alternativo:**
  - Na etapa 2: cliente ainda não possui cadastro → o sistema registra o cliente (UC16) antes de prosseguir para a etapa 3.
- **Exceções:**
  - Na etapa 1: equipamento informado sem cliente vinculado e sem dados para novo cadastro → o sistema rejeita a abertura (RN01).
  - Na etapa 1: cliente ou equipamento está inativo → o sistema impede a abertura de nova OS (RN14).
  - Na etapa 3: problema relatado não informado → o sistema impede o avanço para a etapa 4 (RNF09).
- **Pós-condição:** OS registrada com status "Aberta", pronta para a vistoria (UC31/UC32).

### UC25 - Consultar Ordem de Serviço (RF10)

- **Ator(es):** Recepcionista (as que atendeu), Técnico (as que executou), Cliente (só as próprias), Admin (todas).
- **Pré-condição:** Usuário autenticado (UC01).
- **Fluxo principal:**
  1. O ator busca ou lista ordens de serviço.
  2. O sistema exibe as OS dentro do escopo do perfil (RN10), incluindo finalizadas e canceladas.
- **Exceções:**
  - Na etapa 2: Cliente tenta consultar OS de outro cliente → o sistema nega o acesso (RI10).
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC26 - Editar Ordem de Serviço (RN07)

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); OS previamente criada (UC24), não finalizada.
- **Fluxo principal:**
  1. O atendente seleciona a OS.
  2. O atendente altera os campos administrativos (ex.: problema relatado, dados de contato usados no atendimento).
  3. O sistema grava a alteração e atualiza `atualizado_em`.
- **Exceções:**
  - Na etapa 2: tentativa de alterar campo técnico (diagnóstico, resultado do teste) → o sistema rejeita, pois esse campo pertence à função do Técnico (RN07, RI08).
  - Na etapa 1: OS com status "finalizada" → o sistema impede a alteração, exceto pelo Admin (RN08, RI09).
- **Pós-condição:** Dados administrativos da OS atualizados.

### UC27 - Cancelar Ordem de Serviço (RN09)

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); OS previamente criada (UC24), não finalizada.
- **Fluxo principal:**
  1. O atendente seleciona a OS a ser cancelada.
  2. O sistema marca a OS com status "cancelada".
  3. O sistema preserva o registro para fins de histórico (RN09).
- **Exceções:**
  - Na etapa 1: OS já finalizada → o sistema impede o cancelamento (RN08).
- **Pós-condição:** OS cancelada, mantida no histórico (não pode ser excluída fisicamente, RI11).

### UC28 - Atribuir Técnico à Ordem de Serviço (RF07, RN04)

*Fluxograma: "Direcionar para técnico disponível", em Diagnóstico Técnico.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); OS previamente criada (UC24); técnico(s) cadastrado(s) e ativo(s) (UC12).
- **Fluxo principal:**
  1. O atendente seleciona a OS e um ou mais técnicos disponíveis.
  2. O sistema grava um registro em `tecnico_os` para cada técnico atribuído (RN04).
- **Fluxo alternativo:**
  - Na etapa 1: mais de um técnico é necessário para o serviço → repete-se a atribuição, uma linha por técnico, todas vinculadas à mesma OS (RN04).
- **Exceções:**
  - Na etapa 1: técnico selecionado está inativo → o sistema impede a atribuição (RN14).
  - Na etapa 2: usuário selecionado não tem `perfil` técnico nem admin → o sistema rejeita o registro (RN15, RI15, reforçado por trigger).
- **Pós-condição:** OS com um ou mais técnicos atribuídos, pronta para a vistoria (UC31/UC32).

### UC29 - Registrar Entrega e Pagamento (RF09)

*Fluxograma: "Notificar cliente" → "Apresentar OS" → "Receber pagamento", em Entrega ao Cliente.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); teste de funcionamento aprovado (UC34).
- **Fluxo principal:**
  1. O atendente notifica o cliente e apresenta a OS.
  2. O sistema registra o pagamento recebido.
- **Exceções:**
  - Na etapa 1: OS sem teste de funcionamento aprovado → o sistema impede a notificação de entrega (RF08).
  - Na etapa 2: valor pago não corresponde ao valor do orçamento aprovado → o sistema alerta a divergência antes de prosseguir (RNF09).
- **Pós-condição:** Pagamento registrado, OS pronta para o feedback do cliente (UC30).

### UC30 - Registrar Feedback do Cliente (RF09)

*Fluxograma: "Cliente satisfeito?" → "Receber feedback", em Entrega ao Cliente.*

- **Ator(es):** Recepcionista registra (ou Admin, RN11); Cliente informa se está satisfeito.
- **Pré-condição:** Usuário autenticado (UC01); entrega e pagamento já registrados (UC29).
- **Fluxo principal:**
  1. O cliente informa se está satisfeito com o serviço.
  2. Satisfeito → o sistema registra o feedback e finaliza a OS (status "finalizada").
- **Fluxo alternativo:**
  - Na etapa 2: cliente não satisfeito → o sistema registra o feedback e o erro relatado, retornando a OS para execução de manutenção (UC33).
- **Exceções:** nenhuma além das já cobertas pela pré-condição.
- **Pós-condição:** OS finalizada com pagamento e feedback registrados, ou reaberta para novo reparo.

---

## 8. Gerenciar Manutenção (Generalizado)

O pacote mais denso: cobre vistoria/diagnóstico (RF05), execução do reparo (RF07) e teste pós-reparo (RF08).

### UC31 - Registrar Observação Técnica sobre Equipamento (RF05)

*Fluxograma: "Inspeção visual" → "Identificar falhas", em Diagnóstico Técnico.*

- **Ator(es):** Técnico (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Técnico ou Admin (UC01); OS atribuída ao técnico (UC28).
- **Fluxo principal:**
  1. O técnico realiza a inspeção visual e os testes iniciais no equipamento.
  2. O técnico registra a observação técnica com o que foi identificado.
  3. O técnico pode editar a observação quantas vezes forem necessárias enquanto a vistoria continua.
- **Fluxo alternativo:**
  - Na etapa 1: nenhuma falha aparente é encontrada de imediato → o técnico segue para novos testes ou encerra a vistoria registrando esse resultado.
- **Exceções:**
  - Na etapa 1: OS não atribuída ao técnico → o sistema impede o início da vistoria (RN15).
- **Pós-condição:** Observação técnica registrada (opcional, editável), servindo de apoio ao diagnóstico final (UC32).

### UC32 - Registrar Diagnóstico do Equipamento (RF05)

*Fluxograma: "Testes funcionaram?" → "Relatório de diagnóstico", em Diagnóstico Técnico.*

- **Ator(es):** Técnico (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Técnico ou Admin (UC01); OS atribuída ao técnico (UC28).
- **Fluxo principal:**
  1. O técnico consolida a observação técnica (UC31) em um diagnóstico final.
  2. O técnico registra a situação do equipamento e o serviço necessário para o reparo.
  3. O sistema libera a OS para a etapa de orçamento (RF06).
- **Fluxo alternativo:**
  - Na etapa 1: nenhuma falha foi identificada → o técnico registra o diagnóstico final indicando equipamento sem defeito encontrado.
- **Exceções:**
  - Na etapa 1: diagnóstico final registrado sem nenhuma observação técnica preenchida → o sistema alerta antes de permitir o fechamento (RNF09).
- **Pós-condição:** OS com diagnóstico técnico registrado, pronta para orçamento (UC36).

### UC33 - Executar Manutenção (RF07)

*Fluxograma: "Separar peças" → "Realizar reparos" → "Registrar ações", em Execução do Serviço.*

- **Ator(es):** um ou mais Técnicos (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Técnico ou Admin (UC01); orçamento aprovado (UC39).
- **Fluxo principal:**
  1. O(s) técnico(s) separam peças e realizam o reparo.
  2. O técnico registra as ações realizadas.
  3. O sistema atualiza o status da OS.
- **Fluxo alternativo:**
  - Na etapa 1: mais de um técnico é necessário → cada um registra suas próprias ações, vinculadas à mesma OS (RN04).
  - Na etapa 2: reparo não funciona → o técnico registra o problema e o fluxo reinicia a partir da etapa 1.
- **Exceções:**
  - Na etapa 1: OS sem orçamento aprovado → o sistema impede o início da execução (RF06).
- **Pós-condição:** OS com reparo concluído, pronta para teste (UC34).

### UC34 - Registrar Teste de Funcionamento (RF08)

*Fluxograma: Teste de Funcionamento (inteiro).*

- **Ator(es):** Técnico (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Técnico ou Admin (UC01); reparo concluído (UC33).
- **Fluxo principal:**
  1. O técnico realiza o teste pós-reparo.
  2. O sistema registra se os testes funcionaram.
  3. Testes funcionaram → o técnico verifica o desempenho.
  4. Bom desempenho → aprovação interna → a OS segue para entrega (RF09).
- **Fluxo alternativo:**
  - Na etapa 2: testes não funcionaram → o técnico detecta o problema e a OS retorna à etapa 1 de UC33 (novo reparo).
  - Na etapa 3: desempenho não é bom → o técnico verifica o problema e realiza um novo reparo (retorna a UC33).
- **Exceções:**
  - Na etapa 1: OS sem reparo registrado como concluído → o sistema impede o início do teste (RF07).
- **Pós-condição:** OS aprovada internamente (pronta para UC35) ou reencaminhada para novo reparo.

### UC35 - Informar Conclusão ao Cliente (RF09)

*Fluxograma: ponte entre Teste de Funcionamento e Entrega ao Cliente.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); teste de funcionamento aprovado internamente (UC34).
- **Fluxo principal:**
  1. O atendente é avisado de que a OS está pronta para entrega.
  2. O atendente informa o cliente de que o serviço foi concluído.
- **Exceções:**
  - Na etapa 1: OS sem aprovação interna do teste → o sistema impede o aviso de conclusão (RF08).
- **Pós-condição:** Cliente informado, OS pronta para entrega e pagamento (UC29).

---

## 9. Gerenciar Orçamento (Generalizado)

### UC36 - Registrar Orçamento (RF06)

*Fluxograma: "Analisar relatório técnico" → "Calcular custo" → "Montar proposta" → "Enviar", em Elaboração do Orçamento.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); diagnóstico técnico registrado (UC32).
- **Fluxo principal:**
  1. O sistema apresenta o relatório de diagnóstico.
  2. O atendente calcula o custo e monta a proposta.
  3. O sistema envia o orçamento ao cliente.
- **Exceções:**
  - Na etapa 1: não existe diagnóstico técnico registrado para a OS → o sistema impede o início do orçamento (RF05).
- **Pós-condição:** Orçamento enviado, aguardando aprovação do cliente (UC39/UC40).

### UC37 - Editar Orçamento (RF06)

*Fluxograma: "Negociar", em Aprovação do Cliente.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); orçamento previamente registrado (UC36) e recusado pelo cliente (UC40).
- **Fluxo principal:**
  1. O atendente ajusta a proposta com base na recusa do cliente.
  2. O sistema reenvia o orçamento atualizado ao cliente, reabrindo o ciclo de aprovação (UC39/UC40).
- **Exceções:**
  - Na etapa 1: orçamento sem recusa registrada → o sistema não permite a renegociação (pré-condição não satisfeita).
- **Pós-condição:** Orçamento atualizado, aguardando nova resposta do cliente.

### UC38 - Consultar Orçamento (RF10)

- **Ator(es):** Recepcionista (ou Admin, RN11); Cliente consulta o orçamento das próprias OS.
- **Pré-condição:** Usuário autenticado (UC01).
- **Fluxo principal:**
  1. O ator seleciona a OS.
  2. O sistema exibe o valor e o status do orçamento.
- **Exceções:**
  - Cliente tenta consultar orçamento de OS de outro cliente → o sistema nega o acesso (RI10).
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC39 - Registrar Aprovação do Orçamento (RF06)

*Fluxograma: "Cliente aprova? → Sim → Iniciar serviço", em Aprovação do Cliente.*

- **Ator(es):** Cliente.
- **Pré-condição:** Usuário autenticado com perfil Cliente (UC01); orçamento enviado (UC36 ou UC37).
- **Fluxo principal:**
  1. O cliente analisa o orçamento recebido.
  2. O cliente aprova a proposta.
  3. O sistema libera a OS para execução da manutenção (UC33).
- **Exceções:**
  - Na etapa 1: não há orçamento pendente de aprovação para essa OS → o sistema não exibe a opção.
- **Pós-condição:** OS com orçamento aprovado, pronta para execução.

### UC40 - Registrar Recusa do Orçamento (RF06)

*Fluxograma: "Cliente aprova? → Não → Negociar", em Aprovação do Cliente.*

- **Ator(es):** Cliente.
- **Pré-condição:** Usuário autenticado com perfil Cliente (UC01); orçamento enviado (UC36 ou UC37).
- **Fluxo principal:**
  1. O cliente analisa o orçamento recebido.
  2. O cliente recusa a proposta.
  3. O sistema abre uma rodada de negociação (UC37).
- **Exceções:** nenhuma além da pré-condição.
- **Pós-condição:** OS aguardando renegociação (UC37) ou arquivamento (UC41), conforme o resultado da negociação.

### UC41 - Arquivar Orçamento (RN09)

*Fluxograma: "Cliente aprova? (negociado) → Não → Arquivar OS", em Aprovação do Cliente.*

- **Ator(es):** Recepcionista (ou Admin, RN11).
- **Pré-condição:** Usuário autenticado com perfil Recepcionista ou Admin (UC01); orçamento renegociado (UC37) e recusado novamente pelo cliente (UC40).
- **Fluxo principal:**
  1. O sistema constata que a proposta negociada também foi recusada.
  2. O sistema arquiva a OS, preservando o histórico (RN09).
- **Exceções:** nenhuma além da pré-condição.
- **Pós-condição:** OS arquivada - preservada no histórico, sem seguir para execução (RI11, não pode ser excluída fisicamente).

---

## 10. Gerenciar Histórico e Relatórios (Generalizado)

Consultas sobre dados já registrados nas etapas anteriores - não alteram nenhum registro.

### UC42 - Consultar Histórico dos Equipamentos (RF10)

- **Ator(es):** Cliente (os próprios), Técnico, Recepcionista, Admin.
- **Pré-condição:** Usuário autenticado (UC01).
- **Fluxo principal:**
  1. O ator seleciona um equipamento.
  2. O sistema exibe o histórico de OS desse equipamento, dentro do escopo do perfil (RN10).
- **Exceções:**
  - Cliente tenta consultar histórico de equipamento de outro cliente → o sistema nega o acesso (RI10).
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas.

### UC43 - Consultar Relatórios (RF10)

- **Ator(es):** Admin.
- **Pré-condição:** Usuário autenticado com perfil Admin (UC01).
- **Fluxo principal:**
  1. O Admin escolhe um filtro (ex.: período de tempo).
  2. O sistema retorna contagens/indicadores operacionais sobre o histórico (ex.: quantos equipamentos passaram por manutenção no período).
- **Exceções:** nenhuma.
- **Pós-condição:** Nenhuma alteração de estado - consulta apenas. Não inclui indicadores financeiros, de custo ou gerenciais - esses seguem fora do escopo (ver [Fora do Escopo](../requisitos/requisitos.md#fora-do-escopo)).

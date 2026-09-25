# Descrições dos Casos de Uso - formato Astah

Descrição de **cada elipse** do diagrama de casos de uso (`diagramas.asta`, nesta mesma pasta), organizada nos campos da aba **Use Case Description** do Astah: Summary, Actor, Precondition, Postcondition, Base Sequence, Branch Sequence, Exception Sequence, Sub UseCase, Note. Os casos de uso seguem a mesma ordem dos pacotes no Astah. Quando o caso de uso tem uma etapa correspondente em algum dos [fluxogramas](../Fluxograma/), isso é indicado no Summary.

## Como ler os diagramas

O `diagramas.asta` é dividido em pacotes (um diagrama por área). Convenções usadas em todos eles:

- **Boneco (ator):** quem interage com o sistema - Admin, Recepcionista, Técnico ou Cliente.
- **Elipse:** um caso de uso - uma ação que o sistema oferece.
- **Seta simples (ator → elipse):** associação - esse ator pode executar esse caso de uso.
- **Seta com triângulo vazado (ator → ator):** generalização entre atores. Usada para representar a RN11 - o Administrador generaliza Recepcionista e Técnico, ou seja, herda tudo o que esses perfis podem fazer, cobrindo a ausência de um deles.

O campo **Actor** lista os atores ligados diretamente ao caso de uso no diagrama. O Admin só aparece nele quando tem associação direta (cadastros de usuários e relatórios). Nos demais, ele participa por generalização, e o Astah não permite vinculá-lo ali. Por isso o Actor traz uma linha "Admin: por generalização..." e a linha "Herança" da Note explica o motivo.

## Padrões usados

- **Fluxo principal:** alterna ator e sistema e termina com "O caso de uso é encerrado."
- **Extensões:** identificadas pelo passo em que ocorrem, em uma única linha já explicada: `2a: <situação>. <o que acontece>. <para onde volta / encerramento>.` `3a` é a primeira extensão do passo 3 e `3b` a segunda do mesmo passo. A letra não se repete no mesmo caso de uso, mesmo entre Branch e Exception.
- **Branch Sequence:** extensão que termina com sucesso por outro caminho. Termina dizendo para onde volta ("Retorna ao passo N.").
- **Exception Sequence:** extensão que impede o objetivo. Termina sempre com "O caso de uso é encerrado."
- **Regras:** usam a numeração oficial de [requisitos.md](../requisitos/requisitos.md) (RN01-RN19, RI01-RI17, RNF01-RNF10). Não criar numeração local por caso de uso.
- **Note:** sempre com as mesmas 4 linhas, na mesma ordem:
  - **Herança** (ou **Associação direta**): explica por que o Admin aparece ou não no Actor. Texto padronizado, igual em todos.
  - **Regras:** as RN/RI/RF/RNF que o caso de uso aplica, com uma frase curta de cada.
  - **Dados:** as tabelas e os campos do [schema.sql](../Modelagem_DB/schema.sql) que o caso de uso lê ou altera. Serve de ponte para o código e para o diagrama de sequência.
  - **Observação:** detalhe que não cabe nos fluxos. Também registra decisões do grupo que não aparecem nos requisitos.

---

## 1. Geral (Generalizado)

### UC01 - Realizar Login

**Summary**
Permite que um usuário cadastrado se autentique com e-mail e senha e acesse as funcionalidades do seu perfil (RF01). Disponível para Cliente, Técnico, Recepcionista e, por herança, Admin.

**Actor**
Cliente, Técnico, Recepcionista.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário previamente cadastrado no sistema, com `status` ativo (RN12).

**Postcondition**
Sessão iniciada, com o perfil do usuário identificado.

**Base Sequence**
1. O usuário informa e-mail e senha.
2. O sistema localiza o cadastro pelo e-mail (RN13, único).
3. O sistema compara a senha informada com o hash armazenado (RNF10).
4. O sistema libera as funcionalidades do perfil (`perfil`) conforme a Matriz de Permissões (RN10).
5. O caso de uso é encerrado.

**Branch Sequence**
3a: Senha incorreta. O sistema informa que as credenciais são inválidas, sem indicar se o e-mail existe ou não. O usuário informa novamente a senha. Retorna ao passo 3.

**Exception Sequence**
2a: E-mail não cadastrado. O sistema rejeita o login com a mesma mensagem genérica da senha incorreta, para não expor quais e-mails existem. O caso de uso é encerrado.

4a: Usuário inativo. O sistema identifica `status` inativo e impede o login mesmo com credenciais corretas (RN12). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN10 - libera o menu conforme a Matriz de Permissões do perfil; RN13 - o e-mail é único e identifica o usuário; RN12 - usuário inativo não autentica; RNF10 - a senha é comparada pelo hash, nunca em texto plano.
Dados: lê `usuario` (email, senha, perfil, status). Não altera nenhum registro.
Observação: a mensagem de erro deve ser a mesma para e-mail inexistente e senha incorreta, para não revelar quais e-mails estão cadastrados. É a pré-condição de todos os demais casos de uso.

### UC02 - Fazer Logout

**Summary**
Permite que o usuário autenticado encerre a própria sessão no sistema (RF01). Disponível para Cliente, Técnico, Recepcionista e, por herança, Admin.

**Actor**
Cliente, Técnico, Recepcionista.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (sessão ativa, UC01).

**Postcondition**
Sessão encerrada.

**Base Sequence**
1. O usuário solicita encerrar a sessão.
2. O sistema encerra a sessão.
3. O sistema retorna à tela de login.
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
Não se aplica.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: nenhuma regra de negócio específica.
Dados: não altera registros; apenas descarta os dados da sessão (usuário logado) mantidos em memória.
Observação: depois do logout, qualquer outro caso de uso volta a exigir o UC01.

### UC03 - Consultar Meus Dados

**Summary**
Permite que o usuário autenticado visualize as informações do próprio cadastro (RF01, RF10). Disponível para Cliente, Técnico, Recepcionista e, por herança, Admin.

**Actor**
Cliente, Técnico, Recepcionista.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O usuário solicita ver os próprios dados.
2. O sistema identifica o usuário pela sessão ativa.
3. O sistema exibe o cadastro do usuário autenticado (dados comuns do perfil).
4. O caso de uso é encerrado.

**Branch Sequence**
3a: Dados específicos do perfil. Se o usuário for Técnico (ou Admin atuando como técnico), o sistema também exibe a especialidade. Retorna ao passo 4.

**Exception Sequence**
Não se aplica. O usuário só acessa o próprio registro (RI10 impede ver dados de outro usuário por essa via).

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RI10 - o usuário só vê o próprio registro; RNF10 - a senha nunca é exibida.
Dados: lê `usuario` pelo id da sessão (nome, email, telefone, cpf, perfil, especialidade).
Observação: os campos exibidos variam por perfil: CPF existe só para Cliente e especialidade só para Técnico. Este caso de uso não edita dados: a própria senha é trocada no UC44 (Alterar Minha Senha), e os demais dados nos casos de uso "Editar" (Admin ou Recepcionista).

### UC44 - Alterar Minha Senha

**Summary**
Permite que o usuário autenticado troque a própria senha, informando a senha atual e digitando a nova senha duas vezes para confirmação (RF01, RN19). Disponível para Cliente, Técnico, Recepcionista e, por herança, Admin.

**Actor**
Cliente, Técnico, Recepcionista.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (UC01).

**Postcondition**
Senha do usuário alterada. O próximo login (UC01) passa a exigir a nova senha.

**Base Sequence**
1. O usuário solicita alterar a própria senha.
2. O sistema solicita a senha atual, a nova senha e a confirmação da nova senha.
3. O usuário informa a senha atual, a nova senha e a confirmação.
4. O sistema confere a senha atual com o hash armazenado (RNF10).
5. O sistema confere se a nova senha e a confirmação são iguais.
6. O sistema grava o hash da nova senha e atualiza `atualizado_em`.
7. O sistema informa que a senha foi alterada com sucesso.
8. O caso de uso é encerrado.

**Branch Sequence**
5a: Confirmação diferente da nova senha. O sistema informa que as senhas não conferem e solicita novamente a nova senha e a confirmação. Retorna ao passo 3.

**Exception Sequence**
3a: Nova senha vazia. O sistema impede o avanço, pois a senha é obrigatória (RNF09). O caso de uso é encerrado.

4a: Senha atual incorreta. O sistema informa que a senha atual não confere e não altera a senha. O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN19 - todo usuário pode trocar a própria senha, confirmando-a duas vezes; RI10 - o usuário só altera a própria senha; RNF09 - campo obrigatório; RNF10 - a senha é gravada como hash, nunca em texto plano.
Dados: atualiza `usuario.senha` (hash) e `usuario.atualizado_em` do usuário da sessão. Não altera nenhum outro campo.
Observação: pedir a senha atual impede que outra pessoa troque a senha num terminal deixado com a sessão aberta. A troca da senha de outro usuário continua sendo feita pelo Admin nos casos de uso "Editar" (UC06, UC10, UC14) ou pelo Recepcionista no UC18 (clientes). Caso de uso novo: ainda precisa ser incluído no diagrama Geral do Astah, associado a Cliente, Técnico e Recepcionista.

---

## 2. Gerenciar Admin

### UC04 - Cadastrar Admin

**Summary**
Permite que o Admin cadastre um novo administrador no sistema (RF02, RN05). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01).

**Postcondition**
Novo administrador cadastrado, apto a autenticar-se (UC01).

**Base Sequence**
1. O Admin acessa o cadastro de administradores.
2. O Admin informa nome, e-mail e senha do novo administrador.
3. O sistema valida os campos obrigatórios e a unicidade do e-mail (RN13).
4. O sistema grava o cadastro com `perfil = admin` e `status = ativo`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: E-mail já cadastrado. O sistema rejeita o cadastro, pois o e-mail já pertence a outro usuário (RN13, RI13). O caso de uso é encerrado.

3b: Campo obrigatório vazio. O sistema indica o campo não preenchido e impede o avanço (RNF09). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - somente o Admin cadastra usuários; RN13/RI13 - e-mail único; RNF09 - campos obrigatórios; RNF10 - senha gravada como hash.
Dados: insere em `usuario` com perfil = "admin" e status = "ativo"; cpf e especialidade ficam vazios.
Observação: RI01 - Recepcionista, Técnico e Cliente não têm acesso a esta opção no menu.

### UC05 - Consultar Admin

**Summary**
Permite que o Admin busque e liste os administradores cadastrados, inclusive os inativos (RF02, RF10). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O Admin busca ou lista os administradores cadastrados.
2. O sistema exibe os registros, incluindo os inativos (RN12 preserva o histórico).
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
Não se aplica.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - gestão de usuários restrita ao Admin; RN12 - inativos continuam listados.
Dados: lê `usuario` onde perfil = "admin".
Observação: exibir o status de cada registro para diferenciar ativos e inativos. A senha nunca é exibida.

### UC06 - Editar Admin

**Summary**
Permite que o Admin altere os dados de um administrador cadastrado (RF02). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01); administrador-alvo previamente cadastrado (UC04).

**Postcondition**
Cadastro do administrador atualizado.

**Base Sequence**
1. O Admin seleciona um administrador cadastrado.
2. O Admin altera os dados desejados (nome, e-mail, senha).
3. O sistema valida os campos e, se o e-mail foi alterado, sua unicidade (RN13).
4. O sistema grava a alteração e atualiza `atualizado_em`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: E-mail já utilizado. O sistema rejeita a alteração, pois o novo e-mail já é usado por outro usuário (RN13, RI13). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - somente o Admin altera usuários; RN13/RI13 - e-mail único; RNF10 - nova senha gravada como hash.
Dados: atualiza `usuario` (nome, email, senha) e `atualizado_em`. Perfil e status não mudam aqui.
Observação: o status só é alterado pelo UC07 (Inativar Admin).

### UC07 - Inativar Admin

**Summary**
Permite que o Admin inative um administrador, preservando o histórico das OS relacionadas (RF02, RN12). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01); administrador-alvo com `status` ativo.

**Postcondition**
Administrador inativo. Não pode mais autenticar-se (RN12) nem ser vinculado a uma nova OS.

**Base Sequence**
1. O Admin seleciona um administrador cadastrado.
2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" em que esse administrador atue como atendente ou como técnico substituto (RN14).
3. O sistema marca `status = inativo`, preservando o histórico das OS relacionadas (RN12).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
1a: Admin seleciona o próprio cadastro. O sistema impede a inativação, pois é necessário manter pelo menos um Admin ativo para configurar o sistema (RN17, RI17). O caso de uso é encerrado.

2a: OS em andamento vinculada. O sistema impede a inativação e informa a existência de OS em andamento (RN14, RI14). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - somente o Admin inativa usuários; RN12/RI12 - inativação no lugar de exclusão física; RN14/RI14 - bloqueio com OS aberta ou em andamento; RN17/RI17 - o Admin não inativa o próprio cadastro.
Dados: atualiza `usuario.status` para "inativo". O banco reforça a RN14 pelo trigger `trg_usuario_valida_inativacao`; a RN17 é validada pelo programa, comparando o administrador-alvo com o usuário da sessão.
Observação: como o Admin nunca inativa a si mesmo, o sistema sempre mantém pelo menos um Admin ativo: para inativar um administrador, é preciso que outro Admin ativo faça a operação.

---

## 3. Gerenciar Recepcionista

### UC08 - Cadastrar Recepcionista

**Summary**
Permite que o Admin cadastre um novo recepcionista (RF02, RN05). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01).

**Postcondition**
Novo recepcionista cadastrado, apto a autenticar-se (UC01) e a abrir OS (RF04).

**Base Sequence**
1. O Admin acessa o cadastro de recepcionistas.
2. O Admin informa nome, e-mail e senha do novo recepcionista.
3. O sistema valida os campos obrigatórios e a unicidade do e-mail (RN13).
4. O sistema grava o cadastro com `perfil = recepcionista` e `status = ativo`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: E-mail já cadastrado. O sistema rejeita o cadastro (RN13, RI13). O caso de uso é encerrado.

3b: Campo obrigatório vazio. O sistema indica o campo não preenchido e impede o avanço (RNF09). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - somente o Admin cadastra usuários; RN13/RI13 - e-mail único; RNF09 - campos obrigatórios; RNF10 - senha gravada como hash.
Dados: insere em `usuario` com perfil = "recepcionista" e status = "ativo".
Observação: RI01 - Recepcionista, Técnico e Cliente não têm acesso a esta opção no menu.

### UC09 - Consultar Recepcionista

**Summary**
Permite que o Admin busque e liste os recepcionistas cadastrados, inclusive os inativos (RF02, RF10). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O Admin busca ou lista os recepcionistas cadastrados.
2. O sistema exibe os registros, incluindo os inativos.
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
Não se aplica.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - gestão de usuários restrita ao Admin; RN12 - inativos continuam listados.
Dados: lê `usuario` onde perfil = "recepcionista".
Observação: exibir o status de cada registro. A senha nunca é exibida.

### UC10 - Editar Recepcionista

**Summary**
Permite que o Admin altere os dados de um recepcionista cadastrado (RF02). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01); recepcionista-alvo previamente cadastrado (UC08).

**Postcondition**
Cadastro do recepcionista atualizado.

**Base Sequence**
1. O Admin seleciona um recepcionista cadastrado.
2. O Admin altera os dados desejados.
3. O sistema valida os campos e, se o e-mail foi alterado, sua unicidade (RN13).
4. O sistema grava a alteração e atualiza `atualizado_em`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: E-mail já utilizado. O sistema rejeita a alteração (RN13, RI13). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - somente o Admin altera usuários; RN13/RI13 - e-mail único; RNF10 - nova senha gravada como hash.
Dados: atualiza `usuario` e `atualizado_em`. Perfil e status não mudam aqui.
Observação: o status só é alterado pelo UC11 (Inativar Recepcionista).

### UC11 - Inativar Recepcionista

**Summary**
Permite que o Admin inative um recepcionista, preservando o histórico (RF02, RN12). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01); recepcionista-alvo com `status` ativo.

**Postcondition**
Recepcionista inativo. Não pode mais autenticar-se nem abrir novas OS.

**Base Sequence**
1. O Admin seleciona um recepcionista cadastrado.
2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" com esse recepcionista como atendente (`os.id_atendente`, RN14).
3. O sistema marca `status = inativo` (RN12).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: OS em andamento atendida por ele. O sistema impede a inativação (RN14, RI14). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05; RN12/RI12 - inativação no lugar de exclusão; RN14/RI14 - bloqueio com OS aberta ou em andamento.
Dados: atualiza `usuario.status` para "inativo". O trigger `trg_usuario_valida_inativacao` verifica `os.id_atendente`.
Observação: as OS já atendidas por ele continuam apontando para o registro, preservando o histórico.

---

## 4. Gerenciar Técnico

### UC12 - Cadastrar Técnico

**Summary**
Permite que o Admin cadastre um novo técnico, incluindo sua especialidade (RF02, RN05). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01).

**Postcondition**
Novo técnico cadastrado, apto a autenticar-se (UC01) e a ser atribuído a uma OS (RF07).

**Base Sequence**
1. O Admin acessa o cadastro de técnicos.
2. O Admin informa nome, e-mail, senha e especialidade do novo técnico.
3. O sistema valida os campos obrigatórios e a unicidade do e-mail (RN13).
4. O sistema grava o cadastro com `perfil = tecnico` e `status = ativo`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: E-mail já cadastrado. O sistema rejeita o cadastro (RN13, RI13). O caso de uso é encerrado.

3b: Campo obrigatório vazio, incluindo especialidade. O sistema indica o campo não preenchido e impede o avanço (RNF09). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - somente o Admin cadastra usuários; RN13/RI13 - e-mail único; RNF09 - campos obrigatórios, incluindo especialidade; RNF10 - senha gravada como hash.
Dados: insere em `usuario` com perfil = "tecnico", status = "ativo" e especialidade preenchida.
Observação: só usuários com perfil "tecnico" (ou "admin", pela RN11) podem ser atribuídos a uma OS (RN15, UC28).

### UC13 - Consultar Técnico

**Summary**
Permite que o Admin busque e liste os técnicos cadastrados, com especialidade e inclusive os inativos (RF02, RF10). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O Admin busca ou lista os técnicos cadastrados.
2. O sistema exibe os registros, incluindo a especialidade e os inativos.
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
Não se aplica.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - gestão de usuários restrita ao Admin; RN12 - inativos continuam listados.
Dados: lê `usuario` onde perfil = "tecnico", incluindo especialidade.
Observação: exibir o status de cada registro. A senha nunca é exibida.

### UC14 - Editar Técnico

**Summary**
Permite que o Admin altere os dados de um técnico cadastrado, inclusive a especialidade (RF02). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01); técnico-alvo previamente cadastrado (UC12).

**Postcondition**
Cadastro do técnico atualizado.

**Base Sequence**
1. O Admin seleciona um técnico cadastrado.
2. O Admin altera os dados desejados, incluindo a especialidade.
3. O sistema valida os campos e, se o e-mail foi alterado, sua unicidade (RN13).
4. O sistema grava a alteração e atualiza `atualizado_em`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: E-mail já utilizado. O sistema rejeita a alteração (RN13, RI13). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05 - somente o Admin altera usuários; RN13/RI13 - e-mail único; RNF09 - especialidade continua obrigatória; RNF10 - nova senha gravada como hash.
Dados: atualiza `usuario` (incluindo especialidade) e `atualizado_em`.
Observação: o status só é alterado pelo UC15 (Inativar Técnico).

### UC15 - Inativar Técnico

**Summary**
Permite que o Admin inative um técnico, preservando o histórico (RF02, RN12). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01); técnico-alvo com `status` ativo.

**Postcondition**
Técnico inativo. Não pode mais autenticar-se nem ser atribuído a novas OS.

**Base Sequence**
1. O Admin seleciona um técnico cadastrado.
2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" em que esse técnico esteja atribuído (`tecnico_os`, RN14).
3. O sistema marca `status = inativo` (RN12).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: OS em andamento atribuída a ele. O sistema impede a inativação (RN14, RI14). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN05; RN12/RI12 - inativação no lugar de exclusão; RN14/RI14 - bloqueio com OS aberta ou em andamento.
Dados: atualiza `usuario.status` para "inativo". O trigger `trg_usuario_valida_inativacao` verifica as atribuições em `tecnico_os`.
Observação: o histórico de atribuições em `tecnico_os` é preservado.

---

## 5. Gerenciar Cliente (Generalizado)

### UC16 - Cadastrar Cliente

**Summary**
Permite que o Recepcionista cadastre um novo cliente, normalmente durante o próprio atendimento (RF02, RF04). Disponível para Recepcionista e, por herança, Admin. Fluxograma: "Registro do Cliente", em Cadastro e Abertura de Serviço.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01).

**Postcondition**
Cliente cadastrado, apto a ter equipamentos vinculados (RF03) e OS abertas (RF04).

**Base Sequence**
1. O atendente informa nome, e-mail, telefone, senha e CPF do cliente.
2. O sistema valida os campos obrigatórios e a unicidade de e-mail e CPF (RN13).
3. O sistema grava o cadastro com `perfil = cliente` e `status = ativo`.
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: E-mail ou CPF já cadastrados. O sistema rejeita o cadastro (RN13, RI13). O caso de uso é encerrado.

2b: Campo obrigatório vazio. O sistema indica o campo não preenchido e impede o avanço (RNF09). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica. Pode ser acionado a partir de UC24 - Criar Ordem de Serviço.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF02 - o Recepcionista pode cadastrar clientes durante o atendimento; RN13/RI13 - e-mail e CPF únicos; RNF09 - campos obrigatórios; RNF10 - senha gravada como hash.
Dados: insere em `usuario` com perfil = "cliente", status = "ativo", cpf e telefone.
Observação: é o único cadastro de usuário que não é feito só pelo Admin: a RN05 vale para a equipe, e o cliente é registrado durante o atendimento (UC24). A senha permite ao cliente acessar o sistema para consultar suas OS e responder ao orçamento.

### UC17 - Consultar Cliente

**Summary**
Permite buscar e listar clientes cadastrados, respeitando o escopo de cada perfil (RF10, RN10). Disponível para Recepcionista, Técnico e, por herança, Admin.

**Actor**
Recepcionista, Técnico.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista, Admin ou Técnico (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O ator busca ou lista clientes cadastrados.
2. O sistema exibe os registros dentro do escopo do perfil (RN10).
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: Cliente fora do escopo do Técnico. O Técnico tenta consultar um cliente de uma OS não atribuída a ele. O sistema nega o acesso (RN10). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN10 - escopo de consulta por perfil.
Dados: lê `usuario` onde perfil = "cliente". Para o Técnico, filtra pelos clientes dos equipamentos das OS em que ele está em `tecnico_os`.
Observação: o Cliente não consulta outros clientes; vê apenas os próprios dados no UC03.

### UC18 - Editar Cliente

**Summary**
Permite que o Recepcionista altere os dados de um cliente cadastrado (RF02). Disponível para Recepcionista e, por herança, Admin.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); cliente-alvo previamente cadastrado (UC16).

**Postcondition**
Cadastro do cliente atualizado.

**Base Sequence**
1. O atendente seleciona um cliente cadastrado.
2. O atendente altera os dados desejados.
3. O sistema valida os campos e, se e-mail/CPF foram alterados, sua unicidade (RN13).
4. O sistema grava a alteração e atualiza `atualizado_em`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: E-mail ou CPF já utilizados. O sistema rejeita a alteração (RN13, RI13). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF02; RN13/RI13 - e-mail e CPF únicos; RNF09 - campos obrigatórios.
Dados: atualiza `usuario` (cliente) e `atualizado_em`.
Observação: o status só é alterado pelo UC19 (Inativar Cliente).

### UC19 - Inativar Cliente

**Summary**
Permite que o Recepcionista inative um cliente, preservando o histórico (RN12). Disponível para Recepcionista e, por herança, Admin.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); cliente-alvo com `status` ativo.

**Postcondition**
Cliente inativo. Não pode mais autenticar-se, ter novos equipamentos vinculados nem novas OS abertas.

**Base Sequence**
1. O atendente seleciona um cliente cadastrado.
2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" vinculada a um equipamento desse cliente (RN14).
3. O sistema marca `status = inativo` (RN12).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: OS em andamento vinculada. O sistema impede a inativação (RN14, RI14). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN12/RI12 - inativação no lugar de exclusão; RN14/RI14 - bloqueio com OS aberta ou em andamento.
Dados: atualiza `usuario.status` para "inativo". O trigger `trg_usuario_valida_inativacao` verifica as OS dos equipamentos do cliente.
Observação: os equipamentos do cliente continuam ativos, mas não recebem nova OS, porque a abertura exige cliente ativo (trigger `trg_os_valida_ativos_ins`).

---

## 6. Gerenciar Equipamento (Generalizado)

### UC20 - Cadastrar Equipamento

**Summary**
Permite que o Recepcionista cadastre um equipamento (hardware ou software) vinculado a um cliente (RF03, RN01). Disponível para Recepcionista e, por herança, Admin. Fluxograma: "Registro do equipamento", em Cadastro e Abertura de Serviço.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); cliente proprietário previamente cadastrado e ativo (UC16).

**Postcondition**
Equipamento cadastrado, apto a receber uma OS (RF04).

**Base Sequence**
1. O atendente seleciona o cliente proprietário.
2. O atendente informa categoria (hardware ou software), tipo, marca e modelo/número de série (ou chave de licença).
3. O sistema valida os campos obrigatórios e vincula o equipamento ao cliente (RN01).
4. O sistema grava o cadastro com `status = ativo`.
5. O caso de uso é encerrado.

**Branch Sequence**
2a: Equipamento de software. O atendente informa a chave de licença no lugar do número de série. Retorna ao passo 3.

**Exception Sequence**
1a: Cliente inativo. O sistema impede o cadastro do equipamento (RN14). O caso de uso é encerrado.

3a: Campo obrigatório vazio. O sistema indica o campo não preenchido e impede o avanço (RNF09). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN01 - todo equipamento pertence a um cliente; RN16/RI16 - o dono precisa ter perfil "cliente" (trigger); RN14 - cliente inativo não recebe equipamento; RNF09 - campos obrigatórios.
Dados: insere em `equipamento` (id_cliente, categoria "hardware" | "software", tipo, marca, modelo_numero_serie, status = "ativo").
Observação: não existe campo separado para chave de licença: no software ela vai em `modelo_numero_serie`. No banco só a categoria é obrigatória (NOT NULL); a obrigatoriedade dos demais campos é validada pelo programa (RNF09).

### UC21 - Consultar Equipamento

**Summary**
Permite buscar e listar equipamentos respeitando o escopo de cada perfil (RF10, RN10). Disponível para Cliente, Técnico, Recepcionista e, por herança, Admin. Fluxograma: "Tipo de equipamento", em Atendimento ao Cliente.

**Actor**
Cliente, Técnico, Recepcionista.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O ator busca ou lista equipamentos.
2. O sistema exibe os registros dentro do escopo do perfil (RN10).
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: Cliente consulta equipamento de outro cliente. O sistema nega o acesso (RI10). O caso de uso é encerrado.

2b: Técnico consulta equipamento fora do seu escopo. O Técnico tenta consultar equipamento de uma OS não atribuída a ele. O sistema nega o acesso (RN10). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN10 - escopo de consulta por perfil; RI10 - o Cliente não vê equipamentos de outro cliente.
Dados: lê `equipamento`. Cliente: filtra id_cliente = usuário logado. Técnico: filtra pelos equipamentos das OS em que ele está em `tecnico_os`.
Observação: equipamentos inativos também aparecem, com o status indicado, para preservar a consulta ao histórico.

### UC22 - Editar Equipamento

**Summary**
Permite que o Recepcionista altere os dados de um equipamento cadastrado (RF03). Disponível para Recepcionista e, por herança, Admin.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); equipamento-alvo previamente cadastrado (UC20).

**Postcondition**
Cadastro do equipamento atualizado.

**Base Sequence**
1. O atendente seleciona um equipamento cadastrado.
2. O atendente altera os dados desejados (tipo, marca, modelo/número de série).
3. O sistema valida os campos.
4. O sistema grava a alteração, atualizando `atualizado_em`.
5. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
3a: Campo obrigatório removido/vazio. O sistema impede o avanço (RNF09). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF03; RNF09 - campos obrigatórios; RN16 - se o dono (id_cliente) for alterado, o trigger valida o novo dono.
Dados: atualiza `equipamento` e `atualizado_em`.
Observação: o status só é alterado pelo UC23 (Inativar Equipamento). A transferência para outro cliente não faz parte do fluxo principal.

### UC23 - Inativar Equipamento

**Summary**
Permite que o Recepcionista inative um equipamento, preservando seu histórico de OS (RN12). Disponível para Recepcionista e, por herança, Admin.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); equipamento-alvo com `status` ativo.

**Postcondition**
Equipamento inativo. Não pode mais receber uma nova OS.

**Base Sequence**
1. O atendente seleciona um equipamento cadastrado.
2. O sistema verifica se existe alguma OS "aberta" ou "em_andamento" vinculada a esse equipamento (RN14).
3. O sistema marca `status = inativo` (RN12).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: OS em andamento vinculada. O sistema impede a inativação (RN14, RI14). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN12/RI12 - inativação no lugar de exclusão; RN14/RI14 - bloqueio com OS aberta ou em andamento.
Dados: atualiza `equipamento.status` para "inativo" (trigger de inativação verifica as OS do equipamento).
Observação: o histórico de OS do equipamento continua consultável (UC42).

---

## 7. Gerenciar Ordem de Serviço

### UC24 - Criar Ordem de Serviço

**Summary**
Permite que o Recepcionista abra uma OS para um equipamento, registrando o problema relatado pelo cliente (RF04, RN02, RN03). Disponível para Recepcionista e, por herança, Admin. Fluxograma: Cadastro e Abertura de Serviço.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); equipamento identificado (UC20).

**Postcondition**
OS registrada com status "Aberta", pronta para a vistoria (UC31/UC32).

**Base Sequence**
1. O atendente coleta os dados do cliente e do equipamento.
2. O sistema verifica se o cliente já possui cadastro.
3. O atendente registra o problema relatado.
4. O sistema abre a OS com status "Aberta", registra o atendente responsável (RN03) e a encaminha para diagnóstico.
5. O caso de uso é encerrado.

**Branch Sequence**
2a: Cliente sem cadastro. O sistema registra o cliente (UC16). Retorna ao passo 3.

**Exception Sequence**
1a: Equipamento sem cliente vinculado. O equipamento foi informado sem cliente vinculado e sem dados para novo cadastro. O sistema rejeita a abertura (RN01, RI02). O caso de uso é encerrado.

1b: Cliente ou equipamento inativo. O sistema impede a abertura de nova OS (RN14). O caso de uso é encerrado.

3a: Problema relatado não informado. O sistema impede o avanço para o passo 4 (RNF09). O caso de uso é encerrado.

**Sub UseCase**
UC16 - Cadastrar Cliente (quando o cliente não possui cadastro).
UC20 - Cadastrar Equipamento (quando o equipamento não está cadastrado).

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN02 - a OS pertence a um equipamento; RN03 - aberta pelo Recepcionista (ou Admin, RN11); RN14/RI14 - equipamento, cliente e atendente ativos; RI02 - não abre OS sem equipamento vinculado a cliente; RNF09.
Dados: insere em `os` (id_equipamento, id_atendente = usuário logado, data_abertura, problema_relatado, status = "aberta"). O trigger `trg_os_valida_ativos_ins` reforça a RN14.
Observação: é o ponto de entrada do processo: os casos de uso de manutenção, orçamento e entrega dependem desta OS.

### UC25 - Consultar Ordem de Serviço

**Summary**
Permite buscar e listar ordens de serviço, incluindo finalizadas e canceladas, respeitando o escopo de cada perfil (RF10, RN10). Disponível para Cliente, Técnico, Recepcionista e, por herança, Admin.

**Actor**
Cliente, Técnico, Recepcionista.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O ator busca ou lista ordens de serviço.
2. O sistema exibe as OS dentro do escopo do perfil (RN10), incluindo finalizadas e canceladas.
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: Cliente consulta OS de outro cliente. O sistema nega o acesso (RI10). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN10 - escopo por perfil; RI10 - o Cliente não vê OS de outro cliente; RN06 - o Admin vê todas; RN09 - canceladas e finalizadas continuam visíveis.
Dados: lê `os` (com `equipamento` e `usuario`). Recepcionista: por id_atendente. Técnico: via `tecnico_os`. Cliente: via `equipamento.id_cliente`.
Observação: a etapa em que a OS está pode ser deduzida dos campos preenchidos (diagnostico, orcamento, resultado_resposta_cliente, resultado_teste), além do status.

### UC26 - Editar Ordem de Serviço

**Summary**
Permite que o Recepcionista altere os dados administrativos de uma OS não finalizada (RN07). Disponível para Recepcionista e, por herança, Admin.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); OS previamente criada (UC24), não finalizada.

**Postcondition**
Dados administrativos da OS atualizados.

**Base Sequence**
1. O atendente seleciona a OS.
2. O atendente altera os campos administrativos (ex.: problema relatado).
3. O sistema grava a alteração e atualiza `atualizado_em`.
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
1a: OS finalizada. O sistema impede a alteração, exceto pelo Admin (RN08, RI09). O caso de uso é encerrado.

2a: Alteração de campo técnico. O atendente tenta alterar diagnóstico ou resultado do teste. O sistema rejeita, pois esse campo pertence à função do Técnico (RN07, RI08). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN07/RI08 - o Recepcionista altera só dados administrativos; RN08/RI09 - OS finalizada não é alterada, exceto pelo Admin (RN06).
Dados: atualiza `os.problema_relatado` e `atualizado_em`. Campos técnicos (diagnostico, resultado_teste, `tecnico_os.observacoes_tecnicas`) ficam bloqueados.
Observação: a OS não guarda dados do cliente. Ela aponta só para o equipamento (id_equipamento), e o cliente é obtido pelo dono do equipamento (`equipamento.id_cliente` → `usuario`). Para alterar dados do cliente, usar o UC18.

### UC27 - Cancelar Ordem de Serviço

**Summary**
Permite que o Recepcionista cancele uma OS não finalizada, mantendo-a no histórico (RN09). Disponível para Recepcionista e, por herança, Admin.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); OS previamente criada (UC24), não finalizada.

**Postcondition**
OS cancelada, mantida no histórico (não pode ser excluída fisicamente, RI11).

**Base Sequence**
1. O atendente seleciona a OS a ser cancelada.
2. O sistema marca a OS com status "cancelada".
3. O sistema preserva o registro para fins de histórico (RN09).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
1a: OS já finalizada. O sistema impede o cancelamento (RN08). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN09/RI11 - a OS cancelada permanece no histórico; RN08 - OS finalizada não pode ser cancelada.
Dados: atualiza `os.status` para "cancelada".
Observação: cancelada e arquivada usam o mesmo status no banco ("cancelada"); a diferença é só o motivo de negócio (ver UC41). Não há campo para o motivo do cancelamento.

### UC28 - Atribuir Técnico à Ordem de Serviço

**Summary**
Permite que o Recepcionista atribua um ou mais técnicos a uma OS (RF07, RN04). Disponível para Recepcionista e, por herança, Admin. Fluxograma: "Direcionar para técnico disponível", em Diagnóstico Técnico.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); OS previamente criada (UC24); técnico(s) cadastrado(s) e ativo(s) (UC12).

**Postcondition**
OS com um ou mais técnicos atribuídos, pronta para a vistoria (UC31/UC32).

**Base Sequence**
1. O atendente seleciona a OS e um ou mais técnicos disponíveis.
2. O sistema grava um registro em `tecnico_os` para cada técnico atribuído (RN04).
3. O caso de uso é encerrado.

**Branch Sequence**
1a: Mais de um técnico necessário. O atendente repete a atribuição, uma linha por técnico, todas vinculadas à mesma OS (RN04). Retorna ao passo 2.

**Exception Sequence**
1b: Técnico inativo. O sistema impede a atribuição (RN14). O caso de uso é encerrado.

2a: Usuário sem perfil técnico nem admin. O sistema rejeita o registro (RN15, RI15, reforçado por trigger). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN04 - um ou mais técnicos por OS; RN15/RI15 - só perfil "tecnico" ou "admin" (trigger); RN14 - técnico inativo não é atribuído (trigger).
Dados: insere em `tecnico_os` (id_tecnico, id_os, data_atribuicao), uma linha por técnico.
Observação: pela RN11, o Admin pode ser atribuído como técnico quando não houver técnico disponível. A especialidade do técnico ajuda na escolha.

### UC29 - Registrar Entrega e Pagamento

**Summary**
Permite que o Recepcionista registre a entrega do equipamento e o pagamento recebido (RF09). Disponível para Recepcionista e, por herança, Admin. Fluxograma: "Notificar cliente" → "Apresentar OS" → "Receber pagamento", em Entrega ao Cliente.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); teste de funcionamento aprovado (UC34).

**Postcondition**
Pagamento registrado, OS pronta para o feedback do cliente (UC30).

**Base Sequence**
1. O atendente notifica o cliente e apresenta a OS.
2. O atendente informa o valor recebido.
3. O sistema registra o pagamento recebido.
4. O caso de uso é encerrado.

**Branch Sequence**
3a: Valor pago divergente. O sistema alerta que o valor não corresponde ao orçamento aprovado (RNF09). O atendente corrige ou confirma o valor. Retorna ao passo 3.

**Exception Sequence**
1a: OS sem teste aprovado. O sistema impede a notificação de entrega (RF08). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF09; RI07 - o Técnico não registra entrega nem pagamento; RNF09 - valor informado.
Dados: atualiza `os.data_entrega` e `os.valor_pago`.
Observação: o valor de referência para a divergência (3a) é `os.orcamento` já aprovado pelo cliente. Quando o orçamento é de valor zero (RN18), a entrega é registrada com `valor_pago` = 0,00.

### UC30 - Registrar Feedback do Cliente

**Summary**
Permite registrar se o cliente ficou satisfeito com o serviço, finalizando a OS ou reabrindo-a para novo reparo (RF09). Disponível para Cliente, Recepcionista e, por herança, Admin. Fluxograma: "Cliente satisfeito?" → "Receber feedback", em Entrega ao Cliente.

**Actor**
Cliente, Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (UC01); entrega e pagamento já registrados (UC29).

**Postcondition**
OS finalizada com pagamento e feedback registrados, ou reaberta para novo reparo.

**Base Sequence**
1. O cliente informa se está satisfeito com o serviço.
2. O sistema registra o feedback.
3. O sistema finaliza a OS (status "finalizada").
4. O caso de uso é encerrado.

**Branch Sequence**
1a: Cliente não satisfeito. O sistema registra o feedback e o erro relatado. O sistema retorna a OS para execução de manutenção (UC33). O caso de uso é encerrado.

**Exception Sequence**
Não se aplica. Nenhuma exceção além das já cobertas pela pré-condição.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF09; RI07 - o Técnico não registra feedback; RN08 - após finalizada, a OS só é alterada pelo Admin.
Dados: atualiza `os.satisfeito` ("sim" | "nao") e `os.feedback_cliente`. Se satisfeito, `os.status` = "finalizada".
Observação: o Cliente dá a resposta e o Recepcionista registra. Se o cliente não estiver satisfeito, a OS continua "em_andamento" e volta para o UC33.

---

## 8. Gerenciar Manutenção (Generalizado)

### UC31 - Registrar Observação Técnica sobre Equipamento

**Summary**
Permite que o Técnico registre o que identificou na inspeção e nos testes iniciais do equipamento (RF05). Disponível para Técnico e, por herança, Admin. Fluxograma: "Inspeção visual" → "Identificar falhas", em Diagnóstico Técnico.

**Actor**
Técnico.
Admin: por generalização (herda de Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Técnico ou Admin (UC01); OS atribuída ao técnico (UC28).

**Postcondition**
Observação técnica registrada (opcional, editável), servindo de apoio ao diagnóstico final (UC32).

**Base Sequence**
1. O técnico realiza a inspeção visual e os testes iniciais no equipamento.
2. O técnico registra a observação técnica com o que foi identificado.
3. O sistema grava a observação vinculada à OS.
4. O caso de uso é encerrado.

**Branch Sequence**
1a: Nenhuma falha aparente. O técnico segue para novos testes ou encerra a vistoria registrando esse resultado. Retorna ao passo 2.

3a: Edição da observação. O técnico edita a observação enquanto a vistoria continua. Retorna ao passo 3.

**Exception Sequence**
1b: OS não atribuída ao técnico. O sistema impede o início da vistoria (RN10). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Técnico, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF05; RI03 - Recepcionista e Cliente não registram dados técnicos; RN10 - só o técnico atribuído à OS acessa a vistoria.
Dados: atualiza `tecnico_os.observacoes_tecnicas` (uma observação por técnico atribuído).
Observação: é opcional e editável; serve de apoio ao diagnóstico final (UC32).

### UC32 - Registrar Diagnóstico do Equipamento

**Summary**
Permite que o Técnico consolide as observações em um diagnóstico final e libere a OS para orçamento (RF05). Disponível para Técnico e, por herança, Admin. Fluxograma: "Testes funcionaram?" → "Relatório de diagnóstico", em Diagnóstico Técnico.

**Actor**
Técnico.
Admin: por generalização (herda de Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Técnico ou Admin (UC01); OS atribuída ao técnico (UC28).

**Postcondition**
OS com diagnóstico técnico registrado, pronta para orçamento (UC36).

**Base Sequence**
1. O técnico consolida a observação técnica (UC31) em um diagnóstico final.
2. O técnico registra a situação do equipamento e o serviço necessário para o reparo.
3. O sistema libera a OS para a etapa de orçamento (RF06).
4. O caso de uso é encerrado.

**Branch Sequence**
1a: Nenhuma falha identificada. O técnico registra o diagnóstico final indicando equipamento sem defeito encontrado e nenhum serviço necessário (RN18). Retorna ao passo 3.

**Exception Sequence**
1b: Nenhuma observação técnica preenchida. O sistema alerta antes de permitir o fechamento do diagnóstico (RNF09). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Técnico, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF05; RI03 - Recepcionista e Cliente não registram diagnóstico; RN07 - campo técnico, do Técnico.
Dados: atualiza `os.diagnostico`.
Observação: com o diagnóstico preenchido, a OS fica liberada para o orçamento (UC36). Mesmo sem defeito encontrado (1a), a OS segue o fluxo normal, com orçamento de valor zero (RN18): orçamento → aprovação → execução sem reparo → teste → entrega.

### UC33 - Executar Manutenção

**Summary**
Permite que um ou mais técnicos registrem o reparo realizado no equipamento (RF07, RN04). Disponível para Técnico e, por herança, Admin. Fluxograma: "Separar peças" → "Realizar reparos" → "Registrar ações", em Execução do Serviço.

**Actor**
Técnico.
Admin: por generalização (herda de Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Técnico ou Admin (UC01); orçamento aprovado (UC39).

**Postcondition**
OS com reparo concluído, pronta para teste (UC34).

**Base Sequence**
1. O(s) técnico(s) separam peças e realizam o reparo.
2. O técnico registra as ações realizadas.
3. O sistema atualiza o status da OS.
4. O caso de uso é encerrado.

**Branch Sequence**
1a: Mais de um técnico. Cada técnico registra suas próprias ações, vinculadas à mesma OS (RN04). Retorna ao passo 3.

1b: Nenhum reparo necessário. O diagnóstico não encontrou defeito e o orçamento aprovado é de valor zero (RN18). O técnico não separa peças e registra que nenhum reparo foi necessário. Retorna ao passo 3.

2a: Reparo não funciona. O técnico registra o problema. Retorna ao passo 1.

**Exception Sequence**
1c: OS sem orçamento aprovado. O sistema impede o início da execução (RF06). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Técnico, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF07; RN04 - mais de um técnico por OS; RN18 - OS sem defeito segue o fluxo com orçamento zero; RI05 - Recepcionista e Cliente não executam o reparo.
Dados: atualiza `os.status` para "em_andamento" e registra as ações em `tecnico_os.observacoes_tecnicas`.
Observação: as ações realizadas no reparo ficam no mesmo campo das observações da vistoria (`tecnico_os.observacoes_tecnicas`), sem campo próprio. Controle de peças e estoque está fora do escopo.

### UC34 - Registrar Teste de Funcionamento

**Summary**
Permite que o Técnico registre o teste pós-reparo e aprove internamente a OS para entrega (RF08). Disponível para Técnico e, por herança, Admin. Fluxograma: Teste de Funcionamento (inteiro).

**Actor**
Técnico.
Admin: por generalização (herda de Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Técnico ou Admin (UC01); reparo concluído (UC33).

**Postcondition**
OS aprovada internamente (pronta para UC35) ou reencaminhada para novo reparo.

**Base Sequence**
1. O técnico realiza o teste pós-reparo.
2. O sistema registra se os testes funcionaram.
3. O técnico verifica o desempenho.
4. O sistema registra a aprovação interna e a OS segue para entrega (RF09).
5. O caso de uso é encerrado.

**Branch Sequence**
2a: Testes não funcionaram. O técnico detecta o problema. A OS retorna ao passo 1 de UC33 (novo reparo). O caso de uso é encerrado.

3a: Desempenho insatisfatório. O técnico verifica o problema. A OS retorna a UC33 para novo reparo. O caso de uso é encerrado.

**Exception Sequence**
1a: Reparo não concluído. O sistema impede o início do teste (RF07). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Técnico, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF08; RI06 - Recepcionista e Cliente não registram o teste.
Dados: atualiza `os.resultado_teste` ("aprovado" | "reprovado").
Observação: só com "aprovado" a OS segue para o UC35 e o UC29. Com "reprovado", volta para o UC33.

### UC35 - Informar Conclusão ao Cliente

**Summary**
Permite que o Recepcionista avise o cliente de que o serviço foi concluído (RF09). Disponível para Recepcionista e, por herança, Admin. Fluxograma: ponte entre Teste de Funcionamento e Entrega ao Cliente.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); teste de funcionamento aprovado internamente (UC34).

**Postcondition**
Cliente informado, OS pronta para entrega e pagamento (UC29).

**Base Sequence**
1. O sistema avisa o atendente de que a OS está pronta para entrega.
2. O atendente informa o cliente de que o serviço foi concluído.
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
1a: OS sem aprovação interna do teste. O sistema impede o aviso de conclusão (RF08). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF09; RF08 - só OS com teste aprovado.
Dados: não altera registros; lista as OS com `resultado_teste` = "aprovado" ainda sem `data_entrega`.
Observação: o aviso ao cliente acontece fora do sistema (telefone ou presencial), porque o sistema é só de terminal (RNF02).

---

## 9. Gerenciar Orçamento (Generalizado)

### UC36 - Registrar Orçamento

**Summary**
Permite que o Recepcionista monte o orçamento a partir do diagnóstico e o envie ao cliente (RF06). Disponível para Recepcionista e, por herança, Admin. Fluxograma: "Analisar relatório técnico" → "Calcular custo" → "Montar proposta" → "Enviar", em Elaboração do Orçamento.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); diagnóstico técnico registrado (UC32).

**Postcondition**
Orçamento enviado, aguardando aprovação do cliente (UC39/UC40).

**Base Sequence**
1. O sistema apresenta o relatório de diagnóstico.
2. O atendente calcula o custo e monta a proposta.
3. O sistema envia o orçamento ao cliente.
4. O caso de uso é encerrado.

**Branch Sequence**
2a: Diagnóstico sem defeito. O atendente registra o orçamento com valor zero (R$ 0,00), informando na proposta que nenhum reparo é necessário (RN18). Retorna ao passo 3.

**Exception Sequence**
1a: Sem diagnóstico registrado. O sistema impede o início do orçamento (RF05). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF06; RF05 - exige diagnóstico registrado; RN18 - diagnóstico sem defeito gera orçamento de valor zero; RI04 - Técnico e Cliente não elaboram orçamento.
Dados: atualiza `os.orcamento` (0,00 quando não há defeito).
Observação: "enviar ao cliente" significa deixar o orçamento disponível para consulta e resposta (UC38, UC39, UC40). O sistema não envia e-mail.

### UC37 - Editar Orçamento

**Summary**
Permite que o Recepcionista renegocie um orçamento recusado e o reenvie ao cliente (RF06). Disponível para Recepcionista e, por herança, Admin. Fluxograma: "Negociar", em Aprovação do Cliente.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); orçamento previamente registrado (UC36) e recusado pelo cliente (UC40).

**Postcondition**
Orçamento atualizado, aguardando nova resposta do cliente.

**Base Sequence**
1. O atendente ajusta a proposta com base na recusa do cliente.
2. O sistema reenvia o orçamento atualizado ao cliente, reabrindo o ciclo de aprovação (UC39/UC40).
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
1a: Orçamento sem recusa registrada. O sistema não permite a renegociação (pré-condição não satisfeita). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF06; RI04 - Técnico e Cliente não alteram orçamento.
Dados: sobrescreve `os.orcamento` e volta `os.resultado_resposta_cliente` para vazio, aguardando a nova resposta do cliente.
Observação: o valor anterior não é guardado. Existe um único campo de orçamento, que sempre mostra o valor vigente da negociação.

### UC38 - Consultar Orçamento

**Summary**
Permite consultar o valor e o status do orçamento de uma OS, respeitando o escopo de cada perfil (RF10, RN10). Disponível para Cliente, Recepcionista e, por herança, Admin.

**Actor**
Cliente, Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O ator seleciona a OS.
2. O sistema exibe o valor e o status do orçamento.
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: Cliente consulta orçamento de outro cliente. O sistema nega o acesso (RI10). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN10 - escopo por perfil; RI10 - o Cliente só vê orçamentos das próprias OS.
Dados: lê `os.orcamento` e `os.resultado_resposta_cliente` (vazio = aguardando resposta; "aprovado" ou "recusado").
Observação: o Técnico não consulta orçamento, pois não está associado a este caso de uso.

### UC39 - Registrar Aprovação do Orçamento

**Summary**
Permite que o Cliente aprove o orçamento recebido, liberando a OS para execução (RF06). Disponível para Cliente, Recepcionista e, por herança, Admin. Fluxograma: "Cliente aprova? → Sim → Iniciar serviço", em Aprovação do Cliente.

**Actor**
Cliente, Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Cliente, Recepcionista ou Admin (UC01); orçamento enviado (UC36 ou UC37).

**Postcondition**
OS com orçamento aprovado, pronta para execução.

**Base Sequence**
1. O cliente analisa o orçamento recebido.
2. O cliente aprova a proposta.
3. O sistema libera a OS para execução da manutenção (UC33).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
1a: Nenhum orçamento pendente. O sistema não exibe a opção de aprovação. O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF06; RN10 - o Cliente só responde orçamentos das próprias OS.
Dados: atualiza `os.resultado_resposta_cliente` para "aprovado" (valores aceitos pelo banco: "aprovado" | "recusado").
Observação: o próprio Cliente registra a resposta pelo sistema, ou o Recepcionista registra por ele quando a resposta vem no balcão ou por telefone. Quando o orçamento é de valor zero (RN18), a aprovação funciona como a confirmação do cliente para a OS seguir até a entrega.

### UC40 - Registrar Recusa do Orçamento

**Summary**
Permite que o Cliente recuse o orçamento recebido, abrindo uma rodada de negociação (RF06). Disponível para Cliente, Recepcionista e, por herança, Admin. Fluxograma: "Cliente aprova? → Não → Negociar", em Aprovação do Cliente.

**Actor**
Cliente, Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Cliente, Recepcionista ou Admin (UC01); orçamento enviado (UC36 ou UC37).

**Postcondition**
OS aguardando renegociação (UC37) ou arquivamento (UC41), conforme o resultado da negociação.

**Base Sequence**
1. O cliente analisa o orçamento recebido.
2. O cliente recusa a proposta.
3. O sistema abre uma rodada de negociação (UC37).
4. O caso de uso é encerrado.

**Branch Sequence**
3a: Recusa após renegociação. Se a proposta recusada já era a renegociada, o sistema encaminha a OS para arquivamento (UC41). O caso de uso é encerrado.

**Exception Sequence**
Não se aplica. Nenhuma exceção além da pré-condição.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RF06; RN10 - o Cliente só responde orçamentos das próprias OS.
Dados: atualiza `os.resultado_resposta_cliente` para "recusado" (valores aceitos pelo banco: "aprovado" | "recusado").
Observação: o próprio Cliente registra a resposta, ou o Recepcionista registra por ele. A primeira recusa leva à renegociação (UC37); a recusa da proposta renegociada leva ao arquivamento (UC41).

### UC41 - Arquivar Orçamento

**Summary**
Arquiva a OS quando o cliente recusa também a proposta renegociada, preservando o histórico (RN09). Disponível para Recepcionista e, por herança, Admin. Fluxograma: "Cliente aprova? (negociado) → Não → Arquivar OS", em Aprovação do Cliente.

**Actor**
Recepcionista.
Admin: por generalização (herda de Recepcionista, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado com perfil Recepcionista ou Admin (UC01); orçamento renegociado (UC37) e recusado novamente pelo cliente (UC40).

**Postcondition**
OS arquivada. Preservada no histórico, sem seguir para execução (RI11, não pode ser excluída fisicamente).

**Base Sequence**
1. O sistema constata que a proposta negociada também foi recusada.
2. O atendente confirma o arquivamento.
3. O sistema arquiva a OS, preservando o histórico (RN09).
4. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
Não se aplica. Nenhuma exceção além da pré-condição.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização do Recepcionista, herdando as associações dele. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN09/RI11 - a OS arquivada permanece no histórico e não é excluída.
Dados: atualiza `os.status` para "cancelada".
Observação: "arquivada" não é um status próprio no banco: é o nome de negócio para uma OS cancelada por recusa do orçamento renegociado (ver der-simplificado.md).

---

## 10. Gerenciar Histórico e Relatórios (Generalizado)

### UC42 - Consultar Histórico dos Equipamentos

**Summary**
Permite consultar o histórico de OS de um equipamento, respeitando o escopo de cada perfil (RF10, RN10). Disponível para Cliente, Técnico, Recepcionista e, por herança, Admin.

**Actor**
Cliente, Técnico, Recepcionista.
Admin: por generalização (herda de Recepcionista e Técnico, RN11). Não aparece vinculado neste campo - ver Note.

**Precondition**
Usuário autenticado (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O ator seleciona um equipamento.
2. O sistema exibe o histórico de OS desse equipamento, dentro do escopo do perfil (RN10).
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
2a: Cliente consulta equipamento de outro cliente. O sistema nega o acesso (RI10). O caso de uso é encerrado.

**Sub UseCase**
Não se aplica.

**Note**
Herança (RN11): o Admin executa este caso de uso por ser especialização dos atores Recepcionista e Técnico, herdando as associações deles. Por isso não aparece no campo Actor nem ligado diretamente à elipse no diagrama.
Regras: RN10 - escopo por perfil; RI10 - o Cliente só vê os próprios equipamentos; RN09 - OS canceladas e finalizadas fazem parte do histórico.
Dados: lê `os` por id_equipamento (com `tecnico_os` para os técnicos envolvidos), inclusive de equipamentos inativos.
Observação: o nome da elipse no diagrama está "Consultar Histórico dos Equipamento" (sem o "s") e vale corrigir no Astah.

### UC43 - Consultar Relatórios

**Summary**
Permite que o Admin consulte indicadores operacionais sobre o histórico, filtrando por período (RF10). Disponível apenas para o Admin (associação direta).

**Actor**
Admin (associação direta).

**Precondition**
Usuário autenticado com perfil Admin (UC01).

**Postcondition**
Nenhuma alteração de estado. Consulta apenas.

**Base Sequence**
1. O Admin escolhe um filtro (ex.: período de tempo).
2. O sistema retorna contagens/indicadores operacionais sobre o histórico (ex.: quantos equipamentos passaram por manutenção no período).
3. O caso de uso é encerrado.

**Branch Sequence**
Não se aplica.

**Exception Sequence**
Não se aplica.

**Sub UseCase**
Não se aplica.

**Note**
Associação direta: caso de uso exclusivo do Admin (RN05), ligado diretamente a ele no diagrama. Não depende da generalização.
Regras: RN06 - o Admin consulta quaisquer dados; RN10 - funcionalidade exclusiva do perfil Admin.
Dados: lê `os` (contagens por data_abertura, status e equipamento).
Observação: não inclui indicadores financeiros, de custo ou gerenciais, que estão fora do escopo (ver requisitos.md, Fora do Escopo).

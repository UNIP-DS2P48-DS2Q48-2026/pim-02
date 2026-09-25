# Requisitos do Sistema

## Identificação do Projeto

- **Projeto:** Sistema de Gestão de Manutenção de Equipamentos.

- **Organização:** ByteTech - Empresa fictícia especializada em manutenção de **hardware** (conserto de computadores, notebooks, impressoras e demais equipamentos) e de **software** (licenças, instalação e manutenção de programas) para **pessoas físicas**.

- **Tipo de sistema:** Sistema de gerenciamento executado exclusivamente em terminal.

- **Linguagem de programação:** C.

- **Interface:** Terminal/console.

- **Objetivo:** Desenvolver um sistema simples para auxiliar a empresa no controle de clientes, equipamentos, técnicos, recepcionistas e ordens de serviço, substituindo o controle manual realizado por planilhas e anotações.

## Escopo do Sistema

O sistema será desenvolvido exclusivamente em linguagem C e funcionará por meio do terminal. Todas as operações deverão ser realizadas utilizando menus, opções numéricas, entradas de texto e mensagens exibidas no próprio terminal. O foco será desenvolver uma aplicação de console que permita representar os principais processos da empresa de manutenção, desde o atendimento ao cliente e abertura da ordem de serviço até a execução, o teste e a entrega - cobrindo tanto serviços de **hardware** (conserto físico de equipamentos) quanto de **software** (licenças, instalação e manutenção de programas).

## Fora do Escopo

Itens deliberadamente fora do sistema - decisão tomada, não uma lacuna:

- **Controle de estoque de peças.** Peças usadas ficam registradas como texto/observação na OS (RF07), sem controle de quantidade, entrada/saída ou fornecedores.
- **Agendamento e manutenção preventiva.** O atendimento é sempre sob demanda: o processo começa quando o cliente traz um problema (RF04); não há agenda de visitas, monitoramento remoto ou lembretes automáticos.
- **Envio automático de notificações.** O sistema registra que o cliente foi notificado (RF09), mas não se integra a e-mail, SMS ou aplicativos de mensagem.
- **Processamento de pagamento.** O sistema registra o valor recebido (RF09), mas não se integra a gateways de pagamento (cartão, Pix, boleto).
- **Centro de custos e relatórios financeiros/gerenciais.** Fora do escopo desta versão (ver [DER Simplificado](../Modelagem_DB/der-simplificado.md)).
- **Gestão de fornecedores.** Decorre diretamente da ausência de controle de estoque.
- **Interface gráfica, web ou mobile.** O sistema é exclusivamente terminal/console (RNF02).
- **Operação em rede com múltiplos usuários simultâneos.** O programa roda localmente com persistência em arquivo; arquitetura de rede é uma proposta conceitual separada (Etapa 6 do PIM), não implementada no código em C.

## Problema Identificado

A empresa atualmente realiza o controle de seus processos utilizando planilhas e anotações. Esse modelo dificulta:

- Localizar ordens de serviço.

- Saber quem está responsável por uma OS.

- Acompanhar o andamento de um serviço, do orçamento até a entrega.

- Consultar diagnósticos e resultados de testes.

- Consultar informações históricas.

- Obter indicadores sobre os serviços realizados.

O sistema deverá permitir registrar informações mais completas, como data de abertura, responsável pelo atendimento, técnico(s) responsável(is), diagnóstico, orçamento, aprovação do cliente, resultado do teste de funcionamento e situação da OS.

## Perfis de Usuário

Todos os perfis do sistema são armazenados em um único cadastro (`usuario`), diferenciados por um campo `perfil`: **Administrador**, **Recepcionista**, **Técnico** ou **Cliente**. Técnicos possuem um campo próprio (especialidade) armazenado nesse mesmo cadastro - não existe uma tabela separada por perfil. Todo usuário (de qualquer perfil) tem também um `status` (ativo/inativo, RN12).

A matriz abaixo é a fonte única de permissões do sistema. Qualquer requisito ou regra de negócio que trate de "quem pode fazer o quê" remete a ela, em vez de repetir a mesma informação em vários lugares.

| Ação | Admin | Recepcionista | Técnico | Cliente |
|---|---|---|---|---|
| Autenticar-se no sistema | ✔ | ✔ | ✔ | ✔ |
| Alterar a própria senha | ✔ | ✔ | ✔ | ✔ |
| Gerenciar usuários da equipe (admin/recepcionista/técnico) | ✔ | - | - | - |
| Cadastrar cliente (durante o atendimento) | ✔¹ | ✔ | - | - |
| Gerenciar equipamentos | ✔¹ | ✔ | - | consulta os próprios |
| Abrir ordem de serviço | ✔¹ | ✔ | - | - |
| Registrar diagnóstico técnico | ✔¹ | - | ✔ | - |
| Elaborar orçamento / registrar aprovação | ✔¹ | ✔ | - | aprova ou recusa |
| Executar manutenção (um ou mais técnicos) | ✔¹ | - | ✔ | - |
| Registrar teste de funcionamento | ✔¹ | - | ✔ | - |
| Registrar entrega, pagamento e feedback | ✔¹ | ✔ | - | dá o feedback |
| Consultar ordens de serviço | todas | as que atendeu | as que executou | só as próprias |

¹ O Administrador pode ocupar qualquer uma dessas ações quando não houver um Recepcionista ou Técnico disponível para realizá-la (RN11).

## Requisitos Funcionais

**RF01: Autenticação e controle de acesso por perfil**

O sistema deverá permitir o login por **e-mail** e senha e, após identificar o `perfil` do usuário, apresentar somente as funcionalidades permitidas para ele, conforme a Matriz de Permissões. Todo usuário autenticado poderá também alterar a própria senha (RN19).

**RF02: Gerenciar usuários**

O sistema deverá permitir cadastrar, consultar, alterar e ativar/inativar usuários com perfil de administrador, recepcionista ou técnico (RN05, RN12). Para técnicos, deverá ser registrada a especialidade, armazenada no próprio cadastro de usuário. O cadastro de clientes poderá também ser realizado pelo Recepcionista durante o atendimento (RF04).

**RF03: Gerenciar equipamentos**

O sistema deverá permitir cadastrar, consultar, alterar e ativar/inativar equipamentos (RN12), cobrindo tanto itens de **hardware** (computador, notebook, impressora) quanto de **software** (licença, programa instalado). Cada equipamento deverá estar vinculado a um cliente (RN01) e possuir categoria (hardware ou software), tipo, marca, modelo e número de série (ou chave de licença, quando aplicável).

**RF04: Abrir ordem de serviço**

O sistema deverá permitir registrar uma nova ordem de serviço vinculada a um equipamento (RN02), com a data de abertura, o problema relatado pelo cliente e o atendente responsável (RN03).

**RF05: Registrar diagnóstico técnico**

O sistema deverá permitir a um técnico realizar a vistoria de uma ordem de serviço atribuída a ele, registrando:

a) uma **observação técnica**, que pode ser atualizada/editada pelo técnico ao longo da vistoria, descrevendo falhas identificadas e testes realizados no equipamento;

b) o **diagnóstico final**, consolidando a situação atual do equipamento e o serviço necessário para o reparo.

O diagnóstico final é obrigatório e é ele que libera a OS para a etapa de orçamento (RF06). A observação técnica é opcional e serve de apoio ao processo de vistoria, mas não substitui o diagnóstico final.

**RF06: Elaborar orçamento e registrar aprovação do cliente**

O sistema deverá permitir calcular e enviar o orçamento ao cliente e registrar sua aprovação, recusa ou negociação, podendo repetir o ciclo de negociação até a aprovação ou o arquivamento da OS.

**RF07: Executar manutenção**

O sistema deverá permitir atribuir um ou mais técnicos a uma ordem de serviço (RN04) e registrar as ações realizadas até a conclusão do reparo.

**RF08: Registrar teste de funcionamento**

O sistema deverá permitir registrar os testes realizados após o reparo e sua aprovação interna antes da entrega ao cliente.

**RF09: Registrar entrega, pagamento e feedback**

O sistema deverá permitir registrar a notificação ao cliente, o pagamento recebido e o feedback de satisfação sobre o serviço prestado.

**RF10: Consultar informações e histórico**

O sistema deverá permitir consultar ordens de serviço, equipamentos e usuários de acordo com o perfil autenticado, conforme a Matriz de Permissões, incluindo o histórico de ordens finalizadas e canceladas.

Essa consulta inclui contagens e filtros operacionais sobre o histórico (por exemplo, quantidade de equipamentos que passaram por manutenção em um determinado período), restritos ao próprio perfil autenticado. Não inclui indicadores financeiros, de custo ou gerenciais - esses seguem fora do escopo (ver [Fora do Escopo](#fora-do-escopo)).

Detalhamento de cada caso de uso do sistema (ator, pré-condição, fluxo principal, alternativo e de exceção, pós-condição), organizado por diagrama e correlacionado com o fluxograma, em [Casos de Uso](../Modalagem_casos_de_uso/descricoes-astah.md).

## Requisitos Não Funcionais

**RNF01:** O sistema deverá ser desenvolvido utilizando a linguagem C.

**RNF02:** O sistema deverá funcionar exclusivamente através do terminal.

**RNF03:** A interface deverá utilizar menus textuais e opções numéricas.

**RNF04:** As informações deverão permanecer armazenadas mesmo após o encerramento do programa, podendo ser utilizados arquivos.

**RNF05:** As operações deverão apresentar resposta adequada para o volume de dados previsto.

**RNF06:** O código deverá ser organizado utilizando funções e estruturas separadas.

**RNF07:** O sistema deverá ser executável em ambientes compatíveis com compiladores C.

**RNF08:** A interface textual deverá apresentar mensagens claras, menus organizados e instruções objetivas.

**RNF09:** O sistema deverá validar, em todas as operações, campos obrigatórios, relacionamentos entre registros e permissões de acesso, garantindo a integridade dos dados antes de qualquer cadastro, alteração ou remoção.

**RNF10:** As senhas dos usuários deverão ser armazenadas de forma protegida (hash), nunca em texto plano.

## Regras de Negócio

- **RN01:** Todo equipamento deverá estar vinculado a um cliente.

- **RN02:** Toda ordem de serviço deverá estar vinculada a um equipamento.

- **RN03:** A abertura de uma ordem de serviço deverá ser realizada por um Recepcionista, ressalvado o disposto na RN11.

- **RN04:** Uma ordem de serviço poderá ser atribuída a um ou mais Técnicos para realização da manutenção.

- **RN05:** Somente o Administrador poderá cadastrar, alterar e ativar/inativar usuários.

- **RN06:** O Administrador poderá consultar e editar quaisquer dados do sistema quando necessário.

- **RN07:** Cada perfil só poderá alterar os campos da OS relativos à sua função: o Recepcionista, os dados administrativos; o Técnico, os dados técnicos.

- **RN08:** Uma OS finalizada não deverá ser alterada como se estivesse em andamento, salvo operação administrativa (RN06).

- **RN09:** Uma OS cancelada ou arquivada deverá permanecer registrada no sistema para fins de histórico.

- **RN10:** O acesso, a visibilidade das informações e as funcionalidades disponíveis na interface deverão seguir a Matriz de Permissões por perfil.

- **RN11:** Na ausência de um Recepcionista ou Técnico disponível, o Administrador poderá ocupar esse papel na ordem de serviço.

- **RN12:** "Remover" um usuário ou equipamento significa marcar seu `status` como inativo, preservando o histórico das ordens de serviço relacionadas - o sistema não deverá excluir esses registros fisicamente. Um usuário inativo não poderá mais acessar o sistema (login bloqueado).

- **RN13:** O e-mail de cada usuário e o CPF de cada cliente deverão ser únicos no sistema.

- **RN14:** Um usuário, cliente ou equipamento inativo não poderá ser vinculado a uma nova ordem de serviço, nem poderá ser inativado enquanto tiver uma ordem de serviço em andamento - reforçado no próprio banco de dados por trigger, não apenas na lógica do programa (ver [Integridade de status](../Modelagem_DB/der-simplificado.md#integridade-de-status-vínculos-com-usuárioequipamento-inativo-rn14)).

- **RN15:** Um registro em `tecnico_os` só poderá referenciar um usuário cujo `perfil` seja `tecnico` ou, na ausência deste (RN11), `admin` - reforçado no próprio banco de dados por trigger, não apenas na lógica do programa (ver [Integridade de papel](../Modelagem_DB/der-simplificado.md#integridade-de-papel-tecnico_os-e-equipamento)).

- **RN16:** Um `equipamento` só poderá estar vinculado (`id_cliente`) a um usuário cujo `perfil` seja `cliente` - reforçado no próprio banco de dados por trigger, não apenas na lógica do programa (ver [Integridade de papel](../Modelagem_DB/der-simplificado.md#integridade-de-papel-tecnico_os-e-equipamento)).

- **RN17:** O Administrador não poderá inativar o próprio cadastro, garantindo que o sistema tenha sempre ao menos um Administrador ativo para configurá-lo.

- **RN18:** Quando o diagnóstico técnico não encontrar defeito no equipamento, a ordem de serviço seguirá o fluxo normal com orçamento de valor zero, passando pela aprovação do cliente, pelo teste de funcionamento e pela entrega, sem cobrança.

- **RN19:** Todo usuário autenticado poderá alterar a própria senha, informando a senha atual e digitando a nova senha duas vezes para confirmação.

## Requisitos Inversos

O que o sistema **não** deve permitir - o inverso de cada requisito/regra, explícito para não depender de interpretação da Matriz de Permissões.

| Código | O sistema NÃO deve permitir que... | Relacionado a |
|---|---|---|
| RI01 | Recepcionista, Técnico ou Cliente cadastrem, alterem, ativem ou inativem usuários da equipe. | RF02, RN05 |
| RI02 | uma ordem de serviço seja aberta sem um equipamento vinculado a um cliente. | RF04, RN01, RN02 |
| RI03 | Recepcionista ou Cliente registrem diagnóstico técnico. | RF05 |
| RI04 | Técnico ou Cliente elaborem o orçamento. | RF06 |
| RI05 | Recepcionista ou Cliente executem o reparo. | RF07 |
| RI06 | Recepcionista ou Cliente registrem o teste de funcionamento. | RF08 |
| RI07 | o Técnico registre entrega, pagamento ou feedback do cliente. | RF09 |
| RI08 | um perfil altere campos da OS fora da sua função (ex.: Técnico alterando dado administrativo). | RN07 |
| RI09 | uma OS finalizada seja alterada como se estivesse em andamento, exceto pelo Administrador. | RN06, RN08 |
| RI10 | um Cliente visualize dados, equipamentos ou ordens de serviço de outro cliente. | RN10 |
| RI11 | uma OS cancelada ou arquivada seja excluída definitivamente - o histórico deve ser preservado. | RN09 |
| RI12 | um usuário ou equipamento com ordens de serviço vinculadas seja excluído fisicamente - deve ser inativado. | RN12 |
| RI13 | dois usuários com o mesmo e-mail, ou dois clientes com o mesmo CPF, sejam cadastrados. | RN13 |
| RI14 | uma OS seja aberta para cliente/equipamento inativo, ou que um usuário/equipamento seja inativado com OS em andamento. | RN14 |
| RI15 | um usuário com `perfil` Recepcionista ou Cliente seja relacionado como técnico em `tecnico_os` - o próprio banco rejeita o registro. | RN15 |
| RI16 | um `equipamento` seja vinculado (`id_cliente`) a um usuário com `perfil` diferente de Cliente (ex.: um Técnico) - o próprio banco rejeita o registro. | RN16 |
| RI17 | um Administrador inative o próprio cadastro, deixando o sistema sem nenhum Administrador ativo. | RN17 |

## Modelo de Dados (Visão Geral)

O sistema é sustentado por 4 tabelas, nomeadas no singular e em português: `usuario` (todos os perfis, diferenciados por `perfil`), `equipamento`, `os` (ordem de serviço) e `tecnico_os` (associação entre técnicos e OS, permitindo um ou mais técnicos por ordem). Não existe controle de estoque ou de peças como funcionalidade do sistema.

Detalhamento completo dos campos e o diagrama entidade-relacionamento em [Modelagem de Banco de Dados - DER Simplificado](../Modelagem_DB/der-simplificado.md).

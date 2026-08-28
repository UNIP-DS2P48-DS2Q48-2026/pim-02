# Requisitos do Sistema

## Identificação do Projeto

- **Projeto:** Sistema de Gestão de Manutenção de Equipamentos.
- **Organização fictícia:** Empresa especializada em manutenção de computadores, notebooks, impressoras e equipamentos utilizados por pequenas empresas.
- **Tipo de sistema:** Sistema de gerenciamento executado exclusivamente em terminal.
- **Linguagem de programação:** C.
- **Interface:** Terminal/console.
- **Objetivo:** Desenvolver um sistema simples para auxiliar a empresa no controle de clientes, equipamentos, técnicos, ordens de serviço e peças, substituindo o controle manual realizado por planilhas e anotações.

## Escopo do Sistema

O sistema será desenvolvido exclusivamente em linguagem C e funcionará por meio do terminal. Todas as operações deverão ser realizadas utilizando menus, opções numéricas, entradas de texto e mensagens exibidas no próprio terminal. O foco será desenvolver uma aplicação de console que permita representar os principais processos da empresa de manutenção.

## Problema Identificado

A empresa atualmente realiza o controle de seus processos utilizando planilhas e anotações. Esse modelo dificulta:

- Localizar ordens de serviço.
- Saber quem está responsável por uma OS.
- Acompanhar o andamento de um serviço.
- Consultar diagnósticos.
- Registrar peças utilizadas.
- Controlar a quantidade de peças disponíveis.
- Consultar informações históricas.
- Obter indicadores sobre os serviços realizados.

O sistema deverá permitir registrar informações mais completas, como data de entrada, responsável, diagnóstico, peças utilizadas, orçamento, aprovação e situação da OS.

## Hierarquia de Usuários

### Administrador

O Administrador possuirá acesso geral ao sistema, podendo:

- Cadastrar, alterar e consultar clientes.
- Cadastrar e consultar equipamentos.
- Cadastrar e consultar técnicos.
- Cadastrar peças e consultar estoque.
- Criar ordens de serviço e associar técnicos às OS.
- Consultar ordens de serviço e alterar status das OS.
- Consultar relatórios e indicadores.
- Gerenciar usuários.

### Técnico

O Técnico possuirá acesso às funcionalidades relacionadas às ordens de serviço atribuídas a ele, podendo:

- Consultar suas ordens de serviço.
- Visualizar dados do equipamento e o problema informado.
- Registrar diagnóstico, peças utilizadas e serviço realizado.
- Atualizar o andamento da OS.
- Finalizar uma ordem de serviço.

O técnico não deverá possuir acesso às funcionalidades administrativas do sistema.

### Cliente

O Cliente possuirá acesso limitado às informações relacionadas aos seus próprios equipamentos e ordens de serviço, podendo:

- Consultar seus dados, seus equipamentos e suas ordens de serviço.
- Consultar o status de uma OS e informações básicas sobre o serviço.

O cliente não poderá alterar informações técnicas ou administrativas.

## Requisitos Funcionais

**RF01: Gerenciar usuários e controle de acesso**
O sistema deverá permitir ao Administrador realizar o gerenciamento dos usuários, incluindo cadastro, consulta, alteração e remoção de usuários. Cada usuário deverá possuir uma hierarquia de acesso, sendo Administrador, Técnico ou Cliente. O sistema deverá realizar a autenticação por meio de login e senha no terminal e, após a identificação do usuário, apresentar somente as funcionalidades permitidas para sua hierarquia.

**RF02: Gerenciar clientes e equipamentos**
O sistema deverá permitir ao Administrador realizar o gerenciamento dos clientes e dos equipamentos cadastrados, incluindo cadastro, consulta, alteração e remoção dos registros. Cada equipamento deverá estar vinculado a um cliente e deverá possuir informações como tipo, marca, modelo e número de série. O sistema deverá permitir consultar os equipamentos associados a determinado cliente e seu histórico de ordens de serviço.

**RF03: Gerenciar técnicos e peças**
O sistema deverá permitir ao Administrador realizar o gerenciamento dos técnicos e das peças utilizadas pela empresa, incluindo cadastro, consulta, alteração e remoção dos registros. Para os técnicos, deverão ser armazenadas informações como nome, especialidade e situação. Para as peças, deverão ser armazenadas informações como nome, categoria, quantidade disponível e preço. O sistema deverá impedir operações que resultem em quantidade negativa ou utilização de peças superior à quantidade disponível no estoque.

**RF04: Gerenciar ordens de serviço**
O sistema deverá permitir ao Administrador realizar o gerenciamento das ordens de serviço, incluindo abertura, consulta, alteração e cancelamento. Cada ordem de serviço deverá estar vinculada a um equipamento e deverá registrar informações como data de abertura, problema informado, técnico responsável, status e demais informações relacionadas ao atendimento. O sistema deverá permitir localizar uma OS por diferentes critérios, como código, cliente, equipamento, técnico ou status.

**RF05: Executar manutenção da ordem de serviço**
O sistema deverá permitir que o Técnico acompanhe e atualize as ordens de serviço atribuídas a ele. Durante a execução da manutenção, o Técnico deverá poder registrar o diagnóstico, orçamento, aprovação, peças utilizadas, serviço realizado e alterações no andamento da OS. O sistema deverá manter essas informações associadas à ordem de serviço para permitir a consulta posterior do histórico do atendimento.

**RF06: Controlar o ciclo e o histórico da ordem de serviço**
O sistema deverá controlar o ciclo de uma ordem de serviço por meio de diferentes estados, como Aberta, Em análise, Aguardando orçamento, Aguardando aprovação, Em manutenção, Aguardando peça, Pronta, Finalizada e Cancelada. O sistema deverá permitir a alteração do status conforme o andamento do serviço e deverá manter as informações registradas durante o processo. Quando uma peça for utilizada, o sistema deverá atualizar automaticamente a quantidade disponível no estoque.

**RF07: Consultar informações e relatórios**
O sistema deverá permitir que os usuários consultem as informações de acordo com sua hierarquia de acesso. O Administrador poderá consultar informações gerais da empresa, incluindo clientes, equipamentos, técnicos, ordens de serviço e estoque. O Técnico poderá consultar suas próprias ordens de serviço e respectivas informações. O Cliente poderá consultar somente seus dados, seus equipamentos e suas ordens de serviço. O sistema também deverá apresentar relatórios textuais e indicadores simples diretamente no terminal, como quantidade de OS abertas, finalizadas e pendentes, além de informações sobre técnicos e peças utilizadas.

**RF08: Validar e manter os dados do sistema**
O sistema deverá validar as informações inseridas pelos usuários antes de realizar qualquer cadastro ou alteração. Deverá verificar campos obrigatórios, códigos existentes, relacionamentos entre registros e operações que possam gerar dados inconsistentes. As informações cadastradas deverão ser armazenadas de forma persistente, permitindo que os dados continuem disponíveis após o encerramento do programa.

## Requisitos Não Funcionais

**RNF01:** O sistema deverá ser desenvolvido utilizando a linguagem C.

**RNF02:** O sistema deverá funcionar exclusivamente através do terminal.

**RNF03:** A interface deverá utilizar menus textuais e opções numéricas.

**RNF04:** As informações deverão permanecer armazenadas mesmo após o encerramento do programa, podendo ser utilizados arquivos.

**RNF05:** As operações deverão apresentar resposta adequada para o volume de dados previsto.

**RNF06:** O sistema deverá restringir funcionalidades de acordo com o nível de acesso.

**RNF07:** O sistema deverá evitar o armazenamento de informações inválidas ou inconsistentes.

**RNF08:** O código deverá ser organizado utilizando funções e estruturas separadas.

**RNF09:** O sistema deverá ser executável em ambientes compatíveis com compiladores C.

**RNF10:** A interface textual deverá apresentar mensagens claras, menus organizados e instruções objetivas.

## Regras de Negócio

- **RN01:** Todo equipamento deverá estar vinculado a um cliente.
- **RN02:** Toda ordem de serviço deverá estar vinculada a um equipamento.
- **RN03:** Uma ordem de serviço poderá possuir um técnico responsável.
- **RN04:** Somente o Administrador poderá cadastrar técnicos.
- **RN05:** Somente o Administrador poderá cadastrar e alterar peças do estoque.
- **RN06:** O Técnico poderá consultar e alterar somente as informações técnicas permitidas para suas ordens.
- **RN07:** O Cliente poderá consultar somente suas próprias informações.
- **RN08:** Uma peça utilizada em uma OS deverá ser descontada do estoque.
- **RN09:** O sistema não deverá permitir utilização de quantidade superior à disponível no estoque.
- **RN10:** Uma OS finalizada não deverá continuar sendo alterada como uma ordem em andamento, salvo operações administrativas específicas.
- **RN11:** Uma OS cancelada deverá permanecer registrada no sistema para fins de histórico.
- **RN12:** O acesso às funcionalidades deverá respeitar a hierarquia do usuário.

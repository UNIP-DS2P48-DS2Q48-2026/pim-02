# Requisitos do Sistema

## Identificação do Projeto

- **Projeto:** Sistema de Gestão de Manutenção de Equipamentos.

- **Organização:** ByteTech - Empresa fictíca especializada em manutenção de computadores, notebooks, impressoras e equipamentos utilizados por pequenas empresas.

- **Tipo de sistema:** Sistema de gerenciamento executado exclusivamente em terminal.

- **Linguagem de programação:** C.

- **Interface:** Terminal/console.

- **Objetivo:** Desenvolver um sistema simples para auxiliar a empresa no controle de clientes, equipamentos, técnicos, recepcionistas, ordens de serviço e peças, substituindo o controle manual realizado por planilhas e anotações.

## Escopo do Sistema

O sistema será desenvolvido exclusivamente em linguagem C e funcionará por meio do terminal. Todas as operações deverão ser realizadas utilizando menus, opções numéricas, entradas de texto e mensagens exibidas no próprio terminal. O foco será desenvolver uma aplicação de console que permita representar os principais processos da empresa de manutenção, desde o atendimento ao cliente e abertura da ordem de serviço até a execução e conclusão da manutenção.

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

O sistema deverá permitir registrar informações mais completas, como data de entrada, responsável pelo atendimento, técnico responsável, diagnóstico, peças utilizadas, orçamento, aprovação e situação da OS.

## Hierarquia de Usuários

### Administrador

O Administrador possuirá acesso geral às funcionalidades administrativas do sistema, sendo responsável pela gestão dos usuários, dados e estoque.

Poderá:

- Gerenciar usuários.

- Cadastrar, alterar e consultar clientes.

- Cadastrar, alterar e consultar equipamentos.

- Cadastrar, alterar e consultar técnicos e recepcionistas.

- Consultar e editar ordens de serviço quando necessário.

- Gerenciar exclusivamente o estoque de peças.

- Cadastrar, alterar e consultar peças.

- Registrar entradas e saídas de peças.

- Consultar relatórios e indicadores.

O Administrador não será responsável pelo atendimento cotidiano dos clientes ou pela execução das manutenções, ficando essas atividades sob responsabilidade do Recepcionista e do Técnico.

### Recepcionista

O Recepcionista será responsável pelo atendimento ao cliente e pelo acompanhamento administrativo das ordens de serviço.

Poderá:

- Cadastrar e consultar clientes.

- Cadastrar e consultar equipamentos.

- Abrir ordens de serviço.

- Registrar o problema informado pelo cliente.

- Encaminhar ordens de serviço para os técnicos.

- Consultar o andamento das ordens de serviço.

- Registrar e consultar informações relacionadas ao orçamento.

- Registrar a aprovação ou não aprovação do orçamento.

- Atualizar informações administrativas da OS.

- Informar ao cliente sobre o andamento do serviço.

O Recepcionista não poderá alterar informações técnicas registradas pelo Técnico nem realizar operações de controle do estoque.

### Técnico

O Técnico possuirá acesso às funcionalidades relacionadas às ordens de serviço atribuídas a ele, podendo:

- Consultar suas ordens de serviço.

- Visualizar dados do equipamento e o problema informado.

- Registrar diagnóstico.

- Registrar peças utilizadas ou necessárias.

- Registrar o serviço realizado.

- Atualizar o andamento técnico da OS.

- Registrar observações técnicas.

- Finalizar a execução técnica de uma ordem de serviço.

O Técnico não deverá possuir acesso às funcionalidades administrativas do sistema ou ao controle do estoque.

### Cliente

O Cliente possuirá acesso limitado às informações relacionadas aos seus próprios equipamentos e ordens de serviço, podendo:

- Consultar seus dados.

- Consultar seus equipamentos.

- Consultar suas ordens de serviço.

- Consultar o status de uma OS.

- Consultar informações básicas sobre o serviço.

O Cliente não poderá alterar informações técnicas, administrativas ou de estoque.

## Requisitos Funcionais

**RF01: Gerenciar usuários e controle de acesso**

O sistema deverá permitir ao Administrador realizar o gerenciamento dos usuários, incluindo cadastro, consulta, alteração e remoção de usuários. Cada usuário deverá possuir uma hierarquia de acesso, sendo Administrador, Recepcionista, Técnico ou Cliente. O sistema deverá realizar a autenticação por meio de login e senha no terminal e, após a identificação do usuário, apresentar somente as funcionalidades permitidas para sua hierarquia.

**RF02: Gerenciar clientes e equipamentos**

O sistema deverá permitir o gerenciamento dos clientes e equipamentos cadastrados, incluindo cadastro, consulta, alteração e remoção dos registros conforme as permissões de cada usuário. Cada equipamento deverá estar vinculado a um cliente e deverá possuir informações como tipo, marca, modelo e número de série. O sistema deverá permitir consultar os equipamentos associados a determinado cliente e seu histórico de ordens de serviço.

**RF03: Gerenciar técnicos e recepcionistas**

O sistema deverá permitir ao Administrador realizar o gerenciamento dos técnicos e recepcionistas da empresa, incluindo cadastro, consulta, alteração e remoção dos registros. Para os técnicos, deverão ser armazenadas informações como nome, especialidade e situação. Para os recepcionistas, deverão ser armazenadas informações necessárias à identificação e ao acesso ao sistema.

**RF04: Gerenciar ordens de serviço**

O sistema deverá permitir ao Recepcionista realizar a abertura e o gerenciamento administrativo das ordens de serviço. Cada ordem de serviço deverá estar vinculada a um equipamento e deverá registrar informações como data de abertura, problema informado pelo cliente, responsável pelo atendimento, técnico responsável, orçamento, aprovação e status. O sistema deverá permitir localizar uma OS por diferentes critérios, como código, cliente, equipamento, técnico ou status. O Administrador também poderá consultar e editar os dados das ordens de serviço quando necessário.

**RF05: Executar manutenção da ordem de serviço**

O sistema deverá permitir que o Técnico acompanhe e atualize as ordens de serviço atribuídas a ele. Durante a execução da manutenção, o Técnico deverá poder registrar o diagnóstico, peças utilizadas, serviço realizado, observações técnicas e alterações no andamento da OS. O sistema deverá manter essas informações associadas à ordem de serviço para permitir a consulta posterior do histórico do atendimento.

**RF06: Controlar o ciclo e o histórico da ordem de serviço**

O sistema deverá controlar o ciclo de uma ordem de serviço por meio de diferentes estados, como Aberta, Em análise, Aguardando orçamento, Aguardando aprovação, Em manutenção, Aguardando peça, Pronta, Finalizada e Cancelada. O sistema deverá permitir a alteração do status conforme o andamento do serviço e deverá manter as informações registradas durante o processo. Quando uma peça for utilizada em uma OS, sua utilização deverá ser registrada para posterior atualização do estoque pelo Administrador.

**RF07: Gerenciar estoque de peças**

O sistema deverá permitir exclusivamente ao Administrador realizar o gerenciamento do estoque de peças. O Administrador poderá cadastrar, consultar, alterar e remover peças, além de registrar entradas e saídas de estoque. Cada peça deverá possuir informações como nome, categoria, quantidade disponível e preço. O sistema deverá impedir operações que resultem em quantidade negativa ou utilização de peças superior à quantidade disponível.

**RF08: Consultar informações e relatórios**

O sistema deverá permitir que os usuários consultem as informações de acordo com sua hierarquia de acesso. O Administrador poderá consultar informações gerais da empresa, incluindo clientes, equipamentos, usuários, técnicos, recepcionistas, ordens de serviço e estoque. O Recepcionista poderá consultar clientes, equipamentos e ordens de serviço necessárias para o atendimento. O Técnico poderá consultar suas próprias ordens de serviço e respectivas informações técnicas. O Cliente poderá consultar somente seus dados, seus equipamentos e suas ordens de serviço. O sistema também deverá apresentar relatórios textuais e indicadores simples diretamente no terminal, como quantidade de OS abertas, finalizadas e pendentes, além de informações sobre técnicos e peças utilizadas.

**RF09: Validar e manter os dados do sistema**

O sistema deverá validar as informações inseridas pelos usuários antes de realizar qualquer cadastro, alteração ou operação. Deverá verificar campos obrigatórios, códigos existentes, relacionamentos entre registros, permissões de acesso e operações que possam gerar dados inconsistentes. As informações cadastradas deverão ser armazenadas de forma persistente, permitindo que os dados continuem disponíveis após o encerramento do programa.

## Requisitos Não Funcionais

**RNF01:** O sistema deverá ser desenvolvido utilizando a linguagem C.

**RNF02:** O sistema deverá funcionar exclusivamente através do terminal.

**RNF03:** A interface deverá utilizar menus textuais e opções numéricas.

**RNF04:** As informações deverão permanecer armazenadas mesmo após o encerramento do programa, podendo ser utilizados arquivos.

**RNF05:** As operações deverão apresentar resposta adequada para o volume de dados previsto.

**RNF06:** O sistema deverá restringir funcionalidades de acordo com o nível de acesso do usuário.

**RNF07:** O sistema deverá evitar o armazenamento de informações inválidas ou inconsistentes.

**RNF08:** O código deverá ser organizado utilizando funções e estruturas separadas.

**RNF09:** O sistema deverá ser executável em ambientes compatíveis com compiladores C.

**RNF10:** A interface textual deverá apresentar mensagens claras, menus organizados e instruções objetivas.

## Regras de Negócio

- **RN01:** Todo equipamento deverá estar vinculado a um cliente.

- **RN02:** Toda ordem de serviço deverá estar vinculada a um equipamento.

- **RN03:** A abertura de uma ordem de serviço deverá ser realizada pelo Recepcionista.

- **RN04:** Uma ordem de serviço poderá ser atribuída a um Técnico para realização da manutenção.

- **RN05:** Somente o Administrador poderá cadastrar e gerenciar Técnicos e Recepcionistas.

- **RN06:** O Administrador poderá consultar e editar os dados do sistema quando necessário.

- **RN07:** Somente o Administrador poderá cadastrar, alterar e remover peças do estoque.

- **RN08:** Somente o Administrador poderá realizar operações que alterem diretamente a quantidade disponível no estoque.

- **RN09:** O Técnico poderá consultar e alterar somente as informações técnicas permitidas para suas ordens de serviço.

- **RN10:** O Recepcionista poderá consultar e alterar somente as informações administrativas permitidas para as ordens de serviço.

- **RN11:** Uma peça utilizada em uma OS deverá ser registrada e descontada do estoque pelo Administrador.

- **RN12:** O sistema não deverá permitir utilização de quantidade superior à disponível no estoque.

- **RN13:** Uma OS finalizada não deverá continuar sendo alterada como uma ordem em andamento, salvo operações administrativas específicas permitidas ao Administrador.

- **RN14:** Uma OS cancelada deverá permanecer registrada no sistema para fins de histórico.

- **RN15:** O Cliente poderá consultar somente suas próprias informações, equipamentos e ordens de serviço.

- **RN16:** O acesso às funcionalidades deverá respeitar a hierarquia do usuário.
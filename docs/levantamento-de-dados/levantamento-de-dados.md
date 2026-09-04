# Levantamento de Dados e Necessidades

Durante o levantamento de requisitos do sistema, foram analisadas as necessidades apresentadas pelo proprietário da empresa responsável pelos serviços de manutenção de equipamentos.

A partir das informações fornecidas, foram identificados os principais processos realizados no estabelecimento, desde o atendimento inicial do cliente até a conclusão da manutenção. O objetivo desse levantamento foi compreender como a empresa realiza atualmente seus serviços e identificar quais atividades poderiam ser organizadas e controladas pelo sistema.

Segundo o proprietário, o processo inicia-se com o **atendimento ao cliente**, realizado pelo Recepcionista. Nesse momento, são coletadas as informações do cliente, do equipamento e do problema apresentado. Quando necessário, uma nova ordem de serviço é aberta para registrar formalmente o atendimento.

Após a abertura da ordem de serviço, o equipamento é encaminhado para um **Técnico**, que fica responsável pela análise e execução da manutenção. O Técnico deve conseguir consultar as ordens atribuídas a ele, registrar o diagnóstico, informar os serviços realizados, registrar as peças utilizadas e atualizar o andamento do atendimento.

Durante esse processo, o **Recepcionista** continua responsável pelo contato com o cliente, principalmente em relação ao orçamento, aprovação do serviço e acompanhamento da situação da ordem de serviço.

Também foi identificado que o **estoque de peças** necessita de um controle específico. Conforme solicitado pelo proprietário, essa atividade ficará sob responsabilidade exclusiva do **Administrador**, que será responsável pelo cadastro das peças, controle das quantidades disponíveis e registro das entradas e saídas do estoque.

O **Administrador** terá uma função de gestão geral do sistema. Apesar de possuir acesso aos dados e poder realizar alterações quando necessário, ele não será responsável pelas atividades operacionais realizadas diariamente pelo Recepcionista e pelo Técnico. Dessa forma, cada perfil possuirá responsabilidades específicas, evitando a concentração das atividades em um único usuário.

Com base nesse levantamento, foram definidas as seguintes necessidades principais:

* Controle de acesso dos usuários conforme suas funções.
* Cadastro e gerenciamento de clientes.
* Cadastro e gerenciamento de equipamentos.
* Cadastro e gerenciamento de técnicos e recepcionistas.
* Abertura e acompanhamento de ordens de serviço.
* Registro de diagnósticos e serviços realizados.
* Acompanhamento do status das ordens de serviço.
* Controle de orçamento e aprovação do cliente.
* Controle de peças utilizadas nas manutenções.
* Gerenciamento do estoque pelo Administrador.
* Consulta do histórico dos atendimentos.
* Geração de consultas e indicadores sobre os serviços realizados.
* Persistência das informações para que os dados não sejam perdidos após o encerramento do sistema.

A partir dessas necessidades levantadas junto ao proprietário, a equipe transformou os processos identificados em **User Stories**, permitindo organizar as funcionalidades do sistema de acordo com o ponto de vista de cada usuário e facilitar o planejamento do desenvolvimento.

# User Stories

## US01 – Estrutura inicial do projeto

PBI relacionado: PBI01

Como equipe de desenvolvimento,
quero uma estrutura de projeto organizada com persistência em arquivo,
para começar o desenvolvimento das funcionalidades do sistema.

### Critérios de aceitação

* Projeto compila sem erros.
* Estrutura de pastas definida.
* Dados salvos em arquivo continuam disponíveis depois de reiniciar o programa.

## US02 – Autenticação

PBI relacionado: PBI02

Como usuário do sistema,
quero fazer login com usuário e senha,
para acessar o sistema de acordo com o meu perfil.

### Critérios de aceitação

* Permitir login com usuário e senha.
* Identificar o perfil do usuário: Administrador, Recepcionista, Técnico ou Cliente.
* Exibir mensagem quando o login for inválido.
* Exibir somente as funcionalidades permitidas para o perfil autenticado.

## US03 – Gerenciar clientes

PBI relacionado: PBI03

Como Administrador ou Recepcionista,
quero gerenciar os dados dos clientes,
para manter as informações dos clientes registradas e disponíveis para os atendimentos.

### Critérios de aceitação

* Permitir cadastrar cliente.
* Permitir consultar clientes.
* Permitir editar cliente conforme as permissões do usuário.
* Permitir excluir cliente conforme as permissões do usuário.
* Validar campos obrigatórios.
* Persistir os dados em arquivo.

## US04 – Gerenciar equipamentos

PBI relacionado: PBI04

Como Administrador ou Recepcionista,
quero gerenciar os equipamentos dos clientes,
para manter os equipamentos registrados e vinculados aos seus respectivos proprietários.

### Critérios de aceitação

* Permitir cadastrar equipamento.
* Permitir consultar equipamentos.
* Permitir editar equipamento conforme as permissões do usuário.
* Permitir excluir equipamento conforme as permissões do usuário.
* Vincular equipamento a um cliente.
* Impedir o cadastro de equipamento sem cliente vinculado.

## US05 – Gerenciar técnicos e recepcionistas

PBI relacionado: PBI05

Como Administrador,
quero gerenciar técnicos e recepcionistas,
para manter os funcionários responsáveis pelas atividades da empresa registrados no sistema.

### Critérios de aceitação

* Permitir cadastrar técnico.
* Permitir consultar técnicos.
* Permitir editar técnico.
* Permitir excluir técnico conforme as regras do sistema.
* Permitir cadastrar recepcionista.
* Permitir consultar recepcionistas.
* Permitir editar recepcionista.
* Permitir excluir recepcionista conforme as regras do sistema.

## US06 – Gerenciar peças e estoque

PBI relacionado: PBI06

Como Administrador,
quero gerenciar as peças e o estoque da empresa,
para controlar a disponibilidade dos componentes utilizados nas manutenções.

### Critérios de aceitação

* Permitir cadastrar peça.
* Permitir consultar peças.
* Permitir editar peça.
* Permitir excluir peça conforme as regras do sistema.
* Permitir registrar entrada de peças.
* Permitir registrar saída de peças.
* Controlar a quantidade disponível em estoque.
* Impedir quantidade negativa.
* Impedir saída superior à quantidade disponível.

## US07 – Gerenciar ordens de serviço

PBI relacionado: PBI07

Como Recepcionista,
quero abrir e gerenciar ordens de serviço,
para registrar os atendimentos realizados pela empresa e acompanhar o serviço solicitado pelo cliente.

### Critérios de aceitação

* Permitir abrir uma OS vinculada a um equipamento.
* Registrar o problema informado pelo cliente.
* Permitir atribuir a OS a um Técnico.
* Permitir consultar ordens de serviço.
* Permitir acompanhar o andamento da OS.
* Permitir registrar orçamento e aprovação.
* Permitir cancelar uma OS conforme as regras do sistema.
* Permitir ao Administrador consultar e editar os dados da OS quando necessário.

## US08 – Executar manutenção da ordem de serviço

PBI relacionado: PBI08

Como Técnico,
quero registrar as informações técnicas das minhas ordens de serviço,
para documentar o diagnóstico e os serviços realizados no equipamento.

### Critérios de aceitação

* Permitir consultar as OS atribuídas ao Técnico.
* Permitir visualizar os dados do equipamento e o problema informado.
* Permitir registrar diagnóstico.
* Permitir registrar peças utilizadas ou necessárias.
* Permitir registrar o serviço realizado.
* Permitir atualizar o andamento da manutenção.
* Permitir registrar observações técnicas.
* Permitir finalizar a execução técnica da OS.

## US09 – Controlar status e histórico da ordem de serviço

PBI relacionado: PBI09

Como usuário responsável pelo acompanhamento da OS,
quero acompanhar o status e o histórico da ordem de serviço,
para saber em que etapa o atendimento se encontra e manter o histórico do serviço.

### Critérios de aceitação

* Permitir alterar o status da OS conforme o andamento.
* Possibilitar estados como Aberta, Em análise, Aguardando orçamento, Aguardando aprovação, Em manutenção, Aguardando peça, Pronta, Finalizada e Cancelada.
* Manter o histórico das ordens finalizadas.
* Manter o histórico das ordens canceladas.
* Impedir alterações incompatíveis com o estado atual da OS.

## US10 – Consultar informações e relatórios

PBI relacionado: PBI10

Como usuário do sistema,
quero consultar informações de acordo com meu perfil,
para acompanhar os dados relevantes para minhas atividades.

### Critérios de aceitação

* Administrador consulta informações gerais do sistema.
* Recepcionista consulta clientes, equipamentos e ordens de serviço necessárias ao atendimento.
* Técnico consulta suas próprias ordens de serviço.
* Cliente consulta seus próprios dados, equipamentos e ordens de serviço.
* Sistema apresenta indicadores simples, como quantidade de OS abertas, em manutenção, finalizadas e canceladas.
* Sistema permite consultar informações relacionadas ao estoque para o Administrador.

## US11 – Validar dados do sistema

PBI relacionado: PBI11

Como usuário do sistema,
quero que meus dados sejam validados antes de serem salvos,
para evitar cadastros incompletos ou inconsistentes.

### Critérios de aceitação

* Validar campos obrigatórios nos cadastros.
* Verificar a existência de registros relacionados.
* Impedir relacionamentos inválidos.
* Impedir operações não permitidas para o perfil do usuário.
* Impedir operações que gerem quantidade negativa no estoque.
* Exibir mensagem de erro quando os dados forem inválidos.
* Manter os dados persistidos após o encerramento do programa.

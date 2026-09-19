# Levantamento de Dados e Necessidades

Durante o levantamento de requisitos do sistema, foram analisadas as necessidades apresentadas pelo proprietário da empresa responsável pelos serviços de manutenção de equipamentos.

A partir das informações fornecidas, foram identificados os principais processos realizados no estabelecimento, desde o atendimento inicial do cliente até a conclusão da manutenção. O objetivo desse levantamento foi compreender como a empresa realiza atualmente seus serviços e identificar quais atividades poderiam ser organizadas e controladas pelo sistema.

Segundo o proprietário, o processo inicia-se com o **atendimento ao cliente**, realizado pelo Recepcionista. Nesse momento, são coletadas as informações do cliente, do equipamento e do problema apresentado. Quando necessário, uma nova ordem de serviço é aberta para registrar formalmente o atendimento.

Após a abertura da ordem de serviço, o equipamento é encaminhado para um ou mais **Técnicos**, que ficam responsáveis pela análise e execução da manutenção. O(s) Técnico(s) devem conseguir consultar as ordens atribuídas a eles, registrar o diagnóstico, informar os serviços realizados e atualizar o andamento do atendimento, incluindo o teste de funcionamento após o reparo.

Durante esse processo, o **Recepcionista** continua responsável pelo contato com o cliente: elaboração do orçamento, registro da aprovação (ou negociação) do serviço, notificação da entrega, recebimento do pagamento e coleta do feedback do cliente.

O **Administrador** terá uma função de gestão geral do sistema, incluindo o cadastro da equipe (recepcionistas e técnicos). Apesar de possuir acesso aos dados e poder realizar alterações quando necessário, ele não será responsável pelas atividades operacionais realizadas diariamente pelo Recepcionista e pelo Técnico - exceto na ausência de um deles, quando poderá cobrir a função. Dessa forma, cada perfil possuirá responsabilidades específicas, evitando a concentração das atividades em um único usuário.

Com base nesse levantamento, foram definidas as seguintes necessidades principais:

* Controle de acesso dos usuários conforme suas funções.
* Cadastro e gerenciamento de usuários (administrador, recepcionista, técnico) e de clientes.
* Cadastro e gerenciamento de equipamentos, tanto de hardware quanto de software (licenças).
* Abertura e acompanhamento de ordens de serviço.
* Registro de diagnósticos e serviços realizados, com um ou mais técnicos por ordem de serviço.
* Elaboração de orçamento e registro da aprovação ou negociação do cliente.
* Acompanhamento do status das ordens de serviço.
* Registro do teste de funcionamento após o reparo.
* Registro de entrega, pagamento e feedback do cliente.
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
quero fazer login com e-mail e senha,
para acessar o sistema de acordo com o meu perfil.

### Critérios de aceitação

* Permitir login com e-mail e senha.
* Identificar o perfil (`role`) do usuário: Administrador, Recepcionista, Técnico ou Cliente.
* Exibir mensagem quando o login for inválido.
* Exibir somente as funcionalidades permitidas pela Matriz de Permissões.

## US03 – Gerenciar usuários

PBI relacionado: PBI03

Como Administrador,
quero gerenciar os usuários da equipe (administradores, recepcionistas e técnicos),
para manter a equipe registrada e com acesso adequado ao sistema.

### Critérios de aceitação

* Permitir cadastrar, consultar, editar e ativar/inativar usuários com perfil admin, recepcionista ou técnico.
* Registrar especialidade para usuários com perfil técnico.
* Validar campos obrigatórios.
* Persistir os dados em arquivo.

## US04 – Gerenciar equipamentos (hardware e software)

PBI relacionado: PBI04

Como Administrador ou Recepcionista,
quero gerenciar os equipamentos dos clientes, sejam de hardware ou de software,
para manter os equipamentos registrados e vinculados aos seus respectivos proprietários.

### Critérios de aceitação

* Permitir cadastrar equipamento, indicando a categoria (hardware ou software).
* Permitir consultar equipamentos.
* Permitir editar equipamento conforme as permissões do usuário.
* Permitir ativar/inativar equipamento conforme as permissões do usuário.
* Vincular equipamento a um cliente.
* Impedir o cadastro de equipamento sem cliente vinculado.

## US05 – Abrir ordem de serviço

PBI relacionado: PBI05

Como Recepcionista,
quero abrir uma ordem de serviço vinculada a um equipamento,
para registrar formalmente o atendimento e o problema relatado pelo cliente.

### Critérios de aceitação

* Permitir abrir uma OS vinculada a um equipamento existente.
* Permitir cadastrar o cliente durante o atendimento, caso ainda não exista.
* Registrar o problema informado pelo cliente e a data de abertura.
* Impedir abertura de OS sem equipamento vinculado.
* Permitir ao Administrador abrir a OS na ausência de um Recepcionista disponível.

## US06 – Registrar diagnóstico técnico

PBI relacionado: PBI06

Como Técnico,
quero registrar o diagnóstico de uma ordem de serviço atribuída a mim,
para documentar o problema identificado no equipamento.

### Critérios de aceitação

* Permitir consultar as OS atribuídas ao Técnico.
* Permitir visualizar os dados do equipamento e o problema informado.
* Permitir registrar o diagnóstico.

## US07 – Elaborar orçamento e registrar aprovação do cliente

PBI relacionado: PBI07

Como Recepcionista,
quero elaborar o orçamento e registrar a resposta do cliente,
para avançar a OS somente após a aprovação do serviço.

### Critérios de aceitação

* Permitir calcular e registrar o orçamento a partir do diagnóstico.
* Permitir registrar a aprovação do cliente.
* Permitir registrar negociação quando o cliente não aprovar, repetindo o ciclo até aprovação ou arquivamento.
* Permitir arquivar a OS quando não houver aprovação após a negociação.

## US08 – Executar manutenção com um ou mais técnicos

PBI relacionado: PBI08

Como Técnico,
quero registrar as ações realizadas durante o reparo,
para documentar o serviço executado no equipamento.

### Critérios de aceitação

* Permitir atribuir um ou mais técnicos à mesma OS.
* Permitir registrar as ações realizadas por cada técnico.
* Permitir registrar quando o reparo não funcionar e reiniciar a execução.
* Permitir atualizar o status da OS ao concluir o reparo.

## US09 – Registrar teste de funcionamento

PBI relacionado: PBI09

Como Técnico,
quero registrar o teste de funcionamento após o reparo,
para confirmar que o equipamento está pronto para entrega.

### Critérios de aceitação

* Permitir registrar o resultado do teste pós-reparo.
* Permitir aprovar internamente o teste, encaminhando a OS para entrega.
* Permitir reencaminhar para novo reparo quando o teste não for aprovado.

## US10 – Registrar entrega, pagamento e feedback do cliente

PBI relacionado: PBI10

Como Recepcionista,
quero registrar a entrega do equipamento, o pagamento e o feedback do cliente,
para concluir a ordem de serviço.

### Critérios de aceitação

* Permitir notificar o cliente e apresentar a OS.
* Permitir registrar o pagamento recebido.
* Permitir registrar o feedback do cliente (satisfeito ou não).
* Permitir reabrir a OS para correção quando o cliente não estiver satisfeito.
* Permitir finalizar a OS quando o cliente estiver satisfeito.

## US11 – Consultar informações e histórico

PBI relacionado: PBI11

Como usuário do sistema,
quero consultar informações de acordo com meu perfil,
para acompanhar os dados relevantes para minhas atividades.

### Critérios de aceitação

* Administrador consulta todas as ordens de serviço e usuários.
* Recepcionista consulta as ordens de serviço que atende.
* Técnico consulta as ordens de serviço que executa.
* Cliente consulta somente suas próprias ordens de serviço.
* Manter o histórico de ordens finalizadas e canceladas/arquivadas.

## US12 – Validar dados do sistema

PBI relacionado: PBI12

Como usuário do sistema,
quero que meus dados sejam validados antes de serem salvos,
para evitar cadastros incompletos ou inconsistentes.

### Critérios de aceitação

* Validar campos obrigatórios nos cadastros.
* Verificar a existência de registros relacionados (cliente, equipamento, OS).
* Impedir relacionamentos inválidos.
* Impedir operações não permitidas para o perfil do usuário.
* Exibir mensagem de erro quando os dados forem inválidos.
* Manter os dados persistidos após o encerramento do programa.

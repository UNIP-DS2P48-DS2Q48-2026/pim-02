# User Stories

## US01 – Estrutura inicial do projeto

PBI relacionado: PBI01

Como equipe de desenvolvimento,
quero uma estrutura de projeto organizada com persistência em arquivo,
para começar o desenvolvimento das funcionalidades do sistema.

### Critérios de aceitação

- Projeto compila sem erros.
- Estrutura de pastas definida.
- Dados salvos em arquivo continuam disponíveis depois de reiniciar o programa.

## US02 – Autenticação

PBI relacionado: PBI02

Como usuário do sistema,
quero fazer login com usuário e senha,
para acessar o sistema de acordo com o meu perfil.

### Critérios de aceitação

- Permitir login com usuário e senha.
- Identificar o perfil do usuário (Administrador, Técnico ou Cliente).
- Exibir mensagem quando o login for inválido.

## US03 – Gerenciar clientes

PBI relacionado: PBI03

Como Administrador,
quero gerenciar clientes,
para manter os dados dos clientes registrados no sistema.

### Critérios de aceitação

- Permitir cadastrar cliente.
- Permitir consultar clientes.
- Permitir editar cliente.
- Permitir excluir cliente.
- Validar campos obrigatórios.
- Persistir os dados no banco de dados/arquivo.

## US04 – Gerenciar equipamentos

PBI relacionado: PBI04

Como Administrador,
quero gerenciar equipamentos,
para manter os equipamentos dos clientes registrados.

### Critérios de aceitação

- Permitir cadastrar equipamento.
- Permitir consultar equipamentos.
- Permitir editar equipamento.
- Permitir excluir equipamento.
- Vincular equipamento a um cliente.

## US05 – Gerenciar técnicos

PBI relacionado: PBI05

Como Administrador,
quero gerenciar técnicos,
para manter a equipe técnica registrada no sistema.

### Critérios de aceitação

- Permitir cadastrar técnico.
- Permitir consultar técnicos.
- Permitir editar técnico.
- Permitir excluir técnico.

## US06 – Gerenciar peças

PBI relacionado: PBI06

Como Administrador,
quero gerenciar peças e estoque,
para controlar as peças utilizadas nas manutenções.

### Critérios de aceitação

- Permitir cadastrar peça.
- Permitir consultar peças.
- Permitir editar peça.
- Permitir excluir peça.
- Controlar a quantidade em estoque.

## US07 – Gerenciar ordens de serviço

PBI relacionado: PBI07

Como Administrador,
quero gerenciar ordens de serviço,
para registrar os atendimentos realizados pela empresa.

### Critérios de aceitação

- Permitir abrir uma OS vinculada a um equipamento.
- Permitir consultar ordens de serviço.
- Permitir alterar uma OS.
- Permitir cancelar uma OS.

## US08 – Executar manutenção da ordem de serviço

PBI relacionado: PBI08

Como Técnico,
quero registrar as informações da manutenção nas minhas OS,
para documentar o atendimento realizado.

### Critérios de aceitação

- Permitir consultar as OS atribuídas ao técnico.
- Permitir registrar diagnóstico e peças utilizadas.
- Permitir atualizar o andamento da OS.
- Permitir finalizar a OS.

## US09 – Controlar status da ordem de serviço

PBI relacionado: PBI09

Como Administrador ou Técnico,
quero acompanhar o status da OS,
para saber em que etapa o atendimento está.

### Critérios de aceitação

- Permitir alterar o status da OS conforme o andamento.
- Manter o histórico de OS finalizadas e canceladas.

## US10 – Consultar informações e relatórios

PBI relacionado: PBI10

Como usuário do sistema,
quero consultar informações de acordo com meu perfil,
para acompanhar os dados relevantes para mim.

### Critérios de aceitação

- Administrador consulta informações gerais do sistema.
- Técnico consulta suas próprias ordens de serviço.
- Cliente consulta seus próprios dados e ordens de serviço.
- Sistema apresenta indicadores simples (ex.: quantidade de OS abertas e finalizadas).

## US11 – Validar dados do sistema

PBI relacionado: PBI11

Como usuário do sistema,
quero que meus dados sejam validados antes de salvos,
para evitar cadastros incompletos ou inconsistentes.

### Critérios de aceitação

- Validar campos obrigatórios nos cadastros.
- Exibir mensagem de erro quando os dados forem inválidos.

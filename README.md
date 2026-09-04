# pim-02 — Sistema de Gestão de Manutenção de Equipamentos

**Empresa:** ByteTech (fictícia) &nbsp;|&nbsp; **Grupo:** UNIP-DS2P48-DS2Q48-2026

Projeto acadêmico (PIM) desenvolvido em **linguagem C** com interface em **terminal/console**, para controle dos processos de uma empresa de manutenção de equipamentos.

## Integrantes do grupo

| Papel | Nome | RA |
|---|---|---|
| P.O | Leonardo Lemgruber Portugal | R3625D5 |
| S.M | Elton Ruan da Silva Santos | R1508C1 |
| Dev | Arthur dos Santos Moraes | H9189A5 |
| Dev | Brunno Miranda da Silveira | H94GJG9 |
| Dev | Matheus Gonçalves da Silva | H987814 |
| Dev | Natália Nunes Vieira | R3129I8 |

## Sobre a empresa

**ByteTech** é uma empresa fictícia especializada em manutenção de computadores, notebooks, impressoras e demais equipamentos utilizados por pequenas empresas.

Atualmente o controle de clientes, equipamentos, técnicos, ordens de serviço e peças é feito por planilhas e anotações, o que dificulta localizar ordens de serviço, acompanhar seu andamento, controlar o estoque de peças e obter indicadores sobre os serviços realizados. O objetivo do sistema é substituir esse controle manual, organizando esses processos em uma aplicação de console.

Mais detalhes em [Requisitos do Sistema](./docs/requisitos/requisitos.md#identificação-do-projeto) e no [Levantamento de Dados](./docs/levantamento-de-dados/levantamento-de-dados.md).

## Perfis de usuário

O sistema possui quatro níveis de acesso, cada um com permissões específicas:

| Perfil | Responsabilidade principal |
|---|---|
| Administrador | Gestão de usuários, técnicos, recepcionistas e estoque de peças |
| Recepcionista | Atendimento ao cliente e gerenciamento administrativo das ordens de serviço |
| Técnico | Execução e registro técnico das ordens de serviço atribuídas a ele |
| Cliente | Consulta de seus próprios dados, equipamentos e ordens de serviço |

Detalhamento completo em [Requisitos do Sistema](./docs/requisitos/requisitos.md#hierarquia-de-usuários).

## Tecnologias

- **Linguagem:** C
- **Interface:** Terminal/console (menus textuais e opções numéricas)
- **Persistência:** Arquivos (dados mantidos após o encerramento do programa)

## Documentação

> A pasta `docs/` é modular: cada novo documento (requisito, diagrama, ata, sprint etc.) ganha sua própria subpasta e uma linha nova aqui.

### Levantamento e Requisitos

| Documento | Conteúdo |
|---|---|
| [Levantamento de Dados](./docs/levantamento-de-dados/levantamento-de-dados.md) | Entrevista com o proprietário da empresa, necessidades identificadas e User Stories derivadas delas |
| [Requisitos do Sistema](./docs/requisitos/requisitos.md) | Identificação do projeto, escopo, hierarquia de usuários, requisitos funcionais (RF), não funcionais (RNF) e regras de negócio (RN) |

### Planejamento (Scrum)

| Documento | Conteúdo |
|---|---|
| [Product Backlog](./docs/product-backlog/product-backlog.md) | Backlog priorizado (PBIs), vinculado aos requisitos, com prioridade e story points |
| [Sprint 01](./docs/sprints/sprints/sprint-01.md) | Objetivo, tarefas, review e retrospectiva da Sprint 01 |

Novas sprints devem ser adicionadas na mesma pasta (`sprint-02.md`, `sprint-03.md`, ...) e listadas aqui.

### Diagramas

| Documento | Conteúdo |
|---|---|
| [Diagrama de Classes (Astah)](./docs/astah/diagrama_de_classes.asta) | Modelo UML das classes do sistema (abrir com [Astah](https://astah.net/)) |

### Modelagem de Banco de Dados

| Documento | Conteúdo |
|---|---|
| [Modelagem do Banco de Dados](./docs/Modelagem_DB/) | Modelos do banco de dados (abrir com [brModelo](https://www.sis4win.com.br/brmodelo/)) |

### Documentação Acadêmica

| Documento | Conteúdo |
|---|---|
| [Manual PIM II - ADS (PDF)](<./docs/manual-pim/Manual_PIM_II - ADS.pdf>) | Manual acadêmico oficial exigido pela instituição |
| [Manual PIM II - ADS (Word)](<./docs/manual-pim/Manual_PIM_II - ADS.docx>) | Versão editável do manual acadêmico |

Backups periódicos da documentação em Word ficam em [`docs/backup-word/`](./docs/backup-word/) e não devem ser usados como referência — a fonte oficial é sempre a versão em Markdown listada acima.

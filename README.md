# pim-02 - Sistema de Gestão de Manutenção de Equipamentos

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

**ByteTech** é uma empresa fictícia especializada em manutenção de **hardware** (conserto de computadores, notebooks, impressoras e demais equipamentos) e de **software** (licenças, instalação e manutenção de programas) para **pessoas físicas**.

Atualmente o controle de clientes, equipamentos, técnicos e ordens de serviço é feito por planilhas e anotações, o que dificulta localizar ordens de serviço, saber quem é o responsável por cada uma, acompanhar o andamento do serviço (do diagnóstico à entrega) e consultar o histórico dos atendimentos. O objetivo do sistema é substituir esse controle manual por uma aplicação de console. Nesta versão os dados ficam em arquivos de texto (`.txt`); a gravação no banco de dados SQL Server é o próximo passo.

Mais detalhes em [Requisitos do Sistema](./docs/requisitos/requisitos.md#identificação-do-projeto) e no [Levantamento de Dados](./docs/levantamento-de-dados/levantamento-de-dados.md).

## Perfis de usuário

O sistema possui quatro níveis de acesso, cada um com permissões específicas:

| Perfil | Responsabilidade principal |
|---|---|
| Administrador | Gestão de usuários (técnicos, recepcionistas e outros administradores) e cobertura de qualquer etapa da OS na ausência de Recepcionista ou Técnico |
| Recepcionista | Atendimento ao cliente e gerenciamento administrativo das ordens de serviço |
| Técnico | Execução e registro técnico das ordens de serviço atribuídas a ele |
| Cliente | Consulta de seus próprios dados, equipamentos e ordens de serviço |

Detalhamento completo em [Requisitos do Sistema](./docs/requisitos/requisitos.md#hierarquia-de-usuários).

## Tecnologias

- **Linguagem:** C
- **Interface:** Terminal/console (menus textuais e opções numéricas)
- **Persistência:** nesta versão, arquivos de texto (`projeto/dados/*.txt`), um por tabela. O banco já está modelado em SQL Server ([`schema_sqlserver.sql`](./docs/Modelagem_DB/schema_sqlserver.sql)); ligar o programa em C a ele é o próximo passo

## Como executar

No terminal, de dentro da pasta `projeto` (é onde fica a pasta `dados/`):

```
cd projeto
gcc main.c -o output/main.exe
.outputmain.exe
```

Os usuários de exemplo usam a senha `byte123`: `ana@bytetech.com` (Administrador), `carla@bytetech.com` (Recepcionista), `rafael@bytetech.com` (Técnico) e `joao.pereira@email.com` (Cliente). Os dados de exemplo ficam em `projeto/dados/*.txt`; para voltar a eles depois de testar, use `git checkout projeto/dados`.

## Documentação

### Levantamento e Requisitos

| Documento | Conteúdo |
|---|---|
| [Levantamento de Dados](./docs/levantamento-de-dados/levantamento-de-dados.md) | Entrevista com o proprietário da empresa, necessidades identificadas e User Stories derivadas delas |
| [Requisitos do Sistema](./docs/requisitos/requisitos.md) | Identificação do projeto, escopo, perfis de usuário, requisitos funcionais (RF), não funcionais (RNF), regras de negócio (RN) e casos de uso detalhados |

### Planejamento (Scrum)

| Documento | Conteúdo |
|---|---|
| [Product Backlog](./docs/product-backlog/product-backlog.md) | Backlog priorizado (PBIs), vinculado aos requisitos e casos de uso, com prioridade e status |
| [Sprints](./docs/sprints/README.md) | Cadência semanal (1 semana = 1 sprint, fecha toda sexta), Definition of Done e o histórico de cada sprint (planejamento, review e retrospectiva) |

### Diagramas

| Documento | Conteúdo |
|---|---|
| [Diagrama Estendido](<./docs/Modalagem_casos_de_uso/2026-10-16-diagramas-de-casos-de-uso-e-estendido/Diagrama Estendido/Diagrama Estendido.png>) | Telas («boundary») e entidades («entity») do sistema, com seus relacionamentos. Feito a partir do diagrama de classes da UML: como C não tem classes, cada entidade é uma `struct` e cada tela é um arquivo `.c` com o seu menu |
| [Diagrama de Casos de Uso (Astah)](./docs/Modalagem_casos_de_uso/diagramas.asta) | Casos de uso por ator, separados em pacotes (abrir com [Astah](https://astah.net/)) |
| [Casos de Uso](./docs/Modalagem_casos_de_uso/descricoes-astah.md) | Descrição de cada elipse do diagrama no formato do Astah (ator, pré/pós-condição, fluxo principal, alternativo e de exceção), correlacionada com o fluxograma |

### Modelagem de Banco de Dados

| Documento | Conteúdo |
|---|---|
| [Dicionário de Dados](<./docs/dicionario-de-dados/Dicionário de Dados - ByteTech.pdf>) ([Word](<./docs/dicionario-de-dados/Dicionário de Dados - ByteTech.docx>)) | Resumo das 4 tabelas e dos relacionamentos, colunas de cada tabela (tipo, comprimento, restrições, valor padrão e descrição) e mapeamento das chaves estrangeiras, no formato pedido em Modelagem de Banco de Dados |
| [DER Simplificado](./docs/Modelagem_DB/der-simplificado.md) | As 4 tabelas do sistema (`usuario`, `equipamento`, `os`, `tecnico_os`), campos, regras de negócio reforçadas por trigger e diagrama entidade-relacionamento |
| [Modelagem do Banco de Dados](./docs/Modelagem_DB/) | Modelo anterior, mantido como histórico (abrir com [brModelo](https://www.sis4win.com.br/brmodelo/)) |

### Documentação Acadêmica

| Documento | Conteúdo |
|---|---|
| [Documento do PIM](./docs/documentacao-pim/20206-10-12/documentacao_pim_2_inicio.md) | Texto do trabalho em construção: resumo, introdução, caracterização da empresa, pesquisa, engenharia de software e planejamento ágil |
| [Manual PIM II - ADS (PDF)](<./docs/manual-pim/Manual_PIM_II - ADS.pdf>) | Manual acadêmico oficial exigido pela instituição |
| [Manual PIM II - ADS (Word)](<./docs/manual-pim/Manual_PIM_II - ADS.docx>) | Versão editável do manual acadêmico |

O backup não se limita à documentação em Word: todo o conteúdo do repositório é versionado pelo Git, com histórico completo de alterações. O texto do trabalho é escrito em Markdown em [`docs/documentacao-pim/`](./docs/documentacao-pim/) e depois passado para o formato do manual.

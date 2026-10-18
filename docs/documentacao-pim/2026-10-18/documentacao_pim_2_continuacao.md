# Resumo

Este trabalho apresenta o planejamento e o desenvolvimento de um sistema de gestão de manutenção de equipamentos para a ByteTech, empresa fictícia de assistência técnica em informática. No cenário estudado, o uso de planilhas e anotações separadas dificulta a localização das ordens de serviço, a identificação dos responsáveis e a consulta ao histórico dos atendimentos. O projeto tem como objetivo organizar essas informações em uma aplicação em linguagem C, com interface em terminal e atendimento voltado a pessoas físicas. A pesquisa possui finalidade aplicada e abordagem qualitativa e exploratória, apoiada na análise dos documentos do projeto e em publicações sobre gestão da informação, informatização de pequenas empresas e gerenciamento de serviços de tecnologia. A análise resultou na definição de requisitos, regras de negócio, casos de uso e itens do backlog, com planejamento baseado em Scrum. A solução prevê os perfis Administrador, Recepcionista, Técnico e Cliente, além do registro de diagnóstico, orçamento, decisão do cliente, manutenção, teste e entrega. O controle de estoque está fora do escopo. Espera-se facilitar o acompanhamento dos serviços e a recuperação das informações registradas. Como o desenvolvimento ainda está em andamento, esses benefícios dependem da conclusão e da avaliação da aplicação, sem resultados de implantação em uma empresa real.

**Palavras-chave:** ordens de serviço; manutenção de equipamentos; gestão da informação; linguagem C; engenharia de software.

# Abstract

This paper presents the planning and development of an equipment maintenance management system for ByteTech, a fictional computer repair company. In the scenario studied, the use of separate spreadsheets and notes makes it difficult to locate work orders, identify the staff responsible for them, and review service history. The project aims to organize this information in a C application with a terminal interface, designed to serve individual customers. The research is applied, qualitative, and exploratory, drawing on project documents and publications on information management, computerization in small businesses, and IT service management. The analysis led to the definition of requirements, business rules, use cases, and backlog items, with planning based on Scrum. The proposed solution includes Administrator, Receptionist, Technician, and Customer roles, as well as records of diagnosis, quotations, customer decisions, maintenance, testing, and delivery. Inventory control is outside the project scope. The system is expected to make it easier to track services and retrieve recorded information. As development is still in progress, these benefits depend on the completion and evaluation of the application, and no results from deployment in a real company are reported.

**Keywords:** work orders; equipment maintenance; information management; C programming language; software engineering.

# Introdução

O atendimento em uma assistência técnica produz informações desde o recebimento do equipamento até sua devolução ao cliente. A descrição do defeito, o diagnóstico, o orçamento e os registros de manutenção precisam permanecer relacionados ao mesmo serviço. Quando esses dados são mantidos em controles separados, acompanhar o atendimento pode exigir a consulta a diferentes documentos e aos profissionais envolvidos.

Este trabalho apresenta a proposta de um sistema de gestão de manutenção de equipamentos para a ByteTech, empresa fictícia do setor de Tecnologia da Informação. No cenário definido para o projeto, a empresa utiliza planilhas e anotações para controlar seus atendimentos. A dispersão dos registros dificulta a localização das ordens de serviço, a identificação dos responsáveis e a consulta ao histórico de manutenção.

A aplicação proposta utiliza linguagem C e interface em terminal. A solução prevê quatro perfis de acesso: Administrador, Recepcionista, Técnico e Cliente. Cada perfil terá permissões relacionadas às atividades que desempenha no atendimento. Nesta versão, o sistema contempla clientes pessoas físicas.

Para fundamentar a proposta, foram analisados os documentos de caracterização da empresa, os requisitos do projeto e publicações sobre gestão da informação e informatização de pequenas empresas. Também foram consultados trabalhos relacionados à gestão de serviços de tecnologia. A análise desses materiais orienta a definição das funcionalidades e a discussão dos benefícios esperados.

A ByteTech constitui um cenário acadêmico, e as dificuldades descritas fazem parte dessa caracterização. Assim, a contribuição da solução será discutida a partir das necessidades estabelecidas e das evidências disponíveis durante o desenvolvimento, sem atribuir à empresa resultados de uma implantação real.

## Objetivo geral

Desenvolver um sistema de gestão de manutenção de equipamentos para a ByteTech, em linguagem C e com interface em terminal, que permita organizar os cadastros e acompanhar as ordens de serviço de clientes pessoas físicas, de acordo com as responsabilidades de cada perfil de usuário.

## Objetivos específicos

- Caracterizar a ByteTech e seus processos de atendimento, identificando as dificuldades de registro e consulta das informações no cenário definido para a empresa.

- Fundamentar a proposta por meio de pesquisa bibliográfica e análise dos documentos do projeto, explicitando os procedimentos metodológicos adotados.

- Definir os requisitos, as regras de negócio, os casos de uso e a arquitetura do sistema, organizando o desenvolvimento com base em práticas de Engenharia de Software Ágil.

- Modelar o banco de dados relacional para os registros de usuários, equipamentos e ordens de serviço, estabelecendo seus relacionamentos e discutindo possibilidades de utilização de bancos NoSQL.

- Desenvolver algoritmos estruturados e implementar em linguagem C as funcionalidades essenciais de cadastro, controle de acesso e acompanhamento das ordens de serviço, conforme as permissões dos usuários.

- Elaborar uma proposta de infraestrutura de redes para a ByteTech, incluindo medidas de segurança da informação e possibilidades de uso de sistemas distribuídos.

- Analisar aspectos de acessibilidade, inclusão digital, privacidade e respeito aos direitos humanos relacionados ao uso do sistema e ao tratamento das informações dos usuários.

- Verificar as funcionalidades desenvolvidas por meio de testes, registrar as limitações encontradas e indicar melhorias e cuidados para uma futura implantação.

# 1. Desenvolvimento

## 1.1 Caracterização da organização

### 1.1.1 Identificação e ramo de atuação

A ByteTech é uma empresa fictícia de assistência técnica e suporte em Tecnologia da Informação. Seus serviços abrangem manutenção de computadores, notebooks e impressoras, instalação e configuração de programas e suporte a redes. A organização foi escolhida como destinatária da solução desenvolvida pelo grupo neste Projeto Integrado Multidisciplinar.

O atendimento depende da participação de diferentes responsáveis. A recepção reúne os dados iniciais, a equipe técnica examina o equipamento e o cliente decide sobre o orçamento apresentado. As informações produzidas nessas etapas precisam acompanhar a ordem de serviço para que seja possível consultar o trabalho realizado e dar continuidade ao atendimento.

### 1.1.2 Missão e principais serviços

A missão definida para a ByteTech é oferecer assistência técnica e suporte em tecnologia com qualidade, agilidade, transparência e preço justo, buscando soluções confiáveis para as necessidades dos clientes.

Entre os serviços oferecidos estão diagnóstico de falhas, reparo e atualização de equipamentos, troca de componentes, formatação, instalação de sistemas e remoção de programas maliciosos. A empresa também realiza cópias de segurança, recuperação de dados, configuração de redes e suporte presencial ou remoto.

A transparência no atendimento envolve apresentar o orçamento antes da execução do serviço e registrar as informações necessárias para explicar o reparo ao cliente. Esses cuidados estão relacionados à proposta do sistema, que deverá permitir o acompanhamento das decisões tomadas durante a manutenção.

### 1.1.3 Estrutura organizacional

A estrutura da ByteTech contempla direção e gestão, setor técnico, atendimento ao cliente, setor comercial, financeiro e recursos humanos. A direção responde pelo planejamento e pelas decisões administrativas. O setor técnico realiza diagnósticos, reparos, configurações e testes. O atendimento recebe as solicitações e mantém a comunicação com os clientes.

O setor comercial acompanha os orçamentos e a contratação dos serviços. O financeiro controla pagamentos, recebimentos e custos. Recursos humanos cuida da contratação, integração e gestão dos colaboradores.

Para o sistema, foram definidos quatro perfis de usuário. O Administrador gerencia usuários e pode assumir atividades da recepção ou da equipe técnica quando necessário. O Recepcionista realiza o atendimento e os registros administrativos. O Técnico registra as atividades das ordens atribuídas a ele. O Cliente consulta seus próprios dados e atendimentos, decide sobre o orçamento e fornece sua avaliação do serviço.

Os setores da empresa não correspondem, obrigatoriamente, a módulos da aplicação. O projeto se concentra na rotina de manutenção e não abrange toda a gestão comercial, financeira ou de recursos humanos.

### 1.1.4 Principais processos de negócio

No fluxo definido para a ByteTech, o atendimento começa com a descrição do problema pelo cliente. A recepção verifica seu cadastro, registra o equipamento e abre uma ordem de serviço. O atendimento é então encaminhado para diagnóstico.

O técnico examina o equipamento e registra suas observações. O diagnóstico serve de base para a elaboração do orçamento, que é apresentado ao cliente para aprovação ou recusa. Caso seja necessária uma negociação, a proposta poderá ser revista antes da continuidade do serviço.

Após a aprovação, ocorre a manutenção. Uma ordem de serviço poderá envolver mais de um técnico, conforme a necessidade do atendimento. Ao concluir o reparo, a equipe realiza testes de funcionamento. Se o equipamento ainda apresentar o problema, o serviço retorna para correção.

A entrega envolve o registro do pagamento e do retorno do cliente sobre o atendimento. As informações da ordem permanecem disponíveis para consultas posteriores. As peças utilizadas poderão ser mencionadas nas observações do serviço, sem controle de quantidades, entradas ou saídas.

### 1.1.4 Público-alvo e delimitação do projeto

A caracterização da ByteTech considera o atendimento de pessoas físicas e de pequenas e médias empresas. Entre seus clientes estão usuários domésticos, estudantes, profissionais em trabalho remoto, comerciantes, escritórios e consultórios.

A primeira versão do sistema, conforme os requisitos do projeto, está delimitada ao atendimento de pessoas físicas. Esse recorte permite concentrar o desenvolvimento no cadastro de clientes e equipamentos e no acompanhamento das ordens de serviço. O atendimento a pessoas jurídicas faz parte da atuação descrita para a empresa, mas não está incluído nesta versão da aplicação.

### 1.1.5 Problemas identificados

No cenário estudado, os registros de atendimento são distribuídos entre planilhas e anotações. Essa organização dificulta localizar uma ordem, identificar seu responsável e verificar quais atividades já foram realizadas. A consulta a diagnósticos anteriores também depende da recuperação de registros que podem estar separados.

A comunicação entre recepção e equipe técnica é afetada por essa dispersão. Para responder ao cliente sobre o andamento do serviço, o recepcionista pode precisar consultar diretamente o técnico. A falta de um registro atualizado aumenta a dependência dessa comunicação e dificulta o acompanhamento quando o responsável não está disponível.

### 1.1.6 Justificativa da solução proposta

A proposta busca reunir os dados de clientes, equipamentos, usuários e ordens de serviço em uma aplicação em linguagem C. O sistema deverá permitir o registro das informações de diagnóstico, orçamento, aprovação, manutenção, teste e entrega, conforme as permissões de cada perfil.

Espera-se facilitar a localização dos atendimentos e a consulta ao histórico dos equipamentos. O controle de estoque foi retirado do escopo. O registro de pagamento previsto na ordem de serviço também não representa processamento de transações bancárias ou um módulo financeiro completo. A finalidade desta versão é apoiar o acompanhamento da manutenção.

# 2. Pesquisa científica e inovação

## 2.1 Definição do problema

O problema de pesquisa está na dificuldade de organizar e consultar as informações produzidas durante os atendimentos da ByteTech. Uma mesma ordem reúne dados fornecidos pelo cliente, observações técnicas, valores e decisões sobre a execução do serviço. A separação desses registros prejudica a continuidade do acompanhamento entre os envolvidos.

A pesquisa parte da seguinte questão: como um sistema de gestão de manutenção pode contribuir para organizar as informações e acompanhar as ordens de serviço no cenário definido para a ByteTech?

## 2.2 Objetivos da pesquisa

O objetivo geral da pesquisa é analisar como a informatização dos registros pode apoiar a organização das informações e o acompanhamento dos serviços da ByteTech, orientando a definição do sistema proposto.

Os objetivos específicos são:

- identificar as dificuldades de registro e consulta descritas no cenário da empresa;

- relacionar as necessidades de informação às responsabilidades dos usuários;

- estudar publicações sobre gestão da informação, informatização e processos empresariais;

- analisar trabalhos relacionados à informatização de pequenas empresas e à gestão de serviços de tecnologia;

- fundamentar as funcionalidades propostas e delimitar as contribuições esperadas do projeto.

## 2.3 Metodologia utilizada

A pesquisa tem finalidade aplicada, pois busca orientar uma solução para um problema definido no projeto. A abordagem é qualitativa e exploratória, concentrada na compreensão dos processos, das necessidades de informação e das responsabilidades dos usuários.

Foram utilizados dois conjuntos de materiais. O primeiro reúne a caracterização da ByteTech e os documentos produzidos pelo grupo, como levantamento de necessidades, requisitos, casos de uso e modelagem dos dados. Esses registros permitiram identificar o fluxo do atendimento e delimitar o que será contemplado pelo sistema.

O segundo conjunto é formado por artigos e trabalhos acadêmicos sobre gestão da informação, informatização de pequenas empresas e gerenciamento de serviços de tecnologia. A seleção partiu das referências reunidas na etapa de pesquisa do projeto. Os dados bibliográficos e o conteúdo utilizado na redação foram conferidos em páginas de periódicos e em repositórios institucionais. Trata-se de uma pesquisa bibliográfica direcionada ao tema, sem a pretensão de abranger todas as publicações existentes.

Os trabalhos foram analisados considerando o problema abordado, sua relação com o projeto e os limites de aplicação dos resultados. Em seguida, os conceitos estudados foram associados às necessidades documentadas para a ByteTech. Essa relação fundamenta as decisões apresentadas nas seções seguintes.

A análise utiliza um cenário fictício. Não são apresentados resultados de entrevistas com funcionários de uma empresa real nem medições de desempenho após implantação. Os benefícios discutidos são expectativas que ainda dependem da conclusão e da avaliação do sistema.

## 2.4 Fundamentação bibliográfica

### 2.4.1 Gestão da informação em pequenas empresas

Moraes e Escrivão Filho (2006) relacionam a gestão da informação à identificação das necessidades, à obtenção, ao tratamento, à distribuição e ao uso das informações. O estudo também discute características das pequenas empresas que afetam esse processo, como a informalidade e a concentração de responsabilidades. Nessas condições, a recuperação de uma informação pode depender de quem a conhece ou de como ela foi registrada.

Essa discussão ajuda a compreender o problema definido para a ByteTech. O diagnóstico e a decisão do cliente precisam permanecer registrados mesmo quando o profissional que acompanhou o atendimento não está disponível. Por isso, a proposta prevê que essas informações estejam vinculadas à ordem de serviço e possam ser consultadas pelos usuários autorizados.

### 2.4.2 Informatização e necessidades da organização

Beraldi e Escrivão Filho (2000) mostram que a adoção da tecnologia precisa considerar as necessidades da empresa e a preparação para seu uso. O estudo indica que a aquisição de recursos de informática, por si só, não assegura mudanças relevantes na gestão. Planejamento e capacitação fazem parte das condições necessárias para o aproveitamento desses recursos.

No projeto da ByteTech, essa relação orienta a escolha das funcionalidades. A abertura de ordens, a atribuição de técnicos e a consulta ao histórico respondem a dificuldades descritas no levantamento. A utilidade da aplicação deverá ser avaliada pela forma como esses recursos atendem ao fluxo estabelecido para a empresa.

### 2.4.3 Fluxo de informações e processos de negócio

Bannitz e Zafalon (2026) investigam a relação entre fluxo informacional e gestão de processos de negócio por meio de um mapeamento da literatura. O trabalho aproxima a organização das informações das atividades executadas e das necessidades de gestão dos processos.

Na ByteTech, o orçamento utiliza informações do diagnóstico, enquanto a manutenção depende da decisão do cliente sobre a proposta apresentada. Esses vínculos orientam a sequência de registros prevista para a ordem de serviço. A definição de quem registra e de quem consulta cada informação também ajuda a estabelecer as permissões dos perfis de usuário.

### 2.4.4 Integração das informações

Mendes e Escrivão Filho (2002) analisam a adoção de sistemas integrados de gestão em pequenas empresas. Entre os aspectos discutidos estão a integração das informações e as dificuldades de preparação, implantação e treinamento. Os resultados reforçam a necessidade de considerar os processos da organização durante a adoção do sistema.

Embora o projeto da ByteTech tenha um alcance menor que o de um sistema integrado de gestão empresarial, a discussão é útil para sua modelagem. Clientes, equipamentos, ordens e técnicos possuem relações que precisam ser preservadas. Reunir esses registros deverá permitir, por exemplo, consultar a qual cliente pertence um equipamento e quais técnicos participam de uma ordem. A proposta limita essa integração às atividades de manutenção definidas no escopo.

## 2.5 Análise de trabalhos relacionados

### 2.5.1 Informatização no gerenciamento de microempresas

Trigueiro e Elizeu (2024) analisam o uso de sistemas informatizados em cinco microempresas de Cuiabá, incluindo uma assistência técnica de impressoras. As entrevistas com gestores apontam percepções de simplificação e agilidade dos controles, mas os benefícios variam conforme a necessidade atendida. O estudo possui uma amostra pequena e apresenta avaliações dos participantes, o que limita a generalização dos resultados.

A pesquisa contribui para este projeto ao mostrar que a análise de uma solução deve considerar sua utilidade na rotina da organização. Para a ByteTech, isso significa relacionar as funcionalidades às dificuldades registradas no levantamento. As percepções relatadas pelos gestores no estudo não constituem resultados já obtidos pelo sistema deste PIM.

### 2.5.2 GestaoOS: sistema de gerenciamento de serviços de TI

Scheffer e Tibola (2025) apresentam o GestaoOS, um sistema web voltado à gestão de serviços de tecnologia em pequenas e médias assistências técnicas. A proposta inclui registros de clientes e equipamentos, imagens, acessórios, assinatura digital e aprovação de orçamento por link.

O trabalho se aproxima do projeto da ByteTech pelo acompanhamento dos serviços e pelo registro das informações do atendimento. Há diferenças na interface e nos recursos previstos: a aplicação deste PIM utiliza menus em terminal e não inclui aprovação por link, assinatura digital ou anexos de imagens. A comparação ajuda a delimitar a proposta acadêmica, sem estabelecer superioridade entre os sistemas.

O Quadro 1 reúne as principais contribuições desses trabalhos para o projeto.

Quadro 1 — Relação entre os trabalhos consultados e a proposta para a ByteTech

| Trabalho | Foco do estudo | Contribuição para o projeto |
|---|---|---|
| Trigueiro e Elizeu (2024) | Uso da informatização em microempresas | Orienta a análise da utilidade do sistema a partir das necessidades da organização. |
| Scheffer e Tibola (2025) | Sistema web de gerenciamento de serviços de TI | Oferece uma referência para comparar o fluxo de atendimento e delimitar os recursos da aplicação em terminal. |

Fonte: elaboração própria, com base em Trigueiro e Elizeu (2024) e Scheffer e Tibola (2025).

## 2.6 Fundamentação das decisões do projeto

A pesquisa sustenta três decisões centrais da proposta. A primeira é manter relacionados os registros de cliente, equipamento e ordem de serviço. Essa organização responde à dificuldade de localizar informações e consultar atendimentos anteriores.

A segunda é definir responsabilidades de acesso e atualização. Recepcionistas, técnicos, administradores e clientes participam do atendimento de formas diferentes. As permissões deverão refletir essas funções, limitando o acesso do cliente às informações e operações de seus próprios atendimentos.

A terceira é registrar a continuidade do serviço, do recebimento à entrega. Para isso, o modelo atual estabelece quatro situações para a ordem: aberta, em andamento, finalizada e cancelada. Diagnóstico, orçamento, aprovação, manutenção e teste são registrados nos campos correspondentes da ordem e não constituem situações adicionais.

Essas decisões resultam da relação entre a literatura consultada e os requisitos definidos pelo grupo. Sua adequação prática deverá ser verificada em testes que representem o fluxo completo de atendimento, incluindo recusas de orçamento, cancelamentos e restrições de acesso.

## 2.7 Inovação e contribuições esperadas

A contribuição pretendida para a ByteTech está na mudança da forma de acompanhar os serviços. No cenário inicial, os responsáveis dependem de registros dispersos e de consultas diretas entre setores. A proposta reúne as informações em uma estrutura comum e estabelece regras para sua atualização.

Entre as características previstas estão a participação de mais de um técnico em uma ordem, a preservação do histórico de atendimentos e o acesso conforme o perfil do usuário. Essas escolhas procuram atender às necessidades do cenário estudado. Não são apresentadas como funcionalidades inéditas no mercado.

Espera-se que a organização dos registros facilite a consulta ao andamento do serviço e às manutenções anteriores. A confirmação desses benefícios depende da implementação e da avaliação da aplicação. O projeto também possui limites: utiliza interface em terminal, atende pessoas físicas e não contempla estoque ou gestão financeira completa. Esses limites devem orientar a avaliação dos resultados e as possibilidades de ampliação futura.

# 3. Engenharia de software

A definição do sistema partiu das atividades descritas para a ByteTech e das informações necessárias em cada atendimento. O levantamento foi organizado em requisitos funcionais, requisitos não funcionais, regras de negócio e casos de uso. Esses documentos servem de referência para o planejamento das tarefas e para a implementação em C.

Os identificadores utilizados pelo grupo foram mantidos nesta seção para permitir a consulta entre os documentos. A sigla RF identifica um requisito funcional; RNF, um requisito não funcional; RN, uma regra de negócio; e UC, um caso de uso. A especificação atual reúne dez requisitos funcionais, dez não funcionais, dezenove regras de negócio e quarenta e quatro casos de uso.

## 3.1 Requisitos funcionais

Os requisitos funcionais descrevem as operações previstas para os usuários. No projeto, essas operações acompanham o atendimento desde o cadastro até a entrega e a consulta ao histórico. O Quadro 2 apresenta uma síntese dos requisitos documentados.

Quadro 2 — Requisitos funcionais do sistema

| Código | Funcionalidade prevista |
|---|---|
| RF01 | Autenticar por e-mail e senha, controlar o acesso por perfil e permitir a alteração da própria senha. |
| RF02 | Cadastrar, consultar, alterar e ativar ou inativar usuários. Registrar a especialidade dos técnicos e permitir o cadastro de clientes pela recepção. |
| RF03 | Gerenciar equipamentos de hardware ou software, com identificação, categoria e vínculo com o cliente. |
| RF04 | Abrir uma ordem de serviço com equipamento, data, problema relatado e atendente responsável. |
| RF05 | Registrar observações técnicas e o diagnóstico final necessário para a elaboração do orçamento. |
| RF06 | Elaborar e apresentar o orçamento, registrando aprovação, recusa ou negociação com o cliente. |
| RF07 | Atribuir um ou mais técnicos à ordem e registrar as ações de manutenção. |
| RF08 | Registrar o resultado dos testes de funcionamento após o reparo. |
| RF09 | Indicar a conclusão do serviço e registrar entrega, pagamento recebido e avaliação do cliente. |
| RF10 | Consultar registros e histórico conforme o perfil, incluindo filtros e contagens operacionais dos atendimentos. |

Fonte: elaboração própria, com base nos requisitos do projeto (2026).

A apresentação do orçamento e o aviso de conclusão ocorrem no contexto da consulta ao atendimento. Não há previsão de envio automático por e-mail ou aplicativos de mensagem. Da mesma forma, as contagens operacionais do RF10 se limitam aos serviços registrados e não constituem relatórios financeiros ou de custos.

## 3.2 Requisitos não funcionais

Os requisitos não funcionais estabelecem condições para a construção e o uso da aplicação. Eles abrangem a tecnologia adotada, a organização do código, a persistência dos registros e a validação das operações, conforme o Quadro 3.

Quadro 3 — Requisitos não funcionais do sistema

| Código | Condição estabelecida |
|---|---|
| RNF01 | Utilizar a linguagem C. |
| RNF02 | Executar exclusivamente em terminal. |
| RNF03 | Apresentar menus textuais e opções numéricas. |
| RNF04 | Manter os dados após o encerramento do programa, utilizando SQL Server ou arquivos de texto. |
| RNF05 | Oferecer tempo de resposta adequado ao volume de dados previsto. |
| RNF06 | Organizar o código em funções e estruturas separadas. |
| RNF07 | Executar em ambientes compatíveis com compiladores C. |
| RNF08 | Apresentar mensagens claras, menus organizados e instruções objetivas. |
| RNF09 | Validar campos obrigatórios, relacionamentos e permissões antes de cadastrar, alterar ou remover registros. |
| RNF10 | Armazenar senhas por meio de hash, sem mantê-las em texto simples. |

Fonte: elaboração própria, com base nos requisitos do projeto (2026).

Esses itens representam condições que a aplicação deverá atender. Sua presença na especificação não comprova o atendimento pelo programa. O requisito de desempenho, por exemplo, ainda precisa de uma definição de volume e de tempo de resposta para orientar sua avaliação. A proteção das senhas também deverá ser analisada a partir do mecanismo efetivamente utilizado.

## 3.3 Regras de negócio e permissões

As regras de negócio estabelecem vínculos e restrições entre as operações. Todo equipamento deverá pertencer a um cliente, e toda ordem deverá estar vinculada a um equipamento. A atribuição de técnicos permite a participação de mais de um profissional, mantendo a identificação dos responsáveis pelo serviço.

A matriz de permissões diferencia as atividades dos perfis. A gestão da equipe cabe ao Administrador. O Recepcionista conduz o cadastro de clientes e os registros administrativos do atendimento. O Técnico atua nas ordens atribuídas a ele, registrando informações técnicas. O Cliente acessa seus próprios atendimentos, decide sobre o orçamento e fornece sua avaliação. O Administrador pode assumir atividades da recepção ou da equipe técnica na situação de substituição prevista pela RN11.

A preservação do histórico também orienta as operações. O cancelamento de uma ordem não deverá eliminar seu registro. Usuários e equipamentos deverão ser inativados em vez de excluídos fisicamente. As regras ainda impedem o uso de cadastros inativos em novas ordens e a inativação de registros vinculados a ordens abertas ou em andamento.

O diagnóstico final é necessário para elaborar o orçamento. Quando não for encontrado defeito, a RN18 prevê orçamento de valor zero, aprovação do cliente, teste e entrega sem cobrança. Esse percurso mantém o registro do atendimento mesmo quando não há reparo a executar.

## 3.4 Casos de uso

Os casos de uso detalham as interações entre o usuário e o sistema. As descrições produzidas pelo grupo informam atores, condições de entrada, sequência principal, alternativas, exceções e resultado esperado. A seguir, são apresentados três exemplos que ajudam a compreender a especificação.

No **UC01 — Realizar Login**, um usuário informa seu e-mail e sua senha. O sistema deverá localizar o cadastro, conferir as credenciais e verificar se ele está ativo. Com a autenticação concluída, será apresentado o menu do perfil correspondente. Credenciais inválidas ou cadastro inativo impedem o acesso.

O **UC24 — Criar Ordem de Serviço** é iniciado pelo Recepcionista ou pelo Administrador na substituição prevista. O atendente informa o CPF do cliente, seleciona um equipamento já cadastrado e registra o problema relatado. O sistema deverá verificar os vínculos e a situação dos cadastros antes de abrir a ordem com data, responsável e situação inicial aberta. A ausência de cliente ou equipamento cadastrado impede a abertura; nesses casos, os cadastros devem ser realizados antes, por seus casos de uso específicos. Também impedem a operação os cadastros inativos e a ausência da descrição do problema.

No **UC34 — Registrar Teste de Funcionamento**, o Técnico registra o resultado da verificação do equipamento após o reparo. O fluxo considera uma ordem atribuída ao profissional, com orçamento aprovado e ações de manutenção registradas. Um resultado satisfatório permite encaminhar o atendimento à entrega. Quando o teste é reprovado, o serviço retorna à manutenção. A reprovação é um resultado do teste e não cria uma nova situação geral para a ordem.

A Figura 1 apresenta o diagrama do grupo de casos de uso de manutenção. Nele, as atividades técnicas estão relacionadas ao Técnico, enquanto a comunicação da conclusão está associada ao Recepcionista. A generalização representada para o Administrador indica a possibilidade de assumir essas funções conforme a regra de substituição; ela não representa herança entre tipos da linguagem C.

![](C:/Users/math-/Documents/Codex/2026-10-12/vamnos/outputs/documentacao_pim_2_inicio_figuras/figura_1_casos_de_uso.png){width=15.00cm}

*Figura 1 — Casos de uso de manutenção -* Fonte: documentação do projeto, exportação dos diagramas de 30 de setembro de 2026.

## 3.5 Sequência de criação da ordem de serviço

O diagrama de sequência da Figura 2 foi elaborado a partir do UC24 para representar a troca de informações entre o atendente e o sistema. A representação corresponde ao fluxo principal previsto, considerando a autenticação do atendente e a existência dos cadastros necessários.

![](C:/Users/math-/Documents/Codex/2026-10-12/vamnos/outputs/documentacao_pim_2_inicio_figuras/figura_2_sequencia.png){width=12.28cm}

*Figura 2 — Sequência proposta para criação de uma ordem de serviço -* Fonte: elaboração própria, com base no UC24 e nas regras de negócio do projeto (2026).

A verificação dos cadastros antecede a gravação. O número da ordem é informado após o registro dos dados necessários. As situações que impedem a abertura permanecem descritas no caso de uso. A figura representa o comportamento esperado e deverá orientar a implementação e os testes dessa operação.

## 3.6 Arquitetura do sistema

O programa foi organizado como uma aplicação local em C, com execução em terminal e arquivos separados por área funcional. A navegação utiliza menus, e as operações são realizadas por chamadas de funções. Não há interface web nem operação simultânea de vários usuários em rede nesta versão.

Para descrever essa organização, as responsabilidades foram agrupadas em interação, operações e dados, conforme o Quadro 4. O agrupamento é conceitual: o código atual ainda reúne menus validações e gravação em alguns dos mesmos módulos.

| Grupo | Responsabilidade | Elementos do projeto |
|---|---|---|
| Interação | Receber opções e dados e apresentar respostas ao usuário. | Menus por perfil, leitura pelo terminal e mensagens das operações. |
| Operações | Executar cadastros, consultas e regras do atendimento. | Funções de autenticação, usuários, equipamentos, ordens, manutenção e orçamento. |
| Dados | Representar os registros e mantê-los disponíveis entre execuções. | Estruturas em memória, vetores e rotinas de leitura e gravação. |

Quadro 4 — Organização das responsabilidades do programa - Fonte: elaboração própria, com base na organização do código do projeto (2026).

As principais estruturas são Usuário, Equipamento, OrdemServico e TecnicoOS. A primeira reúne os quatro perfis; a segunda representa o item atendido; a terceira organiza os dados da ordem; e a quarta estabelece a associação entre técnicos e ordens. Como a implementação utiliza programação estruturada em C, os dados são representados por estruturas e manipulados por funções, sem classes implementadas.

A persistência presento no codigo foi estabelecida por meio da integração direta com o banco de dados SQL Server. A leitura e a gravação de registros, anteriormente realizadas em arquivos de textos locais, foram substituídas por operações estruturadas no banco, garantindo a centralização e a integridade das informações.

Foram identificadas rotinas de autenticação, de acesso ao banco e de gestão de usuários e equipamentos, as operações de abertura e acompanhamento de ordens, manutenção, orçamento e relatórios ainda possuem funções sem implementação. Portanto,a organização dos módulos demonstra a estrutura do desenvolvimento, mas não equivale á conclusão de todo o fluxo do atendimento. A avaliação do funcionamento será apresentada com as evidências de testes na etapa de programação.

## 3.7 Backlog do produto

O backlog reúne os itens de trabalho necessários para desenvolver a solução. Cada item recebeu um identificador PBI e foi relacionado aos requisitos e casos de uso correspondentes. O Quadro 5 apresenta os doze itens e suas prioridades na documentação do grupo.

Quadro 5 — Síntese do backlog do produto

| Item | Trabalho previsto | Referência principal | Prioridade |
|---|---|---|---|
| PBI01 | Estrutura inicial e persistência dos dados. | RNF01–RNF04; RNF06–RNF07 | Alta |
| PBI02 | Autenticação e acesso por perfil. | RF01 | Alta |
| PBI03 | Gestão de usuários. | RF02 | Alta |
| PBI04 | Gestão de equipamentos. | RF03 | Alta |
| PBI05 | Abertura e gestão inicial de ordens. | RF04 | Alta |
| PBI06 | Registro de diagnóstico técnico. | RF05 | Alta |
| PBI07 | Orçamento e decisão do cliente. | RF06 | Alta |
| PBI08 | Manutenção e atribuição de técnicos. | RF07 | Alta |
| PBI09 | Teste de funcionamento após o reparo. | RF08 | Média |
| PBI10 | Entrega, pagamento e avaliação do cliente. | RF09 | Média |
| PBI11 | Consultas e histórico de atendimentos. | RF10 | Média |
| PBI12 | Validações e integridade dos dados. | RNF08–RNF09 | Alta |

Fonte: elaboração própria, com base no backlog do projeto (2026).

Essa relação permite acompanhar a origem de uma tarefa. Por exemplo, o PBI05 atende ao RF04 e reúne os casos UC24 a UC27, relacionados à criação, consulta, edição e cancelamento de ordens. O PBI12 acompanha as validações das demais operações e deverá ser trabalhado junto delas, em vez de ficar restrito ao final do desenvolvimento.

Os estados de andamento do backlog precisam ser conciliados com o código e com as evidências de testes. A existência de uma função no programa, isoladamente, não é suficiente para considerar o item concluído.

# 4. Planejamento ágil

## 4.1 Organização do trabalho

O grupo adotou o Scrum como referência para organizar o planejamento. Schwaber e Sutherland (2020) descrevem um trabalho organizado em ciclos, com seleção de itens do backlog, avaliação do que foi produzido e adaptação dos próximos passos. No projeto, essa referência foi utilizada para definir responsabilidades, prioridades e períodos de trabalho.

A distribuição registrada atribui a Leonardo Lemgruber Portugal a função de Product Owner e a Elton Ruan da Silva Santos a de Scrum Master. Arthur dos Santos Moraes, Brunno Miranda da Silveira, Matheus Gonçalves da Silva e Natália Nunes Vieira compõem a equipe de desenvolvimento. Essa organização se refere ao grupo acadêmico e não aos perfis de acesso dos usuários da ByteTech.

## 4.2 Sprints e acompanhamento das atividades

O planejamento estabelece ciclos semanais, com revisão do trabalho, retrospectiva e planejamento do ciclo seguinte previstos para as sextas-feiras. Os registros das quatro primeiras sprints foram reconstruídos posteriormente, com base no histórico do projeto, conforme informado pelo grupo. Por isso, esses documentos devem ser lidos como uma organização das atividades registradas, sem comprovar a realização de todos os eventos do Scrum durante cada período.

Quadro 6 — Períodos e focos registrados nas sprints

| Sprint | Período de 2026 | Foco registrado |
|---|---|---|
| 01 | 27 de agosto a 4 de setembro | Estrutura da documentação, modelo de dados inicial e primeiros casos de uso. |
| 02 | 5 a 11 de setembro | Casos de uso por módulo e revisão do modelo de usuários. |
| 03 | 12 a 18 de setembro | Revisão dos requisitos, delimitação do escopo e modelo com quatro tabelas. |
| 04 | 19 a 25 de setembro | Fluxogramas, diagramas, descrições dos casos de uso e modelagem do banco. |
| 05 | 26 de setembro a 2 de outubro | Persistência, estrutura do código, autenticação e documentação. |

Fonte: elaboração própria, com base nos registros de planejamento do projeto (2026).

O primeiro período registrado inclui a organização inicial e ultrapassa a cadência semanal prevista para os seguintes. O documento da Sprint 05 ainda apresenta situação em andamento e não possui revisão e retrospectiva concluídas. Esses registros precisam ser atualizados a partir do que foi efetivamente realizado.

## 4.3 Critérios de conclusão e revisão das entregas

O grupo também estabeleceu critérios para considerar uma tarefa pronta. Os documentos e diagramas devem estar disponíveis no repositório com seus arquivos editáveis. Para o código, os critérios incluem compilação sem erros, verificação do fluxo principal e das exceções, respeito às permissões e manutenção dos dados após encerrar o programa. A conclusão ainda exige o vínculo com o requisito e a atualização do registro da sprint.

Esses critérios orientam a avaliação das entregas. Ao concluir cada funcionalidade, será necessário registrar as verificações realizadas e ajustar o planejamento com base nas pendências encontradas.

## 4.4 Prioridades para a continuidade

A continuidade deverá considerar as dependências entre as funcionalidades. O cadastro de clientes e equipamentos precisa fornecer registros consistentes para a abertura das ordens; os registros técnicos deverão sustentar a elaboração do orçamento; e a decisão do cliente deverá orientar a execução ou o encerramento do atendimento. Essa sequência permite selecionar entregas que possam ser verificadas como parte de um mesmo fluxo.

Para os próximos ciclos, será necessário atualizar o backlog a partir da implementação e registrar as pendências encontradas nos testes. As datas e as atividades de sprints futuras deverão ser definidas pelo grupo conforme sua disponibilidade. O planejamento apresentado não pressupõe a conclusão das tarefas que ainda constam como abertas.

# 5. Modelagem de banco de dados

## 5.1 Modelo conceitual e relacionamentos

A modelagem organiza as informações necessárias ao atendimento da ByteTech. Foram definidos quatro conjuntos de registros: usuários, equipamentos, ordens de serviço e atribuições de técnicos. O controle de estoque não integra essa estrutura. Os perfis Administrador, Recepcionista, Técnico e Cliente compartilham o cadastro de usuários, sendo diferenciados pelo campo `perfil`.

Um cliente pode possuir nenhum ou vários equipamentos, enquanto cada equipamento pertence a um único cliente. Um equipamento pode receber várias ordens ao longo do tempo, mas cada ordem corresponde a somente um equipamento. O atendente também é identificado na abertura: cada ordem possui um responsável por esse registro, e um atendente pode abrir diversas ordens.

A participação técnica estabelece uma relação de muitos para muitos. Uma ordem pode receber vários técnicos, e cada técnico pode participar de diferentes ordens. A entidade associativa `tecnico_os` representa essas atribuições. Uma ordem recém-aberta pode ainda não possuir técnico, portanto a participação mínima nesse relacionamento é zero.

![](C:/Users/math-/Documents/Codex/2026-10-12/vamnos/outputs/documentacao_pim_2_inicio_figuras/figura_3_modelo_dados.png){width=15cm}

*Figura 3 — Relacionamentos e principais atributos do modelo de dados* — Fonte: elaboração própria, com base no modelo e no script SQL Server do projeto (2026).

## 5.2 Modelo lógico

As entidades foram convertidas nas quatro tabelas apresentadas no Quadro 7. Cada tabela possui uma chave primária numérica. As chaves estrangeiras estabelecem os vínculos entre registros, evitando repetir os dados completos do cliente ou do equipamento em cada atendimento.

Quadro 7 — Tabelas e identificadores do modelo lógico

| Tabela | Finalidade | Chaves |
|---|---|---|
| `usuario` | Reunir identificação, contato, acesso e perfil. | Primária: `id_usuario`. |
| `equipamento` | Identificar o item vinculado ao cliente. | Primária: `id_equipamento`; estrangeira: `id_cliente`. |
| `os` | Registrar abertura, diagnóstico, orçamento, teste e entrega. | Primária: `id_os`; estrangeiras: `id_equipamento` e `id_atendente`. |
| `tecnico_os` | Relacionar o técnico à ordem e registrar suas observações. | Primária: `id_tecnico_os`; estrangeiras: `id_tecnico` e `id_os`. |

Fonte: elaboração própria, com base na modelagem do projeto (2026).

As cinco chaves estrangeiras são obrigatórias no script. Essa obrigatoriedade não exige que todo usuário tenha equipamentos ou atribuições; exige que cada registro dependente aponte para um cadastro existente. Diagnóstico, orçamento, resposta do cliente e entrega admitem valores nulos porque podem não existir na abertura da ordem. Valor nulo representa informação ausente, sendo diferente de um orçamento de valor zero.

## 5.3 Normalização

A normalização orienta a organização das tabelas para reduzir repetições e dependências inadequadas. A documentação da [Microsoft (2025)](https://learn.microsoft.com/en-us/previous-versions/troubleshoot/microsoft-365/microsoft-365-apps/access/database-normalization-description) apresenta esse processo por meio das formas normais, com exemplos de separação de informações e de relacionamentos entre registros.

Na primeira forma normal, verifica-se a ausência de grupos repetidos. O projeto utiliza uma linha em `tecnico_os` para cada atribuição, sem criar colunas como “técnico 1” e “técnico 2”. A segunda forma normal trata da dependência dos atributos em relação à totalidade de uma chave candidata. No modelo apresentado, as chaves primárias são simples, e os atributos descrevem o registro identificado por elas. A terceira forma normal também exige avaliar dependências entre atributos não pertencentes às chaves. Por exemplo, o nome do cliente permanece em `usuario`, sendo recuperado por relacionamento, sem repetição em `os`.

Essas decisões contribuem para a organização dos dados, mas não substituem uma verificação completa das dependências funcionais. O campo `modelo_numero_serie`, por exemplo, reúne conceitos que poderão ser separados caso precisem de consulta e validação próprias. Assim, a análise não declara que a terceira forma normal foi integralmente demonstrada.

## 5.4 Modelo físico e integridade

O projeto contém scripts para SQLite, MySQL e SQL Server. O SQL Server é o banco indicado para a futura integração com o programa em C. Seu script utiliza `INT IDENTITY` nos identificadores, `NVARCHAR` nos textos, `DATE` nas datas de atendimento, `DATETIME2` nos registros de criação e alteração e `DECIMAL(10,2)` nos valores monetários.

Quadro 8 — Restrições presentes no modelo físico

| Elemento | Restrição definida |
|---|---|
| Identificação | Chaves primárias e estrangeiras nas quatro tabelas. |
| Usuário | E-mail único; CPF único quando preenchido; quatro perfis permitidos. |
| Usuário e equipamento | Situação limitada a `ativo` ou `inativo`. |
| Ordem de serviço | Situação limitada a `aberta`, `em_andamento`, `finalizada` ou `cancelada`. |
| Respostas | Orçamento: `aprovado` ou `recusado`; teste: `aprovado` ou `reprovado`; satisfação: `sim` ou `nao`, admitindo ausência de resposta. |

Fonte: elaboração própria, com base no script SQL Server do projeto (2026).

O script inclui sete gatilhos, ou *triggers*, para verificar perfis nos vínculos de cliente e técnico, impedir novas atribuições a registros inativos e restringir inativações enquanto houver ordens abertas ou em andamento. Também verifica alterações de perfil incompatíveis com vínculos existentes. Essas regras complementam as restrições das tabelas, mas não cobrem todas as permissões e transições do atendimento.

Há pontos que precisam ser conciliados com os requisitos: `problema_relatado` admite nulo no banco, embora seja obrigatório no UC24; o par `id_tecnico` e `id_os` não possui restrição de unicidade. Além disso, o valor padrão de `atualizado_em` atua na inclusão, exigindo atualização explícita nas alterações.

## 5.5 Exemplos de comandos SQL

Os trechos a seguir são ilustrativos e não representam operações executadas durante a elaboração deste texto. O índice existente no projeto considera somente os CPFs preenchidos. Índices filtrados permitem selecionar as linhas participantes por uma condição, conforme a documentação da [Microsoft (2026)](https://learn.microsoft.com/en-us/sql/relational-databases/indexes/create-filtered-indexes?view=sql-server-ver17).

```sql
CREATE UNIQUE INDEX uq_usuario_cpf
ON usuario (cpf)
WHERE cpf IS NOT NULL;
```

A consulta abaixo relaciona ordens e equipamentos para listar os atendimentos ainda abertos ou em andamento:

```sql
SELECT o.id_os, e.tipo, o.data_abertura, o.status
FROM os AS o
JOIN equipamento AS e
  ON e.id_equipamento = o.id_equipamento
WHERE o.status IN ('aberta', 'em_andamento')
ORDER BY o.data_abertura, o.id_os;
```

## 5.6 Possibilidades de NoSQL e integração relacional

Uma alternativa futura seria armazenar registros de diagnóstico em documentos com campos específicos para cada tipo de atendimento. A documentação do [MongoDB (s.d.)](https://www.mongodb.com/docs/manual/data-modeling/embedding/) descreve documentos que agrupam dados relacionados por meio de estruturas e listas internas. Essa organização poderia atender informações variáveis, mas exigiria definir consultas, validações e a consistência dos vínculos.

Para o escopo atual, o modelo relacional permite representar diretamente as relações entre usuários, equipamentos e ordens. Não há implementação de banco NoSQL. Sua utilização dependeria de uma necessidade que justificasse ampliar a solução.

//PENDENTE DE FINALIZAR A CONEXÃO COM O BANCO PARA TERMINAR ESSE PARAGRAFO

# 6. Programação estruturada em C

A implementação traduz parte dos requisitos da ByteTech em uma aplicação de terminal. A análise considera os arquivos da pasta `projeto`, distinguindo rotinas desenvolvidas de funcionalidades previstas. A leitura do código permite examinar os algoritmos, mas não substitui a compilação e os testes.

## 6.1 Organização do programa e representação dos dados

O arquivo `main.c` reúne estruturas, vetores globais, funções auxiliares e menus. Inclui os demais arquivos C, entre eles `geral.c`, com operações de usuários e autenticação, e `equipamento.c`, voltado aos equipamentos. Menus, regras e persistência ainda compartilham funções e dados globais.

Cada registro utiliza uma `struct`, que agrupa campos de tipos diferentes. Identificadores inteiros relacionam os registros, enquanto vetores de caracteres armazenam textos e datas. O Quadro 9 apresenta as estruturas principais.

Quadro 9 — Estruturas e coleções presentes no programa

| Estrutura | Dados representados | Vetor e capacidade definida |
|---|---|---|
| `Usuario` | Identificação, contato, perfil, credenciais e situação do usuário. | `usuarios`: 200 registros. |
| `Equipamento` | Cliente proprietário, categoria, tipo, marca e identificação do item. | `equipamentos`: 300 registros. |
| `OrdemServico` | Equipamento, atendente, problema, diagnóstico e campos do acompanhamento. | `ordens`: 500 registros. |
| `TecnicoOS` | Associação entre técnico e ordem, com data e observações. | `tecnicosOS`: 1.000 registros. |

Fonte: elaboração própria, com base em `main.c` (2026).

Os contadores indicam as posições ocupadas. As capacidades são limites do código, sem demonstração de desempenho nesses volumes. Nem todas as operações sobre essas estruturas estão implementadas.

## 6.2 Funções, decisões e repetições

As funções dividem o processamento em tarefas menores: `selecionarUsuario` localiza um cadastro, `campoPreenchido` verifica uma entrada obrigatória e `salvarEquipamentos` concentra a gravação. O cadastro de equipamento combina essas operações.

As condicionais determinam a continuidade das operações. No login, por exemplo, a comparação das credenciais antecede a verificação de usuário ativo. As repetições percorrem coleções e mantêm a navegação, conforme o Quadro 10.

Quadro 10 — Aplicações das estruturas de controle

| Recurso | Exemplo identificado | Finalidade |
|---|---|---|
| `if` e `else` | Seleção entre hardware e software. | Solicitar a identificação adequada à categoria. |
| `switch` | Menu de equipamentos. | Encaminhar a opção para cadastro, consulta, edição ou inativação. |
| `for` | Busca de equipamento por identificador. | Percorrer os registros até localizar a correspondência. |
| `while` | Leitura do arquivo de equipamentos. | Carregar linhas enquanto houver dados e capacidade disponível. |
| `do...while` | Menu de equipamentos. | Reapresentar as opções até o usuário escolher voltar. |

Fonte: elaboração própria, com base em `main.c`, `geral.c` e `equipamento.c` (2026).

## 6.3 Algoritmo de cadastro de equipamento

O cadastro de equipamento, associado ao UC20, possui corpo implementado. A operação verifica a capacidade do vetor, solicita um cliente ativo e recebe a categoria e os dados do item. Após validar os campos, atribui o maior identificador existente acrescido de uma unidade.

O pseudocódigo resume essa sequência. O retorno do salvamento não comprova a detecção de todas as falhas possíveis de escrita.

```text
INÍCIO
  SE o vetor estiver cheio, informar e retornar
  Selecionar cliente ativo; SE a seleção for inválida, retornar
  Ler categoria; SE for inválida, informar e retornar
  Ler dados; SE faltar campo obrigatório, informar e retornar
  Obter o maior ID e preparar registro com ID = maior ID + 1
  Vincular ao cliente, definir situação ativa e datas
  Incrementar o total e solicitar salvamento
  SE o salvamento indicar falha
    Decrementar o total e retornar
  SENÃO
    Informar o cadastro e seu identificador
  FIM SE
FIM
```

O fluxograma da Figura 4 representa a lógica examinada, sem constituir evidência de execução.

![](C:/Users/math-/Documents/Codex/2026-10-12/vamnos/outputs/documentacao_pim_2_inicio_figuras/figura_4_fluxograma_cadastro.png){width=15cm}

*Figura 4 — Fluxograma do cadastro de equipamento* — Fonte: elaboração própria, com base em `cadastrarEquipamento`, no arquivo `equipamento.c` (2026).

## 6.4 Manipulação de arquivos e integração prevista

//PENDENTE TERMINAR O BANCO PARA FAZER A CONSTRUÇÃO DO PARÁGRAFO


## 6.5 Proposta de utilização de matriz

Não foi identificada uma matriz bidimensional nos arquivos C. Propõe-se uma matriz de contagem com quatro linhas para os perfis literais `admin`, `recepcionista`, `tecnico` e `cliente`, e duas colunas para `ativo` e `inativo`.

A matriz seria inicializada com zeros. Um laço percorreria `usuarios` e incrementaria a célula correspondente ao perfil e à situação. Dois laços aninhados apresentariam as contagens ao administrador, sem dados pessoais. A proposta depende de implementação e avaliação; não integra os relatórios existentes.

## 6.6 Planejamento dos testes

A verificação deverá incluir entradas válidas, impedimentos e persistência. Os cenários do Quadro 11 estão a executar; não são atribuídos resultados de compilação ou execução. A matriz proposta também deverá ser testada após sua implementação.

Quadro 11 — Plano inicial de testes do programa

| Cenário | Resultado esperado | Situação |
|---|---|---|
| Compilar a versão de terminal. | Gerar executável sem erros; examinar avisos. | A executar. |
| Informar credenciais válidas e usuário ativo. | Apresentar o menu correspondente ao perfil. | A executar. |
| Informar senha incorreta ou usuário inativo. | Impedir a autenticação. | A executar. |
| Cadastrar equipamento com dados válidos. | Vincular ao cliente, atribuir ID e gravar. | A executar. |
| Informar categoria inválida ou campo obrigatório vazio. | Interromper o cadastro sem incluir registro. | A executar. |
| Reiniciar após cadastrar equipamento. | Recuperar o registro e preservar seus campos. | A executar. |

Fonte: elaboração própria, com base nos requisitos e nas rotinas analisadas (2026).

## 6.7 Estado da implementação e limitações

Há código para autenticação, operações de usuários e equipamentos e persistência textual. As funções de criação e acompanhamento de ordens, manutenção, orçamento, histórico e relatórios ainda apresentam corpos vazios. As rotinas de arquivos de ordens e atribuições não completam essas funcionalidades.

Permanecem necessárias a implementação dessas operações, a avaliação da proteção de senhas e da gravação, além dos testes de integração. A conclusão deverá ser sustentada pelo registro das entradas, dos resultados observados e das correções realizadas.

# 7. Redes de computadores e sistemas distribuídos

## 7.1 Situação atual e infraestrutura proposta

A aplicação desenvolvida em C funciona em terminal e possui integração direta com o banco de dados relacional SQL Server. O programa não possui no entanto, comunicação distribuida nativa entre multiplos computadores operando simultaneamente de forma descentralizada. A infraestrutura apresentada neste capítulo constitui uma proposta para uma implatanção na ByteTech, após a conclusão de todas as funcionalidades e atendimento e a adaptação estrutural do sistema ao uso de multiusuário em rede.

Para dimensionar o cenário, consideram-se cinco estações de trabalho: duas na recepção, uma na administração e duas para os técnicos. Essas quantidades são hipóteses de planejamento, sem correspondência com um levantamento de instalações ou funcionários. Propõem-se um switch gerenciável de 16 portas, um roteador com firewall, um ponto de acesso sem fio e um servidor para os serviços internos. Também se prevê um nobreak para o servidor e os equipamentos centrais da rede. A escolha dos modelos dependeria da avaliação de capacidade, suporte e orçamento.

A topologia física seria em estrela, com as conexões concentradas no switch. O cabeamento proposto é de categoria 6, com pontos identificados e organizados em um painel de conexões. As estações utilizariam conexões cabeadas de 1 Gb/s como especificação inicial. A quantidade e a posição dos pontos precisariam ser revistas conforme a planta do imóvel. A Figura 5 apresenta a organização conceitual.

![](C:/Users/math-/Documents/Codex/2026-10-12/vamnos/outputs/documentacao_pim_2_inicio_figuras/figura_5_rede.png){width=15cm}

*Figura 5 — Organização proposta da rede da ByteTech* — Fonte: elaboração própria (2026).

## 7.2 Segmentação e endereçamento

A rede seria dividida em VLANs, que permitem separar logicamente o tráfego em uma infraestrutura física compartilhada ([CISCO, s.d.](https://ci.manage.security.cisco.com/content/docs/c-about-vlan-subinterfaces.html)). Essa divisão permitiria aplicar regras diferentes aos computadores da empresa, aos equipamentos recebidos para manutenção e aos visitantes. O isolamento efetivo dependeria das configurações do switch e do firewall.

O Quadro 12 utiliza endereços privados do bloco 192.168.0.0/16, previsto na RFC 1918 ([REKHTER et al., 1996](https://www.rfc-editor.org/rfc/rfc1918.html)). Cada segmento teria máscara 255.255.255.0, indicada pelo prefixo /24, e o endereço terminado em .1 como gateway.

Quadro 12 — Plano inicial de VLANs e endereçamento

| VLAN | Finalidade | Rede | Atribuição prevista |
|---|---|---|---|
| 10 | Administração e recepção | 192.168.10.0/24 | DHCP: .100 a .199 |
| 20 | Estações dos técnicos | 192.168.20.0/24 | DHCP: .100 a .199 |
| 30 | Serviços internos | 192.168.30.0/24 | Aplicação: .10; banco: .20 |
| 40 | Equipamentos de clientes em manutenção | 192.168.40.0/24 | DHCP: .100 a .199 |
| 50 | Acesso sem fio para visitantes | 192.168.50.0/24 | DHCP: .100 a .199 |

Fonte: elaboração própria (2026), com endereços privados conforme Rekhter et al. (1996).

O roteador utilizaria uma interface lógica por VLAN, com os endereços de gateway indicados, e concentraria as regras de comunicação entre os segmentos. Seu enlace com o switch transportaria as VLANs. O ponto de acesso associaria a rede de visitantes à VLAN 50.

O DHCP distribuiria endereços e parâmetros de rede às estações, conforme a finalidade desse protocolo ([DROMS, 1997](https://www.rfc-editor.org/rfc/rfc2131.html)). Os serviços internos teriam endereços fixos. O DNS permitiria localizar esses serviços por nomes, sem exigir que os usuários memorizassem seus endereços IP ([MOCKAPETRIS, 1987](https://www.rfc-editor.org/rfc/rfc1034.html)). Ambos poderiam ser fornecidos pelo roteador, desde que o equipamento suportasse as configurações propostas.

Visitantes e equipamentos em manutenção teriam acesso à internet, sem permissão para alcançar os registros internos. Também seria previsto isolamento entre os dispositivos desses segmentos. Nas VLANs de trabalho, somente as comunicações necessárias seriam liberadas. A administração dos equipamentos de rede ficaria restrita a uma estação autorizada.

## 7.3 Comunicação entre serviços e possibilidades de expansão

Uma evolução possível seria adotar o modelo cliente-servidor. Os terminais das estações enviariam solicitações a um serviço de aplicação, responsável pelas regras de negócio e pelo acesso ao banco de dados. Aplicação e banco poderiam começar em ambientes virtuais separados no mesmo servidor. Os clientes e o serviço central constituiriam a organização distribuída proposta; essa comunicação ainda não está implementada.

O acesso à aplicação poderia utilizar HTTPS, mantendo uma interface de terminal adaptada à comunicação. O banco aceitaria conexões apenas do serviço autorizado, com restrição no firewall do próprio servidor, pois os dois serviços compartilhariam a VLAN 30. Não seria previsto acesso direto ao banco pela internet. Essa mudança exigiria desenvolver a comunicação, controlar operações simultâneas e tratar falhas de conexão. Compartilhar os arquivos TXT entre várias cópias do programa não resolveria essas necessidades e poderia causar perda de alterações.

A nuvem oferece recursos computacionais acessíveis pela rede e provisionados conforme a demanda ([MELL; GRANCE, 2011](https://csrc.nist.gov/pubs/sp/800/145/final)). Para a ByteTech, propõe-se inicialmente seu uso para uma cópia externa criptografada dos registros. A contratação dependeria da análise de acesso, recuperação, localização dos dados e responsabilidades do fornecedor. Uma migração dos serviços para a nuvem seria avaliada em etapa posterior.

O crescimento poderia começar pela inclusão de estações e pelo aumento dos recursos do servidor. A necessidade de outras instâncias da aplicação dependeria de medições de carga e testes de concorrência. Não foram estabelecidas capacidades de atendimento nem garantias de disponibilidade, pois a infraestrutura não foi implantada.

## 7.4 Segurança e continuidade

Propõem-se atualizações periódicas, contas individuais e revisão de permissões, medidas discutidas no guia da ANPD ([ANPD, 2021](https://www.gov.br/anpd/pt-br/centrais-de-conteudo/materiais-educativos-e-publicacoes/guia-vf.pdf/@@display-file/file)). Para a versão local, a cópia diária dos arquivos seria realizada após o encerramento do programa. Na futura versão com banco, seriam utilizados mecanismos próprios de backup do sistema gerenciador.

O planejamento inclui uma cópia local protegida e outra externa, com credenciais separadas, além de testes mensais de restauração. Essas frequências são propostas iniciais, sujeitas à revisão conforme a quantidade de registros e a perda de dados aceitável. A implantação precisaria verificar as regras entre VLANs, a comunicação dos serviços e a recuperação dos registros antes de liberar o uso operacional.

# 8. Direitos humanos, acessibilidade e segurança

## 8.1 Acessibilidade e inclusão digital

O sistema da ByteTech deve ser analisado considerando as condições de uso dos funcionários e dos clientes. A Lei Brasileira de Inclusão relaciona a acessibilidade à autonomia e à participação social e estabelece o desenho universal como referência para projetos de informação e comunicação ([BRASIL, 2015](https://www.planalto.gov.br/ccivil_03/_ato2015-2018/2015/lei/l13146.htm)). No projeto, isso orienta a atenção às diferentes necessidades de acesso e experiências com computadores.

A interface em terminal permite o uso pelo teclado, mas essa característica não comprova sua acessibilidade. A compreensão das opções, a apresentação das mensagens e o comportamento do terminal também precisam ser avaliados. A orientação WCAG2ICT aborda a aplicação de princípios de acessibilidade a programas fora da web, incluindo interfaces textuais ([W3C, 2025](https://www.w3.org/TR/2025/NOTE-wcag2ict-22-20251211/)).

Propõem-se menus com organização consistente, instruções curtas e mensagens que indiquem o campo incorreto e a correção esperada. Informações importantes não deveriam depender apenas de cores. Também seria necessário verificar a leitura das mensagens por tecnologias assistivas e a possibilidade de ajustar a apresentação do texto. Não foram realizados testes com leitores de tela nem avaliações com pessoas com deficiência; portanto, a acessibilidade permanece como aspecto a desenvolver e verificar.

Para favorecer a inclusão digital, a implantação deveria prever orientação aos funcionários e atendimento assistido aos clientes. Uma pessoa com pouca familiaridade com computadores precisaria conseguir solicitar informações e decidir sobre o orçamento com apoio da recepção, sem compartilhar sua senha. O uso direto do sistema não deveria se tornar uma condição para receber atendimento.

## 8.2 Privacidade e proteção dos dados

O recorte do sistema contempla clientes pessoas físicas. Seus cadastros e históricos podem relacionar nome, CPF, contato, equipamento e serviço solicitado. A LGPD estabelece princípios como finalidade, necessidade e transparência, além de direitos de acesso e correção dos dados ([BRASIL, 2018](https://www.planalto.gov.br/ccivil_03/_ato2015-2018/2018/lei/l13709compilado.htm)). Uma implantação exigiria definir a finalidade e a base legal de cada tratamento, explicar essas condições ao cliente e estabelecer um meio para atender suas solicitações.

A inativação de um cadastro, prevista nas regras do projeto, não equivale à eliminação dos dados. Preservar o histórico das ordens de serviço exige estabelecer critérios de retenção, considerando a finalidade e as hipóteses legais de conservação, sem adotar guarda indefinida por padrão (BRASIL, 2018).

Para este cenário, recomenda-se evitar informações pessoais sem relação com o atendimento nos campos de observação. Arquivos particulares encontrados nos equipamentos também não deveriam ser copiados para o sistema de ordens de serviço. Demonstrações e testes devem utilizar dados fictícios. Estas medidas compõem o planejamento acadêmico e não representam uma declaração de conformidade com a legislação.

## 8.3 Permissões, responsabilidade e impactos sociais

A ANPD recomenda limitar o acesso às necessidades de cada atividade e orientar os funcionários sobre suas responsabilidades ([ANPD, 2021](https://www.gov.br/anpd/pt-br/centrais-de-conteudo/materiais-educativos-e-publicacoes/guia-vf.pdf/@@display-file/file)). Na solução proposta, isso se relaciona aos perfis Administrador, Recepcionista, Técnico e Cliente. O cliente deve consultar apenas seus próprios registros e realizar as ações previstas, como decidir sobre o orçamento e registrar sua avaliação. As permissões precisam ser verificadas em cada operação.

Na versão local, proteger o acesso ao menu deve ser acompanhado pela segurança do próprio servidor. Diferente do uso de arquivos de texto comuns, a proteção dos dados agora depende de uma gestão rigorosa das credenciais de conexão ao SQL Server. Uma futura implantação precisará manter as permissões da aplicação combinadas a rotinas robustas no sistema gerenciador de banco de dados, protegendo o acesso ao servidor físico ou virtual e assegurando a política de cópias de segurança. O requisito de proteção das senhas no banco também deverá ser verificado na implementação antes do uso com dados reais.

Espera-se que registros organizados tornem mais claras as informações sobre o serviço e as responsabilidades pelo atendimento. Entretanto, erros de cadastro ou interpretações inadequadas podem prejudicar clientes e funcionários. A proposta deve permitir a correção dos registros e preservar a avaliação humana em situações de dúvida. O respeito à dignidade, à privacidade e à igualdade de atendimento orienta essa análise, sem atribuir à tecnologia a capacidade de resolver, sozinha, problemas de relacionamento ou discriminação.

# 9. Integração da solução e planejamento da implantação

## 9.1 Relação entre os componentes do projeto

A integração da proposta parte do fluxo de atendimento da ByteTech. O cadastro identifica o cliente e seus equipamentos; a ordem de serviço reúne as informações do atendimento; e a associação entre técnicos e ordens permite indicar os profissionais responsáveis. Os requisitos definem quais operações cada perfil poderá realizar, enquanto o modelo de dados estabelece como os registros se relacionam.

Na aplicação em C, essas informações são representadas por estruturas e manipuladas durante a execução. As rotinas de persistência comunicam-se diretamente com o SQL Server, permitindo a leitura e a gravação nas tabelas. A modelagem relacional oferece o ambiente efetivo de armazenamento, enquanto as funções de acesso e conexão do programa garantem o respeito às restrições do banco nas operações submetidas por cada usuário.

A proposta de redes complementa o planejamento ao descrever como os equipamentos e serviços poderiam ser organizados em uma futura implantação. Sua adoção exigiria adequar a aplicação ao ambiente previsto. A separação dos acessos, a proteção dos registros e as orientações de uso deverão acompanhar essa mudança, pois a instalação da infraestrutura, por si só, não resolve as permissões e as regras do atendimento.

## 9.2 Preparação para uma futura implantação

Antes de utilizar o sistema no cenário de atendimento, será necessário concluir o fluxo de ordens de serviço e verificar os cadastros, as permissões, as alterações de estado e a recuperação dos dados. O grupo deverá consolidar uma versão de referência do programa e registrar quais requisitos foram atendidos, quais testes foram realizados e quais limitações permanecem.

A implantação poderá começar por uma simulação com dados fictícios. Nessa atividade, os participantes assumirão os quatro perfis e realizarão um atendimento completo, desde o cadastro até a entrega, incluindo situações de recusa do orçamento e cancelamento. O objetivo será identificar dificuldades de navegação e falhas no encadeamento das operações antes de qualquer uso com dados reais.

Também deverão ser definidos os responsáveis pela instalação, pelo cadastro inicial, pelo suporte e pelas cópias de segurança. Os procedimentos de orientação precisam explicar as tarefas de cada perfil, a correção de dados e a comunicação de falhas. A liberação de uma versão dependerá da conclusão das verificações, e não apenas de uma data prevista no planejamento.

Como o SQL Server já está integrado à solução, a preparação para a implantação deverá incluir a configuração do ambiente definitivo do servidor, a criação das tabelas no ambiente de produção e a realização de testes de recuperação de backup. Qualquer migração de registros prévios ou legados para o novo banco de dados exigirá conferir identificadores e relacionamentos, comparando origens e destinos e mantendo uma cópia de segurança anterior à mudança. Essas atividades fazem parte do planejamento metodológico da implantação.

## 9.3 Benefícios esperados e critérios de avaliação

Com o fluxo completo, espera-se que a recepção consiga localizar os atendimentos e consultar as informações registradas pela equipe técnica. O cliente poderá acompanhar seus próprios serviços e registrar sua decisão sobre o orçamento. Para os técnicos, a relação entre equipamentos, diagnósticos e manutenções poderá facilitar a consulta ao histórico.

Essas contribuições poderão ser avaliadas por tarefas como localizar uma ordem, identificar seus responsáveis e recuperar um registro após encerrar o programa. Também será necessário verificar se cada perfil consegue executar suas atividades sem acessar informações indevidas. A avaliação deverá registrar o resultado observado e as dificuldades encontradas; não foram estabelecidas reduções percentuais de tempo ou custos, pois não há uma medição anterior que sustente essa comparação.

## 9.4 Limitações e possibilidades de evolução

A principal limitação atual é a diferença entre o conjunto de funcionalidades especificadas e a implementação disponível. O fluxo de manutenção ainda depende da conclusão de operações lógicas por parte da aplicação. Também faltam evidências de testes do atendimento completo (da recepção à entrega) e de avaliações de acessibilidade com usuários, requisitos fundamentais antes de homologar a solução na empresa.

A evolução deve priorizar a conclusão e a verificação desse escopo. Em seguida, poderão ser estudados o aperfeiçoamento da proteção das senhas, o registro de ações para auditoria e a operação com um serviço centralizado. Uma interface gráfica poderá ser considerada em outra versão, após a análise das necessidades de uso. O controle de estoque e o atendimento a pessoas jurídicas permanecem fora do recorte definido para este trabalho.

# Conclusão

O desenvolvimento do PIM permitiu organizar uma proposta de sistema para o acompanhamento dos serviços de manutenção da ByteTech. A caracterização da empresa indicou dificuldades relacionadas à dispersão dos registros, à identificação dos responsáveis e à consulta ao histórico. A partir desse cenário, foram definidos os perfis de usuário, os requisitos, as regras de negócio e os relacionamentos entre os dados.

O trabalho relacionou a pesquisa bibliográfica ao planejamento do software e à modelagem do banco. A programação em C permitiu representar os cadastros por estruturas e organizar operações em funções, com persistência em arquivos na versão analisada. O planejamento de redes ampliou a discussão sobre uma futura implantação, enquanto a análise de direitos humanos destacou cuidados com acesso, privacidade e inclusão digital.

O objetivo de desenvolver o sistema foi atendido parcialmente. A implementação disponível oferece uma base estruturada e segura para o projeto, com destaque para a conectividade estabelecida com o banco de dados SQL Server, mas ainda não demonstra o funcionamento lógico completo do fluxo de atendimento, da abertura da ordem à entrega final do equipamento. A implementação das operações sistêmicas pendentes no código e a execução dos testes práticos precisam ser concluídas para avaliar a solução como um conjunto operante.

A contribuição desta etapa está na organização das necessidades da ByteTech, na consolidação do armazenamento de dados relacional e na definição de uma solução coerente com as falhas de processos apontadas. Os benefícios previstos dependem da continuidade do desenvolvimento e de sua avaliação prática. Como o projeto utiliza uma empresa fictícia, as conclusões se referem ao cenário acadêmico estudado, sem resultados medidos de implantação em uma organização real.

# Referências

AUTORIDADE NACIONAL DE PROTEÇÃO DE DADOS (ANPD). *Guia orientativo sobre segurança da informação para agentes de tratamento de pequeno porte*. Versão 1.0. Brasília, DF: ANPD, out. 2021. Disponível em: <https://www.gov.br/anpd/pt-br/centrais-de-conteudo/materiais-educativos-e-publicacoes/guia-vf.pdf/@@display-file/file>. Acesso em: 18 out. 2026.

BANNITZ, Francisco Saldanha; ZAFALON, Zaira Regina. A interlocução entre o fluxo informacional e a gestão de processos de negócio: evidências de um mapeamento sistemático de literatura. *InCID: Revista de Ciência da Informação e Documentação*, v. 17, e-231446, 2026. DOI: 10.11606/issn.2178-2075.incid.2026.231446. Disponível em: <https://revistas.usp.br/incid/article/view/231446>. Acesso em: 12 out. 2026.

BERALDI, Lairce Castanhera; ESCRIVÃO FILHO, Edmundo. Impacto da tecnologia de informação na gestão de pequenas empresas. *Ciência da Informação*, v. 29, n. 1, p. 46–50, 2000. DOI: 10.1590/S0100-19652000000100005. Disponível em: <https://www.scielo.br/j/ci/a/bgDCMyb7DLNqFBdyvBCmcLm/>. Acesso em: 12 out. 2026.

BRASIL. *Lei nº 13.146, de 6 de julho de 2015*. Institui a Lei Brasileira de Inclusão da Pessoa com Deficiência (Estatuto da Pessoa com Deficiência). Brasília, DF: Presidência da República, 2015. Disponível em: <https://www.planalto.gov.br/ccivil_03/_ato2015-2018/2015/lei/l13146.htm>. Acesso em: 18 out. 2026.

BRASIL. *Lei nº 13.709, de 14 de agosto de 2018*. Lei Geral de Proteção de Dados Pessoais (LGPD). Brasília, DF: Presidência da República, 2018. Texto compilado. Disponível em: <https://www.planalto.gov.br/ccivil_03/_ato2015-2018/2018/lei/l13709compilado.htm>. Acesso em: 18 out. 2026.

CISCO. *Add an ASA VLAN subinterface*. [S. l.]: Cisco, [s. d.]. Disponível em: <https://ci.manage.security.cisco.com/content/docs/c-about-vlan-subinterfaces.html>. Acesso em: 18 out. 2026.

DROMS, Ralph. *Dynamic Host Configuration Protocol*. RFC 2131. [S. l.]: RFC Editor, mar. 1997. DOI: 10.17487/RFC2131. Disponível em: <https://www.rfc-editor.org/rfc/rfc2131.html>. Acesso em: 18 out. 2026.

MELL, Peter; GRANCE, Timothy. *The NIST definition of cloud computing*. Gaithersburg: National Institute of Standards and Technology, set. 2011. NIST Special Publication 800-145. DOI: 10.6028/NIST.SP.800-145. Disponível em: <https://csrc.nist.gov/pubs/sp/800/145/final>. Acesso em: 18 out. 2026.

MENDES, Juliana Veiga; ESCRIVÃO FILHO, Edmundo. Sistemas integrados de gestão ERP em pequenas empresas: um confronto entre o referencial teórico e a prática empresarial. *Gestão & Produção*, v. 9, n. 3, p. 277–296, 2002. DOI: 10.1590/S0104-530X2002000300006. Disponível em: <https://www.scielo.br/j/gp/a/GmLSKVc7dpRYdBhtbChHDcv/>. Acesso em: 12 out. 2026.

MICROSOFT. *Create filtered indexes*. Microsoft Learn, 20 jul. 2026. Disponível em: <https://learn.microsoft.com/en-us/sql/relational-databases/indexes/create-filtered-indexes?view=sql-server-ver17>. Acesso em: 18 out. 2026.

MICROSOFT. *Description of the database normalization basics*. Microsoft Learn, 26 maio 2025. Disponível em: <https://learn.microsoft.com/en-us/previous-versions/troubleshoot/microsoft-365/microsoft-365-apps/access/database-normalization-description>. Acesso em: 18 out. 2026.

MOCKAPETRIS, Paul. *Domain names: concepts and facilities*. RFC 1034. [S. l.]: RFC Editor, nov. 1987. DOI: 10.17487/RFC1034. Disponível em: <https://www.rfc-editor.org/rfc/rfc1034.html>. Acesso em: 18 out. 2026.

MONGODB. *Embedded data in your MongoDB schema*. [S. l.], [s. d.]. Disponível em: <https://www.mongodb.com/docs/manual/data-modeling/embedding/>. Acesso em: 18 out. 2026.

MORAES, Giseli Diniz de Almeida; ESCRIVÃO FILHO, Edmundo. A gestão da informação diante das especificidades das pequenas empresas. *Ciência da Informação*, v. 35, n. 3, p. 124–132, 2006. DOI: 10.18225/ci.inf.v35i3.1119. Disponível em: <https://revista.ibict.br/ciinf/article/view/1119>. Acesso em: 12 out. 2026.

REKHTER, Yakov et al. *Address allocation for private internets*. RFC 1918. [S. l.]: RFC Editor, fev. 1996. DOI: 10.17487/RFC1918. Disponível em: <https://www.rfc-editor.org/rfc/rfc1918.html>. Acesso em: 18 out. 2026.

SCHEFFER, João Gabriel; TIBOLA, Leandro Rosniak. *GestaoOS: Sistema de Gerenciamento de Serviços de TI*. 2025. Artigo científico (Graduação em Ciência da Computação) — Instituto Federal de Educação, Ciência e Tecnologia Farroupilha, Campus Frederico Westphalen, Frederico Westphalen, 2025. Disponível em: <https://arandu.iffarroupilha.edu.br/handle/itemid/849>. Acesso em: 12 out. 2026.

SCHWABER, Ken; SUTHERLAND, Jeff. *O Guia do Scrum: o guia definitivo para o Scrum: as regras do jogo*. nov. 2020. Disponível em: <https://scrumguides.org/docs/scrumguide/v2020/2020-Scrum-Guide-PortugueseBR-3.0.pdf>. Acesso em: 12 out. 2026.

TRIGUEIRO, Francisco Mirialdo Chaves; ELIZEU, Rodrigo Carvalho. Informatização no gerenciamento: análise dos resultados em microempresas de Cuiabá – MT. *Revista Estudos e Pesquisas em Administração*, v. 7, n. 3, 2024. DOI: 10.30781/repad.v7i3.15987. Disponível em: <https://periodicoscientificos.ufmt.br/ojs/index.php/repad/article/view/15987>. Acesso em: 12 out. 2026.

WORLD WIDE WEB CONSORTIUM (W3C). *Guidance on applying WCAG 2 to non-web information and communications technologies (WCAG2ICT)*. W3C Group Note, 11 dez. 2025. [S. l.]: W3C, 2025. Disponível em: <https://www.w3.org/TR/2025/NOTE-wcag2ict-22-20251211/>. Acesso em: 18 out. 2026.

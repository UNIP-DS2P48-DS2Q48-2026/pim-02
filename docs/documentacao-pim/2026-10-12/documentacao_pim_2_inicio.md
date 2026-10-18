# Resumo

Este trabalho apresenta o planejamento e o desenvolvimento de um sistema de gestão de manutenção de equipamentos para a ByteTech, empresa fictícia de assistência técnica em informática. No cenário estudado, o uso de planilhas e anotações separadas dificulta a localização das ordens de serviço, a identificação dos responsáveis e a consulta ao histórico dos atendimentos. O projeto tem como objetivo organizar essas informações em uma aplicação em linguagem C, com interface em terminal e atendimento voltado a pessoas físicas. A pesquisa possui finalidade aplicada e abordagem qualitativa e exploratória, apoiada na análise dos documentos do projeto e em publicações sobre gestão da informação, informatização de pequenas empresas e gerenciamento de serviços de tecnologia. A análise resultou na definição de requisitos, regras de negócio, casos de uso e itens do backlog, com planejamento baseado em Scrum. A solução prevê os perfis Administrador, Recepcionista, Técnico e Cliente, além do registro de diagnóstico, orçamento, decisão do cliente, manutenção, teste e entrega. O controle de estoque está fora do escopo. Espera-se facilitar o acompanhamento dos serviços e a recuperação das informações registradas. Como o desenvolvimento ainda está em andamento, esses benefícios dependem da conclusão e da avaliação da aplicação, sem resultados de implantação em uma empresa real.

**Palavras-chave:** ordens de serviço; manutenção de equipamentos; gestão da informação; linguagem C; engenharia de software.

# Abstract

This paper presents the planning and development of an equipment maintenance management system for ByteTech, a fictional computer repair company. In the scenario studied, the use of separate spreadsheets and notes makes it difficult to locate work orders, identify the staff responsible for them, and review service history. The project aims to organize this information in a C application with a terminal interface, designed to serve individual customers. The research is applied, qualitative, and exploratory, drawing on project documents and publications on information management, computerization in small businesses, and IT service management. The analysis led to the definition of requirements, business rules, use cases, and backlog items, with planning based on Scrum. The proposed solution includes Administrator, Receptionist, Technician, and Customer roles, as well as records of diagnosis, quotations, customer decisions, maintenance, testing, and delivery. Inventory control is outside the project scope. The system is expected to make it easier to track services and retrieve recorded information. As development is still in progress, these benefits depend on the completion and evaluation of the application, and no results from deployment in a real company are reported.

**Keywords:** work orders; equipment maintenance; information management; C programming language; software engineering.

# Introdução

O atendimento em uma assistência técnica produz informações desde o recebimento do equipamento até sua devolução ao cliente. A descrição do defeito, o diagnóstico, o orçamento e os registros de manutenção precisam permanecer relacionados ao mesmo serviço. Quando esses dados são mantidos em controles separados, acompanhar o atendimento pode exigir a consulta a diferentes documentos e aos profissionais envolvidos.

Este trabalho apresenta a proposta de um sistema de gestão de manutenção de equipamentos para a ByteTech, empresa fictícia do setor de Tecnologia da Informação. No cenário definido para o projeto, a empresa utiliza planilhas e anotações para controlar seus atendimentos. A dispersão dos registros dificulta a localização das ordens de serviço, a identificação dos responsáveis e a consulta ao histórico de manutenção.

O objetivo do projeto é desenvolver uma aplicação em linguagem C, com interface em terminal, para organizar os cadastros e acompanhar as ordens de serviço. A solução prevê quatro perfis de acesso: Administrador, Recepcionista, Técnico e Cliente. Cada perfil terá permissões relacionadas às atividades que desempenha no atendimento. Nesta versão, o sistema contempla clientes pessoas físicas.

Para fundamentar a proposta, foram analisados os documentos de caracterização da empresa, os requisitos do projeto e publicações sobre gestão da informação e informatização de pequenas empresas. Também foram consultados trabalhos relacionados à gestão de serviços de tecnologia. A análise desses materiais orienta a definição das funcionalidades e a discussão dos benefícios esperados.

A ByteTech constitui um cenário acadêmico, e as dificuldades descritas fazem parte dessa caracterização. Assim, a contribuição da solução será discutida a partir das necessidades estabelecidas e das evidências disponíveis durante o desenvolvimento, sem atribuir à empresa resultados de uma implantação real.

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
| RNF04 | Manter os dados após o encerramento do programa: nesta versão em arquivos de texto, um por tabela; depois, no SQL Server. |
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

O Diagrama Estendido do projeto foi elaborado a partir do diagrama de classes da UML. Ele mostra as telas, com o estereótipo «boundary», e os dados, com o estereótipo «entity». Como C não possui classes, cada entidade foi implementada como uma estrutura (struct), e cada tela, como um arquivo de código com o seu menu. O Quadro 5 mostra onde cada elemento do diagrama está no código.

Quadro 5 — Correspondência entre o Diagrama Estendido e o código

| Elemento do diagrama | Onde está no código |
|---|---|
| Usuario, Equipamento, OrdemServico, TecnicoOS («entity») | Estruturas (typedef struct) em main.c, gravadas em usuario.txt, equipamento.txt, os.txt e tecnico_os.txt |
| TelaLogin e TelaMeusDados | geral.c: login, logout, meus dados e alterar senha |
| MenuPrincipal | main.c: menu inicial e um menu para cada perfil |
| TelaUsuario | admin.c, recepcionista.c, tecnico.c e cliente.c, que usam as funções comuns de geral.c |
| TelaEquipamento | equipamento.c |
| TelaOrdemServico | ordem_servico.c |
| TelaManutencao | manutencao.c |
| TelaOrcamento | orcamento.c |
| TelaHistoricoRelatorio | relatorio.c |

Fonte: elaboração própria (2026).

Os dados ficam em arquivos de texto, um por tabela. Ao abrir, o programa lê cada arquivo para um vetor de estruturas; a cada alteração, grava o arquivo de novo. O banco SQL Server já foi modelado, mas nesta versão o programa ainda não se conecta a ele. Essa integração é o próximo passo do projeto.

Todos os casos de uso estão implementados: login, cadastros, ordens de serviço, manutenção, orçamento, entrega e relatórios. O funcionamento foi verificado com testes que percorrem o atendimento completo com os quatro perfis.

## 3.7 Backlog do produto

O backlog reúne os itens de trabalho necessários para desenvolver a solução. Cada item recebeu um identificador PBI e foi relacionado aos requisitos e casos de uso correspondentes. O Quadro 6 apresenta os doze itens e suas prioridades na documentação do grupo.

Quadro 6 — Síntese do backlog do produto

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

O grupo adotou o Scrum como referência para organizar o planejamento. Schwaber e Sutherland (2020) descrevem um trabalho organizado em ciclos, com seleção de itens do backlog, avaliação do que foi produzido e adaptação dos próximos passos. No projeto, essa referência foi utilizada para definir responsabilidades, prioridades e períodos de trabalho.

A distribuição registrada atribui a Leonardo Lemgruber Portugal a função de Product Owner e a Elton Ruan da Silva Santos a de Scrum Master. Arthur dos Santos Moraes, Brunno Miranda da Silveira, Matheus Gonçalves da Silva e Natália Nunes Vieira compõem a equipe de desenvolvimento. Essa organização se refere ao grupo acadêmico e não aos perfis de acesso dos usuários da ByteTech.

O planejamento estabelece ciclos semanais, com revisão do trabalho, retrospectiva e planejamento do ciclo seguinte previstos para as sextas-feiras. Os registros das quatro primeiras sprints foram reconstruídos posteriormente, com base no histórico do projeto, conforme informado pelo grupo. Por isso, esses documentos devem ser lidos como uma organização das atividades registradas, sem comprovar a realização de todos os eventos do Scrum durante cada período.

Quadro 7 — Períodos e focos registrados nas sprints

| Sprint | Período de 2026 | Foco registrado |
|---|---|---|
| 01 | 27 de agosto a 4 de setembro | Estrutura da documentação, modelo de dados inicial e primeiros casos de uso. |
| 02 | 5 a 11 de setembro | Casos de uso por módulo e revisão do modelo de usuários. |
| 03 | 12 a 18 de setembro | Revisão dos requisitos, delimitação do escopo e modelo com quatro tabelas. |
| 04 | 19 a 25 de setembro | Fluxogramas, diagramas, descrições dos casos de uso e modelagem do banco. |
| 05 | 26 de setembro a 2 de outubro | Persistência, estrutura do código, autenticação e documentação. |
| 06 | 3 a 9 de outubro | Login, cadastros de usuários e equipamentos gravando em arquivos de texto. |
| 07 | 10 a 16 de outubro | Ordens de serviço, manutenção, orçamento e relatórios; testes e simplificação do código; início deste documento. |
| 08 | 17 a 23 de outubro | Etapas restantes deste documento, protótipos das telas e preparação da apresentação. |

Fonte: elaboração própria, com base nos registros de planejamento do projeto (2026).

O primeiro período registrado inclui a organização inicial e ultrapassa a cadência semanal prevista para os seguintes. As Sprints 05 e 06 foram fechadas com atraso, a partir do histórico do projeto; desde então, cada sprint é fechada na sexta-feira, com revisão e retrospectiva registradas.

O grupo também estabeleceu critérios para considerar uma tarefa pronta. Os documentos e diagramas devem estar disponíveis no repositório com seus arquivos editáveis. Para o código, os critérios incluem compilação sem erros, verificação do fluxo principal e das exceções, respeito às permissões e manutenção dos dados após encerrar o programa. A conclusão ainda exige o vínculo com o requisito e a atualização do registro da sprint.

Esses critérios orientam a avaliação das entregas. Ao concluir cada funcionalidade, será necessário registrar as verificações realizadas e ajustar o planejamento com base nas pendências encontradas.

# Referências

BANNITZ, Francisco Saldanha; ZAFALON, Zaira Regina. A interlocução entre o fluxo informacional e a gestão de processos de negócio: evidências de um mapeamento sistemático de literatura. *InCID: Revista de Ciência da Informação e Documentação*, v. 17, e-231446, 2026. DOI: 10.11606/issn.2178-2075.incid.2026.231446. Disponível em: <https://revistas.usp.br/incid/article/view/231446>. Acesso em: 12 out. 2026.

BERALDI, Lairce Castanhera; ESCRIVÃO FILHO, Edmundo. Impacto da tecnologia de informação na gestão de pequenas empresas. *Ciência da Informação*, v. 29, n. 1, p. 46–50, 2000. DOI: 10.1590/S0100-19652000000100005. Disponível em: <https://www.scielo.br/j/ci/a/bgDCMyb7DLNqFBdyvBCmcLm/>. Acesso em: 12 out. 2026.

MENDES, Juliana Veiga; ESCRIVÃO FILHO, Edmundo. Sistemas integrados de gestão ERP em pequenas empresas: um confronto entre o referencial teórico e a prática empresarial. *Gestão & Produção*, v. 9, n. 3, p. 277–296, 2002. DOI: 10.1590/S0104-530X2002000300006. Disponível em: <https://www.scielo.br/j/gp/a/GmLSKVc7dpRYdBhtbChHDcv/>. Acesso em: 12 out. 2026.

MORAES, Giseli Diniz de Almeida; ESCRIVÃO FILHO, Edmundo. A gestão da informação diante das especificidades das pequenas empresas. *Ciência da Informação*, v. 35, n. 3, p. 124–132, 2006. DOI: 10.18225/ci.inf.v35i3.1119. Disponível em: <https://revista.ibict.br/ciinf/article/view/1119>. Acesso em: 12 out. 2026.

SCHEFFER, João Gabriel; TIBOLA, Leandro Rosniak. *GestaoOS: Sistema de Gerenciamento de Serviços de TI*. 2025. Artigo científico (Graduação em Ciência da Computação) — Instituto Federal de Educação, Ciência e Tecnologia Farroupilha, Campus Frederico Westphalen, Frederico Westphalen, 2025. Disponível em: <https://arandu.iffarroupilha.edu.br/handle/itemid/849>. Acesso em: 12 out. 2026.

SCHWABER, Ken; SUTHERLAND, Jeff. *O Guia do Scrum: o guia definitivo para o Scrum: as regras do jogo*. nov. 2020. Disponível em: <https://scrumguides.org/docs/scrumguide/v2020/2020-Scrum-Guide-PortugueseBR-3.0.pdf>. Acesso em: 12 out. 2026.

TRIGUEIRO, Francisco Mirialdo Chaves; ELIZEU, Rodrigo Carvalho. Informatização no gerenciamento: análise dos resultados em microempresas de Cuiabá – MT. *Revista Estudos e Pesquisas em Administração*, v. 7, n. 3, 2024. DOI: 10.30781/repad.v7i3.15987. Disponível em: <https://periodicoscientificos.ufmt.br/ojs/index.php/repad/article/view/15987>. Acesso em: 12 out. 2026.

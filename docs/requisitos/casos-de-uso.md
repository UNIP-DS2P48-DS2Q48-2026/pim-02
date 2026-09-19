# Casos de Uso Detalhados

Detalhamento de fluxo principal, alternativo e de exceção para os requisitos que representam o processo operacional ([RF04 a RF09](./requisitos.md#requisitos-funcionais)), na ordem em que ocorrem.

Convenção usada abaixo:

- **Fluxo alternativo:** um caminho *recuperável* - o caso de uso continua e ainda pode terminar com sucesso (repetição, negociação).
- **Exceção:** uma condição que *bloqueia ou desvia* o caso de uso do seu término normal (pré-condição não satisfeita, dado obrigatório ausente, regra de negócio violada). Cada exceção indica a etapa exata do fluxo principal em que ocorre, e pode haver mais de uma por caso de uso.

### UC04 - Abrir Ordem de Serviço (RF04)

- **Ator(es):** Recepcionista (ou Administrador, RN11).
- **Pré-condição:** Equipamento identificado.
- **Fluxo principal:**
  1. O atendente coleta os dados do cliente e do equipamento.
  2. O sistema verifica se o cliente já possui cadastro.
  3. O atendente registra o problema relatado.
  4. O sistema abre a OS com status "Aberta" e a encaminha para diagnóstico.
- **Fluxo alternativo:**
  - Na etapa 2: cliente ainda não possui cadastro → o sistema registra o cliente antes de prosseguir para a etapa 3 (RF02).
- **Exceções:**
  - Na etapa 1: equipamento informado sem cliente vinculado e sem dados para novo cadastro → o sistema rejeita a abertura (RN01).
  - Na etapa 1: cliente ou equipamento está inativo → o sistema impede a abertura de nova OS (RN14).
  - Na etapa 3: problema relatado não informado (campo obrigatório vazio) → o sistema impede o avanço para a etapa 4 (RNF09).
- **Pós-condição:** OS registrada com status "Aberta".

### UC06 - Elaborar Orçamento e Registrar Aprovação (RF06)

- **Ator(es):** Recepcionista (ou Administrador, RN11); Cliente aprova ou recusa.
- **Pré-condição:** Diagnóstico técnico registrado (RF05).
- **Fluxo principal:**
  1. O sistema apresenta o relatório técnico.
  2. O atendente calcula o custo e monta a proposta.
  3. O sistema envia o orçamento ao cliente.
  4. O cliente aprova → a OS segue para execução (RF07).
- **Fluxo alternativo:**
  - Na etapa 4: cliente não aprova → o atendente registra a negociação e o ciclo recomeça a partir da etapa 3.
- **Exceções:**
  - Na etapa 1: não existe diagnóstico técnico registrado para a OS → o sistema impede o início do orçamento (pré-condição não satisfeita, RF05).
  - Na etapa 4, após a negociação: cliente não aprova a proposta negociada → a OS é arquivada e o histórico é mantido (RN09).
- **Pós-condição:** OS com orçamento aprovado ou arquivada.

### UC07 - Executar Manutenção (RF07)

- **Ator(es):** um ou mais Técnicos (ou Administrador, RN11).
- **Pré-condição:** Orçamento aprovado (RF06).
- **Fluxo principal:**
  1. O(s) técnico(s) separam peças e realizam o reparo.
  2. O técnico registra as ações realizadas.
  3. O sistema atualiza o status da OS.
- **Fluxo alternativo:**
  - Na etapa 1: mais de um técnico é necessário → cada um gera um registro próprio vinculado à mesma OS (RN04).
  - Na etapa 2: reparo não funciona → o técnico registra o problema e o fluxo reinicia a partir da etapa 1.
- **Exceções:**
  - Na etapa 1: OS sem orçamento aprovado → o sistema impede o início da execução (pré-condição não satisfeita, RF06).
- **Pós-condição:** OS com reparo concluído, pronta para teste.

### UC08 - Registrar Teste de Funcionamento (RF08)

- **Ator(es):** Técnico (ou Administrador, RN11).
- **Pré-condição:** Reparo concluído (RF07).
- **Fluxo principal:**
  1. O técnico realiza o teste pós-reparo.
  2. O sistema registra o desempenho verificado.
  3. Desempenho aprovado → aprovação interna → a OS segue para entrega (RF09).
- **Fluxo alternativo:**
  - Na etapa 3: desempenho não aprovado → o técnico registra o problema e a OS retorna à etapa 1 de UC07 (novo reparo).
- **Exceções:**
  - Na etapa 1: OS sem reparo registrado como concluído → o sistema impede o início do teste (pré-condição não satisfeita, RF07).
- **Pós-condição:** OS aprovada internamente ou reencaminhada para novo reparo.

### UC09 - Registrar Entrega, Pagamento e Feedback (RF09)

- **Ator(es):** Recepcionista (ou Administrador, RN11); Cliente dá o feedback.
- **Pré-condição:** Teste de funcionamento aprovado (RF08).
- **Fluxo principal:**
  1. O atendente notifica o cliente e apresenta a OS.
  2. O sistema registra o pagamento recebido.
  3. O cliente informa se está satisfeito.
  4. Satisfeito → o sistema registra o feedback e finaliza a OS.
- **Fluxo alternativo:**
  - Na etapa 3: cliente não satisfeito → o sistema registra o feedback e o erro relatado, retornando a OS para UC07.
- **Exceções:**
  - Na etapa 1: OS sem teste de funcionamento aprovado → o sistema impede a notificação de entrega (pré-condição não satisfeita, RF08).
  - Na etapa 2: pagamento informado não corresponde ao valor do orçamento aprovado → o sistema alerta a divergência antes de prosseguir (RNF09).
- **Pós-condição:** OS finalizada com pagamento e feedback registrados, ou reaberta para correção.

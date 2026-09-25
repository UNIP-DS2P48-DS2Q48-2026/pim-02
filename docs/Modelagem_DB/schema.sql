-- ByteTech - Sistema de Gestao de Manutencao de Equipamentos
-- Modelo fisico (SQLite). Detalhes e justificativas em der-simplificado.md.
-- Nomenclatura: tabelas no singular e em portugues (usuario, equipamento, os, tecnico_os).

PRAGMA foreign_keys = ON;

CREATE TABLE usuario (
    id_usuario     INTEGER PRIMARY KEY AUTOINCREMENT,
    nome           TEXT    NOT NULL,
    email          TEXT    NOT NULL UNIQUE,
    telefone       TEXT,
    senha          TEXT    NOT NULL,
    perfil         TEXT    NOT NULL CHECK (perfil IN ('admin', 'recepcionista', 'tecnico', 'cliente')),
    cpf            TEXT    UNIQUE,
    especialidade  TEXT,
    status         TEXT    NOT NULL DEFAULT 'ativo' CHECK (status IN ('ativo', 'inativo')),
    criado_em      DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE equipamento (
    id_equipamento       INTEGER PRIMARY KEY AUTOINCREMENT,
    id_cliente           INTEGER NOT NULL REFERENCES usuario(id_usuario),
    categoria            TEXT    NOT NULL CHECK (categoria IN ('hardware', 'software')),
    tipo                 TEXT,
    marca                TEXT,
    modelo_numero_serie  TEXT,
    status               TEXT    NOT NULL DEFAULT 'ativo' CHECK (status IN ('ativo', 'inativo')),
    criado_em            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE os (
    id_os                       INTEGER PRIMARY KEY AUTOINCREMENT,
    id_equipamento              INTEGER NOT NULL REFERENCES equipamento(id_equipamento),
    id_atendente                INTEGER NOT NULL REFERENCES usuario(id_usuario),
    data_abertura               DATE    NOT NULL DEFAULT CURRENT_DATE,
    problema_relatado           TEXT,
    diagnostico                 TEXT,
    orcamento                   DECIMAL(10,2),
    status                      TEXT    NOT NULL DEFAULT 'aberta'
                                 CHECK (status IN ('aberta', 'em_andamento', 'finalizada', 'cancelada')),
    resultado_resposta_cliente  TEXT    CHECK (resultado_resposta_cliente IN ('aprovado', 'recusado')),
    data_entrega                DATE,
    resultado_teste             TEXT    CHECK (resultado_teste IN ('aprovado', 'reprovado')),
    valor_pago                  DECIMAL(10,2),
    feedback_cliente            TEXT,
    satisfeito                  TEXT    CHECK (satisfeito IN ('sim', 'nao')),
    criado_em                   DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em               DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

-- Associativa tecnico <-> os. id_tecnico e' uma FK simples para usuario - o
-- papel certo (tecnico, ou admin cobrindo ausencia via RN11) e' garantido
-- pelo trigger logo abaixo, nao por uma coluna redundante na tabela.
CREATE TABLE tecnico_os (
    id_tecnico_os        INTEGER PRIMARY KEY AUTOINCREMENT,
    id_tecnico           INTEGER NOT NULL REFERENCES usuario(id_usuario),
    id_os                INTEGER NOT NULL REFERENCES os(id_os),
    data_atribuicao      DATE    NOT NULL DEFAULT CURRENT_DATE,
    observacoes_tecnicas TEXT,
    criado_em            DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em        DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP
);

-- ---------------------------------------------------------------------
-- Triggers de integridade de papel (RN15, RN16) - ver "Integridade de
-- papel" em der-simplificado.md. Cada um cobre INSERT e UPDATE da FK.
-- ---------------------------------------------------------------------

-- RN15: tecnico_os.id_tecnico so pode ser um usuario com perfil tecnico ou
-- admin (admin cobre a ausencia de tecnico, RN11).
CREATE TRIGGER trg_tecnico_os_valida_perfil_ins
BEFORE INSERT ON tecnico_os
FOR EACH ROW
WHEN (SELECT perfil FROM usuario WHERE id_usuario = NEW.id_tecnico) NOT IN ('tecnico', 'admin')
BEGIN
    SELECT RAISE(ABORT, 'id_tecnico deve referenciar um usuario com perfil tecnico ou admin');
END;

CREATE TRIGGER trg_tecnico_os_valida_perfil_upd
BEFORE UPDATE OF id_tecnico ON tecnico_os
FOR EACH ROW
WHEN (SELECT perfil FROM usuario WHERE id_usuario = NEW.id_tecnico) NOT IN ('tecnico', 'admin')
BEGIN
    SELECT RAISE(ABORT, 'id_tecnico deve referenciar um usuario com perfil tecnico ou admin');
END;

-- RN16: equipamento.id_cliente so pode ser um usuario com perfil cliente.
CREATE TRIGGER trg_equipamento_valida_cliente_ins
BEFORE INSERT ON equipamento
FOR EACH ROW
WHEN (SELECT perfil FROM usuario WHERE id_usuario = NEW.id_cliente) <> 'cliente'
BEGIN
    SELECT RAISE(ABORT, 'id_cliente deve referenciar um usuario com perfil cliente');
END;

CREATE TRIGGER trg_equipamento_valida_cliente_upd
BEFORE UPDATE OF id_cliente ON equipamento
FOR EACH ROW
WHEN (SELECT perfil FROM usuario WHERE id_usuario = NEW.id_cliente) <> 'cliente'
BEGIN
    SELECT RAISE(ABORT, 'id_cliente deve referenciar um usuario com perfil cliente');
END;

-- ---------------------------------------------------------------------
-- Triggers de RN14 (inativo nao entra em OS nova, nem e' inativado com OS
-- em andamento) - ver "Integridade de status (RN14)" em der-simplificado.md.
-- ---------------------------------------------------------------------

-- Ao abrir uma OS: equipamento, o cliente do equipamento e o atendente
-- precisam estar ativos.
CREATE TRIGGER trg_os_valida_ativos_ins
BEFORE INSERT ON os
FOR EACH ROW
WHEN (SELECT status FROM equipamento WHERE id_equipamento = NEW.id_equipamento) <> 'ativo'
    OR (SELECT status FROM usuario WHERE id_usuario = (
            SELECT id_cliente FROM equipamento WHERE id_equipamento = NEW.id_equipamento
        )) <> 'ativo'
    OR (SELECT status FROM usuario WHERE id_usuario = NEW.id_atendente) <> 'ativo'
BEGIN
    SELECT RAISE(ABORT, 'equipamento, cliente e atendente devem estar ativos para abrir uma OS (RN14)');
END;

-- Ao atribuir um tecnico a uma OS: o tecnico precisa estar ativo.
CREATE TRIGGER trg_tecnico_os_valida_ativo_ins
BEFORE INSERT ON tecnico_os
FOR EACH ROW
WHEN (SELECT status FROM usuario WHERE id_usuario = NEW.id_tecnico) <> 'ativo'
BEGIN
    SELECT RAISE(ABORT, 'tecnico deve estar ativo para ser atribuido a uma OS (RN14)');
END;

-- Ao inativar um usuario: rejeita se ele tiver OS em andamento como
-- atendente, como tecnico (via tecnico_os) ou como cliente (via equipamento).
CREATE TRIGGER trg_usuario_valida_inativacao
BEFORE UPDATE OF status ON usuario
FOR EACH ROW
WHEN NEW.status = 'inativo' AND OLD.status = 'ativo' AND (
    EXISTS (SELECT 1 FROM os WHERE id_atendente = NEW.id_usuario AND status IN ('aberta', 'em_andamento'))
    OR EXISTS (
        SELECT 1 FROM tecnico_os t JOIN os o ON o.id_os = t.id_os
        WHERE t.id_tecnico = NEW.id_usuario AND o.status IN ('aberta', 'em_andamento')
    )
    OR EXISTS (
        SELECT 1 FROM equipamento e JOIN os o ON o.id_equipamento = e.id_equipamento
        WHERE e.id_cliente = NEW.id_usuario AND o.status IN ('aberta', 'em_andamento')
    )
)
BEGIN
    SELECT RAISE(ABORT, 'usuario nao pode ser inativado com ordem de servico em andamento (RN14)');
END;

-- Ao inativar um equipamento: rejeita se ele tiver OS em andamento.
CREATE TRIGGER trg_equipamento_valida_inativacao
BEFORE UPDATE OF status ON equipamento
FOR EACH ROW
WHEN NEW.status = 'inativo' AND OLD.status = 'ativo' AND EXISTS (
    SELECT 1 FROM os WHERE id_equipamento = NEW.id_equipamento AND status IN ('aberta', 'em_andamento')
)
BEGIN
    SELECT RAISE(ABORT, 'equipamento nao pode ser inativado com ordem de servico em andamento (RN14)');
END;

-- ---------------------------------------------------------------------
-- Trigger de alteracao de perfil - ver "Alteracao de perfil apos vinculo
-- existente" na secao "Integridade de papel" em der-simplificado.md.
-- usuario.perfil pode ser alterado via RF02 ("alterar" usuario); este
-- trigger impede que a alteracao deixe uma referencia existente em
-- tecnico_os/equipamento incompativel (RN15/RN16).
-- ---------------------------------------------------------------------
CREATE TRIGGER trg_usuario_valida_troca_perfil
BEFORE UPDATE OF perfil ON usuario
FOR EACH ROW
WHEN (
        EXISTS (SELECT 1 FROM tecnico_os WHERE id_tecnico = NEW.id_usuario)
        AND NEW.perfil NOT IN ('tecnico', 'admin')
    )
    OR (
        EXISTS (SELECT 1 FROM equipamento WHERE id_cliente = NEW.id_usuario)
        AND NEW.perfil <> 'cliente'
    )
BEGIN
    SELECT RAISE(ABORT, 'novo perfil e incompativel com vinculos existentes em tecnico_os ou equipamento');
END;

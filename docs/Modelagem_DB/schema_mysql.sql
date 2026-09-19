-- ByteTech - Sistema de Gestao de Manutencao de Equipamentos
-- Modelo fisico para MySQL/MariaDB (XAMPP). Equivalente a schema.sql (SQLite).
-- Detalhes e justificativas em der-simplificado.md.
--
-- Diferenca deste arquivo para schema.sql/schema_sqlserver.sql: os campos de
-- dominio fixo (perfil, status, categoria) usam ENUM nativo do MySQL/MariaDB,
-- alem de VARCHAR + CHECK - o phpMyAdmin exibe os valores permitidos na
-- coluna "Type" da Structure (SQLite e SQL Server nao possuem ENUM, por isso
-- permanecem apenas com CHECK).
--
-- ENUM isoladamente nao garante a restricao: com o sql_mode padrao do XAMPP
-- (sem STRICT_TRANS_TABLES), um valor fora da lista e aceito e substituido
-- silenciosamente por string vazia, em vez de gerar erro. Por esse motivo o
-- CHECK permanece ao lado do ENUM: CHECK e avaliado independentemente do
-- sql_mode da sessao ou do servidor.

CREATE DATABASE IF NOT EXISTS bytetech
    CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE bytetech;

DROP TABLE IF EXISTS tecnico_os;
DROP TABLE IF EXISTS os;
DROP TABLE IF EXISTS equipamento;
DROP TABLE IF EXISTS usuario;

CREATE TABLE usuario (
    id_usuario     INT           NOT NULL AUTO_INCREMENT,
    nome           VARCHAR(120)  NOT NULL,
    email          VARCHAR(120)  NOT NULL,
    telefone       VARCHAR(20),
    senha          VARCHAR(255)  NOT NULL,
    perfil         ENUM('admin', 'recepcionista', 'tecnico', 'cliente') NOT NULL,
    cpf            VARCHAR(14),
    especialidade  VARCHAR(80),
    status         ENUM('ativo', 'inativo') NOT NULL DEFAULT 'ativo',
    criado_em      DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em  DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (id_usuario),
    UNIQUE KEY uq_usuario_email (email),
    UNIQUE KEY uq_usuario_cpf (cpf),
    CONSTRAINT chk_usuario_perfil CHECK (perfil IN ('admin', 'recepcionista', 'tecnico', 'cliente')),
    CONSTRAINT chk_usuario_status CHECK (status IN ('ativo', 'inativo'))
) ENGINE=InnoDB;

-- id_cliente e' uma FK simples para usuario - o perfil certo (cliente, RN16)
-- e' garantido pelos triggers no final do arquivo, nao por uma coluna redundante.
CREATE TABLE equipamento (
    id_equipamento       INT           NOT NULL AUTO_INCREMENT,
    id_cliente           INT           NOT NULL,
    categoria            ENUM('hardware', 'software') NOT NULL,
    tipo                 VARCHAR(60),
    marca                VARCHAR(60),
    modelo_numero_serie  VARCHAR(120),
    status               ENUM('ativo', 'inativo') NOT NULL DEFAULT 'ativo',
    criado_em            DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em        DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (id_equipamento),
    CONSTRAINT fk_equipamento_cliente FOREIGN KEY (id_cliente) REFERENCES usuario (id_usuario),
    CONSTRAINT chk_equipamento_categoria CHECK (categoria IN ('hardware', 'software')),
    CONSTRAINT chk_equipamento_status CHECK (status IN ('ativo', 'inativo'))
) ENGINE=InnoDB;

CREATE TABLE os (
    id_os                       INT            NOT NULL AUTO_INCREMENT,
    id_equipamento              INT            NOT NULL,
    id_atendente                INT            NOT NULL,
    data_abertura               DATE           NOT NULL,
    problema_relatado           TEXT,
    diagnostico                 TEXT,
    orcamento                   DECIMAL(10,2),
    status                      ENUM('aberta', 'em_andamento', 'finalizada', 'cancelada') NOT NULL DEFAULT 'aberta',
    resultado_resposta_cliente  VARCHAR(255),
    data_entrega                DATE,
    resultado_teste             ENUM('aprovado', 'reprovado'),
    valor_pago                  DECIMAL(10,2),
    feedback_cliente            TEXT,
    satisfeito                  ENUM('sim', 'nao'),
    criado_em                   DATETIME       NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em               DATETIME       NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (id_os),
    CONSTRAINT fk_os_equipamento FOREIGN KEY (id_equipamento) REFERENCES equipamento (id_equipamento),
    CONSTRAINT fk_os_atendente   FOREIGN KEY (id_atendente)   REFERENCES usuario (id_usuario),
    CONSTRAINT chk_os_status CHECK (status IN ('aberta', 'em_andamento', 'finalizada', 'cancelada')),
    CONSTRAINT chk_os_resultado_teste CHECK (resultado_teste IN ('aprovado', 'reprovado')),
    CONSTRAINT chk_os_satisfeito CHECK (satisfeito IN ('sim', 'nao'))
) ENGINE=InnoDB;

-- Associativa tecnico <-> os. id_tecnico e' uma FK simples para usuario - o
-- perfil certo (tecnico, ou admin cobrindo ausencia, RN11/RN15) e' garantido
-- pelos triggers no final do arquivo, nao por uma coluna redundante.
CREATE TABLE tecnico_os (
    id_tecnico_os        INT           NOT NULL AUTO_INCREMENT,
    id_tecnico           INT           NOT NULL,
    id_os                INT           NOT NULL,
    data_atribuicao      DATE          NOT NULL,
    observacoes_tecnicas TEXT,
    criado_em            DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP,
    atualizado_em        DATETIME      NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (id_tecnico_os),
    CONSTRAINT fk_tecnico_os_tecnico FOREIGN KEY (id_tecnico) REFERENCES usuario (id_usuario),
    CONSTRAINT fk_tecnico_os_os      FOREIGN KEY (id_os)      REFERENCES os (id_os)
) ENGINE=InnoDB;

-- ---------------------------------------------------------------------
-- Triggers de integridade de papel (RN15, RN16) - ver "Integridade de
-- papel" em der-simplificado.md. Cada um cobre INSERT e UPDATE da FK.
-- ---------------------------------------------------------------------
DELIMITER $$

CREATE TRIGGER trg_tecnico_os_valida_perfil_ins
BEFORE INSERT ON tecnico_os
FOR EACH ROW
BEGIN
    DECLARE v_perfil VARCHAR(20);
    SELECT perfil INTO v_perfil FROM usuario WHERE id_usuario = NEW.id_tecnico;
    IF v_perfil NOT IN ('tecnico', 'admin') THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'id_tecnico deve referenciar um usuario com perfil tecnico ou admin';
    END IF;
END$$

CREATE TRIGGER trg_tecnico_os_valida_perfil_upd
BEFORE UPDATE ON tecnico_os
FOR EACH ROW
BEGIN
    DECLARE v_perfil VARCHAR(20);
    IF NEW.id_tecnico <> OLD.id_tecnico THEN
        SELECT perfil INTO v_perfil FROM usuario WHERE id_usuario = NEW.id_tecnico;
        IF v_perfil NOT IN ('tecnico', 'admin') THEN
            SIGNAL SQLSTATE '45000'
                SET MESSAGE_TEXT = 'id_tecnico deve referenciar um usuario com perfil tecnico ou admin';
        END IF;
    END IF;
END$$

CREATE TRIGGER trg_equipamento_valida_cliente_ins
BEFORE INSERT ON equipamento
FOR EACH ROW
BEGIN
    DECLARE v_perfil VARCHAR(20);
    SELECT perfil INTO v_perfil FROM usuario WHERE id_usuario = NEW.id_cliente;
    IF v_perfil <> 'cliente' THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'id_cliente deve referenciar um usuario com perfil cliente';
    END IF;
END$$

CREATE TRIGGER trg_equipamento_valida_cliente_upd
BEFORE UPDATE ON equipamento
FOR EACH ROW
BEGIN
    DECLARE v_perfil VARCHAR(20);
    IF NEW.id_cliente <> OLD.id_cliente THEN
        SELECT perfil INTO v_perfil FROM usuario WHERE id_usuario = NEW.id_cliente;
        IF v_perfil <> 'cliente' THEN
            SIGNAL SQLSTATE '45000'
                SET MESSAGE_TEXT = 'id_cliente deve referenciar um usuario com perfil cliente';
        END IF;
    END IF;
END$$

-- ---------------------------------------------------------------------
-- Triggers de RN14 (inativo nao entra em OS nova, nem e' inativado com OS
-- em andamento) - ver "Integridade de status (RN14)" em der-simplificado.md.
-- ---------------------------------------------------------------------

CREATE TRIGGER trg_os_valida_ativos_ins
BEFORE INSERT ON os
FOR EACH ROW
BEGIN
    DECLARE v_status_equip VARCHAR(10);
    DECLARE v_id_cliente INT;
    DECLARE v_status_cliente VARCHAR(10);
    DECLARE v_status_atendente VARCHAR(10);
    SELECT status, id_cliente INTO v_status_equip, v_id_cliente FROM equipamento WHERE id_equipamento = NEW.id_equipamento;
    SELECT status INTO v_status_cliente FROM usuario WHERE id_usuario = v_id_cliente;
    SELECT status INTO v_status_atendente FROM usuario WHERE id_usuario = NEW.id_atendente;
    IF v_status_equip <> 'ativo' OR v_status_cliente <> 'ativo' OR v_status_atendente <> 'ativo' THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'equipamento, cliente e atendente devem estar ativos para abrir uma OS (RN14)';
    END IF;
END$$

CREATE TRIGGER trg_tecnico_os_valida_ativo_ins
BEFORE INSERT ON tecnico_os
FOR EACH ROW
BEGIN
    DECLARE v_status VARCHAR(10);
    SELECT status INTO v_status FROM usuario WHERE id_usuario = NEW.id_tecnico;
    IF v_status <> 'ativo' THEN
        SIGNAL SQLSTATE '45000'
            SET MESSAGE_TEXT = 'tecnico deve estar ativo para ser atribuido a uma OS (RN14)';
    END IF;
END$$

CREATE TRIGGER trg_usuario_valida_inativacao
BEFORE UPDATE ON usuario
FOR EACH ROW
BEGIN
    DECLARE v_em_andamento INT;
    IF NEW.status = 'inativo' AND OLD.status = 'ativo' THEN
        SELECT COUNT(*) INTO v_em_andamento FROM (
            SELECT 1 FROM os WHERE id_atendente = NEW.id_usuario AND status IN ('aberta', 'em_andamento')
            UNION ALL
            SELECT 1 FROM tecnico_os t JOIN os o ON o.id_os = t.id_os
                WHERE t.id_tecnico = NEW.id_usuario AND o.status IN ('aberta', 'em_andamento')
            UNION ALL
            SELECT 1 FROM equipamento e JOIN os o ON o.id_equipamento = e.id_equipamento
                WHERE e.id_cliente = NEW.id_usuario AND o.status IN ('aberta', 'em_andamento')
        ) AS vinculos;
        IF v_em_andamento > 0 THEN
            SIGNAL SQLSTATE '45000'
                SET MESSAGE_TEXT = 'usuario nao pode ser inativado com ordem de servico em andamento (RN14)';
        END IF;
    END IF;
END$$

CREATE TRIGGER trg_equipamento_valida_inativacao
BEFORE UPDATE ON equipamento
FOR EACH ROW
BEGIN
    DECLARE v_em_andamento INT;
    IF NEW.status = 'inativo' AND OLD.status = 'ativo' THEN
        SELECT COUNT(*) INTO v_em_andamento FROM os
            WHERE id_equipamento = NEW.id_equipamento AND status IN ('aberta', 'em_andamento');
        IF v_em_andamento > 0 THEN
            SIGNAL SQLSTATE '45000'
                SET MESSAGE_TEXT = 'equipamento nao pode ser inativado com ordem de servico em andamento (RN14)';
        END IF;
    END IF;
END$$

-- ---------------------------------------------------------------------
-- Trigger de alteracao de perfil - ver "Alteracao de perfil apos vinculo
-- existente" na secao "Integridade de papel" em der-simplificado.md.
-- ---------------------------------------------------------------------
CREATE TRIGGER trg_usuario_valida_troca_perfil
BEFORE UPDATE ON usuario
FOR EACH ROW
BEGIN
    DECLARE v_tem_tecnico_os INT;
    DECLARE v_tem_equipamento INT;
    IF NEW.perfil <> OLD.perfil THEN
        SELECT COUNT(*) INTO v_tem_tecnico_os FROM tecnico_os WHERE id_tecnico = NEW.id_usuario;
        SELECT COUNT(*) INTO v_tem_equipamento FROM equipamento WHERE id_cliente = NEW.id_usuario;
        IF (v_tem_tecnico_os > 0 AND NEW.perfil NOT IN ('tecnico', 'admin'))
           OR (v_tem_equipamento > 0 AND NEW.perfil <> 'cliente') THEN
            SIGNAL SQLSTATE '45000'
                SET MESSAGE_TEXT = 'novo perfil e incompativel com vinculos existentes em tecnico_os ou equipamento';
        END IF;
    END IF;
END$$

DELIMITER ;
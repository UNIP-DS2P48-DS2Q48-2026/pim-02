-- ByteTech - Sistema de Gestao de Manutencao de Equipamentos
-- Modelo fisico para SQL Server. Equivalente a schema.sql (SQLite) / schema_mysql.sql (XAMPP).
-- Detalhes e justificativas em der-simplificado.md.

IF DB_ID('bytetech') IS NULL
    CREATE DATABASE bytetech;
GO

USE bytetech;
GO

-- Necessario para o indice unico filtrado de usuario.cpf mais abaixo (e para
-- qualquer sessao que depois for inserir/atualizar linhas nessa tabela).
SET QUOTED_IDENTIFIER ON;
GO

DROP TABLE IF EXISTS tecnico_os;
DROP TABLE IF EXISTS os;
DROP TABLE IF EXISTS equipamento;
DROP TABLE IF EXISTS usuario;
GO

CREATE TABLE usuario (
    id_usuario     INT             IDENTITY(1,1) PRIMARY KEY,
    nome           NVARCHAR(120)   NOT NULL,
    email          NVARCHAR(120)   NOT NULL,
    telefone       NVARCHAR(20)    NULL,
    senha          NVARCHAR(255)   NOT NULL,
    perfil         NVARCHAR(20)    NOT NULL,
    cpf            NVARCHAR(14)    NULL,
    especialidade  NVARCHAR(80)    NULL,
    status         NVARCHAR(10)    NOT NULL DEFAULT 'ativo',
    criado_em      DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    atualizado_em  DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    CONSTRAINT uq_usuario_email    UNIQUE (email),
    CONSTRAINT chk_usuario_perfil CHECK (perfil IN ('admin', 'recepcionista', 'tecnico', 'cliente')),
    CONSTRAINT chk_usuario_status CHECK (status IN ('ativo', 'inativo'))
);
GO

-- cpf so e' preenchido para perfil = cliente (RN13). Uma UNIQUE constraint comum
-- barraria isso: ao contrario de SQLite/MySQL/ANSI SQL, o SQL Server trata NULL
-- como valor igual a si mesmo numa UNIQUE constraint, entao a partir do 2o usuario
-- sem cpf (admin, recepcionista, tecnico) o INSERT falharia. Um indice UNICO
-- FILTRADO resolve: unico so entre os valores nao nulos.
CREATE UNIQUE INDEX uq_usuario_cpf ON usuario (cpf) WHERE cpf IS NOT NULL;
GO

-- id_cliente e' uma FK simples para usuario - o perfil certo (cliente, RN16)
-- e' garantido pelo trigger no final do arquivo, nao por uma coluna redundante.
CREATE TABLE equipamento (
    id_equipamento       INT             IDENTITY(1,1) PRIMARY KEY,
    id_cliente           INT             NOT NULL,
    categoria            NVARCHAR(10)    NOT NULL,
    tipo                 NVARCHAR(60)    NULL,
    marca                NVARCHAR(60)    NULL,
    modelo_numero_serie  NVARCHAR(120)   NULL,
    status               NVARCHAR(10)    NOT NULL DEFAULT 'ativo',
    criado_em            DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    atualizado_em        DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    CONSTRAINT fk_equipamento_cliente FOREIGN KEY (id_cliente) REFERENCES usuario (id_usuario),
    CONSTRAINT chk_equipamento_categoria CHECK (categoria IN ('hardware', 'software')),
    CONSTRAINT chk_equipamento_status CHECK (status IN ('ativo', 'inativo'))
);
GO

CREATE TABLE os (
    id_os                       INT             IDENTITY(1,1) PRIMARY KEY,
    id_equipamento              INT             NOT NULL,
    id_atendente                INT             NOT NULL,
    data_abertura               DATE            NOT NULL,
    problema_relatado           NVARCHAR(MAX)   NULL,
    diagnostico                 NVARCHAR(MAX)   NULL,
    orcamento                   DECIMAL(10,2)   NULL,
    status                      NVARCHAR(20)    NOT NULL DEFAULT 'aberta',
    resultado_resposta_cliente  NVARCHAR(10)    NULL,
    data_entrega                DATE            NULL,
    resultado_teste             NVARCHAR(10)    NULL,
    valor_pago                  DECIMAL(10,2)   NULL,
    feedback_cliente            NVARCHAR(MAX)   NULL,
    satisfeito                  NVARCHAR(5)     NULL,
    criado_em                   DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    atualizado_em                DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    CONSTRAINT fk_os_equipamento FOREIGN KEY (id_equipamento) REFERENCES equipamento (id_equipamento),
    CONSTRAINT fk_os_atendente   FOREIGN KEY (id_atendente)   REFERENCES usuario (id_usuario),
    CONSTRAINT chk_os_status CHECK (status IN ('aberta', 'em_andamento', 'finalizada', 'cancelada')),
    CONSTRAINT chk_os_resultado_teste CHECK (resultado_teste IN ('aprovado', 'reprovado')),
    CONSTRAINT chk_os_resposta_cliente CHECK (resultado_resposta_cliente IN ('aprovado', 'recusado')),
    CONSTRAINT chk_os_satisfeito CHECK (satisfeito IN ('sim', 'nao'))
);
GO

-- Associativa tecnico <-> os. id_tecnico e' uma FK simples para usuario - o
-- perfil certo (tecnico, ou admin cobrindo ausencia, RN11/RN15) e' garantido
-- pelo trigger no final do arquivo, nao por uma coluna redundante.
CREATE TABLE tecnico_os (
    id_tecnico_os        INT             IDENTITY(1,1) PRIMARY KEY,
    id_tecnico           INT             NOT NULL,
    id_os                INT             NOT NULL,
    data_atribuicao      DATE            NOT NULL,
    observacoes_tecnicas NVARCHAR(MAX)   NULL,
    criado_em            DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    atualizado_em        DATETIME2       NOT NULL DEFAULT SYSDATETIME(),
    CONSTRAINT fk_tecnico_os_tecnico FOREIGN KEY (id_tecnico) REFERENCES usuario (id_usuario),
    CONSTRAINT fk_tecnico_os_os      FOREIGN KEY (id_os)      REFERENCES os (id_os)
);
GO

-- ---------------------------------------------------------------------
-- Triggers de integridade de papel (RN15, RN16) - ver "Integridade de
-- papel" em der-simplificado.md. AFTER INSERT/UPDATE (padrao em T-SQL);
-- avalia todas as linhas afetadas via a tabela virtual "inserted".
-- ---------------------------------------------------------------------
CREATE TRIGGER trg_tecnico_os_valida_perfil
ON tecnico_os
AFTER INSERT, UPDATE
AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (
        SELECT 1
        FROM inserted i
        JOIN usuario u ON u.id_usuario = i.id_tecnico
        WHERE u.perfil NOT IN ('tecnico', 'admin')
    )
    BEGIN
        RAISERROR('id_tecnico deve referenciar um usuario com perfil tecnico ou admin', 16, 1);
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    END
END;
GO

CREATE TRIGGER trg_equipamento_valida_cliente
ON equipamento
AFTER INSERT, UPDATE
AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (
        SELECT 1
        FROM inserted i
        JOIN usuario u ON u.id_usuario = i.id_cliente
        WHERE u.perfil <> 'cliente'
    )
    BEGIN
        RAISERROR('id_cliente deve referenciar um usuario com perfil cliente', 16, 1);
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    END
END;
GO

-- ---------------------------------------------------------------------
-- Triggers de RN14 (inativo nao entra em OS nova, nem e' inativado com OS
-- em andamento) - ver "Integridade de status (RN14)" em der-simplificado.md.
-- ---------------------------------------------------------------------

CREATE TRIGGER trg_os_valida_ativos
ON os
AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (
        SELECT 1
        FROM inserted i
        JOIN equipamento e ON e.id_equipamento = i.id_equipamento
        JOIN usuario cli ON cli.id_usuario = e.id_cliente
        JOIN usuario atd ON atd.id_usuario = i.id_atendente
        WHERE e.status <> 'ativo' OR cli.status <> 'ativo' OR atd.status <> 'ativo'
    )
    BEGIN
        RAISERROR('equipamento, cliente e atendente devem estar ativos para abrir uma OS (RN14)', 16, 1);
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    END
END;
GO

CREATE TRIGGER trg_tecnico_os_valida_ativo
ON tecnico_os
AFTER INSERT
AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (
        SELECT 1 FROM inserted i
        JOIN usuario u ON u.id_usuario = i.id_tecnico
        WHERE u.status <> 'ativo'
    )
    BEGIN
        RAISERROR('tecnico deve estar ativo para ser atribuido a uma OS (RN14)', 16, 1);
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    END
END;
GO

CREATE TRIGGER trg_usuario_valida_inativacao
ON usuario
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (
        SELECT 1
        FROM inserted i
        JOIN deleted d ON d.id_usuario = i.id_usuario
        WHERE i.status = 'inativo' AND d.status = 'ativo'
          AND (
              EXISTS (SELECT 1 FROM os WHERE id_atendente = i.id_usuario AND status IN ('aberta', 'em_andamento'))
              OR EXISTS (
                  SELECT 1 FROM tecnico_os t JOIN os o ON o.id_os = t.id_os
                  WHERE t.id_tecnico = i.id_usuario AND o.status IN ('aberta', 'em_andamento')
              )
              OR EXISTS (
                  SELECT 1 FROM equipamento e JOIN os o ON o.id_equipamento = e.id_equipamento
                  WHERE e.id_cliente = i.id_usuario AND o.status IN ('aberta', 'em_andamento')
              )
          )
    )
    BEGIN
        RAISERROR('usuario nao pode ser inativado com ordem de servico em andamento (RN14)', 16, 1);
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    END
END;
GO

CREATE TRIGGER trg_equipamento_valida_inativacao
ON equipamento
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (
        SELECT 1
        FROM inserted i
        JOIN deleted d ON d.id_equipamento = i.id_equipamento
        WHERE i.status = 'inativo' AND d.status = 'ativo'
          AND EXISTS (SELECT 1 FROM os WHERE id_equipamento = i.id_equipamento AND status IN ('aberta', 'em_andamento'))
    )
    BEGIN
        RAISERROR('equipamento nao pode ser inativado com ordem de servico em andamento (RN14)', 16, 1);
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    END
END;
GO

-- ---------------------------------------------------------------------
-- Trigger de alteracao de perfil - ver "Alteracao de perfil apos vinculo
-- existente" na secao "Integridade de papel" em der-simplificado.md.
-- ---------------------------------------------------------------------
CREATE TRIGGER trg_usuario_valida_troca_perfil
ON usuario
AFTER UPDATE
AS
BEGIN
    SET NOCOUNT ON;
    IF EXISTS (
        SELECT 1
        FROM inserted i
        JOIN deleted d ON d.id_usuario = i.id_usuario
        WHERE i.perfil <> d.perfil
          AND (
              (EXISTS (SELECT 1 FROM tecnico_os WHERE id_tecnico = i.id_usuario) AND i.perfil NOT IN ('tecnico', 'admin'))
              OR (EXISTS (SELECT 1 FROM equipamento WHERE id_cliente = i.id_usuario) AND i.perfil <> 'cliente')
          )
    )
    BEGIN
        RAISERROR('novo perfil e incompativel com vinculos existentes em tecnico_os ou equipamento', 16, 1);
        IF @@TRANCOUNT > 0 ROLLBACK TRANSACTION;
    END
END;
GO

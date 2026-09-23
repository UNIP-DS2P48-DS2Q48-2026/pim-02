USE bytetech;
GO
SET NOCOUNT ON;

PRINT '=========================================================';
PRINT 'TESTES DE STRESS COMPLETOS AOS TRIGGERS (ISOLADOS)';
PRINT '=========================================================';
PRINT '';

-- ---------------------------------------------------------------------
-- TESTE 1: Associar equipamento a um utilizador que NÃO é cliente
-- ---------------------------------------------------------------------
PRINT '-> TESTE 1: Tentar associar equipamento a um utilizador que nao e cliente';
DECLARE @IdNaoCli INT;

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Tecnico T1', 'tec1_' + CAST(NEWID() AS VARCHAR(36)), '123', 'tecnico', 'ativo');
SET @IdNaoCli = SCOPE_IDENTITY();

BEGIN TRY
    -- O trigger trg_equipamento_valida_cliente deve barrar
    INSERT INTO equipamento (id_cliente, categoria, marca) 
    VALUES (@IdNaoCli, 'hardware', 'Equip T1');
    PRINT '   [FALHA] O trigger permitiu inserir equipamento para um tecnico!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 2: Abrir OS para equipamento inativo
-- ---------------------------------------------------------------------
PRINT '-> TESTE 2: Tentar abrir OS para equipamento inativo';
DECLARE @IdCli2 INT, @IdEq2 INT, @IdAtd2 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T2', 'cli2_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli2 = SCOPE_IDENTITY();

-- Equipamento inserido já como INATIVO
INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli2, 'hardware', 'Equip T2', 'inativo');
SET @IdEq2 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T2', 'atd2_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd2 = SCOPE_IDENTITY();

BEGIN TRY
    -- O trigger trg_os_valida_ativos deve barrar
    INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
    VALUES (@IdEq2, @IdAtd2, GETDATE(), 'aberta');
    PRINT '   [FALHA] O trigger permitiu abrir OS para equipamento inativo!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 3: Abrir OS para cliente inativo
-- ---------------------------------------------------------------------
PRINT '-> TESTE 3: Tentar abrir OS para equipamento de cliente inativo';
DECLARE @IdCli3 INT, @IdEq3 INT, @IdAtd3 INT;

-- Cliente inserido como INATIVO
INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T3', 'cli3_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'inativo');
SET @IdCli3 = SCOPE_IDENTITY();

-- Equipamento inserido diretamente (ignorando RNs via app para testar a base de dados)
INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli3, 'hardware', 'Equip T3', 'ativo');
SET @IdEq3 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T3', 'atd3_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd3 = SCOPE_IDENTITY();

BEGIN TRY
    -- O trigger trg_os_valida_ativos deve barrar
    INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
    VALUES (@IdEq3, @IdAtd3, GETDATE(), 'aberta');
    PRINT '   [FALHA] O trigger permitiu abrir OS para cliente inativo!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 4: Atribuir OS a um utilizador que não é técnico
-- ---------------------------------------------------------------------
PRINT '-> TESTE 4: Tentar atribuir OS a um utilizador com perfil de recepcionista';
DECLARE @IdCli4 INT, @IdEq4 INT, @IdAtd4 INT, @IdOs4 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T4', 'cli4_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli4 = SCOPE_IDENTITY();

INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli4, 'hardware', 'Equip T4', 'ativo');
SET @IdEq4 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T4', 'atd4_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd4 = SCOPE_IDENTITY();

INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
VALUES (@IdEq4, @IdAtd4, GETDATE(), 'aberta');
SET @IdOs4 = SCOPE_IDENTITY();

BEGIN TRY
    -- O trigger trg_tecnico_os_valida_perfil deve barrar (atribuindo à recepcionista)
    INSERT INTO tecnico_os (id_tecnico, id_os, data_atribuicao) 
    VALUES (@IdAtd4, @IdOs4, GETDATE());
    PRINT '   [FALHA] O trigger permitiu atribuir OS a recepcionista!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 5: Atribuir OS a um técnico inativo
-- ---------------------------------------------------------------------
PRINT '-> TESTE 5: Tentar atribuir OS a um tecnico inativo';
DECLARE @IdCli5 INT, @IdEq5 INT, @IdAtd5 INT, @IdTec5 INT, @IdOs5 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T5', 'cli5_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli5 = SCOPE_IDENTITY();

INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli5, 'hardware', 'Equip T5', 'ativo');
SET @IdEq5 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T5', 'atd5_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd5 = SCOPE_IDENTITY();

INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
VALUES (@IdEq5, @IdAtd5, GETDATE(), 'aberta');
SET @IdOs5 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Tecnico T5', 'tec5_' + CAST(NEWID() AS VARCHAR(36)), '123', 'tecnico', 'inativo');
SET @IdTec5 = SCOPE_IDENTITY();

BEGIN TRY
    -- O trigger trg_tecnico_os_valida_ativo deve barrar
    INSERT INTO tecnico_os (id_tecnico, id_os, data_atribuicao) 
    VALUES (@IdTec5, @IdOs5, GETDATE());
    PRINT '   [FALHA] O trigger permitiu atribuir OS a tecnico inativo!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 6: Inativar equipamento com OS aberta
-- ---------------------------------------------------------------------
PRINT '-> TESTE 6: Tentar inativar equipamento com OS aberta';
DECLARE @IdCli6 INT, @IdEq6 INT, @IdAtd6 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T6', 'cli6_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli6 = SCOPE_IDENTITY();

INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli6, 'hardware', 'Equip T6', 'ativo');
SET @IdEq6 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T6', 'atd6_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd6 = SCOPE_IDENTITY();

INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
VALUES (@IdEq6, @IdAtd6, GETDATE(), 'aberta');

BEGIN TRY
    -- O trigger trg_equipamento_valida_inativacao deve barrar
    UPDATE equipamento SET status = 'inativo' WHERE id_equipamento = @IdEq6;
    PRINT '   [FALHA] O trigger permitiu inativar o equipamento com OS aberta!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 7: Inativar cliente com OS aberta no seu equipamento
-- ---------------------------------------------------------------------
PRINT '-> TESTE 7: Tentar inativar cliente com OS aberta no seu equipamento';
DECLARE @IdCli7 INT, @IdEq7 INT, @IdAtd7 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T7', 'cli7_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli7 = SCOPE_IDENTITY();

INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli7, 'hardware', 'Equip T7', 'ativo');
SET @IdEq7 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T7', 'atd7_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd7 = SCOPE_IDENTITY();

INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
VALUES (@IdEq7, @IdAtd7, GETDATE(), 'aberta');

BEGIN TRY
    -- O trigger trg_usuario_valida_inativacao deve barrar
    UPDATE usuario SET status = 'inativo' WHERE id_usuario = @IdCli7;
    PRINT '   [FALHA] O trigger permitiu inativar o cliente!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 8: Inativar técnico atribuído a OS aberta
-- ---------------------------------------------------------------------
PRINT '-> TESTE 8: Tentar inativar tecnico atribuido a OS aberta';
DECLARE @IdCli8 INT, @IdEq8 INT, @IdAtd8 INT, @IdTec8 INT, @IdOs8 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T8', 'cli8_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli8 = SCOPE_IDENTITY();

INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli8, 'hardware', 'Equip T8', 'ativo');
SET @IdEq8 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T8', 'atd8_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd8 = SCOPE_IDENTITY();

INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
VALUES (@IdEq8, @IdAtd8, GETDATE(), 'aberta');
SET @IdOs8 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Tecnico T8', 'tec8_' + CAST(NEWID() AS VARCHAR(36)), '123', 'tecnico', 'ativo');
SET @IdTec8 = SCOPE_IDENTITY();

INSERT INTO tecnico_os (id_tecnico, id_os, data_atribuicao) 
VALUES (@IdTec8, @IdOs8, GETDATE());

BEGIN TRY
    -- O trigger trg_usuario_valida_inativacao deve barrar
    UPDATE usuario SET status = 'inativo' WHERE id_usuario = @IdTec8;
    PRINT '   [FALHA] O trigger permitiu inativar o tecnico!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 9: Mudar perfil de cliente para técnico com equipamentos
-- ---------------------------------------------------------------------
PRINT '-> TESTE 9: Tentar mudar perfil de cliente para tecnico mantendo equipamentos';
DECLARE @IdCli9 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T9', 'cli9_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli9 = SCOPE_IDENTITY();

INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli9, 'hardware', 'Equip T9', 'ativo');

BEGIN TRY
    -- O trigger trg_usuario_valida_troca_perfil deve barrar
    UPDATE usuario SET perfil = 'tecnico' WHERE id_usuario = @IdCli9;
    PRINT '   [FALHA] O trigger permitiu mudar o perfil de cliente para tecnico!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
GO

-- ---------------------------------------------------------------------
-- TESTE 10: Mudar perfil de técnico para cliente mantendo OS atribuídas
-- ---------------------------------------------------------------------
PRINT '-> TESTE 10: Tentar mudar perfil de tecnico para cliente mantendo OS atribuidas';
DECLARE @IdCli10 INT, @IdEq10 INT, @IdAtd10 INT, @IdTec10 INT, @IdOs10 INT;

INSERT INTO usuario (nome, email, senha, perfil, cpf, status) 
VALUES ('Cliente T10', 'cli10_' + CAST(NEWID() AS VARCHAR(36)), '123', 'cliente', LEFT(CAST(NEWID() AS VARCHAR(36)), 11), 'ativo');
SET @IdCli10 = SCOPE_IDENTITY();

INSERT INTO equipamento (id_cliente, categoria, marca, status) 
VALUES (@IdCli10, 'hardware', 'Equip T10', 'ativo');
SET @IdEq10 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Atd T10', 'atd10_' + CAST(NEWID() AS VARCHAR(36)), '123', 'recepcionista', 'ativo');
SET @IdAtd10 = SCOPE_IDENTITY();

INSERT INTO os (id_equipamento, id_atendente, data_abertura, status) 
VALUES (@IdEq10, @IdAtd10, GETDATE(), 'aberta');
SET @IdOs10 = SCOPE_IDENTITY();

INSERT INTO usuario (nome, email, senha, perfil, status) 
VALUES ('Tecnico T10', 'tec10_' + CAST(NEWID() AS VARCHAR(36)), '123', 'tecnico', 'ativo');
SET @IdTec10 = SCOPE_IDENTITY();

INSERT INTO tecnico_os (id_tecnico, id_os, data_atribuicao) 
VALUES (@IdTec10, @IdOs10, GETDATE());

BEGIN TRY
    -- O trigger trg_usuario_valida_troca_perfil deve barrar
    UPDATE usuario SET perfil = 'cliente' WHERE id_usuario = @IdTec10;
    PRINT '   [FALHA] O trigger permitiu mudar o perfil de tecnico para cliente!';
END TRY
BEGIN CATCH
    PRINT '   [SUCESSO] Trigger bloqueou: ' + ERROR_MESSAGE();
END CATCH;
PRINT '---------------------------------------------------------';
PRINT 'TODOS OS TESTES FORAM CONCLUÍDOS COM SUCESSO.';
PRINT '=========================================================';
GO
-- Use SOMENTE no computador que possui o banco Pim02 com dados.
-- Antes: faca BACKUP do banco e feche o programa ByteTech.
-- Execute conectado ao banco MASTER (sqlcmd -d master -b -i ...).
-- Esta operacao altera apenas o nome do banco: nao apaga tabelas/usuarios.
-- As sessoes abertas em Pim02 serao desconectadas; transacoes em aberto
-- serao revertidas ao entrar em SINGLE_USER.
USE master;
GO
SET NOCOUNT ON;
IF DB_ID(N'bytetech') IS NOT NULL
    THROW 51001, 'Ja existe um banco bytetech. Nao sera sobrescrito.', 1;
IF DB_ID(N'Pim02') IS NULL
    THROW 51002, 'Banco Pim02 nao encontrado. Nenhuma alteracao foi feita.', 1;

BEGIN TRY
    ALTER DATABASE [Pim02] SET SINGLE_USER WITH ROLLBACK IMMEDIATE;
    ALTER DATABASE [Pim02] MODIFY NAME = [bytetech];
    ALTER DATABASE [bytetech] SET MULTI_USER;
    PRINT 'Banco Pim02 renomeado para bytetech; tabelas e registros preservados.';
END TRY
BEGIN CATCH
    -- Se o nome ja mudou, restaura o acesso pelo novo nome.
    IF DB_ID(N'bytetech') IS NOT NULL
        ALTER DATABASE [bytetech] SET MULTI_USER;
    ELSE IF DB_ID(N'Pim02') IS NOT NULL
        ALTER DATABASE [Pim02] SET MULTI_USER;
    THROW;
END CATCH;
GO
SELECT name FROM sys.databases WHERE name IN (N'Pim02', N'bytetech');
GO

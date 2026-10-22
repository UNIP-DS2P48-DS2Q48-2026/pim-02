-- ByteTech - Dados de exemplo para o SQL Server
-- Os mesmos registros de projeto/dados/*.txt (branch main), para o banco
-- mostrar no SSMS o que o programa em C mostra no modo .txt.
-- Rodar DEPOIS do schema_sqlserver.sql (que apaga e recria as tabelas).
-- No .txt, campo vazio e dinheiro -1.00 viram NULL aqui.

USE bytetech;
GO
SET NOCOUNT ON;
SET QUOTED_IDENTIFIER ON;
GO

-- IDENTITY_INSERT deixa gravar os mesmos ids dos .txt
SET IDENTITY_INSERT usuario ON;
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (1, N'Ana', N'ana@bytetech.com', NULL, N'2654365487', N'admin', NULL, NULL, N'ativo', N'2026-09-01 08:00:00', N'2026-09-01 08:00:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (2, N'Carla', N'carla@bytetech.com', NULL, N'2654365487', N'recepcionista', NULL, NULL, N'ativo', N'2026-09-01 08:00:00', N'2026-09-01 08:00:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (3, N'Rafael', N'rafael@bytetech.com', NULL, N'2654365487', N'tecnico', NULL, N'Hardware', N'ativo', N'2026-09-01 08:00:00', N'2026-09-01 08:00:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (4, N'Joao Pereira', N'joao.pereira@email.com', N'(11) 90000-0000', N'2654365487', N'cliente', N'111.111.111-11', NULL, N'ativo', N'2026-09-01 08:00:00', N'2026-09-01 08:00:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (5, N'Patricia Gomes', N'patricia@bytetech.com', NULL, N'2654365487', N'tecnico', NULL, N'Software', N'ativo', N'2026-09-02 09:00:00', N'2026-09-02 09:00:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (6, N'Bruno Lima', N'bruno@bytetech.com', NULL, N'2654365487', N'recepcionista', NULL, NULL, N'ativo', N'2026-09-02 09:10:00', N'2026-09-02 09:10:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (7, N'Maria Souza', N'maria.souza@email.com', N'(11) 98888-7777', N'2654365487', N'cliente', N'222.222.222-22', NULL, N'ativo', N'2026-09-05 10:00:00', N'2026-09-05 10:00:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (8, N'Carlos Mendes', N'carlos.mendes@email.com', N'(11) 97777-6666', N'2654365487', N'cliente', N'333.333.333-33', NULL, N'ativo', N'2026-09-06 11:30:00', N'2026-09-06 11:30:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (9, N'Fernanda Alves', N'fernanda.alves@email.com', N'(11) 96666-5555', N'2654365487', N'cliente', N'444.444.444-44', NULL, N'inativo', N'2026-08-01 14:00:00', N'2026-09-10 16:00:00');
INSERT INTO usuario (id_usuario, nome, email, telefone, senha, perfil, cpf, especialidade, status, criado_em, atualizado_em) VALUES (10, N'Elton', N'eltonruansilva@gmail.com', NULL, N'2110877786', N'admin', NULL, NULL, N'ativo', N'2026-10-08 15:04:20', N'2026-10-08 15:05:17');
SET IDENTITY_INSERT usuario OFF;
GO

SET IDENTITY_INSERT equipamento ON;
INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, criado_em, atualizado_em) VALUES (1, 4, N'hardware', N'Notebook', N'Dell', N'Inspiron 15 / DL15-88231', N'ativo', N'2026-09-01 08:30:00', N'2026-09-01 08:30:00');
INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, criado_em, atualizado_em) VALUES (2, 4, N'software', N'Sistema Operacional', N'Microsoft', N'W11PR-7KX2M-QH8DT-4RV9C-PB3YF', N'ativo', N'2026-09-01 08:35:00', N'2026-09-01 08:35:00');
INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, criado_em, atualizado_em) VALUES (3, 7, N'hardware', N'Impressora', N'HP', N'DeskJet 2774 / CN12K4L0QZ', N'ativo', N'2026-09-05 10:15:00', N'2026-09-05 10:15:00');
INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, criado_em, atualizado_em) VALUES (4, 7, N'hardware', N'Desktop', N'Positivo', N'Master D610 / PS-55120', N'ativo', N'2026-09-05 10:20:00', N'2026-09-05 10:20:00');
INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, criado_em, atualizado_em) VALUES (5, 8, N'hardware', N'Smartphone', N'Samsung', N'Galaxy A54 / R58T90XYZ', N'ativo', N'2026-09-06 11:40:00', N'2026-09-06 11:40:00');
INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, criado_em, atualizado_em) VALUES (6, 8, N'software', N'Antivirus', N'Kaspersky', N'KP-4H7Q2-ZM8W1-T6RX3', N'ativo', N'2026-09-06 11:45:00', N'2026-09-06 11:45:00');
INSERT INTO equipamento (id_equipamento, id_cliente, categoria, tipo, marca, modelo_numero_serie, status, criado_em, atualizado_em) VALUES (7, 9, N'hardware', N'Notebook', N'Lenovo', N'IdeaPad 3 / LN-PF3K9W2A', N'inativo', N'2026-08-01 14:10:00', N'2026-09-10 16:00:00');
SET IDENTITY_INSERT equipamento OFF;
GO

-- A OS 8 e' historica: foi aberta quando o equipamento 7 e a cliente 9 ainda
-- estavam ativos. O trigger de RN14 so vale para OS nova, entao fica desligado
-- durante a carga e e' religado logo em seguida.
DISABLE TRIGGER trg_os_valida_ativos ON os;
SET IDENTITY_INSERT os ON;
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (1, 1, 2, N'2026-09-15', N'Notebook nao liga', N'Fonte queimada, troca da fonte', 250.00, N'finalizada', N'aprovado', N'2026-09-22', N'aprovado', 250.00, N'Atendimento rapido, tudo funcionando', N'sim', N'2026-09-15 09:00:00', N'2026-09-23 10:00:00');
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (2, 3, 2, N'2026-09-28', N'Impressora nao puxa o papel', N'Rolete de tracao gasto, troca do rolete', 120.00, N'em_andamento', N'aprovado', NULL, NULL, NULL, NULL, NULL, N'2026-09-28 14:00:00', N'2026-09-30 11:00:00');
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (3, 5, 1, N'2026-10-01', N'Tela trincada apos queda', N'Troca do display completo', 680.00, N'em_andamento', N'recusado', NULL, NULL, NULL, NULL, NULL, N'2026-10-01 15:30:00', N'2026-10-03 09:00:00');
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (4, 2, 2, N'2026-10-03', N'Windows lento e travando', NULL, NULL, N'em_andamento', NULL, NULL, NULL, NULL, NULL, NULL, N'2026-10-03 10:00:00', N'2026-10-04 08:30:00');
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (5, 4, 6, N'2026-10-06', N'Computador desliga sozinho', NULL, NULL, N'aberta', NULL, NULL, NULL, NULL, NULL, NULL, N'2026-10-06 16:20:00', N'2026-10-06 16:20:00');
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (6, 6, 2, N'2026-09-20', N'Licenca do antivirus expirada', NULL, NULL, N'cancelada', NULL, NULL, NULL, NULL, NULL, NULL, N'2026-09-20 13:00:00', N'2026-09-21 09:00:00');
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (7, 1, 2, N'2026-10-02', N'Teclas do teclado falhando', N'Oxidacao no teclado, troca do teclado', 180.00, N'em_andamento', N'aprovado', NULL, N'aprovado', NULL, NULL, NULL, N'2026-10-02 11:00:00', N'2026-10-07 17:00:00');
INSERT INTO os (id_os, id_equipamento, id_atendente, data_abertura, problema_relatado, diagnostico, orcamento, status, resultado_resposta_cliente, data_entrega, resultado_teste, valor_pago, feedback_cliente, satisfeito, criado_em, atualizado_em) VALUES (8, 7, 2, N'2026-08-10', N'Notebook superaquecendo', N'Limpeza interna e troca da pasta termica', 150.00, N'finalizada', N'aprovado', N'2026-08-14', N'aprovado', 150.00, N'Resolveu o problema', N'sim', N'2026-08-10 10:00:00', N'2026-08-15 09:00:00');
SET IDENTITY_INSERT os OFF;
ENABLE TRIGGER trg_os_valida_ativos ON os;
GO

SET IDENTITY_INSERT tecnico_os ON;
INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, criado_em, atualizado_em) VALUES (1, 3, 1, N'2026-09-16', N'Fonte sem tensao de saida, placa ok', N'2026-09-16 08:00:00', N'2026-09-18 15:00:00');
INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, criado_em, atualizado_em) VALUES (2, 3, 2, N'2026-09-29', N'Rolete de tracao desgastado, peca encomendada', N'2026-09-29 09:00:00', N'2026-09-30 11:00:00');
INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, criado_em, atualizado_em) VALUES (3, 3, 3, N'2026-10-02', N'Display com trinca no canto superior, touch falhando', N'2026-10-02 10:00:00', N'2026-10-02 16:00:00');
INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, criado_em, atualizado_em) VALUES (4, 5, 4, N'2026-10-04', N'Muitos programas na inicializacao, disco 95% cheio', N'2026-10-04 08:30:00', N'2026-10-04 08:30:00');
INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, criado_em, atualizado_em) VALUES (5, 3, 7, N'2026-10-03', N'Oxidacao nas trilhas da membrana', N'2026-10-03 09:00:00', N'2026-10-07 17:00:00');
INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, criado_em, atualizado_em) VALUES (6, 3, 8, N'2026-08-11', N'Cooler obstruido por poeira', N'2026-08-11 08:00:00', N'2026-08-13 14:00:00');
INSERT INTO tecnico_os (id_tecnico_os, id_tecnico, id_os, data_atribuicao, observacoes_tecnicas, criado_em, atualizado_em) VALUES (7, 5, 7, N'2026-10-03', N'Teste de teclado com software de diagnostico', N'2026-10-03 14:00:00', N'2026-10-07 16:00:00');
SET IDENTITY_INSERT tecnico_os OFF;
GO

SELECT 'usuario' AS tabela, COUNT(*) AS linhas FROM usuario
UNION ALL SELECT 'equipamento', COUNT(*) FROM equipamento
UNION ALL SELECT 'os', COUNT(*) FROM os
UNION ALL SELECT 'tecnico_os', COUNT(*) FROM tecnico_os;
GO

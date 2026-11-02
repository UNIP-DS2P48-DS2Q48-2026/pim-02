#!/usr/bin/env python3
"""Atualiza a configuracao do C sem apagar cadastros no SQL Server.
Rode na pasta `projeto`: python3 Kit_ByteTech_Git/ferramentas/atualizar_conexao.py
"""
from pathlib import Path
from datetime import datetime
import shutil
import subprocess
import sys

pasta = Path.cwd()
arquivo = pasta / 'banco.c'
if not arquivo.is_file() or not (pasta / 'main.c').is_file():
    sys.exit('ERRO: execute na pasta projeto (main.c e banco.c).')

original = arquivo.read_text(encoding='utf-8')
novo = original
if 'DATABASE=bytetech;' in original and 'BYTETECH_DB_SERVER' in original:
    print('Conexao ja aponta para bytetech. Nenhuma edicao necessaria.')
    sys.exit(0)

alvo = '"SERVER=localhost;DATABASE=Pim02;UID=sa;PWD=%s;"'
if original.count(alvo) != 1:
    sys.exit('ERRO: trecho da conexao nao encontrado exatamente uma vez; nao vou alterar o arquivo.')
if original.count('const char *senha = getenv("PIM02_DB_PASSWORD");') != 1:
    sys.exit('ERRO: declaracao de senha diferente do esperado; arquivo intacto.')
if original.count('sizeof(conexao),') != 1 or original.count('sizeof(conexao),\n') != 1:
    sys.exit('ERRO: formato da string de conexao inesperado; arquivo intacto.')
if original.count('"Encrypt=yes;TrustServerCertificate=yes;", senha);') != 1:
    sys.exit('ERRO: argumentos da conexao inesperados; arquivo intacto.')

novo = novo.replace('const char *senha = getenv("PIM02_DB_PASSWORD");', '''const char *senha = getenv("BYTETECH_DB_PASSWORD");
    if (senha == NULL || senha[0] == '\\0')
        senha = getenv("PIM02_DB_PASSWORD");  // compatibilidade com a configuracao antiga
    const char *servidor = getenv("BYTETECH_DB_SERVER");
    if (servidor == NULL || servidor[0] == '\\0')
        servidor = "localhost";''', 1)
novo = novo.replace(alvo, '"SERVER=%s;DATABASE=bytetech;UID=sa;PWD=%s;"', 1)
novo = novo.replace('"Encrypt=yes;TrustServerCertificate=yes;", senha);',
                    '"Encrypt=yes;TrustServerCertificate=yes;", servidor, senha);', 1)
novo = novo.replace('Defina a variavel PIM02_DB_PASSWORD.', 'Defina a variavel BYTETECH_DB_PASSWORD.', 1)
novo = novo.replace('conexao com o banco Pim02', 'conexao com o banco bytetech')
novo = novo.replace('conexao com o banco Pim02', 'conexao com o banco bytetech')

backup = pasta / 'backups_sql' / ('conexao_antes_bytetech_' + datetime.now().strftime('%Y%m%d_%H%M%S'))
backup.mkdir(parents=True, exist_ok=True)
shutil.copy2(arquivo, backup / 'banco.c')
print('Backup:', backup)

# Compila para nome temporario; se falhar, restaura a versao antiga.
temporario = pasta / 'bytetech_validacao_temp'
try:
    arquivo.write_text(novo, encoding='utf-8')
    proc = subprocess.run(['gcc', '-std=c11', '-Wall', '-Wextra',
                           'main.c', 'banco.c', '-o', temporario.name,
                           '-lodbc'], cwd=pasta)
    if proc.returncode != 0:
        raise RuntimeError('Compilacao falhou')
    temporario.replace(pasta / 'bytetech')
except Exception as exc:
    shutil.copy2(backup / 'banco.c', arquivo)
    temporario.unlink(missing_ok=True)
    sys.exit(f'ERRO: {exc}. banco.c original restaurado.')

print('OK: banco.c configurado para o banco bytetech. Programa recompilado.')
print('A senha agora pode ser informada em BYTETECH_DB_PASSWORD (PIM02_DB_PASSWORD continua aceito).')
print('ATENCAO: agora execute a RENOMEACAO no SQL Server; so depois tente abrir o ByteTech.')

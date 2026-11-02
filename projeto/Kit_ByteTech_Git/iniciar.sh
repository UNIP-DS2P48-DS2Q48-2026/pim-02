#!/usr/bin/env bash
# Execute em projeto: bash Kit_ByteTech_Git/iniciar.sh
# O banco em SQL Server continua separado do executavel C.
set -euo pipefail
KIT_DIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
PROJETO=$(dirname "$KIT_DIR")
cd "$PROJETO"

for prog in gcc; do
    command -v "$prog" >/dev/null || { echo "Falta $prog. Instale as dependencias primeiro." >&2; exit 1; }
done
if [[ -z "${BYTETECH_DB_PASSWORD:-}" ]]; then
    read -rsp 'Senha do SQL Server (usuario sa): ' BYTETECH_DB_PASSWORD
    echo
fi
if [[ -z "$BYTETECH_DB_PASSWORD" ]]; then echo 'Senha vazia.' >&2; exit 1; fi
export BYTETECH_DB_PASSWORD
# Variavel antiga tambem e entendida pelo codigo ja existente, se ainda nao atualizado.
export PIM02_DB_PASSWORD="$BYTETECH_DB_PASSWORD"
export SQLCMDPASSWORD="$BYTETECH_DB_PASSWORD"
SERVIDOR=${BYTETECH_DB_SERVER:-localhost}
if command -v sqlcmd >/dev/null; then
    SQLCMD=$(command -v sqlcmd)
elif [[ -x /opt/mssql-tools18/bin/sqlcmd ]]; then
    SQLCMD=/opt/mssql-tools18/bin/sqlcmd
else
    echo 'Falta sqlcmd. Execute instalar_dependencias_ubuntu.sh.' >&2
    exit 1
fi

# Usa SQL Server existente. Se nao houver conexao, oferece iniciar um container.
if ! "$SQLCMD" -S "$SERVIDOR" -U sa -C -d master -l 3 -b -Q 'SELECT 1' >/dev/null 2>&1; then
    if command -v docker >/dev/null && docker compose version >/dev/null 2>&1; then
        echo 'SQL Server ainda nao responde. Tentando iniciar o container Docker...'
        docker compose -f "$KIT_DIR/compose.yaml" up -d sqlserver
        # Espera o servidor estar pronto para logins; interrompe com erro se nao iniciar.
        CONECTOU=0
        for _ in $(seq 1 30); do
            if "$SQLCMD" -S localhost -U sa -C -d master -l 3 -b -Q 'SELECT 1' >/dev/null 2>&1; then
                CONECTOU=1; break
            fi
            sleep 2
        done
        if [[ "$CONECTOU" != 1 ]]; then
            echo 'SQL Server nao respondeu. Verifique: docker compose -f Kit_ByteTech_Git/compose.yaml logs sqlserver' >&2
            exit 1
        fi
        SERVIDOR=localhost
    else
        echo 'SQL Server indisponivel. Instale/inicie-o ou configure Docker Desktop com integracao WSL.' >&2
        exit 1
    fi
fi

# Inspeciona o banco; nao cria tabelas novas sobre uma instalacao existente.
EXISTE=$("$SQLCMD" -S "$SERVIDOR" -U sa -C -d master -b -h -1 -W -Q \
    "SET NOCOUNT ON; SELECT CASE WHEN DB_ID(N'bytetech') IS NULL THEN 0 ELSE 1 END" | tr -d '\r[:space:]')
if [[ "$EXISTE" == 0 ]]; then
    ANTIGO=$("$SQLCMD" -S "$SERVIDOR" -U sa -C -d master -b -h -1 -W -Q \
        "SET NOCOUNT ON; SELECT CASE WHEN DB_ID(N'Pim02') IS NULL THEN 0 ELSE 1 END" | tr -d '\r[:space:]')
    if [[ "$ANTIGO" == 1 ]]; then
        echo 'ATENCAO: encontrei o banco Pim02 antigo neste servidor.' >&2
        echo 'Nao vou criar uma base vazia. Faca o backup e execute a renomeacao primeiro.' >&2
        exit 1
    fi
    echo 'Criando banco e tabelas do ByteTech pela primeira vez...'
    "$SQLCMD" -S "$SERVIDOR" -U sa -C -d master -b -i "$KIT_DIR/database/criar_bytetech.sql"
elif [[ "$EXISTE" == 1 ]]; then
    echo 'Banco bytetech encontrado. Preservando todos os dados.'
else
    echo "Nao consegui verificar o banco (resposta: $EXISTE)" >&2
    exit 1
fi

# Protege contra banco parcial ou de outra versao.
"$SQLCMD" -S "$SERVIDOR" -U sa -C -d bytetech -b -Q \
 "IF OBJECT_ID(N'dbo.usuario',N'U') IS NULL OR OBJECT_ID(N'dbo.equipamento',N'U') IS NULL OR OBJECT_ID(N'dbo.os',N'U') IS NULL OR OBJECT_ID(N'dbo.tecnico_os',N'U') IS NULL OR COL_LENGTH(N'dbo.os',N'atualizado_em') IS NULL THROW 51030, 'Schema ByteTech incompleto: revise o banco antes de iniciar.', 1;"

# Executavel Linux do WSL: compila no computador do usuario.
echo 'Compilando ByteTech...'
gcc -std=c11 -Wall -Wextra main.c banco.c -o bytetech -lodbc -lcrypto
echo 'Abrindo ByteTech...'
export BYTETECH_DB_SERVER="$SERVIDOR"
./bytetech

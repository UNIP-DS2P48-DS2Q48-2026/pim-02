# Kit de instalacao e compartilhamento — ByteTech

Este kit vai dentro da pasta **`projeto/Kit_ByteTech_Git`** do repositorio Git.
Ele **nao substitui** os seus arquivos `.c` existentes. Nunca envie senhas,
backups `.bak` ou o executavel local ao GitHub.

## A) Computador do autor: renomear o banco existente, sem perder dados

1. **Salve todos os arquivos no VS Code**: `Ctrl+K`, depois `S`.
2. Feche o programa `./bytetech` e qualquer conexao ao banco `Pim02`.
3. Faca **backup externo ao Git** de `Pim02`. Por exemplo, se o servidor tiver
   a pasta `/var/opt/mssql/backup` disponivel:

   ```bash
   read -rsp 'Senha sa: ' BYTETECH_DB_PASSWORD; echo; export BYTETECH_DB_PASSWORD
   SQLCMDPASSWORD="$BYTETECH_DB_PASSWORD" /opt/mssql-tools18/bin/sqlcmd \
     -S localhost -U sa -C -d master -b \
     -Q "BACKUP DATABASE [Pim02] TO DISK=N'/var/opt/mssql/backup/Pim02_antes_renomear.bak' WITH NOINIT, CHECKSUM"
   ```

   Se o backup falhar, nao prossiga ate resolver o caminho/permissoes.
   Guarde uma copia do `.bak` fora do repositorio.
4. Na pasta `projeto` (onde esta `main.c`), altere a conexao do codigo:

   ```bash
   python3 Kit_ByteTech_Git/ferramentas/atualizar_conexao.py
   ```

   O script cria backup de `banco.c` em `backups_sql/`, verifica compilacao e
   atualiza o nome do banco para `bytetech`. Mantem a variavel antiga
   `PIM02_DB_PASSWORD` como alternativa e aceita `BYTETECH_DB_PASSWORD`.
5. **So depois**, altere o nome do banco SQL Server:

   ```bash
   SQLCMDPASSWORD="$BYTETECH_DB_PASSWORD" /opt/mssql-tools18/bin/sqlcmd \
     -S localhost -U sa -C -d master -b \
     -i Kit_ByteTech_Git/database/renomear_pim02_para_bytetech.sql
   ```

   O script **recusa** a operacao se ja houver um banco chamado `bytetech`.
   No processo, conexoes ao `Pim02` sao encerradas imediatamente.
   Tabelas e usuarios nao sao apagados, mas transacoes abertas sao revertidas.
6. Teste:

   ```bash
   BYTETECH_DB_PASSWORD="$BYTETECH_DB_PASSWORD" ./bytetech
   ```

   Confirme que aparecem seus usuarios e registros existentes. Nao execute
   `criar_bytetech.sql` sobre a sua base renomeada.

## B) Integrante do grupo: instalar em Windows 10/11 + Ubuntu WSL

Pre-requisitos **que o ZIP sozinho nao instala**:

- WSL2 com Ubuntu (`wsl --install -d Ubuntu`, PowerShell como administrador,
  se nao estiver instalado; pode exigir reinicializacao).
- **Um SQL Server local ja funcionando** OU Docker Desktop com a integracao WSL
  ativada (Docker Desktop precisa estar aberto). No Docker, reserve memoria
  suficiente para SQL Server (idealmente 4 GB ou mais livres).
- Internet e permissao `sudo` no Ubuntu para instalar ODBC e compilador.

1. Clone/baixe o repositorio inteiro. Abra o terminal Ubuntu na pasta `projeto`.
2. Instale as dependencias uma unica vez:

   ```bash
   bash Kit_ByteTech_Git/ferramentas/instalar_dependencias_ubuntu.sh
   ```

   O instalador configura o driver ODBC 18, `sqlcmd`, GCC e OpenSSL,
   seguindo o repositorio oficial da Microsoft.
3. Execute o atalho **a partir da pasta `projeto`**:

   ```bash
   bash Kit_ByteTech_Git/iniciar.sh
   ```

   Ele pede a senha local do `sa` sem exibi-la; verifica se o SQL Server responde,
   tenta subir um container com Docker se nao houver servidor em `localhost`,
   cria a base `bytetech` **somente se ela nao existir**, compila o C e
   executa o ByteTech. Ao abrir em uma base vazia, cadastre o primeiro admin.

4. Nas proximas vezes, so repita:

   ```bash
   bash Kit_ByteTech_Git/iniciar.sh
   ```

**IMPORTANTE:** use a mesma senha ao reiniciar o container Docker, pois a senha
foi configurada no SQL Server na primeira inicializacao. A senha **nao e gravada**
nesse kit. O Docker mantem os dados em volume persistente.

Se a senha contiver `;` ou `}` e houver erro no login ODBC, use outra senha de
ambiente de teste compativel com a string de conexao atual. Use senha forte
(com letras maiusculas, minusculas, numeros e simbolos).

**Se ja houver outro SQL Server na porta 1433**, nao tente subir um segundo
container na mesma porta. O atalho primeiro tenta conectar ao servidor existente.
Se houver credenciais diferentes das digitadas, a conexao falhara; nao altera
nem apaga o servidor existente.

## Sobre executavel para Windows

`./bytetech` compilado no Ubuntu e um executavel **Linux ELF**: nao roda com
duplo clique como um `.exe` nativo do Windows. O `iniciar.sh` e a forma mais
simples de rodar o mesmo codigo no WSL, sem mudar os modulos da aplicacao.
Para produzir um `.exe` Windows real, precisariamos portar `termios.h` e a
entrada de senhas, alem de compilar e distribuir as dependencias para Windows.
Um `.exe` tambem **nao embute o SQL Server**.

## Estrutura e seguranca

- `database/criar_bytetech.sql`: cria a base e quatro tabelas do sistema sem
  fazer `DROP`. Diferente do schema antigo em `docs/Modelagem_DB`, que tem
  `DROP TABLE` e **NAO deve ser usado** sobre bancos com registros.
- `database/renomear_pim02_para_bytetech.sql`: altera o nome do banco do autor.
- `ferramentas/atualizar_conexao.py`: troca apenas a configuracao do `banco.c`,
  guarda backup e recompila.
- `ferramentas/instalar_dependencias_ubuntu.sh`: dependencias Ubuntu.
- `compose.yaml`: opcional, SQL Server Developer em volume persistente.
- `iniciar.sh`: configura, compila e abre.
- `GITIGNORE_ADICIONAR.txt`: sugestoes para `.gitignore` existente.

O conjunto foi conferido por verificacao estatica e compilacao local de codigo
compatível; a abertura efetiva no computador de cada colega depende do SQL
Server, Docker/WSL e dependencias instalados. **Use `sa` somente no ambiente
local de demonstracao; para uso real, crie uma conta com permissoes limitadas.**

#define CONEXAO_SQLSERVER "DRIVER={ODBC Driver 17 for SQL Server};" \
                          "SERVER=localhost\\SQLEXPRESS;"            \
                          "DATABASE=bytetech;Trusted_Connection=yes;"

SQLHDBC  bdConexao = SQL_NULL_HDBC;
SQLHSTMT bdComando = SQL_NULL_HSTMT;

void bdMostrarErro(SQLSMALLINT tipo, SQLHANDLE handle) {
}

int bdConectar(void) {
}

int bdExecutar(const char *sql) {
}

int bdConsultar(const char *sql) {
}

int bdProximaLinha(void) {
}

void bdLerTexto(int coluna, char *destino, int tamanho) {
}

void sqlTexto(char *destino, const char *texto) {
}

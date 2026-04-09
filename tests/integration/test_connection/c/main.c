#include "clockwork/conn.h"
#include "clockwork/errors.h"
#include <stdio.h>

int main() {
  int8_t result = ERR_CONN_NO_ERROR;
  tcp_conn_t conn;

  result = connect_to_server("127.0.0.1", 9090, &conn);
  if (result != ERR_CONN_NO_ERROR) {
    fprintf(stderr, "Failed to connect to server: %d\n", result);
    return 1;
  }
  return 0;
}

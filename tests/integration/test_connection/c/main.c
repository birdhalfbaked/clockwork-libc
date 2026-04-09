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
  // send a connect message
  message_t message = {
      .type = MESSAGE_CONNECT,
      .length = 8,
      .entity_id = 1,
  };
  result = send_message(&conn, &message);
  if (result != ERR_CONN_NO_ERROR) {
    fprintf(stderr, "Failed to send message: %d\n", result);
    return 1;
  }
  return 0;
}

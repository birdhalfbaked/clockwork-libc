#include "clockwork/conn.h"
#include "clockwork/debug.h"
#include "clockwork/errors.h"
#include <winsock2.h>
#include <ws2tcpip.h>

#define WIN32_LEAN_AND_MEAN
static bool winsock_initialized = false;

int8_t assert_winsock_initialized() {
  int result = ERR_CONN_NO_ERROR;
  if (!winsock_initialized) {
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
      winsock_initialized = false;
      // print out the error for now
      DEBUG_PRINT("Failed to initialize Winsock: %d\n", result);
      result = ERR_CONN_CONNECTION_FAILED;
    }
    winsock_initialized = true;
  }
  return ERR_CONN_NO_ERROR;
}

int8_t connect_to_server(const char *address, uint16_t port, tcp_conn_t *conn) {
  int8_t result = assert_winsock_initialized();
  DEBUG_PRINT("Winsock initialized: %d\n", result);
  SOCKADDR_IN addr;
  SOCKET conn_socket;
  DEBUG_PRINT("Creating socket\n");
  conn_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (conn_socket == INVALID_SOCKET) {
    DEBUG_PRINT("Failed to create socket: %d\n", WSAGetLastError());
    result = ERR_CONN_CONNECTION_FAILED;
  }

  DEBUG_PRINT("Converting address: %s\n", address);
  if (result == ERR_CONN_NO_ERROR) {
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    int addr_result = inet_pton(AF_INET, address, &addr.sin_addr);
    if (addr_result == 0) {
      DEBUG_PRINT("Failed to convert address: %s", address);
      WSACleanup();
      result = ERR_CONN_CONNECTION_FAILED;
    }
  }
  DEBUG_PRINT("Address converted: %lu\n", addr.sin_addr.s_addr);

  DEBUG_PRINT("Connecting to server\n");
  if (result == ERR_CONN_NO_ERROR) {
    int connect_result = connect(conn_socket, (SOCKADDR *)&addr, sizeof(addr));
    DEBUG_PRINT("Connect result: %d\n", connect_result);
    conn->socket = conn_socket;
    if (connect_result == SOCKET_ERROR) {
      DEBUG_PRINT("Failed to connect to server: %d\n", WSAGetLastError());
      WSACleanup();
      result = ERR_CONN_CONNECTION_FAILED;
    }
  }

  return result;
}

int8_t disconnect_from_server(tcp_conn_t *conn) { return ERR_CONN_NO_ERROR; }
int8_t receive_message(tcp_conn_t *conn, message_t *out_message) {
  return ERR_CONN_NO_ERROR;
}
int8_t send_message(tcp_conn_t *conn, const message_t *message) {
  return ERR_CONN_NO_ERROR;
}
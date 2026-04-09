#include "clockwork/conn.h"
#include "clockwork/debug.h"
#include "clockwork/errors.h"

#include <winsock2.h>
#include <ws2tcpip.h>

#define WIN32_LEAN_AND_MEAN

static bool winsock_initialized = false;

int8_t assert_winsock_initialized() {
  int8_t result = ERR_CONN_NO_ERROR;
  if (!winsock_initialized) {
    WSADATA wsa_data;
    int startup_result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (startup_result != 0) {
      // print out the error for now
      DEBUG_PRINT("Failed to initialize Winsock: %d\n", startup_result);
      result = ERR_CONN_CONNECTION_FAILED;
    } else {
      winsock_initialized = true;
    }
  }
  return result;
}

int8_t connect_to_server(const char *address, uint16_t port, tcp_conn_t *conn) {
  int8_t result = assert_winsock_initialized();
  conn->socket = CLOCKWORK_INVALID_SOCKET;
  DEBUG_PRINT("Winsock initialized: %d\n", result);
  SOCKADDR_IN addr = {0};
  SOCKET conn_socket = INVALID_SOCKET;

  DEBUG_PRINT("Converting address: %s\n", address);
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  int addr_result = inet_pton(AF_INET, address, &addr.sin_addr);
  if (addr_result <= 0) {
    DEBUG_PRINT("Failed to convert address: %s with result: %d", address,
                addr_result);
    result = ERR_CONN_CONNECTION_FAILED;
  }

  if (result == ERR_CONN_NO_ERROR) {
    DEBUG_PRINT("Creating socket\n");
    conn_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (conn_socket == INVALID_SOCKET) {
      DEBUG_PRINT("Failed to create socket: %d\n", WSAGetLastError());
      result = ERR_CONN_CONNECTION_FAILED;
    }
  }

  DEBUG_PRINT("Connecting to server\n");
  if (result == ERR_CONN_NO_ERROR) {
    int connect_result = connect(conn_socket, (SOCKADDR *)&addr, sizeof(addr));
    DEBUG_PRINT("Connect result: %d\n", connect_result);
    if (connect_result == 0) {
      conn->socket = (uintptr_t)conn_socket;
    } else {
      closesocket(conn_socket);
      conn->socket = CLOCKWORK_INVALID_SOCKET;
      DEBUG_PRINT("Failed to connect to server: %d with error: %d\n",
                  connect_result, WSAGetLastError());
      result = ERR_CONN_CONNECTION_FAILED;
    }
  }

  return result;
}

int8_t disconnect_from_server(tcp_conn_t *conn) {
  if (conn->socket != CLOCKWORK_INVALID_SOCKET) {
    int close_result = closesocket((SOCKET)conn->socket);
    if (close_result != 0) {
      DEBUG_PRINT("Failed to close socket: %d\n", close_result);
      return ERR_CONN_DISCONNECTION_FAILED;
    }
    conn->socket = CLOCKWORK_INVALID_SOCKET;
  }
  return ERR_CONN_NO_ERROR;
}
int8_t receive_message(tcp_conn_t *conn, message_t *out_message) {
  if (conn->socket == CLOCKWORK_INVALID_SOCKET) {
    return ERR_CONN_INVALID_STATE;
  }
  int8_t result = ERR_CONN_NO_ERROR;
  uint8_t out_buffer[256];
  int recv_result =
      recv(conn->socket, (char *)out_buffer, sizeof(out_buffer), MSG_WAITALL);
  if (recv_result <= 0) {
    DEBUG_PRINT("Failed to receive message: %d with error: %d\n", recv_result,
                WSAGetLastError());
    result = ERR_CONN_INVALID_STATE;
  }
  int8_t bytes_to_message_result =
      bytes_to_message(out_buffer, recv_result, out_message);
  if (bytes_to_message_result != recv_result) {
    DEBUG_PRINT("Failed to convert message: %d != %d\n",
                bytes_to_message_result, recv_result);
    result = ERR_CONN_INVALID_STATE;
  }
  return result;
}

int8_t send_message(tcp_conn_t *conn, const message_t *message) {
  if (conn->socket == CLOCKWORK_INVALID_SOCKET) {
    return ERR_CONN_INVALID_STATE;
  }

  uint8_t out_buffer[256];
  int8_t message_to_bytes_result =
      message_to_bytes(message, out_buffer, sizeof(out_buffer));
  if (message_to_bytes_result < 0) {
    DEBUG_PRINT("Failed to convert message: %d != %d\n",
                message_to_bytes_result, sizeof(message_t));
    return ERR_CONN_INVALID_STATE;
  }

  int8_t result = ERR_CONN_NO_ERROR;
  int send_result =
      send(conn->socket, (const char *)out_buffer, message_to_bytes_result, 0);
  if (send_result == -1) {
    DEBUG_PRINT("Failed to send message: %d with error: %d\n", send_result,
                WSAGetLastError());
    result = ERR_CONN_INVALID_STATE;
  }
  return result;
}
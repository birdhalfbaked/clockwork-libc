/**
 * Clockwork
 */

#ifndef CLOCKWORK_CONN_H
#define CLOCKWORK_CONN_H

#include "messages.h"
#include <stdbool.h>
#include <stdint.h>

/// @brief Invalid socket value
/// @details This is used to represent an invalid socket. It is used to
/// represent an invalid socket in the socket field of the tcp_conn_t struct.
#define CLOCKWORK_INVALID_SOCKET ((uintptr_t)-1)

/// @brief TCP connection structure
/// @details This is used to represent a TCP connection. It is used to
/// represent a TCP connection in the tcp_conn_t struct.
typedef struct {
  uintptr_t socket;
} tcp_conn_t;

/// @brief Connect to a server
/// @param address The address of the server to connect to
/// @param port The port of the server to connect to
/// @param conn The TCP connection structure to connect to
/// @return The result of the connection
int8_t connect_to_server(const char *address, uint16_t port, tcp_conn_t *conn);

/// @brief Disconnect from a server
/// @param conn The TCP connection structure to disconnect from
/// @return The result of the disconnection
int8_t disconnect_from_server(tcp_conn_t *conn);

/// @brief Send a message to a server
/// @param conn The TCP connection structure to send the message to
/// @param message The message to send
/// @return The result of the send
int8_t send_message(tcp_conn_t *conn, const message_t *message);

/// @brief Receive a message from a server
/// @param conn The TCP connection structure to receive the message from
/// @param out_message The message to receive
/// @return The result of the receive
int8_t receive_message(tcp_conn_t *conn, message_t *out_message);

#endif
/**
 * Clockwork
 */

#ifndef CLOCKWORK_CONN_H
#define CLOCKWORK_CONN_H

#include "messages.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  uintptr_t socket;
} tcp_conn_t;

int8_t connect_to_server(const char *address, uint16_t port, tcp_conn_t *conn);
int8_t disconnect_from_server(tcp_conn_t *conn);
int8_t send_message(tcp_conn_t *conn, const message_t *message);
int8_t receive_message(tcp_conn_t *conn, message_t *out_message);

#endif
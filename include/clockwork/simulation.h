#ifndef CLOCKWORK_TYPES_H
#define CLOCKWORK_TYPES_H

#include "common_types.h"
#include "conn.h"
#include <stdint.h>

typedef struct {
  entity_id_t entity_id;
  const char *name;
  tick_t current_tick;
} simulation_info_t;

typedef void (*lifecycle_callback_t)(const simulation_info_t *info);

typedef struct {
  const simulation_info_t *info;
  lifecycle_callback_t on_setup;
  lifecycle_callback_t on_pre_tick;
  lifecycle_callback_t on_tick;
  lifecycle_callback_t on_post_tick;
  lifecycle_callback_t on_teardown;
  void *user_data;
  tcp_conn_t *conn;
} simulation_t;

/// @brief Block and wait for the next tick
/// @param simulation Initialized structure containing simulation data
void wait_for_tick(const simulation_t *simulation);

/// @brief Set the user data for the simulation
/// @param simulation Initialized structure containing simulation data
/// @param user_data The user data to set
void set_user_data(simulation_t *simulation, void *user_data);

#endif
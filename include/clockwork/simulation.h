#ifndef CLOCKWORK_SIMULATION_H
#define CLOCKWORK_SIMULATION_H

#include "common_types.h"
#include "conn.h"
#include <stdint.h>

/// @brief Information about the simulation
/// @details This is used to represent the information about the simulation. It
/// is used to represent the information about the simulation in the
/// simulation_info_t struct.
typedef struct {
  /// @privatesection
  tick_t _current_tick;
  /// @publicsection
  /// @brief The entity ID of the simulation component.
  /// @note This must match the entity ID of the simulation component in the
  /// Clockwork server which is predefined by you as a user.
  entity_id_t entity_id;
  /// @brief The name of the simulation component
  const char *name;
  /// @brief The number of pins in the simulation
  uint8_t num_pins;
  pin_level_t pins[256];
} simulation_info_t;

typedef struct _simulation_t simulation_t;

typedef void (*lifecycle_callback_t)(simulation_t *simulation, void *user_data);

struct _simulation_t {
  /// @privatesection
  tcp_conn_t *_conn;
  bool _is_running;
  /// @publicsection
  /// @brief The information about the simulation
  simulation_info_t *info;
  /// @brief The callback to be called when the simulation is setup
  lifecycle_callback_t _on_setup;
  /// @brief The callback to be called when the simulation is tick
  lifecycle_callback_t _on_tick;
  /// @brief The callback to be called when the simulation is teared down
  lifecycle_callback_t _on_teardown;
  /// @brief The user data for the simulation
  /// @details This is used to store user data for the simulation. It will be
  /// passed alongside the simulation information to the callbacks as the
  /// user_data parameter.
  void *user_data;
};

/// @brief Initialize the simulation
/// @param simulation Initialized structure containing simulation info
void initialize_simulation(simulation_t *simulation);

/// @brief run the simulation until termination
/// @param simulation Initialized structure containing simulation data
void run(simulation_t *simulation);

/// @brief stop the simulation
/// @param simulation Initialized structure containing simulation data
void stop(simulation_t *simulation);

/// @brief Set the user data for the simulation
/// @param simulation Initialized structure containing simulation data
/// @param user_data The user data to set
void set_user_data(simulation_t *simulation, void *user_data);

#endif
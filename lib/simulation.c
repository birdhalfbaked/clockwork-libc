#include "clockwork/simulation.h"
#include "clockwork/conn.h"
#include "clockwork/debug.h"
#include "clockwork/errors.h"
#include "clockwork/messages.h"

void initialize_simulation(simulation_t *simulation) {
  if (simulation != NULL) {
    if (simulation->info != NULL) {
      simulation->info->_current_tick = 0;
    }

    simulation->_conn = NULL;
    simulation->_on_setup = NULL;
    simulation->_on_tick = NULL;
    simulation->_on_teardown = NULL;
    simulation->user_data = NULL;
  }
}

bool _wait_for_tick(const simulation_t *simulation) {
  bool ok = true;
  message_t message = {0};
  if (simulation == NULL || simulation->_conn == NULL) {
    ok = false;
  } else {
    int8_t result = receive_message(simulation->_conn, &message);
    if (result != ERR_CONN_NO_ERROR) {
      DEBUG_PRINT("Failed to receive message: %d\n", result);
      ok = false;
    }
    if (message.type != MESSAGE_TICK) {
      DEBUG_PRINT("Received message type mismatch: %d\n", message.type);
      ok = false;
    }
  }
  return ok;
}

bool _step(simulation_t *simulation) {
  bool ok = false;
  if (simulation != NULL && simulation->info != NULL && simulation->_conn != NULL) {
    ok = true;
  }

  if (ok) {
    ok = ok && _wait_for_tick(simulation);
  }

  if (ok && simulation->_on_tick != NULL) {
    simulation->_on_tick(simulation, simulation->user_data);
    message_t message = {0};
    message.type = MESSAGE_SIL_OUTPUT;
    message.length = simulation->info->num_pins;
    message.entity_id = simulation->info->entity_id;
    for (size_t i = 0; i < simulation->info->num_pins; i++) {
      message.pin_levels[i] = (uint8_t)simulation->info->pins[i];
    }
    int8_t result = send_message(simulation->_conn, &message);
    if (result != ERR_CONN_NO_ERROR) {
      DEBUG_PRINT("Failed to send message: %d\n", result);
      ok = false;
    }
  }

  if (ok) {
    simulation->info->_current_tick++;
  }
  return ok;
}

int8_t _do_handshake(simulation_t *simulation) {
  int8_t result = ERR_CONN_INVALID_STATE;
  if (simulation != NULL && simulation->_conn != NULL && simulation->info != NULL) {
    result = connect_to_server("127.0.0.1", 8080, simulation->_conn);
    if (result != ERR_CONN_NO_ERROR) {
      DEBUG_PRINT("Failed to connect to server: %d\n", result);
    }
    if (result == ERR_CONN_NO_ERROR) {
      message_t message = {0};
      message.type = MESSAGE_CONNECT;
      message.length = 8;
      message.entity_id = simulation->info->entity_id;
      result = send_message(simulation->_conn, &message);
      if (result != ERR_CONN_NO_ERROR) {
        DEBUG_PRINT("Failed to send message: %d\n", result);
      }
    }
  }

  return result;
}

void run(simulation_t *simulation) {
  bool ok = false;
  if (simulation != NULL && simulation->info != NULL && simulation->_conn != NULL) {
    ok = true;
  }

  if (ok) {
    ok = (_do_handshake(simulation) == ERR_CONN_NO_ERROR);
  }

  if (ok) {
    simulation->_is_running = true;
  }

  if (ok && simulation->_on_setup != NULL) {
    simulation->_on_setup(simulation, simulation->user_data);
  }

  while (ok && simulation->_is_running && _step(simulation))
    ; // loop until the simulation is not ok

  if (simulation != NULL && simulation->info != NULL) {
    DEBUG_PRINT("Simulation terminated at tick %llu\n",
                simulation->info->_current_tick);
  }
  if (simulation != NULL && simulation->_on_teardown != NULL) {
    simulation->_on_teardown(simulation, simulation->user_data);
  }
}

void stop(simulation_t *simulation) {
  if (simulation != NULL) {
    simulation->_is_running = false;
  }
}

void set_user_data(simulation_t *simulation, void *user_data) {
  if (simulation != NULL) {
    simulation->user_data = user_data;
  }
}
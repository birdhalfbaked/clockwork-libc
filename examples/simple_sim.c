#include "clockwork/simulation.h"
#include <stdio.h>

#define NUM_PINS 1

#define PIN_A 0

void setup(simulation_t *simulation, void *user_data) {
  simulation->info->pins[PIN_A] = PIN_LEVEL_LOW;
}

void tick(simulation_t *simulation, void *user_data) {
  if (simulation->info->_current_tick % 100 == 0) {
    simulation->info->pins[PIN_A] ^= 1;
    fprintf(stderr, "Pin A: %d at tick %llu\n", simulation->info->pins[PIN_A],
            simulation->info->_current_tick);
  }
}

void terminate(simulation_t *simulation, void *user_data) {
  fprintf(stderr, "Simulation terminated\n");
}

int main() {
  fprintf(stderr, "Starting simulation\n");
  // info
  simulation_info_t info = {
      .entity_id = 0,
      .name = "Simple Simulation",
      .num_pins = NUM_PINS,
  };
  fprintf(stderr, "Info: %llu\n", info.entity_id);
  tcp_conn_t conn;
  simulation_t simulation;
  simulation.info = &info;
  fprintf(stderr, "Initialized simulation\n");
  simulation.info = &info;
  initialize_simulation(&simulation);

  simulation._conn = &conn;
  simulation._on_setup = setup;
  simulation._on_tick = tick;
  simulation._on_teardown = terminate;

  run(&simulation);
  return 0;
}
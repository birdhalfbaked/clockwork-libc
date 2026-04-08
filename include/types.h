#ifndef CLOCKWORK_TYPES_H
#define CLOCKWORK_TYPES_H

#include <stdint.h>

typedef uint64_t tick_t;
typedef uint64_t entity_id_t;

typedef struct {
  entity_id_t entity_id;
  tick_t current_tick;
  char *name;
} entity_t;

/// @brief The level of a pin
/// @details This is a simple enum that represents the level of a pin. It is
/// used to represent the level of a pin in a message.
typedef enum {
  /// @brief The pin is low
  PIN_LEVEL_LOW = 0,
  /// @brief The pin is high
  PIN_LEVEL_HIGH = 1,
  /// @brief The pin is in a high impedance state
  PIN_LEVEL_FLOATING = 2,
} pin_level_t;

#endif
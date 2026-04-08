#ifndef CLOCKWORK_MESSAGES_H
#define CLOCKWORK_MESSAGES_H

#include "types.h"
#include <stdint.h>

enum {
  ERR_MESSAGE_INVALID_LENGTH = -1,
  ERR_MESSAGE_INVALID_TYPE = -2,
  ERR_MESSAGE_INVALID_DATA = -3,
};

typedef enum {
  MESSAGE_CONNECT = 0,
  MESSAGE_TICK = 1,
  MESSAGE_SIL_OUTPUT = 2,
  MESSAGE_INVALID = 255
} message_type_t;

/// @brief This is a fat message that contains all the data for the messages we
/// expect
/// @details Done this way as it avoids having to perform either duplication of
/// methods for (de)serialization or having to do pointer magic
typedef struct {
  /// @brief The type of message
  message_type_t type;
  /// @brief The length of the message
  uint8_t length;
  /// @brief The entity ID of the message
  entity_id_t entity_id;
  /// @brief The pin levels of the message (see @ref pin_level_t)
  pin_level_t pin_levels[256];
} message_t;

/// @brief Convert a byte array to a message
/// @param bytes The byte array to convert
/// @param buf_length The length of the byte array
/// @param out_message The message to convert to
/// @return The number of bytes read, or an error code
int8_t bytes_to_message(const uint8_t *bytes, size_t buf_length,
                        message_t *out_message);
/// @brief Convert a message to a byte array
/// @param message The message to convert
/// @param bytes The byte array to convert to
/// @param buf_length The length of the byte array
/// @return The length of the byte array, or an error code
int8_t message_to_bytes(const message_t *message, uint8_t *bytes,
                        size_t buf_length);

#endif
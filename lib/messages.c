#include "clockwork/messages.h"
#include <stdint.h>

int8_t bytes_to_message(const uint8_t *bytes, size_t buf_length,
                        message_t *out_message) {
  message_t message;
  message_type_t type = (message_type_t)bytes[0];
  size_t length = (size_t)bytes[1];
  int8_t result = length + 2;
  if (length + 2 > buf_length) {
    result = ERR_MESSAGE_INVALID_LENGTH;
  }
  if (result >= 0) {
    switch (type) {
    case MESSAGE_CONNECT: {
      if (length != 8) {
        result = ERR_MESSAGE_INVALID_DATA;
        break;
      }

      out_message->type = type;
      out_message->length = length;
      entity_id_t entity_id = 0;
      for (size_t i = 0; i < 8; i++) {
        entity_id |= (entity_id_t)bytes[2 + i] << (i * 8);
      }
      out_message->entity_id = entity_id;
      break;
    }
    case MESSAGE_TICK: {
      out_message->type = type;
      out_message->length = length;
      break;
    }
    case MESSAGE_SIL_OUTPUT: {
      out_message->type = type;
      out_message->length = length;
      for (size_t i = 0; i < length; i++) {
        out_message->pin_levels[i] = (pin_level_t)bytes[2 + i];
      }
      break;
    }
    default:
      out_message->type = MESSAGE_INVALID;
      result = ERR_MESSAGE_INVALID_TYPE;
      break;
    }
  }
  return result;
}

int8_t message_to_bytes(const message_t *message, uint8_t *bytes,
                        size_t buf_length) {
  int8_t result = message->length + 2;
  if (message->length + 2 > buf_length || buf_length > 256 + 2) {
    result = ERR_MESSAGE_INVALID_LENGTH;
  }
  bytes[0] = message->type;
  bytes[1] = (uint8_t)message->length;
  if (result >= 0) {
    switch (message->type) {
    case MESSAGE_CONNECT: {
      // binary encode in LE the 8-byte entity ID
      for (size_t i = 0; i < 8; i++) {
        bytes[2 + i] = (uint8_t)(message->entity_id >> (i * 8)) & 0xFF;
      }
      break;
    }
    case MESSAGE_TICK: {
      break;
    }
    case MESSAGE_SIL_OUTPUT: {
      for (size_t i = 0; i < message->length; i++) {
        bytes[2 + i] = (uint8_t)message->pin_levels[i];
      }
      break;
    }
    default:
      result = ERR_MESSAGE_INVALID_TYPE;
      break;
    }
  }
  return result;
}
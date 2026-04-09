#include "clockwork/messages.h"
#include "testing.h"
#include <stdbool.h>

void test_message_to_bytes() {
  message_t message;
  message.type = MESSAGE_CONNECT;
  message.length = 8;
  message.entity_id = 1;

  uint8_t bytes[256];
  int8_t byte_length = message_to_bytes(&message, bytes, 256);
  if (byte_length != 10) {
    FAIL("Message length mismatch: %d != %d", byte_length, 10);
  }
  if (bytes[0] != MESSAGE_CONNECT) {
    FAIL("Message type mismatch: %d != %d", bytes[0], MESSAGE_CONNECT);
  }
  if (bytes[1] != 8) {
    FAIL("Payload length mismatch: %d != %d", bytes[1], 8);
  }
  if (bytes[2] != 1) {
    FAIL("Message entity ID mismatch: %d != %d", bytes[2], 1);
  }
}

void test_bytes_to_message() {

  uint8_t bytes[10] = {MESSAGE_CONNECT, 8, 1, 0, 0, 0, 0, 0, 0, 0};
  message_t message;
  int8_t result = bytes_to_message(bytes, 10, &message);
  if (result != 10) {
    FAIL("Bytes to message conversion failed: %d", result);
  }
  if (message.type != MESSAGE_CONNECT) {
    FAIL("Message type mismatch: %d != %d", message.type, MESSAGE_CONNECT);
  }
  if (message.length != 8) {
    FAIL("Message length mismatch: %d != %d", message.length, 8);
  }
  if (message.entity_id != 1) {
    FAIL("Message entity ID mismatch: %lld != %lld", message.entity_id, 1LL);
  }
}

void test_invalid_message_type() {
  // bytes_to_message: unknown type should error and set MESSAGE_INVALID
  uint8_t bytes[2] = {123, 0};
  message_t message;
  int8_t result = bytes_to_message(bytes, 2, &message);
  if (result != ERR_MESSAGE_INVALID_TYPE) {
    FAIL("Expected invalid type error: %d != %d", result,
         ERR_MESSAGE_INVALID_TYPE);
  }
  if (message.type != MESSAGE_INVALID) {
    FAIL("Expected MESSAGE_INVALID: %d != %d", message.type, MESSAGE_INVALID);
  }

  // message_to_bytes: unknown type should error
  message_t out;
  out.type = (message_type_t)123;
  out.length = 0;
  out.entity_id = 0;
  uint8_t out_bytes[8] = {0};
  int8_t out_result = message_to_bytes(&out, out_bytes, sizeof(out_bytes));
  if (out_result != ERR_MESSAGE_INVALID_TYPE) {
    FAIL("Expected invalid type error: %d != %d", out_result,
         ERR_MESSAGE_INVALID_TYPE);
  }
}

void register_tests__messages() {
  REGISTER_TEST(test_message_to_bytes);
  REGISTER_TEST(test_bytes_to_message);
  REGISTER_TEST(test_invalid_message_type);
}
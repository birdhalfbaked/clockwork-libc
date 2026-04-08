#include "testing.h"
#include <stdint.h>

extern void messages_tests();

test_result_t test_data[1024];
uint16_t test_count = 0;
uint16_t current_test = 0;

void run_tests() {
  for (size_t i = 0; i < test_count; i++) {
    test_data[i].func();
  }
}

void register_all() { register_tests__messages(); }

int main() {
  register_all();
  run_tests();
  for (uint16_t i = 0; i < test_count; i++) {
    if (!test_data[i].passed) {
      printf("Test %s failed\n", test_data[i].name);
    } else {
      printf("Test %s passed\n", test_data[i].name);
    }
  }
  return 0;
}
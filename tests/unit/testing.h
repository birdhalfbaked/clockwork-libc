#ifndef TESTING_H
#define TESTING_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef void (*test_func_t)();

typedef struct {
  test_func_t func;
  const char *name;
  bool passed;
} test_result_t;

extern test_result_t test_data[1024];
extern uint16_t test_count;
extern uint16_t current_test;

#define FAIL(format, ...)                                                      \
  fprintf(stderr, "%s:%d - " format "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
  test_data[current_test].passed = false;

#define REGISTER_TEST(test_func)                                               \
  test_data[test_count++] = (test_result_t){                                   \
      .func = (test_func_t)test_func, .name = #test_func, .passed = true};

// #####################################
// # TEST REGISTRATION
// #####################################

// test_messages.c
void register_tests__messages();

#endif

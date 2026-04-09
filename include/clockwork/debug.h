#ifndef CLOCKWORK_DEBUG_H
#define CLOCKWORK_DEBUG_H

#ifdef _DEBUG
#include <stdio.h>
#define DEBUG_PRINT(format, ...)                                               \
  fprintf(stderr, "[DEBUG] " format "\n", ##__VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...) // DEBUG_PRINT: noop
#endif

#endif
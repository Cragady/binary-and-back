#ifndef BAB_BAB_H_
#define BAB_BAB_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define BYTE_BITS 8
#define BYTES 1
#define KILO_BYTE 1024

typedef struct {
  char *str;
  size_t len;
} StringData;

typedef struct SimpleData_s {
  char c;
  int32_t i;
  short s;
  bool b;
  uint16_t test;
} SimpleData;

int bab_entry();
int no_sdl();

#endif

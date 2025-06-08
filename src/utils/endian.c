#include "utils/endian.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

static uint8_t filter_byte = UTILS_ENDIAN_H_FILTER_BYTE;
static size_t byte_width = UTILS_ENDIAN_H_BYTE_WIDTH;
static size_t num_bytes = UTILS_ENDIAN_H_NUM_BYTES_INIT;
static EndianOrder target = ENDIAN_TYPE_BIG;
static EndianOrder host = ENDIAN_TYPE_BIG;
static bool should_reverse = false;

bool is_little_endian(EndianOrder target_arg) {
  unsigned int num = 1;
  char *ptr = (char *)&num;
  target = target_arg;

  if (*ptr == 1) {
    host = ENDIAN_TYPE_LITTLE;
  } else {
    host = ENDIAN_TYPE_BIG;
  }

  should_reverse = target != host;
  return host == ENDIAN_TYPE_LITTLE;
}

void log_endianness() {
  switch (host) {
    case ENDIAN_TYPE_BIG:
      printf("Big Endian\n");
      break;
    case ENDIAN_TYPE_LITTLE:
      printf("Little Endian\n");
      break;
  }
}

uint16_t swap16(uint16_t bytes) {
  num_bytes = 2;
  return (uint16_t)swap(bytes);
}

uint32_t swap32(uint32_t bytes) {
  num_bytes = 4;
  return (uint32_t)swap(bytes);
}

uint64_t swap64(uint64_t bytes) {
  num_bytes = 8;
  return swap(bytes);
}

uint64_t swap(uint64_t bytes) {
  if (host == target) return bytes;
  uint64_t dest = 0;

  for (size_t i = 0; i < num_bytes; i++) {
    dest |= (bytes & filter_byte);
    if (i < num_bytes - 1) dest <<= byte_width;
    bytes >>= byte_width;
  }


  return dest;
}


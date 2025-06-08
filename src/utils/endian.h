#ifndef UTILS_ENDIAN_H_
#define UTILS_ENDIAN_H_

#include <stdint.h>
#include <stdbool.h>

#define UTILS_ENDIAN_H_FILTER_BYTE 0xff
#define UTILS_ENDIAN_H_BYTE_WIDTH 8
#define UTILS_ENDIAN_H_NUM_BYTES_INIT 4

typedef enum ENDIAN_TYPE_ {
  ENDIAN_TYPE_BIG,
  ENDIAN_TYPE_LITTLE,
} EndianOrder;

bool is_little_endian(EndianOrder target_arg);
void log_endianness();
uint16_t swap16(uint16_t bytes);
uint32_t swap32(uint32_t bytes);
uint64_t swap64(uint64_t bytes);
uint64_t swap(uint64_t bytes);


#endif

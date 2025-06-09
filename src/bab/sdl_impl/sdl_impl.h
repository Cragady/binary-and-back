#ifndef BAB_SDL_IMPL_SDL_IMPL_H_
#define BAB_SDL_IMPL_SDL_IMPL_H_

#include <stdint.h>
#include <stdbool.h>
#include <SDL3/SDL_iostream.h>

typedef struct SdlData_s {
  char c;
  int32_t i;
  short s;
  bool b;
  uint16_t test;
} SdlData;

typedef enum FILE_OPERATIONS_E {
  FILE_OPERATIONS_OPEN,
  FILE_OPERATIONS_WRITE_PORTION,
  FILE_OPERATIONS_FLUSH,
  FILE_OPERATIONS_CLOSE,
  FILE_OPERATIONS_SUCCESS,
  FILE_OPERATIONS_READ,
} FILE_OPERATIONS_;

int sdl_impl_entry();
void log_sdl_endianness();
void log_sdl_file_operation_status(FILE_OPERATIONS_ code);
void write_file(const char *restrict file_name, SdlData data);
void write_text_file(const char *restrict file_name, SdlData data);
void read_log_compare_data(const char *restrict file_name, SdlData data);

#endif

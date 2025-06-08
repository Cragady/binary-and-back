#ifndef BAB_SDL_IMPL_SDL_IMPL_H_
#define BAB_SDL_IMPL_SDL_IMPL_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct SdlData_s {
  char c;
  int32_t i;
  short s;
  bool b;
  uint16_t test;
} SdlData;

int sdl_impl_entry();
void log_sdl_endianness();
FILE *open_file(const char *restrict file_name, const char *restrict mode);
void write_file(const char *restrict file_name, SdlData data);
void write_text_file(const char *restrict file_name, SdlData data);
void read_log_compare_data(const char *restrict file_name, SdlData data);

#endif

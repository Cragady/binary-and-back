#include "bab/sdl_impl/sdl_impl.h"

#include <assert.h>
#include <stdio.h>
#include <SDL3/SDL_endian.h>

#define FILE_NAME "bin_files/sdl_test"

int sdl_impl_entry() {
  printf("\n\n-------------\n\n");
  printf("SDL Impl: \n");
  log_sdl_endianness();


  SdlData data = { .c = 'd', .i = 8, .s = 4, .b = true, .test = 0x10ff, };

  write_file(FILE_NAME ".wat", data);
  write_text_file(FILE_NAME ".txt", data);
  read_log_compare_data(FILE_NAME ".wat", data);

  return 0;
}

void log_sdl_endianness() {
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
    printf("SDL: Big Endian System\n");
  } else if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
    printf("SDL: Little Endian System\n");
  }
}

FILE *open_file(const char *restrict file_name, const char *restrict mode) {
  FILE *file = fopen(file_name, mode);
  assert(file);
  return file;
}

void write_file(const char *restrict file_name, SdlData data) {
  FILE *file = open_file(file_name, "wb+");

  // NOTE: we have no need to swap a 1 byte large data chunk
  fwrite(&data.c, sizeof(data.c), 1, file);
  // TODO: find better way to do this
  if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
    data.i = SDL_Swap32(data.i);
  }
  fwrite(&data.i, sizeof(data.i), 1, file);
  data.s = SDL_Swap16(data.s);
  fwrite(&data.s, sizeof(data.s), 1, file);
  // NOTE: we have no need to swap a 1 byte large data chunk
  fwrite(&data.b, sizeof(data.b), 1, file);
  data.test = SDL_Swap16(data.test);
  fwrite(&data.test, sizeof(data.test), 1, file);

  fclose(file);
  printf("Manual data write success!\n");
}

void write_text_file(const char *restrict file_name, SdlData data) {
  FILE *txt = open_file(file_name, "w+");

  fprintf(txt, "%b\n", data.c);
  fprintf(txt, "%b\n", data.i);
  fprintf(txt, "%b\n", data.s);
  fprintf(txt, "%b\n", data.b);
  fprintf(txt, "%b\n", data.test);

  fclose(txt);
  printf("Data as binary text write seuccess!\n");
}

void read_log_compare_data(const char *restrict file_name, SdlData data) {
  FILE *file = open_file(file_name, "rb");
  size_t bytes_read = 0;

  // NOTE: can alternatively read byte by byte and convert that way instead,
  // but that seems like a pain. If there are arrays, a buffer may be necessary
  // char buff[BYTES];
  // char data_buff[KILO_BYTE];
  SdlData read_bytes;

  do {
    bytes_read = fread(&read_bytes.c, sizeof(read_bytes.c), 1, file);
    if (bytes_read == 0) break;
    // NOTE: we have no need to swap a 1 byte large data chunk


    bytes_read = fread(&read_bytes.i, sizeof(read_bytes.i), 1, file);
    if (bytes_read == 0) break;
    read_bytes.i = SDL_Swap32BE(read_bytes.i);


    bytes_read = fread(&read_bytes.s, sizeof(read_bytes.s), 1, file);
    if (bytes_read == 0) break;
    read_bytes.s = SDL_Swap16BE(read_bytes.s);


    bytes_read = fread(&read_bytes.b, sizeof(read_bytes.b), 1, file);
    if (bytes_read == 0) break;
    // NOTE: we have no need to swap a 1 byte large data chunk


    bytes_read = fread(&read_bytes.test, sizeof(read_bytes.test), 1, file);
    if (bytes_read == 0) break;
    read_bytes.test = SDL_Swap16BE(read_bytes.test);

  } while (0);

  char c;

  bool need_line = false;
  while (fread(&c, sizeof(char), 1, file)) {
    // printf("%c", c);
    // need_line = true;
    printf("Missed file char: %c\n", c);
  }
  if (need_line) printf("\n");

  printf("read_bytes.c (int) %c\n", read_bytes.c);
  printf("data.c (int) %c\n", data.c);
  printf("read_bytes.i (int) %i\n", read_bytes.i);
  printf("data.i (int) %i\n", data.i);
  printf("read_bytes.s (short) %i\n", read_bytes.s);
  printf("data.s (short) %i\n", data.s);
  printf("read_bytes.b (bool) %i\n", read_bytes.b);
  printf("data.b (bool) %i\n", data.b);
  printf("read_bytes.test (uint16_t) 0x%x\n", read_bytes.test);
  printf("data.test (uint16_t) 0x%x\n", data.test);

  bool end_reached = false;
  if (ferror(file)) {
    printf("Error reading file\n");
  } else if(feof(file)) {
    printf("End of file reached.\n");
    end_reached = true;
  }

  if (!end_reached) {
    printf("End of file was not reached!\n");
  }
  fclose(file);
}

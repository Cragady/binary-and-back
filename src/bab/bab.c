#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/endian.h"

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

int bab_entry() {
  is_little_endian(ENDIAN_TYPE_BIG);
  log_endianness();

  FILE *file = fopen("bin_files/name-here.wat", "wb+");
  if (file == NULL) {
    printf("Could not open, or allocate space, for file!\n");
    exit(1);
  }

  SimpleData data = { .c = 'd', .i = 8, .s = 4, .b = true, .test = 0x10ff, };
  // NOTE: we'll get `copy_data` by having a copy assignment happen in a
  // funciton argument
  SimpleData copy_data = data;

  // NOTE: we have no need to swap a 1 byte large data chunk
  fwrite(&copy_data.c, sizeof(copy_data.c), 1, file);
  copy_data.i = swap32(copy_data.i);
  fwrite(&copy_data.i, sizeof(copy_data.i), 1, file);
  copy_data.s = swap16(copy_data.s);
  fwrite(&copy_data.s, sizeof(copy_data.s), 1, file);
  // NOTE: we have no need to swap a 1 byte large data chunk
  fwrite(&copy_data.b, sizeof(copy_data.b), 1, file);
  copy_data.test = swap16(copy_data.test);
  fwrite(&copy_data.test, sizeof(copy_data.test), 1, file);

  fclose(file);
  printf("Manual data write success!\n");

  FILE *txt = fopen("bin_files/name-here.txt", "w+");

  if (txt == NULL) {
    printf("Could not open, or allocate space, for file!\n");
    exit(1);
  }

  fprintf(txt, "%b\n", data.c);
  fprintf(txt, "%b\n", data.i);
  fprintf(txt, "%b\n", data.s);
  fprintf(txt, "%b\n", data.b);
  fprintf(txt, "%b\n", data.test);

  fclose(txt);

  file = fopen("bin_files/name-here.wat", "rb");
  if (file == NULL) {
    printf("Could not open file!\n");
    exit(0);
  }

  char buff[BYTES];
  char data_buff[KILO_BYTE];
  size_t bytes_read = 0;

  // NOTE: this is for just reading. Segment file into data chunks
  // while ((bytes_read = fread(buff, BYTES, sizeof(buff), file)) > 0) {

  // }

  SimpleData read_bytes;
  do {
    bytes_read = fread(&read_bytes.c, sizeof(read_bytes.c), 1, file);
    if (bytes_read == 0) break;
    // NOTE: we have no need to swap a 1 byte large data chunk


    bytes_read = fread(&read_bytes.i, sizeof(read_bytes.i), 1, file);
    if (bytes_read == 0) break;
    read_bytes.i = swap32(read_bytes.i);


    bytes_read = fread(&read_bytes.s, sizeof(read_bytes.s), 1, file);
    if (bytes_read == 0) break;
    read_bytes.s = swap16(read_bytes.s);


    bytes_read = fread(&read_bytes.b, sizeof(read_bytes.b), 1, file);
    if (bytes_read == 0) break;
    // NOTE: we have no need to swap a 1 byte large data chunk


    bytes_read = fread(&read_bytes.test, sizeof(read_bytes.test), 1, file);
    if (bytes_read == 0) break;
    read_bytes.test = swap16(read_bytes.test);

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

  return 0;
}


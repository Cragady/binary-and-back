#include "bab/sdl_impl/sdl_impl.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_endian.h>
#include <SDL3/SDL_iostream.h>

#define FILE_NAME "bin_files/sdl_test"

// TODO: change instances of SDL_Log to appropriate log level call

int sdl_impl_entry() {
  SDL_Log("\n\n-------------\n\n");
  SDL_Log("SDL Impl: \n");
  log_sdl_endianness();


  SdlData data = { .c = 'd', .i = 8, .s = 4, .b = true, .test = 0x10ff, };

  write_file(FILE_NAME ".wat", data);
  write_text_file(FILE_NAME ".txt", data);
  read_log_compare_data(FILE_NAME ".wat", data);

  return 0;
}

void log_sdl_endianness() {
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
    SDL_Log("SDL: Big Endian System\n");
  } else if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
    SDL_Log("SDL: Little Endian System\n");
  }
}

void log_sdl_file_operation_status(FILE_OPERATIONS_ code) {
  switch (code) {
    case FILE_OPERATIONS_OPEN:
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to open file for read/write!\n%s\n", SDL_GetError());
      break;
    case FILE_OPERATIONS_WRITE_PORTION:
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Unable to write data to file!\n%s\n", SDL_GetError());
      break;
    case FILE_OPERATIONS_FLUSH:
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to flush file!\n%s\n", SDL_GetError());
      break;
    case FILE_OPERATIONS_CLOSE:
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to close file!\n%s\n", SDL_GetError());
      break;
    case FILE_OPERATIONS_SUCCESS:
      SDL_Log("File operations successfully executed!\n");
      break;
    case FILE_OPERATIONS_READ:
      SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Failed to read from file!\n%s\n", SDL_GetError());
      break;
  }
}

void write_file(const char *restrict file_name, SdlData data) {
  FILE_OPERATIONS_ status = 0;

  do {
    bool successful_operation = true;

    SDL_IOStream *file = SDL_IOFromFile(file_name, "wb+");
    if (file == NULL) break;
    status++;

    // NOTE: we have no need to swap a 1 byte large data chunk
    // NOTE: can probably also use `SDL_WriteS8`
    successful_operation = SDL_WriteU8(file, data.c);
    if (!successful_operation) break;

    successful_operation = SDL_WriteS32BE(file, data.i);
    if (!successful_operation) break;

    successful_operation = SDL_WriteS16BE(file, data.s);
    if (!successful_operation) break;

    // NOTE: we have no need to swap a 1 byte large data chunk
    successful_operation = SDL_WriteU8(file, data.b);
    if (!successful_operation) break;

    successful_operation = SDL_WriteU16BE(file, data.test);
    if (!successful_operation) break;

    status++;

    successful_operation = SDL_FlushIO(file);
    if (!successful_operation) break;
    status++;

    successful_operation = SDL_CloseIO(file);
    if (!successful_operation) break;
    status++;

  } while(0);

  log_sdl_file_operation_status(status);
}

void write_text_file(const char *restrict file_name, SdlData data) {
  FILE_OPERATIONS_ status = 0;

  do {
    bool successful_operation = true;

    SDL_IOStream *txt = SDL_IOFromFile(file_name, "w+");
    if (txt == NULL) break;
    status++;

    successful_operation = SDL_IOprintf(txt, "%b\n", data.c) > 0;
    if (!successful_operation) break;
    successful_operation = SDL_IOprintf(txt, "%b\n", data.i) > 0;
    if (!successful_operation) break;
    successful_operation = SDL_IOprintf(txt, "%b\n", data.s) > 0;
    if (!successful_operation) break;
    successful_operation = SDL_IOprintf(txt, "%b\n", data.b) > 0;
    if (!successful_operation) break;
    successful_operation = SDL_IOprintf(txt, "%b\n", data.test) > 0;
    if (!successful_operation) break;

    status++;

    successful_operation = SDL_FlushIO(txt);
    if (!successful_operation) break;
    status++;

    successful_operation = SDL_CloseIO(txt);
    if (!successful_operation) break;
    status++;

  } while(0);

  log_sdl_file_operation_status(status);
}

void read_log_compare_data(const char *restrict file_name, SdlData data) {
  FILE_OPERATIONS_ status;
  do {
    bool successful_operation = true;

    SDL_IOStream *file = SDL_IOFromFile(file_name, "rb");
    if (file == NULL) break;
    size_t bytes_read = 0;
    status = FILE_OPERATIONS_READ;

    // NOTE: can alternatively read byte by byte and convert that way instead,
    // but that seems like a pain. If there are arrays, a buffer may be necessary
    // char buff[BYTES];
    // char data_buff[KILO_BYTE];
    SdlData read_bytes;

    // NOTE: can maybe use `SDL_ReadU8`, or `SDL_ReadS8`
    // NOTE: we have no need to swap a 1 byte large data chunk
    bytes_read = SDL_ReadIO(file, &read_bytes.c, sizeof(read_bytes.c));
    if (bytes_read == 0) break;


    successful_operation = SDL_ReadS32BE(file, &read_bytes.i);
    if (!successful_operation) break;


    successful_operation = SDL_ReadS16BE(file, &read_bytes.s);
    if (!successful_operation) break;


    // NOTE: we have no need to swap a 1 byte large data chunk
    bytes_read = SDL_ReadIO(file, &read_bytes.b, sizeof(read_bytes.b));
    if (bytes_read == 0) break;


    successful_operation = SDL_ReadU16BE(file, &read_bytes.test);
    if (!successful_operation) break;


    char c;

    bool need_line = false;
    while (SDL_ReadIO(file, &c, sizeof(char))) {
      // SDL_Log("%c", c);
      // need_line = true;
      SDL_Log("Missed file char: %c\n", c);
    }
    if (need_line) SDL_Log("\n");

    SDL_Log("read_bytes.c (int) %c\n", read_bytes.c);
    SDL_Log("data.c (int) %c\n", data.c);
    SDL_Log("read_bytes.i (int) %i\n", read_bytes.i);
    SDL_Log("data.i (int) %i\n", data.i);
    SDL_Log("read_bytes.s (short) %i\n", read_bytes.s);
    SDL_Log("data.s (short) %i\n", data.s);
    SDL_Log("read_bytes.b (bool) %i\n", read_bytes.b);
    SDL_Log("data.b (bool) %i\n", data.b);
    SDL_Log("read_bytes.test (uint16_t) 0x%x\n", read_bytes.test);
    SDL_Log("data.test (uint16_t) 0x%x\n", data.test);

    bool end_reached = false;
    SDL_IOStatus file_status = SDL_GetIOStatus(file);
    if (file_status == SDL_IO_STATUS_ERROR) {
      SDL_Log("Error reading file\n");
    } else if(file_status == SDL_IO_STATUS_EOF) {
      SDL_Log("End of file reached.\n");
      end_reached = true;
    }

    if (!end_reached) {
      SDL_Log("End of file was not reached!\n");
    }
    status = FILE_OPERATIONS_CLOSE;

    successful_operation = SDL_CloseIO(file);
    if (!successful_operation) break;
    status++;

  } while (0);

  log_sdl_file_operation_status(status);
}

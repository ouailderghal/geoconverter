#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SV_IMPLEMENTATION
#include "sv.h"

char *read_file(const char *file_path, size_t *out_file_size) {
  FILE *file = fopen(file_path, "rb");
  char *file_buffer = NULL;
  int file_cursor;

  if (file == NULL)
    goto error;

  file_cursor = fseek(file, 0, SEEK_END);

  if (file_cursor < 0)
    goto error;

  long file_size = ftell(file);

  if (file_size < 0)
    goto error;

  file_buffer = malloc(sizeof(char) * file_size);

  if (file_buffer == NULL)
    goto error;

  file_cursor = fseek(file, 0, SEEK_SET);

  if (file_cursor < 0)
    goto error;

  size_t read_file_size = fread(file_buffer, 1, file_size, file);

  assert(read_file_size == (size_t)file_size);

  if (ferror(file))
    goto error;

  if (out_file_size)
    *out_file_size = read_file_size;
  fclose(file);

  return file_buffer;

error:
  if (file)
    fclose(file);

  if (file_buffer)
    free(file_buffer);

  return NULL;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("[ERR] no input file is provided");
    exit(EXIT_FAILURE);
  }

  const char *file_path = argv[1];
  size_t file_size = 0;
  char *file_content = read_file(file_path, &file_size);

  String_View content = {
    .count = file_size,
    .data = file_content,
  };

  printf(SV_Fmt "", SV_Arg(content));

  return EXIT_SUCCESS;
}

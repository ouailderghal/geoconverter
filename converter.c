#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SV_IMPLEMENTATION
#include "sv.h"

typedef struct {
  double degree;
  double minute;
  double seconde;
  char direction;
} SubCoordinate;

typedef struct {
  SubCoordinate lhs;
  SubCoordinate rhs;
} DMSGeoCoordinate;

typedef struct {
  char lat_direction;
  double lat;
  char lon_direction;
  double lon;
} DDGeoCoordinate;

DDGeoCoordinate dms_to_dd(DMSGeoCoordinate dms_coordiante) {
  DDGeoCoordinate dd_coordinate = {0};

  dd_coordinate.lat = dms_coordiante.lhs.degree +
                      (dms_coordiante.lhs.minute / 60) +
                      (dms_coordiante.lhs.seconde / 3600);

  dd_coordinate.lon = dms_coordiante.rhs.degree +
                      (dms_coordiante.rhs.minute / 60) +
                      (dms_coordiante.rhs.seconde / 3600);

  switch (dms_coordiante.lhs.direction) {
  case 'W':
  case 'S':
    dd_coordinate.lat_direction = '-';
    break;
  case 'E':
  case 'N':
    dd_coordinate.lat_direction = '+';
    break;
  }

  switch (dms_coordiante.rhs.direction) {
  case 'W':
  case 'S':
    dd_coordinate.lon_direction = '-';
    break;
  case 'E':
  case 'N':
    dd_coordinate.lon_direction = '+';
    break;
  }

  return dd_coordinate;
}

void print_dd_coordinate(DDGeoCoordinate *coordinate) {
  printf("%c%f, %c%f\n", coordinate->lat_direction, coordinate->lat,
         coordinate->lon_direction, coordinate->lon);
}

void print_dms_coordinate(DMSGeoCoordinate *coordinate) {
  printf("(%f, %f, %f, %c) (%f, %f, %f, %c)\n", coordinate->lhs.degree,
         coordinate->lhs.minute, coordinate->lhs.seconde,
         coordinate->lhs.direction, coordinate->rhs.degree,
         coordinate->rhs.minute, coordinate->rhs.seconde,
         coordinate->rhs.direction);
}

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

  for (size_t row = 0; content.count > 0; ++row) {
    String_View line = sv_chop_by_delim(&content, '\n');
    DMSGeoCoordinate coordinate;

    for (size_t t = 0; line.count > 0; ++t) {
      String_View token_string = sv_chop_by_delim(&line, ' ');

      char *endptr;
      double token = strtod(token_string.data, &endptr);

      if (*endptr == 'N' || *endptr == 'E' || *endptr == 'S' ||
          *endptr == 'W') {
        switch (t) {
        case 3:
          coordinate.lhs.direction = *endptr;
          break;
        case 7:
          coordinate.rhs.direction = *endptr;
        }
      } else {
        switch (t) {
        case 0:
          coordinate.lhs.degree = token;
          break;
        case 1:
          coordinate.lhs.minute = token;
          break;
        case 2:
          coordinate.lhs.seconde = token;
          break;
        case 4:
          coordinate.rhs.degree = token;
          break;
        case 5:
          coordinate.rhs.minute = token;
          break;
        case 6:
          coordinate.rhs.seconde = token;
          break;
        }
      }
    }

    // print_dms_coordinate(&coordinate);
    DDGeoCoordinate c = dms_to_dd(coordinate);
    print_dd_coordinate(&c);
  }

  return EXIT_SUCCESS;
}

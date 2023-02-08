#include <string.h>
#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"

#define CFLAGS "-Wall", "-Wextra", "-std=c11", "-pedantic", "-ggdb"

int main(int argc, char **argv)
{
  GO_REBUILD_URSELF(argc, argv);
  CMD("cc", CFLAGS, "-o", "converter", "converter.c");
  CMD("./converter", "data.csv");

  return 0;
}

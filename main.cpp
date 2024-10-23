#include "assembler.h"
#include <assert.h>
#include <stddef.h>

static int ARGS_REQUIRED_NUMBER = 3;
static size_t INPUT_FILE_NAME_INDEX = 1;
static size_t OUTPUT_FILE_NAME_INDEX = 2;

int main(int argc, const char** argv) {
  assert(argc == ARGS_REQUIRED_NUMBER);
  assert(argv[INPUT_FILE_NAME_INDEX]);
  assert(argv[OUTPUT_FILE_NAME_INDEX]);

  translate_assembler_to_binary(argv[INPUT_FILE_NAME_INDEX],
                                argv[OUTPUT_FILE_NAME_INDEX]);

  return 0;
}

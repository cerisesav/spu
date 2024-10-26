#include "assembler.h"
#include "processor.h"
#include "tokenizer.h"

#include <assert.h>
#include <stddef.h>

static int ARGS_REQUIRED_NUMBER = 3;
static size_t INPUT_FILE_NAME_INDEX = 1;
static size_t OUTPUT_FILE_NAME_INDEX = 2;

void run_processor_from_file(const char* binary_file_name, size_t code_size);

int main(int argc, const char** argv) {

    assert(argc == ARGS_REQUIRED_NUMBER);
    assert(argv[INPUT_FILE_NAME_INDEX]);
    assert(argv[OUTPUT_FILE_NAME_INDEX]);

    translate_assembler_to_binary(argv[INPUT_FILE_NAME_INDEX], argv[OUTPUT_FILE_NAME_INDEX]);

    FILE* out_file = fopen(argv[OUTPUT_FILE_NAME_INDEX], "rb");
    assert(out_file);

    run_processor_from_file(argv[OUTPUT_FILE_NAME_INDEX], get_file_len(out_file));

    return 0;
}

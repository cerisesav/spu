#include "processor.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void run_processor_from_file(const char* binary_file_name, size_t code_size) {

    struct Spu spu = {};
    SpuCtor(&spu, code_size * sizeof(int), register_count);

    FILE* in_file = fopen(binary_file_name, "rb");
    assert(in_file);

    fread(spu.code, sizeof(int), code_size, in_file);

    fclose(in_file);

    Run(&spu, code_size);

    SpuDtor(&spu);
}

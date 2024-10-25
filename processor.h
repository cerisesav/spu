#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "stack.h"

#define FOPEN(file, name, type)   \
FILE *name = fopen(file, type);     \
                                    \
    if (name == NULL)               \
    {                               \
        fprintf(stderr, "Failed to open the file\n"); \
        assert(0);                  \
    }                               \

struct Spu {
    char* code;
    int ip;
    Stack stack;
    int* registers;
};

void Run(Spu* spu, long size);

void SpuCtor(struct Spu* spu, long code_size, int reg_count);

void SpuDtor(struct Spu* spu);

#endif

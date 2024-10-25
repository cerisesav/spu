#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "stack.h"
#include "error.h"
#include "processor.h"
#include "assembler.h"

#define DEF_CMD(name, num, arg, code) \
    case name##_OP: \
        code; \
        break;

void SpuCtor(struct Spu* spu, long code_size, int reg_count)
{
    spu->code = (char*)calloc(code_size, sizeof(int));
    spu->registers = (int*)calloc(reg_count, sizeof(int));
    spu->ip = 0;
    StackCtor(&spu->stack);
}

void SpuDtor(struct Spu* spu)
{
    free(spu->code);
    free(spu->registers);
    StackDtor(&spu->stack);
}

void Run(Spu* spu, long size)
{
    printf("Executing instruction at ip = %d: %d\n", spu->ip, spu->code[spu->ip]);

    while (spu->ip < size)
    {

        switch (spu->code[spu->ip])
        {
            #include "commands.h"

            default:
                printf("Unknown operation: %d %d\n", spu->code[spu->ip], spu->ip);
                spu->ip += sizeof(int);
                break;
        }
    }

    StackDumpMacro(&spu->stack);
}
#undef DEF_CMD


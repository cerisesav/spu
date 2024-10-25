#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "error.h"

void StackDump(struct Stack* stk, const char *file, int line, const char *function)
{
    StackAssert(stk);

    printf("\nTime is %s\n", __TIME__);
    printf("StackDump called from %s (%d) %s\n", function, line, file);
    printf("Stack[%p] \"stk\" in function - %s.\n\n", (void*)stk, function);

    printf("--------------------------------------------------------------------------\n");
    printf("Struct:\n");
    printf("\tsize = %d\n", stk->size);
    printf("\tcapacity = %lu\n", stk->capacity);
    printf("\tAddress of data[] = %p\n", (void*)stk->data);

    for (size_t i = 0; i < stk->capacity; i++) {
        printf("\tdata[%zu] = %lf\n", i, stk->data[i]);
    }

    printf("--------------------------------------------------------------------------\n");
}

void StackAssert(Stack* stk)
{
    if (stk->size < 0)
    {
        stk->err = Size;
        printf("Error: %s\n", StackError(stk));
        assert(0);
    }

    if (stk->capacity < 0)
    {
        stk->err = Size;
        printf("Error: %s\n", StackError(stk));
        assert(0);
    }

    if (stk->data == nullptr)
    {
        stk->err = NullPointer;
        printf("Error: %s\n", StackError(stk));
        assert(0);
    }

    #ifdef HASH

    uint64_t new_hash = StackHashFNV(stk);
    if (stk->hash != new_hash)
    {
        stk->err = HashError;
    }
    #endif

    if (stk->err != NoError)
    {
        printf("Error: %s\n", StackError(stk));
        StackDumpMacro(stk);
        assert(0);
    }
}

uint64_t StackHashFNV(Stack* stk)
{
    uint64_t hash = 0x811C9DC5;
    uint64_t HASH_CONST = 0xEDA;

	for (size_t i = 0; i < stk->size; i++)
	{
		hash ^= (uint32_t)stk->data[i];
        hash *= HASH_CONST;
	}
    stk->hash = hash;

    return stk->hash;
}

const char* StackError(Stack* stk)
{
    switch (stk->err)
    {
        case Overflow: return "Stack Overflow";
        case Size: return "Size/Capacity is not correct";
        case Underflow: return "Stack Underflow";
        case NoError: return "Stack is working fine";
        case NullPointer: return "Null Pointer Error";
        case CanaryError: return "Canary was poisoned";
        case HashError: return "Hash is not equal";
        default: return "Unknown Error";
    }
}


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "error.h"

CodeError StackPush(Stack* stk, StackElem_t elem)
{
    StackAssert(stk);

    if (stk->size >= stk->capacity)
    {
        stk->data = SizeUpStack(stk);
        stk->data[stk->size] = elem;
        stk->size++;
    }

    stk->data[stk->size] = elem;
    stk->size++;

    #ifdef CANARY
    StackAssert(stk);
    #endif

    #ifdef HASH
        stk->hash = StackHashFNV(stk);
    #endif

    return stk->err;
}

int StackPop(Stack* stk)
{
    StackAssert(stk);

    StackElem_t DeletedValue = 0;
    if (stk->size > 0)
    {
        DeletedValue = stk->data[--stk->size];
        stk->data[stk->size] = FILL_VALUE;

    if (stk->capacity / stk->size > 2 && stk->size > 0)
            stk->data = SizeDownStack(stk);
    }
    else
    {
        stk->err = Underflow;
    }

    StackAssert(stk);

    #ifdef HASH
        stk->hash = StackHashFNV(stk);
    #endif

    return DeletedValue;
}

void StackCtor(Stack* stk)
{
    #ifdef CANARY
    stk->LEFT_CANARY = CANARY_VALUE;
    stk->RIGHT_CANARY = CANARY_VALUE;

    #endif

    stk->hash = 0;

    stk->capacity = 10;
    int size = 0;

    #ifdef CANARY
        size = stk->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t);
    #else
        size = stk->capacity * sizeof(StackElem_t);
    #endif

    if (stk->capacity == 0)
    {
        stk->data = nullptr;
        stk->err = NullPointer;
    }

    else
    {
        stk->data = (StackElem_t*)calloc(size, sizeof(StackElem_t));

        if (!stk->data)
        {
            stk->err = NullPointer;
        }
    }

    #ifdef CANARY
    stk->data = (StackElem_t*)((Canary_t*)stk->data + 1);

    *((Canary_t*)stk->data - 1) = CANARY_VALUE;
    *((Canary_t*)stk->data + stk->capacity) = CANARY_VALUE;
    #endif

    stk->size = 0;

    stk->err = NoError;

    #ifdef HASH
        stk->hash = StackHashFNV(stk);
    #endif

    StackAssert(stk);
}


void StackDtor(Stack* stk)
{
    if (stk->data)

    #ifdef CANARY
    {
        free((Canary_t*)stk->data - 1);
        stk->data = nullptr;
    }
    #else
    {
        free(stk->data);
        stk->data = nullptr;
    }
    #endif

    stk->size = -1;
    stk->capacity = -1;
}

StackElem_t* SizeUpStack(Stack* stk)
{
    StackAssert(stk);
    if (stk->size > 1000)
        stk->capacity *= 1.05;
    else if (stk->size > 100)
        stk->capacity *= 1.1;
    else
        stk->capacity += 10;

    #ifdef CANARY
        size_t new_size = stk->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t);
        stk->data = (StackElem_t*)realloc((Canary_t*)stk->data - 1, new_size);
    #else
        stk->data = (StackElem_t*)realloc(stk->data, stk->capacity * sizeof(StackElem_t));
    #endif

    if (!stk->data)
        return NULL;

    #ifdef CANARY
        stk->data = (StackElem_t*)((Canary_t*)stk->data + 1);
        *((Canary_t*)stk->data - 1) = CANARY_VALUE;
        *((Canary_t*)stk->data + stk->capacity) = CANARY_VALUE;
    #endif

    for (size_t i = stk->size; i < stk->capacity; i++)
        stk->data[i] = FILL_VALUE;

    #ifdef CANARY
        StackAssert(stk);
    #endif

    #ifdef HASH
        stk->hash = StackHashFNV(stk);
    #endif

    return stk->data;
}


StackElem_t* SizeDownStack(Stack* stk)
{
    if (stk->size > 10)
        stk->capacity *= 0.6;
    else
        stk->capacity -= 1;

    #ifdef CANARY
        size_t new_size = stk->capacity * sizeof(StackElem_t) + 2 * sizeof(Canary_t);
        stk->data = (StackElem_t*)realloc((Canary_t*)stk->data - 1, new_size);
    #else
        stk->data = (StackElem_t*)realloc(stk->data, stk->capacity * sizeof(StackElem_t));
    #endif

    if (!stk->data)
        return NULL;

    #ifdef CANARY
        stk->data = (StackElem_t*)((Canary_t*)stk->data + 1);
        *((Canary_t*)stk->data - 1) = CANARY_VALUE;
        *((Canary_t*)stk->data + stk->capacity) = CANARY_VALUE;
    #endif

    #ifdef HASH
        stk->hash = StackHashFNV(stk);
    #endif

    return stk->data;
}


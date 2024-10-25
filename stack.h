#ifndef STACK_H
#define STACK_H

#include <ctype.h>
#include <stdint.h>

#define StackDumpMacro(stk) (StackDump(stk, __FILE__, __LINE__, __FUNCTION__))

typedef double StackElem_t;
typedef uint64_t Canary_t;

#define CANARY_VALUE 0xDEAD
#define FILL_VALUE -666

// #define CANARY
// #define HASH

enum CodeError {
    Overflow,
    Size,
    Underflow,
    NullPointer,
    CanaryError,
    HashError,
    NoError
};

struct Stack
{
    Canary_t LEFT_CANARY;
    StackElem_t* data;
    int size;
    size_t capacity;
    CodeError err;
    Canary_t RIGHT_CANARY;
    uint64_t hash;
};

CodeError StackPush(Stack* stk, StackElem_t elem);

int StackPop(Stack* stk);

void StackCtor(Stack* stk);

void StackDtor(Stack* stk);

StackElem_t* SizeUpStack(Stack* stk);

StackElem_t* SizeDownStack(Stack* stk);

#endif

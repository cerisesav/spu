#ifndef ERROR_H
#define ERROR_H

const char* StackError(Stack* stk);

void StackAssert(Stack* stk);

void StackDump(struct Stack* stk, const char *file, int line, const char *function);

uint64_t StackHashFNV(Stack* stk); // static

#endif

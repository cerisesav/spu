DEF_CMD(PUSH, 1, 1,
{
    StackPush(&spu->stack, *(int*)(spu->code + spu->ip + sizeof(int)));
    spu->ip += 2 * sizeof(int);
})

DEF_CMD(POP, 2, 0,
{
    StackPop(&spu->stack);
    spu->ip += sizeof(int);
})

DEF_CMD(ADD, 3, 0,
{
    int a = StackPop(&spu->stack);
    int b = StackPop(&spu->stack);
    int result = a + b;
    StackPush(&spu->stack, result);
    spu->ip += sizeof(int);
})

DEF_CMD(SUB, 4, 0,
{
    int a = StackPop(&spu->stack);
    int b = StackPop(&spu->stack);
    int result = a - b;
    StackPush(&spu->stack, result);
    spu->ip += sizeof(int);
})

DEF_CMD(HLT, 12, 0,
{
    spu->ip += sizeof(int);
    // return
})

DEF_CMD(JMP, 5, 1,
{
    int jump_address = *(int*)(spu->code + spu->ip + sizeof(int));

    if (jump_address < 0 || jump_address >= size) {
        printf("Invalid jump address: %d\n", jump_address);
        break;
    }

    spu->ip = jump_address;
})

DEF_CMD(JB, 6, 1,
{
    int jump_address = *(int*)(spu->code + spu->ip + sizeof(int));

    if (jump_address < 0 || jump_address >= size) {
        printf("Invalid jump address: %d\n", jump_address);
        break;
    }

    spu->ip = jump_address;
})

DEF_CMD(JN, 7, 1,
{
    int jump_address = *(int*)(spu->code + spu->ip + sizeof(int));

    if (jump_address < 0 || jump_address >= size) {
        printf("Invalid jump address: %d\n", jump_address);
        break;
    }

    spu->ip = jump_address;
})

DEF_CMD(JE, 8, 1,
{
    int jump_address = *(int*)(spu->code + spu->ip + sizeof(int));

    if (jump_address < 0 || jump_address >= size) {
        printf("Invalid jump address: %d\n", jump_address);
        break;
    }

    spu->ip = jump_address;
})

DEF_CMD(JNE, 9, 1,
{
    int jump_address = *(int*)(spu->code + spu->ip + sizeof(int));

    if (jump_address < 0 || jump_address >= size) {
        printf("Invalid jump address: %d\n", jump_address);
        break;
    }

    spu->ip = jump_address;
})

DEF_CMD(JBE, 10, 1,
{
    int jump_address = *(int*)(spu->code + spu->ip + sizeof(int));

    if (jump_address < 0 || jump_address >= size) {
        printf("Invalid jump address: %d\n", jump_address);
        break;
    }

    spu->ip = jump_address;
})

DEF_CMD(JAE, 11, 1,
{
    int jump_address = *(int*)(spu->code + spu->ip + sizeof(int));

    if (jump_address < 0 || jump_address >= size) {
        printf("Invalid jump address: %d\n", jump_address);
        break;
    }

    spu->ip = jump_address;
})

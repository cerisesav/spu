#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stddef.h>

#define DEF_CMD(name, num, ...) \
  name##_OP = num,              \

enum TokenType {
    OPERATION,
    NUMBER,
    REGISTER,
    MEMORY_BRACKET,
    LABEL,
    LABEL_REFERENCE
};

enum OperationType {
  #include "commands.h"
};

#undef DEF_CMD

struct Label {
    char* name;
    long position;
};

struct LabelTable {
    struct Label* labels;
    size_t count;
};



enum RegisterType {
    RAX_REG,
    RBX_REG,
    RCX_REG,
    RDX_REG
};

enum MemoryBracketType {
    MEMORY_OPEN_BRACKET,
    MEMORY_CLOSE_BRACKET
};

union TokenValue {
    OperationType op_type;
    int number_value;
    char* label_name;
    RegisterType reg_type;
    MemoryBracketType bracket_type;
};

struct Token {
    TokenType type;
    TokenValue value;
};

Token* tokenize_words(char** words_array, size_t words_num);

void translate_assembler_to_binary(const char* input_file_name,
                                   const char* output_file_name);


#endif

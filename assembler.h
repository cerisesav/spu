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
    LABEL
};

enum OperationType {
  #include "commands.h"
};

#undef DEF_CMD

struct Label {
    char* name;
    int position;
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

struct LabelTable* create_label_table();
void add_label(struct LabelTable* table, const char* name, int position);
int find_label(struct LabelTable* table, const char* name);
void free_label_table(struct LabelTable* table);

void translate_assembler_to_binary(const char* input_file_name,
                                   const char* output_file_name);


#endif

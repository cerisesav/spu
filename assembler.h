#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stddef.h>

enum TokenType {
  OPERATION,
  NUMBER,
  LABEL,
  REGISTER,
  MEMORY_BRACKET
};

enum OperationType {
  PUSH_OP = 1,
  POP_OP,
  ADD_OP,
  SUB_OP,
  MUL_OP,
  DIV_OP,
  JMP_OP,
  JA_OP,
  JAE_OP,
  JB_OP,
  JBE_OP,
  JE_OP,
  JNE_OP,

  HLT = -666
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

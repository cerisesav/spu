#include "assembler.h"
#include "tokenizer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static TokenType determine_token_type(const char* word);
static TokenValue determine_token_value(const char* word, TokenType type);

Token* tokenize_words(char** words_array, size_t words_num) {
    Token* tokens = (Token*)calloc(words_num, sizeof(Token));
    assert(tokens);

    for (size_t i = 0; i < words_num; ++i) {
        TokenType type = determine_token_type(words_array[i]);
        TokenValue value = determine_token_value(words_array[i], type);
        tokens[i].type = type;
        tokens[i].value = value;
    }

    return tokens;
}

static TokenType determine_token_type(const char* word) {

    if (strcmp(word, "push") == 0 || strcmp(word, "pop") == 0) {
        return OPERATION;
    }

    if (isdigit(word[0])) {
        return NUMBER;
    }

    if (strcmp(word, "rax") == 0 || strcmp(word, "rbx") == 0) {
        return REGISTER;
    }

    if (strcmp(word, "[") == 0) {
        return MEMORY_BRACKET;
    }
    return LABEL;
}

static TokenValue determine_token_value(const char* word, TokenType type) {
    TokenValue value;

    switch (type) {

        case OPERATION:
            if (strcmp(word, "push") == 0) {
                value.op_type = PUSH_OP;
            } else if (strcmp(word, "pop") == 0) {
                value.op_type = POP_OP;
            }
            break;

        case NUMBER:
            value.number_value = atoi(word);
            break;

        case REGISTER:

            if (strcmp(word, "rax") == 0) {
                value.reg_type = RAX_REG;
            } else if (strcmp(word, "rbx") == 0) {
                value.reg_type = RBX_REG;
            } else if (strcmp(word, "rcx") == 0) {
                value.reg_type = RCX_REG;
            } else if (strcmp(word, "rdx") == 0) {
                value.reg_type = RDX_REG;
            }
            break;

        case MEMORY_BRACKET:

            if (strcmp(word, "[") == 0) {
                value.bracket_type = MEMORY_OPEN_BRACKET;
            } else if (strcmp(word, "]") == 0) {
                value.bracket_type = MEMORY_CLOSE_BRACKET;
            }
            break;
    }

    return value;
}

void translate_assembler_to_binary(const char *input_file_name,
                                   const char *output_file_name) {

    Text tokens_text = tokenize_text_to_word(input_file_name);

    // debug debug
    printf("%zu\n\n", tokens_text.words_num);

    for (size_t i = 0; i < tokens_text.words_num; i++)
    {
        printf("%s\n", tokens_text.words_array[i]);
    }
    //

    struct Token* tokens = tokenize_words(tokens_text.words_array, tokens_text.words_num);

    FILE* out_file = fopen(output_file_name, "wb");
    assert(out_file);

    for (size_t i = 0; i < tokens_text.words_num; ++i) {
        Token current_token = tokens[i];

        switch (current_token.type) {

            case OPERATION:
                fwrite(&current_token.value.op_type, sizeof(OperationType), 1, out_file);

                break;

            case NUMBER:
                fwrite(&current_token.value.number_value, sizeof(int), 1, out_file);

                break;

            case REGISTER:
                fwrite(&current_token.value.reg_type, sizeof(RegisterType), 1, out_file);
                break;

            case MEMORY_BRACKET:
                fwrite(&current_token.value.bracket_type, sizeof(MemoryBracketType), 1, out_file);
                break;
        }
    }

    fclose(out_file);
    destroy_tokenized_text(&tokens_text);
    free(tokens);
}

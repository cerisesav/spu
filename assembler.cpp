#include "assembler.h"
#include "tokenizer.h"
#include "processor.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define LABEL_TABLE_CAPACITY 1000

static TokenType determine_token_type(const char* word);
static TokenValue determine_token_value(const char* word, TokenType type);
static bool is_label_instruction(const Token* token);
static bool is_jump_instruction(int op_type);

Token* tokenize_words(char** words_array, size_t words_num) {
    Token* tokens = (Token*)calloc(words_num, sizeof(Token));
    assert(tokens);

    for (size_t i = 0; i < words_num; ++i) {
        TokenType type = determine_token_type(words_array[i]);
        TokenValue value = determine_token_value(words_array[i], type);
        tokens[i].type = type;
        tokens[i].value = value;

        printf("Token %zu: type = %d, ", i, tokens[i].type);
        if (tokens[i].type == OPERATION) {
            printf("op_type = %d\n", tokens[i].value.op_type);
        } else if (tokens[i].type == NUMBER) {
            printf("number_value = %d\n", tokens[i].value.number_value);
        } else if (tokens[i].type == REGISTER) {
            printf("reg_type = %d\n", tokens[i].value.reg_type);
        } else if (tokens[i].type == LABEL) {
            printf("label_name = %s\n", tokens[i].value.label_name);
        }
    }
    return tokens;
}

static TokenType determine_token_type(const char* word) {
    if (strcmp(word, "push") == 0 || strcmp(word, "pop") == 0 ||
        strcmp(word, "add") == 0  ||  strcmp(word, "sub") == 0 ||
        strcmp(word, "jmp") == 0  ||  strcmp(word, "je") == 0 ||
        strcmp(word, "jne") == 0  ||  strcmp(word, "hlt") == 0 ||
        strcmp(word, "jb") == 0   ||   strcmp(word, "ja") == 0 ||
        strcmp(word, "out") == 0) {
        return OPERATION;
    }

    else if (isdigit(word[0])) {
        return NUMBER;
    }

    else if (strcmp(word, "rax") == 0 || strcmp(word, "rbx") == 0) {
        return REGISTER;
    }

    else if (strcmp(word, "[") == 0) {
        return MEMORY_BRACKET;
    }

    else if (word[strlen(word) - 1] == ':') {
        return LABEL;
    }

    else
    {
        printf("%s\n", word);
        puts("UNKNOWN");
        assert(0);
    }

}

static TokenValue determine_token_value(const char* word, TokenType type) {
    union TokenValue value;

    switch (type) {
        case OPERATION: {
            if (strcmp(word, "push") == 0) {
                value.op_type = PUSH_OP;
            } else if (strcmp(word, "pop") == 0) {
                value.op_type = POP_OP;
            } else if (strcmp(word, "add") == 0) {
                value.op_type = ADD_OP;
            } else if (strcmp(word, "sub") == 0) {
                value.op_type = SUB_OP;
            } else if (strcmp(word, "hlt") == 0) {
                value.op_type = HLT_OP;
            } else if (strcmp(word, "jmp") == 0) {
                value.op_type = JMP_OP;
            } else if (strcmp(word, "jb") == 0) {
                value.op_type = JB_OP;
            } else if (strcmp(word, "ja") == 0) {
                value.op_type = JA_OP;
            } else if (strcmp(word, "out") == 0) {
                value.op_type = OUT_OP;
            }
            break;
        }
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

        // case MEMORY_BRACKET:
        //     if (strcmp(word, "[") == 0) {
        //         value.bracket_type = MEMORY_OPEN_BRACKET;
        //     } else if (strcmp(word, "]") == 0) {
        //         value.bracket_type = MEMORY_CLOSE_BRACKET;
        //     }
        //     break; - problems with tokens [rax] - one word

        case LABEL:
            value.label_name = strdup(word);
            break;

    }

    return value;
}

struct LabelTable* create_label_table() {

    struct LabelTable* table = (LabelTable*) calloc(1, sizeof(struct LabelTable));

    table->count = 0;
    table->labels = (Label*) calloc(LABEL_TABLE_CAPACITY, sizeof(struct Label));

    return table;
}

void add_label(struct LabelTable* table, const char* name, int position) {

    if (find_label(table, name) != -1) {
        return;
    }

    if (table->count > LABEL_TABLE_CAPACITY) {
        printf("%s\n", "Count > capacity");
        assert(0);
    }

    table->labels[table->count].name = strdup(name);

    table->labels[table->count].position = position;

    table->count++;

    printf("Added label: %s at position %d\n", name, position);
    for (size_t i = 0; i < table->count; i++) {
        printf("Label %zu: name = %s, position = %d\n",
               i, table->labels[i].name, table->labels[i].position);
    }

}

int find_label(struct LabelTable* table, const char* name) {
    for (size_t i = 0; i < table->count; i++) {
        if (strcmp(table->labels[i].name, name) == 0) {
            return table->labels[i].position;
        }
    }
    return -1;
}

void free_label_table(struct LabelTable* table) {
    for (size_t i = 0; i < table->count; i++) {
        free(table->labels[i].name);
    }
    free(table->labels);
    free(table);
}

static bool is_jump_instruction(int op_type) {
    return op_type == JMP_OP || op_type == JA_OP || op_type == JB_OP || op_type == JE_OP || op_type == JNE_OP;
}

void translate_assembler_to_binary(const char *input_file_name,
                                   const char *output_file_name) {

    struct Text tokens_text = tokenize_text_to_word(input_file_name);
    size_t words_num = tokens_text.words_num;

    struct Token* tokens = tokenize_words(tokens_text.words_array, words_num);

    struct LabelTable* label_table = create_label_table();

    size_t instruction_address = 0;

    Token* prev_token = nullptr;

    // первый проход
    for (size_t i = 0; i < words_num; ++i) {

        struct Token current_token = tokens[i];

        switch (current_token.type) {

            case LABEL:

            if (prev_token == NULL || prev_token->type != OPERATION) {
                add_label(label_table, current_token.value.label_name, instruction_address);
            }
            break;

            case OPERATION:
                instruction_address += sizeof(int);

                if (is_jump_instruction(current_token.value.op_type)) {
                    instruction_address += sizeof(int);
                }
                break;

            case NUMBER: {
                    instruction_address += sizeof(int);
                    break;
            }

            case REGISTER: {
                    instruction_address += sizeof(char);
                    break;
                }
        }

        prev_token = &tokens[i];
    }


    FILE* out_file = fopen(output_file_name, "wb");
    assert(out_file);

    for (size_t i = 0; i < words_num; ++i) {
        struct Token current_token = tokens[i];

        printf("Writing token %zu: type = %d\n", i, current_token.type);

        switch (current_token.type) {

            case OPERATION:

                printf("Operation: %d\n", current_token.value.op_type);
                fwrite(&current_token.value.op_type, sizeof(int), 1, out_file);

                if (is_jump_instruction(current_token.value.op_type)) {
                    struct Token label_token = tokens[++i];
                    int label_address = find_label(label_table, label_token.value.label_name);
                    fwrite(&label_address, sizeof(int), 1, out_file);
                }
                break;

            case NUMBER:
                printf("Number: %d\n", current_token.value.number_value);
                fwrite(&current_token.value.number_value, sizeof(int), 1, out_file);
                break;

            case REGISTER:
                fwrite(&current_token.value.reg_type, sizeof(char), 1, out_file);
                break;

            case LABEL:
            {
                struct Token label_token = tokens[i];

                if (label_token.type != LABEL) {
                    // fprintf(stderr, "Error: Expected label after jump instruction\n");
                    // return;
                }

                int label_address = find_label(label_table, current_token.value.label_name);

                if (label_address == -1) {
                    fprintf(stderr, "Error: Undefined label '%s'\n", current_token.value.label_name);
                    return;
                }

                fwrite(&label_address, sizeof(int), 1, out_file);
                break;

            }

            default:
                break;
        }
    }

    fclose(out_file);

    destroy_tokenized_text(&tokens_text);
    free(tokens);
    free_label_table(label_table);
}

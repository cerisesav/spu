#include <assert.h>
#include <string.h>
#include "tokenizer.h"

// Space, \n
static const char* SEPARATORS_STRING = "\t \n";

static FileBuffer read_file(const char* file_name);
static size_t get_file_len(FILE* file);
static char* read_file_text(FILE* file, size_t file_len);
static size_t get_words_number(const FileBuffer* text);
static char** tokenize_text(FileBuffer* text, size_t words_num);

// create tokens of words
Text tokenize_text_to_word(const char* file_name) {
    FileBuffer text_buf = read_file(file_name);
    size_t words_num = get_words_number(&text_buf);
    char** words_array = tokenize_text(&text_buf, words_num);

    Text text = {text_buf, words_array, words_num};
    return text;
}

void destroy_tokenized_text(Text* text) {
    assert (text);
    free(text->text.buffer);
    free(text->words_array);
}

static FileBuffer read_file(const char* file_name) {
    FILE* file = fopen(file_name, "rb");
    assert(file);

    size_t file_len = get_file_len(file);
    char* file_text = read_file_text(file, file_len);
    fclose(file);

    FileBuffer text = {file_text, file_len};
    return text;
}

static size_t get_file_len(FILE* file) {
    fseek(file, 0, SEEK_END);
    size_t file_len = ftell(file);
    rewind(file);
    return file_len;
}

static char* read_file_text(FILE* file, size_t file_len) {
    char* file_buf = (char*)calloc(file_len, sizeof(char));
    assert (file_buf);
    fread(file_buf, sizeof(char), file_len, file); // assert
    return file_buf;
}

static char** tokenize_text(FileBuffer* text, size_t words_num) {
    char** words_array = (char**)calloc(words_num, sizeof(char*));
    assert(words_array);

    words_array[0] = strtok(text->buffer, SEPARATORS_STRING);
    for (size_t i = 1; i < words_num; ++i) {
        words_array[i] = strtok(nullptr, SEPARATORS_STRING);
    }

    return words_array;
}

static size_t get_words_number(const FileBuffer* text) { //amount

    size_t buf_len = text->buf_len;
    size_t words_num = 0;

    bool is_prev_sep = true;

    for (size_t i = 0; i < buf_len; ++i) {

        if (strchr(SEPARATORS_STRING, text->buffer[i]) || text->buffer[i] == '\0') {
            is_prev_sep = true;
            continue;
        }

        if (is_prev_sep) {
            ++words_num;
            is_prev_sep = false;
        }

    }
    return words_num;
}

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include <stdlib.h>

struct FileBuffer { // naming
  char* buffer;
  size_t buf_len;
};

struct Text {
  FileBuffer text;
  char** words_array;
  size_t words_num;
};

Text tokenize_text_to_word(const char* file_name);

void destroy_tokenized_text(Text* text);

size_t get_file_len(FILE* file);

#endif

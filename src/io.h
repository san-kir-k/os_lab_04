#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define BUF_SIZE 64
#define RF_ENDL -1
#define RF_EOF 1
#define RF_INVALID 2
#define RF_VALID 0

// проигнорировать невалидную строку
void skip_str();
// проверка числа на валидность
bool is_valid(char* buf);
// считать float с клавиатуры при помощи read
int read_float(float* n);

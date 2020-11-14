#include "io.h"

// проигнорировать невалидную строку
void skip_str() {
    char ch;
    while (read(0, &ch, 1) > 0 && ch != '\n') {}
}

// проверка числа на валидность
bool is_valid(char* buf) {
    int i = 0;
    int count_of_dots = 0;
    while (buf[i] != '\0') {
        if (isdigit(buf[i]) || buf[i] == '.') {
            if (buf[i] == '.') {
                count_of_dots++;
                i++;
            } else {
                i++;
                continue;
            }
        } else {
            return false;
        }
    }
    return (count_of_dots <= 1 && i > 0) ? true : false;
}

// считать float с клавиатуры при помощи read
int read_float(float* n) {
    char ch;
    char buf[100];
    int count = 0;
    // проверка на еоф (read возвращает 0, если еоф)
    int is_eof = 1;
    float sign = 1.0;
    while ((is_eof = read(0, &ch, 1)) > 0 && ch != ' ' && ch != '\n') {
        if (ch == '-') {
            sign *= -1;
        } else {
            buf[count++] = ch;
        }
    }
    if (is_eof == 0) {
        return RF_EOF;
    }
    buf[count] = '\0';
    if (!is_valid(buf)) {
        if (ch != '\n')
            skip_str();
        return RF_INVALID; // строка невалидна
    }
    *n = atof(buf) * sign;
    if (ch == '\n') {
        return RF_ENDL;
    } else {
        return RF_VALID;
    }
}

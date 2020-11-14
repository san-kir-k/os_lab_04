/*
    Составить и отладить программу на языке Си, осуществляющую работу с процессами и взаимодействие между
    ними в одной из двух операционных систем. В результате работы программа (основной процесс) должен
    создать для решение задачи один или несколько дочерних процессов. Взаимодействие между процессами
    осуществляется через системные сигналы/события и/или через отображаемые файлы (memory-mapped files).

    Необходимо обрабатывать системные ошибки, которые могут возникнуть в результате работы.
    4 вариант
*/

#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "io.h"

#define PAGESIZE 4096

#define CHILD_BLOCKED 0
#define PARENT_BLOCKED 1
#define KILL 2

#define FILE_OPEN_ERR 1
#define FORK_ERR 2
#define MMAP_ERR 3
#define CLOSE_FILE_ERR 4
#define NULL_DIV 5

int mmap_clear(float* mf, int* sig) {
    if (munmap(mf, PAGESIZE) == -1) {
        return MMAP_ERR;
    }
    if (munmap(sig, sizeof(int)) == -1) {
        return MMAP_ERR;
    }
    return 0;
}

int child(char* file_name, float* mf, int* sig) {
    FILE* fp;
    if ((fp = fopen(file_name, "w")) == NULL) {
        perror("unable to open file");
        exit(FILE_OPEN_ERR);
    }
    while (*sig == CHILD_BLOCKED) {}
    while (*sig != KILL) {
        int size = mf[0];
        float result = mf[1];
        for (int i = 2; i < size + 1; ++i) {
            if (mf[i] == 0.0) {
                *sig = KILL;
                if (fclose(fp) != 0) {
                    perror("unable to close file");
                    exit(CLOSE_FILE_ERR);
                }
                exit(NULL_DIV);
            }
            result /= mf[i];
        }
        fprintf(fp, "%f\n", result);
        *sig = CHILD_BLOCKED;
        while (*sig == CHILD_BLOCKED) {}
    }
    if (fclose(fp) != 0) {
        perror("unable to close file");
        exit(CLOSE_FILE_ERR);
    }
    exit(0);
}

int parent(float* mf, int* sig) {
    int get_return;
    float num;
    while (true) {
        int it = 0;
        while (true) {
            get_return = read_float(&num);
            if (get_return == RF_INVALID) {
                perror("invalid num");
                continue;
            }
            mf[it + 1] = num;
            it++;
            if (get_return != RF_VALID) {
                break;
            }
        }
        if (get_return == RF_EOF) {
            break;
        }
        mf[0] = it;
        *sig = PARENT_BLOCKED;
        while (*sig == PARENT_BLOCKED) {}
        if (*sig == KILL) {
            if (mmap_clear(mf, sig) == MMAP_ERR) {
                perror("munmap error");
                exit(MMAP_ERR);
            }
            exit(NULL_DIV);
        }
    }
    *sig = KILL;
    if (mmap_clear(mf, sig) == MMAP_ERR) {
        perror("munmap error");
        exit(MMAP_ERR);
    }
    exit(0);
}

int main() {
    char file_name[256];
    scanf("%s", file_name);
    float* mf = (float*)mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mf == MAP_FAILED) {
        perror("mmap error");
        exit(MMAP_ERR);
    }
    int* sig = (int*)mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE , MAP_SHARED | MAP_ANONYMOUS, -1, 0); 
    if (sig == MAP_FAILED) {
        perror("mmap error");
        exit(MMAP_ERR);
    }
    *sig = CHILD_BLOCKED;
    int id = fork();
    if (id < 0) {
        perror("fork error");
        exit(FORK_ERR);
    } else if (id == 0) {
        child(file_name, mf, sig);
    } else {
        parent(mf, sig);
    }
    return 0;
}

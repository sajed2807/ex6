#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInt(const char* prompt) {
    int val;
    if (prompt) printf("%s", prompt);
    if (scanf("%d", &val) != 1) {
        while (getchar() != '\n');
        return -1;
    }
    return val;
}

int compareItems(void* a, void* b) { (void)a; (void)b; return 0; }
void printItem(void* a) { (void)a; }
void freeItem(void* a) { if (a) free(a); }
int compareMonsters(void* a, void* b) { (void)a; (void)b; return 0; }
void printMonster(void* a) { (void)a; }
void freeMonster(void* a) { if (a) free(a); }

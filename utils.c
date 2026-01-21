#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int getInt(const char* prompt) {
    int val;
    printf("%s", prompt);
    scanf("%d", &val);
    return val;
}

int compareItems(void* a, void* b) { return 0; }
void printItem(void* a) {}
void freeItem(void* a) { free(a); }
int compareMonsters(void* a, void* b) { return 0; }
void printMonster(void* a) {}
void freeMonster(void* a) { free(a); }

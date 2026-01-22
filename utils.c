#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInt(const char* prompt) {
    int n;
    printf("%s", prompt);
    scanf("%d", &n);
    return n;
}

char* getString(const char* prompt) {
    char buffer[256];
    printf("%s", prompt);
    scanf(" %255[^\n]", buffer);
    char* s = malloc(strlen(buffer)+1);
    strcpy(s, buffer);
    return s;
}

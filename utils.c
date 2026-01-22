/***************************
* File: utils.c
* Author: Sajed Isa
* ID: 325949089
* Assignment: ex6
* Description: Utility functions for the game
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

int getInt(const char* prompt) {
    printf("%s", prompt);
    int value;
    scanf("%d", &value);
    while (getchar() != '\n');
    return value;
}

char* getString(const char* prompt) {
    printf("%s", prompt);
    char buffer[1024];
    fgets(buffer, sizeof(buffer), stdin);
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len-1] == '\n') {
        buffer[len-1] = '\0';
    }
    
    char* str = (char*)malloc(strlen(buffer) + 1);
    if (str) strcpy(str, buffer);
    return str;
}

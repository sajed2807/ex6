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
#include "game.h"

char* readLine() {
    char buffer[1024];
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return NULL;
    }
    
    size_t length = strlen(buffer);
    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    }
    
    char* line = (char*)malloc(strlen(buffer) + 1);
    if (line == NULL) {
        return NULL;
    }
    
    strcpy(line, buffer);
    return line;
}

Item* createItem(const char* name, ItemType type, int value) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    if (newItem == NULL) {
        return NULL;
    }
    
    newItem->name = (char*)malloc(strlen(name) + 1);
    if (newItem->name == NULL) {
        free(newItem);
        return NULL;
    }
    
    strcpy(newItem->name, name);
    newItem->type = type;
    newItem->value = value;
    
    return newItem;
}

Monster* createMonster(const char* name, MonsterType type, int hp, int attack) {
    Monster* newMonster = (Monster*)malloc(sizeof(Monster));
    if (newMonster == NULL) {
        return NULL;
    }
    
    newMonster->name = (char*)malloc(strlen(name) + 1);
    if (newMonster->name == NULL) {
        free(newMonster);
        return NULL;
    }
    
    strcpy(newMonster->name, name);
    newMonster->type = type;
    newMonster->hp = hp;
    newMonster->maxHp = hp;
    newMonster->attack = attack;
    
    return newMonster;
}

Room* findRoomById(Room* head, int id) {
    Room* current = head;
    
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

Room* findRoomByCoordinates(Room* head, int x, int y) {
    Room* current = head;
    
    while (current != NULL) {
        if (current->x == x && current->y == y) {
            return current;
        }
        current = current->next;
    }
    
    return NULL;
}

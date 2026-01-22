/***************************
* File: game.c
* Author: Sajed Isa
* ID: 325949089
* Assignment: ex6
* Description: Game logic and missing function implementations
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "bst.h"

// Add these functions at the end of your existing game.c file:

int compareItems(void* a, void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    
    int nameComparison = strcmp(itemA->name, itemB->name);
    if (nameComparison != 0) {
        return nameComparison;
    }
    
    if (itemA->value != itemB->value) {
        return itemA->value - itemB->value;
    }
    
    return (int)itemA->type - (int)itemB->type;
}

void printItem(void* data) {
    Item* item = (Item*)data;
    if (item == NULL) {
        return;
    }
    
    const char* typeString;
    if (item->type == ARMOR) {
        typeString = "ARMOR";
    } else {
        typeString = "SWORD";
    }
    
    printf("[%s] %s - Value: %d\n", typeString, item->name, item->value);
}

void freeItem(void* data) {
    Item* item = (Item*)data;
    if (item == NULL) {
        return;
    }
    
    if (item->name != NULL) {
        free(item->name);
    }
    
    free(item);
}

int compareMonsters(void* a, void* b) {
    Monster* monsterA = (Monster*)a;
    Monster* monsterB = (Monster*)b;
    
    int nameComparison = strcmp(monsterA->name, monsterB->name);
    if (nameComparison != 0) {
        return nameComparison;
    }
    
    if (monsterA->attack != monsterB->attack) {
        return monsterA->attack - monsterB->attack;
    }
    
    if (monsterA->hp != monsterB->hp) {
        return monsterA->hp - monsterB->hp;
    }
    
    return (int)monsterA->type - (int)monsterB->type;
}

void printMonster(void* data) {
    Monster* monster = (Monster*)data;
    if (monster == NULL) {
        return;
    }
    
    const char* typeNames[] = {
        "Phantom",
        "Spider",
        "Demon",
        "Golem",
        "Cobra"
    };
    
    printf("[%s] Type: %s, Attack: %d, HP: %d\n",
           monster->name,
           typeNames[monster->type],
           monster->attack,
           monster->hp);
}

void freeMonster(void* data) {
    Monster* monster = (Monster*)data;
    if (monster == NULL) {
        return;
    }
    
    if (monster->name != NULL) {
        free(monster->name);
    }
    
    free(monster);
}

void initPlayer(GameState* game) {
    if (game == NULL) {
        return;
    }
    
    if (game->rooms == NULL) {
        printf("Create rooms first\n");
        return;
    }
    
    game->player = (Player*)malloc(sizeof(Player));
    if (game->player == NULL) {
        return;
    }
    
    game->player->hp = game->configMaxHp;
    game->player->maxHp = game->configMaxHp;
    game->player->baseAttack = game->configBaseAttack;
    
    game->player->bag = createBST(compareItems, printItem, freeItem);
    
    game->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
    
    game->player->currentRoom = game->rooms;
}

void freeGame(GameState* game) {
    if (game == NULL) {
        return;
    }
    
    Room* currentRoom = game->rooms;
    while (currentRoom != NULL) {
        Room* nextRoom = currentRoom->next;
        
        if (currentRoom->monster != NULL) {
            freeMonster(currentRoom->monster);
        }
        
        if (currentRoom->item != NULL) {
            freeItem(currentRoom->item);
        }
        
        free(currentRoom);
        currentRoom = nextRoom;
    }
    
    if (game->player != NULL) {
        if (game->player->bag != NULL) {
            freeBST(game->player->bag);
        }
        
        if (game->player->defeatedMonsters != NULL) {
            freeBST(game->player->defeatedMonsters);
        }
        
        free(game->player);
    }
    
    free(game);
}


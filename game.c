/*
Name: Sajed Isa
ID: 325949089
Assignment: ex6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "bst.h"
#include "utils.h"

/* Helper to read string safely since getStr is undefined */
static char* readString(const char* prompt) {
    char buffer[256];
    printf("%s", prompt);
    if (scanf(" %255[^\n]", buffer) == 1) {
        char* str = (char*)malloc(strlen(buffer) + 1);
        if (str) strcpy(str, buffer);
        return str;
    }
    return NULL;
}

static void freeRoom(Room* r) {
    if (!r) return;
    if (r->monster) {
        if (r->monster->name) free(r->monster->name);
        free(r->monster);
    }
    if (r->item) {
        if (r->item->name) free(r->item->name);
        free(r->item);
    }
    free(r);
}

GameState* game_create(int hp, int atk) {
    GameState* g = (GameState*)malloc(sizeof(GameState));
    if (!g) return NULL;
    g->rooms = NULL;
    g->player = NULL;
    g->roomCount = 0;
    g->configMaxHp = hp;
    g->configBaseAttack = atk;
    return g;
}

void game_main_menu(GameState* g) {
    if (!g) return;
    int choice;
    while (1) {
        printf("=== MENU ===\n1.Add Room\n2.Init Player\n3.Play\n4.Exit\n");
        choice = getInt("Choice: ");
        if (choice == 4) break;
        switch (choice) {
            case 1: addRoom(g); break;
            case 2: initPlayer(g); break;
            case 3: playGame(g); break;
            default: break;
        }
    }
}

void initPlayer(GameState* g) {
    if (!g || g->player) return;
    g->player = (Player*)malloc(sizeof(Player));
    if (!g->player) return;
    g->player->hp = g->configMaxHp;
    g->player->maxHp = g->configMaxHp;
    g->player->baseAttack = g->configBaseAttack;
    g->player->bag = createBST(compareItems, printItem, freeItem);
    g->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
}

void addRoom(GameState* g) {
    if (!g) return;
    Room* r = (Room*)malloc(sizeof(Room));
    if (!r) return;
    r->id = g->roomCount++;
    r->monster = NULL;
    r->item = NULL;
    r->next = g->rooms;
    g->rooms = r;

    int hasMonster = getInt("Add monster? (1=Yes, 0=No): ");
    if (hasMonster) {
        r->monster = (Monster*)malloc(sizeof(Monster));
        /* Using internal readString to avoid implicit declaration error */
        r->monster->name = readString("Monster name: ");
        r->monster->type = getInt("Type (0-4): ");
        r->monster->hp = getInt("HP: ");
        r->monster->attack = getInt("Attack: ");
    }

    int hasItem = getInt("Add item? (1=Yes, 0=No): ");
    if (hasItem) {
        r->item = (Item*)malloc(sizeof(Item));
        r->item->name = readString("Item name: ");
        r->item->type = getInt("Type (0=Armor, 1=Sword): ");
        r->item->value = getInt("Value: ");
    }
    printf("Created room %d\n", r->id);
}

void playGame(GameState* g) {
    if (!g || !g->player || !g->rooms) return;
    printf("--- Room %d ---\n", g->rooms->id);
    if (g->rooms->monster) printf("Monster: %s (HP:%d)\n", g->rooms->monster->name, g->rooms->monster->hp);
    if (g->rooms->item) printf("Item: %s\n", g->rooms->item->name);
    printf("HP: %d/%d\n1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n", g->player->hp, g->player->maxHp);
}

void game_free(GameState* g) {
    if (!g) return;
    if (g->player) {
        if (g->player->bag) {
            bstFree(g->player->bag->root, g->player->bag->freeData);
            free(g->player->bag);
        }
        if (g->player->defeatedMonsters) {
            bstFree(g->player->defeatedMonsters->root, g->player->defeatedMonsters->freeData);
            free(g->player->defeatedMonsters);
        }
        free(g->player);
    }
    Room* curr = g->rooms;
    while (curr) {
        Room* next = curr->next;
        freeRoom(curr);
        curr = next;
    }
    free(g);
}

void freeGame(GameState* g) { game_free(g); }

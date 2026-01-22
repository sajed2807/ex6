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

typedef struct {
    Room* r;
    int x, y;
} RoomPos;

static RoomPos roomData[100];

void printSpatialMap(GameState* g) {
    if (!g || g->roomCount == 0) return;
    printf("=== SPATIAL MAP ===\n");
    for (int i = 0; i < g->roomCount; i++) {
        printf("[%2d]", roomData[i].r->id);
    }
    printf("\n=== ROOM LEGEND ===\n");
    for (int i = g->roomCount - 1; i >= 0; i--) {
        Room* curr = roomData[i].r;
        printf("ID %d: [M:%c] [I:%c]\n", curr->id,
               curr->monster ? 'V' : 'X',
               curr->item ? 'V' : 'X');
    }
    printf("===================\n");
}

static char* readStr(const char* prompt) {
    char buffer[256];
    printf("%s", prompt);
    scanf(" %255[^\n]", buffer);
    char* s = malloc(strlen(buffer) + 1);
    strcpy(s, buffer);
    return s;
}

GameState* game_create(int hp, int atk) {
    GameState* g = malloc(sizeof(GameState));
    g->rooms = NULL;
    g->player = NULL;
    g->roomCount = 0;
    g->configMaxHp = hp;
    g->configBaseAttack = atk;
    return g;
}

void addRoom(GameState* g) {
    int nx = 0, ny = 0;
    if (g->roomCount > 0) {
        printSpatialMap(g);
        int targetId = getInt("Attach to room ID: ");
        int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");

        for(int i = 0; i < g->roomCount; i++) {
            if(roomData[i].r->id == targetId) {
                nx = roomData[i].x;
                ny = roomData[i].y;
                break;
            }
        }

        if (dir == 0) ny++;
        else if (dir == 1) ny--;
        else if (dir == 2) nx--;
        else if (dir == 3) nx++;
    }

    Room* r = malloc(sizeof(Room));
    r->id = g->roomCount;
    r->monster = NULL;
    r->item = NULL;

    if (getInt("Add monster? (1=Yes, 0=No): ")) {
        r->monster = malloc(sizeof(Monster));
        r->monster->name = readStr("Monster name: ");
        r->monster->type = getInt("Type (0-4): ");
        r->monster->hp = getInt("HP: ");
        r->monster->attack = getInt("Attack: ");
    }

    if (getInt("Add item? (1=Yes, 0=No): ")) {
        r->item = malloc(sizeof(Item));
        r->item->name = readStr("Item name: ");
        r->item->type = getInt("Type (0=Armor,1=Sword): ");
        r->item->value = getInt("Value: ");
    }

    roomData[g->roomCount].r = r;
    roomData[g->roomCount].x = nx;
    roomData[g->roomCount].y = ny;

    r->next = g->rooms;
    g->rooms = r;
    g->roomCount++;
}

void initPlayer(GameState* g) {
    if (g->player) return;
    g->player = malloc(sizeof(Player));
    g->player->hp = g->configMaxHp;
    g->player->maxHp = g->configMaxHp;
    g->player->baseAttack = g->configBaseAttack;
    g->player->bag = createBST(compareItems, printItem, freeItem);
    g->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
}

void playGame(GameState* g) {
    if (!g->player || g->roomCount == 0) {
        printf("Init player and add rooms first.\n");
        return;
    }

    int currentIdx = 0;
    while (1) {
        printSpatialMap(g);
        Room* curr = roomData[currentIdx].r;

        printf("--- Room %d ---\n", curr->id);
        if (curr->monster)
            printf("Monster: %s (HP:%d)\n", curr->monster->name, curr->monster->hp);
        if (curr->item)
            printf("Item: %s\n", curr->item->name);
        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);
        printf("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n");

        int choice = getInt("Choice: ");
        if (choice == 6) break;

        if (choice == 2 && curr->monster) {
            while (curr->monster->hp > 0 && g->player->hp > 0) {
                curr->monster->hp -= g->player->baseAttack;
                if (curr->monster->hp > 0)
                    g->player->hp -= curr->monster->attack;
            }
            insertBST(g->player->defeatedMonsters, curr->monster);
            curr->monster = NULL;
        }

        if (choice == 3 && curr->item) {
            insertBST(g->player->bag, curr->item);
            curr->item = NULL;
        }

        if (choice == 4 || choice == 5) {
            BST* t = (choice == 4) ? g->player->bag : g->player->defeatedMonsters;
            int order = getInt("Traversal (1=Preorder,2=Inorder,3=Postorder): ");
            if (order == 1) bstPreorder(t->root, t->print);
            else if (order == 2) bstInorder(t->root, t->print);
            else bstPostorder(t->root, t->print);
        }
    }
}

void freeGame(GameState* g) {
    // Free rooms
    Room* r = g->rooms;
    while (r) {
        Room* next = r->next;
        if (r->monster) freeMonster(r->monster);
        if (r->item) freeItem(r->item);
        free(r);
        r = next;
    }

    // Free player BSTs
    if (g->player) {
        if (g->player->bag) bstFree(g->player->bag->root, g->player->bag->freeData);
        if (g->player->defeatedMonsters) bstFree(g->player->defeatedMonsters->root, g->player->defeatedMonsters->freeData);
        free(g->player);
    }

    free(g);
}


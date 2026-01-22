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

#define MAX_ROOMS 100
static RoomPos roomData[MAX_ROOMS];

/* ===================== MAP PRINT ===================== */
void printSpatialMap(GameState* g) {
    if (!g || g->roomCount == 0) return;

    int minX = roomData[0].x, maxX = roomData[0].x;
    int minY = roomData[0].y, maxY = roomData[0].y;

    for (int i = 1; i < g->roomCount; i++) {
        if (roomData[i].x < minX) minX = roomData[i].x;
        if (roomData[i].x > maxX) maxX = roomData[i].x;
        if (roomData[i].y < minY) minY = roomData[i].y;
        if (roomData[i].y > maxY) maxY = roomData[i].y;
    }

    printf("=== SPATIAL MAP ===\n");
    for (int y = maxY; y >= minY; y--) {
        for (int x = minX; x <= maxX; x++) {
            int found = 0;
            for (int i = 0; i < g->roomCount; i++) {
                if (roomData[i].x == x && roomData[i].y == y) {
                    printf("[%2d]", roomData[i].r->id);
                    found = 1;
                    break;
                }
            }
            if (!found) printf("    ");
        }
        printf("\n");
    }

    printf("=== ROOM LEGEND ===\n");
    for (int i = g->roomCount - 1; i >= 0; i--) {
        Room* r = roomData[i].r;
        printf("ID %d: [M:%c] [I:%c]\n",
               r->id,
               r->monster ? 'V' : 'X',
               r->item ? 'V' : 'X');
    }
    printf("===================\n");
}

/* ===================== UTILS ===================== */
static char* readStr(const char* prompt) {
    char buffer[256];
    printf("%s", prompt);
    scanf(" %255[^\n]", buffer);
    char* s = malloc(strlen(buffer) + 1);
    strcpy(s, buffer);
    return s;
}

/* ===================== GAME CREATE ===================== */
GameState* game_create(int hp, int atk) {
    GameState* g = malloc(sizeof(GameState));
    g->rooms = NULL;
    g->player = NULL;
    g->roomCount = 0;
    g->configMaxHp = hp;
    g->configBaseAttack = atk;
    return g;
}

/* ===================== ADD ROOM ===================== */
void addRoom(GameState* g) {
    int nx = 0, ny = 0;

    if (g->roomCount > 0) {
        printSpatialMap(g);
        int attachId = getInt("Attach to room ID: ");
        int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");

        for (int i = 0; i < g->roomCount; i++) {
            if (roomData[i].r->id == attachId) {
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

    printf("Created room %d at (%d,%d)\n", r->id, nx, ny);
    g->roomCount++;
}

/* ===================== INIT PLAYER ===================== */
void initPlayer(GameState* g) {
    if (g->player) return;

    g->player = malloc(sizeof(Player));
    g->player->hp = g->configMaxHp;
    g->player->maxHp = g->configMaxHp;
    g->player->baseAttack = g->configBaseAttack;
    g->player->bag = createBST(compareItems, printItem, freeItem);
    g->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
}

/* ===================== PLAY GAME ===================== */
void playGame(GameState* g) {
    if (!g->player || g->roomCount == 0) {
        if (!g->player) printf("Init player first.\n");
        if (g->roomCount == 0) printf("Add rooms first.\n");
        return;
    }

    int idx = 0;
    while (1) {
        printSpatialMap(g);
        Room* curr = roomData[idx].r;
        printf("--- Room %d ---\n", curr->id);
        if (curr->monster) printf("Monster: %s (HP:%d)\n", curr->monster->name, curr->monster->hp);
        if (curr->item) printf("Item: %s\n", curr->item->name);
        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);
        printf("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n");

        int choice = getInt("Choice: ");
        if (choice == 6) break;

        if (choice == 1) {
            int d = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
            int nx = roomData[idx].x, ny = roomData[idx].y;
            if (d == 0) ny++;
            else if (d == 1) ny--;
            else if (d == 2) nx--;
            else if (d == 3) nx++;

            int found = 0;
            for (int i = 0; i < g->roomCount; i++) {
                if (roomData[i].x == nx && roomData[i].y == ny) {
                    idx = i;
                    found = 1;
                    break;
                }
            }
            if (!found) printf("No room in that direction.\n");
        }
        else if (choice == 2) {
            if (curr->monster) {
                while (curr->monster->hp > 0 && g->player->hp > 0) {
                    curr->monster->hp -= g->player->baseAttack;
                    if (curr->monster->hp > 0) {
                        g->player->hp -= curr->monster->attack;
                    }
                }
                printf("Monster defeated!\n");
                g->player->defeatedMonsters->root = bstInsert(
                    g->player->defeatedMonsters->root,
                    curr->monster,
                    g->player->defeatedMonsters->compare
                );
                curr->monster = NULL;
            } else printf("No monster here.\n");
        }
        else if (choice == 3) {
            if (curr->item) {
                printf("Picked up %s\n", curr->item->name);
                g->player->bag->root = bstInsert(
                    g->player->bag->root,
                    curr->item,
                    g->player->bag->compare
                );
                curr->item = NULL;
            } else printf("No item here.\n");
        }
        else if (choice == 4 || choice == 5) {
            BST* target = (choice == 4) ? g->player->bag : g->player->defeatedMonsters;
            printf("1.Preorder 2.Inorder 3.Postorder\n");
            int order = getInt("Choice: ");
            if (order == 1) bstPreorder(target->root, target->print);
            else if (order == 2) bstInorder(target->root, target->print);
            else bstPostorder(target->root, target->print);
        }
    }
}

/* ===================== MAIN MENU ===================== */
void game_main_menu(GameState* g) {
    while (1) {
        printf("=== MENU ===\n1.Add Room\n2.Init Player\n3.Play\n4.Exit\n");
        int choice = getInt("Choice: ");
        if (choice == 4) break;
        if (choice == 1) addRoom(g);
        else if (choice == 2) initPlayer(g);
        else if (choice == 3) playGame(g);
    }
}

/* ===================== FREE GAME ===================== */
void freeGame(GameState* g) {
    if (!g) return;

    Room* r = g->rooms;
    while (r) {
        Room* next = r->next;
        if (r->monster) freeMonster(r->monster);
        if (r->item) freeItem(r->item);
        free(r);
        r = next;
    }

    if (g->player) {
        if (g->player->bag) bstFree(g->player->bag->root, g->player->bag->freeData);
        if (g->player->defeatedMonsters) bstFree(g->player->defeatedMonsters->root, g->player->defeatedMonsters->freeData);
        free(g->player);
    }

    free(g);
}



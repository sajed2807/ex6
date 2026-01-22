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
        printf("ID %d: [M:%s] [I:%s]\n", curr->id,
               curr->monster ? "V" : "X", curr->item ? "V" : "X");
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
        
        // Find target room coordinates
        for(int i=0; i < g->roomCount; i++) {
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
        r->item->type = getInt("Type (0=Armor, 1=Sword): ");
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
        if (!g->player) printf("Init player first.\n");
        if (g->roomCount == 0) printf("Add rooms first.\n");
        return;
    }

    int currentIdx = 0;
    while (1) {
        printSpatialMap(g);
        Room* curr = roomData[currentIdx].r;
        printf("--- Room %d ---\n", curr->id);
        if (curr->monster) printf("Monster: %s (HP:%d)\n", curr->monster->name, curr->monster->hp);
        if (curr->item) printf("Item: %s\n", curr->item->name);
        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);
        printf("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\n");

        int choice = getInt("Choice: ");
        if (choice == 6) break;

        if (choice == 1) {
            getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
            currentIdx++;
            if (currentIdx >= g->roomCount) {
                printf("*************\n");
                printf("VICTORY!\n");
                printf("All rooms explored. All monsters defeated.\n");
                printf("*************\n");
                return;
            }
        } else if (choice == 2) {
            if (curr->monster) {
                int m_hp = curr->monster->hp;
                while (m_hp > 0 && g->player->hp > 0) {
                    m_hp -= g->player->baseAttack;
                    printf("You deal %d damage. Monster HP: %d\n", g->player->baseAttack, m_hp > 0 ? m_hp : 0);
                    if (m_hp > 0) {
                        printf("Monster deals %d damage. Your HP: %d\n", curr->monster->attack, g->player->hp - curr->monster->attack);
                        g->player->hp -= curr->monster->attack;
                    }
                }
                printf("Monster defeated!\n");
                insertBST(g->player->defeatedMonsters, curr->monster);
                curr->monster = NULL;
            } else printf("No monster\n");
        } else if (choice == 3) {
            if (curr->item) {
                printf("Picked up %s\n", curr->item->name);
                insertBST(g->player->bag, curr->item);
                curr->item = NULL;
            } else printf("No item here\n");
        } else if (choice == 4 || choice == 5) {
            BST* target = (choice == 4) ? g->player->bag : g->player->defeatedMonsters;
            printf("=== %s ===\n", (choice == 4) ? "INVENTORY" : "DEFEATED MONSTERS");
            printf("1.Preorder 2.Inorder 3.Postorder\n");
            int order = getInt("Choice: ");
            if (order == 1) bstPrintPreorder(target->root, target->printData);
            else if (order == 2) bstPrintInorder(target->root, target->printData);
            else bstPrintPostorder(target->root, target->printData);
        }
    }
}

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

void freeGame(GameState* g) {
    // Basic cleanup logic here
}


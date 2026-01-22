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

#define MAX_ROOMS 100

typedef struct {
    Room* room;
    int x, y;
} RoomPos;

static RoomPos rooms[MAX_ROOMS];

/* ===================== UTIL ===================== */
static RoomPos* findRoomById(int id, int count) {
    for (int i = 0; i < count; i++)
        if (rooms[i].room->id == id) return &rooms[i];
    return NULL;
}

static RoomPos* findRoomByXY(int x, int y, int count) {
    for (int i = 0; i < count; i++)
        if (rooms[i].x == x && rooms[i].y == y) return &rooms[i];
    return NULL;
}

/* ===================== PRINT MAP ===================== */
void printSpatialMap(GameState* g) {
    if (!g || g->roomCount == 0) return;

    int minX = rooms[0].x, maxX = rooms[0].x;
    int minY = rooms[0].y, maxY = rooms[0].y;

    for (int i = 1; i < g->roomCount; i++) {
        if (rooms[i].x < minX) minX = rooms[i].x;
        if (rooms[i].x > maxX) maxX = rooms[i].x;
        if (rooms[i].y < minY) minY = rooms[i].y;
        if (rooms[i].y > maxY) maxY = rooms[i].y;
    }

    printf("=== SPATIAL MAP ===\n");
    for (int y = maxY; y >= minY; y--) {
        for (int x = minX; x <= maxX; x++) {
            RoomPos* rp = findRoomByXY(x, y, g->roomCount);
            if (rp) printf("[%2d]", rp->room->id);
            else printf("    ");
        }
        printf("\n");
    }

    printf("=== ROOM LEGEND ===\n");
    for (int i = g->roomCount - 1; i >= 0; i--) {
        Room* r = rooms[i].room;
        printf("ID %d: [M:%c] [I:%c]\n",
               r->id,
               r->monster ? 'V' : 'X',
               r->item ? 'V' : 'X');
    }
    printf("===================\n");
}

/* ===================== GAME ===================== */
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
    int x = 0, y = 0;

    if (g->roomCount > 0) {
        int attach = getInt("Attach to room ID: ");
        int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");

        RoomPos* base = findRoomById(attach, g->roomCount);
        if (!base) return;

        x = base->x;
        y = base->y;
        if (dir == 0) y++;
        else if (dir == 1) y--;
        else if (dir == 2) x--;
        else if (dir == 3) x++;
    }

    Room* r = malloc(sizeof(Room));
    r->id = g->roomCount;
    r->monster = NULL;
    r->item = NULL;

    if (getInt("Add monster? (1=Yes, 0=No): ")) {
        r->monster = malloc(sizeof(Monster));
        r->monster->name = getString("Monster name: ");
        r->monster->type = getInt("Type (0-4): ");
        r->monster->hp = getInt("HP: ");
        r->monster->attack = getInt("Attack: ");
    }

    if (getInt("Add item? (1=Yes, 0=No): ")) {
        r->item = malloc(sizeof(Item));
        r->item->name = getString("Item name: ");
        r->item->type = getInt("Type (0=Armor, 1=Sword): ");
        r->item->value = getInt("Value: ");
    }

    rooms[g->roomCount].room = r;
    rooms[g->roomCount].x = x;
    rooms[g->roomCount].y = y;

    r->next = g->rooms;
    g->rooms = r;

    printf("Created room %d at (%d,%d)\n", r->id, x, y);
    g->roomCount++;

    printSpatialMap(g);
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
        printf("Init player first.\n");
        printf("Add rooms first.\n");
        return;
    }

    int x = 0, y = 0;

    while (1) {
        printSpatialMap(g);
        RoomPos* rp = findRoomByXY(x, y, g->roomCount);
        Room* r = rp->room;

        printf("--- Room %d ---\n", r->id);
        if (r->monster)
            printf("Monster: %s (HP:%d)\n", r->monster->name, r->monster->hp);
        if (r->item)
            printf("Item: %s\n", r->item->name);

        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);
        printf("1.Move\n2.Fight\n3.Pickup\n4.Bag\n5.Defeated\n6.Quit\n");

        int c = getInt("Choice: ");
        if (c == 6) return;

        if (c == 1) {
            int d = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
            int nx = x, ny = y;
            if (d == 0) ny++;
            else if (d == 1) ny--;
            else if (d == 2) nx--;
            else if (d == 3) nx++;

            if (findRoomByXY(nx, ny, g->roomCount)) {
                x = nx;
                y = ny;
            } else {
                printf("No room in that direction.\n");
            }
        }
        else if (c == 2 && r->monster) {
            while (r->monster->hp > 0 && g->player->hp > 0) {
                r->monster->hp -= g->player->baseAttack;
                printf("You deal %d damage. Monster HP: %d\n",
                       g->player->baseAttack,
                       r->monster->hp > 0 ? r->monster->hp : 0);
                if (r->monster->hp > 0) {
                    g->player->hp -= r->monster->attack;
                    printf("Monster deals %d damage. Your HP: %d\n",
                           r->monster->attack,
                           g->player->hp);
                }
            }

            printf("Monster defeated!\n");
            g->player->defeatedMonsters->root =
                bstInsert(g->player->defeatedMonsters->root,
                          r->monster,
                          g->player->defeatedMonsters->compare);
            r->monster = NULL;
        }
        else if (c == 3 && r->item) {
            printf("Picked up %s\n", r->item->name);
            g->player->bag->root =
                bstInsert(g->player->bag->root,
                          r->item,
                          g->player->bag->compare);
            r->item = NULL;
        }
        else if (c == 4 || c == 5) {
            BST* t = (c == 4) ? g->player->bag : g->player->defeatedMonsters;
            printf("1.Preorder\n2.Inorder\n3.Postorder\n");
            int o = getInt("Choice: ");

            if (o == 1)
                bstPreorder(t->root, t->print);
            else if (o == 2)
                bstInorder(t->root, t->print);
            else
                bstPostorder(t->root, t->print);
        }
    }
}

void game_main_menu(GameState* g) {
    while (1) {
        printf("=== MENU ===\n1.Add Room\n2.Init Player\n3.Play\n4.Exit\n");
        int c = getInt("Choice: ");
        if (c == 4) return;
        if (c == 1) addRoom(g);
        else if (c == 2) initPlayer(g);
        else if (c == 3) playGame(g);
    }
}




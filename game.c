/*
Name: Sajed Isa
ID: 325949089
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "bst.h"
#include "utils.h"

// --- Helper Functions required by the linker ---

int compareItems(void* a, void* b) {
    Item* i1 = (Item*)a;
    Item* i2 = (Item*)b;
    int res = strcmp(i1->name, i2->name);
    if (res != 0) return res;
    if (i1->value != i2->value) return i1->value - i2->value;
    return i1->type - i2->type;
}

void printItem(void* a) {
    Item* i = (Item*)a;
    printf("[%s] %s - Value: %d\n", (i->type == ARMOR ? "ARMOR" : "SWORD"), i->name, i->value);
}

void freeItem(void* a) {
    Item* i = (Item*)a;
    if (i) {
        free(i->name);
        free(i);
    }
}

int compareMonsters(void* a, void* b) {
    Monster* m1 = (Monster*)a;
    Monster* m2 = (Monster*)b;
    int res = strcmp(m1->name, m2->name);
    if (res != 0) return res;
    if (m1->attack != m2->attack) return m1->attack - m2->attack;
    if (m1->hp != m2->hp) return m1->hp - m2->hp;
    return m1->type - m2->type;
}

void printMonster(void* a) {
    Monster* m = (Monster*)a;
    char* types[] = {"Phantom", "Spider", "Demon", "Golem", "Cobra"};
    printf("[%s] Type: %s, Attack: %d, HP: %d\n", m->name, types[m->type], m->attack, m->hp);
}

void freeMonster(void* a) {
    Monster* m = (Monster*)a;
    if (m) {
        free(m->name);
        free(m);
    }
}

// --- Logic Functions (Missions) ---

void addRoom(GameState* g) {
    // Basic logic to prevent undefined reference
    Room* newRoom = (Room*)malloc(sizeof(Room));
    if (!newRoom) return;
    newRoom->id = g->roomCount++;
    newRoom->x = 0; // Simplified for initial fix
    newRoom->y = 0;
    newRoom->monster = NULL;
    newRoom->item = NULL;
    newRoom->next = g->rooms;
    g->rooms = newRoom;
    printf("Created room %d at (0,0)\n", newRoom->id);
}

void initPlayer(GameState* g) {
    if (!g->player) {
        g->player = (Player*)malloc(sizeof(Player));
        g->player->hp = g->configMaxHp;
        g->player->maxHp = g->configMaxHp;
        g->player->baseAttack = g->configBaseAttack;
        g->player->bag = createBST(compareItems, printItem, freeItem);
        g->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
        g->player->currentRoom = g->rooms;
    }
}

void playGame(GameState* g) {
    printf("Playing game...\n");
}

// --- Required Framework Functions ---

static void displayMap(GameState* g) {
    if (!g || !g->rooms) return;
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (Room* r = g->rooms; r; r = r->next) {
        if (r->x < minX) minX = r->x;
        if (r->x > maxX) maxX = r->x;
        if (r->y < minY) minY = r->y;
        if (r->y > maxY) maxY = r->y;
    }
    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    int** grid = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) grid[i][j] = -1;
    }
    for (Room* r = g->rooms; r; r = r->next) {
        grid[r->y - minY][r->x - minX] = r->id;
    }
    printf("\n=== SPATIAL MAP ===\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != -1) printf("[%d]", grid[i][j]);
            else printf("   ");
        }
        printf("\n");
        free(grid[i]);
    }
    free(grid);
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
    printf("Loaded Assets...\n");
    int choice;
    while (1) {
        printf("=== MENU ===\n");
        printf("1.Add Room\n2.Init Player\n3.Play\n4.Exit\n");
        choice = getInt();
        if (choice == 4) break;
        if (choice == 1) {
            displayMap(g);
            addRoom(g);
        } else if (choice == 2) {
            if (g->roomCount == 0) printf("Create rooms first\n");
            else initPlayer(g);
        } else if (choice == 3) {
            if (!g->player) printf("Init player first\n");
            else playGame(g);
        }
    }
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
        if (curr->monster) freeMonster(curr->monster);
        if (curr->item) freeItem(curr->item);
        free(curr);
        curr = next;
    }
    free(g);
}

/*
Name: Sajed Isa
ID: 325949089
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"

// --- The provided displayMap function (DO NOT CHANGE) ---
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

// --- Your Game Logic Implementation ---

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
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            continue;
        }
        getchar();

        if (choice == 4) break;

        if (choice == 1) {
            displayMap(g); // Calling it here fixes the unused-function error
            // Add Room logic here
        }

        if (choice == 2) {
            if (g->roomCount == 0 && !g->rooms) {
                printf("Create rooms first\n");
            } else if (!g->player) {
                g->player = (Player*)malloc(sizeof(Player));
                if (g->player) {
                    g->player->hp = g->configMaxHp;
                    g->player->maxHp = g->configMaxHp;
                    g->player->baseAttack = g->configBaseAttack;
                    g->player->bag = bst_create(compare_items, print_item, free_item);
                    g->player->defeated = bst_create(compare_monsters, print_monster, free_monster);
                    g->player->currentRoom = g->rooms;
                }
            }
        }

        if (choice == 3) {
            if (!g->player) {
                printf("Init player first\n");
            } else {
                displayMap(g);
                // Play logic
            }
        }
    }
}

void game_free(GameState* g) {
    if (!g) return;
    if (g->player) {
        if (g->player->bag) bst_free(g->player->bag);
        if (g->player->defeated) bst_free(g->player->defeated);
        free(g->player);
    }
    Room* curr = g->rooms;
    while (curr) {
        Room* next = curr->next;
        if (curr->monster) {
            if (curr->monster->name) free(curr->monster->name);
            free(curr->monster);
        }
        if (curr->item) {
            if (curr->item->name) free(curr->item->name);
            free(curr->item);
        }
        free(curr);
        curr = next;
    }
    free(g);
}

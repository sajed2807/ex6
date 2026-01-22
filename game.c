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
        choice = getInt("Enter choice: ");
        if (choice == 4) break;
        if (choice == 2 && !g->player) {
            g->player = (Player*)malloc(sizeof(Player));
            if (g->player) {
                g->player->hp = g->configMaxHp;
                g->player->maxHp = g->configMaxHp;
                g->player->baseAttack = g->configBaseAttack;
                /* Matches createBST signature in bst.h */
                g->player->bag = createBST(compareItems, printItem, freeItem);
                g->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
            }
        }
    }
}

void game_free(GameState* g) {
    if (!g) return;
    if (g->player) {
        if (g->player->bag) {
            /* Passes root and freeData function pointer */
            bstFree(g->player->bag->root, g->player->bag->freeData);
            free(g->player->bag);
        }
        if (g->player->defeatedMonsters) {
            bstFree(g->player->defeatedMonsters->root, g->player->defeatedMonsters->freeData);
            free(g->player->defeatedMonsters);
        }
        free(g->player);
    }
    free(g);
}

void freeGame(GameState* g) { game_free(g); }
void addRoom(GameState* g) { (void)g; }
void initPlayer(GameState* g) { (void)g; }
void playGame(GameState* g) { (void)g; }

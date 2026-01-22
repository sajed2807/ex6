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

/* Real logic needs to be called here to match expected output */
extern void addRoomLogic(GameState* g); // Assuming this is defined elsewhere
extern void playLevel(GameState* g);    // Assuming this is your gameplay loop

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
        printf("=== MENU ===\n");
        printf("1.Add Room\n");
        printf("2.Init Player\n");
        printf("3.Play\n");
        printf("4.Exit\n");

        choice = getInt("Choice: ");

        if (choice == 4) break;

        switch (choice) {
            case 1:
                addRoom(g);
                break;
            case 2:
                initPlayer(g);
                break;
            case 3:
                playGame(g);
                break;
            default:
                break;
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

void playGame(GameState* g) {
    if (!g || !g->player || !g->rooms) return;
    /* You must call your actual game loop here to produce the 
       "--- Room X ---" and "1.Move 2.Fight..." output */
    playLevel(g); 
}

void addRoom(GameState* g) {
    if (!g) return;
    /* This must call the logic that asks for Monster name, HP, etc. */
    addRoomLogic(g); 
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

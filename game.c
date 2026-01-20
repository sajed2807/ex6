#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "utils.h"

// Map display functions
static void displayMap(GameState* g) {
    if (!g->rooms) return;
    
    // Find bounds
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    for (Room* r = g->rooms; r; r = r->next) {
        if (r->x < minX) minX = r->x;
        if (r->x > maxX) maxX = r->x;
        if (r->y < minY) minY = r->y;
        if (r->y > maxY) maxY = r->y;
    }
    
    int width = maxX - minX + 1;
    int height = maxY - minY + 1;
    
    // Create grid
    int** grid = malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) grid[i][j] = -1;
    }
    
    for (Room* r = g->rooms; r; r = r->next)
        grid[r->y - minY][r->x - minX] = r->id;
    
    printf("=== SPATIAL MAP ===\n");
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != -1) printf("[%2d]", grid[i][j]);
            else printf("    ");
        }
        printf("\n");
    }
    
    for (int i = 0; i < height; i++) free(grid[i]);
    free(grid);
}
/*
Name: Sajed Isa
ID: 325949089
*/

#include "game.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

GameState* game_create(int hp, int atk){
    GameState* g = malloc(sizeof(GameState));
    g->rooms = NULL;
    g->player = NULL;
    g->roomCount = 0;
    g->maxHp = hp;
    g->baseAttack = atk;
    return g;
}

void game_main_menu(GameState* g){
    printf("Loaded Assets...\n");
    while (1){
        printf("1.Add Room\n2.Init Player\n3.Play\n4.Exit\n");
        int c; scanf("%d",&c); getchar();
        if (c==4) return;
        if (c==2 && !g->player){
            g->player = malloc(sizeof(Player));
            g->player->hp = g->maxHp;
            g->player->baseAttack = g->baseAttack;
            g->player->bag = bst_create(compare_items, print_item, free_item);
            g->player->defeated = bst_create(compare_monsters, print_monster, free_monster);
            g->player->room = g->rooms;
        }
    }
}

void game_free(GameState* g){
    if (g->player){
        bst_free(g->player->bag);
        bst_free(g->player->defeated);
        free(g->player);
    }
    while (g->rooms){
        Room* n = g->rooms->next;
        if (g->rooms->monster) free_monster(g->rooms->monster);
        if (g->rooms->item) free_item(g->rooms->item);
        free(g->rooms);
        g->rooms = n;
    }
    free(g);
}

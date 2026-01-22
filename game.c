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

int compareItems(void* a, void* b) {
    Item i1 = (Item)a, i2 = (Item)b;
    int res = strcmp(i1->name, i2->name);
    if (res != 0) return res;
    if (i1->value != i2->value) return i1->value - i2->value;
    return i1->type - i2->type;
}

void printItem(void* data) {
    Item* i = (Item*)data;
    printf("[%s] %s - Value: %d\n", i->type == ARMOR ? "ARMOR" : "SWORD", i->name, i->value);
}

void freeItem(void* data) {
    Item* i = (Item*)data;
    if (i) { free(i->name); free(i); }
}

int compareMonsters(void* a, void* b) {
    Monster m1 = (Monster)a, m2 = (Monster)b;
    int res = strcmp(m1->name, m2->name);
    if (res != 0) return res;
    if (m1->attack != m2->attack) return m1->attack - m2->attack;
    if (m1->hp != m2->hp) return m1->hp - m2->hp;
    return m1->type - m2->type;
}

void printMonster(void* data) {
    Monster* m = (Monster*)data;
    char* typeStr[] = {"PHANTOM", "SPIDER", "DEMON", "GOLEM", "COBRA"};
    printf("[%s] Type: %s, Attack: %d, HP: %d\n", m->name, typeStr[m->type], m->attack, m->hp);
}

void freeMonster(void* data) {
    Monster* m = (Monster*)data;
    if (m) { free(m->name); free(m); }
}

void addRoom(GameState* g) {
    int nx = 0, ny = 0;
    if (g->roomCount > 0) {
        displayMap(g);
        int targetId = getInt("Attach to room ID: ");
        int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
        
        Room* temp = g->rooms;
        while(temp && temp->id != targetId) temp = temp->next;
        
        nx = temp->x; ny = temp->y;
        if (dir == 0) ny--; [span_6](start_span)// Up[span_6](end_span)
        else if (dir == 1) ny++; [span_7](start_span)// Down[span_7](end_span)
        else if (dir == 2) nx--; [span_8](start_span)// Left[span_8](end_span)
        else if (dir == 3) nx++; [span_9](start_span)// Right[span_9](end_span)

        Room* check = g->rooms;
        while(check) {
            if (check->x == nx && check->y == ny) {
                printf("Room exists there\n"); [span_10](start_span)//[span_10](end_span)
                return;
            }
            check = check->next;
        }
    }

    Room* r = (Room*)malloc(sizeof(Room));
    r->id = g->roomCount++;
    r->x = nx; r->y = ny; r->visited = 0;
    r->monster = NULL; r->item = NULL;
    r->next = g->rooms;
    g->rooms = r;
}

void freeGame(GameState* g) {
    if (!g) return;
    if (g->player) {
        bstFree(g->player->bag->root, g->player->bag->freeData);
        free(g->player->bag);
        bstFree(g->player->defeatedMonsters->root, g->player->defeatedMonsters->freeData);
        free(g->player->defeatedMonsters);
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

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

// Comparison Functions
int compareItems(void* a, void* b) {
    Item i1 = (Item)a;
    Item i2 = (Item)b;
    int res = strcmp(i1->name, i2->name);
    if (res != 0) return res;
    if (i1->value != i2->value) return i1->value - i2->value;
    return (int)i1->type - (int)i2->type;
}

int compareMonsters(void* a, void* b) {
    Monster m1 = (Monster)a;
    Monster m2 = (Monster)b;
    int res = strcmp(m1->name, m2->name);
    if (res != 0) return res;
    if (m1->attack != m2->attack) return m1->attack - m2->attack;
    if (m1->hp != m2->hp) return m1->hp - m2->hp;
    return (int)m1->type - (int)m2->type;
}

void printItem(void* data) {
    Item* i = (Item*)data;
    printf("[%s] %s - Value: %d\n", i->type == ARMOR ? "ARMOR" : "SWORD", i->name, i->value);
}

void printMonster(void* data) {
    Monster* m = (Monster*)data;
    char* typeStr[] = {"PHANTOM", "SPIDER", "DEMON", "GOLEM", "COBRA"};
    printf("[%s] Type: %s, Attack: %d, HP: %d\n", m->name, typeStr[m->type], m->attack, m->hp);
}

void freeItem(void* data) {
    Item* i = (Item*)data;
    if (i) { free(i->name); free(i); }
}

void freeMonster(void* data) {
    Monster* m = (Monster*)data;
    if (m) { free(m->name); free(m); }
}

GameState* game_create(int hp, int atk) {
    GameState* g = (GameState*)malloc(sizeof(GameState));
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
        displayMap(g);
        int targetId = getInt("Attach to room ID: ");
        int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
        Room* temp = g->rooms;
        while(temp && temp->id != targetId) temp = temp->next;
        if (temp) {
            nx = temp->x; ny = temp->y;
            if (dir == 0) ny--; else if (dir == 1) ny++;
            else if (dir == 2) nx--; else if (dir == 3) nx++;
        }
        Room* check = g->rooms;
        while(check) {
            if (check->x == nx && check->y == ny) { printf("Room exists there\n"); return; }
            check = check->next;
        }
    }
    Room* r = (Room*)malloc(sizeof(Room));
    r->id = g->roomCount++;
    r->x = nx; r->y = ny; r->visited = 0;
    r->monster = NULL; r->item = NULL;
    if (getInt("Add monster? (1=Yes, 0=No): ")) {
        r->monster = (Monster*)malloc(sizeof(Monster));
        r->monster->name = getStr("Monster name: ");
        r->monster->type = getInt("Type (0-4): ");
        r->monster->hp = getInt("HP: ");
        r->monster->maxHp = r->monster->hp;
        r->monster->attack = getInt("Attack: ");
    }
    if (getInt("Add item? (1=Yes, 0=No): ")) {
        r->item = (Item*)malloc(sizeof(Item));
        r->item->name = getStr("Item name: ");
        r->item->type = getInt("Type (0=Armor, 1=Sword): ");
        r->item->value = getInt("Value: ");
    }
    r->next = g->rooms;
    g->rooms = r;
    printf("Created room %d at (%d,%d)\n", r->id, r->x, r->y);
}

void initPlayer(GameState* g) {
    if (g->roomCount == 0) { printf("Create rooms first\n"); return; }
    if (g->player) return;
    g->player = (Player*)malloc(sizeof(Player));
    g->player->hp = g->configMaxHp;
    g->player->maxHp = g->configMaxHp;
    g->player->baseAttack = g->configBaseAttack;
    g->player->bag = createBST(compareItems, printItem, freeItem);
    g->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
    Room* temp = g->rooms;
    while(temp && temp->id != 0) temp = temp->next;
    g->player->currentRoom = temp;
}

void playGame(GameState* g) {
    if (!g->player) { printf("Init player first\n"); return; }
    while (1) {
        displayMap(g);
        Room* r = g->player->currentRoom;
        r->visited = 1;
        printf("--- Room %d ---\n", r->id);
        if (r->monster) printf("Monster: %s (HP:%d)\n", r->monster->name, r->monster->hp);
        if (r->item) printf("Item: %s\n", r->item->name);
        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);
        int choice = getInt("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\nChoice: ");
        if (choice == 6) break;
        if (choice == 1) {
            if (r->monster) { printf("Kill monster first\n"); continue; }
            int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
            int tx = r->x, ty = r->y;
            if (dir == 0) ty--; else if (dir == 1) ty++;
            else if (dir == 2) tx--; else if (dir == 3) tx++;
            Room* target = g->rooms;
            while(target && (target->x != tx || target->y != ty)) target = target->next;
            if (target) g->player->currentRoom = target;
            else printf("No room there\n");
        } else if (choice == 2) {
            if (!r->monster) { printf("No monster\n"); continue; }
            while(r->monster->hp > 0 && g->player->hp > 0) {
                r->monster->hp -= g->player->baseAttack;
                if (r->monster->hp <= 0) break;
                g->player->hp -= r->monster->attack;
            }
            if (g->player->hp <= 0) { printf("--- YOU DIED ---\n"); exit(0); }
            printf("Monster defeated!\n");
            g->player->defeatedMonsters->root = bstInsert(g->player->defeatedMonsters->root, r->monster, g->player->defeatedMonsters->compare);
            r->monster = NULL;
        } else if (choice == 3) {
            if (r->item) {
                g->player->bag->root = bstInsert(g->player->bag->root, r->item, g->player->bag->compare);
                r->item = NULL;
            }
        } else if (choice == 4 || choice == 5) {
            BST* tree = (choice == 4) ? g->player->bag : g->player->defeatedMonsters;
            int order = getInt("1.Preorder 2.Inorder 3.Postorder\nChoice: ");
            if (order == 1) bstPreorder(tree->root, tree->print);
            else if (order == 2) bstInorder(tree->root, tree->print);
            else bstPostorder(tree->root, tree->print);
        }
    }
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


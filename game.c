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

[span_8](start_span)// Comparison logic for items[span_8](end_span)
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

[span_9](start_span)// Comparison logic for monsters[span_9](end_span)
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
    char* typeStr[] = {"Phantom", "Spider", "Demon", "Golem", "Cobra"};
    printf("[%s] Type: %s, Attack: %d, HP: %d\n", m->name, typeStr[m->type], m->attack, m->hp);
}

void freeMonster(void* data) {
    Monster* m = (Monster*)data;
    if (m) { free(m->name); free(m); }
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
        [span_10](start_span)displayMap(g);[span_10](end_span)
        [span_11](start_span)int targetId = getInt("Attach to room ID: ");[span_11](end_span)
        [span_12](start_span)int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");[span_12](end_span)
        
        Room* temp = g->rooms;
        while(temp && temp->id != targetId) temp = temp->next;
        
        nx = temp->x; ny = temp->y;
        [span_13](start_span)if (dir == 0) ny--;[span_13](end_span)
        [span_14](start_span)else if (dir == 1) ny++;[span_14](end_span)
        [span_15](start_span)else if (dir == 2) nx--;[span_15](end_span)
        [span_16](start_span)else if (dir == 3) nx++;[span_16](end_span)

        Room* check = g->rooms;
        while(check) {
            if (check->x == nx && check->y == ny) {
                [span_17](start_span)printf("Room exists there\n");[span_17](end_span)
                return;
            }
            check = check->next;
        }
    }

    Room* r = malloc(sizeof(Room));
    r->id = g->roomCount++;
    r->x = nx; r->y = ny; r->visited = 0;
    r->monster = NULL; r->item = NULL;

    [span_18](start_span)if (getInt("Add monster? (1=Yes, 0=No): ")) {[span_18](end_span)
        r->monster = malloc(sizeof(Monster));
        r->monster->name = getStr("Monster name: ");
        r->monster->type = getInt("Type (0-4): ");
        r->monster->hp = getInt("HP: ");
        r->monster->maxHp = r->monster->hp;
        r->monster->attack = getInt("Attack: ");
    }

    [span_19](start_span)if (getInt("Add item? (1=Yes, 0=No): ")) {[span_19](end_span)
        r->item = malloc(sizeof(Item));
        r->item->name = getStr("Item name: ");
        r->item->type = getInt("Type (0=Armor, 1=Sword): ");
        r->item->value = getInt("Value: ");
    }

    r->next = g->rooms;
    g->rooms = r;
    [span_20](start_span)printf("Created room %d at (%d,%d)\n", r->id, r->x, r->y);[span_20](end_span)
}

void initPlayer(GameState* g) {
    if (g->roomCount == 0) { printf("Create rooms first\n"); return; [span_21](start_span)}
    if (g->player) return;
    g->player = malloc(sizeof(Player));
    g->player->hp = g->configMaxHp;[span_21](end_span)
    g->player->maxHp = g->configMaxHp;
    g->player->baseAttack = g->configBaseAttack;
    g->player->bag = createBST(compareItems, printItem, freeItem);
    g->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
    
    Room* temp = g->rooms;
    while(temp && temp->id != 0) temp = temp->next;
    g->player->currentRoom = temp;
}

void playGame(GameState* g) {
    if (!g->player) { printf("Init player first\n"); return; [span_22](start_span)}
    
    while(1) {
        displayMap(g);
        Room* r = g->player->currentRoom;
        r->visited = 1;

        printf("--- Room %d ---\n", r->id);
        if (r->monster) printf("Monster: %s (HP:%d)\n", r->monster->name, r->monster->hp);
        if (r->item) printf("Item: %s\n", r->item->name);
        printf("HP: %d/%d\n", g->player->hp, g->player->maxHp);

        int choice = getInt("1.Move 2.Fight 3.Pickup 4.Bag 5.Defeated 6.Quit\nChoice: ");[span_22](end_span)
        if (choice == 6) break;

        [span_23](start_span)if (choice == 1) { // Move[span_23](end_span)
            if (r->monster) { printf("Kill monster first\n"); continue; }
            int dir = getInt("Direction (0=Up,1=Down,2=Left,3=Right): ");
            int tx = r->x, ty = r->y;
            if (dir == 0) ty--; else if (dir == 1) ty++;
            else if (dir == 2) tx--; else if (dir == 3) tx++;
            
            Room* target = g->rooms;
            while(target && (target->x != tx || target->y != ty)) target = target->next;
            if (target) g->player->currentRoom = target;
            else printf("No room there\n");
        } 
        [span_24](start_span)else if (choice == 2) { // Fight[span_24](end_span)
            if (!r->monster) { printf("No monster\n"); continue; }
            while(r->monster->hp > 0 && g->player->hp > 0) {
                [span_25](start_span)r->monster->hp -= g->player->baseAttack;[span_25](end_span)
                printf("You deal %d damage. Monster HP: %d\n", g->player->baseAttack, r->monster->hp > 0 ? r->monster->hp : 0);
                if (r->monster->hp <= 0) break;
                [span_26](start_span)g->player->hp -= r->monster->attack;[span_26](end_span)
                printf("Monster deals %d damage. Your HP: %d\n", r->monster->attack, g->player->hp > 0 ? g->player->hp : 0);
            }
            if (g->player->hp <= 0) { printf("--- YOU DIED ---\n"); exit(0); [span_27](start_span)}
            printf("Monster defeated!\n");
            insertBST(g->player->defeatedMonsters, r->monster);
            r->monster = NULL;
        }
    }
}

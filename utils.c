#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
/*
Name: Sajed Isa
ID: 325949089
*/

#include "utils.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* read_line(void){
    char* s = malloc(256);
    fgets(s, 256, stdin);
    s[strcspn(s, "\n")] = 0;
    return s;
}

int compare_items(void* a, void* b){
    Item* x = a; Item* y = b;
    int c = strcmp(x->name, y->name);
    if (c) return c;
    if (x->value != y->value) return x->value - y->value;
    return x->type - y->type;
}

void print_item(void* a){
    Item* i = a;
    printf("%s %d\n", i->name, i->value);
}

void free_item(void* a){
    Item* i = a;
    free(i->name);
    free(i);
}

int compare_monsters(void* a, void* b){
    Monster* x = a; Monster* y = b;
    int c = strcmp(x->name, y->name);
    if (c) return c;
    if (x->attack != y->attack) return x->attack - y->attack;
    return x->hp - y->hp;
}

void print_monster(void* a){
    Monster* m = a;
    printf("%s %d\n", m->name, m->hp);
}

void free_monster(void* a){
    Monster* m = a;
    free(m->name);
    free(m);
}

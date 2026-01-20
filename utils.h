#ifndef UTILS_H
#define UTILS_H

int getInt(const char* prompt);
char* getString(const char* prompt);

#endif
/*
Name: Sajed Isa
ID: 325949089
*/

#ifndef UTILS_H
#define UTILS_H

char* read_line(void);

int compare_items(void*, void*);
void print_item(void*);
void free_item(void*);

int compare_monsters(void*, void*);
void print_monster(void*);
void free_monster(void*);

#endif

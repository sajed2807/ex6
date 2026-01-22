/***************************
* File: game.c
* Author: Sajed Isa
* ID: 325949089
* Assignment: ex6
* Description: Game implementation with all required functions
***************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "bst.h"
#include "utils.h"

// Item comparison function
int compareItems(void* a, void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    
    int nameCmp = strcmp(itemA->name, itemB->name);
    if (nameCmp != 0) return nameCmp;
    
    if (itemA->value != itemB->value) return itemA->value - itemB->value;
    
    return (int)itemA->type - (int)itemB->type;
}

// Item print function
void printItem(void* data) {
    Item* item = (Item*)data;
    if (!item) return;
    
    const char* typeStr = (item->type == ARMOR) ? "ARMOR" : "SWORD";
    printf("[%s] %s - Value: %d\n", typeStr, item->name, item->value);
}

// Item free function
void freeItem(void* data) {
    Item* item = (Item*)data;
    if (!item) return;
    
    if (item->name) free(item->name);
    free(item);
}

// Monster comparison function
int compareMonsters(void* a, void* b) {
    Monster* monsterA = (Monster*)a;
    Monster* monsterB = (Monster*)b;
    
    int nameCmp = strcmp(monsterA->name, monsterB->name);
    if (nameCmp != 0) return nameCmp;
    
    if (monsterA->attack != monsterB->attack) return monsterA->attack - monsterB->attack;
    
    if (monsterA->hp != monsterB->hp) return monsterA->hp - monsterB->hp;
    
    return (int)monsterA->type - (int)monsterB->type;
}

// Monster print function
void printMonster(void* data) {
    Monster* monster = (Monster*)data;
    if (!monster) return;
    
    const char* typeNames[] = {"Phantom", "Spider", "Demon", "Golem", "Cobra"};
    printf("[%s] Type: %s, Attack: %d, HP: %d\n",
           monster->name, typeNames[monster->type], monster->attack, monster->hp);
}

// Monster free function
void freeMonster(void* data) {
    Monster* monster = (Monster*)data;
    if (!monster) return;
    
    if (monster->name) free(monster->name);
    free(monster);
}

// Function to display spatial map
void printSpatialMap(GameState* game) {
    if (!game || !game->rooms) return;
    
    printf("=== SPATIAL MAP ===\n");
    // Simple implementation - you need to complete this based on coordinates
    Room* current = game->rooms;
    while (current) {
        printf("Room %d at (%d, %d)\n", current->id, current->x, current->y);
        current = current->next;
    }
}

// Function to print room legend
void printRoomLegend(GameState* game) {
    if (!game || !game->rooms) return;
    
    printf("=== ROOM LEGEND ===\n");
    Room* current = game->rooms;
    
    // Simple implementation - need to sort by ID descending
    while (current) {
        printf("ID %d: [M:%c] [I:%c]\n", 
               current->id,
               current->monster ? 'V' : 'X',
               current->item ? 'V' : 'X');
        current = current->next;
    }
}

// Initialize player
void initPlayer(GameState* game) {
    if (!game) return;
    
    if (!game->rooms) {
        printf("Create rooms first\n");
        return;
    }
    
    if (game->player) {
        printf("Player already initialized\n");
        return;
    }
    
    game->player = (Player*)malloc(sizeof(Player));
    if (!game->player) return;
    
    game->player->hp = game->configMaxHp;
    game->player->maxHp = game->configMaxHp;
    game->player->baseAttack = game->configBaseAttack;
    
    game->player->bag = createBST(compareItems, printItem, freeItem);
    game->player->defeatedMonsters = createBST(compareMonsters, printMonster, freeMonster);
    game->player->currentRoom = game->rooms;
    
    printf("Player initialized with HP: %d, Attack: %d\n", 
           game->player->hp, game->player->baseAttack);
}

// Add room function (basic implementation)
void addRoom(GameState* game) {
    if (!game) return;
    
    // Create new room
    Room* newRoom = (Room*)malloc(sizeof(Room));
    if (!newRoom) return;
    
    newRoom->id = game->roomCount;
    newRoom->x = 0;
    newRoom->y = 0;
    newRoom->visited = 0;
    newRoom->monster = NULL;
    newRoom->item = NULL;
    newRoom->next = NULL;
    
    // Add to linked list
    if (!game->rooms) {
        game->rooms = newRoom;
    } else {
        Room* current = game->rooms;
        while (current->next) {
            current = current->next;
        }
        current->next = newRoom;
    }
    
    game->roomCount++;
    
    // Ask for monster
    printf("Add monster? (1 = Yes, 0 = No): ");
    int addMonster = getInt("");
    if (addMonster == 1) {
        char* name = getString("Monster name: ");
        printf("Type (0-4): ");
        int type = getInt("");
        printf("HP: ");
        int hp = getInt("");
        printf("Attack: ");
        int attack = getInt("");
        
        newRoom->monster = createMonster(name, type, hp, attack);
        free(name);
    }
    
    // Ask for item
    printf("Add item? (1 = Yes, 0 = No): ");
    int addItem = getInt("");
    if (addItem == 1) {
        char* name = getString("Item name: ");
        printf("Type (0 = Armor, 1 = Sword): ");
        int type = getInt("");
        printf("Value: ");
        int value = getInt("");
        
        newRoom->item = createItem(name, type, value);
        free(name);
    }
    
    printf("Created room %d at (%d, %d)\n", newRoom->id, newRoom->x, newRoom->y);
}

// Play game function (basic implementation)
void playGame(GameState* game) {
    if (!game || !game->player || !game->player->currentRoom) return;
    
    int choice;
    do {
        printf("\n");
        printSpatialMap(game);
        printf("\n");
        printRoomLegend(game);
        printf("\n");
        
        Room* currentRoom = game->player->currentRoom;
        printf("Room %d -- ", currentRoom->id);
        if (currentRoom->monster) {
            printf("Monster: %s (HP:%d) ", 
                   currentRoom->monster->name, currentRoom->monster->hp);
        }
        if (currentRoom->item) {
            printf("Item: %s ", currentRoom->item->name);
        }
        printf("HP: %d/%d\n", game->player->hp, game->player->maxHp);
        
        printf("\n1. Move\n");
        printf("2. Fight\n");
        printf("3. Pickup\n");
        printf("4. Bag\n");
        printf("5. Defeated\n");
        printf("6. Quit\n");
        printf("Choice: ");
        
        choice = getInt("");
        
        switch (choice) {
            case 1: // Move
                printf("Direction (0=Up, 1=Down, 2=Left, 3=Right): ");
                int dir = getInt("");
                printf("Move to direction %d\n", dir);
                break;
            case 2: // Fight
                if (!currentRoom->monster) {
                    printf("No monster\n");
                } else {
                    printf("Fighting monster...\n");
                }
                break;
            case 3: // Pickup
                if (!currentRoom->item) {
                    printf("No item here\n");
                } else {
                    printf("Picked up %s\n", currentRoom->item->name);
                    insertBST(game->player->bag, currentRoom->item);
                    currentRoom->item = NULL;
                }
                break;
            case 4: // Bag
                printf("\n=== INVENTORY ===\n");
                printf("1. Preorder\n");
                printf("2. Inorder\n");
                printf("3. Postorder\n");
                printf("Choice: ");
                int order = getInt("");
                switch (order) {
                    case 1: printPreOrder(game->player->bag); break;
                    case 2: printInOrder(game->player->bag); break;
                    case 3: printPostOrder(game->player->bag); break;
                    default: printf("Invalid choice\n");
                }
                break;
            case 5: // Defeated
                printf("\n=== DEFEATED MONSTERS ===\n");
                printf("1. Preorder\n");
                printf("2. Inorder\n");
                printf("3. Postorder\n");
                printf("Choice: ");
                int defeatOrder = getInt("");
                switch (defeatOrder) {
                    case 1: printPreOrder(game->player->defeatedMonsters); break;
                    case 2: printInOrder(game->player->defeatedMonsters); break;
                    case 3: printPostOrder(game->player->defeatedMonsters); break;
                    default: printf("Invalid choice\n");
                }
                break;
            case 6: // Quit
                printf("Returning to main menu\n");
                break;
            default:
                printf("Invalid choice\n");
        }
    } while (choice != 6);
}

// Free game function
void freeGame(GameState* game) {
    if (!game) return;
    
    // Free all rooms
    Room* currentRoom = game->rooms;
    while (currentRoom) {
        Room* nextRoom = currentRoom->next;
        
        // Free monster if exists
        if (currentRoom->monster) {
            freeMonster(currentRoom->monster);
        }
        
        // Free item if exists
        if (currentRoom->item) {
            freeItem(currentRoom->item);
        }
        
        free(currentRoom);
        currentRoom = nextRoom;
    }
    
    // Free player
    if (game->player) {
        // Free bag BST
        if (game->player->bag) {
            freeBST(game->player->bag);
        }
        
        // Free defeated monsters BST
        if (game->player->defeatedMonsters) {
            freeBST(game->player->defeatedMonsters);
        }
        
        free(game->player);
    }
    
    free(game);
}

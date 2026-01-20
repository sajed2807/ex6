#include <stdlib.h>
#include "bst.h"
/*
Name: Sajed Isa
ID: 325949089
*/

#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    void* data;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct {
    BSTNode* root;
    int (compare)(void, void*);
    void (print)(void);
    void (freeData)(void);
} BST;

typedef enum { PREORDER = 1, INORDER, POSTORDER } TraversalOrder;

BST* bst_create(int (cmp)(void, void*),
                void (print)(void),
                void (freeData)(void));

int bst_insert(BST* tree, void* data);
void bst_traverse(BST* tree, TraversalOrder order);
void bst_free(BST* tree);

#endif

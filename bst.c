#include <stdlib.h>
#include "bst.h"
/*
Name: Sajed Isa
ID: 325949089
*/

#include "bst.h"
#include <stdlib.h>

static BSTNode* create_node(void* data){
    BSTNode* n = malloc(sizeof(BSTNode));
    n->data = data;
    n->left = n->right = NULL;
    return n;
}

BST* bst_create(int (cmp)(void, void*),
                void (print)(void),
                void (freeData)(void)) {
    BST* t = malloc(sizeof(BST));
    t->root = NULL;
    t->compare = cmp;
    t->print = print;
    t->freeData = freeData;
    return t;
}

static int insert_rec(BSTNode** root, void* data, BST* t){
    if (!*root){
        *root = create_node(data);
        return 1;
    }
    int c = t->compare(data, (*root)->data);
    if (c == 0) return 0;
    if (c < 0) return insert_rec(&(*root)->left, data, t);
    return insert_rec(&(*root)->right, data, t);
}

int bst_insert(BST* tree, void* data){
    return insert_rec(&tree->root, data, tree);
}

static void traverse(BSTNode* r, void (print)(void), int o){
    if (!r) return;
    if (o == PREORDER) print(r->data);
    traverse(r->left, print, o);
    if (o == INORDER) print(r->data);
    traverse(r->right, print, o);
    if (o == POSTORDER) print(r->data);
}

void bst_traverse(BST* t, TraversalOrder o){
    traverse(t->root, t->print, o);
}

static void free_rec(BSTNode* r, void (fd)(void)){
    if (!r) return;
    free_rec(r->left, fd);
    free_rec(r->right, fd);
    fd(r->data);
    free(r);
}

void bst_free(BST* t){
    free_rec(t->root, t->freeData);
    free(t);
}

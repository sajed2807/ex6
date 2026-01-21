#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    void* data;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

typedef struct {
    BSTNode* root;
    int (*compare)(void*, void*);
    void (*print)(void*);
    void (*freeData)(void*);
} BST;

BST* createBST(int (*cmp)(void*, void*), void (*print)(void*), void (*freeData)(void*));
BSTNode* bstInsert(BSTNode* root, void* data, int (*cmp)(void*, void*));
void* bstFind(BSTNode* root, void* data, int (*cmp)(void*, void*));
void bstInorder(BSTNode* root, void (*print)(void*));
void bstPreorder(BSTNode* root, void (*print)(void*));
void bstPostorder(BSTNode* root, void (*print)(void*));
void bstFree(BSTNode* root, void (*freeData)(void*));

#endif
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

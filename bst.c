#include "bst.h"
#include <stdlib.h>

BSTNode* bstInsert(BSTNode* root, void* data, int (*cmp)(void*, void*)) {
    if (!root) {
        BSTNode* n = malloc(sizeof(BSTNode));
        n->data = data;
        n->left = n->right = NULL;
        return n;
    }
    if (cmp(data, root->data) < 0)
        root->left = bstInsert(root->left, data, cmp);
    else
        root->right = bstInsert(root->right, data, cmp);
    return root;
}

void insertBST(BST* tree, void* data) {
    if (!tree) return;
    tree->root = bstInsert(tree->root, data, tree->compare);
}

void bstInorder(BSTNode* root, void (*print)(void*)) {
    if (!root) return;
    bstInorder(root->left, print);
    print(root->data);
    bstInorder(root->right, print);
}

void bstPreorder(BSTNode* root, void (*print)(void*)) {
    if (!root) return;
    print(root->data);
    bstPreorder(root->left, print);
    bstPreorder(root->right, print);
}

void bstPostorder(BSTNode* root, void (*print)(void*)) {
    if (!root) return;
    bstPostorder(root->left, print);
    bstPostorder(root->right, print);
    print(root->data);
}

void bstFree(BSTNode* root, void (*freeData)(void*)) {
    if (!root) return;
    bstFree(root->left, freeData);
    bstFree(root->right, freeData);
    if (freeData && root->data) freeData(root->data);
    free(root);
}

void* bstFind(BSTNode* root, void* data, int (*cmp)(void*, void*)) {
    if (!root) return NULL;
    int res = cmp(data, root->data);
    if (res == 0) return root->data;
    if (res < 0) return bstFind(root->left, data, cmp);
    return bstFind(root->right, data, cmp);
}

BST* createBST(int (*cmp)(void*, void*), void (*print)(void*), void (*freeData)(void*)) {
    BST* tree = malloc(sizeof(BST));
    tree->root = NULL;
    tree->compare = cmp;
    tree->print = print;
    tree->freeData = freeData;
    return tree;
}


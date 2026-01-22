#include "bst.h"
#include <stdlib.h>

BST* createBST(int (*cmp)(void*, void*), void (*print)(void*), void (*freeData)(void*)) {
    BST* bst = malloc(sizeof(BST));
    if (!bst) return NULL;
    bst->root = NULL;
    bst->compare = cmp;
    bst->print = print;
    bst->freeData = freeData;
    return bst;
}

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

void* bstFind(BSTNode* root, void* data, int (*cmp)(void*, void*)) {
    if (!root) return NULL;
    int c = cmp(data, root->data);
    if (c == 0) return root->data;
    if (c < 0) return bstFind(root->left, data, cmp);
    return bstFind(root->right, data, cmp);
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


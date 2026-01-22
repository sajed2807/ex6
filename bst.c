#include "bst.h"
#include <stdlib.h>

BST* createBST(int (*cmp)(void*, void*),
               void (*print)(void*),
               void (*freeData)(void*)) {
    BST* bst = malloc(sizeof(BST));
    bst->root = NULL;
    bst->compare = cmp;
    bst->print = print;
    bst->freeData = freeData;
    return bst;
}

void insertBST(BST* bst, void* data) {
    if (!bst) return;

    BSTNode* node = malloc(sizeof(BSTNode));
    node->data = data;
    node->left = node->right = NULL;

    if (!bst->root) {
        bst->root = node;
        return;
    }

    BSTNode* current = bst->root;
    while (1) {
        if (bst->compare(data, current->data) < 0) {
            if (!current->left) { current->left = node; break; }
            else current = current->left;
        } else {
            if (!current->right) { current->right = node; break; }
            else current = current->right;
        }
    }
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
    if (freeData) freeData(root->data);
    free(root);
}

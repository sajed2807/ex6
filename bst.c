#include "bst.h"
#include <stdlib.h>
#include <stdio.h>

BST* createBST(int (*cmp)(void*, void*),
               void (*print)(void*),
               void (*freeData)(void*)) 
{
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
        BSTNode* node = malloc(sizeof(BSTNode));
        if (!node) return NULL;
        node->data = data;
        node->left = node->right = NULL;
        return node;
    }

    int res = cmp(data, root->data);
    if (res < 0) root->left = bstInsert(root->left, data, cmp);
    else if (res > 0) root->right = bstInsert(root->right, data, cmp);
    return root;
}

void* bstFind(BSTNode* root, void* data, int (*cmp)(void*, void*)) {
    if (!root) return NULL;
    int res = cmp(data, root->data);
    if (res == 0) return root->data;
    else if (res < 0) return bstFind(root->left, data, cmp);
    else return bstFind(root->right, data, cmp);
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

void insertBST(BST* tree, void* data) {
    if (!tree) return;
    tree->root = bstInsert(tree->root, data, tree->compare);
}


/***************************
* File: bst.c
* Author: Sajed Isa
* ID: 325949089
* Assignment: ex6
* Description: Implementation of Binary Search Tree functions
***************************/

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

static BSTNode* createNode(void* data) {
    BSTNode* node = (BSTNode*)malloc(sizeof(BSTNode));
    if (!node) return NULL;
    node->data = data;
    node->left = node->right = NULL;
    return node;
}

BST* createBST(int (cmp)(void, void*), void (print)(void), void (freeData)(void)) {
    BST* tree = (BST*)malloc(sizeof(BST));
    if (!tree) return NULL;
    tree->root = NULL;
    tree->compare = cmp;
    tree->print = print;
    tree->freeData = freeData;
    return tree;
}
  
BSTNode* bstInsert(BSTNode* root, void* data, int (cmp)(void, void*)) {
    if (root == NULL) return createNode(data);
    int res = cmp(data, root->data);
    if (res < 0)
        root->left = bstInsert(root->left, data, cmp);
    else if (res > 0)
        root->right = bstInsert(root->right, data, cmp);
    return root;
}

void bstPreorder(BSTNode* root, void (print)(void)) {
    if (!root) return;
    print(root->data);
    bstPreorder(root->left, print);
    bstPreorder(root->right, print);
}

void bstInorder(BSTNode* root, void (print)(void)) {
    if (!root) return;
    bstInorder(root->left, print);
    print(root->data);
    bstInorder(root->right, print);
}

void bstPostorder(BSTNode* root, void (print)(void)) {
    if (!root) return;
    bstPostorder(root->left, print);
    bstPostorder(root->right, print);
    print(root->data);
}

void bstFree(BSTNode* root, void (freeData)(void)) {
    if (!root) return;
    bstFree(root->left, freeData);
    bstFree(root->right, freeData);
    if (freeData) freeData(root->data);
    free(root);
}

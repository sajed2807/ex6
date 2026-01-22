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

static BSTNode* insertHelper(BSTNode* root, void* data, int (cmp)(void, void*)) {
    if (root == NULL) return createNode(data);
    int res = cmp(data, root->data);
    if (res < 0)
        root->left = insertHelper(root->left, data, cmp);
    else if (res > 0)
        root->right = insertHelper(root->right, data, cmp);
    return root;
}

void insertBST(BST* tree, void* data) {
    if (!tree) return;
    tree->root = insertHelper(tree->root, data, tree->compare);
}

void bstPrintPreorder(BSTNode* root, void (print)(void)) {
    if (!root) return;
    print(root->data);
    bstPrintPreorder(root->left, print);
    bstPrintPreorder(root->right, print);
}

void bstPrintInorder(BSTNode* root, void (print)(void)) {
    if (!root) return;
    bstPrintInorder(root->left, print);
    print(root->data);
    bstPrintInorder(root->right, print);
}

void bstPrintPostorder(BSTNode* root, void (print)(void)) {
    if (!root) return;
    bstPrintPostorder(root->left, print);
    bstPrintPostorder(root->right, print);
    print(root->data);
}

static void destroyNodes(BSTNode* root, void (freeData)(void)) {
    if (!root) return;
    destroyNodes(root->left, freeData);
    destroyNodes(root->right, freeData);
    if (freeData) freeData(root->data);
    free(root);
}

void destroyBST(BST* tree) {
    if (!tree) return;
    destroyNodes(tree->root, tree->freeData);
    free(tree);
}

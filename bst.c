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
    node->left = NULL;
    node->right = NULL;
    return node;
}

static BSTNode* insertHelper(BSTNode* root, void* data, int (compare)(void, void*)) {
    if (!root) return createNode(data);
    
    int cmp = compare(data, root->data);
    if (cmp < 0) {
        root->left = insertHelper(root->left, data, compare);
    } else if (cmp > 0) {
        root->right = insertHelper(root->right, data, compare);
    }
    return root;
}

static void printPreOrderHelper(BSTNode* root, void (print)(void)) {
    if (!root) return;
    print(root->data);
    printPreOrderHelper(root->left, print);
    printPreOrderHelper(root->right, print);
}

static void printInOrderHelper(BSTNode* root, void (print)(void)) {
    if (!root) return;
    printInOrderHelper(root->left, print);
    print(root->data);
    printInOrderHelper(root->right, print);
}

static void printPostOrderHelper(BSTNode* root, void (print)(void)) {
    if (!root) return;
    printPostOrderHelper(root->left, print);
    printPostOrderHelper(root->right, print);
    print(root->data);
}

static void freeTreeHelper(BSTNode* root, void (freeData)(void)) {
    if (!root) return;
    freeTreeHelper(root->left, freeData);
    freeTreeHelper(root->right, freeData);
    if (freeData) freeData(root->data);
    free(root);
}

BST* createBST(int (compare)(void, void*), void (print)(void), void (freeData)(void)) {
    BST* tree = (BST*)malloc(sizeof(BST));
    if (!tree) return NULL;
    
    tree->root = NULL;
    tree->compare = compare;
    tree->print = print;
    tree->freeData = freeData;
    return tree;
}

void insertBST(BST* tree, void* data) {
    if (!tree || !data) return;
    tree->root = insertHelper(tree->root, data, tree->compare);
}

void printPreOrder(BST* tree) {
    if (!tree || !tree->print) return;
    printPreOrderHelper(tree->root, tree->print);
}

void printInOrder(BST* tree) {
    if (!tree || !tree->print) return;
    printInOrderHelper(tree->root, tree->print);
}

void printPostOrder(BST* tree) {
    if (!tree || !tree->print) return;
    printPostOrderHelper(tree->root, tree->print);
}

void freeBST(BST* tree) {
    if (!tree) return;
    freeTreeHelper(tree->root, tree->freeData);
    free(tree);
}

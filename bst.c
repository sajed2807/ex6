#include "bst.h"
#include <stdlib.h>

// Fixed function pointers to match bst.h exactly
BST* createBST(int (compare)(void, void*), void (print)(void), void (freeData)(void)) {
    BST* bst = (BST*)malloc(sizeof(BST));
    if (!bst) return NULL;
    bst->root = NULL;
    bst->compare = compare;
    bst->print = print;
    bst->freeData = freeData;
    return bst;
}

void bstFree(BSTNode* root, void (freeData)(void)) {
    if (!root) return;
    bstFree(root->left, freeData);
    bstFree(root->right, freeData);
    if (freeData && root->data) {
        freeData(root->data);
    }
    free(root);
}

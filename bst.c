#include "bst.h"
#include <stdlib.h>

/* Matches bst.h line 17: int (cmp)(void, void*), void (print)(void), void (freeData)(void) */
BST* createBST(int (cmp)(void, void*), void (print)(void), void (freeData)(void)) {
    BST* bst = (BST*)malloc(sizeof(BST));
    if (!bst) return NULL;
    bst->root = NULL;
    bst->compare = cmp;
    bst->print = print;
    bst->freeData = freeData;
    return bst;
}

/* Matches bst.h line 23: void (freeData)(void) */
void bstFree(BSTNode* root, void (freeData)(void)) {
    if (!root) return;
    bstFree(root->left, freeData);
    bstFree(root->right, freeData);
    if (freeData && root->data) {
        freeData(root->data);
    }
    free(root);
}

#include "bst.h"
#include <stdlib.h>

BST* createBST(CompareFunc comp, PrintFunc print, FreeFunc freeD) {
    BST* bst = malloc(sizeof(BST));
    bst->root = NULL;
    bst->compare = comp;
    bst->print = print;
    bst->freeData = freeD;
    return bst;
}

void bstFree(BSTNode* root, FreeFunc freeD) {
    if (!root) return;
    bstFree(root->left, freeD);
    bstFree(root->right, freeD);
    freeD(root->data);
    free(root);
}

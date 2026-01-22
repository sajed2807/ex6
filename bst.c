#include "bst.h"
#include <stdlib.h>
#include <stddef.h>

/* createBST:
 * cmp      - comparison function (void*, void*)
 * print    - print data
 * freeData - free data
 */
BST* createBST(int (cmp)(void, void*),
               void (print)(void),
               void (freeData)(void)) 
{
    BST* bst = malloc(sizeof(BST));
    if (bst == NULL) {
        return NULL;
    }

    bst->root = NULL;
    bst->compare = cmp;
    bst->print = print;
    bst->freeData = freeData;

    return bst;
}

/* Recursively frees BST nodes */
void bstFree(BSTNode* root, void (freeData)(void)) 
{
    if (root == NULL) {
        return;
    }

    bstFree(root->left, freeData);
    bstFree(root->right, freeData);

    if (freeData != NULL && root->data != NULL) {
        freeData(root->data);
    }

    free(root);
}

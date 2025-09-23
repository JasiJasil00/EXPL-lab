#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

struct tnode* createTree(int val, int type, char* c, int nodetype, struct tnode *l, struct tnode *r) {
    struct tnode *temp = (struct tnode*)malloc(sizeof(struct tnode));
    temp->val = val;
    temp->type = type;
    temp->nodetype = nodetype;
    temp->left = l;
    temp->right = r;

    if (c != NULL) {
        temp->varname = (char*)malloc(strlen(c) + 1);
        strcpy(temp->varname, c);
    } else {
        temp->varname = NULL;
    }
    return temp;
}

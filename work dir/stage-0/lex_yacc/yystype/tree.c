#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

static char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}

tnode *makeleafnode(const char *s) {
    tnode *t = (tnode *)malloc(sizeof(tnode));
    t->symbol = xstrdup(s);
    t->left = t->right = NULL;
    return t;
}

tnode *makeoperatornode(const char *op, tnode *l, tnode *r) {
    tnode *t = (tnode *)malloc(sizeof(tnode));
    t->symbol = xstrdup(op);
    t->left = l;
    t->right = r;
    return t;
}

void infixtoprefix(tnode *root) {
    if (!root) return;
    printf("%s ", root->symbol);
    infixtoprefix(root->left);
    infixtoprefix(root->right);
}

void freetree(tnode *root) {
    if (!root) return;
    freetree(root->left);
    freetree(root->right);
    free(root->symbol);
    free(root);
}

#ifndef TREE_H
#define TREE_H

typedef struct tnode {
    char *symbol;               // operator or identifier
    struct tnode *left, *right;
} tnode;

tnode *makeleafnode(const char *s);
tnode *makeoperatornode(const char *op, tnode *l, tnode *r);
void infixtoprefix(tnode *root);
void freetree(tnode *root);

#endif

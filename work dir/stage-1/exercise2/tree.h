#include<stdio.h>
#include<stdlib.h>
struct tnode{
    int val;
    char *op;
    struct tnode *left;
    struct tnode *right;
};

struct tnode * makeleafnode(int n);

struct tnode * makeoperatornode(char c , struct tnode *l, struct tnode *r);

int codeGen(struct tnode *t, FILE *target_file);

int execute(struct tnode *t);
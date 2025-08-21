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



void printTree(struct tnode *t) {
    if (t == NULL) return;

    switch (t->nodetype) {
        case NODE_NUM:    printf("%d ", t->val); break;
        case NODE_ID:     printf("%s ", t->varname); break;
        case NODE_PLUS:   printf("PLUS ");   printTree(t->left); printTree(t->right); break;
        case NODE_MINUS:  printf("MINUS ");  printTree(t->left); printTree(t->right); break;
        case NODE_MUL:    printf("MUL ");    printTree(t->left); printTree(t->right); break;
        case NODE_DIV:    printf("DIV ");    printTree(t->left); printTree(t->right); break;
        case NODE_READ:   printf("READ ");   printTree(t->left); break;
        case NODE_WRITE:  printf("WRITE ");  printTree(t->left); break;
        case NODE_ASSIGN:  printf("ASSIGN "); printTree(t->left); printTree(t->right); break;
        case NODE_CONNECTOR: printf("CONNECTOR "); printTree(t->left); printTree(t->right); break;
    }
}
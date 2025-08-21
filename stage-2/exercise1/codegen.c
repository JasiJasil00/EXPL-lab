#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
int memory[26];
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

int evaluate(struct tnode *t) {
    if (t == NULL) return 0;

    switch (t->nodetype) {
        case NODE_NUM:
            return t->val;

        case NODE_ID: {
            // Example: if you keep variable values in a symbol table
            // For now, assume single-letter variables mapped to memory[]
            int index = t->varname[0] - 'a';
            return memory[index];
        }

        case NODE_PLUS:
            return evaluate(t->left) + evaluate(t->right);

        case NODE_MINUS:
            return evaluate(t->left) - evaluate(t->right);

        case NODE_MUL:
            return evaluate(t->left) * evaluate(t->right);

        case NODE_DIV:
            return evaluate(t->left) / evaluate(t->right);

        case NODE_ASSIGN: {
            int index = t->left->varname[0] - 'a';
            int value = evaluate(t->right);
            memory[index] = value;   // store result
            return value;
        }

        case NODE_READ: {
            int index = t->left->varname[0] - 'a';
            int value;
            printf("Enter value for %c: ", t->left->varname[0]);
            scanf("%d", &value);
            memory[index] = value;
            return value;
        }

        case NODE_WRITE:
            printf("%d\n", evaluate(t->left));
            return 0;

        case NODE_CONNECTOR:
            evaluate(t->left);
            evaluate(t->right);
            return 0;

        default:
            printf("Unknown nodetype: %d\n", t->nodetype);
            return 0;
    }
}

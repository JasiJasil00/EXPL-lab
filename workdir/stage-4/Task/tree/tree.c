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
    temp->Gentry = NULL;
    if (c != NULL) {
        temp->varname = (char*)malloc(strlen(c) + 1);
        strcpy(temp->varname, c);
    } else {
        temp->varname = NULL;
    }
    typevalidate(temp);
    return temp;
}

void typevalidate(struct tnode *t){
    switch(t->nodetype){
        case NODE_IF:
            if(t->left->type!=boolType){
                 yyerror("node type match error if condition");
            }    
            break;
        case NODE_WHILE:
             if(t->left->type!=boolType){
                yyerror("node type match error while condition");
            }
            break;
        case NODE_REPEATUNTIL:
            if( t->left->type != boolType){
                yyerror("node type match error in repeat-until condition");
            }
            break;
        case NODE_DOWHILE:
            if(t->left->type != boolType){
                yyerror("node type match error in do-while condition");
            
            }
        case NODE_ASSIGN:
            if(t->left->type != t->right->type){
                yyerror("node type match error assignment");
               
            }
            break;
        case NODE_PLUS:
            if(t->left->type!=intType || t->right->type!=intType){
                    yyerror("node type match error expression");
                }
            break;
        case NODE_MINUS:
            if(t->left->type!=intType || t->right->type!=intType){
                    yyerror("node type match error expression");
            }
            break;
        case NODE_MUL:
            if(t->left->type!=intType || t->right->type!=intType){
                    yyerror("node type match error expression");
            }
            break;
        case NODE_DIV:
            if(t->left->type!=intType || t->right->type!=intType){
                    yyerror("node type match error expression");
            }
            break;
        case NODE_LESS:
            if(t->left->type!=intType || t->right->type!=intType){
                     yyerror("node type match error logic");
            }    
            break;
        case NODE_LESSOREQUAL:
             if(t->left->type!=intType || t->right->type!=intType){
                     yyerror("node type match error logic");
            }    
            break;
        case NODE_MORE:
             if(t->left->type!=intType || t->right->type!=intType){
                     yyerror("node type match error logic");
            }    
            break;
        case NODE_MOREOREQUAL:
             if(t->left->type!=intType || t->right->type!=intType){
                     yyerror("node type match error logic");
            }    
            break;
        case NODE_EQUAL:
             if(t->left->type!=intType || t->right->type!=intType){
                     yyerror("node type match error logic");
            }    
            break;
        case NODE_NOTEQUAL:
             if(t->left->type!=intType || t->right->type!=intType){
                     yyerror("node type match error logic");
            }    
            break;
    }
}

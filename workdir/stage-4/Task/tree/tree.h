#ifndef TREE_H
#define TREE_H

#define intType 1
#define boolType 2
#define strType 3
#define noType 100
// Extra nodetype for tree building, not a yacc token
  // choose a large value so it won’t clash
#define NODE_CONNECTOR    999
#define NODE_STRING       1000
#define NODE_NUM          1001
#define NODE_ID           1002
#define NODE_PLUS         1003
#define NODE_MINUS        1004
#define NODE_MUL          1005
#define NODE_DIV          1006
#define NODE_READ         1007
#define NODE_WRITE        1008
#define NODE_ASSIGN       1009
#define NODE_LESS         1010
#define NODE_LESSOREQUAL  1011
#define NODE_EQUAL        1012
#define NODE_NOTEQUAL    1013
#define NODE_MORE         1014
#define NODE_MOREOREQUAL  1015
#define NODE_IF           1016
#define NODE_WHILE        1017
#define NODE_BREAK        1018
#define NODE_CONTINUE     1019
#define NODE_REPEATUNTIL  1020
#define NODE_DOWHILE      1021

#define MAX_LOOP_DEPTH 100

struct tnode {
    int val;
    int type;
    char* varname;
    int nodetype;
    struct Gsymbol *Gentry;
    struct tnode *left,*right;
};

struct tnode* createTree(int val, int type, char* c, int nodetype, struct tnode *l, struct tnode *r);
void typevalidate(struct tnode * t);
#endif

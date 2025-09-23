#ifndef TREE_H
#define TREE_H

#define intType 1

// Extra nodetype for tree building, not a yacc token
  // choose a large value so it won’t clash
#define NODE_CONNECTOR 1000
#define NODE_NUM       1001
#define NODE_ID        1002
#define NODE_PLUS      1003
#define NODE_MINUS     1004
#define NODE_MUL       1005
#define NODE_DIV       1006
#define NODE_READ      1007
#define NODE_WRITE     1008
#define NODE_ASSIGN    1009
struct tnode {
    int val;
    int type;
    char* varname;
    int nodetype;
    struct tnode *left,*right;
};

struct tnode* createTree(int val, int type, char* c, int nodetype, struct tnode *l, struct tnode *r);
void printTree(struct tnode *t);
int evaluate(struct tnode *);


#endif

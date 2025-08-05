typedef struct tnode{
    int val;
    char *op;
    struct tnode *left;
    struct tnode *right;
}tnode;

struct tnode * makeleafnode(int n);

struct tnode * makeoperatornode(char c , struct tnode *l, struct tnode *r);

int evaluate(struct tnode *t);


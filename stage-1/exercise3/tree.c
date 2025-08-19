
struct tnode * makeleafnode(int n){
    struct tnode * temp;
    temp = (struct tnode *)malloc(sizeof(struct tnode));
    temp->op=NULL;
    temp->val = n;
    temp->left=NULL;
    temp->right=NULL;
    return temp; 
}
struct tnode * makeoperatornode(char c , struct tnode *l, struct tnode *r){
    struct tnode * temp;
    temp = (struct tnode *)malloc(sizeof(struct tnode));
    temp->op=malloc(sizeof(char));
    *(temp->op)= c;
    temp->left= l;
    temp->right= r;
    return temp;
}



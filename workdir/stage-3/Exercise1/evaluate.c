
int memory[26];
void memoryset()
{
    for (int i = 0; i < 26; i++)
    {
        memory[i] = 0;
    }
}
int evaluate(struct tnode *root)
{
    if (!root)
        return -1;

    switch (root->nodetype)
    {
    case NODE_CONNECTOR:
        evaluate(root->left);
        evaluate(root->right);
        break;
    case NODE_NUM:
        return root->val;
        break;
    case NODE_ID:
        return memory[root->varname[0]-'a'];
        break;
    case NODE_PLUS:
        return evaluate(root->left)+evaluate(root->right);
        break;
    case NODE_MINUS:
        return evaluate(root->left)-evaluate(root->right);
        break;
    case NODE_MUL:
        return evaluate(root->left)*evaluate(root->right);
        break;    
    case NODE_DIV:
        return evaluate(root->left)/evaluate(root->right);
        break;
    case NODE_READ:
        scanf("%d",&memory[root->left->varname[0]-'a']);
        break;
    case NODE_WRITE:
        printf("%c : %d\n",root->left->varname[0],memory[root->left->varname[0]-'a']);
        break;
    case NODE_ASSIGN:
        memory[root->left->varname[0]-'a']=evaluate(root->right);
        break;
    case NODE_LESS:
        if(evaluate(root->left) < evaluate(root->right)){
            return 1;
        }else{
            return 0;
        }
        break;
    case NODE_LESSOREQUAL:
        if(evaluate(root->left) <= evaluate(root->right)){
            return 1;
        }else{
            return 0;
        }
        break;
    case NODE_EQUAL:
        if(evaluate(root->left) == evaluate(root->right)){
            return 1;
        }else{
            return 0;
        }
        break;
    case NODE_NOTEQUAL:
        if(evaluate(root->left) != evaluate(root->right)){
            return 1;
        }else{
            return 0;
        }
        break;
    case NODE_MORE:
        if(evaluate(root->left) > evaluate(root->right)){
            return 1;
        }else{
            return 0;
        }
        break;
    case NODE_MOREOREQUAL:
        if(evaluate(root->left) >= evaluate(root->right)){
            return 1;
        }else{
            return 0;
        }
        break;
    case NODE_IF:
        int truth = evaluate(root->left);
        if(truth == 1){
            return evaluate(root->right->left);
        }else{
            return evaluate(root->right->right);
        }
    case NODE_WHILE:
        int value;
        while(evaluate(root->left)){
            value=evaluate(root->left);
        }
        return value;
    }
    return -1;
}
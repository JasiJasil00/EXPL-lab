#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
int reg = 0;
int label = 0;
int getreg()
{
    return reg++;
}
void freereg()
{
    reg--;
}
int getlabel(){
    return label++;
}
int codeGen(struct tnode *t, FILE *target_file)
{   int i ;
    int j ;
    int p;
    int q;
    if (t == NULL)
        return -1;

    switch (t->nodetype)
    {
    case NODE_NUM:
         i = getreg();
        fprintf(target_file, "MOV R%d, %d\n", i, t->val);
        return i;
        break;
    case NODE_ID:
         i = getreg();
         if (t->varname == NULL) {
    fprintf(stderr, "Error: varname is NULL for nodetype %d\n", t->nodetype);
    exit(1);
}

        fprintf(target_file, "MOV R%d, [%d]\n", i, 4096 + t->varname[0] - 'a');
        return i;
        break;
    case NODE_PLUS:
        p = codeGen(t->left, target_file);
        q = codeGen(t->right, target_file);
        fprintf(target_file, "ADD R%d,R%d\n", p, q);
        freereg();
        return p;
        break;
    case NODE_MINUS:
        p = codeGen(t->left, target_file);
         q = codeGen(t->right, target_file);
        fprintf(target_file, "SUB R%d,R%d\n", p, q);
        freereg();
        return p;
        break;
    case NODE_MUL:
        p = codeGen(t->left, target_file);
        q = codeGen(t->right, target_file);
        fprintf(target_file, "MUL R%d,R%d\n", p, q);
        freereg();
        return p;
        break;
    case NODE_DIV:
        p = codeGen(t->left, target_file);
        q = codeGen(t->right, target_file);
        fprintf(target_file, "DIV R%d,R%d\n", p, q);
        freereg();
        return p;
        break;
    case NODE_READ:
    if (t->left->varname == NULL) {
        fprintf(stderr, "Error: varname is NULL in NODE_READ\n");
        exit(1);
    }
        i = getreg();
        j = getreg();
        fprintf(target_file, "MOV R%d,%d\n", j, 4096 + t->left->varname[0] - 'a');
        fprintf(target_file, "MOV R%d, \"Read\"\n",i);
        fprintf(target_file, "PUSH R%d\n",i);
        fprintf(target_file, "MOV R%d, -1\n",i);
        fprintf(target_file, "PUSH R%d\n",i);
        fprintf(target_file, "MOV R%d, R%d\n",i,j);
        fprintf(target_file, "PUSH R%d\n",i);
        fprintf(target_file, "PUSH R%d\n",i);
        fprintf(target_file, "PUSH R%d\n",i);
        fprintf(target_file, "CALL 0\n");
        fprintf(target_file, "POP R%d\n",i);
        fprintf(target_file, "POP R%d\n",i);
        fprintf(target_file, "POP R%d\n",i);
        fprintf(target_file, "POP R%d\n",i);
        fprintf(target_file, "POP R%d\n",i);
        freereg();
        freereg();
        break;
    case NODE_WRITE:
        i = getreg();
        j = codeGen(t->left,target_file);
        fprintf(target_file, "MOV R%d, \"Write\"\n", i);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "MOV R%d, -2\n", i);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "PUSH R%d \n", j);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "CALL 0\n");
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n",i);
        freereg();
        freereg();
        return -1;
        break;
    case NODE_ASSIGN:
        i = codeGen(t->right,target_file);
        fprintf(target_file,"MOV [%d], R%d\n", 4096 + t->left->varname[0] - 'a',i);
        freereg();
        break;
    case NODE_CONNECTOR:
        codeGen(t->left,target_file);
        codeGen(t->right,target_file);
        break;
    case NODE_LESS:
        p= codeGen(t->left,target_file);
        q= codeGen(t->right,target_file);
        fprintf(target_file,"LT R%d, R%d\n", p,q);
        freereg();
        return p;
        break;
    case NODE_LESSOREQUAL:
        p= codeGen(t->left,target_file);
        q= codeGen(t->right,target_file);
        fprintf(target_file,"LE R%d, R%d\n", p,q);
        freereg();
        return p;
        break;
    case NODE_EQUAL:
        p= codeGen(t->left,target_file);
        q= codeGen(t->right,target_file);
        fprintf(target_file,"EQ R%d, R%d\n", p,q);
        freereg();
        return p;
        break;
    case NODE_NOTEQUAL:
        p= codeGen(t->left,target_file);
        q= codeGen(t->right,target_file);
        fprintf(target_file,"NE R%d, R%d\n", p,q);
        freereg();
        return p;
        break;
    case NODE_MORE:
        p= codeGen(t->left,target_file);
        q= codeGen(t->right,target_file);
        fprintf(target_file,"GT R%d, R%d\n", p,q);
        freereg();
        return p;
        break;
    case NODE_MOREOREQUAL:
        p= codeGen(t->left,target_file);
        q= codeGen(t->right,target_file);
        fprintf(target_file,"GE R%d, R%d\n", p,q);
        freereg();
        return p;
        break;
    case NODE_IF:
        p = codeGen(t->left,target_file);
        i = getlabel();
        if(t->right->right)j = getlabel();
        fprintf(target_file,"JZ R%d, L%d\n", p,i);
        q=codeGen(t->right->left,target_file);
        if(t->right->right){fprintf(target_file,"JMP L%d\n", j);}
        fprintf(target_file,"L%d\n", i);
        if(t->right->right){
            q=codeGen(t->right->right,target_file);
            fprintf(target_file,"L%d\n",j);
        }
        freereg();
        break;
    case NODE_WHILE:
        i=getlabel();
        j=getlabel();
        fprintf(target_file,"L%d\n",i);
        p=codeGen(t->left,target_file);
        fprintf(target_file,"JZ R%d,L%d\n",p,j);
        q=codeGen(t->right,target_file);
        fprintf(target_file,"JMP L%d\n",i);
        fprintf(target_file,"L%d\n",j);
        freereg();
        break;
    }
    return -1;
}
void execute(struct tnode *t)
{

    FILE *target_file = fopen("target_file.xsm", "w"); // task1
    if (!target_file)
    {
        perror("fopen");
        return ;
    }
    fprintf(target_file, "%d\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",
            0,
            2056,
            0,
            0,
            0,
            0,
            0,
            0);
    for (int i = 0; i <= 26; i++)
    {
        fprintf(target_file, "MOV [%d], 0\n", 4096 + i);
    }
    fprintf(target_file, "MOV SP, 4123\n");
    int result = codeGen(t, target_file); // task2
    fprintf(target_file, "MOV R5, \"Exit\"\n");
    fprintf(target_file, "PUSH R5\n");
    fprintf(target_file, "PUSH R1\n");
    fprintf(target_file, "PUSH R4\n");
    fprintf(target_file, "PUSH R0\n");
    fprintf(target_file, "PUSH R0\n");
    fprintf(target_file, "CALL 0\n");
    fprintf(target_file, "POP R0\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file, "POP R1\n");

    fclose(target_file);
    return ;
}

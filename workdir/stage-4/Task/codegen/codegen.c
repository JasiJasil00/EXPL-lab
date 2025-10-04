#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tree/tree.h"
#include "../symbol/symbol.h"
#include "codegen.h"
int looptop = -1;
int reg = 0;
int label = 0;
void pushstack(int breaklabel, int continuelabel)
{
    looptop++;
    if (looptop >= MAX_LOOP_DEPTH)
    {
        fprintf(stderr, "Loop nesting too deep!\n");
        exit(1);
    }
    struct stack * temp = malloc(sizeof(struct stack));
    temp->brk=breaklabel;
    temp->cont=continuelabel;
    temp->next=head;
    head=temp;
}

void popstack()
{
    if (looptop < 0)
    {
        fprintf(stderr, "Loop stack underflow!\n");
        exit(1);
    }
    looptop--;
    struct stack * temp = head;
    head = head->next;
    free(temp); 
}

int getreg()
{
    return reg++;
}
void freereg()
{
    reg--;
}
int getlabel()
{
    return label++;
}
int operator(FILE *target_file,struct tnode *t,const char *s){
        int p = codeGen(t->left, target_file);
        int q = codeGen(t->right, target_file);
        fprintf(target_file, "%s R%d,R%d\n",s, p, q);
        freereg();
        return p;
}
int codeGen(struct tnode *t, FILE *target_file)
{
    int i;
    int j;
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
    case NODE_STRING:
        i = getreg();
        fprintf(target_file,"MOV R%d , %s\n", i , t->varname);
        return i;
        break;
    case NODE_ID:
        i = getreg();
        if (t->varname == NULL)
        {
            fprintf(stderr, "Error: varname is NULL for nodetype %d\n", t->nodetype);
            exit(1);
        }
        if(!t->Gentry){
            fprintf(stderr, "Error: varname is NULL for nodetype %d\n", t->nodetype);
            exit(EXIT_FAILURE);
        }
        fprintf(target_file, "MOV R%d, [%d]\n", i, t->Gentry->binding);
        return i;
        break;
    case NODE_PLUS:
        return operator(target_file,t,"ADD");
        break;
    case NODE_MINUS:
        return operator(target_file,t,"SUB");
        break;
    case NODE_MUL:
        return operator(target_file,t,"MUL");
        break;
    case NODE_DIV:
        return operator(target_file,t,"DIV");
        break;
    case NODE_READ:
        i = getreg();
        j = getreg();
        fprintf(target_file, "MOV R%d,%d\n", j,t->left->Gentry->binding);
        fprintf(target_file, "MOV R%d, \"Read\"\n", i);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "MOV R%d, -1\n", i);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "MOV R%d, R%d\n", i, j);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "PUSH R%d\n", i);
        fprintf(target_file, "CALL 0\n");
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n", i);
        fprintf(target_file, "POP R%d\n", i);
        freereg();
        freereg();
        break;
    case NODE_WRITE:
        i = getreg();
        j = codeGen(t->left, target_file);
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
        fprintf(target_file, "POP R%d\n", i);
        freereg();
        freereg();
        return -1;
        break;
    case NODE_ASSIGN:
        i = codeGen(t->right, target_file);
        fprintf(target_file, "MOV [%d], R%d\n",t->left->Gentry->binding, i);
        freereg();
        break;
    case NODE_CONNECTOR:
        codeGen(t->left, target_file);
        codeGen(t->right, target_file);
        break;
    case NODE_LESS:
        return operator(target_file,t,"LT");
        break;
    case NODE_LESSOREQUAL:
        return operator(target_file,t,"LE");
        break;
    case NODE_EQUAL:
        return operator(target_file,t,"EQ");
        break;
    case NODE_NOTEQUAL:
        return operator(target_file,t,"NE");
        break;
    case NODE_MORE:
        return operator(target_file,t,"GT");
        break;
    case NODE_MOREOREQUAL:
        return operator(target_file,t,"GE");
        break;
    case NODE_IF:
        p = codeGen(t->left, target_file);
        i = getlabel();
        if (t->right->right)
            j = getlabel();
        fprintf(target_file, "JZ R%d, L%d\n", p, i);
        q = codeGen(t->right->left, target_file);
        if (t->right->right)
        {
            fprintf(target_file, "JMP L%d\n", j);
        }
        fprintf(target_file, "L%d:\n", i);
        if (t->right->right)
        {
            q = codeGen(t->right->right, target_file);
            fprintf(target_file, "L%d:\n", j);
        }
        freereg();
        break;
    case NODE_WHILE:
        i = getlabel();
        j = getlabel();
        pushstack(j, i);
        fprintf(target_file, "L%d:\n", i);
        p = codeGen(t->left, target_file);
        fprintf(target_file, "JZ R%d, L%d\n", p, j);
        q = codeGen(t->right, target_file);
        fprintf(target_file, "JMP L%d\n", i);
        fprintf(target_file, "L%d:\n", j);
        popstack();
        freereg();
        break;
    case NODE_REPEATUNTIL:
        i = getlabel();
        j = getlabel();
        pushstack(j, i);

        fprintf(target_file, "L%d:\n", i);
        codeGen(t->right, target_file); // body
        p = codeGen(t->left, target_file);
        fprintf(target_file, "JNZ R%d, L%d\n", p, j); // exit if condition true
        fprintf(target_file, "JMP L%d\n", i);          // else repeat
        fprintf(target_file, "L%d:\n", j);

        popstack();
        freereg();
        break;

    case NODE_DOWHILE:
         i = getlabel();
         j = getlabel();
        pushstack(j, i);

        fprintf(target_file, "L%d:\n", i);
        codeGen(t->right, target_file); // body
         p = codeGen(t->left, target_file);
        fprintf(target_file, "JNZ R%d, L%d\n", p, i); // loop again if condition true
        fprintf(target_file, "L%d:\n", j);

        popstack();
        freereg();
        break;

    case NODE_BREAK:
        if (looptop < 0)
        {
            fprintf(stderr, "Error: 'break' used outside of loop\n");
            exit(1);
        }
        fprintf(target_file, "JMP L%d\n", head->brk);
        break;
    case NODE_CONTINUE:
        if (looptop < 0)
        {
            fprintf(stderr, "Error: 'continue' used outside of loop\n");
            exit(1);
        }
        fprintf(target_file, "JMP L%d\n", head->cont);
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
        return;
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
    for (int i = 0; i <= addresssymbol-4096; i++)
    {
        fprintf(target_file, "MOV [%d], 0\n", 4096 + i);
    }
    fprintf(target_file, "MOV SP, %d\n",addresssymbol);
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
    return;
}



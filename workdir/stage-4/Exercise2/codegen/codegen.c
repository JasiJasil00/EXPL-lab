#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tree/tree.h"
#include "../symbol/symbol.h"
#include "codegen.h"
int looptop = -1;
int reg = 0;
int label = 0;
struct stack * head=NULL;
int error;
int end;
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
    case NODE_VAR:
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
    case NODE_PTR:
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
    case NODE_PTR_VAR:
            i = getreg();
             fprintf(target_file, "MOV R%d, %d\n", i, t->Gentry->binding);
             return i;
        break;
    case NODE_ARRAY:
         i = getreg();
        int indexReg = codeGen(t->right, target_file); // evaluate index
    if (!t->Gentry) {
        fprintf(stderr, "Error: array Gentry is NULL\n");
        exit(1);
    }
    p = getreg();
    fprintf(target_file, "ADD R%d, %d\n", indexReg, t->Gentry->binding); // base + index
     fprintf(target_file,"MOV R%d, %d\n",p, t->Gentry->binding);
        fprintf(target_file,"ADD R%d, %d\n",p,t->Gentry->size);
        fprintf(target_file,"GT R%d, R%d\n",p,indexReg);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
    fprintf(target_file, "MOV R%d, [R%d]\n", i, indexReg);
    freereg();
    freereg();
    return i;
        break;
    case NODE_2D_ARRAY:
        int Reg1 = codeGen(t->left, target_file);
        int Reg2 = codeGen(t->right, target_file);
        p = getreg();
        fprintf(target_file, "MOV R%d, %d\n",p,t->Gentry->size);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg1);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV R%d, %d\n",p,t->Gentry->sizecol);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg2);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MUL R%d, %d\n", Reg2, t->Gentry->size);
        fprintf(target_file, "ADD R%d, R%d\n",Reg1, Reg2);
        fprintf(target_file,"ADD R%d, %d\n",Reg1, t->Gentry->binding);
        fprintf(target_file,"MOV R%d, %d\n",p, t->Gentry->binding);
        fprintf(target_file,"ADD R%d, %d\n",p, t->Gentry->size*t->Gentry->sizecol);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg1);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV R%d, [R%d]\n",p, Reg1);
        freereg();
        freereg();
        return p;
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
    case NODE_MOD:
         return operator(target_file,t,"MOD");
        break;
    case NODE_READ:
        i = getreg();
        j = getreg();
    if(t->left->nodetype == NODE_VAR || t->left->nodetype == NODE_PTR) {
        fprintf(target_file, "MOV R%d, %d\n", j, t->left->Gentry->binding);
    } else if(t->left->nodetype == NODE_ARRAY){
        int indexReg = codeGen(t->left->right, target_file);
        p = getreg();
        fprintf(target_file, "ADD R%d, %d\n", indexReg, t->left->Gentry->binding);
        fprintf(target_file,"MOV R%d, %d\n",p, t->left->Gentry->binding);
        fprintf(target_file,"ADD R%d, %d\n",p,t->left->Gentry->size);
        fprintf(target_file,"GT R%d, R%d\n",p,indexReg);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV R%d, R%d\n", j, indexReg);
        freereg();
        freereg();
    } else if(t->left->nodetype == NODE_2D_ARRAY) {
        int Reg1 = codeGen(t->left->left, target_file);
        int Reg2 = codeGen(t->left->right, target_file);
        p = getreg();
        fprintf(target_file, "MOV R%d, %d\n",p,t->left->Gentry->size);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg1);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV R%d, %d\n",p,t->left->Gentry->sizecol);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg2);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MUL R%d, %d\n", Reg2, t->left->Gentry->size);
        fprintf(target_file, "ADD R%d, R%d\n",Reg1, Reg2);
        fprintf(target_file,"ADD R%d, %d\n",Reg1, t->left->Gentry->binding);
        fprintf(target_file,"MOV R%d, %d\n",p, t->left->Gentry->binding);
        fprintf(target_file,"ADD R%d, %d\n",p, t->left->Gentry->size*t->left->Gentry->sizecol);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg1);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV R%d, R%d\n", j, Reg1);
        freereg();
        freereg();
        freereg();
    }
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
    if(t->left->nodetype == NODE_VAR || t->left->nodetype == NODE_PTR) {
        if(!t->left->Gentry){
            fprintf(stderr, "Error: Left variable Gentry is NULL\n");
            exit(1);
        }
        fprintf(target_file, "MOV [%d], R%d\n", t->left->Gentry->binding, i);
    } else if(t->left->nodetype == NODE_ARRAY) {
        int indexReg = codeGen(t->left->right, target_file);
        if(!t->left->Gentry){
            fprintf(stderr, "Error: Array Gentry is NULL\n");
            exit(1);
        }
        p = getreg();
        fprintf(target_file, "ADD R%d, %d\n", indexReg, t->left->Gentry->binding);
        fprintf(target_file,"MOV R%d, %d\n",p, t->left->Gentry->binding);
        fprintf(target_file,"ADD R%d, %d\n",p,t->left->Gentry->size);
        fprintf(target_file,"GT R%d, R%d\n",p,indexReg);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV [R%d], R%d\n", indexReg, i);
        freereg();
        freereg();
    }else if(t->left->nodetype == NODE_2D_ARRAY ){
        int Reg1 = codeGen(t->left->left, target_file);
        int Reg2 = codeGen(t->left->right, target_file);
        p = getreg();
        fprintf(target_file, "MOV R%d, %d\n",p,t->left->Gentry->size);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg1);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV R%d, %d\n",p,t->left->Gentry->sizecol);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg2);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MUL R%d, %d\n", Reg2, t->left->Gentry->size);
        fprintf(target_file, "ADD R%d, R%d\n",Reg1, Reg2);
        fprintf(target_file,"ADD R%d, %d\n",Reg1, t->left->Gentry->binding);
        fprintf(target_file,"MOV R%d, %d\n",p, t->left->Gentry->binding);
        fprintf(target_file,"ADD R%d, %d\n",p, t->left->Gentry->size*t->left->Gentry->sizecol);
        fprintf(target_file,"GT R%d, R%d\n",p,Reg1);
        fprintf(target_file,"JZ R%d, L%d\n",p,error);
        fprintf(target_file, "MOV [R%d], R%d\n", Reg1, i);
        freereg();
        freereg();
        freereg();
    }
    freereg();
    break;
    case NODE_PTR_ASSIGN:
        t->right->Gentry=Lookup(t->right->varname); if(!t->right->Gentry) yyerror("no entry in symbol table"); t->right->type=t->right->Gentry->type;  t->right->nodetype=NODE_VAR; 
                                  t->left->Gentry=Lookup(t->left->varname); if(!t->left->Gentry)yyerror("no entry in symbol table");  t->left->type=t->left->Gentry->type;
                                  t->left->nodetype=t->left->Gentry->array;    
                                    if(t->left->nodetype!=NODE_PTR_VAR)yyerror("not a pointer");
                                    t->left->Gentry->binding = t->right->Gentry->binding;
                                    t->left->Gentry->size=t->right->Gentry->size;
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
    error = getlabel();
    end   = getlabel();
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
    fprintf(target_file,"JMP L%d\n",end);
    fprintf(target_file,"L%d:\n",error);
    fprintf(target_file,"MOV R2, \"out of bound\"\n");
     fprintf(target_file, "MOV R5, \"Write\"\n");
    fprintf(target_file, "PUSH R5\n");
     fprintf(target_file, "MOV R1, -2\n");
    fprintf(target_file, "PUSH R1\n");
    fprintf(target_file, "PUSH R2 \n");
    fprintf(target_file, "PUSH R2\n");
    fprintf(target_file, "PUSH R2\n");
    fprintf(target_file, "CALL 0\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file, "POP R1\n");
    fprintf(target_file,"L%d:\n",end);
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



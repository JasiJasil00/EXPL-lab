#ifndef CODEGEN_H
#define CODEGEN_H
struct stack{
    int brk;
    int cont;
    struct stack *next ;
};
struct stack * head;
int codeGen(struct tnode *t,FILE *target_file);
void execute(struct tnode * t);
void pushstack(int breaklabel, int continuelabel);
void popstack();
int getreg();
void freereg();
int getlabel();
int codeGen(struct tnode *t, FILE *target_file);
void execute(struct tnode *t);
int operator(FILE *target_file,struct tnode *t,const char *s);
#endif
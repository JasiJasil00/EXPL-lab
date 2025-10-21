#include "symbol.h"
#include "../tree/tree.h"
int addresssymbol = 4096;
struct Gsymbol * symbol = NULL;
struct Gsymbol *Lookup(char * name){
    struct Gsymbol * temp = symbol;
    while( temp!=NULL && strcmp(temp->name,name)){
        temp = temp->next;
    }
    return temp;
}            
void Install(char *name, int type, int size , int sizecol, int array) {
    // Check if the variable is already declared
    if (Lookup(name) != NULL) {
        printf("Error: %s already declared\n", name);
        exit(1);
        return;
    }
    if(size<=0)yyerror("size problem");
    if(sizecol<=0 && array==NODE_2D_ARRAY)yyerror("sizecol problem");
    // Create a new symbol table node
    struct Gsymbol *newnode = malloc(sizeof(struct Gsymbol));
    newnode->name = strdup(name);
    newnode->type = type;
    newnode->size = size;
    newnode->sizecol = sizecol;
    newnode->binding = addresssymbol; 
    newnode->array=array; // You can assign memory locations later
    newnode->next = NULL;
    int num = size;
    if(array==NODE_2D_ARRAY)num = size*sizecol;
    addresssymbol+=num;
    // Insert at the end of the list
    if (symbol == NULL) {
        symbol = newnode;
    } else {
        struct Gsymbol *temp = symbol;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newnode;
    }
}


void printsymboltable(){
    struct Gsymbol * temp = symbol;
    printf("linkno name type size binding next\n");
    int i = 0;
    while(temp!=NULL){
        printf("l%d %s %d %d %d %d %d l%d\n",i,temp->name,temp->type,temp->size,temp->sizecol,temp->binding,temp->array,i+1);
        i++;
        temp = temp->next;
    } 
}
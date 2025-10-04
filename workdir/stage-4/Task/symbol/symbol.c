#include "symbol.h"
int addresssymbol = 4096;
struct Gsymbol * symbol = NULL;
struct Gsymbol *Lookup(char * name){
    struct Gsymbol * temp = symbol;
    while( temp!=NULL && strcmp(temp->name,name)){
        temp = temp->next;
    }
    return temp;
}            
void Install(char *name, int type, int size) {
    // Check if the variable is already declared
    if (Lookup(name) != NULL) {
        printf("Error: %s already declared\n", name);
        return;
    }

    // Create a new symbol table node
    struct Gsymbol *newnode = malloc(sizeof(struct Gsymbol));
    newnode->name = strdup(name);
    newnode->type = type;
    newnode->size = size;
    newnode->binding = addresssymbol;  // You can assign memory locations later
    newnode->next = NULL;
    addresssymbol++;
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
        printf("l%d %s %d %d %d l%d\n",i,temp->name,temp->type,temp->size,temp->binding,i+1);
        i++;
        temp = temp->next;
    } 
}
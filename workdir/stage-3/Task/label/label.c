#include "label.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct label * labelhead=NULL;

struct label * createnode(char labelname[],int addr){
    struct label * temp = malloc(sizeof(struct label));
    strcpy(temp->labelname,labelname);
    temp->addr=addr;
    temp->next=NULL;
}
void insertnode(char labelname[],int addr){
    struct label * newnode = createnode(labelname,addr);
    if(labelhead==NULL){
        labelhead=newnode;
        return;
    }
    struct label * temp = labelhead;
    while(temp->next != NULL) temp = temp->next;

    temp->next=newnode;
    return;

}
int findnode(char labelname[]){
    struct label * temp = labelhead;
    while(temp) {
        if (strcmp(temp->labelname, labelname) == 0)
            return temp->addr;
        temp = temp->next;
    }
    
    printf("Label not found\n");
    exit(1);

    return 0;
}
#ifndef LABEL_H
#define LABEL_H

struct label{
    char labelname[10];
    int addr;
    struct label *next;
};
/*u can use typdef struct label  LABEL;*/

extern struct label * labelhead;

struct label * createnode(char labelname[],int addr);
void insertnode(char labelname[],int addr);
int findnode(char labelname[]);

#endif
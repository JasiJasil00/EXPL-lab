#ifndef SYMBOL_H
#define SYMBOL_H
struct Gsymbol {
    char* name;       // name of the variable
    int type;         // type of the variable
    int size;         // size of the type of the variable
    int binding;      // stores the static memory address allocated to the variable
    struct Gsymbol *next;
};

//modern hashtables are maintained
struct Gsymbol *Lookup(char * name);            // Returns a pointer to the symbol table entry for the variable, returns NULL otherwise.
void Install(char *name, int type, int size);   // Creates a symbol table entry.
void printsymboltable();
#endif
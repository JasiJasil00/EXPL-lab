%{
/*** Auxiliary declarations section ***/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Declare yyerror and yylex to avoid implicit declaration warnings
int yylex(void);
void yyerror(const char *s);

// Variable to keep track of the position of the number in the input
int pos = 0;

// Custom function to print an operator
void print_operator(char op);
%}

/*** YACC Declarations section ***/
%token DIGIT
%left '+'
%left '*'

%%
/*** Rules Section ***/
start:
    expr '\n' { exit(0); }
    ;

expr:
    expr '+' expr { print_operator('+'); }
  | expr '*' expr { print_operator('*'); }
  | '(' expr ')'
  | DIGIT         { printf("NUM%d ", pos); }
  ;
%%

/*** Auxiliary functions section ***/

void print_operator(char c) {
    switch (c) {
        case '+': printf("PLUS "); break;
        case '*': printf("MUL "); break;
    }
}

void yyerror(const char *s) {
    fprintf(stderr, "yyerror: %s\n", s);
}

int yylex(void) {
    int c = getchar();
    if (isdigit(c)) {
        pos++;
        return DIGIT;
    } else if (c == ' ' || c == '\t') {
        return yylex();  // Ignore whitespace
    } else {
        return c;        // Return operator or parenthesis directly
    }
}

int main(void) {
    return yyparse();
}

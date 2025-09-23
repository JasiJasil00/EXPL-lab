%{
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "tree.c"

int yylex(void);
void yyerror(const char *s);
%}

%union {
    char *str;
    struct tnode *no;
}

%token <str> ID
%token PLUS MINUS MUL DIV END
%type  <no>  expr start

%left PLUS MINUS
%left MUL DIV

%%

start
    : expr END     { infixtoprefix($1); printf("\n"); freetree($1);exit(1); }
    ;

expr
    : expr PLUS expr   { $$ = makeoperatornode("+", $1, $3); }
    | expr MINUS expr  { $$ = makeoperatornode("-", $1, $3); }
    | expr MUL expr    { $$ = makeoperatornode("*", $1, $3); }
    | expr DIV expr    { $$ = makeoperatornode("/", $1, $3); }
    | '(' expr ')'     { $$ = $2; }
    | ID               { $$ = makeleafnode($1); free($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    return yyparse();
}

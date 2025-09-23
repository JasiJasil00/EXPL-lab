%{
    #include <stdio.h>
    #include <stdlib.h>  // for exit()
    int yylex(void);
    void yyerror(const char *s);
%}

%union {
    int val;
}

%token <val> DIGIT
%token NEWLINE
%type <val> expr
%left '+' '-'

%%

start : expr NEWLINE {
            printf("\nComplete\n");
            exit(0);
        }
;

expr : expr '+' expr { printf("+ "); $$ = 0; }
     | expr '-' expr { printf("- "); $$ = 0; }
     | '(' expr ')'  { $$ = $2; }
     | DIGIT         { printf("%d ", $1); $$ = $1; }
;

%%

void yyerror(const char *s) {
    printf("yyerror  %s\n", s);
}
int main() {
    return yyparse();
}

%{
    #include<stdio.h>
    int yyerror();
%}

%token DIGITS;

%%

start: expr '\n' {
                    printf("\nCOMPLETE\n");
                    exit(0);
                }
    ;

expr : expr '+' expr {printf("+ ");}
    | expr '*' expr  {printf("* ");}
    | '(' expr ')'
    | DIGITS         {printf("%d ", $1);}
    ;

%%

int yyerror(){
    printf("error");
}

int main(){
    yyparse();
    return 1;
}
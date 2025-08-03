%{
    #include<stdio.h>
    int yyerror();
%}

%token DIGITS

%%

start : expr '\n' {printf("value: %d\n",$1); exit(0);}
        ;

expr : expr '*' expr {$$=$1*$3;}
    | expr '-' expr {$$=$1-$3;}
    | expr '+' expr {$$=$1+$3;}
    | '(' expr ')'  {$$=$2;}
    | DIGITS        {$$=$1;}
        ;

%%

int yyerror(){
    printf("error");
}

int main(){
    yyparse();
    return 1;
}
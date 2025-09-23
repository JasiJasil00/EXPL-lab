%{
    #include<stdio.h>
    int yyerror();

%}

%union {
    char c;
}

%token <c> ID ADD SUB MUL DIV
%type <c> expr

%left ADD SUB
%left MUL DIV
%%

stmt : expr '\n' {printf("\nCOMPLETE\n"); exit(1);}
        ;

expr : expr ADD expr {printf("+");$$=' ';}
     | expr SUB expr {printf("-");$$=' ';}
     | expr MUL expr {printf("*");$$=' ';}
     | expr DIV expr {printf("/");$$=' ';}
     |  ID {printf("%c",$1); $$ = $1;}
        ;
%%

int yyerror(){
    printf("error");
}

int main(){
    yyparse();
    return 1;
}
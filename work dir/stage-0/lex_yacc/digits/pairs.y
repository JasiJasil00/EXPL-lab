%{
    #include<stdio.h>
    int yyerror();
%}

%token DIGITS

%%

start : pair '\n' {printf("\nCOMPLETE\n");exit(0);} 
        ;

pair :  pair ',' pair 
     |   num ',' num {printf("pair(%d,%d) ",$1,$3);}
        ;
num : DIGITS {$$=$1;}
        ;

%%

 int yyerror(){
    printf("\nerror\n");
 }

int main(){
    yyparse();
    return 1;
}
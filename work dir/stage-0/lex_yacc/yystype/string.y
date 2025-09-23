%{
    #include<stdio.h>
    int yyerror();
%}


%union {
    char *c;
}

%token <c> ID
%token PLUS MINUS MUL DIV

%left PLUS MINUS
%left MUL DIV

%%

start : expr '\n' {printf("\nCOMPLETE\n");exit(1);}
    ;
expr : expr PLUS expr {printf("+ ");}
     | expr MINUS expr {printf("- ");}
     | expr DIV expr {printf("/ ");}
     | expr MUL expr {printf("* ");}
     | ID {printf("%s ", $1); free($1);}
     ;

%%
int yyerror(){
    printf("error");
}
int main(){
    yyparse();
    return 1;
}
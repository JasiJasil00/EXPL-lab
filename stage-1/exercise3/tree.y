%{
    #include <stdlib.h>
	#include <stdio.h>
	#include "tree.h"
	#include "tree.c"
	#include "codegen.c"
	int yylex(void);
%}

%union {
    struct tnode *no;
}

%token NUM PLUS MINUS MUL DIV END
%type <no> expr NUM start END
%left PLUS MINUS
%left MUL DIV

%%

start : expr END  { $$=$1;
                    execute($$);
                    exit(0);
                  }
        ;
expr :   PLUS expr expr		{$$ = makeoperatornode('+',$2,$3);}
	 | MINUS expr expr  	{$$ = makeoperatornode('-',$2,$3);}
	 |  MUL expr expr	{$$ = makeoperatornode('*',$2,$3);}
	 |  DIV expr expr	{$$ = makeoperatornode('/',$2,$3);}
	 | NUM			{$$ = $1;}
	 ;

%%

yyerror(char const *s)
{
    printf("yyerror %s",s);
}

int main(void) {
	yyparse();
	
	return 0;
}
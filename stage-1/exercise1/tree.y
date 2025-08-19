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
                    exit(1);
                  }
        ;
expr :  expr PLUS expr		{$$ = makeoperatornode('+',$1,$3);}
	 | expr MINUS expr  	{$$ = makeoperatornode('-',$1,$3);}
	 | expr MUL expr	{$$ = makeoperatornode('*',$1,$3);}
	 | expr DIV expr	{$$ = makeoperatornode('/',$1,$3);}
	 | '(' expr ')'		{$$ = $2;}
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
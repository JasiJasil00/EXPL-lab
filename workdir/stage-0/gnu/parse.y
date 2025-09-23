%{
	#include <stdio.h>
	#include <stdlib.h>
	//#define YYSTYPE tnode*
	#include "tree.h"
	#include "tree.c"
	extern struct tnode* idptr;
	FILE* targetFile;
	int yylex(void);
	extern FILE *yyin;
	extern char* yytext;
	
%}
%union{
	struct tnode* p;
}
%token NUM PLUS MINUS MUL DIV ID begin END READ WRITE EOL IF then ELSE endif WHILE DO endwhile GT LT LE GE NE EQ ASSIGN BREAK CONTINUE
%type <p> expr Slist Stmt Breakstmt Continuestmt InputStmt OutputStmt AsgStmt Ifstmt Whilestmt ID
%left PLUS MINUS
%left MUL DIV
%nonassoc GT LT LE GE EQ NE
%%
Program : expr EOL { 
    infixtoprefix(targetFile, $1);
    printExit(targetFile);
    return 0;
}
;

Slist : Slist Stmt		{$$=createTree(-1,-1,NULL,tCONNECT,$1,$2,NULL);}
	| Stmt			{$$=$1;} 	
	;

Stmt : InputStmt 		{$$=$1;}
	| OutputStmt 		{$$=$1;}
	| AsgStmt 		{$$=$1;}
	| Ifstmt		{$$=$1;}
	| Whilestmt		{$$=$1;}
	| Breakstmt		{$$=$1;}
	| Continuestmt		{$$=$1;}
	;

Breakstmt : BREAK EOL			{$$=createTree(-1,-1,NULL,tBREAK,NULL,NULL,NULL);}
	;

Continuestmt : CONTINUE EOL		{$$=createTree(-1,-1,NULL,tCONTINUE,NULL,NULL,NULL);}
	;
InputStmt : READ '(' ID ')' EOL	{$$=createTree(-1,-1,NULL,tREAD,$3,NULL,NULL);}  
	;

OutputStmt : WRITE '(' expr ')' EOL	{$$=createTree(-1,-1,NULL,tWRITE,$3,NULL,NULL);} 
	;

AsgStmt : ID ASSIGN expr EOL {$$ = createTree(-1,-1,NULL,tASSIGN,$1,$3,NULL);} 
	;
Ifstmt : IF '(' expr ')' then Slist ELSE Slist endif EOL	{$$=createTree(-1,-1,NULL,tIF,$3,$6,$8);}
	| IF '(' expr ')' then Slist endif EOL		{$$=createTree(-1,-1,NULL,tIF,$3,$6,NULL);}
	;
Whilestmt : WHILE '(' expr ')' DO Slist endwhile EOL		{$$=createTree(-1,-1,NULL,tWHILE,$3,$6,NULL);}
	;
expr : expr PLUS expr		{$$ = createTree(-1,-1,"+",tADD,$1,$3,NULL);}
	 | expr MINUS expr  	{$$ = createTree(-1,-1,"-",tSUB,$1,$3,NULL);}
	 | expr MUL expr	{$$ = createTree(-1,-1,"*",tMUL,$1,$3,NULL);}
	 | expr DIV expr	{$$ = createTree(-1,-1,"/",tDIV,$1,$3,NULL);}
	 | '(' expr ')'	{$$ = $2;}
	 | expr LT expr 		{$$ = createTree(-1,-1,NULL,tLT,$1,$3,NULL);}
	 | expr GT expr 		{$$ = createTree(-1,-1,NULL,tGT,$1,$3,NULL);}
	 | expr LE expr 		{$$ = createTree(-1,-1,NULL,tLE,$1,$3,NULL);}
	 | expr GE expr 		{$$ = createTree(-1,-1,NULL,tGE,$1,$3,NULL);}
	 | expr NE expr 		{$$ = createTree(-1,-1,NULL,tNE,$1,$3,NULL);}
	 | expr EQ expr		{$$ = createTree(-1,-1,NULL,tEQ,$1,$3,NULL);}
	 | NUM			{$$ = createTree(atoi(yytext),tINT,NULL,tNUM,NULL,NULL,NULL);}
	 | ID			{$$=$1;}
	 ;

%%

yyerror(char const *s)
{
    printf("yyerror %s and %s",s,yytext);
}


int main(int argc, char*argv[]) {
	  // Open the output file
    targetFile = fopen("targetFile.xsm", "w");
    if (targetFile == NULL) {
        printf("file error\n");
        exit(1);
    }
    // Open the input file
    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        printf("input file not found\n");
        exit(1);
    }
    
    // Parse the input
    yyparse();
    
    // Close the files
    fclose(yyin);
    fclose(targetFile);
    
    return 0;
}
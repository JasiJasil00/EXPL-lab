%{
	#include <stdio.h>
	#include "y.tab.h"
	#include "tree.h"
	#include <stdlib.h>
	int number;
	char*temp;

%}

%%

begin	{return begin;}
end 	{return END;}
break	{return BREAK;}
continue	{return CONTINUE;}
read 	{return READ;}
write 	{return WRITE;}
then 	{return then;}
else 	{return ELSE;}
endif	{return endif;}
if	{return IF;}
do	{return DO;}
endwhile {return endwhile;}
while	{return WHILE;}
[0-9]+	{return NUM;}
[a-z]+ {yylval.p=createTree(-1,tINT,yytext,tVAR,NULL,NULL,NULL); return ID;}
"+"	{return PLUS;}
"-"	{return MINUS;}
"*"	{return MUL;}
"/"	{return DIV;}
"<="	{return LE;}
">="	{return GE;}
"<"	{return LT;}
">"	{return GT;}
"!="	{return NE;}
"=="	{return EQ;}
"="	{return ASSIGN;}
";" {return EOL;}
[ \t]	{}
[' '] {}
[()]	{return *yytext;}
[\n]	{}
.	{}


%%

int yywrap(void) {
	return 1;
}
                          
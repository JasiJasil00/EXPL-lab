%{
    #include <stdio.h> 
    #include<stdlib.h>
	  #include "y.tab.h"
%}
number [0-9]+
%%

{number}	{
          yylval = atoi(yytext);
          return DIGIT;
        }
"+"	  return *yytext;
"-"	  return *yytext;
[()]	  return *yytext;
[\n]      return NEWLINE;

%%

yywrap()
{
	return 1;
}
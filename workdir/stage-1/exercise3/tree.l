%{
    #include<stdlib.h>
    #include<stdio.h>
    #include"y.tab.h"
    #include"tree.h"
    int number;
%}

%%

"+"    {return PLUS;}
"-"    {return MINUS;}
"*"   { return MUL; }  
"/"    {return DIV;}
[0-9]+ {    number=atoi(yytext);
            yylval.no = makeleafnode(number);
            return NUM;
        }
[ \t]   {}
[\n]   {return END;}
.      {printf("unknown character\n");exit(1);}

%%

int yywrap(void){
    return 1;
}

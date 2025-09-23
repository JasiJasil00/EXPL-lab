%{
    #include <stdio.h>
%}

%%

/*** Rules section ***/

%%

/*** C Code section ***/
int main(void)
{
    yylex();
    return 0;
}
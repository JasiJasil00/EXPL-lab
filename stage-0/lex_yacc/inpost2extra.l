%{
    #include <stdio.h>
    #include <stdlib.h>     // Needed for atoi()
    #include "y.tab.h"
%}

%%

[0-9]+ {
          yylval.val = atoi(yytext);  // Assign value properly
          return DIGIT;
        }
"+"     return *yytext;
"-"     return *yytext;
[()]    return *yytext;
\n      return NEWLINE;

%%

int yywrap() {
    return 1;
}

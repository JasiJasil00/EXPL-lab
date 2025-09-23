%{
    #include "y.tab.h"
    #include "tree.h"
    int number;
%}

%%

"begin"     { return BEGINP; }
"end"       { return ENDP; }
"read"      { return READ; }
"write"     { return WRITE; }
[0-9]+      { number = atoi(yytext);
              yylval.no = createTree(number, intType, NULL, NODE_NUM, NULL, NULL);
              return NUM; }
[a-z]       { char *c = strdup(yytext);
              yylval.no = createTree(0, intType, c, NODE_ID, NULL, NULL);
              return ID; }
"="         { return ASSIGN; }
";"         { return SEMICOLON; }
"("         { return '('; }
")"         { return ')'; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
[ \t\n]     { /* ignore whitespace */ }

.           { printf("Unknown symbol: %s\n", yytext); }

%%

int yywrap(void){ return 1; }

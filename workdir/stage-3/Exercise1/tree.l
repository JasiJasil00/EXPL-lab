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
"if"        { return IF;}
"then"      { return THEN;}
"else"      { return ELSE;}
"endif"    { return ENDIF;}
"while"     { return WHILE;}
"do"        { return DO;}
"endwhile"  { return ENDWHILE;}
"<"         {return LE;}
"<="        {return LEEQ;}
">="        {return GTEQ;}
">"         {return GT;}
"!="        {return NEQ;}
"=="        {return EQ;}
[0-9]+      { number = atoi(yytext);
              yylval.no = createTree(number, intType, NULL, NODE_NUM, NULL, NULL);
              return NUM; }
[a-z]       { char *c = strdup(yytext);
              yylval.no = createTree(0, intType, c, NODE_ID, NULL, NULL);
              return ID; }
"="         { return ASSIGN; }
";"         { return ';'; }
"("         { return '('; }
")"         { return ')'; }
"+"         { return PLUS; }
"-"         { return MINUS; }
"*"         { return MUL; }
"/"         { return DIV; }
[ \t\n]     { /* ignore whitespace */ }

.           {}

%%

int yywrap(void){ return 1; }

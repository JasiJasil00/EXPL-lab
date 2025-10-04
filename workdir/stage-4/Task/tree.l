%{
    #include "y.tab.h"
    #include "tree/tree.h"
    #include "symbol/symbol.h"
    int number;
%}


digit [0-9]
num {digit}+
letter [a-zA-Z]
id {letter}({letter}|{digit})*
string   ["][^"]*["]

%%

"begin"     { return BEGINP; }
"end"       { return ENDP; }
"decl"      {return DECL;}
"enddecl"   {return ENDDECL;}
"read"      { return READ; }
"write"     { return WRITE; }
"if"        { return IF;}
"else"      { return ELSE;}
"then"      { return THEN;}
"endif"    { return ENDIF;}
"while"     { return WHILE;}
"do"        { return DO;}
"endwhile"  { return ENDWHILE;}
"continue"  {return CONTINUE;}
"break"     {return BREAK;}
"repeat"    {return REPEAT;}
"until"     {return UNTIL;}
"int"       {return INT;}
"str"       {return STR;}
"<"         {return LE;}
"<="        {return LEEQ;}
">="        {return GTEQ;}
">"         {return GT;}
"!="        {return NEQ;}
"=="        {return EQ;}
{string}    {   char *c = strdup(yytext);
                yylval.no = createTree(0, strType, c, NODE_STRING, NULL, NULL);
                return STRING;
            }
{num}      { number = atoi(yytext);
              yylval.no = createTree(number, intType, NULL, NODE_NUM, NULL, NULL);
              return NUM; }
{id}       { char *c = strdup(yytext);
              yylval.no = createTree(0, noType, c, NODE_ID, NULL, NULL);
              return ID; }
"="         { return ASSIGN; }
";"         { return ';' ; }
","         {return ',';}
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

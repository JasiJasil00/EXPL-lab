%{
#include <stdlib.h>
#include <string.h>
#include "y.tab.h"

// Forward decl so we don't get "implicit declaration" warning
void yyerror(const char *s);

static char *xstrdup(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *)malloc(n);
    if (p) memcpy(p, s, n);
    return p;
}
%}

%%

[a-zA-Z_][a-zA-Z0-9_]*    { yylval.str = xstrdup(yytext); return ID; }

"+"                        { return PLUS; }
"-"                        { return MINUS; }
"*"                        { return MUL; }
"/"                        { return DIV; }
"("                        { return '('; }
")"                        { return ')'; }

[ \t\r]+                   { /* skip whitespace */ }
\n                         { return END; }

.                          { yyerror("unknown character"); }

%%

int yywrap(void) { return 1; }

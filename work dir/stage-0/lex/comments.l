%{
#include <stdio.h>
%}

%%

// Match multi-line C-style comments
view
"/*"([^*]*\*+)*([^*/][^*]*\*+)*"/"    {
    printf("Multi-line comment: %s\n", yytext);
}

// Match single-line C++-style comments
"//".*                                {
    printf("Single-line comment: %s\n", yytext);
}

// Ignore everything else
.|\n                                  ;

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}

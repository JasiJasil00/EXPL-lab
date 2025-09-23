%{
#include <stdio.h>
%}

%%

// Match words of exactly 3 letters starting with 'A'
A[a-zA-Z]{2}     { printf("%s\n", yytext); }

// Skip everything else
.|\n             { /* ignore other characters */ }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}

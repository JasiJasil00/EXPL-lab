%{
int lines = 0, words = 0, chars = 0;
%}

%%

\n              { lines++; chars++; }
[ \t]+          { chars += yyleng; }      // whitespace, not counted as words
[^ \t\n]+       { words++; chars += yyleng; } // non-whitespace = word
.               { chars++; }              // any other character

%%

int main() {
    yylex();
    printf("Lines: %d\nWords: %d\nCharacters: %d\n", lines, words, chars);
    return 0;
}

int yywrap() {
    return 1;
}

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "tree.h"
    #include "evaluate.c"

    int yyerror(char const *s);
    int yylex(void);
%}

%union {
    struct tnode *no;
}

%token <no> NUM ID
%token READ WRITE
%token PLUS MINUS MUL DIV
%token ASSIGN SEMICOLON
%token BEGINP ENDP
%type <no> expr InputStmt OutputStmt AsgStmt Stmt StmtList Program

%%

Program : BEGINP StmtList ENDP SEMICOLON { $$ = $2;
                                            printf("\n%d\n", evaluate($$));
                                            exit(0);
                                            }
        ;

Stmt    : InputStmt
        | OutputStmt
        | AsgStmt
        ;

StmtList: Stmt
        | StmtList Stmt           { $$ = createTree(0,intType,NULL,NODE_CONNECTOR,$1,$2); }
        ;

InputStmt : READ '(' ID ')' SEMICOLON
          { $$ = createTree(0,intType,NULL,NODE_READ,$3,NULL); }
          ;

OutputStmt: WRITE '(' expr ')' SEMICOLON
          { $$ = createTree(0,intType,NULL,NODE_WRITE,$3,NULL); }
          ;

AsgStmt : ID ASSIGN expr SEMICOLON
        { $$ = createTree(0,intType,NULL,NODE_ASSIGN,$1,$3); }
        ;

expr    : expr PLUS expr   { $$ = createTree(0,intType,NULL,NODE_PLUS,$1,$3); }
        | expr MINUS expr  { $$ = createTree(0,intType,NULL,NODE_MINUS,$1,$3); }
        | expr MUL expr    { $$ = createTree(0,intType,NULL,NODE_MUL,$1,$3); }
        | expr DIV expr    { $$ = createTree(0,intType,NULL,NODE_DIV,$1,$3); }
        | NUM              { $$ = $1; }
        | ID               { $$ = $1; }
        | '(' expr ')'     { $$ = $2; }
        ;
%%

int yyerror(char const *s) {
    printf("yyerror: %s\n", s);
    return 0;
}

int main(int argc, char *argv[]) {
    extern FILE *yyin;

    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("File open failed");
            exit(1);
        }
    } else {
        yyin = stdin;  // fallback to keyboard input
    }

    yyparse();
    return 0;
}

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "tree.h"
    #include "tree.c"
    #include "codegen.c"

    void yyerror(char const *s);
    int yylex(void);
%}

%union {
    struct tnode *no;
}

%token <no> NUM ID
%token READ WRITE
%token PLUS MINUS MUL DIV LE LEEQ GT GTEQ EQ NEQ
%token ASSIGN 
%token BEGINP ENDP ENDWHILE DO ENDIF IF WHILE THEN ELSE
%type <no> expr InputStmt OutputStmt AsgStmt Stmt StmtList Program Ifstmt Whilestmt

%left PLUS MINUS
%left MUL DIV
%nonassoc LE LEEQ GT GTEQ EQ NEQ

%%

Program : BEGINP StmtList ENDP ';' { $$ = $2; execute($$); exit(0);}
        ;

StmtList: Stmt
        | StmtList Stmt           { $$ = createTree(0,intType,NULL,NODE_CONNECTOR,$1,$2); }
        ;

Stmt    : InputStmt         {$$=$1;}
        | Ifstmt            {$$=$1;}
        | Whilestmt         {$$=$1;}
        | OutputStmt        {$$=$1;}
        | AsgStmt           {$$=$1;}
        ;

Ifstmt
    : IF '(' expr ')' THEN StmtList ELSE StmtList ENDIF ';'
        { struct tnode *connect = createTree(0,intType,NULL,NODE_CONNECTOR,$6,$8);
           
            if($3->type==intType){
               yyerror("node type match error if condition");
               exit(0);
            }
          $$ = createTree(0,boolType,NULL,NODE_IF,$3,connect); }
    | IF '(' expr ')' THEN StmtList ENDIF ';'
        { if($3->type==intType){
               yyerror("node type match error if condition");
               exit(0);
            }
            struct tnode *connect = createTree(0,intType,NULL,NODE_CONNECTOR,$6,NULL);
            $$ = createTree(0,boolType,NULL,NODE_IF,$3,connect); }
    ;

Whilestmt
    : WHILE '(' expr ')' DO StmtList ENDWHILE
        {   if($3->type!=boolType){
                yyerror("node type match error while condition");
                    exit(0);
        }
            $$ = createTree(0,boolType,NULL,NODE_WHILE,$3,$6); }
    ;



InputStmt : READ '(' ID ')' ';'
          { $$ = createTree(0,intType,NULL,NODE_READ,$3,NULL); }
          ;

OutputStmt: WRITE '(' expr ')' ';'
          { if($3->type!=intType){
            yyerror("node type match error expression");
            exit(0);
          }
            $$ = createTree(0,intType,NULL,NODE_WRITE,$3,NULL); }
          ;

AsgStmt : ID ASSIGN expr ';'
        {  if($3->type!=intType){
                yyerror("node type match error assignment");
                exit(0);
            }
            $$ = createTree(0,intType,NULL,NODE_ASSIGN,$1,$3); }
        ;


expr
    : expr PLUS expr      {  if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error expression");
                                    exit(0);
                                }
                                 $$ = createTree(0,intType,NULL,NODE_PLUS,$1,$3); }
    | expr MINUS expr     { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error expression");
                                    exit(0);
                                }
        $$ = createTree(0,intType,NULL,NODE_MINUS,$1,$3); }
    | expr MUL expr       { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error expression");
                                    exit(0);
                                }
                                $$ = createTree(0,intType,NULL,NODE_MUL,$1,$3); }
    | expr DIV expr       { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error expression");
                                    exit(0);
                                }
                                $$ = createTree(0,intType,NULL,NODE_DIV,$1,$3); }
    | expr LE expr        { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error logic");
                                    exit(0);
                                }
                                $$ = createTree(0,boolType,NULL,NODE_LESS,$1,$3); }
    | expr LEEQ expr      { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error logic");
                                    exit(0);
                                }
                                $$ = createTree(0,boolType,NULL,NODE_LESSOREQUAL,$1,$3); }
    | expr GT expr        { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error logic");
                                    exit(0);
                                }
                                $$ = createTree(0,boolType,NULL,NODE_MORE,$1,$3); }
    | expr GTEQ expr      { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error logic");
                                    exit(0);
                                }
                                $$ = createTree(0,boolType,NULL,NODE_MOREOREQUAL,$1,$3); }
    | expr EQ expr        { if($1->type!=intType || $3->type!=intType){
                                    yyerror("node type match error logic");
                                    exit(0);
                                }
                                $$ = createTree(0,boolType,NULL,NODE_EQUAL,$1,$3); }
    | expr NEQ expr       { if($1->type!=intType || $3->type!=intType){
                                     yyerror("node type match error logic");
                                    exit(0);
                                }
                                $$ = createTree(0,boolType,NULL,NODE_NOTEQUAL,$1,$3); }
    | NUM                 { $$ = $1; }
    | ID                  { $$ = $1; }
    | '(' expr ')'        { $$ = $2; }
    ;

%%

void yyerror(char const *s) {
    printf("yyerror: %s\n",s);
    exit(0);
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

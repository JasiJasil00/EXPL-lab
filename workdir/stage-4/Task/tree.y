%{
    #include <stdio.h>
    #include <stdlib.h>
    void yyerror(char const *s);
    #include "tree/tree.h"
    #include "tree/tree.c"
    #include "symbol/symbol.h"
    #include "symbol/symbol.c"
    #include "codegen/codegen.h"
    #include "codegen/codegen.c"
    int yylex(void);
%}

%union {
    struct tnode *no;
    int type;
}

%token <no> NUM ID STRING
%token READ WRITE
%token PLUS MINUS MUL DIV LE LEEQ GT GTEQ EQ NEQ
%token ASSIGN 
%token DECL ENDDECL INT STR
%token BEGINP ENDP ENDWHILE DO ENDIF IF WHILE THEN ELSE BREAK CONTINUE REPEAT UNTIL
%type <no> expr InputStmt OutputStmt AsgStmt Stmt StmtList Program Ifstmt Whilestmt RepeatStmt DoWhileStmt valtypes
%type <type> VarList Type
%left PLUS MINUS
%left MUL DIV
%nonassoc LE LEEQ GT GTEQ EQ NEQ

%%

Program : BEGINP Declarations StmtList ENDP ';' { $$ = $3; execute($3); exit(1);}
        | BEGINP StmtList ENDP ';' {$$=$2; execute($2); exit(0);}
        ;

Declarations : DECL DeclList ENDDECL  {printsymboltable();}
            | DECL ENDDECL {}
            ;

DeclList : DeclList Decl  {}
            | Decl       {}
            ;
Decl : Type VarList ';'  {}
        ;

Type : INT     { $$= intType;}
    | STR      { $$= strType;}
    ;

VarList : VarList ',' ID  
            { Install($3->varname, $1, 1); $$ = $1; }
        | ID
            { Install($1->varname, $<type>0, 1); $$ = $<type>0;  }
        ;

StmtList: Stmt
        | StmtList Stmt           { $$ = createTree(0,noType,NULL,NODE_CONNECTOR,$1,$2); }
        ;

Stmt    : InputStmt         {$$=$1;}
        | Ifstmt            {$$=$1;}
        | Whilestmt         {$$=$1;}
        | RepeatStmt        { $$=$1; }
        | DoWhileStmt       { $$=$1; }
        | OutputStmt        {$$=$1;}
        | AsgStmt           {$$=$1;}
        | BREAK ';'         {$$=createTree(0,noType,NULL,NODE_BREAK,NULL,NULL);}
        | CONTINUE ';'      {$$=createTree(0,noType,NULL,NODE_CONTINUE,NULL,NULL);}
        ;

Ifstmt
    : IF '(' expr ')' THEN StmtList ELSE StmtList ENDIF ';'
        { struct tnode *connect = createTree(0,intType,NULL,NODE_CONNECTOR,$6,$8);
          $$ = createTree(0,boolType,NULL,NODE_IF,$3,connect); }
    | IF '(' expr ')' THEN StmtList ENDIF ';'
        {
            struct tnode *connect = createTree(0,intType,NULL,NODE_CONNECTOR,$6,NULL);
            $$ = createTree(0,boolType,NULL,NODE_IF,$3,connect); }
    ;

Whilestmt
    : WHILE '(' expr ')' DO StmtList ENDWHILE ';'
        {  
            $$ = createTree(0,boolType,NULL,NODE_WHILE,$3,$6); }
    ;

RepeatStmt : REPEAT StmtList UNTIL '(' expr ')' ';'
            {   
                $$ = createTree(0, boolType, NULL, NODE_REPEATUNTIL, $5, $2);
            }
            ;

DoWhileStmt
    : DO StmtList WHILE '(' expr ')' ';'
      { 
        $$ = createTree(0, boolType, NULL, NODE_DOWHILE, $5, $2); }
    ;

InputStmt : READ '(' ID ')' ';'
          { $3->Gentry=Lookup($3->varname); if(!$3->Gentry) yyerror("no entry in symbol table"); $3->type=$3->Gentry->type; 
            $$ = createTree(0,$3->type,NULL,NODE_READ,$3,NULL); }
          ;

OutputStmt: WRITE '(' valtypes ')' ';'
          { 
            $$ = createTree(0,$3->type,NULL,NODE_WRITE,$3,NULL); }
          ;

AsgStmt : ID ASSIGN valtypes ';'
        {   $1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;
            $$ = createTree(0,$1->type,NULL,NODE_ASSIGN,$1,$3); }
        ;

valtypes :  expr {$$=$1;}
         |  STRING {$$=$1;}
         ;

expr
    : expr PLUS expr      { 
                                 $$ = createTree(0,intType,NULL,NODE_PLUS,$1,$3); }
    | expr MINUS expr     { 
                                $$ = createTree(0,intType,NULL,NODE_MINUS,$1,$3); }
    | expr MUL expr       { 
                                $$ = createTree(0,intType,NULL,NODE_MUL,$1,$3); }
    | expr DIV expr       { 
                                $$ = createTree(0,intType,NULL,NODE_DIV,$1,$3); }
    | expr LE expr        { 
                                $$ = createTree(0,boolType,NULL,NODE_LESS,$1,$3); }
    | expr LEEQ expr      { 
                                $$ = createTree(0,boolType,NULL,NODE_LESSOREQUAL,$1,$3); }
    | expr GT expr        { 
                                $$ = createTree(0,boolType,NULL,NODE_MORE,$1,$3); }
    | expr GTEQ expr      { 
                                $$ = createTree(0,boolType,NULL,NODE_MOREOREQUAL,$1,$3); }
    | expr EQ expr        {
                                $$ = createTree(0,boolType,NULL,NODE_EQUAL,$1,$3); }
    | expr NEQ expr       { 
                                $$ = createTree(0,boolType,NULL,NODE_NOTEQUAL,$1,$3); }
    | NUM                 { $$ = $1; }
    | ID                  {  $1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;  $$ = $1; }
    | '(' expr ')'        { $$ = $2; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "yyerror: %s\n", s);
    fflush(stderr);
    exit(1);   // better than exit(0)
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

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
    int currentType;
    int yylex(void);
%}

%union {
    struct tnode *no;
    int type;
}

%token <no> NUM ID STRING
%token READ WRITE
%token PLUS MINUS MUL DIV LE LEEQ GT GTEQ EQ NEQ MOD
%token ASSIGN 
%token DECL ENDDECL INT STR
%token BEGINP ENDP ENDWHILE DO ENDIF IF WHILE THEN ELSE BREAK CONTINUE REPEAT UNTIL
%type <no> expr InputStmt OutputStmt AsgStmt Stmt StmtList Program Ifstmt Whilestmt RepeatStmt DoWhileStmt valtype readtype assigntype aexpr simplevar arrayref pointerref array2dref 
%type <no> NormalAssign PointerAssign
%type <type> VarList Type decltype
%left PLUS MINUS
%left MOD
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
Decl : Type VarList ';'  { $2=$1; }
        ;

Type : INT     { currentType = intType; $$= intType;}
    | STR      { currentType = strType; $$= strType;}
    ;

VarList : VarList ',' decltype
        | decltype
        ;
 
decltype : ID '[' NUM ']'  {Install($1->varname,currentType,$3->val,0,NODE_ARRAY);}
        |  ID '[' NUM ']' '[' NUM ']' { Install($1->varname,currentType,$3->val,$6->val,NODE_2D_ARRAY);} 
        |   ID              {Install($1->varname,currentType,1,0,NODE_VAR);}
        | MUL ID        {Install($2->varname,currentType,0,0,NODE_PTR_VAR);}
        ;
StmtList: Stmt
        | StmtList Stmt           { $$ = createTree(0,noType,NULL,NODE_CONNECTOR,NULL,$1,$2); }
        ;

Stmt    : InputStmt         {$$=$1;}
        | Ifstmt            {$$=$1;}
        | Whilestmt         {$$=$1;}
        | RepeatStmt        { $$=$1; }
        | DoWhileStmt       { $$=$1; }
        | OutputStmt        {$$=$1;}
        | AsgStmt           {$$=$1;}
        | BREAK ';'         {$$=createTree(0,noType,NULL,NODE_BREAK,NULL,NULL,NULL);}
        | CONTINUE ';'      {$$=createTree(0,noType,NULL,NODE_CONTINUE,NULL,NULL,NULL);}
        ;

Ifstmt
    : IF '(' aexpr ')' THEN StmtList ELSE StmtList ENDIF ';'
                                                            { struct tnode *connect = createTree(0,intType,NULL,NODE_CONNECTOR,NULL,$6,$8);
                                                            $$ = createTree(0,boolType,NULL,NODE_IF,NULL,$3,connect); }
    | IF '(' aexpr ')' THEN StmtList ENDIF ';'
                                            {   
                                                struct tnode *connect = createTree(0,intType,NULL,NODE_CONNECTOR,NULL,$6,NULL);
                                                $$ = createTree(0,boolType,NULL,NODE_IF,NULL,$3,connect); 
                                            }
    ;

Whilestmt
    : WHILE '(' aexpr ')' DO StmtList ENDWHILE ';'
                                                    {  
                                                        $$ = createTree(0,boolType,NULL,NODE_WHILE,NULL,$3,$6);
                                                     }
    ;

RepeatStmt : REPEAT StmtList UNTIL '(' aexpr ')' ';'
                                                        {   
                                                            $$ = createTree(0, boolType, NULL, NODE_REPEATUNTIL, NULL, $5, $2);
                                                        }
            ;

DoWhileStmt
    : DO StmtList WHILE '(' aexpr ')' ';'
                                        { 
                                          $$ = createTree(0, boolType, NULL, NODE_DOWHILE, NULL, $5, $2); 
                                        }
    ;

InputStmt : READ '(' readtype ')' ';'
                                { 
                                   $$ = createTree(0,$3->type,NULL,NODE_READ,NULL,$3,NULL); 
                                }
             ;



readtype : ID {$1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table");  $1->nodetype=$1->Gentry->array; $1->type=$1->Gentry->type; $$=$1; if($1->nodetype==NODE_PTR_VAR) yyerror("its a pointer"); }
          | ID '[' expr ']' {$1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;  $$ = createTree(0,$1->type,$1->varname,NODE_ARRAY,$1->Gentry,$1,$3);}
         |   ID '[' expr ']' '[' expr ']' {$1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;  $$ = createTree(0,$1->type,$1->varname,NODE_2D_ARRAY,$1->Gentry,$3,$6); }
         | MUL ID              {$2->Gentry=Lookup($2->varname); if(!$2->Gentry)yyerror("no entry in symbol table"); if($2->Gentry->binding==0)yyerror("pointer not assigned"); $2->type=$2->Gentry->type; $$ = createTree(0,$2->type,$2->varname,NODE_PTR,$2->Gentry,$2,NULL); }
         ;

OutputStmt: WRITE '(' valtype ')' ';'  
                                        { 
                                            $$ = createTree(0,$3->type,NULL,NODE_WRITE,NULL,$3,NULL); }
          ;

AsgStmt : NormalAssign {$$=$1;}
         | PointerAssign
        ;

NormalAssign: assigntype ASSIGN valtype ';'
                                        { 
                                            $$ = createTree(0,$1->type,NULL,NODE_ASSIGN,NULL,$1,$3); }
            ;   
PointerAssign: ID ASSIGN  '&' ID ';' {
                                    $$ = createTree(0,$1->type,NULL,NODE_PTR_ASSIGN,NULL,$1,$4);
                                     }
              | ID ASSIGN valtype ';' {
                $1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;  $1->nodetype=$1->Gentry->array; 
                $$ = createTree(0,$1->type,NULL,NODE_ASSIGN,NULL,$1,$3);
              }
                ;
assigntype : pointerref {$$=$1;  }
           | array2dref {$$=$1;}
           |  arrayref {$$=$1;}
           ;


valtype :  expr {$$=$1;}
         |  STRING {$$=$1;}
         ;

aexpr : expr LE expr        { 
                                $$ = createTree(0,boolType,NULL,NODE_LESS,NULL,$1,$3); }
    | expr LEEQ expr      { 
                                $$ = createTree(0,boolType,NULL,NODE_LESSOREQUAL,NULL,$1,$3); }
    | expr GT expr        { 
                                $$ = createTree(0,boolType,NULL,NODE_MORE,NULL,$1,$3); }
    | expr GTEQ expr      { 
                                $$ = createTree(0,boolType,NULL,NODE_MOREOREQUAL,NULL,$1,$3); }
    | expr EQ expr        {
                                $$ = createTree(0,boolType,NULL,NODE_EQUAL,NULL,$1,$3); }
    | expr NEQ expr       { 
                                $$ = createTree(0,boolType,NULL,NODE_NOTEQUAL,NULL,$1,$3); }
    ;

expr
    : expr PLUS expr      { 
                                 $$ = createTree(0,intType,NULL,NODE_PLUS,NULL,$1,$3); }
    | expr MINUS expr     { 
                                $$ = createTree(0,intType,NULL,NODE_MINUS,NULL,$1,$3); }
    | expr MUL expr       { 
                                $$ = createTree(0,intType,NULL,NODE_MUL,NULL,$1,$3); }
    | expr DIV expr       { 
                                $$ = createTree(0,intType,NULL,NODE_DIV,NULL,$1,$3); }
    |expr MOD expr        {
                                $$ = createTree(0,intType,NULL,NODE_MOD,NULL,$1,$3);
                            }
    | NUM                 { $$ = $1; }
    | '(' expr ')'        { $$ = $2; }
    |pointerref {$$=$1;}
    | simplevar  {$$=$1;}
    | array2dref {$$=$1;}
    |  arrayref {$$=$1;}
   ;

simplevar : ID {$1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;  $1->nodetype=$1->Gentry->array; $$ = $1; };
            ;
arrayref : ID '[' expr ']' {$1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;  $$ = createTree(0,$1->type,NULL,NODE_ARRAY,$1->Gentry,$1,$3);}
            ;
array2dref :  ID '[' expr ']' '[' expr ']' {$1->Gentry=Lookup($1->varname); if(!$1->Gentry) yyerror("no entry in symbol table"); $1->type=$1->Gentry->type;  $$ = createTree(0,$1->type,$1->varname,NODE_2D_ARRAY,$1->Gentry,$3,$6); }
            ;

pointerref  : MUL ID              {$2->Gentry=Lookup($2->varname); if(!$2->Gentry)yyerror("no entry in symbol table"); if($2->Gentry->binding==0)yyerror("pointer not assigned"); $2->type=$2->Gentry->type; $$ = createTree(0,$2->type,$2->varname,NODE_PTR,$2->Gentry,$2,NULL); }
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

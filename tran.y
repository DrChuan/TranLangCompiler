%{
#include <stdio.h>
#include "utility.h"
#include "AST.h"

AST tree;

extern int lines;
extern int letters;

extern "C"
{
    void yyerror(const char *s)
    {
        printf("%d-%d : %s", lines, letters, s);
    }
    extern int yylex(void);
}


%}


%type<node> start program function type para_list para declaration statements statement dcl_statement if_statement loop_statement exp_statement initialize exp while_loop else_part literal return_statement func_call args lexp class_def class_items class_item
%token<node> FUNC ENDFUNC ID INT DOUBLE STRING VOID EPSILON SEMI INIT IF ENDIF ELSE ELSIF WHILE ENDWHILE INT_LITERAL DOUBLE_LITERAL STRING_LITERAL RETURN CLASS ENDCLASS
%left<node> COMMA
%right<node> ASSIGN
%left<node> AND OR
%left<node> EQUAL NEQ GEQ LEQ GREATER LESS
%left<node> ADD SUB
%left<node> MUL DIV MOD
%right<node> NOT
%left<node> LP RP LB RB DOT

%%
start : program { $$ = (new ASTNode(start, generateVector(1, $1)))->simplify(); tree = AST($$); };
program : function program  { $$ = new ASTNode(program, generateVector(2, $1, $2)); }
        | class_def program { $$ = new ASTNode(program, generateVector(2, $1, $2)); }
        |                   { $$ = NULL; }
        ;
function : FUNC type ID LP para_list RP statements ENDFUNC { $$ = (new ASTNode(function, generateVector(4, $2, $3, $5, $7)))->simplify(); }
         | FUNC type LB RB ID LP para_list RP statements ENDFUNC { $$ = (new ASTNode(function, generateVector(5, $2, $5, $7, $9, $3)))->simplify(); }
         ;
type : INT    { $$ = new ASTNode(type, generateVector(1, $1)); }
     | DOUBLE { $$ = new ASTNode(type, generateVector(1, $1)); }
     | STRING { $$ = new ASTNode(type, generateVector(1, $1)); }
     | VOID   { $$ = new ASTNode(type, generateVector(1, $1)); }
     | ID     { $$ = new ASTNode(type, generateVector(1, $1)); }
     ;
para_list : para COMMA para_list { $$ = new ASTNode(para_list, generateVector(2, $1, $3)); }
          | para                 { $$ = new ASTNode(para_list, generateVector(1, $1)); }
          |                      { $$ = new ASTNode(para_list, generateVector(0)); }
          ;
para : type ID { $$ = new ASTNode(para, generateVector(2, $1, $2)); }
     | type LB RB ID { $$ = new ASTNode(para, generateVector(3, $1, $2, $4)); }
     ;
declaration : type ID                   { $$ = new ASTNode(declaration, generateVector(2, $1, $2)); }
            | type ID LB RB { $$ = new ASTNode(declaration, generateVector(3, $1, $2, $3)); }
            ;
statements : statement statements { $$ = new ASTNode(statements, generateVector(2, $1, $2)); }
           | statement            { $$ = new ASTNode(statements, generateVector(1, $1)); }
           ;
statement : dcl_statement SEMI    { $$ = $1; }
          | if_statement          { $$ = $1; }
          | loop_statement        { $$ = $1; }
          | exp_statement SEMI    { $$ = $1; }
          | return_statement SEMI { $$ = $1; }
          ;
dcl_statement : declaration initialize { $$ = new ASTNode(dcl_statement, generateVector(2, $1, $2)); }
              ;
initialize : INIT exp { $$ = new ASTNode(initialize, generateVector(1, $2)); }
           |          { $$ = NULL; }
           ;
lexp : ID           { $$ = new ASTNode(lexp, generateVector(1, $1)); }
     | ID LB exp RB { $$ = new ASTNode(lexp, generateVector(2, $1, $3)); }
     | exp DOT ID   { $$ = new ASTNode(lexp, generateVector(3, $1, $2, $3)); }
     ;
exp : lexp ASSIGN exp { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp ADD exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp SUB exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp MUL exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp DIV exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp MOD exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp EQUAL exp   { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp NEQ exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }    
    | exp GREATER exp { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp LESS exp    { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp GEQ exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp LEQ exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp AND exp     { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | exp OR exp      { $$ = new ASTNode(exp, generateVector(3, $1, $2, $3)); }
    | NOT exp         { $$ = new ASTNode(exp, generateVector(2, $1, $2)); }
    | LP exp RP       { $$ = $2; }
    | lexp            { $$ = new ASTNode(exp, generateVector(1, $1)); }
    | literal         { $$ = new ASTNode(exp, generateVector(1, $1)); $$->setType($1); }
    | func_call       { $$ = new ASTNode(exp, generateVector(1, $1)); }
    ;
func_call : ID LP args RP { $$ = (new ASTNode(func_call, generateVector(2, $1, $3)))->simplify(); }
args : exp COMMA args { $$ = new ASTNode(args, generateVector(2, $1, $3)); }
     | exp            { $$ = new ASTNode(args, generateVector(1, $1)); }
     |                { $$ = NULL; }
     ;
literal : INT_LITERAL    { $$ = new ASTNode(literal, generateVector(1, $1)); $$->setType($1); }
        | DOUBLE_LITERAL { $$ = new ASTNode(literal, generateVector(1, $1)); $$->setType($1); }
        | STRING_LITERAL { $$ = new ASTNode(literal, generateVector(1, $1)); $$->setType($1); }
        ;
if_statement : IF LP exp RP statements else_part ENDIF { $$ = (new ASTNode(if_statement, generateVector(3, $3, $5, $6)))->simplify(); }
             ;
else_part : ELSIF LP exp RP statements else_part { $$ = (new ASTNode(else_part, generateVector(3, $3, $5, $6)))->simplify(); }
          | ELSE statements                      { $$ = (new ASTNode(else_part, generateVector(1, $2)))->simplify(); }
          |                                      { $$ = NULL; }
          ;
loop_statement : while_loop { $$ = $1; }
               ;
while_loop : WHILE LP exp RP statements ENDWHILE { $$ = (new ASTNode(while_loop, generateVector(2, $3, $5)))->simplify(); }
           ;
exp_statement : exp { $$ = new ASTNode(exp_statement, generateVector(1, $1)); }
              ;
return_statement : RETURN exp { $$ = new ASTNode(return_statement, generateVector(1, $2)); }
                 | RETURN     { $$ = new ASTNode(return_statement, generateVector(0)); }
                 ;
class_def : CLASS ID class_items ENDCLASS { $$ = (new ASTNode(class_def, generateVector(2, $2, $3)))->simplify(); }
          ;
class_items : class_item class_items { $$ = new ASTNode(class_items, generateVector(2, $1, $2)); }
            |                        { $$ = NULL; }
            ;
class_item : dcl_statement SEMI { $$ = $1; }
           ;

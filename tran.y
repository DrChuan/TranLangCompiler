%{
#include <stdio.h>
#include "utility.h"

ASTNode *root;

extern int lines;
extern int letters;

int yylex();
void yyerror(char *s)
{
    printf("%d-%d : %s\n", lines, letters, s);
}

enum {
    start, program, function, type, para_list, para, declaration, statements, statement, dcl_statement, if_statement, loop_statement, 
    exp_statement, initialize, exp, while_loop, else_part, literal, return_statement, func_call, args, lexp, class_def, class_items, 
    class_item, access, ctor_def
};

%}

%union {
    struct Node *node;
}

%type<node> start program function type para_list para declaration statements statement dcl_statement if_statement loop_statement exp_statement initialize exp while_loop else_part literal return_statement func_call args lexp class_def class_items class_item access ctor_def
%token<node> FUNC ENDFUNC ID INT DOUBLE STRING VOID EPSILON SEMI INIT IF ENDIF ELSE ELSIF WHILE ENDWHILE INT_LITERAL DOUBLE_LITERAL STRING_LITERAL RETURN CLASS ENDCLASS PRIVATE PUBLIC PROTECTED INHERIT THIS CTOR ENDCTOR
%left<node> COMMA
%right<node> ASSIGN
%left<node> AND OR
%left<node> EQUAL NEQ GEQ LEQ GREATER LESS
%left<node> ADD SUB
%left<node> MUL DIV MOD
%right<node> NOT
%left<node> LP RP LB RB

%%
start : program { $$ = createNormalNode(start, 1, $1); root = $$; };
program : function program  { $$ = createNormalNode(program, 2, $1, $2); }
        | class_def program { $$ = createNormalNode(program, 2, $1, $2); }
        |                   { $$ = NULL; }
        ;
function : FUNC type ID LP para_list RP statements ENDFUNC { $$ = createNormalNode(function, 4, $2, $3, $5, $7); }
        ;
type : INT    { $$ = createNormalNode(type, 1, $1); }
     | DOUBLE { $$ = createNormalNode(type, 1, $1); }
     | STRING { $$ = createNormalNode(type, 1, $1); }
     | VOID   { $$ = createNormalNode(type, 1, $1); }
     | ID     { $$ = createNormalNode(type, 1, $1); }
     ;
para_list : para COMMA para_list { $$ = createNormalNode(para_list, 2, $1, $3); }
          | para                 { $$ = createNormalNode(para_list, 1, $1); }
          |                      { $$ = NULL; }
          ;
para : type ID { $$ = createNormalNode(para, 2, $1, $2); }
     | type LB RB ID { $$ = createNormalNode(para, 3, $1, $2, $4); }
     ;
declaration : type ID                   { $$ = createNormalNode(declaration, 2, $1, $2); }
            | type ID LB INT_LITERAL RB { $$ = createNormalNode(declaration, 3, $1, $2, $4); }
            ;
statements : statement statements { $$ = createNormalNode(statements, 2, $1, $2); }
           |                      { $$ = NULL; }
           ;
statement : dcl_statement SEMI    { $$ = createNormalNode(statement, 1, $1); }
          | if_statement          { $$ = createNormalNode(statement, 1, $1); }
          | loop_statement        { $$ = createNormalNode(statement, 1, $1); }
          | exp_statement SEMI    { $$ = createNormalNode(statement, 1, $1); }
          | return_statement SEMI { $$ = createNormalNode(statement, 1, $1); }
          ;
dcl_statement : declaration initialize { $$ = createNormalNode(dcl_statement, 2, $1, $2); }
              ;
initialize : INIT exp { $$ = createNormalNode(initialize, 1, $2); }
           |          { $$ = NULL; }
           ;
lexp : ID           { $$ = createNormalNode(lexp, 1, $1); }
     | ID LB exp RB { $$ = createNormalNode(lexp, 2, $1, $3); }
     ;
exp : lexp ASSIGN exp { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp ADD exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp SUB exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp MUL exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp DIV exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp MOD exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp EQUAL exp   { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp NEQ exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }    
    | exp GREATER exp { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp LESS exp    { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp GEQ exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp LEQ exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp AND exp     { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | exp OR exp      { $$ = createNormalNode(exp, 3, $1, $2, $3); }
    | NOT exp         { $$ = createNormalNode(exp, 2, $1, $2); }
    | LP exp RP       { $$ = createNormalNode(exp, 1, $2); }
    | lexp            { $$ = createNormalNode(exp, 1, $1); }
    | literal         { $$ = createNormalNode(exp, 1, $1); $$->val.type = $1->val.type; }
    | func_call       { $$ = createNormalNode(exp, 1, $1); }
    ;
func_call : ID LP args RP { $$ = createNormalNode(func_call, 2, $1, $3); }
args : exp COMMA args { $$ = createNormalNode(args, 2, $1, $3); }
     | exp            { $$ = createNormalNode(args, 1, $1); }
     |                { $$ = NULL; }
     ;
literal : INT_LITERAL    { $$ = createNormalNode(literal, 1, $1); $$->val.type = $1->val.type; }
        | DOUBLE_LITERAL { $$ = createNormalNode(literal, 1, $1); $$->val.type = $1->val.type; }
        | STRING_LITERAL { $$ = createNormalNode(literal, 1, $1); $$->val.type = $1->val.type; }
        ;
if_statement : IF LP exp RP statements else_part ENDIF { $$ = createNormalNode(if_statement, 3, $3, $5, $6); }
             ;
else_part : ELSIF LP exp RP statements else_part { $$ = createNormalNode(else_part, 3, $3, $5, $6); }
          | ELSE statements                      { $$ = createNormalNode(else_part, 1, $2); }
          |                                      { $$ = NULL; }
          ;
loop_statement : while_loop { $$ = createNormalNode(loop_statement, 1, $1); }
               ;
while_loop : WHILE LP exp RP statements ENDWHILE { $$ = createNormalNode(while_loop, 2, $3, $5); }
           ;
exp_statement : exp { $$ = createNormalNode(exp_statement, 1, $1); }
              ;
return_statement : RETURN exp { $$ = createNormalNode(return_statement, 1, $1); }
                 | RETURN     { $$ = createNormalNode(return_statement, 1, createNormalNode(EPSILON, 0)); }
                 ;
class_def : CLASS ID class_items ENDCLASS { $$ = createNormalNode(class_def, 2, $2, $3); }
          | CLASS ID INHERIT ID class_items ENDCLASS { $$ = createNormalNode(class_def, 4, $2, $3, $4, $5); }
          ;
class_items : class_item class_items { $$ = createNormalNode(class_items, 2, $1, $2); }
            |                        { $$ = NULL; }
            ;
class_item : access declaration SEMI { $$ = createNormalNode(class_item, 2, $1, $2); }
           | access function { $$ = createNormalNode(class_item, 2, $1, $2); }
           | access ctor_def { $$ = createNormalNode(class_item, 2, $1, $2); }
           ;
access : PUBLIC { $$ = createNormalNode(access, 1, $1); }
       | PRIVATE { $$ = createNormalNode(access, 1, $1); }
       | PROTECTED { $$ = createNormalNode(access, 1, $1); }
       ;
ctor_def : CTOR LP para_list RP statements ENDCTOR { $$ = createNormalNode(ctor_def, 2, $3, $5); }
         ;
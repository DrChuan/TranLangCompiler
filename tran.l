%{
#include "y.tab.h"
#include "utility.h"

int lines = 1;
int letters = 0;

int yywrap(void)
{
}
%}

%%
\n   { lines += 1; letters = 0; }
\(   { letters += yyleng; yylval.node = createNormalNode(LP, 0); return LP; }
\)   { letters += yyleng; yylval.node = createNormalNode(RP, 0); return RP; }
\[   { letters += yyleng; yylval.node = createNormalNode(LB, 0); return LB; }
\]   { letters += yyleng; yylval.node = createNormalNode(RB, 0); return RB; }
\,   { letters += yyleng; yylval.node = createNormalNode(COMMA, 0); return COMMA; }
\;   { letters += yyleng; yylval.node = createNormalNode(SEMI, 0); return SEMI; }
\:\= { letters += yyleng; yylval.node = createNormalNode(INIT, 0); return INIT; }
\=   { letters += yyleng; yylval.node = createNormalNode(ASSIGN, 0); return ASSIGN; }
\+   { letters += yyleng; yylval.node = createNormalNode(ADD, 0); return ADD; }
\-   { letters += yyleng; yylval.node = createNormalNode(SUB, 0); return SUB; }
\*   { letters += yyleng; yylval.node = createNormalNode(MUL, 0); return MUL; }
\/   { letters += yyleng; yylval.node = createNormalNode(DIV, 0); return DIV; }
\%   { letters += yyleng; yylval.node = createNormalNode(MOD, 0); return MOD; }
\=\= { letters += yyleng; yylval.node = createNormalNode(EQUAL, 0); return EQUAL; }
\!\= { letters += yyleng; yylval.node = createNormalNode(NEQ, 0); return NEQ; }
\>   { letters += yyleng; yylval.node = createNormalNode(GREATER, 0); return GREATER; }
\<   { letters += yyleng; yylval.node = createNormalNode(LESS, 0); return LESS; }
\>\= { letters += yyleng; yylval.node = createNormalNode(GEQ, 0); return GEQ; }
\<\= { letters += yyleng; yylval.node = createNormalNode(LEQ, 0); return LEQ; }
func      { letters += yyleng; yylval.node = createNormalNode(FUNC, 0); return FUNC; }
endfunc   { letters += yyleng; yylval.node = createNormalNode(ENDFUNC, 0); return ENDFUNC; }
if        { letters += yyleng; yylval.node = createNormalNode(IF, 0); return IF; }
endif     { letters += yyleng; yylval.node = createNormalNode(ENDIF, 0); return ENDIF; }
else      { letters += yyleng; yylval.node = createNormalNode(ELSE, 0); return ELSE; }
elsif     { letters += yyleng; yylval.node = createNormalNode(ELSIF, 0); return ELSIF; }
while     { letters += yyleng; yylval.node = createNormalNode(WHILE, 0); return WHILE; }
endwhile  { letters += yyleng; yylval.node = createNormalNode(ENDWHILE, 0); return ENDWHILE; }
int       { letters += yyleng; yylval.node = createNormalNode(INT, 0); return INT; }
double    { letters += yyleng; yylval.node = createNormalNode(DOUBLE, 0); return DOUBLE; }
string    { letters += yyleng; yylval.node = createNormalNode(STRING, 0); return STRING; }
void      { letters += yyleng; yylval.node = createNormalNode(VOID, 0); return VOID; }
return    { letters += yyleng; yylval.node = createNormalNode(RETURN, 0); return RETURN; }
class     { letters += yyleng; yylval.node = createNormalNode(CLASS, 0); return CLASS; }
endclass  { letters += yyleng; yylval.node = createNormalNode(ENDCLASS, 0); return ENDCLASS; }
private   { letters += yyleng; yylval.node = createNormalNode(PRIVATE, 0); return PRIVATE; }
protected { letters += yyleng; yylval.node = createNormalNode(PROTECTED, 0); return PROTECTED; }
public    { letters += yyleng; yylval.node = createNormalNode(PUBLIC, 0); return PUBLIC; }
this      { letters += yyleng; yylval.node = createNormalNode(THIS, 0); return THIS; }
\<\<\<    { letters += yyleng; yylval.node = createNormalNode(INHERIT, 0); return INHERIT; }
-?[0-9]+                 { letters += yyleng; yylval.node = createIntNode(INT_LITERAL, atoi(yytext), 0); return INT_LITERAL; }
-?[0-9]+(\.[0-9]+)?      { letters += yyleng; yylval.node = createDoubleNode(DOUBLE_LITERAL, atof(yytext), 0); return DOUBLE_LITERAL; }
\".*\"                   { letters += yyleng; yylval.node = createStringNode(STRING_LITERAL, strdup(yytext), 0); return STRING_LITERAL; }
[A-Za-z_]([A-Za-z0-9_])* { letters += yyleng; yylval.node = createIDNode(ID, strdup(yytext), 0); return ID; }

%{
#include "utility.h"
#include "AST.h"
#include "y.tab.h"

int lines = 1;
int letters = 0;

extern "C"
{
    int yywrap(void)
    {
        return 1;
    }
    int yylex(void);
}

%}

%%
\n   { lines += 1; letters = 0; }
\(   { letters += yyleng; yylval.node = new ASTNode(LP); return LP; }
\)   { letters += yyleng; yylval.node = new ASTNode(RP); return RP; }
\[   { letters += yyleng; yylval.node = new ASTNode(LB); return LB; }
\]   { letters += yyleng; yylval.node = new ASTNode(RB); return RB; }
\,   { letters += yyleng; yylval.node = new ASTNode(COMMA); return COMMA; }
\;   { letters += yyleng; yylval.node = new ASTNode(SEMI); return SEMI; }
\:\= { letters += yyleng; yylval.node = new ASTNode(INIT); return INIT; }
\=   { letters += yyleng; yylval.node = new ASTNode(ASSIGN); return ASSIGN; }
\+   { letters += yyleng; yylval.node = new ASTNode(ADD); return ADD; }
\-   { letters += yyleng; yylval.node = new ASTNode(SUB); return SUB; }
\*   { letters += yyleng; yylval.node = new ASTNode(MUL); return MUL; }
\/   { letters += yyleng; yylval.node = new ASTNode(DIV); return DIV; }
\%   { letters += yyleng; yylval.node = new ASTNode(MOD); return MOD; }
\=\= { letters += yyleng; yylval.node = new ASTNode(EQUAL); return EQUAL; }
\!\= { letters += yyleng; yylval.node = new ASTNode(NEQ); return NEQ; }
\>   { letters += yyleng; yylval.node = new ASTNode(GREATER); return GREATER; }
\<   { letters += yyleng; yylval.node = new ASTNode(LESS); return LESS; }
\>\= { letters += yyleng; yylval.node = new ASTNode(GEQ); return GEQ; }
\<\= { letters += yyleng; yylval.node = new ASTNode(LEQ); return LEQ; }
\&\& { letters += yyleng; yylval.node = new ASTNode(AND); return AND; }
\|\| { letters += yyleng; yylval.node = new ASTNode(OR); return OR; }
\!   { letters += yyleng; yylval.node = new ASTNode(NOT); return NOT; }
func      { letters += yyleng; yylval.node = new ASTNode(FUNC); return FUNC; }
endfunc   { letters += yyleng; yylval.node = new ASTNode(ENDFUNC); return ENDFUNC; }
if        { letters += yyleng; yylval.node = new ASTNode(IF); return IF; }
endif     { letters += yyleng; yylval.node = new ASTNode(ENDIF); return ENDIF; }
else      { letters += yyleng; yylval.node = new ASTNode(ELSE); return ELSE; }
elsif     { letters += yyleng; yylval.node = new ASTNode(ELSIF); return ELSIF; }
while     { letters += yyleng; yylval.node = new ASTNode(WHILE); return WHILE; }
endwhile  { letters += yyleng; yylval.node = new ASTNode(ENDWHILE); return ENDWHILE; }
int       { letters += yyleng; yylval.node = new ASTNode(INT); return INT; }
double    { letters += yyleng; yylval.node = new ASTNode(DOUBLE); return DOUBLE; }
string    { letters += yyleng; yylval.node = new ASTNode(STRING); return STRING; }
void      { letters += yyleng; yylval.node = new ASTNode(VOID); return VOID; }
return    { letters += yyleng; yylval.node = new ASTNode(RETURN); return RETURN; }
class     { letters += yyleng; yylval.node = new ASTNode(CLASS); return CLASS; }
endclass  { letters += yyleng; yylval.node = new ASTNode(ENDCLASS); return ENDCLASS; }
private   { letters += yyleng; yylval.node = new ASTNode(PRIVATE); return PRIVATE; }
protected { letters += yyleng; yylval.node = new ASTNode(PROTECTED); return PROTECTED; }
public    { letters += yyleng; yylval.node = new ASTNode(PUBLIC); return PUBLIC; }
this      { letters += yyleng; yylval.node = new ASTNode(THIS); return THIS; }
ctor      { letters += yyleng; yylval.node = new ASTNode(CTOR); return CTOR; }
endctor   { letters += yyleng; yylval.node = new ASTNode(ENDCTOR); return ENDCTOR; }
\<\<\<    { letters += yyleng; yylval.node = new ASTNode(INHERIT); return INHERIT; }
-?[0-9]+                 { letters += yyleng; yylval.node = new ASTNode(INT_LITERAL, atoi(yytext), vector<ASTNode*>(0)); return INT_LITERAL; }
-?[0-9]+(\.[0-9]+)?      { letters += yyleng; yylval.node = new ASTNode(DOUBLE_LITERAL, atof(yytext), vector<ASTNode*>(0)); return DOUBLE_LITERAL; }
\".*\"                   { letters += yyleng; yylval.node = new ASTNode(STRING_LITERAL, strdup(yytext), vector<ASTNode*>(0)); return STRING_LITERAL; }
[A-Za-z_]([A-Za-z0-9_])* { letters += yyleng; yylval.node = new ASTNode(ID, strdup(yytext), vector<ASTNode*>(0), true); return ID; }

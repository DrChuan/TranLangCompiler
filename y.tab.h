/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FUNC = 258,
    ENDFUNC = 259,
    ID = 260,
    INT = 261,
    DOUBLE = 262,
    STRING = 263,
    VOID = 264,
    EPSILON = 265,
    SEMI = 266,
    INIT = 267,
    IF = 268,
    ENDIF = 269,
    ELSE = 270,
    ELSIF = 271,
    WHILE = 272,
    ENDWHILE = 273,
    INT_LITERAL = 274,
    DOUBLE_LITERAL = 275,
    STRING_LITERAL = 276,
    RETURN = 277,
    CLASS = 278,
    ENDCLASS = 279,
    PRIVATE = 280,
    PUBLIC = 281,
    PROTECTED = 282,
    INHERIT = 283,
    THIS = 284,
    CTOR = 285,
    ENDCTOR = 286,
    COMMA = 287,
    ASSIGN = 288,
    AND = 289,
    OR = 290,
    EQUAL = 291,
    NEQ = 292,
    GEQ = 293,
    LEQ = 294,
    GREATER = 295,
    LESS = 296,
    ADD = 297,
    SUB = 298,
    MUL = 299,
    DIV = 300,
    MOD = 301,
    NOT = 302,
    LP = 303,
    RP = 304,
    LB = 305,
    RB = 306
  };
#endif
/* Tokens.  */
#define FUNC 258
#define ENDFUNC 259
#define ID 260
#define INT 261
#define DOUBLE 262
#define STRING 263
#define VOID 264
#define EPSILON 265
#define SEMI 266
#define INIT 267
#define IF 268
#define ENDIF 269
#define ELSE 270
#define ELSIF 271
#define WHILE 272
#define ENDWHILE 273
#define INT_LITERAL 274
#define DOUBLE_LITERAL 275
#define STRING_LITERAL 276
#define RETURN 277
#define CLASS 278
#define ENDCLASS 279
#define PRIVATE 280
#define PUBLIC 281
#define PROTECTED 282
#define INHERIT 283
#define THIS 284
#define CTOR 285
#define ENDCTOR 286
#define COMMA 287
#define ASSIGN 288
#define AND 289
#define OR 290
#define EQUAL 291
#define NEQ 292
#define GEQ 293
#define LEQ 294
#define GREATER 295
#define LESS 296
#define ADD 297
#define SUB 298
#define MUL 299
#define DIV 300
#define MOD 301
#define NOT 302
#define LP 303
#define RP 304
#define LB 305
#define RB 306

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

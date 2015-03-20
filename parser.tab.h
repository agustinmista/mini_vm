/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
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
    TOKNUM = 258,
    TOKREG = 259,
    TOKCOMA = 260,
    TOKOPAREN = 261,
    TOKCR = 262,
    TOKCPAREN = 263,
    TOKNUMBER = 264,
    TOKID = 265,
    TOKIMM = 266,
    TOKNOP = 267,
    TOKMOV = 268,
    TOKSW = 269,
    TOKLW = 270,
    TOKPUSH = 271,
    TOKPOP = 272,
    TOKPRINT = 273,
    TOKREAD = 274,
    TOKADD = 275,
    TOKSUB = 276,
    TOKDIV = 277,
    TOKMUL = 278,
    TOKCMP = 279,
    TOKJMP = 280,
    TOKJMPE = 281,
    TOKJMPL = 282,
    TOKHLT = 283,
    TOKLABEL = 284,
    TOKAND = 285,
    TOKOR = 286,
    TOKXOR = 287,
    TOKLSH = 288,
    TOKRSH = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 7 "parser.y" /* yacc.c:1909  */

  int i;
  char *strval;
  struct Operand *oper;
  struct Instruction *inst;

#line 96 "parser.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */

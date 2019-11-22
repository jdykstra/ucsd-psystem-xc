/*
 * UCSD p-System cross compiler
 * Copyright (C) 2010, 2012 Peter Miller
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * you option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>
 */

%{

#include <lib/ac/stdio.h>
#include <lib/ac/stdlib.h>

#include <lib/debug.h>
#include <lib/long_integer.h>

#include <test_long_integer/grammar.h>
#include <test_long_integer/lex.h>

#ifndef YYDEBUG
#define YYDEBUG 1
#endif

#define YYERROR_VERBOSE 1

%}

%token ASSIGN
%token AT
%token EOLN
%token JUNK
%token LP
%token MINUS
%token NOT
%token NUMBER
%token PERCENT
%token PLUS
%token RP
%token SLASH
%token STAR

%union {
    long_integer *lv_number;
}

%type <lv_number> expression
%type <lv_number> NUMBER

%{

void
parse(const char *filename)
{
    lex_open(filename);
    extern int yyparse(void);
    yyparse();
    lex_close();

    extern int yydebug;
    yydebug = !!debug_level;
}

%}

%left EQ NE
%left LT LE GT GE
%left PLUS MINUS
%left STAR SLASH PERCENT
%right unary

%%

file
    : /* empty */
    | file line
    ;

line
    : print_expression EOLN
    | EOLN
        { printf("\n"); }
    | error
    ;

print_expression
    : expression
        {
            rcstring s = $1->to_string();
            delete $1;
            printf("%s\n", s.c_str());
        }
    | AT expression
        {
            rcstring s = $2->representation();
            delete $2;
            printf("%s\n", s.c_str());
        }
    ;

expression
    : NUMBER
        { $$ = $1; }
    | LP expression RP
        { $$ = $2; }
    | PLUS expression
        %prec unary
        { $$ = $2; }
    | MINUS expression
        %prec unary
        {
            long_integer n = -*$2;
            delete $2;
            $$ = new long_integer(n);
        }
    | expression STAR expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            long_integer n = lhs * rhs;
            $$ = new long_integer(n);
        }
    | expression SLASH expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            if (rhs == 0)
            {
                grammar_error("divide by zero");
                $$ = new long_integer();
            }
            else
            {
                long_integer n = lhs / rhs;
                $$ = new long_integer(n);
            }
        }
    | expression PERCENT expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            if (rhs == 0)
            {
                grammar_error("modulo by zero");
                $$ = new long_integer();
            }
            else
            {
                long_integer n = lhs % rhs;
                $$ = new long_integer(n);
            }
        }
    | expression PLUS expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            long_integer n = lhs + rhs;
            $$ = new long_integer(n);
        }
    | expression MINUS expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            long_integer n = lhs - rhs;
            $$ = new long_integer(n);
        }
    | expression LT expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            bool n = lhs < rhs;
            $$ = new long_integer((int)n);
        }
    | expression LE expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            bool n = lhs <= rhs;
            $$ = new long_integer((int)n);
        }
    | expression GT expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            bool n = lhs > rhs;
            $$ = new long_integer((int)n);
        }
    | expression GE expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            bool n = lhs >= rhs;
            $$ = new long_integer((int)n);
        }
    | expression EQ expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            bool n = lhs == rhs;
            $$ = new long_integer((int)n);
        }
    | expression NE expression
        {
            long_integer lhs = *$1;
            delete $1;
            long_integer rhs = *$3;
            delete $3;
            bool n = lhs != rhs;
            $$ = new long_integer((int)n);
        }
    ;


/* vim: set ts=8 sw=4 et : */

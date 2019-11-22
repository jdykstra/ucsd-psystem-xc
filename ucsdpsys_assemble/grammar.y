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

#include <lib/expression/list.h>

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/expression/6502/absolute.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indexed.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indexed_indirect.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indirect.h>
#include <ucsdpsys_assemble/expression/6502/absolute_indirect_indexed.h>
#include <ucsdpsys_assemble/expression/6502/immediate.h>
#include <ucsdpsys_assemble/expression/special/x.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode.h>

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#define YYFPRINTF lex_trace_print

static assembler *context;


opcode::pointer
grammar_lookup_opcode(const rcstring &name)
{
    return context->lookup_opcode(name);
}


expression::pointer
grammar_lookup_special(const rcstring &name)
{
    return context->lookup_special(name);
}


void
assembler::parse(const rcstring &filename)
{
    context = this;
    extern int yyparse(void);
    lex_open(filename);
    yyparse();
    lout.flush();
    lex_close();
}


output::pointer
assembler::get_source_column(void)
{
    assert(context);
    return context->get_source_column_inner();
}


output::pointer
assembler::get_source_column_inner(void)
{
    return lout.get_source_column();
}


static bool
is_x_register(const expression::pointer &ep)
{
    return !!dynamic_cast<const expression_special_x *>(ep.get());
}


%}

%token AND_AND
%token ASSIGN
%token AT
%token BIT_AND
%token BIT_OR
%token CARAT
%token COLON
%token COMMA
%token DIV
%token DOLLAR
%token DOT_ELSE
%token DOT_END
%token DOT_ENDC
%token DOT_EQU
%token DOT_FUNC
%token DOT_IF
%token DOT_MACRO
%token DOT_PROC
%token EOLN
%token EQ
%token GE
%token GT
%token HASH
%token INTEGER_CONSTANT
%token JUNK
%token LB
%token LBRACE
%token LE
%token LGROUP
%token LP
%token LT
%token MINUS
%token MOD
%token NAME
%token NE
%token NOT
%token OPCODE
%token OPCODE_6502
%token OPCODE_PRELUDE
%token OPCODE_NAME_LIST
%token OR_OR
%token PLUS
%token QUESTION_MARK
%token RB
%token RBRACE
%token REAL_CONSTANT
%token RGROUP
%token RP
%token SHIFT_LEFT
%token SHIFT_RIGHT
%token SPECIAL /* usually a CPU register */
%token STAR
%token STRING_CONSTANT
%token TILDE

%union {
    opcode::pointer *lv_opcode;
    int lv_integer;
    expression::pointer *lv_expression;
    expression_list *lv_expression_list;
    rcstring *lv_string;
    double lv_real;
    variable_name *lv_variable_name;
}

/* sort -f -k 3 */
%type <lv_expression> expr6502
%type <lv_expression> expression
%type <lv_expression_list> expression_list
%type <lv_expression_list> expressions
%type <lv_integer> INTEGER_CONSTANT
%type <lv_string> NAME
%type <lv_expression_list> name_list
%type <lv_expression> name_list_item
%type <lv_opcode> OPCODE
%type <lv_opcode> OPCODE_6502
%type <lv_opcode> OPCODE_NAME_LIST
%type <lv_opcode> OPCODE_PRELUDE
%type <lv_real> REAL_CONSTANT
%type <lv_expression> SPECIAL
%type <lv_string> STRING_CONSTANT
%type <lv_variable_name> variable_name

%left COMMA
%left DOT_EQU ASSIGN
%left LT GT LE GE EQ NE
%left PLUS MINUS CARAT BIT_OR OR_OR
%left STAR DIV MOD BIT_AND AND_AND
%right unary PLUS_PLUS MINUS_MINUS TILDE
%left NAME SPECIAL INTEGER_CONSTANT STRING_CONSTANT
%left highest_precedence

%%

assemble
    : prelude proc_list end
        { context->dot_end(); }
    ;

end
    : /* empty */
        {
            yyerror("missing .end directive");
        }
    | DOT_END EOLN
        {
            lex_discard_input();
            context->listing_eoln();
        }
    ;

prelude
    : /* empty */
    | prelude prelude_opcode
    ;

prelude_opcode
    : empty_line
    | prelop
    | error
    ;

empty_line
    : EOLN
        {
            context->listing_eoln();
        }
    ;

proc_list
    : /* empty */
    | proc_list proc
    ;

proc
    : proc_begin opcodes
        {
            context->proc_end();
        }
    ;

proc_begin
    : DOT_PROC variable_name COMMA expression EOLN
        {
            context->proc_begin(*$2, *$4, false);
            delete $2;
            delete $4;
            context->listing_eoln();
        }
    | DOT_PROC variable_name EOLN
        {
            context->proc_begin(*$2, expression::pointer(), false);
            delete $2;
            context->listing_eoln();
        }
    | DOT_FUNC variable_name COMMA expression EOLN
        {
            context->proc_begin(*$2, *$4, true);
            delete $2;
            delete $4;
            context->listing_eoln();
        }
    | DOT_FUNC variable_name EOLN
        {
            context->proc_begin(*$2, expression::pointer(), true);
            delete $2;
            context->listing_eoln();
        }
    ;

opcodes
    : /* empty */
    | opcodes opcode
    ;

opcode
    : label opcode
    | OPCODE expression_list EOLN
        {
            /*
             * This is the generic form, and we try to get as many
             * opcodes to use it as pissible.
             *
             * But some vendors came up with some really funky
             * annotation, and this simple consistent form can sometimes
             * be insufficient.
             */
            opcode::pointer op = *$1;
            op->interpret(*$2);
            delete $1;
            delete $2;
            context->listing_eoln();
        }
    | prelop
    | OPCODE_6502 expr6502 EOLN
        {
            opcode::pointer op = *$1;
            expression_list args;
            args.push_back(*$2);
            op->interpret(args);
            delete $1;
            delete $2;
            context->listing_eoln();
        }
    | empty_line
    | error
    ;

prelop
    : OPCODE_PRELUDE expression_list EOLN
        {
            opcode::pointer op = *$1;
            op->interpret(*$2);
            delete $1;
            delete $2;
            context->listing_eoln();
        }
    | variable_name DOT_EQU expression EOLN
        {
            context->define_symbol(*$1, *$3);
            delete $1;
            delete $3;
            context->listing_eoln();
        }
    | variable_name ASSIGN expression EOLN
        {
            context->define_symbol(*$1, *$3);
            delete $1;
            delete $3;
            context->listing_eoln();
        }
    | DOT_MACRO variable_name EOLN
        {
            rcstring_list contents;
            lex_macro_slurp(contents);
            context->define_macro(*$2, contents);
            delete $2;
            context->listing_eoln();
        }
    | DOT_IF expression EOLN
        {
            context->dot_if(*$2);
            delete $2;
            context->listing_eoln();
        }
    | OPCODE_NAME_LIST name_list EOLN
        {
            /*
             * This form is used for opcodes that take list of names.
             * For example: .ref, .public
             */
            opcode::pointer op = *$1;
            op->interpret(*$2);
            delete $1;
            delete $2;
            context->listing_eoln();
        }
    ;

name_list
    : name_list_item
        {
            $$ = new expression_list();
            $$->push_back(*$1);
            delete $1;
        }
    | name_list COMMA name_list_item
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    ;

name_list_item
    : NAME
        {
            variable_name vn(*$1, lex_location());
            expression::pointer ep =
                context->name_expression_factory(vn);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    ;

/*
 * Note: we put the specialized forms before the generic forms, so that
 * in the event of a reduce/reduce conflict, the specialized form takes
 * precedence.
 */
expr6502
    : SPECIAL
        { $$ = $1; }
    | expression
        {
            // Absolute
            // Zero Page
            expression::pointer ep = expression_6502_absolute::create(*$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | expression COMMA SPECIAL
        {
            // Absolute Indexed
            // Zero Page Indexed
            expression::pointer ep =
                expression_6502_absolute_indexed::create(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | LP expression COMMA SPECIAL RP
        %prec highest_precedence
        {
            // Abolute Indexed Indirect
            // Zero Page Indexed Indirect
            expression::pointer ep =
                expression_6502_absolute_indexed_indirect::create(*$2, *$4);
            $$ = new expression::pointer(ep);
            delete $2;
            delete $4;
        }
    | LP expression RP
        %prec highest_precedence
        {
            // Absolute Indirect
            expression::pointer ep =
                expression_6502_absolute_indirect::create(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | AT expression
        %prec highest_precedence
        {
            // Absolute Indirect
            //
            // Apple Pascal Operating System Reference Manual, p. 149
            expression::pointer ep =
                expression_6502_absolute_indirect::create(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | LP expression RP COMMA SPECIAL
        %prec highest_precedence
        {
            // Absolute Indirect Indexed
            // Zero Page Indirect Indexed
            expression::pointer ep =
                expression_6502_absolute_indirect_indexed::create(*$2, *$5);
            $$ = new expression::pointer(ep);
            delete $2;
            delete $5;
        }
    | AT expression COMMA SPECIAL
        %prec highest_precedence
        {
            // Apple Pascal Operating System Reference Manual, p. 149
            if (is_x_register(*$4))
            {
                // @e,X  ==>  (e,X)
                expression::pointer ep =
                    expression_6502_absolute_indexed_indirect::create(*$2, *$4);
                $$ = new expression::pointer(ep);
            }
            else
            {
                // @e,Y  ==>  (e),Y
                expression::pointer ep =
                    expression_6502_absolute_indirect_indexed::create(*$2, *$4);
                $$ = new expression::pointer(ep);
            }
            delete $2;
            delete $4;
        }
    | HASH expression
        {
            // Immediate
            expression::pointer ep = expression_6502_immediate::create(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    ;

expression_list
    : /* empty */
        { $$ = new expression_list(); }
    | expressions
        { $$ = $1; }
    ;

expressions
    : expression
        {
            $$ = new expression_list();
            $$->push_back(*$1);
            delete $1;
        }
    | expressions COMMA expression
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    ;

expression
    : NAME
        {
            variable_name vn(*$1, lex_location());
            expression::pointer ep =
                context->name_expression_factory(vn);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | INTEGER_CONSTANT
        {
            expression::pointer ep =
                context->integer_expression_factory(lex_location(), $1);
            $$ = new expression::pointer(ep);
        }
    | STRING_CONSTANT
        {
            expression::pointer ep =
                context->string_expression_factory(lex_location(), *$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | LGROUP expression RGROUP
        { $$ = $2; }
    | MINUS expression
        %prec unary
        {
            expression::pointer ep = context->negate_expression_factory(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | PLUS expression
        %prec unary
        {
            expression::pointer ep =
                context->unary_plus_expression_factory(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | TILDE expression
        {
            expression::pointer ep =
                context->bitwise_not_expression_factory(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | DOLLAR
        {
            expression::pointer ep =
                context->dollar_expression_factory(lex_location());
            $$ = new expression::pointer(ep);
        }
    | expression PLUS expression
        {
            expression::pointer ep =
                context->addition_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression MINUS expression
        {
            expression::pointer ep =
                context->subtraction_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression STAR expression
        {
            expression::pointer ep =
                context->multiplication_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression DIV expression
        {
            expression::pointer ep =
                context->division_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression MOD expression
        {
            expression::pointer ep =
                context->modulo_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression CARAT expression
        {
            expression::pointer ep =
                context->xor_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression BIT_OR expression
        {
            expression::pointer ep =
                context->bitwise_or_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression BIT_AND expression
        {
            expression::pointer ep =
                context->bitwise_and_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression EQ expression
        {
            expression::pointer ep =
                context->eq_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression NE expression
        {
            expression::pointer ep =
                context->ne_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression LE expression
        {
            expression::pointer ep =
                context->le_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression LT expression
        {
            expression::pointer ep =
                context->lt_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression GE expression
        {
            expression::pointer ep =
                context->ge_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression GT expression
        {
            expression::pointer ep =
                context->gt_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    ;

variable_name
    : NAME
        {
            $$ = new variable_name(*$1, lex_location());
            delete $1;
        }
    ;

label
    : variable_name
        {
            context->define_label(*$1);
            delete $1;
        }
    | variable_name COLON
        {
            context->define_label(*$1);
            delete $1;
        }
    ;


// vim: set ts=8 sw=4 et :

/*
 * UCSD p-System cross compiler
 * Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#include <lib/bit_address.h>
#include <lib/case/list.h>
#include <lib/case/range.h>
#include <lib/debug.h>
#include <lib/expression/list.h>
#include <lib/name_type.h>
#include <lib/name_type/list.h>
#include <lib/pascal/grammar.h>
#include <lib/pascal/lex.h>
#include <lib/rcstring/list.h>
#include <lib/statement.h>
#include <lib/statement/list.h>
#include <lib/translator.h>
#include <lib/type/anything.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/error.h>
#include <lib/type/file.h>
#include <lib/type/integer.h>
#include <lib/type/list.h>
#include <lib/type/nothing.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/real.h>
#include <lib/type/record.h>
#include <lib/type/reference/implicit.h>
#include <lib/type/string.h>
#include <lib/variable/name.h>
#include <lib/variable/name/list.h>
#include <lib/variant.h>

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#define YYFPRINTF pascal_lex_trace_print

static translator *context;

%}

%token END_OF_FILE 0 "end of file"
%token AND
%token ARRAY
%token ARROW         "->"
%token ASSIGN        ":="
%token AT
%token BEGIN
%token BITWISE_AND
%token BITWISE_NOT
%token BITWISE_OR
%token CARAT         "^"
%token CASE
%token CODE_tok      "code"
%token COLON         ":"
%token COMMA         ","
%token COMMENT
%token CONST
%token DATA_tok      "data"
%token DIV
%token DO
%token DOT           "."
%token DOWNTO
%token ELIPSIS       ".."
%token ELSE
%token END
%token EQ            "="
%token EXTERNAL
%token FILE_tok      "file"
%token FOR
%token FORWARD
%token FUNCTION
%token GE            ">="
%token GOTO
%token GT            ">"
%token IF
%token IMPLEMENTATION
%token IN
%token INTEGER_CONSTANT
%token INTERFACE
%token INTRINSIC
%token JUNK
%token LABEL
%token LB            "["
%token LE            "<="
%token LONG_INTEGER_CONSTANT
%token LP            "("
%token LT            "<"
%token MINUS         "-"
%token MOD
%token MUL           "*"
%token NAME
%token NE            "<>"
%token NOT
%token OF
%token OR
%token OTHERWISE
%token PACKED
%token PLUS          "+"
%token PROCEDURE
%token PROGRAM
%token QUESTION_MARK "?"
%token RB            "]"
%token REAL_CONSTANT
%token RECORD
%token REPEAT
%token RP            ")"
%token SCAN
%token SEGMENT
%token SEMICOLON     ";"
%token SEPARATE
%token SET
%token SIZEOF_tok    "sizeof"
%token SLASH         "/"
%token STRING_CONSTANT
%token THEN
%token TO
%token TYPE
%token TYPE_NAME
%token UNIT
%token UNTIL
%token USES
%token VAR
%token WHILE
%token WITH
%token WRITE
%token WRITELN

/*
 * Note: you can't have anything with a constructor or destructor in the
 * union, because the compiler has no idea when to run them.  The net
 * result is that we must have pointers to things like rcstring and the
 * smart_pointers, even though they already *are* a pointer (but that's
 * internal to the implementation, and we can't "see" it here).
 */
%union {
    bit_address *lv_bit_address;
    case_list *lv_case_list;
    case_range *lv_case_range;
    expression::pointer *lv_expression;
    expression_list *lv_expression_list;
    statement::pointer *lv_statement; // MUST be a pointer
    statement_list *lv_statement_list;
    rcstring *lv_string; // MUST be a pointer
    variable_name *lv_variable_name;
    variable_name_list *lv_variable_name_list;
    type::pointer *lv_type;
    type_record::pointer *lv_type_record;
    name_type *lv_name_type;
    name_type_list *lv_name_type_list;
    int lv_int;
    variant::pointer *lv_variant;
    type_list *lv_type_list;
}

/* !sort -k 3 */
%type <lv_int>                array_begin
%type <lv_type_list>          array_dims
%type <lv_expression>         case_begin
%type <lv_expression>         case_expression
%type <lv_case_list>          case_item
%type <lv_case_list>          case_item_list
%type <lv_case_range>         case_range
%type <lv_statement>          case_statement
%type <lv_string>             COMMENT
%type <lv_type>               complex_type
%type <lv_type>               complex_type_unbound
%type <lv_statement>          compound_statement
%type <lv_statement>          empty_statement
%type <lv_type>               enumerated_begin
%type <lv_type>               enumerated_type
%type <lv_expression>         expression
%type <lv_expression_list>    expression_list
%type <lv_statement>          expression_statement
%type <lv_int>                for_dirn
%type <lv_statement>          for_statement
%type <lv_statement>          goto_statement
%type <lv_statement>          if_statement
%type <lv_string>             INTEGER_CONSTANT
%type <lv_string>             LONG_INTEGER_CONSTANT
%type <lv_variable_name>      label_name
%type <lv_variable_name>      label_name_colon
%type <lv_variable_name_list> label_names
%type <lv_statement>          label_statement
%type <lv_string>             NAME
%type <lv_int>                num
%type <lv_name_type>          parameter
%type <lv_name_type_list>     parameters
%type <lv_string>             proc_proto_name
%type <lv_string>             REAL_CONSTANT
%type <lv_type_record>        record_begin
%type <lv_variant>            record_body
%type <lv_bit_address>        record_mark
%type <lv_type>               record_type
%type <lv_variant>            record_variant_part
%type <lv_int>                scan_op
%type <lv_int>                segment
%type <lv_expression>         set_expression_part
%type <lv_expression>         set_expression_parts
%type <lv_type>               simple_type
%type <lv_type>               simple_type_unbound
%type <lv_statement>          sl_statement
%type <lv_statement>          statement
%type <lv_statement_list>     statement_list
%type <lv_string>             STRING_CONSTANT
%type <lv_type>               subrange_type
%type <lv_string>             TYPE_NAME
%type <lv_string>             unit_name
%type <lv_string>             unit_name_and_segments
%type <lv_statement>          until_statement
%type <lv_variable_name>      variable_name
%type <lv_variable_name_list> variable_names
%type <lv_statement>          var_statement
%type <lv_statement>          while_statement
%type <lv_expression>         with_expression
%type <lv_statement>          with_part
%type <lv_statement>          with_statement
%type <lv_expression>         writeln_expression
%type <lv_expression_list>    writeln_expression_list
%type <lv_int>                writeln_selector
%type <lv_statement>          writeln_statement

/* lowest to highest */
%right ELSE
%left SEMICOLON
%left COMMA
%right ASSIGN
%right ternary QUESTION_MARK
%left LT GT LE GE EQ NE IN
%left PLUS MINUS OR BITWISE_OR
%left MUL SLASH DIV MOD AND BITWISE_AND
%right unary CARAT NOT BITWISE_NOT SIZEOF_tok AT
%left DOT
%left LP RP LB RB
%left NAME LONG_INTEGER_CONSTANT INTEGER_CONSTANT REAL_CONSTANT STRING_CONSTANT
%right highest_prec

%{

static void
print_token_int(FILE *fp, const YYSTYPE *yylval)
{
    YYFPRINTF(fp, "%d", yylval->lv_int);
}


static void
print_token_string(FILE *fp, const YYSTYPE *yylval)
{
    YYFPRINTF(fp, "%s", yylval->lv_string->quote_c().c_str());
}

%}

%printer {
    print_token_int(pascal_grammar_output, pascal_grammar_valuep);
} array_begin for_dirn num scan_op segment writeln_selector

%printer {
    print_token_string(pascal_grammar_output, pascal_grammar_valuep);
} COMMENT INTEGER_CONSTANT LONG_INTEGER_CONSTANT NAME proc_proto_name

%printer {
    print_token_string(pascal_grammar_output, pascal_grammar_valuep);
} REAL_CONSTANT STRING_CONSTANT TYPE_NAME unit_name unit_name_and_segments

%%

pascal
    : program program_scope_part DOT
        {
            // do NOT call context->scope_end here, so there is
            // something left for translator::traverse to work on.
            context->program_end();
        }
    | unit
        {
            // do NOT call context->scope_end here, so there is
            // something left for translator::traverse to work on.
            context->program_end();
        }
    ;

define_builtin_symbols
    : /* empty */
        {
            //
            // We define the builtin symbols after the first token has
            // been fetched, so that any (*$XXX*) control comments
            // have had a chance to operate first.  In this way, for
            // example, system compiles (*$U-*) can influence which
            // builtin symbols are created, and (*$B*) will change the
            // codefile's byte sex.
            //
            context->define_builtin_symbols();
        }
    ;

program
    : PROGRAM define_builtin_symbols NAME SEMICOLON
        {
            context->program_scope(*$3);
            delete $3;
        }
    ;

scope_part
    : scope_part_clauses compound_statement
        {
            context->scope_attach_body(*$2);
            delete $2;
        }
    ;

program_scope_part
    : program_scope_part_clauses compound_statement
        {
            context->scope_attach_body(*$2);
            delete $2;
        }
    ;

/*
 * Pascal usually has a very strict ordering of these clauses.  But
 * UCSD Pascal allows interesting things to happen in and around include
 * files.  The easiest way to get the same behaviour is to allow any of
 * these clauses (and even functions) in any order.
 */
scope_part_clauses
    : /* empty */
        { context->scope_part_begin(); }
    | scope_part_clauses scope_part_clause
    | scope_part_clauses error
    ;

program_scope_part_clauses
    : /* empty */
        { context->scope_part_begin(); }
    | program_scope_part_clauses program_scope_part_clause
    | program_scope_part_clauses error
    ;

scope_part_clause
    : const_clause
    | function
    | label_clause
    | type_clause
    | var_clause
    ;

program_scope_part_clause
    : const_clause
    | function
    | label_clause
    | subunit_clause
    | type_clause
    | uses_clause
    | var_clause
    ;

const_clause
    : CONST constant_declaration
    | const_clause constant_declaration
    ;

constant_declaration
    : variable_name type_eq expression SEMICOLON
        {
            context->constant_declaration(*$1, *$3);
            delete $1;
            delete $3;
        }
    ;

label_clause
    : LABEL label_names SEMICOLON
        {
            context->label_declaration(*$2);
            delete $2;
        }
    ;

label_names
    : label_name
        {
            $$ = new variable_name_list();
            $$->push_back(*$1);
            delete $1;
        }
    | label_names comma label_name
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    ;

label_name
    : NAME
        {
            // Extension, not in UCSD pascal.
            $$ = new variable_name(*$1);
            delete $1;
            context->complain_about_named_label(*$$);
        }
    | TYPE_NAME
        {
            // Extension, not in UCSD pascal.  This can happen when a
            // label shadows a type in an outer scope.
            $$ = new variable_name(*$1);
            delete $1;
            context->complain_about_named_label(*$$);
        }
    | INTEGER_CONSTANT
        {
            int n = pascal_lex_integer_constant_to_int(*$1);
            rcstring ns = rcstring::printf("%d", n);
            $$ = new variable_name(ns);
            delete $1;
        }
    ;

type_clause
    : TYPE type_declaration
    | type_clause type_declaration
    ;

type_eq
    : EQ
    | ASSIGN
        { yyerror("that \":=\" should be \"=\""); }
    | COLON
        { yyerror("that \":\" should be \"=\""); }
    ;

type_declaration
    : variable_name type_eq complex_type_unbound SEMICOLON
        {
            // Hint a name to the type.  It is possible the type is an
            // anonymous record or an anonymous enumeration.
            type::pointer tp = *$3;
            tp->name_hint($1->get_name());

            context->type_declaration(*$1, tp);
            delete $1;
            delete $3;

            //
            // This type could resolve an outstanding forward type,
            // go looking to see if it has.
            //
            context->late_binding_opportunity();
        }
    ;

var_clause
    : VAR variable_declaration
    | var_clause variable_declaration
    ;

var_colon
    : COLON
    | EQ
        { yyerror("that \"=\" should be \":\""); }
    | ASSIGN
        { yyerror("that \":=\" should be \":\""); }
    ;

variable_declaration
    : variable_names var_colon complex_type SEMICOLON
        {
            // Hint a name to the type.  It is possible the type is an
            // anonymous record or an anonymous enumeration.
            type::pointer tp = *$3;
            tp->name_hint($1->get(0).get_name());

            //
            // All relevant types should be resolved by now.  There
            // should be no forward declared types in the type of this
            // variable.  Barf if there is.
            //
            if (tp->need_late_binding())
            {
                pascal_lex_error
                (
                    $1->get(0).get_location(),
                    "the type is not completely defined"
                );
                tp = type_error::create();
            }

            context->variable_declaration(*$1, tp);
            delete $1;
            delete $3;
        }
    ;

variable_names
    : variable_name
        {
            $$ = new variable_name_list();
            $$->push_back(*$1);
            delete $1;
        }
    | variable_names comma variable_name
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    ;

comma
    : COMMA
    | /* empty */
        { yyerror("missing comma"); }
    ;

variable_name
    : NAME
        {
            // We do this so the variable name has an accurate location
            // attached to it.  This makes for better error messages.
            $$ = new variable_name(*$1);
            delete $1;
        }
    | TYPE_NAME
        {
            // This can happen when a variable shadows a type in an
            // outer scope.
            $$ = new variable_name(*$1);
            delete $1;
        }
    ;

simple_type
    : simple_type_unbound
        {
            $$ = new type::pointer(type::late_binding(*$1));
            delete $1;
        }
    ;

simple_type_unbound
    : TYPE_NAME
        {
            // There is no guarantee this has been bound.
            type::pointer tsp = context->type_lookup(*$1, false);
            $$ = new type::pointer(tsp);
            delete $1;
        }
    | TYPE_NAME LB expression RB
        {
            type::pointer tp = context->type_lookup(*$1, *$3);
            $$ = new type::pointer(tp);
            delete $1;
            delete $3;
        }
    | subrange_type
        { $$ = $1; }
    | CARAT complex_type_unbound
        {
            type::pointer tp = type_pointer_unpacked::create(*$2);
            $$ = new type::pointer(tp);
            delete $2;
        }
    | PACKED FILE_tok OF complex_type_unbound
        {
            yyerror("packed files are not supported");
            type::pointer tp = type_file::create(*$4);
            $$ = new type::pointer(tp);
            delete $4;
        }
    | FILE_tok OF complex_type_unbound
        {
            type::pointer tp = type_file::create(*$3);
            $$ = new type::pointer(tp);
            delete $3;
        }
    | FILE_tok
        {
            type::pointer vp = type_nothing::create();
            type::pointer tp = type_file::create(vp);
            $$ = new type::pointer(tp);
        }
    | SET OF complex_type_unbound
        {
            type::pointer tp = context->set_type(*$3);
            $$ = new type::pointer(tp);
            delete $3;
        }
    | array_begin LB array_dims RB OF complex_type_unbound
        {
            type::pointer tp = context->new_array_type($1, *$3, *$6);
            $$ = new type::pointer(tp);
            delete $3;
            delete $6;
        }
    ;

complex_type
    : complex_type_unbound
        {
            type::pointer tp = type::late_binding(*$1);
            $$ = new type::pointer(tp);
            delete $1;
        }
    ;

complex_type_unbound
    : simple_type_unbound
        { $$ = $1; }
    | record_type
        {
            type::pointer tp = *$1;
            $$ = new type::pointer(tp);
            delete $1;
        }
    | NAME
        {
            //
            // This almost always means that it is not a defined type
            // name, even though the user has used it as one.  However,
            // it is also possible to forward reference a type name in a
            // type declaration.
            //
            type::pointer tsp = context->type_lookup(*$1, true);
            $$ = new type::pointer(tsp);
            delete $1;
        }
    | enumerated_type
        %prec highest_prec
        { $$ = $1; }
    ;

array_begin
    : PACKED ARRAY
        { $$ = 1; }
    | ARRAY
        { $$ = 0; }
    ;

/*
 * The index type(s) must be bound at this point, otherwise
 * error checking is very difficult.  If there is a real
 * example of using an unbound type, this can be revisited.
 */
array_dims
    : complex_type
        {
            $$ = new type_list();
            $$->push_back(*$1);
            delete $1;
        }
    | array_dims COMMA complex_type
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    ;

enumerated_type
    : enumerated_begin enumerated_tags enumerated_end
        %prec highest_prec
        { $$ = $1; }
    ;

enumerated_begin
    : LP
        %prec highest_prec
        {
            type::pointer tp = context->enumerated_begin();
            $$ = new type::pointer(tp);
        }
    ;

enumerated_end
    : RP
        %prec ASSIGN
        { context->enumerated_end(); }
    ;

enumerated_tags
    : enumerated_tag
    | enumerated_tags comma enumerated_tag
    ;

enumerated_tag
    : variable_name
        %prec highest_prec
        {
            context->enumerated_tag(*$1);
            delete $1;
        }
    ;

elipsis
    : ELIPSIS
    | COLON
        {
            pascal_lex_warning
            (
                "Oh, barf!  I know that using a colon here instead of "
                "\"..\" is allegedly valid Pascal, but do we really "
                "have to be this ugly?"
            );
        }
    ;

subrange_type
    : expression elipsis expression
        {
            type::pointer tp = context->subrange_type(*$1, *$3);
            $$ = new type::pointer(tp);
            delete $1;
            delete $3;
        }
    | REAL_CONSTANT
        {
            rcstring_list a;
            a.split(*$1, ".");
            while (a.size() < 2)
                a.push_back("13");
            pascal_lex_error
            (
                pascal_lex_location(),
                "the subrange appears to be missing a dot, "
                    "did you mean %s..%s instead?",
                a[0].c_str(),
                a[1].c_str()
            );
            location locn = pascal_lex_location();
            expression::pointer e1 =
                context->integer_expression_factory(locn, a[0]);
            expression::pointer e2 =
                context->integer_expression_factory(locn, a[1]);
            type::pointer tp = context->subrange_type(e1, e2);
            $$ = new type::pointer(tp);
            delete $1;
        }
    ;

record_type
    : record_begin record_body END
        {
            type_record::pointer trp = *$1;
            trp->attach_variant(*$2);
            context->record_end();
            $$ = new type::pointer(trp);
            delete $2;
        }
    ;

record_body
    : record_fields optional_semicolon
        {
            unsigned nwords = context->get_data_mark().get_size_in_words();
            variant::pointer vp = variant::create(nwords);
            $$ = new variant::pointer(vp);
        }
    | record_fields SEMICOLON record_variant_part optional_semicolon
        { $$ = $3; }
    | record_variant_part optional_semicolon
        { $$ = $1; }
    | error
        {
            unsigned nwords = context->get_data_mark().get_size_in_words();
            variant::pointer vp = variant::create(nwords);
            $$ = new variant::pointer(vp);
        }
    ;

record_begin
    : RECORD
        {
            type_record::pointer tp = context->record_begin(false);
            $$ = new type_record::pointer(tp);
        }
    | PACKED RECORD
        {
            type_record::pointer tp = context->record_begin(true);
            $$ = new type_record::pointer(tp);
        }
    ;

record_fields
    : record_field
    | record_fields SEMICOLON record_field
    ;

record_field
    : variable_names COLON complex_type_unbound
        {
            context->record_field(*$1, *$3);
            delete $1;
            delete $3;
        }
    ;

record_variant_part
    : CASE record_variant_cond OF record_variants
        {
            variant::pointer vp = context->variant_record_end();
            $$ = new variant::pointer(vp);
        }
    ;

record_variant_cond
    : simple_type
        {
            context->variant_record_begin(*$1);
        }
    | variable_name COLON simple_type
        {
            // The variable is NOT part of the variant section.
            variable_name_list vnl;
            vnl.push_back(*$1);
            context->record_field(vnl, *$3);
            delete $1;

            context->variant_record_begin(*$3);
            delete $3;
        }
    ;

record_variants
    : record_variant
    | record_variants SEMICOLON
    | record_variants SEMICOLON record_variant
    ;

record_variant
    : expression_list COLON LP record_mark record_body RP
        {
            context->variant_record_field(*$1, *$5);
            context->set_data_mark(*$4);
            delete $1;
            delete $4;
            delete $5;
        }
    ;

record_mark
    : /* empty */
        { $$ = new bit_address(context->get_data_mark()); }
    ;


function
    : function_prototype scope_part SEMICOLON
        {
            context->scope_end();
        }
    | function_prototype FORWARD SEMICOLON
        {
            context->scope_to_be_continued();
        }
    | function_prototype EXTERNAL SEMICOLON
        {
            context->scope_end_external();
        }
    ;

function_prototype
    : function_introducer function_arguments COLON simple_type SEMICOLON
        {
            if (context->current_function_definition_was_forward())
            {
                yyerror
                (
                    "must not specify a return type for functions "
                    "previously declared as forward"
                );
            }
            else
                context->scope_attach_return_type(*$4);
            delete $4;
        }
    | function_introducer function_arguments SEMICOLON
        {
            // Sigh.  Yet another fugly Pascal-ism.  This isn't an error
            // if they already forward declared this function.  What if
            // we made FUNCTION and PROCEDURE the same keyword?  Correct
            // programs would compile without errors, but so would some
            // incorrect programs.
            if (!context->current_function_definition_was_forward())
                yyerror("must specify a return type for functions");
        }
    | procedure_introducer function_arguments COLON simple_type SEMICOLON
        {
            yyerror("must not specify a return type for procedures");
            context->scope_attach_return_type(*$4);
            delete $4;
        }
    | procedure_introducer function_arguments SEMICOLON
    ;

function_introducer
    : segment FUNCTION proc_proto_name
        {
            context->function_scope(*$3, true, $1);
            delete $3;

            // Attach a bogus type to start with, to give the pretty
            // printer a clue that it's a function and not a procedure.
            if (!context->current_function_definition_was_forward())
                context->scope_attach_return_type(type_integer::create());
        }
    ;

segment
    : /* empty */
        { $$ = 0; }
    | SEGMENT
        { $$ = 1; }
    ;

proc_proto_name
    : NAME
        { $$ = $1; }
    | TYPE_NAME
        {
            // This can happen when the new procedure name or function
            // name shadows a named type.
            $$ = $1;
        }
    ;

procedure_introducer
    : segment PROCEDURE proc_proto_name
        {
            context->function_scope(*$3, false, $1);
            delete $3;
        }
    ;

function_arguments
    : /* empty */
        { context->scope_attach_function_parameters(new name_type_list); }
    | LP RP
        {
            context->complain_about_empty_parens();
            context->scope_attach_function_parameters(new name_type_list);
        }
    | LP parameters RP
        { context->scope_attach_function_parameters($2); }
    ;

parameters
    : parameter
        {
            $$ = new name_type_list();
            $$->push_back(*$1);
            delete $1;
        }
    | parameters SEMICOLON parameter
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    ;

parameter
    : variable_names COLON simple_type
        {
            type::pointer tp = *$3;
            if (false)
                tp = type_reference::create(tp);

            //
            // Strings and arrays and records are always passed as references,
            // even when the user asked for call by value.  They get copied at
            // the beginning of the function, instead.
            //
            // But not long ints or sets.
            //
            if
            (
                type_string::is_a(tp)
            ||
                type_array::is_a(tp)
            ||
                type_record::is_a(tp)
            )
                tp = type_reference_implicit::create(tp);

            $$ = new name_type(*$1, tp);
            delete $1;
            delete $3;
        }
    | VAR variable_names COLON simple_type
        {
            type::pointer tp = *$4;
            tp = type_reference::create(tp);

            $$ = new name_type(*$2, tp);
            delete $2;
            delete $4;
        }
    | VAR variable_names
        {
            // This allows you to pass "anything" to native machine code.
            type::pointer tp = type_reference::create(type_anything::create());
            $$ = new name_type(*$2, tp);
            delete $2;
        }
    ;

compound_statement
    : compound_begin statement_list compound_end
        {
            statement::pointer sp = context->compound_statement_factory(*$2);
            $$ = new statement::pointer(sp);
            delete $2;
        }
    | compound_begin compound_end
        {
            statement_list sl;
            statement::pointer sp = context->compound_statement_factory(sl);
            $$ = new statement::pointer(sp);
        }
    ;

compound_begin
    : BEGIN
        { context->compound_scope_begin(); }
    ;

compound_end
    : END
        { context->compound_scope_end(); }
    ;

statement_list
    : sl_statement
        {
            $$ = new statement_list();
            $$->push_back(*$1);
            delete $1;
        }
    | statement_list SEMICOLON sl_statement
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    | statement_list error
        { $$ = $1; }
    ;

/*
 * Each statement in a statement list is wrapped with a tranmsparent
 * scope bein and end, so that expressions are free to use as much
 * temprary variable space as they need.
 *
 * Except the VAR statement, that declares variables that last until the
 * statement list ends.
 */
sl_statement
    : var_statement
        { $$ = $1; }
    | twrap_begin statement twrap_end
        { $$ = $2; }
    ;

twrap_begin
    : /* empty */
        { context->compound_scope_begin(); }
    ;

twrap_end
    : /* empty */
        { context->compound_scope_end(); }
    ;


statement
    : case_statement
        { $$ = $1; }
    | compound_statement
        { $$ = $1; }
    | empty_statement
        { $$ = $1; }
    | expression_statement
        { $$ = $1; }
    | for_statement
        { $$ = $1; }
    | goto_statement
        { $$ = $1; }
    | if_statement
        { $$ = $1; }
    | label_statement
        { $$ = $1; }
    | until_statement
        { $$ = $1; }
    | while_statement
        { $$ = $1; }
    | with_statement
        { $$ = $1; }
    | writeln_statement
        { $$ = $1; }
    ;

empty_statement
    : /* empty */
        {
            statement::pointer sp = context->empty_statement_factory();
            $$ = new statement::pointer(sp);
        }
    ;

until_statement
    : repeat_begin statement_list UNTIL expression
        {
            statement::pointer sp = context->until_statement(*$2, *$4);
            $$ = new statement::pointer(sp);
            delete $2;
            delete $4;

            /*
             * We end the scope here, rather than at UNTIL, so that a
             * variable allocated and calculated within the loop can be
             * used to control the loop exit.
             */
            context->compound_scope_end();
        }
    ;

repeat_begin
    : REPEAT
        { context->compound_scope_begin(); }
    ;

with_statement
    : WITH with_part
        { $$ = $2; }
    ;

with_expression
    : expression
        {
            context->with_scope_begin(*$1);
            $$ = $1;
        }
    ;

with_part
    : with_expression DO statement
        {
            statement::pointer sp = context->with_scope_end(*$1, *$3);
            $$ = new statement::pointer(sp);
            delete $1;
            delete $3;
        }
    | with_expression COMMA with_part
        {
            statement::pointer sp = context->with_scope_end(*$1, *$3);
            $$ = new statement::pointer(sp);
            delete $1;
            delete $3;
        }
    ;

while_statement
    : WHILE expression DO statement
        {
            statement::pointer sp = context->while_statement(*$2, *$4);
            $$ = new statement::pointer(sp);
            delete $2;
            delete $4;
        }
    ;

writeln_statement
    : writeln_selector LP writeln_expression_list RP
        {
            statement::pointer sp =
                context->writeln_statement_factory(*$3, $1);
            $$ = new statement::pointer(sp);
            delete $3;
        }
    | writeln_selector LP RP
        {
            context->complain_about_empty_parens();
            statement::pointer sp =
                context->writeln_statement_factory(expression_list(), $1);
            $$ = new statement::pointer(sp);
        }
    | writeln_selector
        {
            statement::pointer sp =
                context->writeln_statement_factory(expression_list(), $1);
            $$ = new statement::pointer(sp);
        }
    ;

writeln_selector
    : WRITELN
        { $$ = 1; }
    | WRITE
        { $$ = 0; }
    ;

writeln_expression_list
    : writeln_expression
        {
            $$ = new expression_list();
            $$->push_back(*$1);
            delete $1;
        }
    | writeln_expression_list COMMA writeln_expression
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    | writeln_expression_list error
        {
            $$ = $1;
        }
    ;

writeln_expression
    : expression
        {
            expression::pointer ep = context->writeln1_expression_factory(*$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | expression COLON expression
        {
            expression::pointer ep =
                context->writeln2_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression COLON expression COLON expression
        {
            expression::pointer ep =
                context->writeln3_expression_factory(*$1, *$3, *$5);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
            delete $5;
        }
    ;

expression_statement
    : expression
        {
            statement::pointer sp = context->expression_statement(*$1);
            $$ = new statement::pointer(sp);
            delete $1;
        }
    ;

expression_list
    : expression
        {
            $$ = new expression_list();
            $$->push_back(*$1);
            delete $1;
        }
    | expression_list comma expression
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    | expression_list error
        { $$ = $1; }
    ;

expression
    : LP expression RP
        { $$ = $2; }
    | INTEGER_CONSTANT
        {
            expression::pointer ep =
                context->integer_expression_factory(pascal_lex_location(), *$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | LONG_INTEGER_CONSTANT
        {
            if (!context->feature_long_integer_constants())
            {
                pascal_lex_warning
                (
                    "long integer constants were not available in the "
                    "UCSD native compiler"
                );
            }

            expression::pointer ep =
                context->lint_expression_factory(pascal_lex_location(), *$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | REAL_CONSTANT
        {
            expression::pointer ep =
                context->real_expression_factory(pascal_lex_location(), *$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | STRING_CONSTANT
        {
            expression::pointer ep =
                context->string_expression_factory(pascal_lex_location(), *$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | NAME
        {
            //
            // For now, we assume this expression is on the right-hand
            // side of an assignment.  A later production deals with
            // flipping to the left-hand side.
            //
            expression::pointer ep =
                context->name_expression(pascal_lex_location(), *$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | PROGRAM
        {
            // Pascal is weird.  So we defined "program" as a built-in symbol,
            // to make some feeble attempt at bringing a smidgin of consistency
            // to what is, basically, a totally WTF language.
            expression::pointer ep =
                context->name_expression(pascal_lex_location(), "program");
            $$ = new expression::pointer(ep);
        }
    | SIZEOF_tok LP expression RP
        {
            expression::pointer ep = context->sizeof_expression(*$3);
            $$ = new expression::pointer(ep);
            delete $3;
        }
    | SIZEOF_tok LP simple_type RP
        {
            expression::pointer ep =
                context->sizeof_expression(pascal_lex_location(), *$3);
            $$ = new expression::pointer(ep);
            delete $3;
        }
    | expression DOT variable_name
        {
            expression::pointer ep = context->dot_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression CARAT
        {
            expression::pointer ep = context->dereference_expression(*$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | MUL expression
        %prec unary
        {
            yyerror
            (
                "this is Pascal, not C, the dereference operator is a "
                "post-fix \"^\" not a prefix \"*\""
            );
            expression::pointer ep = context->dereference_expression(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | expression LB expression RB
        {
            expression::pointer ep = context->array_index_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression LB expression_list RB
        {
            expression::pointer ep = context->array_index_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression LP expression_list RP
        {
            expression::pointer ep =
                context->function_call_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression LP RP
        {
            context->complain_about_empty_parens();
            expression::pointer ep =
                context->function_call_expression(*$1, expression_list());
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | MINUS expression
        %prec unary
        {
            expression::pointer ep = context->negate_expression(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | PLUS expression
        %prec unary
        {
            expression::pointer ep = context->unary_plus_expression(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | NOT expression
        {
            expression::pointer ep = context->not_expression(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | AT expression
        {
            // This is an extension used by some Pascal implementations.
            // UCSD Pascal didn't originally have it.
            context->complain_about_address_of();
            expression::pointer ep =
                context->address_of_expression_factory(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
            // FIXME: we need a (*$extension*) to turn this on and off
        }
    | BITWISE_AND expression
        %prec AT
        {
            yyerror
            (
                "this is Pacal, not C, the address-of operator is "
                "written \"@\" not \"&\""
            );
            context->complain_about_address_of();
            expression::pointer ep =
                context->address_of_expression_factory(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    | expression MUL expression
        {
            expression::pointer ep =
                context->multiplication_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression DIV expression
        {
            expression::pointer ep =
                context->integer_division_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression MOD expression
        {
            expression::pointer ep = context->modulo_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression SLASH expression
        {
            expression::pointer ep =
                context->real_division_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression AND expression
        {
            expression::pointer ep = context->and_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression IN expression
        {
            DEBUG(1, "mark a");
            expression::pointer ep = context->in_expression(*$1, *$3);
            DEBUG(1, "mark b");
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
            DEBUG(1, "mark c");
        }
    | expression PLUS expression
        {
            expression::pointer ep =
                context->addition_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression MINUS expression
        {
            expression::pointer ep =
                context->subtraction_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression OR expression
        {
            expression::pointer ep =
                context->logical_or_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression EQ expression
        {
            expression::pointer ep = context->eq_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression NE expression
        {
            expression::pointer ep = context->ne_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression GE expression
        {
            expression::pointer ep = context->ge_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression GT expression
        {
            expression::pointer ep = context->gt_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression LE expression
        {
            expression::pointer ep = context->le_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression LT expression
        {
            expression::pointer ep = context->lt_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | expression ASSIGN expression
        {
            expression::pointer ep =
                context->assignment_expression_factory(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    | LB RB
        {
            expression::pointer ep = context->empty_set_expression();
            $$ = new expression::pointer(ep);
        }
    | LB set_expression_parts RB
        { $$ = $2; }
    | expression QUESTION_MARK expression COLON expression
        %prec ternary
        {
            expression::pointer ep =
                context->ternary_expression(*$1, *$3, *$5);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
            delete $5;
        }
    | SCAN LP expression COMMA scan_op expression COMMA expression RP
        {
            expression::pointer ep =
                context->scan_expression_factory(*$3, $5, *$6, *$8);
            $$ = new expression::pointer(ep);

            delete $3;
            delete $6;
            delete $8;
        }
    ;

scan_op
    : EQ
        { $$ = 0; }
    | NE
        { $$ = 1; }
    ;


set_expression_parts
    : set_expression_part
        { $$ = $1; }
    | set_expression_parts comma set_expression_part
        {
            expression::pointer ep = context->addition_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    ;

set_expression_part
    : expression
        {
            expression::pointer ep = context->set_expression(*$1);
            $$ = new expression::pointer(ep);
            delete $1;
        }
    | expression elipsis expression
        {
            expression::pointer ep = context->set_expression(*$1, *$3);
            $$ = new expression::pointer(ep);
            delete $1;
            delete $3;
        }
    ;

if_statement
    : IF expression THEN statement
        {
            DEBUG(1, "if {");
            statement::pointer sp = context->if_statement(*$2, *$4);
            $$ = new statement::pointer(sp);
            delete $2;
            delete $4;
            DEBUG(1, "}");
        }
    | IF expression THEN statement ELSE statement
        {
            DEBUG(1, "if {");
            statement::pointer sp = context->if_statement(*$2, *$4, *$6);
            $$ = new statement::pointer(sp);
            delete $2;
            delete $4;
            delete $6;
            DEBUG(1, "}");
        }
    ;

optional_semicolon
    : /* empty */
    | SEMICOLON
    ;

uses_clause
    : uses_clause_begin uses_clause_interfaces
    ;

uses_clause_begin
    : USES variable_names SEMICOLON
        {
            context->uses(*$2);
            delete $2;
        }
    ;

uses_clause_interfaces
    : /* empty */
    | uses_clause_interfaces uses_clause_interface
    ;

/*
 * After the translator (context) sees the USES clause, it will
 * insert a pseudo include file.  The specific tokens are fake, the
 * implementation_declarations actually come out of the appropriate
 * segment of the system.library file.
 */
uses_clause_interface
    : uses_clause_interface_push
      uses_clause_declarations
      uses_clause_interface_pop
    ;

/*
 * This is actually internal to the compiler, and not seen
 * in user programs.  It is used to parse the text extracted
 * from the unit's codefile.
 */
uses_clause_interface_push
    : UNIT NAME num num BEGIN
        {
            context->uses_interface_push(*$2, $3, -1, $4);
            delete $2;
        }
    | UNIT NAME num num num BEGIN
        {
            context->uses_interface_push(*$2, $3, $4, $5);
            delete $2;
        }
    ;

num
    : INTEGER_CONSTANT
        {
            $$ = pascal_lex_integer_constant_to_int(*$1);
            delete $1;
        }
    | MINUS num
        %prec unary
        {
            $$ = -$2;
        }
    ;

uses_clause_interface_pop
    : UNIT END SEMICOLON
        { context->uses_interface_pop(); }
    ;

uses_clause_declarations
    : /* empty */
    | uses_clause_declarations uses_clause_declaration
    ;

uses_clause_declaration
    : const_clause
    | type_clause
    | var_clause
    | function_prototype
        { context->scope_end(); }
    ;

for_statement
    : FOR expression ASSIGN expression for_dirn expression DO statement
        {
            statement::pointer sp =
                context->for_statement(*$2, *$4, $5, *$6, *$8);
            $$ = new statement::pointer(sp);
            delete $2;
            delete $4;
            delete $6;
            delete $8;
        }
    ;

for_dirn
    : TO
        { $$ = 0; }
    | DOWNTO
        { $$ = 1; }
    ;

case_statement
    : case_begin case_item_list optional_semicolon case_end
        {
            statement::pointer none;
            statement::pointer sp =
                context->case_statement_factory(*$1, *$2, none);
            $$ = new statement::pointer(sp);
            delete $1;
            delete $2;
        }
    | case_begin case_item_list SEMICOLON otherwise statement optional_semicolon
      case_end
        {
            statement::pointer sp =
                context->case_statement_factory(*$1, *$2, *$5);
            $$ = new statement::pointer(sp);
            delete $1;
            delete $2;
            delete $5;
        }
    ;

otherwise
    : OTHERWISE no_colon_here
        {
            context->complain_about_otherwise();
        }
    ;

no_colon_here
    : /* empty */
    | COLON
        {
            yyerror("there should be no colon after \"otherwise\"");
        }
    ;


case_begin
    : CASE expression OF
        {
            expression::pointer ep = context->case_stack_push(*$2);
            $$ = new expression::pointer(ep);
            delete $2;
        }
    ;

case_end
    : END
        { context->case_stack_pop(); }
    ;

case_item_list
    : case_item
        { $$ = $1; }
    | case_item_list SEMICOLON case_item
        {
            $$ = $1;
            $$->push_back(*$3);
            delete $3;
        }
    ;

case_item
    : case_range COLON statement
        {
            statement::pointer sp = *$3;
            $$ = new case_list(context->case_stack_type());
            $1->push_into_list(*$$, sp);
            delete $1;
            delete $3;
        }
    ;

case_expression
    : expression
        {
            // Quietly convert string constants of length one into
            // character constants, if necessary.
            expression::pointer ep = *$1;
            delete $1;
            expression::pointer ep2 =
                context->char_from_string_of_length_one(ep);
            if (ep2)
                ep = ep2;
            $$ = new expression::pointer(ep);
        }
    ;

case_range
    : case_expression
        {
            expression::pointer ep = *$1;
            delete $1;

            $$ = new case_range(context->case_stack_type());
            if (context->case_stack_check(ep))
                $$->push_back(ep);
        }
    | case_expression ELIPSIS case_expression
        {
            expression::pointer ep1 = *$1;
            delete $1;
            expression::pointer ep3 = *$3;
            delete $3;

            $$ = new case_range(context->case_stack_type());
            if
            (
                context->case_stack_check(ep1)
            &&
                context->case_stack_check(ep3)
            )
            {
                long v1 = ep1->optimize()->get_integer_value();
                long v3 = ep3->optimize()->get_integer_value();
                if (v1 > v3)
                {
                    pascal_lex_error
                    (
                        ep1->get_location(),
                        "case value range reversed"
                    );
                    expression::pointer x = ep1;
                    ep1 = ep3;
                    ep3 = x;
                }
                $$->push_back(ep1, ep3);
            }
        }
    | case_range COMMA case_range
        {
            $$ = new case_range(*$1 + *$3);
            delete $1;
            delete $3;
        }
    ;

label_statement
    : label_name_colon statement
        {
            statement::pointer sp = context->label_statement(*$1, *$2);
            $$ = new statement::pointer (sp);
            delete $1;
            delete $2;
        }
    ;

label_name_colon
    : label_name COLON
        { $$ = $1; }
    ;

goto_statement
    : GOTO label_name
        {
            statement::pointer sp = context->goto_statement(*$2);
            $$ = new statement::pointer(sp);
            delete $2;
        }
    ;

var_statement
    : VAR variable_names COLON complex_type
        {
            statement::pointer sp = context->var_statement(*$2, *$4);
            $$ = new statement::pointer(sp);
            delete $2;
            delete $4;
        }
    | VAR variable_name ASSIGN expression
        {
            statement::pointer sp = context->var_statement(*$2, *$4);
            $$ = new statement::pointer(sp);
            delete $2;
            delete $4;
        }
    ;

/*
 * Watch out for define_builtin_symbols.  You have to be sure that the
 * parse has yylex()ed at least one symbol, first, to ensure that any
 * (*$comments*) have been processed before the built-in symbols are
 * defined, because some (*$comments*) affect some built-in symbols.  In
 * this case, it had to fetch one symbol to know whether it was looking
 * at PROGRAM or SEGMENT or UNIT token.
 */
unit
    : define_builtin_symbols unit_common DOT
    ;

unit_common
    : unit_name_and_segments unit_interface unit_implementation unit_init
    ;

unit_name_and_segments
    : unit_name unit_segments
        {
            context->unit_scope(*$1);
            delete $1;
        }
    ;

unit_separate
    : /* empty */
    | SEPARATE
        {
            pascal_lex_warning
            (
                "the \"separate\" keyword is not present in the II.1 native "
                "compiler, and it's a bit odd in the II.0 native compiler"
            );

            //
            // Not nothing, precisely.
            //
            // The Apple Pascal compiler, based on II.1, treats the
            // SEPARATE keyword as an error.
            //
            // The II.0 compiler sources, on the other hand, use it to
            // choose between segkinds.  "SEPARATE" thus means "not
            // completely linked yet".
            // I.e. use a segkind of SEPRTSEG instead of UNITSEG.
            //
            // Sure would be nice to have some II.1 sources to read.
            //
            context->unit_is_separate();
        }
    ;

unit_name
    : unit_separate UNIT NAME SEMICOLON
        {
            $$ = $3;
            context->complain_if_not_swapping();
        }
    ;

unit_segments
    : /* empty */
    | INTRINSIC unit_code SEMICOLON
    | INTRINSIC unit_code unit_data SEMICOLON
    ;

unit_code
    : CODE_tok INTEGER_CONSTANT
        {
            long n = pascal_lex_integer_constant_to_int(*$2);
            context->unit_intrinsic_code(n);
            delete $2;
        }
    ;

unit_data
    : DATA_tok INTEGER_CONSTANT
        {
            long n = pascal_lex_integer_constant_to_int(*$2);
            context->unit_intrinsic_data(n);
            delete $2;
        }
    ;

unit_interface
    : INTERFACE unit_interface_declarations
        { context->unit_interface_end(); }
    ;

unit_interface_declarations
    : /* empty */
        { context->unit_interface_begin(); }
    | unit_interface_declarations unit_interface_declaration
    ;

unit_interface_declaration
    : const_clause
    | type_clause
    | var_clause
    /* | uses_clause */
    | function_prototype
        {
            // This is a FORWARD declaration in disguise
            context->scope_to_be_continued();
        }
    ;

unit_implementation
    : IMPLEMENTATION unit_implementation_declarations
        { context->unit_implementation_end(); }
    ;

unit_implementation_declarations
    : /* empty */
        { context->unit_implementation_begin(); }
    | unit_implementation_declarations unit_implementation_declaration
    ;

unit_implementation_declaration
    : const_clause
    | type_clause
    | var_clause
    | function
    ;

unit_init
    : compound_statement
        {
            context->scope_attach_body(*$1);
            delete $1;
        }
    | END
    ;

subunit_clause
    : subunit_check unit_common SEMICOLON
        {
            context->scope_end();
        }
    ;

subunit_check
    : /* empty */
        {
            context->complain_if_not_syscomp();
        }
    ;

%%


void
pascal_parse(translator *tp, const rcstring &filename)
{
    context = tp;
    extern int yyparse();
    pascal_lex_open(filename);
    yyparse();
    pascal_lex_close();
}


void
pascal_grammar_debug_set(bool flag)
{
    yydebug = flag;
}


void
pascal_grammar_comment(const rcstring &arg)
{
    context->comment(arg);
}


bool
pascal_grammar_warnings_are_silent(void)
{
    return context->warnings_are_silent();
}


bool
pascal_grammar_warnings_are_errors(void)
{
    return context->warnings_are_errors();
}


bool
pascal_grammar_string_escapes(void)
{
    return context->feature_string_escapes();
}


bool
pascal_grammar_type_name(const rcstring &name)
{
    return context->is_a_type_name(name);
}


void
pascal_grammar_complain_about_hex_constant(void)
{
    context->complain_about_hex_constant();
}


void
pascal_grammar_complain_about_binary_constant(void)
{
    context->complain_about_binary_constant();
}


void
pascal_grammar_fail_hook(void)
{
    if (context)
        context->fail_hook();
}


void
pascal_grammar_notify_interface_text(const rcstring &text)
{
    assert(context);
    context->unit_interface_text(text);
}


// vim: set ts=8 sw=4 et :

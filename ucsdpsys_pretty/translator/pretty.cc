//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or (at
// you option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#include <lib/pascal/lex.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/integer.h>
#include <lib/type/nothing.h>
#include <lib/type/real.h>
#include <lib/type/set/unsized.h>
#include <lib/type/string.h>

#include <ucsdpsys_pretty/expression/addition/pretty.h>
#include <ucsdpsys_pretty/expression/address_of.h>
#include <ucsdpsys_pretty/expression/boolean/pretty.h>
#include <ucsdpsys_pretty/expression/char/pretty.h>
#include <ucsdpsys_pretty/expression/dereference/pretty.h>
#include <ucsdpsys_pretty/expression/enum_tag/pretty.h>
#include <ucsdpsys_pretty/expression/eq/pretty.h>
#include <ucsdpsys_pretty/expression/ge/pretty.h>
#include <ucsdpsys_pretty/expression/gt/pretty.h>
#include <ucsdpsys_pretty/expression/in/pretty.h>
#include <ucsdpsys_pretty/expression/integer_division/pretty.h>
#include <ucsdpsys_pretty/expression/integer/pretty.h>
#include <ucsdpsys_pretty/expression/lint2lint/pretty.h>
#include <ucsdpsys_pretty/expression/lint_from_integer/pretty.h>
#include <ucsdpsys_pretty/expression/le/pretty.h>
#include <ucsdpsys_pretty/expression/logical_and/pretty.h>
#include <ucsdpsys_pretty/expression/logical_not/pretty.h>
#include <ucsdpsys_pretty/expression/logical_or/pretty.h>
#include <ucsdpsys_pretty/expression/long_integer/pretty.h>
#include <ucsdpsys_pretty/expression/lt/pretty.h>
#include <ucsdpsys_pretty/expression/modulo/pretty.h>
#include <ucsdpsys_pretty/expression/multiplication/pretty.h>
#include <ucsdpsys_pretty/expression/name/pretty.h>
#include <ucsdpsys_pretty/expression/negate/pretty.h>
#include <ucsdpsys_pretty/expression/ne/pretty.h>
#include <ucsdpsys_pretty/expression/nil/pretty.h>
#include <ucsdpsys_pretty/expression/paoc_from_string/pretty.h>
#include <ucsdpsys_pretty/expression/real_division/pretty.h>
#include <ucsdpsys_pretty/expression/real_from_integer/pretty.h>
#include <ucsdpsys_pretty/expression/real/pretty.h>
#include <ucsdpsys_pretty/expression/scan/pretty.h>
#include <ucsdpsys_pretty/expression/set/pretty.h>
#include <ucsdpsys_pretty/expression/set2set/pretty.h>
#include <ucsdpsys_pretty/expression/sgs/pretty.h>
#include <ucsdpsys_pretty/expression/sizeof/expression.h>
#include <ucsdpsys_pretty/expression/sizeof/type.h>
#include <ucsdpsys_pretty/expression/srs/pretty.h>
#include <ucsdpsys_pretty/expression/string/pretty.h>
#include <ucsdpsys_pretty/expression/subtraction/pretty.h>
#include <ucsdpsys_pretty/expression/ternary/pretty.h>
#include <ucsdpsys_pretty/expression/writeln.h>
#include <ucsdpsys_pretty/scope/builtin/pretty.h>
#include <ucsdpsys_pretty/scope/interface/pretty.h>
#include <ucsdpsys_pretty/scope/function/pretty.h>
#include <ucsdpsys_pretty/scope/program/pretty.h>
#include <ucsdpsys_pretty/scope/record/pretty.h>
#include <ucsdpsys_pretty/scope/temporary/compound/pretty.h>
#include <ucsdpsys_pretty/scope/temporary/with/pretty.h>
#include <ucsdpsys_pretty/scope/unit/pretty.h>
#include <ucsdpsys_pretty/statement/case/pretty.h>
#include <ucsdpsys_pretty/statement/compound/pretty.h>
#include <ucsdpsys_pretty/statement/for/pretty.h>
#include <ucsdpsys_pretty/statement/goto/pretty.h>
#include <ucsdpsys_pretty/statement/if/pretty.h>
#include <ucsdpsys_pretty/statement/infinite_loop/pretty.h>
#include <ucsdpsys_pretty/statement/label/pretty.h>
#include <ucsdpsys_pretty/statement/until/pretty.h>
#include <ucsdpsys_pretty/statement/while/pretty.h>
#include <ucsdpsys_pretty/statement/var_expression.h>
#include <ucsdpsys_pretty/statement/var_names.h>
#include <ucsdpsys_pretty/statement/with.h>
#include <ucsdpsys_pretty/statement/writeln/pretty.h>
#include <ucsdpsys_pretty/symbol/expression/constant/pretty.h>
#include <ucsdpsys_pretty/symbol/function/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


static rcstring minus("-");
static rcstring space(" ");
static rcstring comma(",");
static rcstring then("then");


static bool
undented_keyword(const rcstring &s)
{
    return (s == then);
}


translator_pretty::~translator_pretty()
{
}


translator_pretty::translator_pretty(const rcstring &filename) :
    os(output::factory(filename)),
    page_width(80),
    indent_width(4),
    indent_depth(0),
    undent(0)
{
}


scope::pointer
translator_pretty::program_scope_factory(const rcstring &name)
{
    return scope_program_pretty::create(name, this);
}


scope::pointer
translator_pretty::builtin_scope_factory(void)
{
    return scope_builtin_pretty::create(this);
}


scope::pointer
translator_pretty::unit_scope_factory(const rcstring &name, int code_seg,
    int data_seg, int llht, segkind_t)
{
    return scope_unit_pretty::create(name, code_seg, data_seg, llht, this);
}


scope::pointer
translator_pretty::function_scope_factory(const rcstring &name, bool is_func,
    bool is_seg)
{
    return scope_function_pretty::create(name, is_func, is_seg, this);
}


statement::pointer
translator_pretty::compound_statement_factory(const statement_list &arg)
{
    return statement_compound_pretty::create(arg, this);
}


statement::pointer
translator_pretty::writeln_statement_factory(const expression_list &arg,
    bool eoln)
{
    return statement_writeln_pretty::create(arg, eoln, this);
}


expression::pointer
translator_pretty::integer_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_integer_pretty::create(locn, arg, this);
}


expression::pointer
translator_pretty::lint_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_long_integer_pretty::create(locn, arg, this);
}


expression::pointer
translator_pretty::integer_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_pretty::create(arg, this);
}


expression::pointer
translator_pretty::real_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_pretty::create(arg, this);
}


expression::pointer
translator_pretty::lint_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_pretty::create(arg, this);
}


expression::pointer
translator_pretty::real_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_real_pretty::create(locn, arg, this);
}


expression::pointer
translator_pretty::string_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_string_pretty::create(locn, arg, this);
}


void
translator_pretty::indent_reset()
{
    indent_depth = 0;
}


void
translator_pretty::indent_more()
{
    indent_depth += indent_width;
}


void
translator_pretty::indent_less()
{
    if (line.empty())
        indent_depth -= indent_width;
    else
        ++undent;
}


void
translator_pretty::print_token(const rcstring &text)
{
    // Watch out for infelicitous combinations, like (-(-1))
    if (text == minus && !line.empty() && line.back() == minus)
        line.push_back(space);

    line.push_back(text);
}


void
translator_pretty::print_space()
{
    if (!line.empty() && line.back() != space)
        line.push_back(space);
}


static bool
line_too_long(const rcstring_list &line, size_t left, int max)
{
    //
    // If the line has only one token, it can't be too long, because it
    // can't be broken.
    //
    if (line.size() < 2)
        return false;

    if (max <= 0)
        return true;
    int column = 0;
    for (size_t j = left; j < line.size(); ++j)
    {
        column += line[j].size();
        if (column > max)
            return true;
    }
    return false;
}


static size_t
find_rightmost_space(const rcstring_list &line, size_t left, int max)
{
    size_t right = left;
    int column = 0;
    for (size_t j = left; j < line.size(); ++j)
    {
        const rcstring &tok = line[j];
        if (tok == space)
            right = j;
        if (j > left && undented_keyword(tok))
        {
            right = j;
            while (right > left && line[right - 1] == space)
                --right;
            return right;
        }
        int new_column = column + tok.size();
        if (new_column > max)
            return right;
        column = new_column;
    }
    return line.size();
}


static size_t
find_rightmost_comma(const rcstring_list &line, size_t left, int max)
{
    size_t best = 0;
    int column = 0;
    for (size_t right = left; right < line.size(); ++right)
    {
        const rcstring &tok = line[right];
        if (right > left && undented_keyword(tok))
        {
            while (right > left && line[right - 1] == space)
                --right;
            return right;
        }
        int new_column = column + tok.size();
        if (new_column > max)
            return best;
        if (tok == comma)
            best = right + 1;
        column = new_column;
    }
    return line.size();
}


static size_t
find_rightmost_token(const rcstring_list &line, size_t left, int max)
{
    //
    // To cope with the first token on the line being too long,
    // we always eat at least one token.
    //
    size_t right = left + 1;
    int column = line[left].size();
    for (; right < line.size(); ++right)
    {
        const rcstring &tok = line[right];
        if (undented_keyword(tok))
        {
            while (right > left && line[right - 1] == space)
                --right;
            return right;
        }
        int new_column = column + tok.size();
        if (new_column > max)
            return right;
        column = new_column;
    }
    return line.size();
}


static size_t
find_wrap_point(const rcstring_list &line, size_t left, int max)
{
    size_t right = find_rightmost_comma(line, left, max);
    if (right <= left)
        right = find_rightmost_space(line, left, max);
    if (right <= left)
        right = find_rightmost_token(line, left, max);
    return right;
}


void
translator_pretty::print_eoln(int extra_wrap_indent)
{
    size_t left = 0;
    int left_margin = indent_depth;

    //
    // Remove trailing spaces from the line.
    // (There shouldn't be any, most of the time, but there a few cases.)
    //
    while (!line.empty() && line.back() == space)
        line.pop_back();

    //
    // Get the trivial case out for the way first.
    //
    if (line.empty())
    {
        os->fputc('\n');
        goto done;
    }

    //
    // See if the rest of it fits on a single line.
    // If it does, we are done.
    //
    if (!line_too_long(line, 0, page_width - indent_depth))
    {
        for (int j = 0; j < indent_depth; ++j)
            os->fputc(' ');
        for (size_t k = 0; k < line.size(); ++k)
        {
            const rcstring &s(line[k]);
            os->fputs(s);
        }
        os->fputc('\n');
        goto done;
    }

    left = 0;
    left_margin = indent_depth;
    for (;;)
    {
        //
        // Try to find white space before the break point; failing that,
        // find *any* token before the break point.
        //
        size_t right =
            find_wrap_point(line, left, page_width - left_margin);

        //
        // Output the partial line we have so far.
        //
        int column = 0;
        if (undented_keyword(line[left]))
        {
            for (; column < indent_depth; ++column)
                os->fputc(' ');
        }
        else
        {
            for (; column < left_margin; ++column)
                os->fputc(' ');
        }
        for (size_t k = left; k < right; ++k)
        {
            const rcstring &s(line[k]);
            os->fputs(s);
            column += s.size();
        }
        if (right >= line.size())
        {
            os->fputc('\n');
            break;
        }
        os->fputc('\n');

        //
        // We use a hanging indent for wrapped lines.
        //
        if (left == 0)
            left_margin += indent_width + extra_wrap_indent;

        //
        // Advance to the next line.
        //
        left = right;
        while (left < line.size() && line[left] == space)
            ++left;
        if (left >= line.size())
            break;
    }

    //
    // Prepare for the next line of code.
    //
done:
    line.clear();
    indent_depth -= undent * indent_width;
    undent = 0;
    if (indent_depth < 0)
        indent_depth = 0;
}


expression::pointer
translator_pretty::modulo_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_modulo_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_plus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_plus_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_union_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_plus_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_minus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_minus_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_difference_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_times_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_times_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_intersection_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_div_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_integer_division_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_over_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_real_division_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::boolean_expression_factory(const location &locn, bool value)
{
    return expression_boolean_pretty::create(locn, value, this);
}


expression::pointer
translator_pretty::array_eq_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::boolean_eq_boolean_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_eq_integer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_eq_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::pointer_eq_pointer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_eq_real_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::string_eq_string_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::array_ne_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::boolean_ne_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_ne_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::pointer_ne_pointer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_ne_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::string_ne_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_ne_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::boolean_lt_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_lt_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_lt_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::string_lt_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::boolean_le_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_le_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_le_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::string_le_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_le_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::boolean_gt_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_gt_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_gt_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::string_gt_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::boolean_ge_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_ge_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::real_ge_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::string_ge_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_pretty::create(lhs, rhs, this);
}


statement::pointer
translator_pretty::if_statement_factory(const expression::pointer &a_cond,
    const statement::pointer &a_thn, const statement::pointer &a_els)
{
    return statement_if_pretty::create(a_cond, a_thn, a_els, this);
}


symbol::pointer
translator_pretty::function_symbol_factory(rcstring const &a_name,
    const scope::pointer &scp, int a_lex_level, bool new_seg)
{
    return
        symbol_function_pretty::create(a_name, scp, a_lex_level, new_seg, this);
}


symbol::pointer
translator_pretty::enumerated_symbol_factory(const variable_name &vname,
    const type_enumerated::pointer &vtype, int lex_level, int value)
{
    return
        symbol_expression_constant_pretty::create
        (
            vname,
            lex_level,
            expression_enum_tag_pretty::create
            (
                vname,
                vtype,
                value,
                this
            ),
            this
        );
}


expression::pointer
translator_pretty::dereference_expression_factory(
    const expression::pointer &operand)
{
    return expression_dereference_pretty::create(operand, this);
}


expression::pointer
translator_pretty::address_of_expression_factory(
    const expression::pointer &operand)
{
    return expression_address_of::create(operand, this);
}


statement::pointer
translator_pretty::while_statement_factory(const expression::pointer &a1,
    const statement::pointer &a2)
{
    return statement_while_pretty::create(a1, a2, this);
}


statement::pointer
translator_pretty::with_statement_factory(const expression::pointer &ep,
    const statement::pointer &, const statement::pointer &body)
{
    return statement_with::create(ep, body, this);
}


statement::pointer
translator_pretty::until_statement_factory(const statement_list &a1,
    const expression::pointer &a2)
{
    return statement_until_pretty::create(a1, a2, this);
}


statement::pointer
translator_pretty::infinite_loop_factory(const statement::pointer &arg)
{
    return statement_infinite_loop_pretty::create(arg, this);
}


expression::pointer
translator_pretty::logical_and_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_and_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::bitwise_and_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_and_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::logical_or_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_or_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::bitwise_or_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_or_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::logical_not_expression_factory(
    const expression::pointer &arg)
{
    return expression_logical_not_pretty::create(arg, this);
}


expression::pointer
translator_pretty::bitwise_not_expression_factory(
    const expression::pointer &arg)
{
    return expression_logical_not_pretty::create(arg, this);
}


expression::pointer
translator_pretty::real_from_integer_expression_factory(
    const expression::pointer &arg)
{
    return expression_real_from_integer_pretty::create(arg, this);
}


expression::pointer
translator_pretty::sizeof_expression_factory(
    const expression::pointer &arg)
{
    return expression_sizeof_expression::create(arg, this);
}


expression::pointer
translator_pretty::sizeof_expression_factory(const location &locn,
    const type::pointer &arg)
{
    return expression_sizeof_type::create(locn, arg, this);
}


symbol::pointer
translator_pretty::builtin_abs_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("abs", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_arctan_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("arctan", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_atan_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("atan", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_blockread_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("blockread", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_blockwrite_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("blockwrite", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_chr_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("chr", this);
    sp->attach_return_type(type_char::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_close_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("close", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_concat_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("concat", this);
    sp->attach_return_type(type_string::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_copy_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("copy", this);
    sp->attach_return_type(type_string::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_cos_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("cos", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_dec_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("dec", this);
    sp->attach_return_type(type_char::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_delete_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("delete", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_eof_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("eof", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_eoln_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("eoln", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_exit_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("exit", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_exp_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("exp", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_fillchar_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("fillchar", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_get_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("get", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_gotoxy_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("gotoxy", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_halt_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("halt", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_idsearch_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("idsearch", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_inc_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("inc", this);
    sp->attach_return_type(type_char::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_insert_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("insert", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_ioresult_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("ioresult", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_length_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("length", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_ln_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("ln", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_log_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("log", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_mark_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("mark", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_memavail_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("memavail", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_moveleft_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("moveleft", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_moveright_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("moveright", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_new_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("new", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_odd_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("odd", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_opennew_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("opennew", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_openold_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("openold", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_ord_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("ord", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_page_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("page", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_pos_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("pos", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_sin_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("sin", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_pred_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("pred", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_put_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("put", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_pwroften_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("pwroften", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_read_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("read", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_readln_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("readln", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_release_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("release", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_reset_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("reset", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_rewrite_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("rewrite", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_round_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("round", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_seek_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("seek", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_sqr_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("sqr", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_sqrt_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("sqrt", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_str_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("str", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_succ_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("succ", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_time_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("time", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_treesearch_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("treesearch", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_trunc_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("trunc", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_unitbusy_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("unitbusy", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_unitclear_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("unitclear", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_unitread_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("unitread", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_unitstatus_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("unitstatus", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_unitwait_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("unitwait", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_pretty::builtin_unitwrite_factory()
{
    symbol_function::pointer sp =
        symbol_function_pretty::create_builtin("unitwrite", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol_function::pointer
translator_pretty::builtin_syscall_factory(const rcstring &name, int, int)
{
    return symbol_function_pretty::create_builtin(name, this);
}


expression::pointer
translator_pretty::set2set_expression_factory(const type::pointer &to,
    const expression::pointer &from)
{
    return expression_set2set_pretty::create(to, from, this);
}


expression::pointer
translator_pretty::empty_set_expression_factory(const location &locn,
    const type::pointer &tp)
{
    return expression_set_pretty::create(locn, tp, set(), this);
}


expression::pointer
translator_pretty::set_expression_factory(const expression::pointer &sng)
{
    return expression_sgs_pretty::create(sng, this);
}


expression::pointer
translator_pretty::set_expression_factory(expression::pointer const &lhs,
    const expression::pointer &rhs)
{
    return expression_srs_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::char_expression_factory(location const &locn,
    unsigned char value)
{
    return expression_char_pretty::create(locn, value, this);
}


expression::pointer
translator_pretty::writeln1_expression_factory(const expression::pointer &arg)
{
    return arg;
}


expression::pointer
translator_pretty::writeln2_expression_factory(const expression::pointer &arg1,
    const expression::pointer &arg2)
{
    return expression_writeln::create(arg1, arg2, this);
}


expression::pointer
translator_pretty::writeln3_expression_factory(const expression::pointer &arg1,
    const expression::pointer &arg2, const expression::pointer &arg3)
{
    return
        writeln2_expression_factory
        (
            writeln2_expression_factory(arg1, arg2),
            arg3
        );
}


scope::pointer
translator_pretty::interface_scope_factory(const rcstring &name, int, int,
    segkind_t)
{
    return scope_interface_pretty::create(name, this);
}


scope::pointer
translator_pretty::compound_temporary_scope_factory(scope_stack &stk)
{
    return scope_temporary_compound_pretty::create(stk, this);
}


scope::pointer
translator_pretty::with_temporary_scope_factory(scope_stack &stk,
    const expression::pointer &base, const statement::pointer &assign)
{
    return scope_temporary_with_pretty::create(stk, base, assign, this);
}


scope::pointer
translator_pretty::record_scope_factory(bool packed)
{
    return scope_record_pretty::create(packed, this);
}


expression::pointer
translator_pretty::implicit_dot_expression(const expression::pointer &lhs,
    const variable_name &rhs)
{
    type_record::pointer trp =
        boost::dynamic_pointer_cast<type_record>(lhs->get_type());
    assert(trp);
    symbol::pointer sp = trp->lookup(rhs.get_name());
    assert(sp);
    sp->add_reference(rhs);
    return sp->name_expression_factory(sp, rhs.get_location(), get_lex_level());
}


expression::pointer
translator_pretty::nil_expression_factory()
{
    return expression_nil_pretty::create(pascal_lex_location(), this);
}


statement::pointer
translator_pretty::for_statement_factory(const expression::pointer &var,
    const expression::pointer &limit1, bool descending,
    const expression::pointer &limit2, const statement::pointer &body)
{
    return
        statement_for_pretty::create
        (
            var,
            limit1,
            descending,
            limit2,
            body,
            this
        );
}


statement::pointer
translator_pretty::case_statement_factory(const expression::pointer &control,
    const case_list &cases, const statement::pointer &otherwise)
{
    return statement_case_pretty::create(control, cases, otherwise, this);
}


statement::pointer
translator_pretty::label_statement_factory(const symbol::pointer &sp,
    const statement::pointer &body)
{
    return statement_label_pretty::create(sp, body, this);
}


statement::pointer
translator_pretty::goto_statement_factory(const symbol::pointer &sp)
{
    return statement_goto_pretty::create(sp, this);
}


expression::pointer
translator_pretty::in_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_in_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::paoc_from_string_expression_factory(
    const expression::pointer &rhs, const type::pointer &tp)
{
    return expression_paoc_from_string_pretty::create(rhs, tp);
}


expression::pointer
translator_pretty::array_lt_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::array_le_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::array_ge_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::array_gt_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::record_eq_record_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::record_ne_record_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_eq_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_le_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_lt_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_lt_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_ge_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_ge_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_gt_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_gt_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::set_ne_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_pretty::create(lhs, rhs, this);
}


statement::pointer
translator_pretty::var_statement_factory(const variable_name_list &names,
    const type::pointer &tp)
{
    return statement_var_names::create(names, tp, this);
}


statement::pointer
translator_pretty::var_statement_factory(const variable_name &name,
    const expression::pointer &value)
{
    return statement_var_expression::create(name, value, this);
}


expression::pointer
translator_pretty::scan_expression_factory(const expression::pointer &maxdisp,
    int mask, const expression::pointer &value,
    const expression::pointer &start)
{
    return expression_scan_pretty::create(maxdisp, mask, value, start, this);
}


void
translator_pretty::unit_interface_text(const rcstring &)
{
    // Ignore.
}


expression::pointer
translator_pretty::lint2lint_expression_factory(const type::pointer &to,
    const expression::pointer &from)
{
    return expression_lint2lint_pretty::create(to, from, this);
}


expression::pointer
translator_pretty::lint_from_integer_expression_factory(
    const expression::pointer &operand)
{
    return expression_lint_from_integer_pretty::create(operand);
}


expression::pointer
translator_pretty::lint_minus_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_times_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::lint_over_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_integer_division_pretty::create(lhs, rhs, this);
}


expression::pointer
translator_pretty::ternary_expression_factory(const expression::pointer &cond,
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ternary_pretty::create(cond, lhs, rhs, this);
}


expression::pointer
translator_pretty::integer_from_lint_expression_factory(
    const expression::pointer &operand)
{
    expression_list parms;
    expression::pointer f = name_expression(operand->get_location(), "trunc");
    parms.push_back(f);
    parms.push_back(operand);
    return f->function_call_expression_factory(parms);
}


// vim: set ts=8 sw=4 et :

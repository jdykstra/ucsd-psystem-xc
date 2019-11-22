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
#include <lib/symbol/expression/with.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/integer.h>
#include <lib/type/nothing.h>
#include <lib/type/real.h>
#include <lib/type/set/unsized.h>
#include <lib/type/string.h>

#include <ucsdpsys_littoral/expression/addition/littoral.h>
#include <ucsdpsys_littoral/expression/address_of.h>
#include <ucsdpsys_littoral/expression/boolean/littoral.h>
#include <ucsdpsys_littoral/expression/char/littoral.h>
#include <ucsdpsys_littoral/expression/dereference/littoral.h>
#include <ucsdpsys_littoral/expression/enum_tag/littoral.h>
#include <ucsdpsys_littoral/expression/eq/littoral.h>
#include <ucsdpsys_littoral/expression/ge/littoral.h>
#include <ucsdpsys_littoral/expression/gt/littoral.h>
#include <ucsdpsys_littoral/expression/in/littoral.h>
#include <ucsdpsys_littoral/expression/integer_division/littoral.h>
#include <ucsdpsys_littoral/expression/integer/littoral.h>
#include <ucsdpsys_littoral/expression/lint2lint/littoral.h>
#include <ucsdpsys_littoral/expression/lint_from_integer/littoral.h>
#include <ucsdpsys_littoral/expression/le/littoral.h>
#include <ucsdpsys_littoral/expression/logical_and/littoral.h>
#include <ucsdpsys_littoral/expression/logical_not/littoral.h>
#include <ucsdpsys_littoral/expression/logical_or/littoral.h>
#include <ucsdpsys_littoral/expression/long_integer/littoral.h>
#include <ucsdpsys_littoral/expression/lt/littoral.h>
#include <ucsdpsys_littoral/expression/modulo/littoral.h>
#include <ucsdpsys_littoral/expression/multiplication/littoral.h>
#include <ucsdpsys_littoral/expression/name/littoral.h>
#include <ucsdpsys_littoral/expression/negate/littoral.h>
#include <ucsdpsys_littoral/expression/ne/littoral.h>
#include <ucsdpsys_littoral/expression/nil/littoral.h>
#include <ucsdpsys_littoral/expression/paoc_from_string/littoral.h>
#include <ucsdpsys_littoral/expression/real_division/littoral.h>
#include <ucsdpsys_littoral/expression/real_from_integer/littoral.h>
#include <ucsdpsys_littoral/expression/real/littoral.h>
#include <ucsdpsys_littoral/expression/scan/littoral.h>
#include <ucsdpsys_littoral/expression/set/littoral.h>
#include <ucsdpsys_littoral/expression/set2set/littoral.h>
#include <ucsdpsys_littoral/expression/sgs/littoral.h>
#include <ucsdpsys_littoral/expression/sizeof/expression.h>
#include <ucsdpsys_littoral/expression/sizeof/type.h>
#include <ucsdpsys_littoral/expression/srs/littoral.h>
#include <ucsdpsys_littoral/expression/string/littoral.h>
#include <ucsdpsys_littoral/expression/subtraction/littoral.h>
#include <ucsdpsys_littoral/expression/ternary/littoral.h>
#include <ucsdpsys_littoral/expression/writeln.h>
#include <ucsdpsys_littoral/scope/builtin/littoral.h>
#include <ucsdpsys_littoral/scope/temporary/compound/littoral.h>
#include <ucsdpsys_littoral/scope/temporary/with/littoral.h>
#include <ucsdpsys_littoral/scope/interface/littoral.h>
#include <ucsdpsys_littoral/scope/function/littoral.h>
#include <ucsdpsys_littoral/scope/program/littoral.h>
#include <ucsdpsys_littoral/scope/record/littoral.h>
#include <ucsdpsys_littoral/scope/unit/littoral.h>
#include <ucsdpsys_littoral/statement/case/littoral.h>
#include <ucsdpsys_littoral/statement/compound/littoral.h>
#include <ucsdpsys_littoral/statement/empty/littoral.h>
#include <ucsdpsys_littoral/statement/expression/littoral.h>
#include <ucsdpsys_littoral/statement/for/littoral.h>
#include <ucsdpsys_littoral/statement/goto/littoral.h>
#include <ucsdpsys_littoral/statement/if/littoral.h>
#include <ucsdpsys_littoral/statement/infinite_loop/littoral.h>
#include <ucsdpsys_littoral/statement/label/littoral.h>
#include <ucsdpsys_littoral/statement/until/littoral.h>
#include <ucsdpsys_littoral/statement/while/littoral.h>
#include <ucsdpsys_littoral/statement/var_expression.h>
#include <ucsdpsys_littoral/statement/var_names.h>
#include <ucsdpsys_littoral/statement/with.h>
#include <ucsdpsys_littoral/statement/writeln/littoral.h>
#include <ucsdpsys_littoral/symbol/function/littoral.h>
#include <ucsdpsys_littoral/symbol/expression/constant/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


static rcstring minus("-");
static rcstring space(" ");
static rcstring comma(",");
static rcstring then("then");


static bool
undented_keyword(const rcstring &s)
{
    return (s == then);
}


translator_littoral::~translator_littoral()
{
}


translator_littoral::translator_littoral(const rcstring &filename) :
    os(output::factory(filename)),
    page_width(80),
    indent_width(4),
    indent_depth(0),
    undent(0)
{
}


scope::pointer
translator_littoral::program_scope_factory(const rcstring &name)
{
    return scope_program_littoral::create(name, this);
}


scope::pointer
translator_littoral::builtin_scope_factory(void)
{
    return scope_builtin_littoral::create(this);
}


scope::pointer
translator_littoral::unit_scope_factory(const rcstring &name, int code_seg,
    int data_seg, int llht, segkind_t)
{
    return scope_unit_littoral::create(name, code_seg, data_seg, llht, this);
}


scope::pointer
translator_littoral::function_scope_factory(const rcstring &name, bool, bool)
{
    return scope_function_littoral::create(name, this);
}


statement::pointer
translator_littoral::compound_statement_factory(const statement_list &arg)
{
    return statement_compound_littoral::create(arg, this);
}


statement::pointer
translator_littoral::writeln_statement_factory(const expression_list &arg,
    bool eoln)
{
    return statement_writeln_littoral::create(arg, eoln, this);
}


expression::pointer
translator_littoral::integer_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_integer_littoral::create(locn, arg, this);
}


expression::pointer
translator_littoral::lint_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_long_integer_littoral::create(locn, arg, this);
}


expression::pointer
translator_littoral::integer_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_littoral::create(arg, this);
}


expression::pointer
translator_littoral::real_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_littoral::create(arg, this);
}


expression::pointer
translator_littoral::lint_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_littoral::create(arg, this);
}


expression::pointer
translator_littoral::real_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_real_littoral::create(locn, arg, this);
}


expression::pointer
translator_littoral::string_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_string_littoral::create(locn, arg, this);
}


void
translator_littoral::indent_reset()
{
    indent_depth = 0;
}


void
translator_littoral::indent_more()
{
    indent_depth += indent_width;
}


void
translator_littoral::indent_less()
{
    if (line.empty())
        indent_depth -= indent_width;
    else
        ++undent;
}


void
translator_littoral::print_token(const rcstring &text)
{
    // Watch out for infelicitous combinations, like (-(-1))
    if (text == minus && !line.empty() && line.back() == minus)
        line.push_back(space);

    line.push_back(text);
}


void
translator_littoral::print_space()
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
translator_littoral::print_eoln(int extra_wrap_indent)
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
    os->flush();

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
translator_littoral::modulo_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_modulo_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_plus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_plus_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_union_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_plus_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_addition_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_minus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_minus_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_difference_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_times_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_times_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_intersection_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_div_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_integer_division_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_over_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_real_division_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::boolean_expression_factory(
    const location &locn, bool value)
{
    return expression_boolean_littoral::create(locn, value, this);
}


expression::pointer
translator_littoral::array_eq_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::boolean_eq_boolean_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_eq_integer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_eq_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::pointer_eq_pointer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_eq_real_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::string_eq_string_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::array_ne_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::boolean_ne_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_ne_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::pointer_ne_pointer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_ne_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::string_ne_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_ne_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::boolean_lt_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_lt_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_lt_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::string_lt_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::boolean_le_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_le_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_le_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::string_le_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_le_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::boolean_gt_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_gt_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_gt_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::string_gt_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::boolean_ge_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_ge_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::real_ge_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::string_ge_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_littoral::create(lhs, rhs, this);
}


statement::pointer
translator_littoral::if_statement_factory(const expression::pointer &a_cond,
    const statement::pointer &a_thn, const statement::pointer &a_els)
{
    return statement_if_littoral::create(a_cond, a_thn, a_els, this);
}


symbol::pointer
translator_littoral::function_symbol_factory(rcstring const &a_name,
    const scope::pointer &scp, int a_lex_level, bool new_seg)
{
    return
        symbol_function_littoral::create
        (
            a_name,
            scp,
            a_lex_level,
            new_seg,
            this
        );
}


symbol::pointer
translator_littoral::enumerated_symbol_factory(const variable_name &vname,
    const type_enumerated::pointer &vtype, int lex_level, int value)
{
    return
        symbol_expression_constant_littoral::create
        (
            vname,
            lex_level,
            expression_enum_tag_littoral::create(vname, vtype, value, this),
            this
        );
}


expression::pointer
translator_littoral::dereference_expression_factory(
    const expression::pointer &operand)
{
    return expression_dereference_littoral::create(operand, this);
}


expression::pointer
translator_littoral::address_of_expression_factory(
    const expression::pointer &operand)
{
    return expression_address_of::create(operand, this);
}


statement::pointer
translator_littoral::while_statement_factory(const expression::pointer &a1,
    const statement::pointer &a2)
{
    return statement_while_littoral::create(a1, a2, this);
}


statement::pointer
translator_littoral::with_statement_factory(const expression::pointer &ep,
    const statement::pointer &base_asign, const statement::pointer &body)
{
    if (!base_asign)
        return body;
    return statement_with::create(ep, body, this);
}


statement::pointer
translator_littoral::until_statement_factory(const statement_list &a1,
    const expression::pointer &a2)
{
    return statement_until_littoral::create(a1, a2, this);
}


statement::pointer
translator_littoral::infinite_loop_factory(const statement::pointer &arg)
{
    return statement_infinite_loop_littoral::create(arg, this);
}


expression::pointer
translator_littoral::logical_and_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_and_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::bitwise_and_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_and_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::logical_or_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_or_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::bitwise_or_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_or_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::logical_not_expression_factory(
    const expression::pointer &arg)
{
    return expression_logical_not_littoral::create(arg, this);
}


expression::pointer
translator_littoral::bitwise_not_expression_factory(
    const expression::pointer &arg)
{
    return expression_logical_not_littoral::create(arg, this);
}


expression::pointer
translator_littoral::real_from_integer_expression_factory(
    const expression::pointer &arg)
{
    return expression_real_from_integer_littoral::create(arg, this);
}


expression::pointer
translator_littoral::sizeof_expression_factory(
    const expression::pointer &arg)
{
    return expression_sizeof_expression::create(arg, this);
}


expression::pointer
translator_littoral::sizeof_expression_factory(const location &locn,
    const type::pointer &arg)
{
    return expression_sizeof_type::create(locn, arg, this);
}


symbol::pointer
translator_littoral::builtin_abs_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("abs", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_arctan_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("arctan", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_atan_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("atan", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_blockread_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("blockread", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_blockwrite_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("blockwrite", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_chr_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("chr", this);
    sp->attach_return_type(type_char::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_close_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("close", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_concat_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("concat", this);
    sp->attach_return_type(type_string::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_copy_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("copy", this);
    sp->attach_return_type(type_string::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_cos_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("cos", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_dec_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("dec", this);
    sp->attach_return_type(type_char::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_delete_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("delete", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_eof_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("eof", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_eoln_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("eoln", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_exit_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("exit", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_exp_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("exp", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_fillchar_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("fillchar", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_get_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("get", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_gotoxy_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("gotoxy", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_halt_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("halt", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_idsearch_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("idsearch", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_inc_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("inc", this);
    sp->attach_return_type(type_char::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_insert_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("insert", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_ioresult_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("ioresult", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_length_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("length", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_ln_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("ln", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_log_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("log", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_mark_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("mark", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_memavail_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("memavail", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_moveleft_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("moveleft", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_moveright_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("moveright", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_new_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("new", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_odd_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("odd", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_opennew_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("opennew", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_openold_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("openold", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_ord_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("ord", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_page_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("page", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_pos_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("pos", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_sin_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("sin", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_pred_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("pred", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_put_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("put", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_pwroften_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("pwroften", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_read_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("read", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_readln_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("readln", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_release_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("release", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_reset_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("reset", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_rewrite_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("rewrite", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_round_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("round", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_seek_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("seek", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_sqr_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("sqr", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_sqrt_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("sqrt", this);
    sp->attach_return_type(type_real::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_str_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("str", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_succ_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("succ", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_time_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("time", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_treesearch_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("treesearch", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_trunc_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("trunc", this);
    sp->attach_return_type(type_integer::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_unitbusy_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("unitbusy", this);
    sp->attach_return_type(type_boolean::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_unitclear_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("unitclear", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_unitread_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("unitread", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_unitstatus_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("unitstatus", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_unitwait_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("unitwait", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol::pointer
translator_littoral::builtin_unitwrite_factory()
{
    symbol_function::pointer sp =
        symbol_function_littoral::create_builtin("unitwrite", this);
    // sp->attach_return_type(type_nothing::create());
    return sp;
}


symbol_function::pointer
translator_littoral::builtin_syscall_factory(const rcstring &name, int, int)
{
    return symbol_function_littoral::create_builtin(name, this);
}


expression::pointer
translator_littoral::set2set_expression_factory(const type::pointer &to,
    const expression::pointer &from)
{
    return expression_set2set_littoral::create(to, from, this);
}


expression::pointer
translator_littoral::empty_set_expression_factory(const location &locn,
    const type::pointer &tp)
{
    return expression_set_littoral::create(locn, tp, set(), this);
}


expression::pointer
translator_littoral::set_expression_factory(const expression::pointer &sng)
{
    return expression_sgs_littoral::create(sng, this);
}


expression::pointer
translator_littoral::set_expression_factory(expression::pointer const &lhs,
    const expression::pointer &rhs)
{
    return expression_srs_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::char_expression_factory(location const &locn,
    unsigned char value)
{
    return expression_char_littoral::create(locn, value, this);
}


expression::pointer
translator_littoral::writeln1_expression_factory(const expression::pointer &arg)
{
    return arg;
}


expression::pointer
translator_littoral::writeln2_expression_factory(
    const expression::pointer &arg1, const expression::pointer &arg2)
{
    return expression_writeln::create(arg1, arg2, this);
}


expression::pointer
translator_littoral::writeln3_expression_factory(
    const expression::pointer &arg1, const expression::pointer &arg2,
    const expression::pointer &arg3)
{
    return
        writeln2_expression_factory
        (
            writeln2_expression_factory(arg1, arg2),
            arg3
        );
}


scope::pointer
translator_littoral::interface_scope_factory(const rcstring &name, int, int,
    segkind_t)
{
    return scope_interface_littoral::create(name, this);
}


scope::pointer
translator_littoral::compound_temporary_scope_factory(scope_stack &stk)
{
    return scope_temporary_compound_littoral::create(stk, this);
}


scope::pointer
translator_littoral::with_temporary_scope_factory(scope_stack &stk,
    const expression::pointer &base, const statement::pointer &assign)
{
    return scope_temporary_with_littoral::create(stk, base, assign, this);
}


scope::pointer
translator_littoral::record_scope_factory(bool packed)
{
    return scope_record_littoral::create(packed, this);
}


expression::pointer
translator_littoral::nil_expression_factory()
{
    return expression_nil_littoral::create(pascal_lex_location(), this);
}


statement::pointer
translator_littoral::for_statement_factory(const expression::pointer &var,
    const expression::pointer &limit1, bool descending,
    const expression::pointer &limit2, const statement::pointer &body)
{
    return
        statement_for_littoral::create
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
translator_littoral::case_statement_factory(const expression::pointer &control,
    const case_list &cases, const statement::pointer &otherwise)
{
    return statement_case_littoral::create(control, cases, otherwise, this);
}


statement::pointer
translator_littoral::label_statement_factory(const symbol::pointer &sp,
    const statement::pointer &body)
{
    return statement_label_littoral::create(sp, body, this);
}


statement::pointer
translator_littoral::goto_statement_factory(const symbol::pointer &sp)
{
    return statement_goto_littoral::create(sp, this);
}


expression::pointer
translator_littoral::in_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_in_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::paoc_from_string_expression_factory(
    const expression::pointer &rhs, const type::pointer &tp)
{
    return expression_paoc_from_string_littoral::create(rhs, tp);
}


expression::pointer
translator_littoral::array_lt_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::array_le_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::array_ge_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::array_gt_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::record_eq_record_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::record_ne_record_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_eq_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_le_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_lt_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_lt_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_ge_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_ge_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_gt_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_gt_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::set_ne_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_littoral::create(lhs, rhs, this);
}


statement::pointer
translator_littoral::var_statement_factory(const variable_name_list &names,
    const type::pointer &tp)
{
    return statement_var_names::create(names, tp, this);
}


statement::pointer
translator_littoral::var_statement_factory(const variable_name &name,
    const expression::pointer &value)
{
    return statement_var_expression::create(name, value, this);
}


expression::pointer
translator_littoral::scan_expression_factory(const expression::pointer &maxdisp,
    int mask, const expression::pointer &value,
    const expression::pointer &start)
{
    return expression_scan_littoral::create(maxdisp, mask, value, start, this);
}


void
translator_littoral::unit_interface_text(const rcstring &)
{
    // Ignore.
}


expression::pointer
translator_littoral::lint2lint_expression_factory(const type::pointer &to,
    const expression::pointer &from)
{
    return expression_lint2lint_littoral::create(to, from, this);
}


expression::pointer
translator_littoral::lint_from_integer_expression_factory(
    const expression::pointer &operand)
{
    return expression_lint_from_integer_littoral::create(operand);
}


expression::pointer
translator_littoral::lint_minus_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_times_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::lint_over_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_integer_division_littoral::create(lhs, rhs, this);
}


expression::pointer
translator_littoral::ternary_expression_factory(const expression::pointer &cond,
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ternary_littoral::create(cond, lhs, rhs, this);
}


expression::pointer
translator_littoral::integer_from_lint_expression_factory(
    const expression::pointer &operand)
{
    expression_list parms;
    expression::pointer f = name_expression(operand->get_location(), "trunc");
    parms.push_back(f);
    parms.push_back(operand);
    return f->function_call_expression_factory(parms);
}


statement::pointer
translator_littoral::expression_statement_factory(
    const expression::pointer &arg)
{
    return statement_expression_littoral::create(arg, this);
}


statement::pointer
translator_littoral::empty_statement_factory(void)
{
    return statement_empty_littoral::create(this);
}


// vim: set ts=8 sw=4 et :

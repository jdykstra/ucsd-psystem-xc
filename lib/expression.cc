//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#include <lib/ac/assert.h>
#include <lib/ac/typeinfo>

#include <lib/debug.h>
#include <lib/expression.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/type/error.h>
#include <lib/type/record.h>


expression::~expression()
{
}


expression::expression() :
    loc(pascal_lex_location())
{
}


expression::expression(const location &arg) :
    loc(arg)
{
}


expression::expression(const expression &arg) :
    loc(arg.loc)
{
}


expression &
expression::operator=(const expression &arg)
{
    if (this != &arg)
    {
        loc = arg.loc;
    }
    return *this;
}


void
expression::traversal(int)
    const
{
    // Do nothing.
}


bool
expression::need_parens(int parent_precedence)
    const
{
    return need_parens(parent_precedence, get_precedence());
}


bool
expression::need_parens(int parent_precedence, int child_precedence)
    const
{
    return (parent_precedence > child_precedence);
}


expression::pointer
expression::assignment_expression_factory(const expression::pointer &)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", typeid(*this).name());
    pascal_lex_error
    (
        get_location(),
        "left hand side of assignment not appropriate"
    );
    return expression_error::create(get_location());
}


expression::pointer
expression::function_call_expression_factory(const expression_list &rhs)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", typeid(*this).name());
    assert(rhs.size() >= 1);
    pascal_lex_error
    (
        rhs.get(0)->get_location(),
        "left hand side of function call (%s) not appropriate",
        get_description().c_str()
    );
    return expression_error::create(get_location());
}


expression::pointer
expression::dot_expression_factory(const location &locn,
    const symbol::pointer &) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", typeid(*this).name());
    if (is_lvalue())
    {
        type_record::pointer trp =
            boost::dynamic_pointer_cast<type_record>(get_type());
        if (trp)
        {
            pascal_lex_error
            (
                locn,
                "the %s class has problem with its dot_expression_factory "
                    "method (you have found a bug)",
                typeid(*this).name()
            );
            return expression_error::create(get_location());
        }
    }

    //
    // Most of these errors will already have been caught by the general
    // error checking and symbol lookup done by the translator base
    // class.
    //
    pascal_lex_error
    (
        get_location(),
        "the left hand side of dot the (.) operator should be a record "
            "variable, not a %s",
        get_description().c_str()
    );
    return expression_error::create(get_location());
}


expression::pointer
expression::array_index_expression_factory(const expression::pointer &)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", typeid(*this).name());
    pascal_lex_error
    (
        get_location(),
        "left hand side of array index not appropriate"
    );
    return expression_error::create(get_location());
}


expression::side_effect_t
operator+(expression::side_effect_t lhs, expression::side_effect_t rhs)
{
    //
    // The logic here needs some explanation.
    //
    // When two expressions are combined, the complete expression only
    // has no side effect if neither parameter has a side effect.
    //
    // If either expression has any side effect, the combined expression
    // has a partial side effect.
    //
    // Expressions with a direct side effect will not need to use this
    // operator, and thus there is no case where side_effect_yes is
    // produced.
    //
    switch (lhs)
    {
    case expression::side_effect_no:
        switch (rhs)
        {
        case expression::side_effect_no:
            return expression::side_effect_no;

        case expression::side_effect_partial:
        case expression::side_effect_yes:
            break;
        }
        break;

    case expression::side_effect_partial:
    case expression::side_effect_yes:
        break;
    }
    return expression::side_effect_partial;
}


bool
expression::is_constant()
    const
{
    DEBUG(1, "false");
    return false;
}


void
expression::logical_traversal(int pprec, label::pointer &true_branch,
    label::pointer &false_branch, bool fall_through_preference) const
{
    traversal(pprec);
    if (fall_through_preference)
        false_branch->branch_from_here_if_false();
    else
        true_branch->branch_from_here_if_true();
}


bool
expression::execution_flows_out()
    const
{
    return true;
}


expression::pointer
expression::strip_indirection()
    const
{
    return expression::pointer();
}


symbol::pointer
expression::get_symbol_hint()
    const
{
    return symbol::pointer();
}


bool
expression::is_error()
    const
{
    return type_error::is_a(get_type());
}


bool
expression::is_simple_with_stmt(void)
    const
{
    return false;
}


rcstring
expression::get_description(void)
    const
{
    rcstring vov(is_lvalue() ? "variable" : "value");
    vov = get_type()->get_name() + " " + vov;
    symbol::pointer sp = get_symbol_hint();
    if (sp)
        vov = vov + " " + sp->get_name().quote_c();
    return vov;
}


bool
expression::is_empty_set(void)
    const
{
    return false;
}


expression::pointer
expression::get_byte_pointer(void)
    const
{
    return expression::pointer();
}


void
expression::comparison_always_same_due_to_limited_range_of_data_type(bool value)
    const
{
    pascal_lex_warning
    (
        get_location(),
        "comparison always %s due to limited range of data type",
        (value ? "true" : "false")
    );
}


expression::pointer
expression::ord_odd_hack(void)
    const
{
    return expression::pointer();
}


rcstring
expression::get_printable_value(void)
    const
{
    const char *value = typeid(*this).name();
    return value;
}


// vim: set ts=8 sw=4 et :

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

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type.h>


expression::pointer
translator::char_le_char_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return integer_le_integer_expression_factory(lhs, rhs);
}


expression::pointer
translator::char_le_string_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    expression::pointer rhs2 = char_from_string_of_length_one(rhs);
    if (!rhs2)
    {
        return op_le.complain(lhs, rhs);
    }

    return char_le_char_expression(lhs, rhs2);
}


expression::pointer
translator::string_le_char_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    expression::pointer lhs2 = char_from_string_of_length_one(lhs);
    if (!lhs2)
    {
        return op_le.complain(lhs, rhs);
    }

    return char_le_char_expression(lhs2, rhs);
}


expression::pointer
translator::set_le_set_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    if (lhs->get_type() != rhs->get_type())
    {
        return op_le.complain(lhs, rhs);
    }

    return set_le_set_expression_factory(lhs, rhs);
}


expression::pointer
translator::array_le_array_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    if (lhs->get_type() != rhs->get_type())
    {
        return op_le.complain(lhs, rhs);
    }

    return array_le_array_expression_factory(lhs, rhs);
}


expression::pointer
translator::array_le_string_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    expression::pointer rhs2 = optional_paoc_from_string(tap, rhs);
    assert(rhs2);

    return array_le_array_expression_factory(lhs, rhs2);
}


expression::pointer
translator::string_le_array_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(rhs->get_type());
    assert(tap);
    expression::pointer lhs2 = optional_paoc_from_string(tap, lhs);
    assert(lhs2);

    return array_le_array_expression_factory(lhs2, rhs);
}


expression::pointer
translator::enum_le_enum_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    //
    // It is possible we have been given enum values from two completely
    // different enumerated types.  This isn't detected by the
    // dispatcher (it makes the dispatcher code very ugly to do this
    // test in the dispatcher).
    //
    if (lhs->get_type() != rhs->get_type())
    {
        return op_le.complain(lhs, rhs);
    }

    return integer_le_integer_expression_factory(lhs, rhs);
}


expression::pointer
translator::integer_le_real_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        real_le_real_expression_factory
        (
            real_from_integer_expression_factory(lhs),
            rhs
        );
}


expression::pointer
translator::real_le_integer_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        real_le_real_expression_factory
        (
            lhs,
            real_from_integer_expression_factory(rhs)
        );
}


expression::pointer
translator::le_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    expression::pointer e1 = call_noparam_func_if_necessary(lhs);
    expression::pointer e2 = call_noparam_func_if_necessary(rhs);
    return op_le.dispatch(e1, e2);
}


expression::pointer
translator::lint_le_int_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        lint_le_lint_expression_factory
        (
            lhs,
            lint_from_integer_expression_factory(rhs)
        );
}


expression::pointer
translator::int_le_lint_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        lint_le_lint_expression_factory
        (
            lint_from_integer_expression_factory(lhs),
            rhs
        );
}


// vim: set ts=8 sw=4 et :

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
#include <lib/type/array.h>
#include <lib/type/string.h>


expression::pointer
translator::char_eq_char_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return integer_eq_integer_expression_factory(lhs, rhs);
}


expression::pointer
translator::char_eq_string_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    expression::pointer rhs2 = char_from_string_of_length_one(rhs);
    if (!rhs2)
    {
        return op_eq.complain(lhs, rhs);
    }

    return char_eq_char_expression(lhs, rhs2);
}


expression::pointer
translator::string_eq_char_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    expression::pointer lhs2 = char_from_string_of_length_one(lhs);
    if (!lhs2)
    {
        return op_eq.complain(lhs, rhs);
    }

    return char_eq_char_expression(lhs2, rhs);
}


expression::pointer
translator::set_eq_set_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    if (lhs->get_type() != rhs->get_type())
    {
        return op_eq.complain(lhs, rhs);
    }

    return set_eq_set_expression_factory(lhs, rhs);
}


expression::pointer
translator::record_eq_record_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;
    if (lhs->get_type() != rhs->get_type())
    {
        return op_eq.complain(lhs, rhs);
    }

    return record_eq_record_expression_factory(lhs, rhs);
}


expression::pointer
translator::array_eq_array_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    if (lhs->is_error())
        return lhs;
    if (rhs->is_error())
        return rhs;
    if (lhs->get_type() != rhs->get_type())
    {
        return op_eq.complain(lhs, rhs);
    }

    return array_eq_array_expression_factory(lhs, rhs);
}


expression::pointer
translator::optional_paoc_from_string(const type_array::pointer &tap,
    const expression::pointer &ep)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    // If the left hand side is not a packed array of char, simply
    // return the expression ep.  This will result in an error being
    // printed by the array_eq_array method.
    if (!tap->is_packed_array_of_char())
    {
        DEBUG(1, "}");
        return ep;
    }

    if (ep->is_error())
    {
        DEBUG(1, "}");
        return ep;
    }
    DEBUG(2, "%s", ep->lisp_representation().c_str());
    assert(type_string::is_a(ep->get_type()));

    if (!ep->is_constant())
    {
        pascal_lex_error
        (
            ep->get_location(),
            "only constant strings can be converted to packed array of char"
        );
        DEBUG(1, "}");
        return expression_error::create(ep->get_location());
    }
    rcstring text = ep->optimize()->get_string_value();
    if (tap->get_width() != text.size())
    {
        pascal_lex_error
        (
            ep->get_location(),
            "the string constant must be of length %u, not %lu",
            tap->get_width(),
            (unsigned long)text.size()
        );
        DEBUG(1, "}");
        return expression_error::create(ep->get_location());
    }

    expression::pointer result = paoc_from_string_expression_factory(ep, tap);
    DEBUG(2, "return %s", result->lisp_representation().c_str());
    DEBUG(1, "}");
    return result;
}


expression::pointer
translator::array_eq_string_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    expression::pointer rhs2 = optional_paoc_from_string(tap, rhs);
    assert(rhs2);

    return array_eq_array_expression_factory(lhs, rhs2);
}


expression::pointer
translator::string_eq_array_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(rhs->get_type());
    assert(tap);
    expression::pointer lhs2 = optional_paoc_from_string(tap, lhs);
    assert(lhs2);

    return array_eq_array_expression_factory(lhs2, rhs);
}


expression::pointer
translator::pointer_eq_pointer_expression(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    if (lhs->get_type() != rhs->get_type())
    {
        return op_eq.complain(lhs, rhs);
    }

    return pointer_eq_pointer_expression_factory(lhs, rhs);
}


expression::pointer
translator::enum_eq_enum_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    //
    // It is possible we have been given enum values from two completely
    // different enumerated types.  This isn't detected by the
    // dispatcher (it makes the dispatcher code very ugly to do this
    // test in the dispatcher).
    //
    if (lhs->get_type() != rhs->get_type())
    {
        return op_eq.complain(lhs, rhs);
    }

    return integer_eq_integer_expression_factory(lhs, rhs);
}


expression::pointer
translator::integer_eq_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return
        real_eq_real_expression_factory
        (
            real_from_integer_expression_factory(lhs),
            rhs
        );
}


expression::pointer
translator::real_eq_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return
        real_eq_real_expression_factory
        (
            lhs,
            real_from_integer_expression_factory(rhs)
        );
}


expression::pointer
translator::eq_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    expression::pointer e1 = call_noparam_func_if_necessary(lhs);
    expression::pointer e2 = call_noparam_func_if_necessary(rhs);
    return op_eq.dispatch(e1, e2);
}


expression::pointer
translator::int_eq_lint_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        lint_eq_lint_expression_factory
        (
            lint_from_integer_expression_factory(lhs),
            rhs
        );
}


expression::pointer
translator::lint_eq_int_expression(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return
        lint_eq_lint_expression_factory
        (
            lhs,
            lint_from_integer_expression_factory(rhs)
        );
}


// vim: set ts=8 sw=4 et :

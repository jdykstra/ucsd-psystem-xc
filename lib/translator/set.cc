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
#include <lib/type/anything.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/set/unsized.h>
#include <lib/type/string.h>


expression::pointer
translator::empty_set_expression()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    type::pointer tp = type_set_unsized::create(type_anything::create());
    return empty_set_expression_factory(pascal_lex_location(), tp);
}


expression::pointer
translator::set_expression(const expression::pointer &ep0)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression::pointer ep = call_noparam_func_if_necessary(ep0);
    if (ep->is_error())
        return ep;

    // A char constant is valid for a CASE statement,
    // a string constant is not.
    {
        expression::pointer ep2 = char_from_string_of_length_one(ep);
        if (ep2)
            ep = ep2;
    }

    if
    (
        !type_boolean::is_a(ep->get_type())
    &&
        !type_integer::is_a(ep->get_type())
    &&
        !type_enumerated::is_a(ep->get_type())
    &&
        !type_char::is_a(ep->get_type())
    )
    {
        pascal_lex_error
        (
            ep->get_location(),
            "set cannot be of %s type",
            ep->get_type()->get_name().c_str()
        );
        return expression_error::create(ep->get_location());
    }

    //
    // Looks safe enough...
    //
    return set_expression_factory(ep);
}


expression::pointer
translator::set_expression(const expression::pointer &lo,
    const expression::pointer &hi)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression::pointer lhs = call_noparam_func_if_necessary(lo);
    expression::pointer rhs = call_noparam_func_if_necessary(hi);

    // A char constant is valid for a CASE statement,
    // a string constant is not.
    {
        expression::pointer lhs2 = char_from_string_of_length_one(lhs);
        if (lhs2)
            lhs = lhs2;
    }
    {
        expression::pointer rhs2 = char_from_string_of_length_one(rhs);
        if (rhs2)
            rhs = rhs2;
    }

    if
    (
        !type_boolean::is_a(lhs->get_type())
    &&
        !type_integer::is_a(lhs->get_type())
    &&
        !type_enumerated::is_a(lhs->get_type())
    &&
        !type_char::is_a(lhs->get_type())
    )
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "set subranges cannot be of %s type",
            lhs->get_type()->get_name().c_str()
        );
        return expression_error::create(lhs->get_location());
    }

    if (lhs->get_type() != rhs->get_type())
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "set subrange limits must be the same type, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(lhs->get_location());
    }

    //
    // Looks safe enough...
    //
    return set_expression_factory(lhs, rhs);
}


expression::pointer
translator::in_expression(const expression::pointer &lhs0,
    const expression::pointer &rhs0)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    expression::pointer rhs = call_noparam_func_if_necessary(rhs0);
    if (rhs->is_error())
        return rhs;
    if (!type_set::is_a(rhs))
    {
        DEBUG(2, "rhs not set");
        pascal_lex_error
        (
            rhs->get_location(),
            "the right hand side of the IN operator must be a value of "
                "SET type, not a %s",
            rhs->get_description().c_str()
        );
        DEBUG(1, "}");
        return expression_error::create(rhs->get_location());
    }
    DEBUG(2, "rhs ok");

    expression::pointer lhs = call_noparam_func_if_necessary(lhs0);
    if (lhs->is_error())
        return lhs;
    if (lhs->get_type() != rhs->get_type()->get_subtype())
    {
        DEBUG(2, "lhs not correct type");
        pascal_lex_error
        (
            lhs->get_location(),
            "the left hand side of the IN operator must be a value of "
                "%s type, not a %s",
            rhs->get_type()->get_subtype()->get_name().c_str(),
            lhs->get_description().c_str()
        );
        DEBUG(1, "}");
        return expression_error::create(rhs->get_location());
    }
    DEBUG(2, "lhs ok");

    //
    // Looks safe enough...
    //
    expression::pointer result = in_expression_factory(lhs, rhs);
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :

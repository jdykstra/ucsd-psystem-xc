//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>


expression::pointer
translator::case_stack_push(const expression::pointer &ep0)
{
    expression::pointer ep = call_noparam_func_if_necessary(ep0);

    // A char constant is more likely than string, because you can never
    // dispatch based on a string value.
    {
        expression::pointer ep2 = char_from_string_of_length_one(ep);
        if (ep2)
            ep = ep2;
    }

    // FIXME: subranges
    if
    (
        !type_boolean::is_a(ep->get_type())
    &&
        !type_char::is_a(ep->get_type())
    &&
        !type_enumerated::is_a(ep->get_type())
    &&
        !type_integer::is_a(ep->get_type())
    )
    {
        pascal_lex_error
        (
            ep->get_location(),
            "case control expression must be of a countable type, "
                "not a %s",
            ep->get_description().c_str()
        );
        case_stack.push_back(expression_error::create(ep->get_location()));
    }
    else
    {
        if (ep->is_constant() && warning_constant_branch())
        {
            expression::pointer ep2 = ep->optimize();
            long n = ep2->optimize()->get_integer_value();
            rcstring ns =
                ep2->get_type()->get_human_readable_representation_of_value(n);
            pascal_lex_warning
            (
                ep->get_location(),
                "the case statement control expression is constant, "
                    "is is always %s",
                ns.c_str()
            );
        }
        // FIXME: nuke all the other branches if it is constant
        case_stack.push_back(ep);
    }
    return ep;
}


void
translator::case_stack_pop()
{
    // FIXME: if the type is not a simple integer, warn when values are missing
    // e.g. char, or subrange integer, or enumerated constants
    case_stack.pop_back();
}


type::pointer
translator::case_stack_type()
    const
{
    assert(!case_stack.empty());
    return case_stack.back()->get_type();
}


bool
translator::case_stack_check(const expression::pointer &ep)
    const
{
    assert(!case_stack.empty());
    if (!ep->is_constant())
    {
        DEBUG(0, "%s", typeid(*ep).name());
        pascal_lex_error
        (
            ep->get_location(),
            "case value is not constant"
        );
        return false;
    }
    if (case_stack.back()->is_error())
        return false;
    if (case_stack.back()->get_type() != ep->get_type())
    {
        pascal_lex_error
        (
            ep->get_location(),
            "case value type mismatch, expected a %s, not a %s",
            case_stack.back()->get_type()->get_name().c_str(),
            ep->get_description().c_str()
        );
        return false;
    }

    // looks OK so far
    return true;
}


// vim: set ts=8 sw=4 et :

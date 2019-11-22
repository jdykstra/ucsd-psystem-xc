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

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/expression/string.h>
#include <lib/pascal/lex.h>
#include <lib/symbol.h>
#include <lib/translator.h>
#include <lib/type.h>
#include <lib/type/anything.h>
#include <lib/type/array.h>
#include <lib/type/char.h>
#include <lib/type/integer.h>
#include <lib/type/function.h>
#include <lib/type/long_integer/sized.h>
#include <lib/type/real.h>
#include <lib/type/set.h>
#include <lib/type/string.h>
#include <lib/type/subrange.h>


expression::pointer
translator::assignment_expression_factory(const expression::pointer &lhs,
    const expression::pointer &a_rhs)
{
    //
    // Do nothing if one of them is a previously reported error.
    //
    if (lhs->is_error())
    {
        return lhs;
    }
    expression::pointer rhs = a_rhs;
    if (rhs->is_error())
    {
        return rhs;
    }

    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());

    //
    // Warning: Fugly Pascal-ism: function returns are achieved by
    // assigning to an implicit variable with the same name as the
    // function.  Ugh.
    //
    type::pointer left_type = lhs->get_type();
    if (type_function::is_a(left_type))
        left_type = left_type->get_subtype();

    //
    // Cope with function calls on the right.
    //
    rhs = call_noparam_func_if_necessary(rhs);

    //
    // Quietly promote integers to long integers when needed.
    //
    if (type_long_integer::is_a(left_type) && type_integer::is_a(rhs))
    {
        rhs = lint_from_integer_expression_factory(rhs);
    }

    //
    // Not so quietly truncate long integers to integers when needed.
    // Because the UCSD compiler required this to be explicit, the cross
    // compiler will issue a warning... in the common code.
    //
    if (type_integer::is_a(left_type) && type_long_integer::is_a(rhs))
    {
        rhs = integer_from_lint_expression(rhs);
    }

    //
    // Quietly adjust the size of long integers.
    //
    if (type_long_integer::is_a(left_type) && type_long_integer::is_a(rhs))
    {
        rhs = lint2lint_expression_factory(left_type, rhs);
    }

    //
    // Cope with subrange types on the right.
    //
    type::pointer right_type = rhs->get_type();
    if (type_subrange::is_a(right_type))
        right_type = right_type->get_subtype();

    //
    // Quietly promote integers to reals when needed.
    //
    if (type_real::is_a(left_type) && type_integer::is_a(right_type))
    {
        rhs = real_from_integer_expression_factory(rhs);
        right_type = rhs->get_type();
    }

    //
    // Set assignments are complicated.  Not only can we assign
    // like-to-like, but we can cast up and down for unlike subranges of
    // like subrange subtypes.  Don't worry about the like-to-like case,
    // it falls out from the subrange cases.
    //
    // Plus, constant empty sets are set-of-void, and can be assigned to
    // any other kind of set.
    //
    if (type_set::is_a(left_type) && type_set::is_a(right_type))
    {
        if (type_anything::is_a(right_type->get_subtype()))
        {
            //
            // You can assign the empty set (set-of-anything) to any other
            // set type.  Build a new empty set expression node with the
            // correct type.
            //
            rhs = empty_set_expression_factory(rhs->get_location(), left_type);
            right_type = rhs->get_type();
        }

        //
        // Always convert.  The code generation step will do nothing if
        // at all possible, but it also takes care of sized vs unsized
        // issues, too.
        //
        rhs = set2set_expression_factory(left_type, rhs);
        right_type = rhs->get_type();
        assert(left_type == right_type);
    }

    //
    // It is possible to assign a string constant to a packed array of
    // char of identical size.
    //
    if (type_string::is_a(rhs->get_type()))
    {
        type_array::pointer tap =
            boost::dynamic_pointer_cast<type_array>(lhs->get_type());
        if (tap)
        {
            rhs = optional_paoc_from_string(tap, rhs);
            DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
            goto ok;
        }
    }

    //
    // It is possible to assign a string constant of length 1 to a
    // char variable.
    //
    if (type_char::is_a(left_type))
    {
        expression::pointer rhs2 = char_from_string_of_length_one(rhs);
        if (rhs2)
        {
            rhs = rhs2;
            right_type = rhs->get_type();
        }
    }

    //
    // Make sure the left hand side is of the same type as the right hand side.
    //
    if
    (
        left_type != right_type
    &&
        (
            !type_subrange::is_a(left_type)
        ||
            left_type->get_subtype() != right_type
        )
    )
    {
        pascal_lex_error
        (
            rhs->get_location(),
            "assignment type mismatch, "
                "unable to assign a %s...",
            rhs->get_description().c_str()
        );
        pascal_lex_error
        (
            lhs->get_location(),
            "... to a %s",
            lhs->get_description().c_str()
        );
        return
            expression_error::create(lhs->get_location() + rhs->get_location());
    }

    //
    // Ask the left hand side to turn itself into an assignment.  We
    // don't go via a virtual method of the translator base class
    // because the assignment will be specific to the left hand side
    // expression, we go via the lhs virtual method.  And the left hand
    // side was manufactured from the translator derived class, so there
    // is ample opportunity for the translator derived classes to seize
    // control, if they need to.
    //
    ok:
    return lhs->assignment_expression_factory(rhs);
}


expression::pointer
translator::integer_from_lint_expression(const expression::pointer &ep)
{
    pascal_lex_warning
    (
        ep->get_location(),
        "you must use the \"trunc\" function when converting a %s to an "
            "integer",
        ep->get_description().c_str()
    );
    type::pointer tp = type_long_integer_sized::create();
    expression::pointer ep2 = lint2lint_expression_factory(tp, ep);
    return integer_from_lint_expression_factory(ep2);
}


// vim: set ts=8 sw=4 et :

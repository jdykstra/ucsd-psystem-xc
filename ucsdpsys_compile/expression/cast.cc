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
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/real.h>

#include <ucsdpsys_compile/expression/address.h>
#include <ucsdpsys_compile/expression/cast.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/real/compile.h>


expression_cast::~expression_cast()
{
}


expression_cast::expression_cast(
    const type::pointer &a_to,
    const expression::pointer &a_from,
    translator_compile *cntxt
) :
    expression(a_from->get_location()),
    code(*cntxt),
    to(a_to),
    from(a_from)
{
    assert(to->get_size_in_words() == from->get_type()->get_size_in_words());
}


expression_cast::pointer
expression_cast::create(const type::pointer &a_to,
    const expression::pointer &a_from, translator_compile *cntxt)
{
    return pointer(new expression_cast(a_to, a_from, cntxt));
}


void
expression_cast::traversal(int pprec)
    const
{
    from->traversal(pprec);
}


void
expression_cast::logical_traversal(int pprec, label::pointer &true_branch,
    label::pointer &false_branch, bool fall_thru_pref) const
{
    assert(type_boolean::is_a(to));
    if (type_boolean::is_a(from))
    {
        from->logical_traversal
        (
            pprec,
            true_branch,
            false_branch,
            fall_thru_pref
        );
    }
    else
    {
        // Do the same as the odd() function...
        //                                  ... i.e nothing.
        //
        // We do not actually do anything.  The boolean opcodes
        // (including the FJP opcode) simply look at the least
        // significant bit, which (surprize!) indicates whether or not
        // the integer value is odd.
        //
        if (fall_thru_pref)
            false_branch->branch_from_here_if_false();
        else
            true_branch->branch_from_here_if_true();
    }
}


int
expression_cast::get_precedence()
    const
{
    return from->get_precedence();
}


type::pointer
expression_cast::get_type()
    const
{
    return to;
}


bool
expression_cast::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_cast::has_side_effect()
    const
{
    return (side_effect_no + from->has_side_effect());
}


expression::pointer
expression_cast::clone(const location &locn)
    const
{
    return create(to, from->clone(locn), &code);
}


expression::pointer
expression_cast::optimize()
    const
{
    DEBUG(2, "from = %s", from->lisp_representation().c_str());
    expression::pointer e1 = from->optimize();
    DEBUG(2, "e1 = %s", e1->lisp_representation().c_str());
    if (e1->is_constant())
    {
        //
        // Oh, yuck, now we have N**2 possibilities.
        // Just deal with the simple ones.
        //
        if (to == e1->get_type())
            return e1;

        //
        // Cast from integer to real, usually implicit.
        //
        if (type_real::is_a(to) && type_integer::is_a(e1))
        {
            return
                expression_real_compile::create
                (
                    e1->get_location(),
                    e1->get_integer_value(),
                    &code
                );
        }

        //
        // So far, this is only for ord(x)
        //
        if (type_integer::is_a(to))
        {
            if (type_boolean::is_a(from))
            {
                return
                    expression_integer_compile::create
                    (
                        e1->get_location(),
                        e1->get_boolean_value(),
                        &code
                    );
            }
            if
            (
                type_char::is_a(from)
            ||
                type_enumerated::is_a(from)
            ||
                type_integer::is_a(from)
            )
            {
                return
                    expression_integer_compile::create
                    (
                        e1->get_location(),
                        e1->get_integer_value(),
                        &code
                    );
            }
        }
    }

    // look for addresses
    {
        expression_address::pointer eap =
            boost::dynamic_pointer_cast<expression_address>(e1);
        if (eap)
            return eap->cast(to);
    }

    return expression_cast::create(to, e1, &code);
}


rcstring
expression_cast::lisp_representation(void)
    const
{
    return
        (
            "(cast "
        +
            to->get_pascal_name()
        +
            " "
        +
            from->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :

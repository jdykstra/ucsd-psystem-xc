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
#include <lib/type/anything.h>
#include <lib/type/integer.h>
#include <lib/type/set/sized.h>
#include <lib/type/set/unsized.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/expression/set2set/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


void
translator_compile::fix_binary_set_operands(expression::pointer &lhs,
    expression::pointer &rhs)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "lhs = %s", lhs->lisp_representation().c_str());
    if (type_anything::is_a(lhs->get_type()->get_subtype()))
    {
        DEBUG(2, "lhs empty");
        lhs = lhs->optimize();
        // this only happens for the empty set.
        expression_set_compile::pointer escp =
            boost::dynamic_pointer_cast<expression_set_compile>(lhs);
        assert(escp);
        assert(escp->get_value().empty());

        type::pointer tp = rhs->get_type();
        if (boost::dynamic_pointer_cast<type_anything>(tp->get_subtype()))
        {
            tp =
                type_set_sized::create
                (
                    type_subrange::create(0, 0, type_integer::create())
                );
        }
        lhs =
            expression_set_compile::create
            (
                lhs->get_location(),
                tp,
                set(),
                this
            );
    }

    // make sure the set on the stack is sized (includes the size)
    {
        type_set_unsized::pointer tsup =
            boost::dynamic_pointer_cast<type_set_unsized>(lhs->get_type());
        if (tsup)
        {
            lhs =
                expression_set2set_compile::create
                (
                    tsup->get_sized_equiv(),
                    lhs,
                    this
                );
        }
    }

    DEBUG(2, "rhs = %s", rhs->lisp_representation().c_str());
    if (type_anything::is_a(rhs->get_type()->get_subtype()))
    {
        rhs = rhs->optimize();
        // this only happens for the empty set.
        expression_set_compile::pointer escp =
            boost::dynamic_pointer_cast<expression_set_compile>(rhs);
        assert(escp);
        assert(escp->get_value().empty());

        rhs =
            expression_set_compile::create
            (
                rhs->get_location(),
                lhs->get_type(),
                set(),
                this
            );
    }

    // make sure the set on the stack is sized (includes the size)
    {
        type_set_unsized::pointer tsup =
            boost::dynamic_pointer_cast<type_set_unsized>(rhs->get_type());
        if (tsup)
        {
            rhs =
                expression_set2set_compile::create
                (
                    tsup->get_sized_equiv(),
                    rhs,
                    this
                );
        }
    }

    DEBUG(1, "}");
}


// vim: set ts=8 sw=4 et :

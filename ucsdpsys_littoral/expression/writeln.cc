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

#include <lib/type/nothing.h>

#include <ucsdpsys_littoral/expression/writeln.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_writeln::~expression_writeln()
{
}


expression_writeln::expression_writeln(
    const expression::pointer &arg1,
    const expression::pointer &arg2,
    translator_littoral *arg3
) :
    expression(arg1->get_location()),
    lhs(arg1),
    rhs(arg2),
    littoral(*arg3)
{
}


expression_writeln::pointer
expression_writeln::create(const expression::pointer &arg1,
    const expression::pointer &arg2, translator_littoral *arg3)
{
    return pointer(new expression_writeln(arg1, arg2, arg3));
}


void
expression_writeln::traversal(int)
    const
{
    lhs->traversal(prec_assign);
    littoral.print_token(":");
    littoral.print_space();
    rhs->traversal(prec_assign);
}


void
expression_writeln::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    assert(false);
}


int
expression_writeln::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_writeln::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_writeln::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_writeln::has_side_effect()
    const
{
    return side_effect_yes;
}


expression::pointer
expression_writeln::optimize(void)
    const
{
    return create(lhs, rhs, &littoral);
}


expression::pointer
expression_writeln::clone(const location &locn)
    const
{
    return create(lhs->clone(locn), rhs->clone(locn), &littoral);
}


rcstring
expression_writeln::lisp_representation(void)
    const
{
    return
        (
            "(writeln "
        +
            lhs->lisp_representation()
        +
            " "
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :

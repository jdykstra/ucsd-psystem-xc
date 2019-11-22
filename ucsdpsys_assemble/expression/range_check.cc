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

#include <lib/cardinal.h>
#include <lib/type/integer.h>
#include <lib/expression/error.h>

#include <ucsdpsys_assemble/expression/integer/asm.h>
#include <ucsdpsys_assemble/expression/range_check.h>
#include <ucsdpsys_assemble/lex.h>


expression_range_check::~expression_range_check()
{
}


expression_range_check::expression_range_check(
    int a_pnum,
    const expression::pointer &a_operand,
    long a_minimum,
    long a_maximum
) :
    expression(a_operand->get_location()),
    parameter_number(a_pnum),
    operand(a_operand),
    minimum(a_minimum),
    maximum(a_maximum)
{
}


expression::pointer
expression_range_check::create(const expression::pointer &op, long min,
    long max)
{
    return create(1, op, min, max);
}


expression::pointer
expression_range_check::create(int pnum, const expression::pointer &op,
    long min, long max)
{
    if (op->is_error())
        return op;
    if (!type_integer::is_a(op))
    {
        lex_error
        (
            op->get_location(),
            "parameter %s: integer value required, not a %s",
            cardinal(pnum).c_str(),
            op->get_description().c_str()
        );
        return expression_error::create(op->get_location());
    }
    return pointer(new expression_range_check(pnum, op, min, max));
}


void
expression_range_check::traversal(int)
    const
{
    assert(false);
}


void
expression_range_check::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(false);
}


int
expression_range_check::get_precedence(void)
    const
{
    return 0;
}


type::pointer
expression_range_check::get_type(void)
    const
{
    return operand->get_type();
}


bool
expression_range_check::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_range_check::has_side_effect(void)
    const
{
    return side_effect_no;
}


expression::pointer
expression_range_check::optimize(void)
    const
{
    expression::pointer ep = operand->optimize();
    if (ep->is_error())
        return ep;
    assert(type_integer::is_a(ep));
    if (ep->is_constant())
    {
        long n = ep->get_integer_value();
        if (n < minimum || n > maximum)
        {
            lex_error
            (
                ep->get_location(),
                "parameter %s: expression value %ld not in the range %ld..%ld",
                cardinal(parameter_number).c_str(),
                n,
                minimum,
                maximum
            );
            n = (n < minimum ? minimum : maximum);
            ep = expression_integer_asm::create(n, get_location());
        }
        return ep;
    }
    return create(ep, minimum, maximum);
}


rcstring
expression_range_check::lisp_representation(void)
    const
{
    return
        (
            "(range-check "
        +
            operand->lisp_representation()
        +
            rcstring::printf(" %ld %ld)", minimum, maximum)
        );
}


expression::pointer
expression_range_check::clone(const location &locn)
    const
{
    return create(operand->clone(locn), minimum, maximum);
}


// vim: set ts=8 sw=4 et :

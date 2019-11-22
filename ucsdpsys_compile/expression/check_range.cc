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

#include <lib/pcode.h>
#include <lib/pascal/lex.h>
#include <ucsdpsys_compile/expression/check_range.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_check_range::~expression_check_range()
{
}


expression_check_range::expression_check_range(
    const expression::pointer &a_value,
    const expression::pointer &a_minimum,
    const expression::pointer &a_maximum,
    translator_compile *cntxt
) :
    expression(a_value->get_location()),
    code(*cntxt),
    value(a_value),
    minimum(a_minimum),
    maximum(a_maximum)
{
}


expression_check_range::pointer
expression_check_range::create(const expression::pointer &a_value,
    const expression::pointer &a_minimum, const expression::pointer &a_maximum,
    translator_compile *cntxt)
{
    return
        pointer
        (
            new expression_check_range(a_value, a_minimum, a_maximum, cntxt)
        );
}


void
expression_check_range::traversal(int pprec)
    const
{
    value->traversal(pprec);
    minimum->traversal(pprec);
    maximum->traversal(pprec);
    code.breakpoint(get_location());
    code.emit_byte(CHK);
}


expression::pointer
expression_check_range::optimize(void)
    const
{
    expression::pointer e1 = value->optimize();
    if (e1->is_constant())
    {
        assert(minimum->is_constant());
        assert(maximum->is_constant());
        int n = e1->get_integer_value();
        int lo = minimum->optimize()->get_integer_value();
        int hi = maximum->optimize()->get_integer_value();
        if (n < lo || n > hi)
        {
            type::pointer tp = e1->get_type();
            pascal_lex_error
            (
                e1->get_location(),
                "expression value %s is not in the range %s..%s",
                tp->get_human_readable_representation_of_value(n).c_str(),
                tp->get_human_readable_representation_of_value(lo).c_str(),
                tp->get_human_readable_representation_of_value(hi).c_str()
            );
        }
        return e1;
    }
    return expression_check_range::create(e1, minimum, maximum, &code);
}


expression::pointer
expression_check_range::clone(const location &locn)
    const
{
    return
        create
        (
            value->clone(locn),
            minimum->clone(locn),
            maximum->clone(locn),
            &code
        );
}


void
expression_check_range::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"shouldn't happen, not boolean");
}


int
expression_check_range::get_precedence()
    const
{
    return value->get_precedence();
}


type::pointer
expression_check_range::get_type()
    const
{
    return value->get_type();
}


bool
expression_check_range::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_check_range::has_side_effect()
    const
{
    return (side_effect_no + value->has_side_effect());
}


symbol::pointer
expression_check_range::get_symbol_hint()
    const
{
    return value->get_symbol_hint();
}


rcstring
expression_check_range::lisp_representation(void)
    const
{
    return
        (
            "(check-range "
        +
            value->lisp_representation()
        +
            " "
        +
            minimum->lisp_representation()
        +
            " "
        +
            maximum->lisp_representation()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :

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

#include <lib/pcode.h>
#include <lib/type/long_integer/sized.h>
#include <lib/type/long_integer/unsized.h>

#include <ucsdpsys_compile/expression/lint2lint/compile.h>
#include <ucsdpsys_compile/expression/long_integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_lint2lint_compile::~expression_lint2lint_compile()
{
}


expression_lint2lint_compile::expression_lint2lint_compile(
    const type::pointer &a_tp,
    const expression::pointer &a_from,
    translator_compile *a_code
) :
    expression_lint2lint(a_tp, a_from),
    code(*a_code)
{
    // for the DECOPS procedure
    code.need_segment(DECOPS_SEG_NUM);
}


expression::pointer
expression_lint2lint_compile::create(const type::pointer &a_tp,
    const expression::pointer &a_from, translator_compile *a_code)
{
    assert(type_long_integer::is_a(a_tp));
    assert(type_long_integer::is_a(a_from));
    if
    (
        type_long_integer_sized::is_a(a_tp)
    &&
        type_long_integer_sized::is_a(a_from->get_type())
    )
        return a_from;
    if
    (
        type_long_integer_unsized::is_a(a_tp)
    &&
        type_long_integer_unsized::is_a(a_from->get_type())
    )
    {
        if
        (
            a_tp->get_size_in_words()
        ==
            a_from->get_type()->get_size_in_words()
        )
            return a_from;

        // Pass it through an intermediate sized type
        expression::pointer e1 =
            create(type_long_integer_sized::create(), a_from, a_code);
        return create(a_tp, e1, a_code);
    }
    return pointer(new expression_lint2lint_compile(a_tp, a_from, a_code));
}


void
expression_lint2lint_compile::traversal(int)
    const
{
    get_from()->traversal(0);

    if (type_long_integer_sized::is_a(get_type()))
    {
        // something => sized
        if (type_long_integer_sized::is_a(get_from()->get_type()))
        {
            // sized => sized
            assert(!"should have been taken care of by create");
        }
        else
        {
            // unsized => sized
            code.breakpoint(get_location());
            code.emit_ldci(get_from()->get_type()->get_size_in_words());
        }
    }
    else
    {
        // something => unsized
        if (type_long_integer_sized::is_a(get_from()->get_type()))
        {
            // sized => unsigned
            code.breakpoint(get_location());
            code.emit_ldci(get_type()->get_size_in_words());
            code.emit_ldci(0);
            // call DECOPS
            code.emit_byte(CXP);
            code.emit_byte(DECOPS_SEG_NUM);
            code.emit_byte(DECOPS_PROC_NUM);
        }
        else
        {
            // unsized => unsized
            assert(!"should have been taken care of by create");
        }
    }
}


expression::pointer
expression_lint2lint_compile::optimize(void)
    const
{
    expression::pointer ep = get_from()->optimize();

    if
    (
        type_long_integer_sized::is_a(get_type())
    &&
        code.feature_long_integer_constants()
    )
    {
        expression_long_integer::pointer k1 =
            boost::dynamic_pointer_cast<expression_long_integer>(ep);
        if (k1)
        {
            return
                expression_long_integer_compile::create
                (
                    ep->get_location(),
                    get_type(),
                    k1->get_value(),
                    &code
                );
        }
    }

    return create(get_type(), ep, &code);
}


expression::pointer
expression_lint2lint_compile::clone(location const &locn)
    const
{
    return create(get_type(), get_from()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :

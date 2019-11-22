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

#include <lib/pascal/lex.h>

#include <ucsdpsys_compile/expression/ternary/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ternary_compile::~expression_ternary_compile()
{
}


expression_ternary_compile::expression_ternary_compile(
    const expression::pointer &a_cond,
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *a_code
) :
    expression_ternary(a_cond, a_lhs, a_rhs, a_code),
    code(*a_code)
{
}


expression_ternary_compile::pointer
expression_ternary_compile::create(const expression::pointer &a_cond,
    const expression::pointer &a_lhs, const expression::pointer &a_rhs,
    translator_compile *a_code)
{
    return
        pointer(new expression_ternary_compile(a_cond, a_lhs, a_rhs, a_code));
}


expression::pointer
expression_ternary_compile::optimize(void)
    const
{
    expression::pointer e1 = get_cond()->optimize();
    expression::pointer e2 = get_lhs()->optimize();
    expression::pointer e3 = get_rhs()->optimize();
    if (e1->is_constant())
    {
        bool c = e1->get_boolean_value();
        if (code.warning_constant_branch())
        {
            pascal_lex_warning
            (
                e1->get_location(),
                "ternary condition is constant, it is always %s",
                (c ? "true" : "false")
            );
        }
        return (c ? e2 : e3);
    }
    return create(e1, e2, e3, &code);
}


expression::pointer
expression_ternary_compile::clone(const location &locn)
    const
{
    return
        create
        (
            get_cond()->clone(locn),
            get_lhs()->clone(locn),
            get_rhs()->clone(locn),
            &code
        );

}


// vim: set ts=8 sw=4 et :

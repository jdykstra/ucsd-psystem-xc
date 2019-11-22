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

#include <lib/pcode.h>
#include <lib/label.h>
#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_boolean_compile::~expression_boolean_compile()
{
}


expression_boolean_compile::expression_boolean_compile(
    const location &where,
    bool a_value,
    translator_compile *cntxt
) :
    expression_boolean(where, a_value),
    code(*cntxt)
{
}


expression_boolean_compile::pointer
expression_boolean_compile::create(const location &where, bool a_value,
    translator_compile *cntxt)
{
    return pointer(new expression_boolean_compile(where, a_value, cntxt));
}


void
expression_boolean_compile::traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(get_value() ? SLDC_1 : SLDC_0);
}


void
expression_boolean_compile::logical_traversal(int,
    label::pointer &true_branch, label::pointer &false_branch,
    bool fall_through_preference) const
{
    if (get_value())
    {
        if (!fall_through_preference)
            true_branch->goto_from_here();
    }
    else
    {
        if (fall_through_preference)
            false_branch->goto_from_here();
    }
}


expression::pointer
expression_boolean_compile::optimize(void)
    const
{
    return create(get_location(), get_value(), &code);
}


expression::pointer
expression_boolean_compile::clone(const location &locn)
    const
{
    return create(locn, get_value(), &code);
}


// vim: set ts=8 sw=4 et :

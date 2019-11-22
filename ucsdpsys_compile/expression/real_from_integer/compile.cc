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
#include <lib/pcode.h>
#include <lib/type/integer.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/expression/real_from_integer/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_real_from_integer_compile::~expression_real_from_integer_compile()
{
}


expression_real_from_integer_compile::expression_real_from_integer_compile(
    const expression::pointer &a_arg,
    translator_compile *cntxt
) :
    expression_real_from_integer(a_arg),
    code(*cntxt)
{
}


expression_real_from_integer_compile::pointer
expression_real_from_integer_compile::create(const expression::pointer &a_arg,
    translator_compile *cntxt)
{
    return pointer(new expression_real_from_integer_compile(a_arg, cntxt));
}


void
expression_real_from_integer_compile::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(FLT);
}


expression::pointer
expression_real_from_integer_compile::optimize(void)
    const
{
    expression::pointer c1 = get_operand();
    DEBUG(2, "c1 = %s", c1->lisp_representation().c_str());
    expression::pointer e1 = c1->optimize();
    DEBUG(2, "e1 = %s", e1->lisp_representation().c_str());
    if (e1->is_constant())
    {
        assert(type_integer::is_a(e1));
        long n = e1->get_integer_value();
        double d = n;
        return expression_real_compile::create(get_location(), d, &code);
    }
    return expression_real_from_integer_compile::create(e1, &code);
}


expression::pointer
expression_real_from_integer_compile::clone(const location &locn)
    const
{
    return create(get_operand()->clone(locn), &code);
}


void
expression_real_from_integer_compile::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"no way this is bool");
}


// vim: set ts=8 sw=4 et :

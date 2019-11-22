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

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/set/unsized.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/expression/sgs/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_sgs_compile::~expression_sgs_compile()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_sgs_compile::expression_sgs_compile(
    const expression::pointer &a_value,
    translator_compile *cntxt
) :
    expression_sgs(a_value),
    code(*cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_sgs_compile::pointer
expression_sgs_compile::create(const expression::pointer &a_value,
    translator_compile *cntxt)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_sgs_compile(a_value, cntxt));
}


void
expression_sgs_compile::post_order_traversal(int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    code.breakpoint(get_location());
    code.emit_byte(SGS);
}


expression::pointer
expression_sgs_compile::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "type is %s", get_type()->get_pascal_name().c_str());
    expression::pointer ep = get_value()->optimize();
    if (ep->is_constant())
    {
        int n = ep->get_integer_value();
        if (n < 0 || n > set::range_maximum)
        {
            pascal_lex_error
            (
                ep->get_location(),
                "the value %d falls outside the available 0..%d range",
                n,
                set::range_maximum
            );
            return expression_error::create(get_location());
        }
        set s(n);
        type::pointer nt =
            expression_set_compile::minimum_subrange(get_type(), s);
        DEBUG(2, "nt = %s", nt->get_pascal_name().c_str());
        return expression_set_compile::create(get_location(), nt, s, &code);
    }
    return expression_sgs_compile::create(ep, &code);
}


expression::pointer
expression_sgs_compile::clone(const location &locn)
    const
{
    return create(get_value()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :

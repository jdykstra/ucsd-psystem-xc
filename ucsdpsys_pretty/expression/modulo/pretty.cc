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

#include <lib/ac/assert.h>

#include <ucsdpsys_pretty/expression/modulo/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_modulo_pretty::~expression_modulo_pretty()
{
}


expression_modulo_pretty::expression_modulo_pretty(
    const expression_modulo_pretty &arg
) :
    expression_modulo(arg),
    pretty(arg.pretty)
{
}


expression_modulo_pretty::expression_modulo_pretty(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_pretty *cntxt
) :
    expression_modulo(a_lhs, a_rhs),
    pretty(*cntxt)
{
}


expression_modulo_pretty::pointer
expression_modulo_pretty::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_pretty *cntxt)
{
    return pointer(new expression_modulo_pretty(a_lhs, a_rhs, cntxt));
}


void
expression_modulo_pretty::pre_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token("(");
}


void
expression_modulo_pretty::infix_order_traversal(int)
    const
{
    pretty.print_space();
    pretty.print_token("mod");
    pretty.print_space();
}


void
expression_modulo_pretty::post_order_traversal(int pprec)
    const
{
    if (need_parens(pprec))
        pretty.print_token(")");
}


void
expression_modulo_pretty::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    assert(0);
    traversal(pprec);
}


expression::pointer
expression_modulo_pretty::optimize(void)
    const
{
    return create(get_lhs(), get_rhs(), &pretty);
}


expression::pointer
expression_modulo_pretty::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &pretty);
}


// vim: set ts=8 sw=4 et :

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

#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/set.h>
#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/expression/srs/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_srs_compile::~expression_srs_compile()
{
}


expression_srs_compile::expression_srs_compile(
    const expression::pointer &a_lhs,
    const expression::pointer &a_rhs,
    translator_compile *cntxt
) :
    expression_srs(a_lhs, a_rhs),
    code(*cntxt)
{
}


expression_srs_compile::pointer
expression_srs_compile::create(const expression::pointer &a_lhs,
    const expression::pointer &a_rhs, translator_compile *cntxt)
{
    return pointer(new expression_srs_compile(a_lhs, a_rhs, cntxt));
}


void
expression_srs_compile::post_order_traversal(int)
    const
{
    code.breakpoint(get_location());
    code.emit_byte(SRS);
}


expression::pointer
expression_srs_compile::optimize(void)
    const
{
    expression::pointer e1 = get_lhs()->optimize();
    expression::pointer e2 = get_rhs()->optimize();
    assert(e1->get_type() == e2->get_type());

    if (e1->is_constant())
    {
        int n1 = e1->get_integer_value();
        if (n1 < 0 || n1 > set::range_maximum)
        {
            pascal_lex_error
            (
                e1->get_location(),
                "subrange start (%d) out of range [0..%d]",
                n1,
                set::range_maximum
            );
        }
    }

    if (e2->is_constant())
    {
        int n2 = e2->get_integer_value();
        if (n2 < 0 || n2 > set::range_maximum)
        {
            pascal_lex_error
            (
                e2->get_location(),
                "subrange finish (%d) out of range [0..%d]",
                n2,
                set::range_maximum
            );
        }
    }

    if (e1->is_constant() && e2->is_constant())
    {
        int n1 = e1->get_integer_value();
        int n2 = e2->get_integer_value();
            location locn = e1->get_location() + e2->get_location();
        set s;
        type::pointer h = e1->get_type();
        if (n1 > n2)
        {
            //
            // It seems like this should be an error, but runtime produces the
            // empty set and no error, so we just issue a warning.
            //
            pascal_lex_warning
            (
                locn,
                "subrange [%s..%s] backwards, result is the empty set",
                h->get_human_readable_representation_of_value(n1).c_str(),
                h->get_human_readable_representation_of_value(n2).c_str()
            );
        }
        else
        {
            s = set(n1, n2);
        }
        type::pointer nt =
            expression_set_compile::minimum_subrange(get_type(), s);
        return expression_set_compile::create(locn, nt, s, &code);
    }
    return create(e1, e2, &code);
}


expression::pointer
expression_srs_compile::clone(const location &locn)
    const
{
    return create(get_lhs()->clone(locn), get_rhs()->clone(locn), &code);
}


// vim: set ts=8 sw=4 et :

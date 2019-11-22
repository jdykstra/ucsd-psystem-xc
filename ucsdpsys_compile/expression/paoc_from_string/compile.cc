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
#include <lib/type/pointer/unpacked.h>

#include <ucsdpsys_compile/expression/lpa.h>
#include <ucsdpsys_compile/expression/paoc_from_string/compile.h>
#include <ucsdpsys_compile/expression/load/indirect.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_paoc_from_string_compile::~expression_paoc_from_string_compile()
{
}


expression_paoc_from_string_compile::expression_paoc_from_string_compile(
    const expression::pointer &a_rhs,
    const type::pointer &a_tp,
    translator_compile *a_code
) :
    expression_paoc_from_string(a_rhs, a_tp),
    code(*a_code)
{
}


expression_paoc_from_string_compile::pointer
expression_paoc_from_string_compile::create(const expression::pointer &a_rhs,
    const type::pointer &a_tp, translator_compile *a_code)
{
    return
        pointer(new expression_paoc_from_string_compile(a_rhs, a_tp, a_code));
}


void
expression_paoc_from_string_compile::traversal(int)
    const
{
    assert(!"should have been optimized away");
}


expression::pointer
expression_paoc_from_string_compile::optimize(void)
    const
{
    expression::pointer rhs2 = get_rhs()->optimize();
    rcstring text = rhs2->get_string_value();
    expression::pointer ep =
        expression_load_indirect::create
        (
            expression_lpa::create
            (
                text.c_str(),
                text.size(),
                get_location(),
                get_type(),
                &code
            ),
            &code
        );
    return ep->optimize();
}


expression::pointer
expression_paoc_from_string_compile::clone(const location &locn)
    const
{
    return create(get_rhs()->clone(locn), get_type(), &code);
}


// vim: set ts=8 sw=4 et :

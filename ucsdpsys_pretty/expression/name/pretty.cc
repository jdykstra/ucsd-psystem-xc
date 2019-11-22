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

#include <lib/symbol.h>
#include <lib/type/reference.h>
#include <ucsdpsys_pretty/expression/array_index.h>
#include <ucsdpsys_pretty/expression/assignment/pretty.h>
#include <ucsdpsys_pretty/expression/name/pretty.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_name_pretty::~expression_name_pretty()
{
}


static type::pointer
no_ref(const type::pointer tp)
{
    if (type_reference::is_a(tp))
        return tp->get_subtype();
    return tp;
}


expression_name_pretty::expression_name_pretty(
    const location &locn,
    const symbol::pointer &a_sp,
    translator_pretty *cntxt
) :
    expression_name(locn, a_sp),
    pretty(*cntxt)
{
}


expression_name_pretty::pointer
expression_name_pretty::create(const location &locn, const symbol::pointer &arg,
    translator_pretty *cntxt)
{
    return pointer(new expression_name_pretty(locn, arg, cntxt));
}


expression::pointer
expression_name_pretty::clone(void)
    const
{
    return clone(get_location());
}


type::pointer
expression_name_pretty::get_type(void)
    const
{
    return no_ref(get_sp()->get_type());
}


void
expression_name_pretty::traversal(int)
    const
{
    pretty.print_token(get_sp()->get_name());
}


expression::pointer
expression_name_pretty::assignment_expression_factory(
    const expression::pointer &rhs) const
{
    return expression_assignment_pretty::create(clone(), rhs, &pretty);
}


expression::pointer
expression_name_pretty::array_index_expression_factory(
    const expression::pointer &rhs) const
{
    return expression_array_index::create(clone(), rhs, &pretty);
}


void
expression_name_pretty::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


expression::pointer
expression_name_pretty::optimize(void)
    const
{
    return clone();
}


expression::pointer
expression_name_pretty::clone(const location &locn)
    const
{
    return create(locn, get_sp(), &pretty);
}


// vim: set ts=8 sw=4 et :

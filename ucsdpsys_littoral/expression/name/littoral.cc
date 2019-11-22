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
#include <lib/symbol/enumerated.h>
#include <lib/symbol/expression.h>
#include <lib/type/reference.h>

#include <ucsdpsys_littoral/expression/array_index.h>
#include <ucsdpsys_littoral/expression/assignment/littoral.h>
#include <ucsdpsys_littoral/expression/dot.h>
#include <ucsdpsys_littoral/expression/name/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_name_littoral::~expression_name_littoral()
{
}


static type::pointer
no_ref(const type::pointer tp)
{
    if (type_reference::is_a(tp))
        return tp->get_subtype();
    return tp;
}


expression_name_littoral::expression_name_littoral(
    const location &locn,
    const symbol::pointer &a_sp,
    translator_littoral *cntxt
) :
    expression_name(locn, a_sp),
    littoral(*cntxt)
{
}


expression_name_littoral::pointer
expression_name_littoral::create(const location &locn,
    const symbol::pointer &arg, translator_littoral *cntxt)
{
    return pointer(new expression_name_littoral(locn, arg, cntxt));
}


expression::pointer
expression_name_littoral::clone(void)
    const
{
    return create(get_location(), get_sp(), &littoral);
}


type::pointer
expression_name_littoral::get_type(void)
    const
{
    return no_ref(get_sp()->get_type());
}


void
expression_name_littoral::traversal(int)
    const
{
    littoral.print_token(get_sp()->get_name().downcase());
}


expression::pointer
expression_name_littoral::assignment_expression_factory(
    const expression::pointer &rhs) const
{
    return expression_assignment_littoral::create(clone(), rhs, &littoral);
}


expression::pointer
expression_name_littoral::array_index_expression_factory(
    const expression::pointer &rhs) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return expression_array_index::create(clone(), rhs, &littoral);
}


bool
expression_name_littoral::is_lvalue(void)
    const
{
    return true;
}


void
expression_name_littoral::logical_traversal(int pprec, label::pointer &,
    label::pointer &, bool) const
{
    traversal(pprec);
}


expression::pointer
expression_name_littoral::optimize(void)
    const
{
    return clone();
}


expression::pointer
expression_name_littoral::clone(const location &locn)
    const
{
    return create(locn, get_sp(), &littoral);
}


bool
expression_name_littoral::is_constant(void)
    const
{
    {
        symbol_expression::pointer sep =
            boost::dynamic_pointer_cast<symbol_expression>(get_sp());
        if (sep)
            return sep->get_value()->is_constant();
    }

    {
        symbol_enumerated::pointer sep =
            boost::dynamic_pointer_cast<symbol_enumerated>(get_sp());
        if (sep)
            return true;
    }

    return false;
}


long
expression_name_littoral::get_integer_value(void)
    const
{
    symbol_enumerated::pointer s2 =
        boost::dynamic_pointer_cast<symbol_enumerated>(get_sp());
    if (s2)
        return s2->get_value();

    symbol_expression::pointer sep =
        boost::dynamic_pointer_cast<symbol_expression>(get_sp());
    assert(sep);
    return sep->get_value()->optimize()->get_integer_value();
}


bool
expression_name_littoral::get_boolean_value(void)
    const
{
    symbol_expression::pointer sep =
        boost::dynamic_pointer_cast<symbol_expression>(get_sp());
    assert(sep);
    return sep->get_value()->optimize()->get_boolean_value();
}


expression::pointer
expression_name_littoral::dot_expression_factory(const location &,
    const symbol::pointer &rhs) const
{
    return expression_dot::create(clone(), rhs, &littoral);
}


bool
expression_name_littoral::is_simple_with_stmt(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :

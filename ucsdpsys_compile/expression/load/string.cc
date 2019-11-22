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
#include <lib/type/pointer.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/byte_pointer.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/ixs.h>
#include <ucsdpsys_compile/expression/ldb.h>
#include <ucsdpsys_compile/expression/load/string.h>
#include <ucsdpsys_compile/expression/sas.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_load_string::~expression_load_string()
{
}


expression_load_string::expression_load_string(
    const expression::pointer &a_operand,
    translator_compile *cntxt
) :
    expression_load(a_operand->get_location(),
                    a_operand->get_type()->get_subtype()),
    code(*cntxt),
    operand(a_operand)
{
}


expression_load_string::pointer
expression_load_string::create(const expression::pointer &a_operand,
    translator_compile *cntxt)
{
    return pointer(new expression_load_string(a_operand, cntxt));
}


void
expression_load_string::traversal(int)
    const
{
    operand->traversal(0);
}


void
expression_load_string::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"this should have been optimized away");
}


expression::pointer
expression_load_string::optimize(void)
    const
{
    expression::pointer ep = operand->optimize();
    return expression_load_string::create(ep, &code);
}


expression::pointer
expression_load_string::clone(const location &locn)
    const
{
    return create(operand->clone(locn), &code);
}


expression::pointer
expression_load_string::strip_indirection()
    const
{
    return operand;
}


expression::pointer
expression_load_string::assignment_expression_factory(
    const expression::pointer &rhs) const
{
    assert(type_string::is_a(get_type()));
    return expression_sas::create(operand, rhs, &code);
}


rcstring
expression_load_string::lisp_representation(void)
    const
{
    return
        (
            "(load-string {"
        +
            get_type()->get_pascal_name()
        +
            "} ["
        +
            get_location().representation()
        +
            "] "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


expression::pointer
expression_load_string::array_index_expression_factory(
    const expression::pointer &rhs) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    type_string::pointer tsp =
        boost::dynamic_pointer_cast<type_string>(get_type());
    assert(tsp);
    expression::pointer ep = expression_byte_pointer::create(operand, rhs);
    if (code.issue_range_check())
        ep = expression_ixs::create(ep, &code);
    return expression_ldb::create(ep, &code);
}


expression::pointer
expression_load_string::get_byte_pointer(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", lisp_representation().c_str());
    expression::pointer idx =
        expression_integer_compile::create(operand->get_location(), 0, &code);
    expression::pointer result = expression_byte_pointer::create(operand, idx);
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :

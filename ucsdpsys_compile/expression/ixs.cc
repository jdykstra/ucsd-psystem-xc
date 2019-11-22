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
#include <lib/type/char.h>
#include <lib/type/pointer/byte.h>

#include <ucsdpsys_compile/expression/ixs.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ixs::~expression_ixs()
{
}


expression_ixs::expression_ixs(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression(a_operand->get_location()),
    operand(a_operand),
    code(*a_code)
{
    assert(type_pointer_byte::is_a(operand->get_type()));
}


expression_ixs::pointer
expression_ixs::create(const expression::pointer &a_operand,
    translator_compile *a_code)
{
    return pointer(new expression_ixs(a_operand, a_code));
}


void
expression_ixs::traversal(int)
    const
{
    operand->traversal(0);

    //
    // "Index string array.  Index the byte pointer tos-1 by the integer
    // index tos, and push the resulting byte pointer if it is in the
    // range 1..current length.  If not, give an execution error."
    //
    // What it omits to mention is that tos and tos-1 are unchanged
    // after this opcode.  For (*$R-*) this opcode is simply omitted.
    //
    code.breakpoint(get_location());
    code.emit_byte(IXS);
}


expression::pointer
expression_ixs::optimize(void)
    const
{
    return create(operand->optimize(), &code);
}


expression::pointer
expression_ixs::clone(const location &locn)
    const
{
    return create(operand->clone(locn), &code);
}


void
expression_ixs::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"can never be boolean");
}


int
expression_ixs::get_precedence(void)
    const
{
    return operand->get_precedence();
}


type::pointer
expression_ixs::get_type(void)
    const
{
    return type_pointer_byte::create(type_char::create());
}


bool
expression_ixs::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_ixs::has_side_effect(void)
    const
{
    return side_effect_no;
}


rcstring
expression_ixs::lisp_representation(void)
    const
{
    return "(ixs " + operand->lisp_representation() + ")";
}


// vim: set ts=8 sw=4 et :

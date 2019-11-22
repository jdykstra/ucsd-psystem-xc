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

#include <lib/debug.h>
#include <lib/pcode.h>
#include <lib/type/char.h>
#include <lib/type/pointer.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/ldb.h>
#include <ucsdpsys_compile/expression/stb.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ldb::~expression_ldb()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_ldb::expression_ldb(
    const expression::pointer &a_operand,
    translator_compile *a_code
) :
    expression(a_operand->get_location()),
    operand(a_operand),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "operand = %s", operand->get_description().c_str());
    assert(type_pointer::is_a(operand));
}


expression_ldb::pointer
expression_ldb::create(const expression::pointer &a_operand,
    translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_ldb(a_operand, a_code));
}


void
expression_ldb::traversal(int)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    operand->traversal(0);

    //
    // "Load byte.  Index the byte pointer tos-1 by the integer index
    // tos, and push the byte (after zeroing high byte) pointed to by
    // the resulting byte pointer."
    //
    code.breakpoint(get_location());
    code.emit_byte(LDB);
    DEBUG(1, "}");
}


type::pointer
expression_ldb::get_type(void)
    const
{
    assert(type_pointer::is_a(operand->get_type()));
    if (type_string::is_a(operand->get_type()->get_subtype()))
        return type_char::create();
    return operand->get_type()->get_subtype();
}


void
expression_ldb::logical_traversal(int, label::pointer &, label::pointer &, bool)
    const
{
    assert(!"can't ever be boolean");
}


int
expression_ldb::get_precedence()
    const
{
    return prec_unary;
}


bool
expression_ldb::is_lvalue(void)
    const
{
    return true;
}


expression::side_effect_t
expression_ldb::has_side_effect()
    const
{
    return operand->has_side_effect();
}


expression::pointer
expression_ldb::optimize(void)
    const
{
    return create(operand->optimize(), &code);
}


expression::pointer
expression_ldb::clone(const location &locn)
    const
{
    return create(operand->clone(locn), &code);
}


rcstring
expression_ldb::lisp_representation(void)
    const
{
    return
        (
            "(ldb {"
        +
            get_type()->get_pascal_name()
        +
            "} "
        +
            operand->lisp_representation()
        +
            ")"
        );
}


expression::pointer
expression_ldb::assignment_expression_factory(
    const expression::pointer &value) const
{
    return expression_stb::create(operand, value, &code);
}


expression::pointer
expression_ldb::get_byte_pointer(void)
    const
{
    return operand;
}


// vim: set ts=8 sw=4 et :

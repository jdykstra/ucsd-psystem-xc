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

#include <lib/label.h>
#include <lib/pcode.h>
#include <lib/type/boolean.h>
#include <ucsdpsys_compile/expression/ldl.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_ldl::~expression_ldl()
{
}


expression_ldl::expression_ldl(const expression_ldl &arg) :
    expression(arg),
    code(arg.code),
    ptype(arg.ptype),
    offset(arg.offset)
{
}


expression_ldl::expression_ldl(
    const location &a_locn,
    const type::pointer &a_type,
    int a_offset,
    translator_compile *cntxt
) :
    expression(a_locn),
    code(*cntxt),
    ptype(a_type),
    offset(a_offset)
{
    assert(offset > 0);
    assert(offset < 32768);
}


expression_ldl::pointer
expression_ldl::create(const location &a_locn, const type::pointer &a_type,
    int a_offset, translator_compile *cntxt)
{
    return pointer(new expression_ldl(a_locn, a_type, a_offset, cntxt));
}


void
expression_ldl::traversal(int)
    const
{
    code.breakpoint(get_location());
    if (offset <= 16)
    {
        assert(offset > 0);
        code.emit_byte(SLDL_1 - 1 + offset);
    }
    else
    {
        code.emit_byte(LDL);
        code.emit_big(offset);
    }
}


expression::pointer
expression_ldl::optimize(void)
    const
{
    return create(get_location(), ptype, offset, &code);
}


expression::pointer
expression_ldl::clone(const location &locn)
    const
{
    return create(locn, ptype, offset, &code);
}


void
expression_ldl::logical_traversal(int pprec, label::pointer &true_label,
    label::pointer &false_label, bool fall_through_preference) const
{
    assert(type_boolean::is_a(get_type()));
    traversal(pprec);
    if (fall_through_preference)
    {
        false_label->branch_from_here_if_false();
    }
    else
    {
        true_label->branch_from_here_if_true();
    }
}


int
expression_ldl::get_precedence()
    const
{
    return prec_id;
}


type::pointer
expression_ldl::get_type()
    const
{
    return ptype;
}


bool
expression_ldl::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_ldl::has_side_effect()
    const
{
    return side_effect_no;
}


rcstring
expression_ldl::lisp_representation(void)
    const
{
    return
        (
            "(ldl {"
        +
            get_type()->get_pascal_name()
        +
            "} ["
        +
            get_location().representation()
        +
            "] "
        +
            rcstring::printf("%d", offset)
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :

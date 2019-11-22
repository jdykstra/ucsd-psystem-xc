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

#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/symbol.h>
#include <lib/type/function.h>
#include <lib/type/integer.h>
#include <ucsdpsys_compile/expression/load_address.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_load_address::~expression_load_address()
{
}


expression_load_address::expression_load_address(
    const location &locn,
    const symbol::pointer &a_sp,
    translator_compile *cntxt
) :
    expression(locn),
    code(*cntxt),
    sp(a_sp)
{
}


void
expression_load_address::traversal(int)
    const
{
    code.breakpoint(get_location());
    if (type_function::is_a(sp->get_type()))
    {
        // This is ugly.  See the comments in the
        // ucsdpsys_compile/expression/name/compile.cc file
        // (assignment_expression_factory) for the full rant.
        code.emit_byte(LLA);
        code.emit_big(0);
        return;
    }

    bit_address offset = sp->get_bit_offset();
    assert(offset.get_bit_number() == 0);
    assert(offset.get_word_addr() > 0);
    if (sp->get_lex_level() == 0)
    {
        // Global variable
        code.emit_byte(LAO);
        code.emit_big(offset.get_word_addr());
    }
    else if(sp->get_lex_level() == code.get_lex_level())
    {
        // Local variable
        code.emit_byte(LLA);
        code.emit_big(offset.get_word_addr());
    }
    else
    {
        // Intermediate variable
        code.emit_byte(LDA);
        code.emit_byte(code.get_lex_level() - sp->get_lex_level());
        code.emit_big(offset.get_word_addr());
    }
}


int
expression_load_address::get_precedence()
    const
{
    return 0;
}


type::pointer
expression_load_address::get_type()
    const
{
    // Not quite, but it's the right size.
    return type_integer::create();
}


bool
expression_load_address::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_load_address::has_side_effect()
    const
{
    return side_effect_no;
}


void
expression_load_address::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"addresses are not boolean");
}


rcstring
expression_load_address::lisp_representation(void)
    const
{
    return ("(load-address " + sp->get_name() + ")");
}


// vim: set ts=8 sw=4 et :

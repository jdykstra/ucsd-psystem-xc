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
#include <ucsdpsys_compile/expression/load/local.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_load_local::~expression_load_local()
{
}


expression_load_local::expression_load_local(
    const location &a_locn,
    const type::pointer &a_type,
    int a_offset,
    translator_compile *cntxt
) :
    expression_load(a_locn, a_type),
    code(*cntxt),
    offset(a_offset)
{
}


void
expression_load_local::traversal(int)
    const
{
    code.breakpoint(get_location());
    unsigned size = get_type()->get_size_in_bytes();
    assert(size >= 1);
    if (size == 1)
    {
        if (offset >= 1 && offset <= 16)
        {
            code.emit_byte(SLDL_1 - 1 + offset);
        }
        else
        {
            code.emit_byte(LDL);
            code.emit_big(offset);
        }
    }
    else
    {
        assert(size < 256);
        code.emit_byte(LLA);
        code.emit_big(offset);
        code.emit_byte(LDM);
        code.emit_byte(size);
    }
}


void
expression_load_local::logical_traversal(int pprec, label::pointer &true_label,
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


// vim: set ts=8 sw=4 et :

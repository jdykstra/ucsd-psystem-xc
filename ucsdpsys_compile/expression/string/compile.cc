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

#include <lib/pcode.h>
#include <lib/type/reference.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/string/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_string_compile::~expression_string_compile()
{
}


static size_t
plen(const rcstring &s)
{
    size_t len = s.size();
    if (len == 0)
        return 1;
    if (len >= 256)
        return 255;
    return len;
}


expression_string_compile::expression_string_compile(
    const location &locn,
    const rcstring &arg,
    const type::pointer &a_whatsit,
    translator_compile *a_code
) :
    expression_string(locn, arg),
    whatsit(a_whatsit),
    code(*a_code)
{
}


expression_string_compile::pointer
expression_string_compile::create(const location &locn, const rcstring &arg,
    const type::pointer &tp, translator_compile *a_code)
{
    return pointer(new expression_string_compile(locn, arg, tp, a_code));
}


expression_string_compile::pointer
expression_string_compile::create(const location &locn, const rcstring &arg,
    translator_compile *a_code)
{
    return create(locn, arg, type_string::create(plen(arg)), a_code);
}


expression_string_compile::pointer
expression_string_compile::create_ref(const location &locn, const rcstring &arg,
    translator_compile *a_code)
{
    return
        create
        (
            locn,
            arg,
            type_reference::create(type_string::create(plen(arg))),
            a_code
        );
}


void
expression_string_compile::traversal(int)
    const
{
    code.breakpoint(get_location());

    //
    // The actual data of the string constant must be on an even byte
    // boundary.  If necessary, insert a NOP opcode so that the LSA
    // opcode is at an odd address.
    //
    code.emit_odd_alignment(NOP);

    code.emit_byte(LSA);
    rcstring v = get_value();
    size_t len = v.size();
    if (len > 255)
    {
        code.error
        (
            get_location(),
            "string constant too long (by %d)",
            int(len - 255)
        );
        len = 255;
    }
    code.emit_byte(len);
    const char *cp = v.c_str();
    while (len > 0)
    {
        code.emit_byte(*cp++);
        --len;
    }
}


void
expression_string_compile::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"string constants are never bool");
}


expression::pointer
expression_string_compile::optimize(void)
    const
{
    return create(get_location(), get_value(), whatsit, &code);
}


expression::pointer
expression_string_compile::clone(const location &locn)
    const
{
    return create(locn, get_value(), whatsit, &code);
}


type::pointer
expression_string_compile::get_type()
    const
{
    return whatsit;
}


expression::pointer
expression_string_compile::strip_indirection()
    const
{
    if (!type_string::is_a(whatsit))
        return expression::pointer();
    return create_ref(get_location(), get_value(), &code);
}


// vim: set ts=8 sw=4 et :

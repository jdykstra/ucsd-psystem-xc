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
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/type/array.h>
#include <lib/type/char.h>
#include <lib/type/integer.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/char/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/load/indirect_packed.h>
#include <ucsdpsys_compile/expression/scan/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_scan_compile::~expression_scan_compile()
{
}


expression_scan_compile::expression_scan_compile(
    const expression::pointer &a_maxdisp,
    int a_mask,
    const expression::pointer &a_value,
    const expression::pointer &a_start,
    translator_compile *a_code
) :
    expression_scan(a_maxdisp, a_mask, a_value, a_start),
    code(*a_code)
{
}


expression::pointer
expression_scan_compile::create(const expression::pointer &a_maxdisp,
    int a_mask, const expression::pointer &a_value,
    const expression::pointer &a_start, translator_compile *a_code)
{
    //
    // Check the first parameter
    //
    if (!type_integer::is_a(a_maxdisp))
    {
        pascal_lex_error
        (
            a_maxdisp->get_location(),
            "call of function scan: parameter one: given a %s, "
                "but it should be of integer type",
            a_maxdisp->get_description().c_str()
        );
        return expression_error::create(a_maxdisp->get_location());
    }

    //
    // Check the second parameter.
    //
    expression::pointer value2 = a_value;
    if (!type_char::is_a(value2))
    {
        expression::pointer v2c =
            a_code->char_from_string_of_length_one(value2);
        if (v2c)
        {
            value2 = v2c;
        }
        else
        {
            pascal_lex_error
            (
                value2->get_location(),
                "call of function scan: parameter two: given a %s, "
                    "but it should be of char type",
                value2->get_description().c_str()
            );
            return expression_error::create(a_value->get_location());
        }
    }

    //
    // Check the third parameter.
    //
    // var a: packed array of char
    // or, it could be a[n]
    expression::pointer start2 = a_start->get_byte_pointer();
    if (!start2)
    {
        pascal_lex_error
        (
            start2->get_location(),
            "call of function scan: parameter three: must be a packed "
                "array of char, not a %s",
            start2->get_description().c_str()
        );
        DEBUG(1, "}");
        return expression_error::create(start2->get_location());
    }

    // FIXME: check that 0 <= index <= declared size
    // FIXME: check that 0 <= maxdisp+index <= declared size

    //
    // Looks OK.
    //
    return
        pointer
        (
            new expression_scan_compile
            (
                a_maxdisp,
                a_mask,
                value2,
                start2,
                a_code
            )
        );
}


void
expression_scan_compile::traversal(int)
    const
{
    get_maxdisp()->traversal(0);
    code.emit_ldci(get_mask());
    get_value()->traversal(0);
    get_start()->traversal(0);
    code.breakpoint(get_location());
    code.emit_ldci(0);
    code.emit_byte(CSP);
    code.emit_byte(CSP_SCAN);
}


expression::pointer
expression_scan_compile::optimize(void)
    const
{
    return
        pointer
        (
            new expression_scan_compile
            (
                get_maxdisp()->optimize(),
                get_mask(),
                get_value()->optimize(),
                get_start()->optimize(),
                &code
            )
        );
}


expression::pointer
expression_scan_compile::clone(const location &locn)
    const
{
    return
        pointer
        (
            new expression_scan_compile
            (
                get_maxdisp()->clone(locn),
                get_mask(),
                get_value()->clone(locn),
                get_start()->clone(locn),
                &code
            )
        );
}


// vim: set ts=8 sw=4 et :

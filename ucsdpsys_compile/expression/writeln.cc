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

#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/type/array.h>
#include <lib/type/char.h>
#include <lib/type/error.h>
#include <lib/type/nothing.h>
#include <lib/type/integer.h>
#include <lib/type/long_integer/unsized.h>
#include <lib/type/real.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/writeln.h>
#include <ucsdpsys_compile/expression/writeln/bytes.h>
#include <ucsdpsys_compile/expression/writeln/char.h>
#include <ucsdpsys_compile/expression/writeln/integer.h>
#include <ucsdpsys_compile/expression/writeln/long_integer.h>
#include <ucsdpsys_compile/expression/writeln/real.h>
#include <ucsdpsys_compile/expression/writeln/string.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_writeln::~expression_writeln()
{
}


expression_writeln::expression_writeln(
    const expression::pointer &a_value,
    const expression::pointer &a_width
) :
    expression(a_value->get_location()),
    value(a_value),
    width(a_width)
{
}


expression_writeln::expression_writeln(const expression_writeln &arg) :
    expression(arg),
    value(arg.value),
    width(arg.width)
{
}


expression::pointer
expression_writeln::create(const expression::pointer &a_value,
    const expression::pointer &a_width, translator_compile *a_code)
{
    expression::pointer value = a_code->call_noparam_func_if_necessary(a_value);
    expression::pointer width = a_code->call_noparam_func_if_necessary(a_width);

    //
    // Check that the width is an integer
    //
    if (!type_integer::is_a(width))
    {
        pascal_lex_error
        (
            width->get_location(),
            "the width must be an integer value, not a %s",
            width->get_description().c_str()
        );
        return expression_error::create(width->get_location());
    }

    //
    // Note: ISO 7185 Standard PASCAL can write BOOLEAN values as well,
    // but UCSD Pascal can not.
    //

    {
        type_array::pointer tap =
            boost::dynamic_pointer_cast<type_array>(value->get_type());
        if (tap && tap->is_packed_array_of_char())
        {
            // As usual, packed array of char is an exception.
            return
                expression_writeln_bytes::create
                (
                    value,
                    width,
                    tap->get_width(),
                    a_code
                );
        }
    }

    if (type_char::is_a(value))
    {
        return expression_writeln_char::create(value, width, a_code);
    }
    if (type_integer::is_a(value))
    {
        return expression_writeln_integer::create(value, width, a_code);
    }
    if (type_long_integer::is_a(value))
    {
        return expression_writeln_long_integer::create(value, width, a_code);
    }
    if (type_real::is_a(value))
    {
        return
            expression_writeln_real::create
            (
                value,
                width,
                expression_integer_compile::create
                (
                    width->get_location(),
                    0,
                    a_code
                ),
                a_code
            );
    }
    if (type_string::is_a(value))
    {
        return expression_writeln_string::create(value, width, a_code);
    }

    if (!type_error::is_a(value->get_type()))
    {
        // avoid secondary errors
        pascal_lex_error
        (
            value->get_location(),
            "unable to writeln a %s",
            value->get_description().c_str()
        );
    }
    return expression_error::create(value->get_location());
}


expression::pointer
expression_writeln::create(const expression::pointer &a_value,
    const expression::pointer &a_width, const expression::pointer &a_prec,
    translator_compile *a_code)
{
    expression::pointer value = a_code->call_noparam_func_if_necessary(a_value);
    expression::pointer width = a_code->call_noparam_func_if_necessary(a_width);
    expression::pointer prec = a_code->call_noparam_func_if_necessary(a_prec);

    if (!type_integer::is_a(width))
    {
        pascal_lex_error
        (
            width->get_location(),
            "the width must be of integer type, not a %s",
            width->get_description().c_str()
        );
        return expression_error::create(width->get_location());
    }
    if (!type_integer::is_a(prec))
    {
        pascal_lex_error
        (
            prec->get_location(),
            "the precision must be of integer type, not a %s",
            prec->get_description().c_str()
        );
        return expression_error::create(prec->get_location());
    }
    if (!type_real::is_a(value))
    {
        pascal_lex_error
        (
            value->get_location(),
            "unable to writeln a %s with a precision specification",
            value->get_description().c_str()
        );
        return expression_error::create(value->get_location());
    }
    return expression_writeln_real::create(value, width, prec, a_code);
}


void
expression_writeln::traversal(int)
    const
{
    assert(false);
}


void
expression_writeln::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    // The grammar should prevent this from ever being called.
    assert(!"grammar should prevent this");
}


int
expression_writeln::get_precedence()
    const
{
    return prec_assign;
}


type::pointer
expression_writeln::get_type()
    const
{
    return type_nothing::create();
}


bool
expression_writeln::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_writeln::has_side_effect()
    const
{
    return side_effect_yes;
}


// vim: set ts=8 sw=4 et :

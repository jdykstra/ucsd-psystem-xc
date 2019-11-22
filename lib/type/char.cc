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
#include <lib/ac/ctype.h>

#include <lib/expression.h>
#include <lib/type/char.h>
#include <lib/type/subrange.h>


type_char::~type_char()
{
}


type_char::type_char(bool a_interactive) :
    interactive(a_interactive)
{
}


type_char::pointer
type_char::create()
{
    static pointer tsp;
    if (!tsp)
        tsp = pointer(new type_char(false));
    return tsp;
}


type_char::pointer
type_char::create_interactive()
{
    return pointer(new type_char(true));
}


rcstring
type_char::get_name(void)
    const
{
    return "char";
}


rcstring
type_char::get_pascal_name(void)
    const
{
    return get_name();
}


rcstring
type_char::get_c_declaration(const rcstring &cname)
    const
{
    return ("unsigned char " + cname);
}


unsigned
type_char::get_size_in_bits(void)
    const
{
    return 8;
}


bool
type_char::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_char::is_a(const type::pointer &arg)
{
    type::pointer r1 = arg;
    for (;;)
    {
        type_subrange::pointer rr =
            boost::dynamic_pointer_cast<type_subrange>(r1);
        if (!rr)
            break;
        r1 = rr->get_subtype();
    }

    return boost::dynamic_pointer_cast<type_char>(r1);
}


bool
type_char::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    return is_a(rhs);
}


bool
type_char::is_a_variable(void)
    const
{
    return true;
}


type::pointer
type_char::get_subtype(void)
    const
{
    assert(0);
    return create();
}


int
type_char::get_minimum_value(void)
    const
{
    return 0;
}


int
type_char::get_maximum_value(void)
    const
{
    return 255;
}


bool
type_char::may_be_used_as_array_index(void)
    const
{
    return true;
}


int
type_char::get_finit_recwords(void)
    const
{
    // See system/system.b.text in the II.0
    // source for the FINIT implementation.
    return (interactive ? 0 : -2);
}


bool
type_char::need_late_binding(void)
    const
{
    return false;
}


rcstring
type_char::get_human_readable_representation_of_value(int n)
    const
{
    if (n == '\'')
        return "''''";
    if (n < 0 || n >= 256 || !isprint((unsigned char)n))
        return rcstring::printf("chr(%d)", n);
    return rcstring::printf("'%c'", n);
}


// vim: set ts=8 sw=4 et :

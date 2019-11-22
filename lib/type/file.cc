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

#include <lib/type/char.h>
#include <lib/type/file.h>
#include <lib/type/nothing.h>


type_file::~type_file()
{
}


type_file::type_file(const type::pointer &a_subtype) :
    subtype(a_subtype)
{
}


type_file::pointer
type_file::create(const type::pointer &a_subtype)
{
    return pointer(new type_file(a_subtype));
}


rcstring
type_file::get_name()
    const
{
    // Ignore the name hint.
    if (type_nothing::is_a(subtype))
        return "raw file";
    if (type_char::is_a(subtype))
        return "text";
    return "file of " + subtype->get_name();
}


rcstring
type_file::get_pascal_name()
    const
{
    if (type_nothing::is_a(subtype))
        return "file";
    if (type_char::is_a(subtype))
        return "text";
    return "file of " + subtype->get_pascal_name();
}


rcstring
type_file::get_c_declaration(const rcstring &vname)
    const
{
    return ("FILE *" + vname);
}


void
type_file::name_hint(const rcstring &suggestion)
{
    hint = suggestion;
}


type::pointer
type_file::get_subtype()
    const
{
    return subtype;
}


unsigned
type_file::get_size_in_bits()
    const
{
    if (type_nothing::is_a(subtype))
        return (nil_file_size_in_words * 16);
    return (file_size_in_words * 16 + subtype->get_size_in_bits());
}


bool
type_file::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_file::is_a(const type::pointer &arg)
{
    return is_a(arg.get());
}


bool
type_file::is_a(const type *arg)
{
    return (0 != dynamic_cast<const type_file *>(arg));
}


bool
type_file::same_type_as(const type::pointer &rhs)
    const
{
    // Don't worry about matching "anything" as well,
    // because the == operator took care of it.
    return (type_file::is_a(rhs) && subtype == rhs->get_subtype());
}


bool
type_file::is_a_variable()
    const
{
    return true;
}


type::pointer
type_file::late_binding_or_null()
    const
{
    return create(type::late_binding(subtype));
}


int
type_file::get_finit_recwords()
    const
{
    return subtype->get_finit_recwords();
}


bool
type_file::need_late_binding(void)
    const
{
    return subtype->need_late_binding();
}


rcstring
type_file::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :

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

#include <lib/translator.h>
#include <lib/pascal/lex.h>
#include <lib/type/anything.h>
#include <lib/type/forward.h>


type_forward::~type_forward()
{
}


type_forward::type_forward(const rcstring &a_name, translator *cntxt) :
    xlat(*cntxt),
    name(a_name)
{
}


type_forward::pointer
type_forward::create(const rcstring &a_name, translator *a_cntxt)
{
    return pointer(new type_forward(a_name, a_cntxt));
}


bool
type_forward::need_late_binding(void)
    const
{
    return true;
}


type::pointer
type_forward::late_binding_or_null(void)
    const
{
    return xlat.type_lookup(name, true);
}


rcstring
type_forward::get_name(void)
    const
{
    return "forward " + name;
}


rcstring
type_forward::get_c_declaration(const rcstring &vname)
    const
{
    return ("forward_t " + vname);
}


rcstring
type_forward::get_pascal_name(void)
    const
{
    return name;
}


bool
type_forward::is_func(void)
    const
{
    assert(false);
    return false;
}


type::pointer
type_forward::get_subtype(void)
    const
{
    assert(false);
    return type_anything::create();
}


unsigned
type_forward::get_size_in_words(void)
    const
{
    pascal_lex_error
    (
        pascal_lex_location(),
        "attempted to obtain word size of forward declared %s type",
        name.quote_c().c_str()
    );
    return 1;
}


unsigned
type_forward::get_size_in_bits(void)
    const
{
    pascal_lex_error
    (
        pascal_lex_location(),
        "attempted to obtain bit size of forward declared %s type",
        name.quote_c().c_str()
    );
    return 16;
}


bool
type_forward::is_an_implicit_reference(void)
    const
{
    assert(false);
    return false;
}


bool
type_forward::same_type_as(const type::pointer &)
    const
{
    return false;
}


void
type_forward::name_hint(const rcstring &)
{
    assert(false);
}


bool
type_forward::is_a_variable(void)
    const
{
    return false;
}


int
type_forward::get_minimum_value(void)
    const
{
    assert(false);
    return 0;
}


int
type_forward::get_maximum_value(void)
    const
{
    assert(false);
    return 0;
}


bool
type_forward::may_be_used_as_array_index(void)
    const
{
    return false;
}


rcstring
type_forward::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


bool
type_forward::is_a(const type::pointer &candidate)
{
    return !!dynamic_cast<const type_forward *>(candidate.get());
}


// vim: set ts=8 sw=4 et :

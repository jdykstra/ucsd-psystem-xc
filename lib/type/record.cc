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
#include <lib/ac/string.h>

#include <lib/anonymous_name.h>
#include <lib/pascal_id.h>
#include <lib/scope/record.h>
#include <lib/type/record.h>


type_record::~type_record()
{
}


type_record::type_record(const scope::pointer &a_sp) :
    sp(a_sp),
    name(anonymous_name())
{
}


type_record::pointer
type_record::create(const scope::pointer &a_sp)
{
    return pointer(new type_record(a_sp));
}


rcstring
type_record::get_name()
    const
{
    return ("record " + name);
}


rcstring
type_record::get_pascal_name()
    const
{
    return name;
}


rcstring
type_record::get_c_declaration(const rcstring &vname)
    const
{
    return (name.downcase() + " " + vname);
}


void
type_record::name_hint(const rcstring &suggestion)
{
    if (strchr(name.c_str(), ':'))
        name = suggestion;
}


unsigned
type_record::get_size_in_bits()
    const
{
    return sp->get_size_in_bits();
}


symbol::pointer
type_record::lookup(const rcstring &field_name)
    const
{
    return sp->lookup(pascal_id(field_name));
}


rcstring
type_record::lookup_fuzzy(const rcstring &field_name, double &best)
    const
{
    return sp->lookup_fuzzy(pascal_id(field_name), best);
}


symbol::pointer
type_record::lookup_fuzzy(const rcstring &field_name)
    const
{
    double junk;
    rcstring other = lookup_fuzzy(field_name, junk);
    if (other.empty())
        return symbol::pointer();
    return lookup(other);
}


bool
type_record::is_a_variable()
    const
{
    return true;
}


bool
type_record::is_a(const expression::pointer &arg)
{
    return is_a(arg->get_type());
}


bool
type_record::is_a(const type::pointer &arg)
{
    return (0 != dynamic_cast<const type_record *>(arg.get()));
}


bool
type_record::same_type_as(const type::pointer &rhs)
    const
{
    type_record::pointer rhs2 = boost::dynamic_pointer_cast<type_record>(rhs);
    return (rhs2 && name == rhs2->name);
}


type::pointer
type_record::get_subtype()
    const
{
    assert(!"should not be trying to get a record's subtype");
    return pointer();
}


bool
type_record::need_late_binding(void)
    const
{
    return sp->need_late_binding();
}


type::pointer
type_record::late_binding_or_null()
    const
{
    sp->late_binding();

    // We always return NULL because we never actually create a new
    // scope or a new record type.
    return pointer();
}


void
type_record::attach_variant(const variant::pointer &vp)
{
    assert(!recvar);
    assert(vp);
    if (!vp->terminal())
        recvar = vp;
}

void
type_record::symbol_table_walk(symbol_functor &f)
{
    sp->symbol_table_walk(f);
}


rcstring
type_record::get_human_readable_representation_of_value(int)
    const
{
    assert(!"surely you are joking Mr Feynman");
    return rcstring();
}


// vim: set ts=8 sw=4 et :

//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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
#include <lib/segment/builder/interface.h>


segment_builder_interface::~segment_builder_interface()
{
}


segment_builder_interface::segment_builder_interface(
    const rcstring &a_name,
    unsigned a_number
) :
    name(a_name),
    number(a_number),
    next_proc_num(2)
{
}


segment_builder_interface::pointer
segment_builder_interface::create(const rcstring &a_name, unsigned a_number)
{
    return pointer(new segment_builder_interface(a_name, a_number));
}


bool
segment_builder_interface::emit_byte(unsigned char)
{
    return true;
}


bool
segment_builder_interface::emit_word(unsigned)
{
    return true;
}


bool
segment_builder_interface::emit_big(unsigned)
{
    return true;
}


bool
segment_builder_interface::emit_word_alignment(unsigned char)
{
    return true;
}


bool
segment_builder_interface::emit_odd_alignment(unsigned char)
{
    return true;
}


int
segment_builder_interface::next_procedure_number(void)
{
    return next_proc_num++;
}


void
segment_builder_interface::procedure_parameter_size(unsigned)
{
}


void
segment_builder_interface::procedure_variable_size(unsigned)
{
}


void
segment_builder_interface::procedure_enter_ic()
{
}


void
segment_builder_interface::procedure_exit_ic()
{
}


void
segment_builder_interface::procedure_end(int, int,
    const code_source_map::pointer &, mtype_t)
{
}


void
segment_builder_interface::complete(void)
{
}


unsigned
segment_builder_interface::get_address()
    const
{
    return 0;
}


void
segment_builder_interface::poke(unsigned, unsigned char)
{
}


void
segment_builder_interface::poke_word(unsigned, unsigned)
{
}


int
segment_builder_interface::new_jump_table_entry(unsigned)
{
    return 0;
}


unsigned
segment_builder_interface::get_segment_number(void)
    const
{
    return number;
}


rcstring
segment_builder_interface::get_segment_name()
    const
{
    return name;
}


void
segment_builder_interface::unit_interface_text(const rcstring &)
{
    // ignore.
}


void
segment_builder_interface::segment_not_linked(const link_info::pointer &)
{
    // Ignore.
}


void
segment_builder_interface::link_globref_big(const rcstring &)
{
    // Ignore.
}


void
segment_builder_interface::link_publref_big(const rcstring &)
{
    // Ignore.
}


void
segment_builder_interface::link_privref_big(const rcstring &, int)
{
    // Ignore.
}


void
segment_builder_interface::reloc_type1_lcrel(unsigned)
{
    // Ignore.
}


void
segment_builder_interface::reloc_type2_ref(unsigned)
{
    // Ignore.
}


void
segment_builder_interface::reloc_type3_pubpriv(unsigned)
{
    // Ignore.
}


void
segment_builder_interface::link_publdef(const rcstring &, int)
{
    // Ignore.
    DEBUG(2, "%s\n", __PRETTY_FUNCTION__);
}


bool
segment_builder_interface::is_an_interface(void)
    const
{
    return true;
}


// vim: set ts=8 sw=4 et :

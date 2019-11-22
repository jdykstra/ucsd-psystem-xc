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
#include <lib/expression/integer.h>
#include <lib/type/integer.h>

#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/patch.h>
#include <ucsdpsys_assemble/type/segment_relative.h>


patch::~patch()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


patch::patch(
    unsigned a_address,
    kind_t a_kind,
    const expression::pointer &a_value
) :
    address(a_address),
    kind(a_kind),
    value(a_value)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "address = $%04X", address);
    DEBUG(2, "kind = %d", kind);
    DEBUG(2, "value = %s", value->lisp_representation().c_str());
}


patch::pointer
patch::create(unsigned a_address, kind_t a_kind,
    const expression::pointer &a_value)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new patch(a_address, a_kind, a_value));
}


bool
patch::apply(const segment_builder::pointer &sbp)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    expression::pointer ep = value->optimize();
    if (ep->is_error())
    {
        // Already complained about.
        // Returning true will suppress secondary errors.
        return true;
    }
    if (!type_integer::is_a(ep) && !type_segment_relative::is_a(ep))
        return false;
    long n = ep->get_integer_value();
    switch (kind)
    {
    case kind_byte:
        DEBUG(2, "sbp->poke($%04X, $%02X)", address, (unsigned char)n);
        sbp->poke(address, (unsigned char)n);
        break;

    case kind_big:
        sbp->poke(address, ((n >> 8) & 0x7F) | 0x80);
        sbp->poke(address + 1, n & 0xFF);
        break;

    case kind_word_le:
        sbp->poke(address, (unsigned char)n);
        sbp->poke(address + 1, (unsigned char)(n >> 8));
        break;

    case kind_word_be:
        sbp->poke(address, (unsigned char)(n >> 8));
        sbp->poke(address + 1, (unsigned char)n);
        break;
    }
    return true;
}


void
patch::error_too_complex(void)
    const
{
    lex_error(value->get_location(), "expression too complex for relocation");
}


rcstring
patch::representation(void)
    const
{
    rcstring_accumulator ac;
    ac.push_back("{ address = ");
    ac.printf("$%04X", address);
    ac.push_back(", kind = ");
    switch (kind)
    {
    case kind_byte:
        ac.push_back("byte");
        break;

    case kind_big:
        ac.push_back("big");
        break;

    case kind_word_le:
        ac.push_back("word_le");
        break;

    case kind_word_be:
        ac.push_back("word_be");
        break;
    }
    ac.push_back(", value = ");
    ac.push_back(value->lisp_representation());
    ac.push_back(" }");
    return ac.mkstr();
}


// vim: set ts=8 sw=4 et :

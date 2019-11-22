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
#include <lib/symbol/expression.h>

#include <ucsdpsys_assemble/expression/name/asm.h>


expression_name_asm::~expression_name_asm()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_name_asm::expression_name_asm(
    const location &locn,
    const symbol::pointer &a_sp
) :
    expression_name(locn, a_sp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_name_asm::pointer
expression_name_asm::create(const location &locn,
    const symbol::pointer &a_sp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_name_asm(locn, a_sp));
}


expression::pointer
expression_name_asm::optimize(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    symbol_expression::pointer sep =
        boost::dynamic_pointer_cast<symbol_expression>(get_sp());
    if (sep)
    {
        expression::pointer value = sep->get_value();
        if (value)
        {
            value = value->optimize();
            if (value->is_constant())
                return value;
        }
    }
    return create(get_location(), get_sp());
}


expression::pointer
expression_name_asm::clone(const location &locn)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(locn, get_sp());
}


void
expression_name_asm::traversal(int)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(!"should not happen");
}


void
expression_name_asm::logical_traversal(int, label::pointer &, label::pointer &,
    bool) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(!"should not happen");
}


// vim: set ts=8 sw=4 et :

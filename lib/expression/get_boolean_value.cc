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
#include <lib/ac/typeinfo>

#include <lib/debug.h>
#include <lib/expression/boolean.h>


bool
expression::get_boolean_value(void)
    const
{
    DEBUG(0, "class %s does not handle get_boolean_value method correctly, "
        "or the caller failed to check the type first, either way this is "
        "definitely a bug", typeid(*this).name());
    return false;
}


// vim: set ts=8 sw=4 et :

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

#include <lib/translator.h>
#include <lib/type/string.h>


expression::pointer
translator::char_from_string_of_length_one(const expression::pointer &ep)
{
    if (!ep->is_constant())
        return expression::pointer();
    if (!type_string::is_a(ep))
        return expression::pointer();
    rcstring value = ep->optimize()->get_string_value();
    if (value.size() != 1)
        return expression::pointer();
    return char_expression_factory(ep->get_location(), value[0]);
}


// vim: set ts=8 sw=4 et :

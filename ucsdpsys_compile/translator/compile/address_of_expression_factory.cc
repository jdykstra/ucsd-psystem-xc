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

#include <ucsdpsys_compile/translator/compile.h>


expression::pointer
translator_compile::address_of_expression_factory(const expression::pointer &ep)
{
    expression::pointer result = ep->strip_indirection();
    if (result)
        return result;
    pascal_lex_error
    (
        ep->get_location(),
        "unable to obtain the address of a %s",
        ep->get_description().c_str()
    );
    return expression_error::create(ep->get_location());
}


// vim: set ts=8 sw=4 et :

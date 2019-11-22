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

#include <lib/ac/ctype.h>

#include <ucsdpsys_littoral/expression/char/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_char_littoral::~expression_char_littoral()
{
}


expression_char_littoral::expression_char_littoral(
    const location &a_locn,
    unsigned char a_value,
    translator_littoral *cntxt
) :
    expression_char(a_locn, a_value),
    littoral(*cntxt)
{
}


expression_char_littoral::pointer
expression_char_littoral::create(const location &a_locn, unsigned char a_value,
    translator_littoral *cntxt)
{
    return pointer(new expression_char_littoral(a_locn, a_value, cntxt));
}


void
expression_char_littoral::traversal(int)
    const
{
    unsigned char c = get_integer_value();
    if (c == '\'')
        littoral.print_token("''''");
    else if (isprint(c))
        littoral.print_token(rcstring::printf("'%c'", c));
    else
        littoral.print_token(rcstring::printf("chr(%d)", c));
}


expression::pointer
expression_char_littoral::optimize(void)
    const
{
    return create(get_location(), get_integer_value(), &littoral);
}


expression::pointer
expression_char_littoral::clone(const location &locn)
    const
{
    return create(locn, get_integer_value(), &littoral);
}


// vim: set ts=8 sw=4 et :

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

#include <lib/type/long_integer/sized.h>

#include <ucsdpsys_littoral/expression/long_integer/littoral.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_long_integer_littoral::~expression_long_integer_littoral()
{
}


expression_long_integer_littoral::expression_long_integer_littoral(
    const location &locn,
    const rcstring &a_original,
    translator_littoral *a_littoral
) :
    expression_long_integer
    (
        locn,
        type_long_integer_sized::create(),
        long_integer::pascal_lex_lint_constant(a_original)
    ),
    original(a_original),
    littoral(*a_littoral)
{
}


expression_long_integer_littoral::pointer
expression_long_integer_littoral::create(const location &locn,
    const rcstring &orig1, translator_littoral *a_littoral)
{
    rcstring orig2 = orig1;
    if (orig2.back() != 'L')
        orig2 += "L";
    return
        pointer(new expression_long_integer_littoral(locn, orig2, a_littoral));
}


void
expression_long_integer_littoral::traversal(int)
    const
{
    littoral.print_token(original);
}


expression::pointer
expression_long_integer_littoral::optimize(void)
    const
{
    return create(get_location(), original, &littoral);
}


expression::pointer
expression_long_integer_littoral::clone(const location &locn)
    const
{
    return create(locn, original, &littoral);
}


// vim: set ts=8 sw=4 et :

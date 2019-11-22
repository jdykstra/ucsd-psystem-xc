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

#include <ucsdpsys_littoral/expression/lint2lint/littoral.h>


expression_lint2lint_littoral::~expression_lint2lint_littoral()
{
}


expression_lint2lint_littoral::expression_lint2lint_littoral(
    const type::pointer &a_to,
    const expression::pointer &a_from,
    translator_littoral *a_littoral
) :
    expression_lint2lint(a_to, a_from),
    littoral(*a_littoral)
{
}


expression_lint2lint_littoral::pointer
expression_lint2lint_littoral::create(const type::pointer &a_to,
    const expression::pointer &a_from, translator_littoral *a_littoral)
{
    return pointer(new expression_lint2lint_littoral(a_to, a_from, a_littoral));
}


void
expression_lint2lint_littoral::traversal(int pprec)
    const
{
    get_from()->traversal(pprec);
}


expression::pointer
expression_lint2lint_littoral::optimize(void)
    const
{
    return create(get_type(), get_from(), &littoral);
}


expression::pointer
expression_lint2lint_littoral::clone(const location &locn)
    const
{
    return create(get_type(), get_from()->clone(locn), &littoral);
}


bool
expression_lint2lint_littoral::is_constant(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :

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

#include <lib/pascal/lex.h>
#include <lib/type/integer.h>
#include <ucsdpsys_pretty/expression/sizeof/type.h>
#include <ucsdpsys_pretty/translator/pretty.h>


expression_sizeof_type::~expression_sizeof_type()
{
}


expression_sizeof_type::expression_sizeof_type(
    const location &locn,
    const type::pointer &a_subject,
    translator_pretty *cntxt
) :
    expression(locn),
    pretty(*cntxt),
    subject(a_subject)
{
}


expression_sizeof_type::pointer
expression_sizeof_type::create(const location &locn,
    const type::pointer &a_subject, translator_pretty *cntxt)
{
    return pointer(new expression_sizeof_type(locn, a_subject, cntxt));
}


void
expression_sizeof_type::traversal(int)
    const
{
    pretty.print_token("sizeof");
    pretty.print_token("(");
    pretty.print_token(subject->get_name());
    pretty.print_token(")");
}


void
expression_sizeof_type::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(0);
}


int
expression_sizeof_type::get_precedence()
    const
{
    return 0;
}


type::pointer
expression_sizeof_type::get_type()
    const
{
    return type_integer::create();
}


bool
expression_sizeof_type::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_sizeof_type::has_side_effect()
    const
{
    return side_effect_no;
}


expression::pointer
expression_sizeof_type::optimize(void)
    const
{
    return create(get_location(), subject, &pretty);
}


expression::pointer
expression_sizeof_type::clone(const location &locn)
    const
{
    return create(locn, subject, &pretty);
}


rcstring
expression_sizeof_type::lisp_representation(void)
    const
{
    return
        (
            "(sizeof-type "
        +
            subject->get_pascal_name()
        +
            ")"
        );
}


// vim: set ts=8 sw=4 et :

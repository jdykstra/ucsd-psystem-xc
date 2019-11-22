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

#include <lib/type/pointer/unpacked.h>

#include <ucsdpsys_littoral/expression/address_of.h>
#include <ucsdpsys_littoral/translator/littoral.h>


expression_address_of::~expression_address_of()
{
}


expression_address_of::expression_address_of(
    const expression::pointer &a_rhs,
    translator_littoral *a_littoral
) :
    expression(a_rhs->get_location()),
    rhs(a_rhs),
    littoral(*a_littoral)
{
}


expression_address_of::pointer
expression_address_of::create(const expression::pointer &a_rhs,
    translator_littoral *a_littoral)
{
    return pointer(new expression_address_of(a_rhs, a_littoral));
}


void
expression_address_of::traversal(int pprec)
    const
{
    littoral.print_token("@");
    if (need_parens(pprec))
        littoral.print_token("(");
    rhs->traversal(get_precedence());
    if (need_parens(pprec))
        littoral.print_token("(");
}


void
expression_address_of::logical_traversal(int, label::pointer &,
    label::pointer &, bool) const
{
    assert(!"the grammar should prevent this");
}


int
expression_address_of::get_precedence()
    const
{
    return prec_unary;
}


type::pointer
expression_address_of::get_type()
    const
{
    return type_pointer_unpacked::create(rhs->get_type());
}


bool
expression_address_of::is_lvalue()
    const
{
    return false;
}


expression::side_effect_t
expression_address_of::has_side_effect()
    const
{
    return rhs->has_side_effect();
}


expression::pointer
expression_address_of::optimize(void)
    const
{
    return create(rhs->optimize(), &littoral);
}


expression::pointer
expression_address_of::clone(const location &locn)
    const
{
    return create(rhs->clone(locn), &littoral);
}


rcstring
expression_address_of::lisp_representation(void)
    const
{
    return
        (
            "(address-of "
        +
            rhs->lisp_representation()
        +
            ")"
        );
}


bool
expression_address_of::is_constant(void)
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :

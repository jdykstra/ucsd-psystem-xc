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
#include <lib/type/char.h>
#include <lib/type/file.h>
#include <lib/type/integer.h>
#include <lib/type/nothing.h>
#include <lib/type/real.h>
#include <lib/type/string.h>

#include <ucsdpsys_compile/expression/comma.h>
#include <ucsdpsys_compile/expression/function_call/user_defined.h>
#include <ucsdpsys_compile/translator/compile.h>


expression_comma::~expression_comma()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_comma::expression_comma(
    const expression_list &a_params,
    translator_compile *a_code
) :
    expression(a_params.get_location()),
    rhs(a_params),
    code(*a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


expression_comma::pointer
expression_comma::create(const expression_list &a_params,
    translator_compile *a_code)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return pointer(new expression_comma(a_params, a_code));
}


void
expression_comma::traversal(int)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    for (size_t j = 0; j < rhs.size(); ++j)
        rhs[j]->traversal(0);
    DEBUG(1, "}");
}


void
expression_comma::logical_traversal(int pprec, label::pointer &true_branch,
    label::pointer &false_branch, bool fall_through_preference) const
{
    traversal(pprec);
    if (fall_through_preference)
        false_branch->branch_from_here_if_false();
    else
        true_branch->branch_from_here_if_true();
}


int
expression_comma::get_precedence(void)
    const
{
    return prec_comma;
}


type::pointer
expression_comma::get_type(void)
    const
{
    if (rhs.empty())
        return type_nothing::create();
    return rhs.back()->get_type();
}


bool
expression_comma::is_lvalue(void)
    const
{
    return false;
}


expression::side_effect_t
expression_comma::has_side_effect(void)
    const
{
    return side_effect_yes;
}


expression::pointer
expression_comma::optimize(void)
    const
{
    // Don't actually optimize yet.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(rhs.optimize(), &code);
}


expression::pointer
expression_comma::clone(const location &locn)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return create(rhs.clone(locn), &code);
}


rcstring
expression_comma::lisp_representation(void)
    const
{
    rcstring result = "(comma";
    for (size_t j = 0; j < rhs.size(); ++j)
        result += " " + rhs[j]->lisp_representation();
    result += ")";
    return result;
}


// vim: set ts=8 sw=4 et :

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

#include <lib/debug.h>
#include <lib/expression.h>
#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type/boolean.h>


statement::pointer
translator::if_statement(const expression::pointer &condition,
    const statement::pointer &then_clause,
    const statement::pointer &else_clause)
{
    //
    // Silently fail if we have already reported an error with the condition.
    //
    DEBUG(1, "check for errors");
    if (condition->is_error())
        return empty_statement_factory();

    //
    // Make sure the condition is boolean.
    //
    DEBUG(1, "check for bool");
    DEBUG(1, "cond = %s", condition->lisp_representation().c_str());
    expression::pointer e0 = call_noparam_func_if_necessary(condition);
    DEBUG(1, "e0 = %s", e0->lisp_representation().c_str());
    if (e0->is_error())
        return empty_statement_factory();
    expression::pointer e1 = e0->optimize();
    DEBUG(1, "e1 = %s", e1->lisp_representation().c_str());
    if (e1->is_error())
        return empty_statement_factory();
    if (!type_boolean::is_a(e1->get_type()))
    {
        DEBUG(1, "e1 = %s", e1->lisp_representation().c_str());
        pascal_lex_error
        (
            e1->get_location(),
            "the if statement condition expression must be of boolean type, "
                "not a %s",
            e1->get_description().c_str()
        );
        return empty_statement_factory();
    }

    //
    // Watch out for constant conditions.
    //
    DEBUG(1, "check for constant");
    if (e1->is_constant())
    {
        bool c = e1->optimize()->get_boolean_value();
        if (warning_constant_branch())
        {
            pascal_lex_warning
            (
                e1->get_location(),
                "the if statement control expression is constant, "
                    "it is always %s",
                (c ? "true" : "false")
            );
        }
        if (c)
            return then_clause;
        if (!else_clause)
            return empty_statement_factory();
        return else_clause;
    }

    //
    // The normal case.
    //
    DEBUG(1, "normal");
    return if_statement_factory(e1, then_clause, else_clause);
}


// vim: set ts=8 sw=4 et :

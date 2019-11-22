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

#include <lib/expression.h>
#include <lib/pascal/lex.h>
#include <lib/statement/list.h>
#include <lib/translator.h>
#include <lib/type/boolean.h>


statement::pointer
translator::until_statement(const statement_list &body,
    const expression::pointer &condition)
{
    //
    // Suppress secondary errors.
    //
    if (condition->is_error())
        return empty_statement_factory();

    //
    // Make sure the condition is boolean.
    //
    expression::pointer e1 = call_noparam_func_if_necessary(condition);
    e1 = e1->optimize();
    if (!type_boolean::is_a(e1->get_type()))
    {
        pascal_lex_error
        (
            condition->get_location(),
            "until condition must be of boolean type, not a %s",
            condition->get_description().c_str()
        );
        return empty_statement_factory();
    }

    //
    // If the loop body never terminates, there is no need for the loop
    // control expression.
    //
    if (!body.execution_flows_out())
    {
        pascal_lex_warning
        (
            body.get_location(),
            "repeat until body never terminates"
        );
        return compound_statement_factory(body);
    }

    //
    // Watch out for constant conditions.
    //
    if (e1->is_constant())
    {
        bool c = e1->optimize()->get_boolean_value();
        if (warning_constant_branch())
        {
            pascal_lex_warning
            (
                e1->get_location(),
                "the repeat until statement control expression is constant, "
                "it is always %s",
                (c ? "true" : "false")
            );
        }
        statement::pointer sp = compound_statement_factory(body);
        if (!c)
            return infinite_loop_factory(sp);

        // just execute the loop body once
        return sp;
    }

    //
    // The normal case.
    //
    return until_statement_factory(body, e1);
}


// vim: set ts=8 sw=4 et :

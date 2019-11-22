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
#include <lib/pascal/lex.h>
#include <lib/translator.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/integer.h>
#include <lib/type/real.h>


statement::pointer
translator::for_statement(const expression::pointer &var,
    const expression::pointer &limit1p, bool descending,
    const expression::pointer &limit2p, const statement::pointer &body)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (var->is_error() || limit1p->is_error() || limit2p->is_error())
    {
        DEBUG(1, "}");
        return empty_statement_factory();
    }
    DEBUG(2, "var = %s", var->lisp_representation().c_str());
    if (!var->is_lvalue())
    {
        yuck:
        pascal_lex_error
        (
            var->get_location(),
            "for control expression must name a variable of a countable type, "
                "not a %s",
            var->get_description().c_str()
        );
        DEBUG(1, "}");
        return empty_statement_factory();
    }
    DEBUG(1, "it _is_ a variable");
    expression::pointer limit1 = limit1p;
    {
        expression::pointer ep = char_from_string_of_length_one(limit1);
        if (ep)
            limit1 = ep;
    }
    expression::pointer limit2 = limit2p;
    {
        expression::pointer ep = char_from_string_of_length_one(limit2);
        if (ep)
            limit2 = ep;
    }

    if (type_real::is_a(var))
    {
        // Silently promote integers to reals.
        if (type_integer::is_a(limit1))
            limit1 = real_from_integer_expression_factory(limit1);
        if (type_integer::is_a(limit2))
            limit2 = real_from_integer_expression_factory(limit1);
    }
    else if
    (
        !type_boolean::is_a(var)
    &&
        !type_char::is_a(var)
    &&
        !type_enumerated::is_a(var)
    &&
        !type_integer::is_a(var)
    )
    {
        DEBUG(1, "wrong");
        goto yuck;
    }

    DEBUG(2, "mark");
    if (var->get_type() != limit1->get_type())
    {
        pascal_lex_error
        (
            limit1->get_location(),
            "initial value type mismatch; cannot assign a %s to a %s",
            limit1->get_description().c_str(),
            var->get_description().c_str()
        );
        DEBUG(1, "}");
        return empty_statement_factory();
    }
    DEBUG(2, "mark");
    if (var->get_type() != limit2->get_type())
    {
        pascal_lex_error
        (
            limit2->get_location(),
            "final value type mismatch; cannot assign a %s to %s",
            limit2->get_description().c_str(),
            var->get_description().c_str()
        );
        DEBUG(1, "}");
        return empty_statement_factory();
    }
    DEBUG(2, "mark");
    if (limit1->is_constant() && limit2->is_constant())
    {
        if (type_real::is_a(var))
        {
            double v1 = limit1->optimize()->get_real_value();
            double v2 = limit2->optimize()->get_real_value();
            // Check that the range is around the right way.
            if (!descending)
            {
                // ascending
                if (v1 > v2)
                {
                    pascal_lex_warning
                    (
                        limit1->get_location(),
                        "the initial value (%g) is greater than final value "
                            "(%g), the statement has no effect",
                        v1,
                        v2
                    );
                    DEBUG(1, "}");
                    return empty_statement_factory();
                }
            }
            else
            {
                // descending
                if (v1 < v2)
                {
                    pascal_lex_warning
                    (
                        limit1->get_location(),
                        "the initial value (%g) is less than final value (%g), "
                            "the statement has no effect",
                        v1,
                        v2
                    );
                    DEBUG(1, "}");
                    return empty_statement_factory();
                }
            }
        }
        else
        {
            long v1 = limit1->optimize()->get_integer_value();
            long v2 = limit2->optimize()->get_integer_value();
            // Check that the range is around the right way.
            if (!descending)
            {
                // ascending
                if (v1 > v2)
                {
                    rcstring v1s =
                        limit1->get_type()->
                            get_human_readable_representation_of_value(v1);
                    rcstring v2s =
                        limit1->get_type()->
                            get_human_readable_representation_of_value(v2);
                    pascal_lex_warning
                    (
                        limit1->get_location(),
                        "the initial value (%s) is greater than final value "
                            "(%s), the statement has no effect",
                        v1s.c_str(),
                        v2s.c_str()
                    );
                    DEBUG(1, "}");
                    return empty_statement_factory();
                }
            }
            else
            {
                // descending
                if (v1 < v2)
                {
                    rcstring v1s =
                        limit1->get_type()->
                            get_human_readable_representation_of_value(v1);
                    rcstring v2s =
                        limit1->get_type()->
                            get_human_readable_representation_of_value(v2);
                    pascal_lex_warning
                    (
                        limit1->get_location(),
                        "the initial value (%s) is less than final value (%s), "
                            "the statement has no effect",
                        v1s.c_str(),
                        v2s.c_str()
                    );
                    DEBUG(1, "}");
                    return empty_statement_factory();
                }
            }
        }
    }

    //
    // Looks good.  Have the derived class actually build it.
    //
    DEBUG(2, "mark");
    statement::pointer result =
        for_statement_factory(var, limit1, descending, limit2, body);
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :

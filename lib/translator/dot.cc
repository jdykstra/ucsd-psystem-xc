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

#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/symbol.h>
#include <lib/translator.h>
#include <lib/type/pointer.h>
#include <lib/type/record.h>
#include <lib/variable/name.h>


expression::pointer
translator::dot_expression(const expression::pointer &a_lhs,
    const variable_name &rhs)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    expression::pointer lhs = a_lhs;
    if (lhs->is_error())
    {
        DEBUG(1, "}");
        return lhs;
    }

    //
    // It is a common error to forget the ^ operator, so look for it and
    // tell them if this has happened.  This reduces secondary errors.
    //
    if
    (
        type_pointer::is_a(lhs->get_type())
    &&
        type_record::is_a(lhs->get_type()->get_subtype())
    )
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "to access a field of a pointer, you need the ^. operators"
        );
        lhs = dereference_expression(lhs);
    }

    //
    // Make sure the left hand side is a record type.
    //
    type_record::pointer trp =
        boost::dynamic_pointer_cast<type_record>(lhs->get_type());
    if (!trp)
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "the left hand side of the dot (.) operator is a %s, "
                "it needs to be of record type",
            lhs->get_description().c_str()
        );
        DEBUG(1, "}");
        return expression_error::create(lhs->get_location());
    }

    //
    // Look up the name on the right in the left hand side's symbol
    // table.  If the symbol isn't found, look again for misspellings.
    // Misspelings are a common error, so this reduces secondary errors.
    //
    symbol::pointer sp = trp->lookup(rhs.get_name());
    if (!sp)
    {
        rcstring other;
        sp = trp->lookup_fuzzy(rhs.get_name());
        if (!sp)
        {
            pascal_lex_error
            (
                rhs.get_location(),
                "record field %s unknown",
                rhs.get_name().quote_c().c_str()
            );
            DEBUG(1, "}");
            return expression_error::create(rhs.get_location());
        }

        pascal_lex_error
        (
            rhs.get_location(),
            "record field %s unknown, "
                "guessing you meant %s instead",
            rhs.get_name().quote_c().c_str(),
            sp->get_name().quote_c().c_str()
        );
    }
    sp->add_reference(rhs);

    //
    // Make sure the left hand side is an l-value.
    //
    if (!lhs->is_lvalue())
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "the left hand side of the dot (.) operator must be a variable"
        );
    }

    //
    // We have run the gauntlet and survived, so manufacture the
    // translator-specific expression instance.
    //
    expression::pointer result =
        lhs->dot_expression_factory(rhs.get_location(), sp);
    DEBUG(1, "}");
    return result;
}


// vim: set ts=8 sw=4 et :

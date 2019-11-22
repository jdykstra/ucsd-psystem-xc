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
#include <lib/pascal/lex.h>
#include <lib/symbol/typedef.h>
#include <lib/translator.h>
#include <lib/type/error.h>
#include <lib/type/forward.h>
#include <lib/type/integer.h>
#include <lib/type/long_integer/unsized.h>
#include <lib/type/string.h>


type::pointer
translator::type_lookup(const rcstring &name, bool deferable)
{
    symbol::pointer sp = scopes.lookup(name);
    if (!sp)
    {
        if (deferable)
            return type_forward::create(name, this);
        sp = scopes.lookup_fuzzy(name);
        if (sp)
        {
            pascal_lex_error
            (
                pascal_lex_location(),
                "symbol %s unknown, guessing you meant %s instead",
                name.quote_c().c_str(),
                sp->get_name().quote_c().c_str()
            );
        }
        else
        {
            pascal_lex_error
            (
                pascal_lex_location(),
                "type %s unknown",
                name.quote_c().c_str()
            );
            return type_error::create();
        }
    }
    symbol_typedef::pointer stdp =
        boost::dynamic_pointer_cast<symbol_typedef>(sp);
    if (!stdp)
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "symbol %s is not the name of a type",
            sp->get_name().quote_c().c_str()
        );
        return type_error::create();
    }
    return sp->get_type();
}


type::pointer
translator::type_lookup(const rcstring &name, const expression::pointer &size)
{
    type::pointer tp = type_lookup(name, false);
    if (type_error::is_a(tp))
        return tp;

    if (type_integer::is_a(tp))
    {
        // build and return a long integer type
        expression::pointer ep = size->optimize();
        if (!ep->is_constant())
        {
            pascal_lex_error(size->get_location(), "integer size not constant");
            return type_integer::create();
        }
        long ndig = ep->optimize()->get_integer_value();
        if (ndig < 1 || ndig > type_long_integer::max_digits)
        {
            pascal_lex_error
            (
                size->get_location(),
                "integer size %ld not in the 1..%d range",
                ndig,
                type_long_integer::max_digits
            );
            return type_integer::create();
        }
        return type_long_integer_unsized::create(ndig);
    }

    //
    // String types can have a maximum length specified.
    //
    // FIXME: This is a too liberal, it even works for already-qualified
    // string types.
    //
    if (type_string::is_a(tp))
    {
        expression::pointer ep = size->optimize();
        if (!ep->is_constant())
        {
            pascal_lex_error(size->get_location(), "string size not constant");
            return type_string::create();
        }
        long n = ep->optimize()->get_integer_value();
        if (n < 1 || n > 255)
        {
            pascal_lex_error
            (
                size->get_location(),
                "string size %ld out of 1..255 range",
                n
            );
            return type_string::create();
        }
        return type_string::create(n);
    }

    //
    // No other types accept size qualifiers.
    //
    pascal_lex_error
    (
        size->get_location(),
        "the %s type does not accept a [size] qualifier",
        tp->get_name().c_str()
    );
    return tp;
}


void
translator::late_binding_opportunity()
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    scopes.late_binding();
    DEBUG(1, "}");
}


// vim: set ts=8 sw=4 et :

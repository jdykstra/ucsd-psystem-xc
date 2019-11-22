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

#include <lib/anonymous_name.h>
#include <lib/debug.h>
#include <lib/pascal/lex.h>
#include <lib/scope/temporary/with.h>
#include <lib/symbol/expression/with.h>
#include <lib/translator.h>


scope_temporary_with::~scope_temporary_with()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


scope_temporary_with::scope_temporary_with(
    scope_stack &stk,
    const expression::pointer &a_base,
    const statement::pointer &a_base_asgn,
    translator *a_context
) :
    scope_temporary(stk),
    base(a_base),
    trp(boost::dynamic_pointer_cast<type_record>(a_base->get_type())),
    base_assignment(a_base_asgn),
    context(a_context)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


symbol::pointer
scope_temporary_with::lookup(const rcstring &qname)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "qname = %s", qname.quote_c().c_str());
    if (!trp)
    {
        DEBUG(1, "}");
        return symbol::pointer();
    }
    symbol::pointer field = trp->lookup(qname);
    if (!field)
    {
        DEBUG(1, "}");
        return symbol::pointer();
    }

    //
    // We are going to build a transient symbol_expression_with instance
    // to handle this case.  The value of the symbol is going to be the
    // appropriate dot expression tree.
    //
    // This way the reference list in the field symbol in the record
    // gets updated correctly.  Another way would have been, at scope
    // creation time, to build a snoot-load of dot expressions, one per
    // record field, and define them as actual symbols, but that gets
    // the cross-reference information wrong.
    //
    // The use of expression::clone() means that the compiler listing
    // will locate the base expression more accurately, as if it occurs
    // where the field name appears.
    //
    location locn = pascal_lex_location();
    variable_name varnam(qname, locn);
    expression::pointer bp = base->clone(locn);
    expression::pointer ep = context->implicit_dot_expression(bp, varnam);
    DEBUG(1, "ep = %p", ep.get());
    assert(ep);
    assert(!ep->is_error());
    symbol::pointer sewp =
        symbol_expression_with::create(qname, field->get_lex_level(), ep);
    DEBUG(1, "return %p }", sewp.get());
    return sewp;
}


rcstring
scope_temporary_with::lookup_fuzzy(const rcstring &qname, double &best)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (!trp)
        return "";
    return trp->lookup_fuzzy(qname, best);
}


// vim: set ts=8 sw=4 et :

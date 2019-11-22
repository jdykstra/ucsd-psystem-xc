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
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/scope/temporary/with.h>
#include <lib/symbol/functor/with.h>
#include <lib/translator.h>
#include <lib/type/pointer/unpacked.h>


void
translator::with_scope_begin(const expression::pointer &a_base)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    expression::pointer base(a_base);

    //
    // Common error: did they forget to deference?
    //
    if
    (
        type_pointer::is_a(base->get_type())
    &&
        type_record::is_a(base->get_type()->get_subtype())
    )
    {
        pascal_lex_error
        (
            base->get_location(),
            "missing dereference (^) operator"
        );
        base = dereference_expression(base);
    }

    statement::pointer base_assignment;

    //
    // The expression must be a record and an L-value, so that we can
    // take it's address.
    //
    if (!base->is_lvalue())
    {
        yuck:
        pascal_lex_error
        (
            base->get_location(),
            "the with expression must be a record variable, not a %s",
            base->get_description().c_str()
        );

        //
        // All ways out of this method need to have pushed a
        // scope, otherwise with_scope_end is going to make a mess.
        //
        scope::pointer sw =
            with_temporary_scope_factory
            (
                scopes,
                expression_error::create(base->get_location()),
                base_assignment
            );
        scopes.push_back(sw);
        DEBUG(1, "}");
        return;
    }
    type_record::pointer rtp =
        boost::dynamic_pointer_cast<type_record>(base->get_type());
    if (!rtp)
        goto yuck;

    //
    // This can go two ways: a simple enough expression doesn't need
    // a temporary pointer, but anything else causes us to stash the
    // pointer and re-use it later.
    //
    if (!feature_short_with_flag || !base->is_simple_with_stmt())
    {
        rcstring aname = anonymous_name();
        variable_name vn(aname, base->get_location());
        type::pointer tp = type_pointer_unpacked::create(base->get_type());
        variable_name_list vnl;
        vnl.push_back(vn);
        variable_declaration(vnl, tp);

        expression::pointer ep1 = name_expression(base->get_location(), aname);
        expression::pointer ep2 = address_of_expression_factory(base);
        expression::pointer ep3 = assignment_expression_factory(ep1, ep2);
        base_assignment = expression_statement(ep3);

        base = this->dereference_expression(ep1);
    }

    //
    // Build a scope that synthesizes appropriate expressions when
    // symbols from the record are seen.
    //
    scope::pointer stw =
        with_temporary_scope_factory(scopes, base, base_assignment);

    //
    // Check that the record fields do not shadow earlier symbols.
    //
    if (warning_shadow_flag)
    {
        symbol_functor_with fun(scopes, base->get_location());
        rtp->symbol_table_walk(fun);
    }

    //
    // Add our shiny new scope to the stack.
    //
    scopes.push_back(stw);
    DEBUG(1, "}");
}


statement::pointer
translator::with_scope_end(const expression::pointer &ep,
    const statement::pointer &body)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    scope_temporary_with::pointer stw =
        boost::dynamic_pointer_cast<scope_temporary_with>(scopes.back());
    assert(stw);
    scopes.pop_back();
    statement::pointer sp =
        with_statement_factory(ep, stw->get_base_assignment(), body);
    stw.reset();
    DEBUG(1, "}");
    return sp;
}


// vim: set ts=8 sw=4 et :

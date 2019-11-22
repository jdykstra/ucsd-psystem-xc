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
#include <lib/expression/list.h>
#include <lib/pascal/lex.h>
#include <lib/statement/compound.h>
#include <lib/statement/empty.h>
#include <lib/statement/list.h>
#include <lib/type/char.h>
#include <lib/type/file.h>
#include <lib/type/nothing.h>
#include <lib/type/reference.h>

#include <ucsdpsys_compile/translator/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/lod.h>
#include <ucsdpsys_compile/expression/writeln.h>
#include <ucsdpsys_compile/statement/expression/compile.h>


statement::pointer
translator_compile::writeln_statement_factory(const expression_list &args,
    bool eoln)
{
    location locn =
        (args.empty() ? pascal_lex_location() : args.get_location());

    expression::pointer first_parameter;
    size_t first_index = 0;
    if (args.empty() || !type_file::is_a(args.get(0)->get_type()))
    {
        // There is an implicit OUTPUT expression at the start, if
        // no FILE expression is given.
        first_parameter = name_expression(locn, "output");
        DEBUG(1, "output is a %s",
            first_parameter->get_description().c_str());
    }
    else
    {
        first_parameter = args.get(0);
        first_index = 1;

        if (!type_char::is_a(first_parameter->get_type()->get_subtype()))
        {
            const char *suggest = "put";
            if (type_nothing::is_a(first_parameter->get_type()->get_subtype()))
                suggest = "blockwrite";
            pascal_lex_error
            (
                first_parameter->get_location(),
                "call of %s procedure: parameter one: must be a file of char, "
                    "not a %s; the \"%s\" procedure is more appropriate",
                (eoln ? "writeln" : "write"),
                first_parameter->get_description().c_str(),
                suggest
            );
            return statement_empty::create();
        }
    }
    assert(first_parameter);

    //
    // We turn the parameter list into a sequence of procedure calls,
    // all with the same first parameter.
    //
    statement_list seq;
    for (size_t j = first_index; j < args.size(); ++j)
    {
        expression::pointer ep = args.get(j);
        if (ep->is_error())
            continue;
        expression_writeln::pointer ewp =
            boost::dynamic_pointer_cast<expression_writeln>(ep);
        if (!ewp)
        {
            expression::pointer zero =
                expression_integer_compile::create(ep->get_location(), 0, this);
            ep = expression_writeln::create(ep, zero, this);
            ewp = boost::dynamic_pointer_cast<expression_writeln>(ep);
            if (!ewp)
            {
                assert(ewp->is_error());
                continue;
            }
        }

        //
        // Everything looks OK.  Build an expression statement from a
        // procedure call.
        //
        ep = ewp->function_call_expression_factory(first_parameter);
        ep = ep->optimize();
        statement::pointer sp = statement_expression_compile::create(ep, this);
        seq.push_back(sp);
    }

    //
    // Add the FWRITELN call.
    //
    if (eoln)
    {
        if (!args.empty())
            locn = args.back()->get_location();
        expression_list nargs;
        expression::pointer ep = name_expression(locn, "sys:fwriteln");
        assert(ep);
        nargs.push_back(ep);
        nargs.push_back(first_parameter);
        ep = ep->function_call_expression_factory(nargs);
        ep = ep->optimize();
        statement::pointer sp = statement_expression_compile::create(ep, this);
        seq.push_back(sp);
    }

    if (seq.empty())
    {
        pascal_lex_warning(locn, "statement with no effect");
    }

    //
    // Return a compound statement with all of the parts.
    //
    if (seq.size() == 1)
        return seq.get(0);
    return statement_compound::create(seq);
}


// vim: set ts=8 sw=4 et :

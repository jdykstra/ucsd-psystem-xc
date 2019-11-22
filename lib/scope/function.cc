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

#include <lib/pascal/lex.h>
#include <lib/scope/function.h>
#include <lib/symbol.h>
#include <lib/symbol/function.h>
#include <lib/type/nothing.h>


scope_function::~scope_function()
{
}


scope_function::scope_function(const rcstring &a_name) :
    scope(a_name),
    return_type(type_nothing::create())
{
}


void
scope_function::attach_return_type(const type::pointer &tp)
{
    assert(tp->get_size_in_bits() <= 32);
    return_type = tp;
    symbol_function::pointer fsp =
        boost::dynamic_pointer_cast<symbol_function>(get_sp());
    if (fsp)
        fsp->attach_return_type(tp);
    else
        pascal_grammar_error("attach return type to non-function symbol (bug)");
}


void
scope_function::attach_function_parameters(name_type_list *ntlp)
{
    symbol_function::pointer fsp =
        boost::dynamic_pointer_cast<symbol_function>(get_sp());
    if (fsp)
        fsp->attach_function_parameters(*ntlp);
    else
        pascal_grammar_error("attach parameters to non-function symbol (bug)");

    notify_function_parameters(*ntlp);
}


bool
scope_function::was_forward(void)
    const
{
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(get_sp());
    assert(sfp);
    return (sfp && sfp->is_forward());
}


void
scope_function::set_forward(bool ign_seg0)
{
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(get_sp());
    assert(sfp);
    if (sfp)
        sfp->set_forward(ign_seg0);
}


void
scope_function::drop_locals(void)
{
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(get_sp());
    assert(sfp);
    if (sfp)
        sfp->drop_locals();
}


int
scope_function::get_lex_level_height(void)
    const
{
    return 1;
}


void
scope_function::label_declaration_notify(const variable_name_list &)
{
    // Ignore.
}


link_info::pointer
scope_function::get_link_info_extproc(void)
    const
{
    symbol_function::pointer sfp =
        boost::dynamic_pointer_cast<symbol_function>(get_sp());
    assert(sfp);

    bool is_a_proc = type_nothing::is_a(sfp->get_type()->get_subtype());
    int nparm = get_param_size_in_bytes() / 2;
    if (!is_a_proc)
        nparm -= 2;
    return
        link_info::create
        (
            sfp->get_name(),
            (is_a_proc ? EXTPROC : EXTFUNC),
            sfp->get_procedure_number(),
            nparm
        );
}


// vim: set ts=8 sw=4 et :

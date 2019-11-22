//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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
#include <lib/ac/typeinfo>

#include <lib/code_source_map.h>
#include <lib/debug.h>
#include <lib/pascal_id.h>
#include <lib/scope.h>
#include <lib/symbol/functor.h>
#include <lib/symbol/variable/normal.h>
#include <lib/type.h>


scope::~scope()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
}


scope::scope(const rcstring &a_name, bool a_packed) :
    name(a_name),
    sp(),
    packed(a_packed)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
}


void
scope::define(const symbol::pointer &arg, bool isa_param)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    DEBUG(1, "arg = %p %s %s", arg.get(), arg->get_kind(),
            arg->get_name().quote_c().c_str());
    define(pascal_id(arg->get_name()), arg, isa_param);
    DEBUG(1, "}");
}


void
scope::define(const rcstring &key, const symbol::pointer &arg, bool isa_param)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    DEBUG(1, "key = %s", key.quote_c().c_str());
    DEBUG(1, "arg = %p %s %s", arg.get(), arg->get_kind(),
            arg->get_name().quote_c().c_str());
    //
    // Add the symbol to the local symbol table.
    //
    symbol_table.assign(key, arg);

    //
    // Keep track of the symbols in need of late binding.
    //
    if (arg->need_late_binding())
        symbols_in_need_of_late_binding.push_back(arg);

    //
    // If it's not a variable (it could be a function or a constant,
    // etc) then do nothing.
    //
    if (!boost::dynamic_pointer_cast<symbol_variable>(arg))
    {
        // do nothing
    }
    else if (arg->get_type()->is_a_variable())
    {
        assert(!isa_param || !packed);

        //
        // Allocate space in the local variables for the new variable or
        // function parameter.
        //
        bool new_packed = !isa_param && packed;
        bit_address b = allocate_space(arg->get_type());
        arg->set_bit_offset(b, new_packed);
        if (isa_param)
        {
            assert(parameter_size.on_word_boundary());
            int nwords = arg->get_type()->get_size_in_words();
            parameter_size.advance_by_words(nwords);
        }
        // FIXME: check for stack frame too large
    }
    DEBUG(1, "}");
}


bit_address
scope::allocate_space(const type::pointer &tp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (packed)
    {
        unsigned nbits = tp->get_size_in_bits();
        if (nbits < 16)
            return get_next_bit_address(nbits);
    }

    unsigned nbits = tp->get_size_in_words() * 16;
    return get_next_bit_address(nbits);
}


bit_address
scope::get_next_bit_address(unsigned nbits)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    assert(nbits > 0);
    assert(!(nbits & 15) || nbits <= 16);
    bit_address result = frame_size_current;
    if (!frame_size_current.have_space_for(nbits))
    {
        frame_size_current.advance_to_word_boundary();
        result = frame_size_current;
    }
    frame_size_current.advance_by_bits(nbits);
    if (nbits > 16)
        frame_size_current.advance_to_word_boundary();
    if (frame_size_highwater < frame_size_current)
        frame_size_highwater = frame_size_current;
    return result;
}


void
scope::define(const symbol::pointer &arg, const bit_address &where)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    rcstring key = pascal_id(arg->get_name());
    DEBUG(1, "key = %s", key.quote_c().c_str());
    DEBUG(1, "arg = %p %s %s", arg.get(), arg->get_kind(),
            arg->get_name().quote_c().c_str());
    //
    // Add the symbol to the local symbol table.
    //
    symbol_table.assign(key, arg);

    //
    // Set the variable's bit address
    //
    assert(!arg->need_late_binding());
    assert(boost::dynamic_pointer_cast<symbol_variable>(arg));
    arg->set_bit_offset(where, true);
    DEBUG(1, "}");
}


unsigned
scope::get_data_size_in_bytes(void)
    const
{
    return
        (
            (
                frame_size_highwater.get_size_in_words()
            -
                parameter_size.get_size_in_words()
            )
        *
            2
        );
}


unsigned
scope::get_param_size_in_bytes(void)
    const
{
    return parameter_size.get_word_addr() * 2;
}


unsigned
scope::get_size_in_bytes(void)
    const
{
    return get_size_in_words() * 2;
}


unsigned
scope::get_size_in_bits(void)
    const
{
    return get_size_in_words() * 16;
}


symbol::pointer
scope::lookup(const rcstring &kname)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring key = pascal_id(kname);
    symbol::pointer *result = symbol_table.query(key);
    return (result ? *result : symbol::pointer());
}


rcstring
scope::lookup_fuzzy(const rcstring &kname, double &best)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    rcstring key = pascal_id(kname);
    return symbol_table.query_fuzzy(key, best);
}


void
scope::check_defined(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    //
    // Make sure there are no symbols that were declared FORWARD that
    // are still undefined.
    //
    for
    (
        symbol_table_t::const_iterator it = symbol_table.begin();
        it != symbol_table.end();
        ++it
    )
    {
        symbol::pointer s = **it;
        DEBUG(2, "%s", s->get_name().quote_c().c_str());
        s->check_defined();
    }
    DEBUG(1, "}");
}


void
scope::traversal(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    pre_order_traversal();
    if (body)
        body->traversal();
    post_order_traversal();
}


void
scope::pre_order_traversal(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    // Do nothing.
}


void
scope::post_order_traversal(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    // Do nothing.
}


void
scope::attach_return_type(const type::pointer &)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    // Do nothing.  The scope_function class takes care of this when it
    // happens for real.
    assert(!"this should be overridden");
}


void
scope::attach_body(const statement::pointer &arg)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    body = arg;
}


void
scope::attach_symbol(const symbol::pointer &arg)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    assert(arg);
    sp = arg;
}


void
scope::reserve_param_space_in_bytes(unsigned nbytes)
{
    // This is only ever called from the constructor of a function
    // scope, to reserve space for the function return value.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    assert(nbytes > 0);
    assert((nbytes & 1) == 0);
    assert(parameter_size.empty());
    assert(frame_size_current.empty());
    assert(frame_size_highwater.empty());
    unsigned nwords = nbytes >> 1;
    parameter_size.advance_by_words(nwords);
    frame_size_current.advance_by_words(nwords);
    frame_size_highwater = frame_size_current;
}


void
scope::attach_function_parameters(name_type_list *ntlp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    //
    // Let the derived class chew on it, if necessary.
    //
    notify_function_parameters(*ntlp);
}


bool
scope::do_not_search_past_this_point(void)
    const
{
    return false;
}


symbol::pointer
scope::get_sp(void)
    const
{
    return sp;
}


unsigned
scope::get_size_in_words(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    return frame_size_highwater.get_size_in_words();
}


bit_address
scope::get_next_local_bit_offset(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    return frame_size_current;
}


bit_address
scope::get_data_mark(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    assert(frame_size_current <= frame_size_highwater);
    return frame_size_current;
}

void
scope::set_data_mark(const bit_address &arg)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    assert(arg <= frame_size_current);
    assert(arg <= frame_size_highwater);
    frame_size_current = arg;
}


bool
scope::need_late_binding(void)
    const
{
    return !symbols_in_need_of_late_binding.empty();
}


void
scope::late_binding(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    symbols_in_need_of_late_binding_t temp = symbols_in_need_of_late_binding;
    symbols_in_need_of_late_binding.clear();
    for
    (
        symbols_in_need_of_late_binding_t::iterator it = temp.begin();
        it != temp.end();
        ++it
    )
    {
        symbol::pointer symptr = *it;
        DEBUG(1, "symptr = %p", symptr.get());
        assert(symptr);
        DEBUG(1, "symptr = %s", symptr->get_name().quote_c().c_str());
        symptr->type_late_binding();

        // It may not be completely successful.
        if (symptr->need_late_binding())
            symbols_in_need_of_late_binding.push_back(symptr);
    }
    DEBUG(1, "}");
}


bool
scope::was_forward(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    return false;
}


void
scope::set_forward(bool)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    assert(!"this should be overridden");
}


void
scope::drop_locals(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    // Do nothing.
}


void
scope::symbol_table_walk(symbol_functor &func)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    for
    (
        symbol_table_t::const_iterator it = symbol_table.begin();
        it != symbol_table.end();
        ++it
    )
    {
        assert(*it);
        symbol::pointer sy = **it;
        assert(sy);
        func(sy);
    }
    DEBUG(1, "}");
}


bool
scope::is_temporary(void)
    const
{
    return false;
}


void
scope::begin_notify(void)
{
    // Do nothing.
}


void
scope::list_symbols(code_source_map &where)
    const
{
    for
    (
        symbol_table_t::const_iterator it = symbol_table.begin();
        it != symbol_table.end();
        ++it
    )
    {
        assert(*it);
        symbol::pointer sy = **it;
        assert(sy);
        where.symbol_listing(*sy);
    }
}


bool
scope::will_accept_enums(void)
    const
{
    return true;
}


bool
scope::is_a_unit(void)
    const
{
    return false;
}


link_info::pointer
scope::get_link_info_extproc(void)
    const
{
    return link_info::pointer();
}


symbol_variable::pointer
scope::variable_factory(const variable_name &vname, const type::pointer &, int)
{
    DEBUG(1, "name = %s", vname.get_name().quote_c().c_str());
    DEBUG(1, "class = %s", typeid(*this).name());
    assert(!"variable_factory not sufficiently refined");
    return symbol_variable::pointer();
}


symbol::pointer
scope::constant_factory(const variable_name &vname, int,
    const expression::pointer &)
{
    DEBUG(1, "name = %s", vname.get_name().quote_c().c_str());
    DEBUG(1, "class = %s", typeid(*this).name());
    assert(!"constant_factory not sufficiently refined");
    return symbol::pointer();
}


void
scope::unit_interface_begin(void)
{
}


void
scope::unit_interface_end(void)
{
}


void
scope::unit_implementation_begin(void)
{
}


void
scope::unit_implementation_end(void)
{
}


// vim: set ts=8 sw=4 et :

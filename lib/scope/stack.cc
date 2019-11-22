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

#include <lib/debug.h>
#include <lib/pascal_id.h>
#include <lib/scope.h>
#include <lib/scope/stack.h>
#include <lib/symbol/enumerated.h>


scope_stack::~scope_stack()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    delete [] list;
}


scope_stack::scope_stack() :
    length(0),
    maximum(0),
    list(0),
    lex_level_cache(0)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", representation().c_str());
}


void
scope_stack::push_back(const scope::pointer &sp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 8;
        scope::pointer *new_list = new scope::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_list[j] = list[j];
        delete [] list;
        list = new_list;
        maximum = new_maximum;
    }
    list[length++] = sp;
    lex_level_cache += sp->get_lex_level_height();
    DEBUG(2, "%s = %d", representation().c_str(), lex_level_cache);
    DEBUG(1, "}");
}


void
scope_stack::push_front(const scope::pointer &sp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    if (length >= maximum)
    {
        size_t new_maximum = maximum * 2 + 8;
        scope::pointer *new_list = new scope::pointer [new_maximum];
        for (size_t j = 0; j < length; ++j)
            new_list[j] = list[j];
        delete [] list;
        list = new_list;
        maximum = new_maximum;
    }

    ++length;
    for (size_t k = length - 1; k > 0; --k)
        list[k] = list[k - 1];
    list[0] = sp;
    // lex_level_cache += sp->get_lex_level_height();
    DEBUG(2, "%s", representation().c_str());
    DEBUG(1, "}");
}


void
scope_stack::pop_back(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(length > 0);
    --length;
    scope::pointer sp = list[length];
    list[length].reset();
    lex_level_cache -= sp->get_lex_level_height();
    DEBUG(2, "%s", representation().c_str());
    DEBUG(1, "}");
}


scope::pointer
scope_stack::back(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (!length)
        return scope::pointer();
    return list[length - 1];
}


rcstring
scope_stack::representation(void)
    const
{
    rcstring result("[");
    for (size_t j = 0; j < length; ++j)
    {
        if (result.size() > 1)
            result += ", ";
        result += list[j]->get_name();
    }
    result += "]";
    return result;
}


void
scope_stack::define(const symbol::pointer &sp, bool isa_param)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    DEBUG(1, "sp = %p %s", sp.get(), sp->get_name().quote_c().c_str());
    DEBUG(2, "%s", representation().c_str());
    rcstring name = pascal_id(sp->get_name());

    // We have to make sure that record scopes don't also grab and hang
    // onto their enums.  For example
    //
    //     var foo: record
    //                tricky: (complex, critical, delicate, intricate,
    //                         involved, knotty, quirky, perplexing,
    //                         precariousm problematic, risky, rocky,
    //                         sensitive, sticky, thorny, ticklish,
    //                         touch_and_go, touchy);
    //              end;
    //
    // The enumerated constants are not defined within the record scope,
    // they are instead defined in the surrounding function, procedure,
    // program or unit scope.
    //
    {
        symbol_enumerated::pointer sep =
            boost::dynamic_pointer_cast<symbol_enumerated>(sp);
        if (sep)
        {
            scope::pointer scp = topmost_will_accept_enums();
            assert(scp);
            scp->define(name, sp, isa_param);
            DEBUG(1, "}");
            return;
        }
    }

    scope::pointer scp = topmost_non_temporary();
    assert(scp);
    scp->define(name, sp, isa_param);
    DEBUG(1, "}");
}


void
scope_stack::define(const symbol::pointer &sp, const bit_address &where)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    DEBUG(1, "sp = %p %s", sp.get(), sp->get_name().quote_c().c_str());
    DEBUG(2, "%s", representation().c_str());
    assert(length > 0);
    scope::pointer scp = list[length - 1];
    scp->define(sp, where);
    DEBUG(1, "}");
}


bit_address
scope_stack::allocate_space(const type::pointer &tp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    scope::pointer sp = topmost_non_temporary();
    assert(sp);
    return sp->allocate_space(tp);
}


symbol::pointer
scope_stack::lookup(const rcstring &s, scope::pointer *from_p)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    DEBUG(2, "%s", representation().c_str());
    rcstring name = pascal_id(s);
    for (size_t j = length; j > 0; --j)
    {
        scope::pointer scp = list[j - 1];
        symbol::pointer result = scp->lookup(name);
        if (result)
        {
            if (from_p)
                *from_p = scp;
            return result;
        }
        if (scp->do_not_search_past_this_point())
            break;
    }
    return symbol::pointer();
}


symbol::pointer
scope_stack::lookup_fuzzy(const rcstring &s, scope::pointer *from_p)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    rcstring name = pascal_id(s);
    double best_weight = 0.6;
    rcstring best_name;
    for (size_t j = length; j > 0; --j)
    {
        scope::pointer sp = list[j - 1];
        rcstring result = sp->lookup_fuzzy(name, best_weight);
        if (!result.empty())
            best_name = result;
        if (sp->do_not_search_past_this_point())
            break;
    }
    if (best_name.empty())
        return symbol::pointer();
    return lookup(best_name, from_p);
}


void
scope_stack::traversal(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(1, "this = %p", this);
    DEBUG(2, "%s", representation().c_str());
    assert(length >= 1);
    list[length - 1]->traversal();
}


void
scope_stack::attach_return_type(const type::pointer &tp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", representation().c_str());
    assert(length >= 1);
    list[length - 1]->attach_return_type(tp);
}


void
scope_stack::attach_body(const statement::pointer &sp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", representation().c_str());
    assert(length >= 1);
    list[length - 1]->attach_body(sp);
}


void
scope_stack::attach_function_parameters(name_type_list *ntlp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "%s", representation().c_str());
    assert(length >= 1);
    list[length - 1]->attach_function_parameters(ntlp);
}


void
scope_stack::begin_notify(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    list[length - 1]->begin_notify();
    DEBUG(1, "}");
}


void
scope_stack::constant_declaration_notify(const variable_name &name,
    const expression::pointer &value)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    DEBUG(2, "%s", representation().c_str());
    list[length - 1]->constant_declaration_notify(name, value);
}


void
scope_stack::variable_declaration_notify(const symbol_variable_p &svp)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    DEBUG(2, "%s", representation().c_str());
    list[length - 1]->variable_declaration_notify(svp);
}


void
scope_stack::label_declaration_notify(const variable_name_list &names)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    DEBUG(2, "%s", representation().c_str());
    list[length - 1]->label_declaration_notify(names);
}


scope::pointer
scope_stack::topmost_non_temporary(void)
    const
{
    size_t len = length;
    for (;;)
    {
        if (len == 0)
            return scope::pointer();
        scope::pointer sp = list[len - 1];
        if (!sp->is_temporary())
            return sp;
        --len;
    }
}


scope::pointer
scope_stack::topmost_will_accept_enums(void)
    const
{
    size_t len = length;
    for (;;)
    {
        if (len == 0)
            return scope::pointer();
        --len;
        scope::pointer sp = list[len];
        if (sp->will_accept_enums())
            return sp;
    }
}


bool
scope_stack::is_top_symbol(const symbol::pointer &sp)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    scope::pointer top = topmost_non_temporary();
    return (top && top->get_sp() == sp);
}


bool
scope_stack::function_is_on_scope_stack(const symbol::pointer &sy)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    for (size_t j = 0; j < length; ++j)
    {
        scope::pointer p = list[j];
        assert(p);
        if (!p->is_temporary() && p->get_sp() == sy)
            return true;
    }
    return false;
}


void
scope_stack::type_declaration_notify(const variable_name &name,
    const type::pointer &type)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    list[length - 1]->type_declaration_notify(name, type);
}


bit_address
scope_stack::get_data_mark(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    scope::pointer top = topmost_non_temporary();
    assert(top);
    return top->get_data_mark();
}


void
scope_stack::set_data_mark(const bit_address &arg)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    scope::pointer top = topmost_non_temporary();
    assert(top);
    top->set_data_mark(arg);
}


void
scope_stack::set_forward(bool ign_seg0)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    DEBUG(2, "%s", representation().c_str());
    list[length - 1]->set_forward(ign_seg0);
}


void
scope_stack::drop_locals(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(length >= 1);
    DEBUG(2, "%s", representation().c_str());
    list[length - 1]->drop_locals();
    DEBUG(1, "}");
}


bool
scope_stack::current_function_definition_was_forward(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    scope::pointer top = topmost_non_temporary();
    assert(top);
    return top->was_forward();
}


void
scope_stack::check_defined(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(length >= 1);
    DEBUG(2, "%s", representation().c_str());
    list[length - 1]->check_defined();
    DEBUG(1, "}");
}


void
scope_stack::late_binding(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(length >= 1);
    DEBUG(2, "%s", representation().c_str());
    list[length - 1]->late_binding();
    DEBUG(1, "}");
}


void
scope_stack::list_symbols(code_source_map &where)
    const
{
    assert(length >= 1);
    list[length - 1]->list_symbols(where);
}


bool
scope_stack::within_a_unit(void)
    const
{
    for (int j = (int)length - 1; j >= 0; --j)
    {
        if (list[j]->is_a_unit())
            return true;
    }
    return false;
}


link_info::pointer
scope_stack::get_link_info_extproc(void)
    const
{
    assert(length >= 1);
    return list[length - 1]->get_link_info_extproc();
}


symbol_variable::pointer
scope_stack::variable_factory(const variable_name &name,
    const type::pointer &tp, int lex_level)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    return list[length - 1]->variable_factory(name, tp, lex_level);
}


symbol_variable::pointer
scope_stack::parameter_factory(const variable_name &name,
    const type::pointer &tp, int lex_level)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    return list[length - 1]->parameter_factory(name, tp, lex_level);
}


symbol::pointer
scope_stack::constant_factory(const variable_name &name,
    int lex_level, const expression::pointer &value)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    return list[length - 1]->constant_factory(name, lex_level, value);
}


void
scope_stack::unit_interface_begin(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    return list[length - 1]->unit_interface_begin();
}


void
scope_stack::unit_interface_end(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    return list[length - 1]->unit_interface_end();
}


void
scope_stack::unit_implementation_begin(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    return list[length - 1]->unit_implementation_begin();
}


void
scope_stack::unit_implementation_end(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(length >= 1);
    return list[length - 1]->unit_implementation_end();
}


// vim: set ts=8 sw=4 et :

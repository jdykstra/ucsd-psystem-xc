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

#include <lib/cardinal.h>
#include <lib/expression/list.h>
#include <lib/type/integer.h>

#include <ucsdpsys_assemble/assembler.h>
#include <ucsdpsys_assemble/lex.h>
#include <ucsdpsys_assemble/opcode.h>


opcode::~opcode()
{
}


opcode::opcode(assembler *a_context) :
    context(a_context)
{
}


void
opcode::emit_byte(unsigned char value)
{
    context->emit_byte(value);
}


void
opcode::emit_byte(const expression::pointer &value)
{
    context->emit_byte(value);
}


void
opcode::emit_word(unsigned short value)
{
    context->emit_word(value);
}


void
opcode::emit_word(const expression::pointer &value)
{
    context->emit_word(value);
}


void
opcode::emit_big(unsigned short value)
{
    context->emit_big(value);
}


void
opcode::emit_big(const expression::pointer &value)
{
    context->emit_big(value);
}


void
opcode::emit_even_alignment(void)
{
    context->emit_even_alignment();
}


unsigned
opcode::get_address(void)
    const
{
    return context->get_address();
}


bool
opcode::check_argument_count(const expression_list &args, unsigned expect)
    const
{
    if (args.size() != expect)
    {
        lex_error
        (
            (args.empty() ? lex_location() : args.get_location()),
            "opcode %s: expected %s parameter%s, not %s",
            get_name().quote_c().c_str(),
            cardinal(expect).c_str(),
            (expect == 1 ? "" : "s"),
            cardinal(args.size()).c_str()
        );
        return false;
    }

    //
    // Also return false if any of the expressions are an error that
    // has been reported already.  This reduces the number of error
    // paths (and their cleanup) in the opcode implementations.
    //
    for (size_t j = 0; j < args.size(); ++j)
        if (args[j]->is_error())
            return false;

    // looks OK
    return true;
}


void
opcode::dot_architecture(const rcstring &name)
{
    context->dot_architecture(name);
}


void
opcode::dot_host(const rcstring &name)
{
    context->dot_host(name);
}


bool
opcode::check_argument_const_int(int argnum, const expression::pointer &ep,
    long lo, long hi) const
{
    if (!type_integer::is_a(ep) || !ep->is_constant())
    {
        lex_error
        (
            ep->get_location(),
            "opcode %s: parameter %s: require a constant integer value, "
                "not a %s",
            get_name().quote_c().c_str(),
            cardinal(argnum).c_str(),
            ep->get_description().c_str()
        );
        return false;
    }
    long n = ep->optimize()->get_integer_value();
    if (n < lo || n > hi)
    {
        lex_error
        (
            ep->get_location(),
            "opcode %s: parameter %s: value %ld is not in the range %ld..%ld",
            get_name().quote_c().c_str(),
            cardinal(argnum).c_str(),
            n,
            lo,
            hi
        );
        return false;
    }
    return true;
}


opcode::kind_t
opcode::get_kind(void)
    const
{
    return kind_standard;
}


bool
opcode::next_argument_always_a_string(void)
    const
{
    return false;
}


rcstring
opcode::trim_quotes(const rcstring &text)
{
    if (text.size() >= 2)
    {
        if (text.front() == '\'' && text.back() == '\'')
            return text.substring(1, text.size() - 2);
        if (text.front() == '"' && text.back() == '"')
            return text.substring(1, text.size() - 2);
        if (text.front() == '<' && text.back() == '>')
            return text.substring(1, text.size() - 2);
    }
    return text;
}


void
opcode::reloc_type1_lcrel(unsigned pc)
{
    context->reloc_type1_lcrel(pc);
}


void
opcode::reloc_type1_lcrel(void)
{
    context->reloc_type1_lcrel();
}


void
opcode::reloc_type2_ref(unsigned pc)
{
    context->reloc_type2_ref(pc);
}


void
opcode::reloc_type3_pubpriv(unsigned pc)
{
    context->reloc_type3_pubpriv(pc);
}


// vim: set ts=8 sw=4 et :

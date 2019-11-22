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

#include <lib/pcode.h>
#include <ucsdpsys_compile/label/compile.h>
#include <ucsdpsys_compile/translator/compile.h>


label_compile::~label_compile()
{
    assert(defined);
    assert(repair_length == 0);
    delete [] repair_list;
}


label_compile::label_compile(translator_compile *cntxt) :
    code(*cntxt),
    address(0),
    defined(false),
    jump_table_index(0),
    repair_length(0),
    repair_maximum(0),
    repair_list(0)
{
}


label_compile::pointer
label_compile::create(translator_compile *cntxt)
{
    return pointer(new label_compile(cntxt));
}


void
label_compile::define_here()
{
    assert(!defined);
    address = code.get_address();
    defined = true;

    if (repair_length)
        repair_now();
    self_relative_repair_now();
}


void
label_compile::goto_from_here()
{
    jump_from_here(UJP);
}


void
label_compile::branch_from_here_if_false()
{
    jump_from_here(FJP);
}


void
label_compile::branch_from_here_if_true()
{
    code.emit_byte(LNOT);
    jump_from_here(FJP);
}


void
label_compile::branch_from_here_if_equal()
{
    if (code.feature_efj_nfj())
    {
        // The opcode's name is confusing
        // NFJ = "test for Not equal, Jump if False"
        jump_from_here(NFJ);
    }
    else
    {
        code.emit_byte(NEQI);
        jump_from_here(FJP);
    }
}


void
label_compile::branch_from_here_if_not_equal()
{
    if (code.feature_efj_nfj())
    {
        // The opcode's name is confusing
        // EFJ = "test for Equal, Jump if False"
        jump_from_here(EFJ);
    }
    else
    {
        code.emit_byte(EQUI);
        jump_from_here(FJP);
    }
}


void
label_compile::jump_from_here(unsigned char jump_opcode)
{
    code.emit_byte(jump_opcode);
    code.emit_byte(0);

    // We do it in this order in case it repairs immediately.
    repair_subscribe(code.get_address());
}


void
label_compile::repair_subscribe(unsigned poke_addr)
{
    if (repair_length >= repair_maximum)
    {
        size_t new_repair_maximum = repair_maximum * 2 + 8;
        unsigned *new_repair_list = new unsigned [new_repair_maximum];
        for (size_t j = 0; j < repair_length; ++j)
            new_repair_list[j] = repair_list[j];
        delete [] repair_list;
        repair_list = new_repair_list;
        repair_maximum = new_repair_maximum;
    }
    repair_list[repair_length++] = poke_addr;

    if (defined)
        repair_now();
}


void
label_compile::repair_now()
{
    while (repair_length > 0)
    {
        --repair_length;
        unsigned poke_addr = repair_list[repair_length];

        //
        // Look for the simplest case, a short jump forwards.
        //
        if (address >= poke_addr)
        {
            int offset = address - poke_addr;
            if (offset < 128)
            {
                code.poke(poke_addr - 1, offset);
                continue;
            }
        }

        //
        // We need to go backwards, or we need to go a long way
        // forwards, so use the jump table.
        //
        // Strangely, even when we actually know the exact negative offset we
        // need, backwards jumps always indirect through the jump table.
        //
        // Speculation: Is the idea that forward jumps are pretty common
        // (e.g. "if" statements) but backwards jumps are less common
        // (only used by loops)?
        //
        if (jump_table_index == 0)
        {
            jump_table_index = code.new_jump_table_entry(address);
            assert(jump_table_index < 0);
            assert(jump_table_index >= -128);
        }
        code.poke(poke_addr - 1, jump_table_index & 0xFF);
    }
}


void
label_compile::self_relative_from_here(void)
{
    unsigned here = code.get_address();
    if (defined)
    {
        code.emit_word(here - address);
        return;
    }
    code.emit_word(0);
    self_relative_repair_subscribe(here);
}


void
label_compile::self_relative_repair_subscribe(unsigned poke_addr)
{
    assert(!defined);
    self_relative_repair.push_back(poke_addr);
}


void
label_compile::self_relative_repair_now()
{
    assert(defined);
    while (!self_relative_repair.empty())
    {
        unsigned poke_addr = self_relative_repair.back();
        self_relative_repair.pop_back();
        code.poke_word(poke_addr, poke_addr - address);
    }
}


bool
label_compile::is_defined(void)
    const
{
    return defined;
}


// vim: set ts=8 sw=4 et :

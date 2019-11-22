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

#include <ucsdpsys_assemble/personality/pseudo.h>
#include <ucsdpsys_assemble/opcode/pseudo/arch.h>
#include <ucsdpsys_assemble/opcode/pseudo/ascii.h>
#include <ucsdpsys_assemble/opcode/pseudo/asciz.h>
#include <ucsdpsys_assemble/opcode/pseudo/byte.h>
#include <ucsdpsys_assemble/opcode/pseudo/def.h>
#include <ucsdpsys_assemble/opcode/pseudo/error.h>
#include <ucsdpsys_assemble/opcode/pseudo/host.h>
#include <ucsdpsys_assemble/opcode/pseudo/include.h>
#include <ucsdpsys_assemble/opcode/pseudo/list.h>
#include <ucsdpsys_assemble/opcode/pseudo/nolist.h>
#include <ucsdpsys_assemble/opcode/pseudo/print.h>
#include <ucsdpsys_assemble/opcode/pseudo/radix.h>
#include <ucsdpsys_assemble/opcode/pseudo/ref.h>
#include <ucsdpsys_assemble/opcode/pseudo/sbttl.h>
#include <ucsdpsys_assemble/opcode/pseudo/title.h>
#include <ucsdpsys_assemble/opcode/pseudo/word.h>


personality_pseudo::~personality_pseudo()
{
}


personality_pseudo::personality_pseudo(assembler *context)
{
    // .a
    register_opcode(opcode_pseudo_arch::create(context));
    register_opcode(opcode_pseudo_ascii::create(context));
    register_opcode(opcode_pseudo_asciz::create(context));

    // .b
    opcode::pointer dot_byte = opcode_pseudo_byte::create(context);
    register_opcode(dot_byte);

    // .c
    // .d
    register_opcode("db", dot_byte);
    register_opcode(opcode_pseudo_def::create(context));
    opcode::pointer dot_word = opcode_pseudo_word::create(context);
    register_opcode("dw", dot_word);

    // .e
    register_opcode(opcode_pseudo_error::create(context));

    // .f
    // .g
    // .h
    register_opcode(opcode_pseudo_host::create(context));

    // .i
    opcode::pointer dot_include = opcode_pseudo_include::create(context);
    register_opcode(dot_include);
    register_opcode("include", dot_include);

    // .j
    // .k
    // .l
    register_opcode(opcode_pseudo_list::create(context));

    // .m
    // .n
    register_opcode(opcode_pseudo_nolist::create(context));

    // .o
    // .p
    register_opcode(opcode_pseudo_print::create(context));

    // .q
    // .r
    register_opcode(opcode_pseudo_radix::create(context));
    register_opcode(opcode_pseudo_ref::create(context));

    // .s
    register_opcode(opcode_pseudo_sbttl::create(context));

    // .t
    register_opcode(opcode_pseudo_title::create(context));

    // .u
    // .v
    // .w
    register_opcode(dot_word);

    // .x
    // .y
    // .z
}


personality_pseudo::pointer
personality_pseudo::create(assembler *context)
{
    return pointer(new personality_pseudo(context));
}


byte_sex_t
personality_pseudo::get_byte_sex(void)
    const
{
    assert(!"should never be called");
    // Doesn't matter what you choose.
    return little_endian;
}


mtype_t
personality_pseudo::get_mtype(void)
    const
{
    assert(!"should never be called");
    // Doesn't matter what you choose.
    return mtype_undefined;
}


unsigned char
personality_pseudo::get_one_byte_nop(void)
    const
{
    return 0;
}


// vim: set ts=8 sw=4 et :

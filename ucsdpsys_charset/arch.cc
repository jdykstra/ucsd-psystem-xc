//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#include <lib/ac/ctype.h>
#include <lib/ac/stdio.h>
#include <libexplain/output.h>

#include <ucsdpsys_charset/arch.h>


arch::~arch()
{
}


arch::arch(mtype_t a_mtype) :
    mtype(a_mtype)
{
}


rcstring
arch::get_binary_format_name(void)
    const
{
    return "apple";
}


rcstring
arch::get_text_format_name(void)
    const
{
    return "text";
}


// vim: set ts=8 sw=4 et :

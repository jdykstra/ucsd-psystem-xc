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

#include <ucsdpsys_charset/arch/terak.h>


arch_terak::~arch_terak()
{
}


arch_terak::arch_terak(mtype_t a_mtype) :
    arch(a_mtype)
{
}


arch_terak::pointer
arch_terak::create(mtype_t a_mtype)
{
    return pointer(new arch_terak(a_mtype));
}


rcstring
arch_terak::get_binary_format_name(void)
    const
{
    return "terak.charset";
}


// vim: set ts=8 sw=4 et :

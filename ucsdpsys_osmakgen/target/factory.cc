//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#include <lib/config.h>
#include <libexplain/output.h>

#include <ucsdpsys_osmakgen/target/build/cook.h>
#include <ucsdpsys_osmakgen/target/build/makefile.h>
#include <ucsdpsys_osmakgen/target/debian.h>


target::pointer
target::factory(const rcstring &filename)
{
    if (filename.gmatch("*cook*"))
        return target_build_cook::create(filename);
    if (filename.downcase().gmatch("*makefile*"))
        return target_build_makefile::create(filename);
    if (filename.gmatch("*debian*"))
        return target_debian::create(filename);
    explain_output_error_and_die
    (
        "do not know how to generate a %s file",
        filename.quote_c().c_str()
    );
    return pointer();
}


// vim: set ts=8 sw=4 et :

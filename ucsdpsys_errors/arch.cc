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

#include <lib/config.h>
#include <libexplain/output.h>

#include <ucsdpsys_errors/arch.h>


arch::~arch()
{
}


arch::arch(mtype_t a_mtype) :
    mtype(a_mtype)
{
}


void
arch::encode(const rcstring &infile, const rcstring &outfile)
{
    read_text(infile);
    write_binary(outfile);
}


void
arch::decode(const rcstring &infile, const rcstring &outfile)
{
    read_binary(infile);
    write_text(outfile);
}


void
arch::no_function_specified(const rcstring &, const rcstring &)
{
    explain_output_error_and_die("no function specified on command line");
}


// vim: set ts=8 sw=4 et :

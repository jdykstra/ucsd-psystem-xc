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

#include <ucsdpsys_opcodes/arch.h>


arch::~arch()
{
}


arch::arch(mtype_t a_mtype) :
    mtype(a_mtype),
    byte_sex(byte_sex_from_mtype(a_mtype))
{
}


void
arch::put_word(unsigned char *data, unsigned value)
    const
{
    byte_sex_put_word(byte_sex, data, value);
}


unsigned
arch::get_word(const unsigned char *data)
    const
{
    return byte_sex_get_word(byte_sex, data);
}


void
arch::flip_byte_sex(void)
{
    byte_sex =
        (byte_sex_t)(((int)big_endian + (int)little_endian) - (int)byte_sex);
}


void
arch::no_function_specified(const rcstring &, const rcstring &)
{
    explain_output_error_and_die("no function specified on the command line");
}


void
arch::encode(const rcstring &ifn, const rcstring &ofn)
{
    read_text(ifn);
    write_binary(ofn);
}


void
arch::decode(const rcstring &ifn, const rcstring &ofn)
{
    read_binary(ifn);
    write_text(ofn);
}


void
arch::random_test_data(const rcstring &, const rcstring &ofn)
{
    random_test_data();
    write_text(ofn);
}


rcstring
arch::rname(void)
{
    int n = 2 + random() % 6;
    char buf[8];
    for (int j = 0; j < n; ++j)
        buf[j] = 'A' + random() % 26;
    return rcstring(buf, n);
};


// vim: set ts=8 sw=4 et :

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

#include <lib/ac/stdio.h>
#include <libexplain/program_name.h>

#include <lib/version.h>
#include <lib/version_stamp.h>


void
version_print(void)
{
    const char *prog = explain_program_name_get();
    printf("%s version %s\n", prog, version_stamp());
    printf("Copyright (C) %s Peter Miller\n", copyright_years());
    printf("\n");
    printf("The %s program comes with ABSOLUTELY NO WARRANTY;\n", prog);
    printf("for details see the GNU General Public License.  The\n");
    printf("%s program is free software, and you are welcome to\n", prog);
    printf("redistribute it under certain conditions; for details\n");
    printf("see the GNU General Public License.\n");
}


// vim: set ts=8 sw=4 et :

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


#include <lib/ac/stdio.h>
#include <lib/ac/stdlib.h>
#include <lib/ac/signal.h>
#include <execinfo.h>
#include <unistd.h>

#include <lib/segfault.h>
#include <lib/sizeof.h>


#ifdef HAVE_BACKTRACE

static void
segfault(int)
{
    void *buffer[200];
    int nptrs = backtrace(buffer, SIZEOF(buffer));
    backtrace_symbols_fd(buffer, nptrs, STDERR_FILENO);
    exit(1);
}


void
set_segfault_handler(void)
{
    static struct sigaction zero;
    struct sigaction sa(zero);
    sa.sa_handler = segfault;
    sigaction(SIGSEGV, &sa, 0);
    sigaction(SIGABRT, &sa, 0);
}

#else

void
set_segfault_handler(void)
{
}

#endif


// vim: set ts=8 sw=4 et :

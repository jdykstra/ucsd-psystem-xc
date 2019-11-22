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

#include <lib/label/placebo.h>


label_placebo::~label_placebo()
{
}


label_placebo::label_placebo() :
    defined_flag(false)
{
}


label_placebo::pointer
label_placebo::create(void)
{
    return pointer(new label_placebo());
}


void
label_placebo::define_here(void)
{
    defined_flag = true;
}


void
label_placebo::goto_from_here(void)
{
    // Do nothing.
}


void
label_placebo::self_relative_from_here(void)
{
    // Do nothing.
}


void
label_placebo::branch_from_here_if_false(void)
{
    // Do nothing.
}


void
label_placebo::branch_from_here_if_true(void)
{
    // Do nothing.
}


void
label_placebo::branch_from_here_if_equal(void)
{
    // Do nothing.
}


void
label_placebo::branch_from_here_if_not_equal(void)
{
    // Do nothing.
}


bool
label_placebo::is_defined(void)
    const
{
    return defined_flag;
}


// vim: set ts=8 sw=4 et :

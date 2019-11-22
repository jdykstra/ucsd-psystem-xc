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

#include <lib/codefile/filter/notice.h>


codefile_filter_notice::~codefile_filter_notice()
{
}


codefile_filter_notice::codefile_filter_notice(
    const codefile::pointer &a_deeper,
    const rcstring &a_notice
) :
    codefile_filter(a_deeper),
    notice(a_notice)
{
}


codefile_filter_notice::pointer
codefile_filter_notice::create(const codefile::pointer &a_deeper,
    const rcstring &a_notice)
{
    return pointer(new codefile_filter_notice(a_deeper, a_notice));
}


void
codefile_filter_notice::set_copyright_notice(rcstring const&)
{
    // Ignore.
}


rcstring
codefile_filter_notice::get_copyright_notice()
    const
{
    return notice;
}


// vim: set ts=8 sw=4 et :

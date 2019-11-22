#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at
# you option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program. If not, see <http://www.gnu.org/licenses/>
#
unset LANGUAGE
unset LC_ALL
LANG=C
export LANG
cat << 'fubar'
<html>
<body>
<h1>
<img src="icon64.png" align=left>
<img src="icon64.png" align=right>
<center>Source Code Size</center></h1>
The information on this page is produced by the <i>sloccount</i> program
by David A. Wheeler <dwheeler@dwheeler.com>.
<p>
For more information see the SLOCCount website
at <a href="http://www.dwheeler.com/sloccount.">
http://www.dwheeler.com/sloccount</a>.  Note that more detailed
documentation is available both on the website and with the SLOCCount
package.
<pre>
fubar

here=`pwd`
test $? -eq 0 || exit 1

temp=$here/.sloc
rm -rf $temp
test $? -eq 0 || exit 1
mkdir $temp
test $? -eq 0 || exit 1

fail() {
    cd $here
    rm -rf $temp
    exit 1
}

trap "fail" 1 2 3 15

sloccount --datadir $temp "$@" | sed '1,/Computing results/d'

echo '</pre>'
echo 'This page last updated'
date
echo '</body></html>'
exit 0
# vim: set ts=8 sw=4 et :

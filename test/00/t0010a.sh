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

TEST_SUBJECT="compile variable references"
. test_prelude

#
# compile variable references
#
cat > test.in << 'fubar'
program variables;var v1:integer;v2:integer;v3:integer;v4:
integer;v5:integer;v6:integer;v7:integer;v8:integer;v9:
integer;v10:integer;v11:integer;v12:integer;v13:integer;v14:
integer;v15:integer;v16:integer;v17:integer;v18:integer;v19:
integer;begin writeln(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,
v12,v13,v14,v15,v16,v17,v18,v19)end.
fubar
test $? -eq 0 || no_result

cat > test.ok << 'fubar'
program variables;
var
    v1: integer;
    v2: integer;
    v3: integer;
    v4: integer;
    v5: integer;
    v6: integer;
    v7: integer;
    v8: integer;
    v9: integer;
    v10: integer;
    v11: integer;
    v12: integer;
    v13: integer;
    v14: integer;
    v15: integer;
    v16: integer;
    v17: integer;
    v18: integer;
    v19: integer;
begin
    writeln(v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15,
        v16, v17, v18, v19)
end.
fubar
test $? -eq 0 || no_result

$bindir/ucsdpsys_pretty test.in > test.out
test $? -eq 0 || fail

diff test.ok test.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :

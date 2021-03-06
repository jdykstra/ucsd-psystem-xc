#!/bin/sh
#
# UCSD p-System cross compiler
# Copyright (C) 2010-2012 Peter Miller
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

TEST_SUBJECT="ucsdpsys_osmakgen vs assembler errors"
. test_prelude

mkdir -p arch/z80/assembler
test $? -eq 0 || no_result

touch arch/z80/assembler/main.text
test $? -eq 0 || no_result

TAB=`awk 'BEGIN{printf("%c",9)}' /dev/null`

sed "s|{TAB}|$TAB|g" > ok << 'fubar'
#
# This file is generated by the ucsdpsys_osmakgen(1) command, from the
# ucsd-psystem-xc project.  The ucsd-psystem-xc project is distributed under the
# terms and conditions of the GNU GPL version 3.  As a special exception, the
# contents of this file are not subject to those terms and conditions.
#

# values provided by the ./configure script
prefix = @prefix@
exec_prefix = @exec_prefix@
datadir = $(DESTDIR)@datadir@
datarootdir = $(DESTDIR)@datarootdir@
sysconfdir = $(DESTDIR)@sysconfdir@

all: stage1.disks

host = klebsch

arch := $(shell ucsdpsys_osmakgen --arch-from-host $(host))

# cross compiler flags
XCFLAGS = --arch=$(arch) -Wno-shadow

filenames = arch/z80/assembler/error-messages.text \
{TAB}{TAB}arch/z80/assembler/main.text arch/z80/assembler/opcode-data.text

stage1/codefiles/arch/z80/assembler/z80.errors: \
{TAB}{TAB}arch/z80/assembler/error-messages.text
{TAB}@mkdir -p stage1/codefiles/arch/z80/assembler
{TAB}ucsdpsys_errors --arch=z80 arch/z80/assembler/error-messages.text \
{TAB}{TAB}stage1/codefiles/arch/z80/assembler/z80.errors

stage2/codefiles/arch/z80/assembler/z80.errors: \
{TAB}{TAB}arch/z80/assembler/error-messages.text
{TAB}@mkdir -p stage2/codefiles/arch/z80/assembler
{TAB}ucsdpsys_errors --arch=z80 arch/z80/assembler/error-messages.text \
{TAB}{TAB}stage2/codefiles/arch/z80/assembler/z80.errors

stage3/codefiles/arch/z80/assembler/z80.errors: \
{TAB}{TAB}arch/z80/assembler/error-messages.text
{TAB}@mkdir -p stage3/codefiles/arch/z80/assembler
{TAB}ucsdpsys_errors --arch=z80 arch/z80/assembler/error-messages.text \
{TAB}{TAB}stage3/codefiles/arch/z80/assembler/z80.errors

ifeq ($(host),z80)

stage1/$(host)/codefiles/arch/z80/assembler/main.code: \
{TAB}{TAB}arch/z80/assembler/main.text
{TAB}@mkdir -p stage1/$(host)/codefiles/arch/z80/assembler
{TAB}ucsdpsys_compile $(XCFLAGS) -fignore-undefined-segment-zero \
{TAB}{TAB}-Iarch/z80/assembler -o \
{TAB}{TAB}stage1/$(host)/codefiles/arch/z80/assembler/main.code \
{TAB}{TAB}arch/z80/assembler/main.text
{TAB}ucsdpsys_librarian \
{TAB}{TAB}--file=stage1/$(host)/codefiles/arch/z80/assembler/main.code \
{TAB}{TAB}--remove-system-segments
endif

stage1/$(host)/codefiles/assembler/z80.opcodes: \
{TAB}{TAB}arch/z80/assembler/opcode-data.text
{TAB}@mkdir -p stage1/$(host)/codefiles/assembler
{TAB}ucsdpsys_opcodes --arch=z80 arch/z80/assembler/opcode-data.text \
{TAB}{TAB}stage1/$(host)/codefiles/assembler/z80.opcodes

stage2/$(host)/codefiles/assembler/z80.opcodes: \
{TAB}{TAB}arch/z80/assembler/opcode-data.text
{TAB}@mkdir -p stage2/$(host)/codefiles/assembler
{TAB}ucsdpsys_opcodes --arch=z80 arch/z80/assembler/opcode-data.text \
{TAB}{TAB}stage2/$(host)/codefiles/assembler/z80.opcodes

stage3/$(host)/codefiles/assembler/z80.opcodes: \
{TAB}{TAB}arch/z80/assembler/opcode-data.text
{TAB}@mkdir -p stage3/$(host)/codefiles/assembler
{TAB}ucsdpsys_opcodes --arch=z80 arch/z80/assembler/opcode-data.text \
{TAB}{TAB}stage3/$(host)/codefiles/assembler/z80.opcodes

stage1.disks: stage1/$(host)/system.vol

stage2.disks: stage2/$(host)/system.vol

stage3.disks: stage3/$(host)/system.vol

stage1/system/system.assmbler: stage1/codefiles/arch/$(arch)/assembler/main.code
{TAB}@mkdir -p stage1/system
{TAB}cp stage1/codefiles/arch/$(arch)/assembler/main.code \
{TAB}{TAB}stage1/system/system.assmbler

stage2/system/system.assmbler: stage2/codefiles/arch/$(arch)/assembler/main.code
{TAB}@mkdir -p stage2/system
{TAB}cp stage2/codefiles/arch/$(arch)/assembler/main.code \
{TAB}{TAB}stage2/system/system.assmbler

stage3/system/system.assmbler: stage3/codefiles/arch/$(arch)/assembler/main.code
{TAB}@mkdir -p stage3/system
{TAB}cp stage3/codefiles/arch/$(arch)/assembler/main.code \
{TAB}{TAB}stage3/system/system.assmbler

ifneq ($(wildcard arch/$(arch)/assembler/main.text),)
stage1_assembler_code = stage1/$(host)/system/system.assmbler
stage2_assembler_code = stage2/$(host)/system/system.assmbler
stage3_assembler_code = stage3/$(host)/system/system.assmbler
else
stage1_assembler_code =
stage2_assembler_code =
stage3_assembler_code =
endif

stage1/$(host)/system/$(arch).errors: \
{TAB}{TAB}stage1/$(host)/codefiles/assembler/$(arch).errors
{TAB}@mkdir -p stage1/$(host)/system
{TAB}cp stage1/$(host)/codefiles/assembler/$(arch).errors \
{TAB}{TAB}stage1/$(host)/system/$(arch).errors

stage2/$(host)/system/$(arch).errors: \
{TAB}{TAB}stage2/$(host)/codefiles/assembler/$(arch).errors
{TAB}@mkdir -p stage2/$(host)/system
{TAB}cp stage2/$(host)/codefiles/assembler/$(arch).errors \
{TAB}{TAB}stage2/$(host)/system/$(arch).errors

stage3/$(host)/system/$(arch).errors: \
{TAB}{TAB}stage3/$(host)/codefiles/assembler/$(arch).errors
{TAB}@mkdir -p stage3/$(host)/system
{TAB}cp stage3/$(host)/codefiles/assembler/$(arch).errors \
{TAB}{TAB}stage3/$(host)/system/$(arch).errors

ifneq ($(wildcard arch/$(arch)/assembler/error-messages.text),)
stage1_assembler_error_data = stage1/system/$(arch).errors
stage2_assembler_error_data = stage2/system/$(arch).errors
stage3_assembler_error_data = stage3/system/$(arch).errors
else
stage1_assembler_error_data =
stage2_assembler_error_data =
stage3_assembler_error_data =
endif

stage1/system/$(arch).opcodes: \
{TAB}{TAB}stage1/codefiles/arch/$(arch)/assembler/$(arch).opcodes
{TAB}@mkdir -p stage1/system
{TAB}cp stage1/codefiles/arch/$(arch)/assembler/$(arch).opcodes \
{TAB}{TAB}stage1/system/$(arch).opcodes

stage2/system/$(arch).opcodes: \
{TAB}{TAB}stage2/codefiles/arch/$(arch)/assembler/$(arch).opcodes
{TAB}@mkdir -p stage2/system
{TAB}cp stage2/codefiles/arch/$(arch)/assembler/$(arch).opcodes \
{TAB}{TAB}stage2/system/$(arch).opcodes

stage3/system/$(arch).opcodes: \
{TAB}{TAB}stage3/codefiles/arch/$(arch)/assembler/$(arch).opcodes
{TAB}@mkdir -p stage3/system
{TAB}cp stage3/codefiles/arch/$(arch)/assembler/$(arch).opcodes \
{TAB}{TAB}stage3/system/$(arch).opcodes

ifneq ($(wildcard arch/$(arch)/assembler/opcode-data.text),)
stage1_assembler_opcode_data = stage1/system/$(arch).opcodes
stage2_assembler_opcode_data = stage2/system/$(arch).opcodes
stage3_assembler_opcode_data = stage3/system/$(arch).opcodes
else
stage1_assembler_opcode_data =
stage2_assembler_opcode_data =
stage3_assembler_opcode_data =
endif

# build stage one system disk image

stage1/$(host)/system.vol: $(stage1_assembler_code) \
{TAB}{TAB}$(stage1_assembler_error_data) $(stage1_assembler_opcode_data)
{TAB}@mkdir -p stage1/$(host)
{TAB}ucsdpsys_mkfs --arch=$(arch) -B256 --label=system \
{TAB}{TAB}stage1/$(host)/system.vol
{TAB}ucsdpsys_disk --file stage1/$(host)/system.vol --text --put \
{TAB}{TAB}$(stage1_assembler_code) $(stage1_assembler_error_data) \
{TAB}{TAB}$(stage1_assembler_opcode_data)
{TAB}ucsdpsys_disk --file stage1/$(host)/system.vol --crunch --list \
{TAB}{TAB}--sort=name

# build stage two system disk image

stage2/$(host)/system.vol: $(stage2_assembler_code) \
{TAB}{TAB}$(stage2_assembler_error_data) $(stage2_assembler_opcode_data)
{TAB}@mkdir -p stage2/$(host)
{TAB}ucsdpsys_mkfs --arch=$(arch) -B256 --label=system \
{TAB}{TAB}stage2/$(host)/system.vol
{TAB}ucsdpsys_disk --file stage2/$(host)/system.vol --text --put \
{TAB}{TAB}$(stage2_assembler_code) $(stage2_assembler_error_data) \
{TAB}{TAB}$(stage2_assembler_opcode_data)
{TAB}ucsdpsys_disk --file stage2/$(host)/system.vol --crunch --list \
{TAB}{TAB}--sort=name

# build stage three system disk image

stage3/$(host)/system.vol: $(stage3_assembler_code) \
{TAB}{TAB}$(stage3_assembler_error_data) $(stage3_assembler_opcode_data)
{TAB}@mkdir -p stage3/$(host)
{TAB}ucsdpsys_mkfs --arch=$(arch) -B256 --label=system \
{TAB}{TAB}stage3/$(host)/system.vol
{TAB}ucsdpsys_disk --file stage3/$(host)/system.vol --text --put \
{TAB}{TAB}$(stage3_assembler_code) $(stage3_assembler_error_data) \
{TAB}{TAB}$(stage3_assembler_opcode_data)
{TAB}ucsdpsys_disk --file stage3/$(host)/system.vol --crunch --list \
{TAB}{TAB}--sort=name

clean: clean-files
clean-files:
{TAB}rm -f stage1/$(host)/codefiles/arch/z80/assembler/main.code
{TAB}rm -f stage1/$(host)/codefiles/assembler/z80.opcodes
{TAB}rm -f stage1/$(host)/system.vol stage1/$(host)/system/$(arch).errors
{TAB}rm -f stage1/codefiles/arch/z80/assembler/z80.errors
{TAB}rm -f stage1/system/$(arch).opcodes stage1/system/system.assmbler
{TAB}rm -f stage2/$(host)/codefiles/assembler/z80.opcodes
{TAB}rm -f stage2/$(host)/system.vol stage2/$(host)/system/$(arch).errors
{TAB}rm -f stage2/codefiles/arch/z80/assembler/z80.errors
{TAB}rm -f stage2/system/$(arch).opcodes stage2/system/system.assmbler
{TAB}rm -f stage3/$(host)/codefiles/assembler/z80.opcodes
{TAB}rm -f stage3/$(host)/system.vol stage3/$(host)/system/$(arch).errors
{TAB}rm -f stage3/codefiles/arch/z80/assembler/z80.errors
{TAB}rm -f stage3/system/$(arch).opcodes stage3/system/system.assmbler

distclean: clean dist-clean-files dist-clean-dirs
dist-clean-files:
{TAB}rm -f Makefile config.log config.status
dist-clean-dirs:
{TAB}rm -rf stage1

rebuild-makefile:
{TAB}ucsdpsys_osmakgen $(filenames)

install: stage1.install

stage1.install: stage1.disks
{TAB}mkdir -p $(datadir)/ucsd-psystem-os
{TAB}cp -r stage1/$(host)/diagnostic $(datadir)/ucsd-psystem-os/diagnostic
{TAB}cp stage1/$(host)/diagnostic.vol \
{TAB}{TAB}$(datadir)/ucsd-psystem-os/diagnostic.vol
{TAB}cp -r stage1/$(host)/system $(datadir)/ucsd-psystem-os/system
{TAB}cp stage1/$(host)/system.vol $(datadir)/ucsd-psystem-os/system.vol
{TAB}cp -r stage1/$(host)/utility $(datadir)/ucsd-psystem-os/utility
{TAB}cp stage1/$(host)/utility.vol $(datadir)/ucsd-psystem-os/utility.vol

stage2.install: stage2.disks
{TAB}mkdir -p $(datadir)/ucsd-psystem-os
{TAB}cp -r stage2/$(host)/diagnostic $(datadir)/ucsd-psystem-os/diagnostic
{TAB}cp stage2/$(host)/diagnostic.vol \
{TAB}{TAB}$(datadir)/ucsd-psystem-os/diagnostic.vol
{TAB}cp -r stage2/$(host)/system $(datadir)/ucsd-psystem-os/system
{TAB}cp stage2/$(host)/system.vol $(datadir)/ucsd-psystem-os/system.vol
{TAB}cp -r stage2/$(host)/utility $(datadir)/ucsd-psystem-os/utility
{TAB}cp stage2/$(host)/utility.vol $(datadir)/ucsd-psystem-os/utility.vol

stage3.install: stage3.disks
{TAB}mkdir -p $(datadir)/ucsd-psystem-os
{TAB}cp -r stage3/$(host)/diagnostic $(datadir)/ucsd-psystem-os/diagnostic
{TAB}cp stage3/$(host)/diagnostic.vol \
{TAB}{TAB}$(datadir)/ucsd-psystem-os/diagnostic.vol
{TAB}cp -r stage3/$(host)/system $(datadir)/ucsd-psystem-os/system
{TAB}cp stage3/$(host)/system.vol $(datadir)/ucsd-psystem-os/system.vol
{TAB}cp -r stage3/$(host)/utility $(datadir)/ucsd-psystem-os/utility
{TAB}cp stage3/$(host)/utility.vol $(datadir)/ucsd-psystem-os/utility.vol
fubar
test $? -eq 0 || no_result

ucsdpsys_osmakgen -o test.makefile.out -b arch/z80/assembler/main.text \
        arch/z80/assembler/error-messages.text \
        arch/z80/assembler/opcode-data.text
test $? -eq 0 || fail

diff ok test.makefile.out
test $? -eq 0 || fail

#
# The functionality exercised by this test worked.
# No other assertions are made.
#
pass
# vim: set ts=8 sw=4 et :

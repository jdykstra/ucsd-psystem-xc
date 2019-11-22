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

#ifndef LIB_LIBDIR_H
#define LIB_LIBDIR_H

/**
  * The configured_bindir function is used to obtain the executable
  * binaries directory determined by the ./configure script.
  */
const char *configured_bindir(void);

/**
  * The configured_libdir function is used to obtain the library
  * determined by the ./configure script.
  */
const char *configured_libdir(void);

/**
  * The configured_datadir function is used to obtain the data directory
  * determined by the ./configure script.
  */
const char *configured_datadir(void);

/**
  * The configured_datarootdir function is used to obtain the root data
  * directory determined by the ./configure script.
  */
const char *configured_datarootdir(void);

/**
  * The configured_sysconfdir function is used to obtain the system
  * configuration directory determined by the ./configure script.
  */
const char *configured_sysconfdir(void);

#endif // LIB_LIBDIR_H
// vim: set ts=8 sw=4 et :

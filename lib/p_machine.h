//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#ifndef LIB_P_MACHINE_H
#define LIB_P_MACHINE_H

#include <lib/rcstring.h>

enum p_machine_t
{
    p_machine_ii_1,
    p_machine_ii_0,
    p_machine_i_5,
    p_machine_i_3,
};

/**
  * The p_machine_set function is used to set the system idea of the
  * p-machine release of interest.
  *
  * @param value
  *     The desired p-machine release.
  *
  * @note
  *     It is an error to set the p-machine release more than once.
  */
void p_machine_set(p_machine_t value);

/**
  * The p_machine_set function is used to set the system idea of the
  * p-machine release of interest.  This is the form most often used
  * when processing command line arguments..
  *
  * @param value
  *     The desired p-machine release.
  *
  * @note
  *     It is an error to set the p-machine release more than once.  If
  *     the p-machine value cannot be determined, this function will
  *     print an error message, and exit rather than return.
  */
void p_machine_set(const rcstring &value);

/**
  * The p_machine_get function is used to obtain the system idea of the
  * p-machine release of interest.
  */
p_machine_t p_machine_get(void);

/**
  * The p_machine_name_uc function may be used to turn a p-machine
  * release value into a human readable string, in upper case.
  *
  * @param value
  *     The p-machine release of interest
  * @returns
  *     A string containing the human readable release name, with all
  *     the letters (if any) in upper case.
  */
rcstring p_machine_name(p_machine_t value);

/**
  * The p_machine_set_explicitly function may be used to determine
  * whether or not the p-machine release has been set explicitly,
  * usually as the result of a command line option.
  */
bool p_machine_set_explicitly(void);

#endif // LIB_P_MACHINE_H
// vim: set ts=8 sw=4 et :

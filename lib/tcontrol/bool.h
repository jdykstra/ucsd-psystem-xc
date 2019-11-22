//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <http://www.gnu.org/licenses/>
//

#ifndef LIB_OPTION_BOOL_H
#define LIB_OPTION_BOOL_H

#include <lib/tcontrol.h>

/**
  * The tcontrol_bool class is used to represent the processing necessary
  * to deal with boolean tcontrols.
  */
class tcontrol_bool:
    public tcontrol
{
public:
    /**
      * The destructor.
      */
    virtual ~tcontrol_bool();

protected:
    /**
      * The default constructor.
      * For use be derived classes only.
      */
    tcontrol_bool();

    /**
      * The looks_boolean class method is used to examine a string
      * for boolesan-ness.
      *
      * @param arg
      *     The string to examine
      * @returns
      *     true if the string looks like a boolean,
      *     false if the string does not match any of the boolean patterns.
      */
    static bool looks_boolean(const rcstring &arg);

    /**
      * The check_value_implicit method verifies that the parameter
      * list is acceptable, and issues an error message if there is a
      * problem.
      *
      * @param args
      *     The parameters to the tcontrol.  There should be zero or one.
      * @param dflt
      *     The default value to be used of there is a problem, or no
      *     value is supplied.
      * @returns
      *     The boolean value corresponding to the args, dflt if there
      *     was an error, or there were no args.
      */
    bool check_value_implicit(const rcstring_list &args, bool dflt);

    /**
      * The check_value method varifies that the parameter list is
      * acceptable, and issues an error message if there is a problem.
      *
      * @param args
      *     The parameters to the tcontrol.  There should be exactly one.
      * @param dflt
      *     The default value to be used of there is a problem.
      * @returns
      *     The boolean value corresponding to the args, dflt if there
      *     was an error, or there were no args.
      */
    bool check_value(const rcstring_list &args, bool dflt);

    /**
      * The copy constructor.   Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_bool(const tcontrol_bool &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_bool &operator=(const tcontrol_bool &rhs);
};

#endif // LIB_OPTION_BOOL_H
// vim: set ts=8 sw=4 et :

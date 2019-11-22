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

#ifndef LIB_TCONTROL_NOARG_H
#define LIB_TCONTROL_NOARG_H

#include <lib/tcontrol.h>

/**
  * The tcontrol_noarg class is used to represent a tcontrol that takes
  * exactly zero parameters.
  */
class tcontrol_noarg:
    public tcontrol
{
public:
    typedef boost::shared_ptr<tcontrol_noarg> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol_noarg();

protected:
    /**
      * The default constructor.
      * For use by defined classes only.
      */
    tcontrol_noarg();

    /**
      * The check value method is used to confirm that the parameter list
      * is is of the correct form.
      *
      * @param args
      *     The parameter list to be checked.
      * @returns
      *     true if no problems, false if any problems
      */
    bool check_value(const rcstring_list &args);

private:
    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_noarg(const tcontrol_noarg &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_noarg &operator=(const tcontrol_noarg &rhs);
};

#endif // LIB_TCONTROL_NOARG_H
// vim: set ts=8 sw=4 et :

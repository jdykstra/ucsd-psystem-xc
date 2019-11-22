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

#ifndef LIB_TCONTROL_STRING_H
#define LIB_TCONTROL_STRING_H

#include <lib/tcontrol.h>

/**
  * The tcontrol_string class is used to represent an abstract control
  * that takes a single string as its parameter.
  */
class tcontrol_string:
    public tcontrol
{
public:
    typedef boost::shared_ptr<tcontrol_string> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol_string();

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    tcontrol_string();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_string(const tcontrol_string &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_string &operator=(const tcontrol_string &rhs);

    /**
      * The check_value method is sued to check that the parameters list
      * is in an acceptable form.
      *
      * @param args
      *     The parameters passed to the #set method.
      * @returns
      *     true if parameters look OK,
      *     false if the parameters had a problem.
      */
    bool check_value(const rcstring_list &args);
};

#endif // LIB_TCONTROL_STRING_H
// vim: set ts=8 sw=4 et :

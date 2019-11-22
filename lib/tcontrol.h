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

#ifndef LIB_TCONTROL_H
#define LIB_TCONTROL_H

#include <boost/shared_ptr.hpp>

#include <lib/rcstring/list.h>

/**
  * The tcontrol abstract class is used to represent an option that can be
  * set, usually by (*$something*) but also from the command line.
  * The tcontrol could be anything (bool, integer, etc).
  *
  * My first choice for the name of the class hierarchy was "option",
  * but that conflicts with <getopt.h>
  */
class tcontrol
{
public:
    typedef boost::shared_ptr<tcontrol> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol();

    /**
      * The set method is used to set the tcontrol.
      *
      * @param args
      *     The parameters to be set, usually args[0] is the name and
      *     args[1] is the value to be set.  Should print an error if
      *     too many or too few args are given.
      */
    virtual void set(const rcstring_list &args) = 0;

    /**
      * The set method is a helper.  It split the strings at white
      * space, and tenn calls the string list variant.
      *
      * @param args
      *     The parameters to the tcontrol setting.
      */
    void set(const rcstring &args);

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    tcontrol();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol(const tcontrol &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol &operator=(const tcontrol &rhs);
};

#endif // LIB_TCONTROL_H
// vim: set ts=8 sw=4 et :

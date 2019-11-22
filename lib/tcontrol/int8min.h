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

#ifndef LIB_TCONTROL_INT8MIN_H
#define LIB_TCONTROL_INT8MIN_H

#include <lib/tcontrol.h>

/**
  * The tcontrol_int8min class is used to represent a control for
  * setting the pascal-id maximum length.
  */
class tcontrol_int8min:
    public tcontrol
{
public:
    typedef boost::shared_ptr<tcontrol_int8min> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol_int8min();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param cntrl
      *     The control to be set.
      */
    static pointer create(int &cntrl);

protected:
    // See base class for documentation.
    void set(const rcstring_list &args);

private:
    /**
      * The nctrl instanve variable i sused to rememeber the location of
      * the control to be set.
      */
    int &cntrl;

    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param cntrl
      *     The control to be set.
      */
    tcontrol_int8min(int &cntrl);

    /**
      * The default constructor.  Do not use.
      */
    tcontrol_int8min();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_int8min(const tcontrol_int8min &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_int8min &operator=(const tcontrol_int8min &rhs);
};

#endif // LIB_TCONTROL_INT8MIN_H
// vim: set ts=8 sw=4 et :

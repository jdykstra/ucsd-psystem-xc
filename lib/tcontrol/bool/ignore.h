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

#ifndef LIB_TCONTROL_BOOL_IGNORE_H
#define LIB_TCONTROL_BOOL_IGNORE_H

#include <lib/tcontrol/bool.h>

/**
  * The tcontrol_bool_ignore class is used to represent an on/off toggle
  * that is ignored, at the moment.
  */
class tcontrol_bool_ignore:
    public tcontrol_bool
{
public:
    typedef boost::shared_ptr<tcontrol_bool_ignore> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol_bool_ignore();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

protected:
    // See base class for documentation.
    void set(const rcstring_list &args);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    tcontrol_bool_ignore();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_bool_ignore(const tcontrol_bool_ignore &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_bool_ignore &operator=(const tcontrol_bool_ignore &rhs);
};

#endif // LIB_TCONTROL_BOOL_IGNORE_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TCONTROL_BOOL_AMBIGUOUS_H
#define LIB_TCONTROL_BOOL_AMBIGUOUS_H

#include <lib/tcontrol/bool.h>

/**
  * The tcontrol_bool_ambiguous class is used to represent an ambiguous
  * control that could be one thing if the argumkent is boolean, or it
  * could be another non-boolean thing.
  */
class tcontrol_bool_ambiguous:
    public tcontrol_bool
{
public:
    typedef boost::shared_ptr<tcontrol_bool_ambiguous> pointer;

    /**
      * The destructor.
      */
    virtual ~tcontrol_bool_ambiguous();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param bool_case
      *     The control to set of the parameter is boolean
      * @param non_bool_case
      *     The control to set of the parameter is not boolean
      */
    static pointer create(const tcontrol::pointer &bool_case,
        const tcontrol::pointer &non_bool_case);

protected:
    // See base class for documentation.
    void set(const rcstring_list &args);

private:
    /**
      * The bool_case instance variable is used to remember The control
      * to set of the parameter is boolean
      */
    tcontrol::pointer bool_case;

    /**
      * The bool_case instance variable is used to remember the control
      * to set of the parameter is not boolean
      */
    tcontrol::pointer non_bool_case;

    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param bool_case
      *     The control to set of the parameter is boolean
      * @param non_bool_case
      *     The control to set of the parameter is not boolean
      */
    tcontrol_bool_ambiguous(const tcontrol::pointer &bool_case,
        const tcontrol::pointer &non_bool_case);

    /**
      * The default constructor.  Do not use.
      */
    tcontrol_bool_ambiguous();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_bool_ambiguous(const tcontrol_bool_ambiguous &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_bool_ambiguous &operator=(const tcontrol_bool_ambiguous &rhs);
};

#endif // LIB_TCONTROL_BOOL_AMBIGUOUS_H
// vim: set ts=8 sw=4 et :

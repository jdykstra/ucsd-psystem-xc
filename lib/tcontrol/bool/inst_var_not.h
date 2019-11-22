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

#ifndef LIB_OPTION_BOOL_INST_VAR_NOT_H
#define LIB_OPTION_BOOL_INST_VAR_NOT_H

#include <lib/tcontrol/bool.h>

/**
  * The tcontrol_bool_inst_var_not class is used to represent a boolean
  * tcontrol that is an instance variable of a class.  The actual value
  * given will be inverted.
  *
  * For example, (*$feature little-endian true*) is a symonym for
  * (*$feature big-endian false*) and this class is used to implement
  * it.
  */
template <class T>
class tcontrol_bool_inst_var_not:
    public tcontrol_bool
{
public:
    /**
      * The destructor.
      */
    virtual ~tcontrol_bool_inst_var_not() { }

    /**
      * The create class method i sused to create new dynamically
      * allocated instances of this class.
      *
      * @param a_object
      *     The object to operate on
      * @param a_instvar
      *     The instance variable within the object.
      */
    static pointer
    create(T *a_object, bool T::*a_instvar)
    {
        return pointer(new tcontrol_bool_inst_var_not(a_object, a_instvar));
    }

    // See base class for documentation.
    void
    set(const rcstring_list &args)
    {
        object->*instvar = !check_value(args, true);
    }

private:
    /**
      * The object instance variable is used to remember the object we
      * are going to work on.
      */
    T *object;

    /**
      * the instvar instance variable is used to remember the instance
      * variable we are going to work on.
      */
    bool T::*instvar;

    /**
      * The constructor.
      *
      * @param a_object
      *     The object to operate on
      * @param a_instvar
      *     The instance variable within the object.
      */
    tcontrol_bool_inst_var_not(T *a_object, bool T::*a_instvar) :
        object(a_object),
        instvar(a_instvar)
    {
    }

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    tcontrol_bool_inst_var_not(const tcontrol_bool_inst_var_not &rhs) :
        tcontrol_bool(rhs)
    {
    }

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    tcontrol_bool_inst_var_not &
    operator=(const tcontrol_bool_inst_var_not &rhs)
    {
        if (this != &rhs)
        {
            tcontrol_bool::operator=(rhs);
        }
        return *this;
    }
};

#endif // LIB_OPTION_BOOL_INST_VAR_NOT_H
// vim: set ts=8 sw=4 et :

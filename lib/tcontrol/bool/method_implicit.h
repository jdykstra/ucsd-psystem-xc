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

#ifndef LIB_TCONTROL_BOOL_METHOD_IMPLICIT_H
#define LIB_TCONTROL_BOOL_METHOD_IMPLICIT_H

#include <lib/tcontrol/bool.h>

/**
  * The tcontrol_bool_method_implicit class is used to represent a
  * control that calls a method, if no boolean value supplied, defaults
  * to true.
  */
template <class T>
class tcontrol_bool_method_implicit:
    public tcontrol_bool
{
public:
    typedef boost::shared_ptr<tcontrol_bool_method_implicit> pointer;

    typedef void (T::*method_t)(bool);

    /**
      * The destructor.
      */
    virtual ~tcontrol_bool_method_implicit() { }

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param a_object
      *     The object on which the method call is to be made
      * @param a_method
      *     The method of the object to be called.
      */
    static pointer
    create(T *a_object, method_t a_method)
    {
        return pointer(new tcontrol_bool_method_implicit(a_object, a_method));
    }

protected:
    // See base class for documentation.
    void
    set(const rcstring_list &args)
    {
        (object->*method)(check_value_implicit(args, true));
    }

private:
    /**
      * The object instance variable is used to remember the object on
      * which the method call is to be made
      */
    T *object;

    /**
      * The method instance variable is used to remember the method of
      * the object to be called.
      */
    method_t method;

    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param a_object
      *     The object on which the method call is to be made
      * @param a_method
      *     The method of the object to be called.
      */
    tcontrol_bool_method_implicit(T *a_object, method_t a_method) :
        object(a_object),
        method(a_method)
    {
    }
};

#endif // LIB_TCONTROL_BOOL_METHOD_IMPLICIT_H
// vim: set ts=8 sw=4 et :

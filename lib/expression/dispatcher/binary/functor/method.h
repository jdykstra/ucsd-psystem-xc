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

#ifndef LIB_EXPRESSION_DISPATCHER_BINARY_FUNCTOR_METHOD_H
#define LIB_EXPRESSION_DISPATCHER_BINARY_FUNCTOR_METHOD_H

#include <lib/expression/dispatcher/binary/functor.h>

/**
  * The expression_dispatcher_binary_functor_method class is used to represent
  * a method to be called on a class, once a match has been found.
  */
template <class T>
class expression_dispatcher_binary_functor_method:
    public expression_dispatcher_binary_functor
{
public:
    typedef boost::shared_ptr<expression_dispatcher_binary_functor_method>
        pointer;

    /**
      * The destructor.
      */
    virtual ~expression_dispatcher_binary_functor_method() { }

    typedef expression::pointer (T::*method_p)(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer
    create(T *a_obj, method_p a_method)
    {
        return
            pointer
            (
                new expression_dispatcher_binary_functor_method(a_obj, a_method)
            );
    }

    // See base class for documentation.
    expression::pointer
    action(const expression::pointer &lhs, const expression::pointer &rhs)
        const
    {
        return (obj->*method)(lhs, rhs);
    }

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    expression_dispatcher_binary_functor_method(T *a_obj, method_p a_method) :
        obj(a_obj),
        method(a_method)
    {
    }

    /**
      * The obj instance variable is used to remember the object who's
      * method is to be called.
      */
    T *obj;

    /**
      * The method instance variable is used to remember which of the
      * object's method is to be called.
      */
    method_p method;
};

#endif // LIB_EXPRESSION_DISPATCHER_BINARY_FUNCTOR_METHOD_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TYPE_LONG_INTEGER_SIZED_H
#define LIB_TYPE_LONG_INTEGER_SIZED_H

#include <lib/type/long_integer.h>

/**
  * The type_long_integer_sized class is used to represent an
  * INTEGER[] type with an variable number of words, BUT the number of
  * words has been push onto the stack along with the value.
  */
class type_long_integer_sized:
    public type_long_integer
{
public:
    typedef boost::shared_ptr<type_long_integer_sized> pointer;

    /**
      * The destructor.
      */
    virtual ~type_long_integer_sized();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    /**
      * The is_a class method is used to determine whether or not the
      * given type is a long integer.
      *
      * @param tp
      *     The type to be tested.
      */
    static bool is_a(const type::pointer &tp);

    /**
      * The is_a class method is used to determine wherther or not the
      * given type is a long integer.
      *
      * @param ep
      *     The expression who's type is to be tested.
      */
    static bool is_a(const expression::pointer &ep);

protected:
    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    unsigned get_size_in_words(void) const;

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    type_long_integer_sized();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    type_long_integer_sized(const type_long_integer_sized &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    type_long_integer_sized &operator=(const type_long_integer_sized &rhs);
};

#endif // LIB_TYPE_LONG_INTEGER_SIZED_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TYPE_POINTER_BYTE_H
#define LIB_TYPE_POINTER_BYTE_H

#include <lib/type/pointer.h>

/**
  * The type_pointer_byte class is used to represent a two-word value
  * on the stack.  The first is a word pointer to the base of an array
  * of bytes, the second is an index into the byte array (it could be a
  * packed array of char, it could be a string, etc).
  */
class type_pointer_byte:
    public type_pointer
{
public:
    typedef boost::shared_ptr<type_pointer_byte> pointer;

    /**
      * The destructor.
      */
    virtual ~type_pointer_byte();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const type::pointer &sub);

    /**
      * The is_a class method is used to determine whether a given type
      * is a pointer type.
      */
    static bool is_a(const type::pointer &candidate);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    type_pointer_byte(const type::pointer &sub);

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

private:
    /**
      * The default constructor.  Do not use.
      */
    type_pointer_byte();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    type_pointer_byte(const type_pointer_byte &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    type_pointer_byte &operator=(const type_pointer_byte &rhs);
};

#endif // LIB_TYPE_POINTER_BYTE_H
// vim: set ts=8 sw=4 et :

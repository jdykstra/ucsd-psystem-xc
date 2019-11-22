//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#ifndef LIB_TYPE_POINTER_PACKED_H
#define LIB_TYPE_POINTER_PACKED_H

#include <lib/type/pointer.h>

/**
  * The type_pointer_packed class is used to represent the type of a
  * value which is a 3-word pointer to a packed data item.
  */
class type_pointer_packed:
    public type_pointer
{
public:
    typedef boost::shared_ptr<type_pointer_packed> pointer;

    /**
      * The destructor.
      */
    virtual ~type_pointer_packed();

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      *
      * @param subtype
      *     The type of symbol being pointed at.
      */
    type_pointer_packed(const type::pointer &subtype);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param subtype
      *     The type of symbol being pointed at.
      */
    static pointer create(const type::pointer &subtype);

    /**
      * The is_a class method is used to determine whether a given type
      * is a packed pointer type.
      */
    static bool is_a(const type::pointer &arg);

protected:
    // See base class for documentation.
    unsigned get_size_in_bits() const;

    // See base class for documentation.
    bool is_a_variable() const;

private:
    /**
      * The subtype instance variable is used to remember the type of
      * the value being pointed at.
      */
    type::pointer subtype;

    /**
      * The default constructor.  Do not use.
      */
    type_pointer_packed();

    /**
      * The copy constructor.  Do not use.
      */
    type_pointer_packed(const type_pointer_packed &);

    /**
      * The assignment operator.  Do not use.
      */
    type_pointer_packed &operator=(const type_pointer_packed &);
};

#endif // LIB_TYPE_POINTER_PACKED_H
// vim: set ts=8 sw=4 et :

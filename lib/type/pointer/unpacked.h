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

#ifndef LIB_TYPE_POINTER_UNPACKED_H
#define LIB_TYPE_POINTER_UNPACKED_H

#include <lib/type/pointer.h>

/**
  * The type_pointer_unpacked class is used to represent an unpacked pointer
  * type (1 word).
  */
class type_pointer_unpacked:
    public type_pointer
{
public:
    typedef boost::shared_ptr<type_pointer_unpacked> pointer;

    /**
      * The destructor.
      */
    virtual ~type_pointer_unpacked();

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      */
    type_pointer_unpacked(const type::pointer &arg);

public:
    /**
      * The create class method is used to creatre new dynamically
      * allocated instances of this class.
      */
    static pointer create(const type::pointer &arg);

    /**
      * The is_a class method is used to determine whether a given type
      * is an unpacked pointer type.
      */
    static bool is_a(const type::pointer &candidate);

protected:
    // See base class for documentation.
    unsigned get_size_in_bits() const;

    // See base class for documentation.
    type::pointer late_binding_or_null() const;

private:
    /**
      * The default constructor.  Do not use.
      */
    type_pointer_unpacked();

    /**
      * The copy constructor.  Do not use.
      */
    type_pointer_unpacked(const type_pointer_unpacked &);

    /**
      * The assignment operator.  Do not use.
      */
    type_pointer_unpacked &operator=(const type_pointer_unpacked &);
};

#endif // LIB_TYPE_POINTER_UNPACKED_H
// vim: set ts=8 sw=4 et :

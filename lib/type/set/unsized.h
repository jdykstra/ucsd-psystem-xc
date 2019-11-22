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

#ifndef LIB_TYPE_SET_UNSIZED_H
#define LIB_TYPE_SET_UNSIZED_H

#include <lib/type/set.h>

/**
  * The type_set_unsized class is used to represent a set value which
  * does not include the size of the set.
  */
class type_set_unsized:
    public type_set
{
public:
    typedef boost::shared_ptr<type_set_unsized> pointer;

    /**
      * The destructor.
      */
    virtual ~type_set_unsized();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param subtype
      *     The types of the things in the set.
      */
    static pointer create(const type::pointer &subtype);

    /**
      * The get_sized_equiv method is used to obtain the equivalent
      * sized set type.
      */
    type::pointer get_sized_equiv(void) const;

    /**
      * The is_a method is used to determine if the given type is
      * a set type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a sized set type, false if not
      */
    static bool is_a(const type::pointer &arg);

protected:
    // See base class for documentation.
    unsigned get_size_in_bits() const;

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      *
      * @param subtype
      *     The types of the things in the set.
      */
    type_set_unsized(const type::pointer &subtype);

    /**
      * The default constructor.  Do not use.
      */
    type_set_unsized();

    /**
      * The copy constructor.  Do not use.
      */
    type_set_unsized(const type_set_unsized &);

    /**
      * The assignment operator.  Do not use.
      */
    type_set_unsized &operator=(const type_set_unsized &);
};

#endif // LIB_TYPE_SET_UNSIZED_H
// vim: set ts=8 sw=4 et :

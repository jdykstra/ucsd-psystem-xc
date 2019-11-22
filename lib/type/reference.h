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

#ifndef LIB_TYPE_REFERENCE_H
#define LIB_TYPE_REFERENCE_H

#include <lib/type.h>

/**
  * The type_reference class is used to represent a reference to some
  * other type.  It appears on the stack as a one-word pointer.
  */
class type_reference:
    public type
{
public:
    typedef boost::shared_ptr<type_reference> pointer;

    /**
      * The destructor.
      */
    virtual ~type_reference();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param subtype
      *     The type being referenced.
      */
    static pointer create(const type::pointer &subtype);

    /**
      * The is_a method is used to determine if the given expression is
      * of reference type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a reference type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a method is used to determine if the given expression is
      * of reference type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a reference type, false if not
      */
    static bool is_a(const type *arg);

protected:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method on purpose.
      *
      * @param subtype
      *     The type being referenced.
      */
    type_reference(const type::pointer &subtype);

    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    rcstring get_pascal_name(void) const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

    // See base class for documentation.
    int get_minimum_value(void) const;

    // See base class for documentation.
    int get_maximum_value(void) const;

    // See base class for documentation.
    bool may_be_used_as_array_index(void) const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The subtype instance variable is used to remember the type of
      * variable being referenced.
      */
    type::pointer subtype;

    /**
      * The default constructor.
      */
    type_reference();

    /**
      * The copy constructor.
      */
    type_reference(const type_reference &);

    /**
      * The assignment operator.
      */
    type_reference &operator=(const type_reference &);
};

#endif // LIB_TYPE_REFERENCE_H
// vim: set ts=8 sw=4 et :

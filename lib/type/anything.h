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

#ifndef LIB_TYPE_ANYTHING_H
#define LIB_TYPE_ANYTHING_H

#include <lib/type.h>

/**
  * The type_anything class is used to represent a type that matches
  * any other type.  It is useful for declaring system calls with very
  * liberal parameters.  This type is inaccessable through the grammar.
  *
  * Examples include system call parameters such as
  *     file of anything
  *     reference to anything
  *     pointer to anything
  */
class type_anything:
    public type
{
public:
    typedef boost::shared_ptr<type_anything> pointer;

    /**
      * The destructor.
      */
    virtual ~type_anything();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    /**
      * The is-a class method is used to test a type to see if it is the
      * "anything" type.
      */
    static bool is_a(const type::pointer &candidate);

protected:
    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    rcstring get_pascal_name(void) const;

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    type_anything();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    type_anything(const type_anything &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    type_anything &operator=(const type_anything &rhs);
};

#endif // LIB_TYPE_ANYTHING_H
// vim: set ts=8 sw=4 et :

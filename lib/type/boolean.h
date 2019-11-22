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

#ifndef LIB_TYPE_BOOLEAN_H
#define LIB_TYPE_BOOLEAN_H

#include <lib/type.h>
#include <lib/expression.h>

/**
  * The type_boolean class is used to represent a 1-bit boolean (true or
  * false) type.
  */
class type_boolean:
    public type
{
public:
    typedef boost::shared_ptr<type_boolean> pointer;

    /**
      * The destructor.
      */
    virtual ~type_boolean();

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * "create" class method instead.
      */
    type_boolean();

public:
    /**
      * The factory class method is used to get access to the singleton.
      */
    static pointer create();

    /**
      * The is_a method is used to determine if the given expression is
      * of boolean type.
      *
      * @param arg
      *     The expression to be tested
      * @returns
      *     bool; true if it is a boolean expression, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of boolean type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a boolean type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of boolean type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a boolean type, false if not
      */
    static bool is_a(const type *arg);

protected:
    // See base class for documentation.
    rcstring get_name() const;

    // See base class for documentation.
    rcstring get_pascal_name() const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    unsigned get_size_in_bits() const;

    // See base class for documentation.
    bool is_a_variable() const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    type::pointer get_subtype() const;

    // See base class for documentation.
    int get_minimum_value() const;

    // See base class for documentation.
    int get_maximum_value() const;

    // See base class for documentation.
    bool may_be_used_as_array_index() const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The copy constructor.  Do not use.
      */
    type_boolean(const type_boolean &);

    /**
      * The assignment operator.  Do not use.
      */
    type_boolean &operator=(const type_boolean &);
};

#endif // LIB_TYPE_BOOLEAN_H
// vim: set ts=8 sw=4 et :

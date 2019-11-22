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

#ifndef LIB_TYPE_INTEGER_H
#define LIB_TYPE_INTEGER_H

#include <lib/type.h>
#include <lib/expression.h>

/**
  * The type_integer class is used to represent two-byte (16 bit) value.
  */
class type_integer:
    public type
{
public:
    typedef boost::shared_ptr<type_integer> pointer;

    /**
      * The destructor.
      */
    virtual ~type_integer();

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * "create" class method instead.
      */
    type_integer();

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    /**
      * The is_a method is used to determine if the given expression is
      * of integer type, or an integer subrange type.
      *
      * @param arg
      *     The expression to be tested
      * @returns
      *     bool; true if it is an integer expression, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of integer type, or an integer subrange type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is an integer type, false if not
      */
    static bool is_a(const type::pointer &arg);

protected:
    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    rcstring get_pascal_name(void) const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    int get_minimum_value(void) const;

    // See base class for documentation.
    int get_maximum_value(void) const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The copy constructor.  Do not use.
      */
    type_integer(const type_integer &);

    /**
      * The assignment operator.  Do not use.
      */
    type_integer &operator=(const type_integer &);
};

#endif // LIB_TYPE_INTEGER_H
// vim: set ts=8 sw=4 et :

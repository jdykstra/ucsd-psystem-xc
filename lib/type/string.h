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

#ifndef LIB_TYPE_STRING_H
#define LIB_TYPE_STRING_H

#include <lib/type.h>
#include <lib/expression.h>

/**
  * The type_string class is used to represent multi-byte string value.
  * Note: UCSD Pascal strings have a length byte, and then that many
  * bytes of text follow.  This is nothing like C strings.
  */
class type_string:
    public type
{
public:
    typedef boost::shared_ptr<type_string> pointer;

    /**
      * The destructor.
      */
    virtual ~type_string();

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      *
      * @param size
      *     The size of the string.  Must be in the range 1..255
      */
    type_string(int size);

public:
    enum { default_size = 80 };

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param size
      *     The size of the string.  Must be in the range 1..255
      */
    static pointer create(int size = default_size);

    int get_size(void) const { return size; }

    /**
      * The is_a method is used to determine if the given expression is
      * of string type.
      *
      * @param arg
      *     The expression to be tested
      * @returns
      *     bool; true if it is a string-valued expression, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of string type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a string-valued expression, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of string type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a string-valued expression, false if not
      */
    static bool is_a(const type *arg);

protected:
    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    rcstring get_pascal_name(void) const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The size instance variable is used to remember the size of the
      * string.  The size of a string is the maximum string length that
      * it can hold.  The size must be in the range 1 to 255.
      */
    int size;

    /**
      * The default constructor.  Do not use.
      */
    type_string();

    /**
      * The copy constructor.  Do not use.
      */
    type_string(const type_string &);

    /**
      * The assignment operator.  Do not use.
      */
    type_string &operator=(const type_string &);
};

#endif // LIB_TYPE_STRING_H
// vim: set ts=8 sw=4 et :

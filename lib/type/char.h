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

#ifndef LIB_TYPE_CHAR_H
#define LIB_TYPE_CHAR_H

#include <lib/type.h>
#include <lib/expression.h>

/**
  * The type_char class is used to represent an ASCII character (8 bit) value.
  */
class type_char:
    public type
{
public:
    typedef boost::shared_ptr<type_char> pointer;

    /**
      * The destructor.
      */
    virtual ~type_char();

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * #create class method instead.
      *
      * @param interactive
      *     whether or not file char is "interactive", for the purposes of
      *     "interactive = file of char" declarations.
      */
    type_char(bool interactive = false);

public:
    /**
      * The create class method is used to create new
      * dynamically allocated instances of this class.
      */
    static pointer create();

    /**
      * The create class method is used to create new
      * dynamically allocated instances of this class.
      *
      * This is used explicitly in the
      *     type interactive = file of char;
      * declaration in lib/translator/builtin.cc There is (intentionally)
      * no way for source code to declare a char type of this kind.
      */
    static pointer create_interactive();

    /**
      * The is_a method is used to determine if the given expression is
      * of char type, or a char subrange.
      *
      * @param arg
      *     The expression to be tested
      * @returns
      *     bool; true if it is an char expression, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of char type, or a char subrange.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is an char type, false if not
      */
    static bool is_a(const type::pointer &arg);

protected:
    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

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
    type::pointer get_subtype() const;

    // See base class for documentation.
    int get_minimum_value() const;

    // See base class for documentation.
    int get_maximum_value() const;

    // See base class for documentation.
    bool may_be_used_as_array_index() const;

    // See base class for documentation.
    int get_finit_recwords() const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The interactive instance variable is used to remember whether or
      * not file char is "interactive", for the purposes of "interactive
      * = file of char" declarations.
      */
    bool interactive;

    /**
      * The copy constructor.  Do not use.
      */
    type_char(const type_char &);

    /**
      * The assignment operator.  Do not use.
      */
    type_char &operator=(const type_char &);
};

#endif // LIB_TYPE_CHAR_H
// vim: set ts=8 sw=4 et :

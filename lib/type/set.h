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

#ifndef LIB_TYPE_SET_H
#define LIB_TYPE_SET_H

#include <lib/expression.h>
#include <lib/type.h>

/**
  * The type_set class is used to represent a data type which is a
  * set of values (integer subrange, char, char subrange, enum, enum
  * subrange).
  */
class type_set:
    public type
{
public:
    typedef boost::shared_ptr<type_set> pointer;

    /**
      * The destructor.
      */
    virtual ~type_set();

protected:
    /**
      * The constructor.
      * Thou shalt derive from this class.
      */
    type_set(const type::pointer &subtype);

public:
    /**
      * The is_a class method is used to determine if the given type is
      * a set type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a set type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a class method is used to determine if the given type is
      * a set type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a set type, false if not
      */
    static bool is_a(const type *arg);

    /**
      * The is_a class method is used to determine if the given
      * expression is of set type.
      *
      * @param arg
      *     The expression to be tested
      * @returns
      *     bool; true if it is a set expression, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The bounded class method is used to check the subtype to make sure it
      * falls within the 0..maximum range interval.  If it is too big (e.g. an
      * integer, or a huge subrange) the result will be a smaller subrange type.
      */
    static type::pointer bounded(const type::pointer &tp);

protected:
    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    rcstring get_pascal_name(void) const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

    // See base class for documentation.
    void name_hint(const rcstring &name);

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The subtype instance variable is used to remember the type this
      * is a set of.
      */
    type::pointer subtype;

    /**
      * The default constructor.  Do not use.
      */
    type_set();

    /**
      * The copy constructor.  Do not use.
      */
    type_set(const type_set &);

    /**
      * The assignment operator.  Do not use.
      */
    type_set &operator=(const type_set &);
};

#endif // LIB_TYPE_SET_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TYPE_LONG_INTEGER_H
#define LIB_TYPE_LONG_INTEGER_H

#include <lib/expression.h>
#include <lib/type.h>

/**
  * The type_long_integer class is used to represent the long integer
  * (radix 10K) integer representation.
  */
class type_long_integer:
    public type
{
public:
    typedef boost::shared_ptr<type_long_integer> pointer;

    enum { max_digits = 36 };

    /**
      * The destructor.
      */
    virtual ~type_long_integer();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param ndig
      *     The number of decimal digits (1..36)
      */
    static pointer create(int ndig);

    /**
      * The is_a class method is used to determine wherther or not the
      * given type is a long integer.
      *
      * @param tp
      *     The type to be tested.
      */
    static bool is_a(const type::pointer &tp);

    /**
      * The is_a class method is used to determine wherther or not the
      * given type is a long integer.
      *
      * @param ep
      *     The expression who's type is to be tested.
      */
    static bool is_a(const expression::pointer &ep);

protected:
    /**
      * The default constructor.  Do not use.
      * For use by derived clases only.
      */
    type_long_integer();

    // See base class for documentation.
    rcstring get_pascal_name(void) const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    type_long_integer(const type_long_integer &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    type_long_integer &operator=(const type_long_integer &rhs);
};

#endif // LIB_TYPE_LONG_INTEGER_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TYPE_LONG_INTEGER_UNSIZED_H
#define LIB_TYPE_LONG_INTEGER_UNSIZED_H

#include <lib/type/long_integer.h>

/**
  * The type_long_integer_unsized class is used to represent an
  * INTEGER[n] type with a fixed number of words.
  */
class type_long_integer_unsized:
    public type_long_integer
{
public:
    typedef boost::shared_ptr<type_long_integer_unsized> pointer;

    // This is an arbitrary limit imposed by the UBSD native compiler,
    // not any particular deficiency of the representation.
    enum { max_digits = 36 };

    /**
      * The destructor.
      */
    virtual ~type_long_integer_unsized();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param ndig
      *     The number of decimal digits (1..36)
      */
    static pointer create(int ndig);

    /**
      * The is_a class method is used to determine whether or not the
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
    // See base class for documentation.
    rcstring get_name(void) const;

    // See base class for documentation.
    unsigned get_size_in_words(void) const;

    // See base class for documentation.
    unsigned get_size_in_bits(void) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param ndig
      *     The number of decimal digits (1..36)
      */
    type_long_integer_unsized(int ndig);

    /**
      * The nwords instance variable is used ot remember the number of
      * 16-bit words used to store the type's value.
      */
    unsigned nwords;

    /**
      * The default constructor.  Do not use.
      */
    type_long_integer_unsized();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    type_long_integer_unsized(const type_long_integer_unsized &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    type_long_integer_unsized &operator=(const type_long_integer_unsized &rhs);
};

#endif // LIB_TYPE_LONG_INTEGER_UNSIZED_H
// vim: set ts=8 sw=4 et :

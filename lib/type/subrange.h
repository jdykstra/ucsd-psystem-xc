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

#ifndef LIB_TYPE_SUBRANGE_H
#define LIB_TYPE_SUBRANGE_H

#include <lib/type.h>

/**
  * The type_subrange class is used to represent
  */
class type_subrange:
    public type
{
public:
    typedef boost::shared_ptr<type_subrange> pointer;

    /**
      * The destructor.
      */
    virtual ~type_subrange();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param minimum
      *     The least value in the enum
      * @param maximum
      *     The greatest value in the enum
      * @param subtype
      *     The base type of which this enum is a derived type.
      */
    static pointer create(int minimum, int maximum,
        const type::pointer &subtype);

    /**
      * The is_a method is used to determine if the given type is
      * of subrange type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a subrange type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * of subrange type.
      *
      * @param arg
      *     The type to be tested
      * @returns
      *     bool; true if it is a subrange type, false if not
      */
    static bool is_a(const type *arg);

    // See base class for documentation.
    int get_minimum_value() const;

    // See base class for documentation.
    int get_maximum_value() const;

    // See base class for documentation.
    unsigned get_size_in_bits() const;

    // See base class for documentation.
    type::pointer get_subtype() const;

    // See base class for documentation.
    rcstring get_name() const;

    // See base class for documentation.
    rcstring get_pascal_name() const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    void name_hint(const rcstring &name);

    // See base class for documentation.
    bool is_a_variable() const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool may_be_used_as_array_index() const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      *
      * @param minimum
      *     The least value in the enum
      * @param maximum
      *     The greatest value in the enum
      * @param subtype
      *     The base type of which this enum is a derived type.
      */
    type_subrange(int minimum, int maximum, const type::pointer &subtype);

    int minimum;
    int maximum;
    type::pointer subtype;
    rcstring name;

    /**
      * The default constructor.  Do not use.
      */
    type_subrange();

    /**
      * The copy constructor.  Do not use.
      */
    type_subrange(const type_subrange &);

    /**
      * The assignment operator.  Do not use.
      */
    type_subrange &operator=(const type_subrange &);
};

#endif // LIB_TYPE_SUBRANGE_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TYPE_UNIT_H
#define LIB_TYPE_UNIT_H

#include <lib/type.h>

/**
  * The type_unit class is used to represent a "unit" type for the
  * unit symbol.
  */
class type_unit:
    public type
{
public:
    typedef boost::shared_ptr<type_unit> pointer;

    /**
      * The destructor.
      */
    virtual ~type_unit();

    /**
      * The create method is used to obtain access to the unit class
      * singleton instance.
      */
    static pointer create(void);

    /**
      * The is_a class method is used to test whether the given type is
      * the unit type.
      *
      * @param rhs
      *     The type to be tested.
      * @returns
      *     true if is unit type, false if any other type
      */
    static bool is_a(const type::pointer &rhs);

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
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable() const;

    // See base class for documentation.
    type::pointer get_subtype() const ;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The default constructor.  It is private on purpose, use the
      * #create class method instead.
      */
    type_unit();

    /**
      * The copy constructor.
      */
    type_unit(const type_unit &);

    /**
      * The assignment operator.
      */
    type_unit &operator=(const type_unit &);
};

#endif // LIB_TYPE_UNIT_H
// vim: set ts=8 sw=4 et :

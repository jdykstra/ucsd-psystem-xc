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

#ifndef LIB_TYPE_ERROR_H
#define LIB_TYPE_ERROR_H

#include <lib/type.h>

/**
  * The type_error class is used to represent an abstract error, meaning
  * the type cannot be determined.
  */
class type_error:
    public type
{
public:
    typedef boost::shared_ptr<type_error> pointer;

    /**
      * The destructor.
      */
    virtual ~type_error();

private:
    /**
      * The default constructor.  It is private on purpose, user the
      * "create" class method instead.
      */
    type_error();

public:
    /**
      * The create method is used to create new dynamically allocated
      * instances of this class.
      */
    static pointer create(void);

    /**
      * The is_a class method is used to determine whether a given type
      * is an error type.
      */
    static bool is_a(const type::pointer &candidate);

    /**
      * The is_a class method is used to determine whether a given type
      * is an error type.
      */
    static bool is_a(const type *candidate);

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
      * The copy constructor.  Do not use.
      */
    type_error(const type_error &);

    /**
      * The assignment operator.  Do not use.
      */
    type_error &operator=(const type_error &);
};

#endif // LIB_TYPE_ERROR_H
// vim: set ts=8 sw=4 et :

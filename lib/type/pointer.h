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

#ifndef LIB_TYPE_POINTER_H
#define LIB_TYPE_POINTER_H

#include <lib/expression.h>
#include <lib/type.h>

/**
  * The type_pointer class is used to represent a pointer to some
  * other type.  It appears on the stack as a one-word pointer.
  */
class type_pointer:
    public type
{
public:
    typedef boost::shared_ptr<type_pointer> pointer;

    /**
      * The destructor.
      */
    virtual ~type_pointer();

    /**
      * The is_a class method is used to determine whether a given
      * expression is of a pointer type.
      */
    static bool is_a(const expression::pointer &candidate);

    /**
      * The is_a class method is used to determine whether a given type
      * is a pointer type.
      */
    static bool is_a(const type::pointer &candidate);

    /**
      * The is_a class method is used to determine whether a given type
      * is a pointer type.
      */
    static bool is_a(const type *candidate);

    // See base class for documentation.
    type::pointer get_subtype() const;

protected:
    /**
      * The constructor.
      * For use by derived classs only.
      */
    type_pointer(const type::pointer &subtype);

    // See base class for documentation.
    rcstring get_name() const;

    // See base class for documentation.
    rcstring get_pascal_name() const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable() const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The subtype instance variable is used to remember the type of
      * variable being pointerd.
      */
    type::pointer subtype;

    /**
      * The default constructor.  Do not use.
      */
    type_pointer();

    /**
      * The copy constructor.  Do not use.
      */
    type_pointer(const type_pointer &);

    /**
      * The assignment operator.  Do not use.
      */
    type_pointer &operator=(const type_pointer &);
};

#endif // LIB_TYPE_POINTER_H
// vim: set ts=8 sw=4 et :

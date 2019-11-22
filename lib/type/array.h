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

#ifndef LIB_TYPE_ARRAY_H
#define LIB_TYPE_ARRAY_H

#include <lib/expression.h>
#include <lib/type.h>

/**
  * The type_array class is used to represent a single dimensioned array
  * type.  The index is a subrange type.
  */
class type_array:
    public type
{
public:
    typedef boost::shared_ptr<type_array> pointer;

    /**
      * The destructor.
      */
    virtual ~type_array();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(bool packed, const type::pointer &index_type,
        const type::pointer &subtype);

    /**
      * The get_index_type method is used to get the type for the array
      * index.
      *
      * @returns
      *     a type, the return type if this is indeed a type with a
      *     subtype, or the error type if its anything else.
      */
    type::pointer get_index_type() const { return index_type; }

    /**
      * The is_a class method is used to determine whether or not the
      * given type is an array type.
      *
      * @param arg
      *     The type to be tested.
      * @returns
      *     bool; true if is an array type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The is_a class method is used to determine whether or not the
      * given type is an array type.
      *
      * @param arg
      *     The expression who's type is to be tested.
      * @returns
      *     bool; true if is an array type, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The is_packed method is used to see if this is a packed array.
      * This is important, because packed and unpacked arrays are
      * indexed using different opcodes.
      */
    bool is_packed() const { return packed; }

    /**
      * The is_packed_array_of_char method is used to determine of this
      * type is a packed array of char.
      *
      * @return
      *     true if is a packed array of char, false if anything else
      */
    bool is_packed_array_of_char(void) const;

    /**
      * The get_width method may be used to determine how wide the array
      * is, in array members.  This is only the same as the number of
      * bytes when it is a packed array of char.
      */
    unsigned get_width(void) const;

    // See base class for documentation.
    type::pointer get_subtype() const;

protected:
    // See base class for documentation.
    rcstring get_name() const;

    // See base class for documentation.
    rcstring get_pascal_name() const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    void name_hint(const rcstring &name);

    // See base class for documentation.
    unsigned get_size_in_bits() const;

    // See base class for documentation.
    unsigned get_size_in_words() const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    bool is_a_variable() const ;

    // See base class for documentation.
    type::pointer late_binding_or_null() const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      */
    type_array(bool packed, const type::pointer &index_type,
        const type::pointer &subtype);

    /**
      * The packed instance variable is used to remember whether or not
      * thgis is a packed array type.  Packed arrays remember more than
      * one array member per 16-bit word.
      */
    bool packed;

    /**
      * The name instance variable is used to remember the name of this
      * type, for error messages.
      */
    rcstring name;

    /**
      * The index_type instance variable is used to remember the type of
      * the array index.
      */
    type::pointer index_type;

    /**
      * The subtype instance variable is used to remember the type of
      * the array elements.
      */
    type::pointer subtype;

    /**
      * The default constructor.  Do not use.
      */
    type_array();

    /**
      * The copy constructor.  Do not use.
      */
    type_array(const type_array &);

    /**
      * The assignment operator.  Do not use.
      */
    type_array &operator=(const type_array &);
};

#endif // LIB_TYPE_ARRAY_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TYPE_FORWARD_H
#define LIB_TYPE_FORWARD_H

#include <lib/type.h>

class translator; // forward

/**
  * The type_forward class is used to represent a forward reference to
  * an as-yet undefined type.
  */
class type_forward:
    public type
{
public:
    typedef boost::shared_ptr<type_forward> pointer;

    /**
      * The destructor.
      */
    virtual ~type_forward();

private:
    /**
      * The constructor.  It is private on purpose, use the "create"
      * class method instead.
      *
      * @param name
      *     The name of the type being referenced that doesn't exist
      *     yet.
      * @param cntxt
      *     The translation context.
      */
    type_forward(const rcstring &name, translator *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the type being referenced that doesn't exist
      *     yet.
      * @param cntxt
      *     The translation context.
      */
    static pointer create(const rcstring &name, translator *cntxt);

    /**
      * The is_a method is used to determine if the given expression is
      * of integer type, or an integer subrange type.
      *
      * @param candidate
      *     The type to be tested
      * @returns
      *     bool; true if it is an forward type, false if not
      */
    static bool is_a(const type::pointer &candidate);

protected:
    // See base class for documentation.
    rcstring get_name() const;

    // See base class for documentation.
    rcstring get_pascal_name() const;

    // See base class for documentation.
    rcstring get_c_declaration(const rcstring &name) const;

    // See base class for documentation.
    bool is_func() const;

    // See base class for documentation.
    type::pointer get_subtype() const;

    // See base class for documentation.
    unsigned get_size_in_words() const;

    // See base class for documentation.
    unsigned get_size_in_bits() const;

    // See base class for documentation.
    bool is_an_implicit_reference() const;

    // See base class for documentation.
    bool same_type_as(const type::pointer &rhs) const;

    // See base class for documentation.
    void name_hint(const rcstring &name);

    // See base class for documentation.
    bool is_a_variable() const;

    // See base class for documentation.
    int get_minimum_value() const;

    // See base class for documentation.
    int get_maximum_value() const;

    // See base class for documentation.
    bool may_be_used_as_array_index() const;

    // See base class for documentation.
    type::pointer late_binding_or_null() const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

private:
    /**
      * The xlat instance variable is used to remember where to lookup
      * real types, when the time comes.
      */
    translator &xlat;

    /**
      * The name instance variable is used to remember name of the type
      * being referenced.
      */
    rcstring name;

    /**
      * The default constructor.
      */
    type_forward();

    /**
      * The copy constructor.
      */
    type_forward(const type_forward &);

    /**
      * The assignment operator.
      */
    type_forward &operator=(const type_forward &);
};

#endif // LIB_TYPE_FORWARD_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_TYPE_ENUMERATED_H
#define LIB_TYPE_ENUMERATED_H

#include <lib/type.h>
#include <lib/expression.h>
#include <lib/symbol/list.h>

/**
  * The type_enumerated class is used to represent an enumerated type,
  * made up of an ordered list of tags.
  */
class type_enumerated:
    public type
{
public:
    typedef boost::shared_ptr<type_enumerated> pointer;

    /**
      * The destructor.
      */
    virtual ~type_enumerated();

private:
    /**
      * The default constructor.  Usually you should use the "create"
      * class method instead.
      */
    type_enumerated();

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    /**
      * The is_a method is used to determine if the given expression is
      * of an enumerated type.
      *
      * @param arg
      *     The expression to be tested
      * @returns
      *     bool; true if it is an enumerated type, false if not
      */
    static bool is_a(const expression::pointer &arg);

    /**
      * The is_a method is used to determine if the given type is
      * is an enumerated type.
      *
      * @param arg
      *     The expression to be tested
      * @returns
      *     bool; true if it is an enumerated type, false if not
      */
    static bool is_a(const type::pointer &arg);

    /**
      * The size method is used to determine the number of enumerated
      * values in the type.
      */
    size_t size(void) const { return members.size(); }

    /**
      * The push_back method is used to add another enum symbol to the
      * type.
      */
    void push_back(const symbol::pointer &sp) { members.push_back(sp); }

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
    void name_hint(const rcstring &name);

    // See base class for documentation.
    bool is_a_variable(void) const;

    // See base class for documentation.
    type::pointer get_subtype(void) const;

    // See base class for documentation.
    int get_minimum_value(void) const;

    // See base class for documentation.
    int get_maximum_value(void) const;

    // See base class for documentation.
    bool may_be_used_as_array_index(void) const;

    // See base class for documentation.
    bool need_late_binding(void) const;

    // See base class for documentation.
    rcstring get_human_readable_representation_of_value(int n) const;

    /**
      * The register_tag method is used by the parse to register an enum
      * tag with the enumerated type, to that values can be mapped back
      * to symbols.
      */
    void register_tag(const symbol::pointer &sp);

private:
    /**
      * The name instance variable is used to remember the name of the
      * type.
      */
    rcstring name;

    /**
      * The members instance variable is used to remember the list of
      * symbols in this type.
      */
    symbol_list members;

    /**
      * The copy constructor.  Do not use.
      */
    type_enumerated(const type_enumerated &);

    /**
      * The assignment operator.  Do not use.
      */
    type_enumerated &operator=(const type_enumerated &);
};

#endif // LIB_TYPE_ENUMERATED_H
// vim: set ts=8 sw=4 et :

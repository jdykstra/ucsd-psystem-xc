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

#ifndef LIB_EXPRESSION_ENUM_TAG_H
#define LIB_EXPRESSION_ENUM_TAG_H

#include <lib/expression.h>

class variable_name; // forward

/**
  * The expression_enum_tag class is used to represent
  * an enumeration tag's value and type.
  */
class expression_enum_tag:
    public expression
{
public:
    typedef boost::shared_ptr<expression_enum_tag> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_enum_tag();

protected:
    /**
      * The constructor.
      * Only for use by derived classes.
      *
      * @param name
      *     The name of the enumeration tag.
      * @param typ
      *     The type (parent enumeration) of the tag.
      * @param value
      *     The integer value of the tag.
      */
    expression_enum_tag(const variable_name &name, const type::pointer &typ,
        int value);

    /**
      * The get_name method may be used to obtain the enum tag's name.
      * It is protected on purpose, use sparingly.
      */
    rcstring get_name(void) const { return name; }

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    long get_integer_value(void) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    bool is_constant(void) const;

private:
    /**
      * The name instance variable is used to remember
      * the name of the enumeration tag.
      */
    rcstring name;

    /**
      * The type instance variable is used to remember
      * the type (parent enumeration) of the tag.
      */
    type::pointer typ;

    /**
      * The value instance variable is used to remember
      * the integer value of the tag.
      */
    int value;

    /**
      * The default constructor.  Do not use.
      */
    expression_enum_tag();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_enum_tag(const expression_enum_tag &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_enum_tag &operator=(const expression_enum_tag &rhs);
};

#endif // LIB_EXPRESSION_ENUM_TAG_H
// vim: set ts=8 sw=4 et :

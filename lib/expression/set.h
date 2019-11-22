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

#ifndef LIB_EXPRESSION_SET_H
#define LIB_EXPRESSION_SET_H

#include <lib/expression.h>
#include <lib/set.h>


/**
  * The expression_set class is used to represent a set constant
  * expression.
  */
class expression_set:
    public expression
{
public:
    typedef boost::shared_ptr<expression_set> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_set();

    /**
      * The constructor.
      *
      * @param locn
      *     The location of this expression in the source code.
      * @param tp
      *     The type of this expression node.
      * @param value
      *     The constant value of this expression node.
      */
    expression_set(const location &locn, const type::pointer &tp,
        const set &value);

    /**
      * The get_value method is used to gain access to the value of the
      * constant set.
      */
    const set &get_value() const { return value; }

protected:
    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    bool is_constant() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    bool is_empty_set(void) const;

private:
    /**
      * The tp instance variable is used to remember the type of this
      * expression node.
      */
    type::pointer tp;

    /**
      * The value instance variable is used to remember the constant
      * value of this expression node.
      */
    set value;

    /**
      * The default constructor.  Do not use.
      */
    expression_set();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_set(const expression_set &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_set &operator=(const expression_set &);
};

#endif // LIB_EXPRESSION_SET_H
// vim: set ts=8 sw=4 et :

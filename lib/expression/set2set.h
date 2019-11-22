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

#ifndef LIB_EXPRESSION_SET2SET_H
#define LIB_EXPRESSION_SET2SET_H

#include <lib/expression.h>

/**
  * The expression_set2set class is used to represent an abstraction
  * expression node which converts one set value of one set type to
  * another set value of another set type.
  */
class expression_set2set:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_set2set();

    /**
      * The constructor.
      */
    expression_set2set(const type::pointer &to,
        const expression::pointer &from);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    /**
      * The pre_order_traversal method is call by the traversal
      * method before the sub-expression is traversed.  The default
      * implementation does nothing.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is call by the traversal
      * method after the sub-expression is traversed.  The default
      * implementation does nothing.
      */
    virtual void post_order_traversal(int pprec) const;

    /**
      * The get_from method is used to obtain the input expression.
      */
    expression::pointer get_from(void) const { return from; }

private:
    /**
      * The to instance variable is used to remember the result type of
      * this expression.
      */
    type::pointer to;

    /**
      * The from instance variable is used to remember the value
      * expression to be converted.
      */
    expression::pointer from;

    /**
      * The default constructor.  Do not use.
      */
    expression_set2set();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_set2set(const expression_set2set &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_set2set &operator=(const expression_set2set &);
};

#endif // LIB_EXPRESSION_SET2SET_H
// vim: set ts=8 sw=4 et :

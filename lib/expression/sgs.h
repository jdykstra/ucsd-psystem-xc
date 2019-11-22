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

#ifndef LIB_EXPRESSION_SGS_H
#define LIB_EXPRESSION_SGS_H

#include <lib/expression.h>

/**
  * The expression_sgs class is used to represent an abstract expression
  * node which constructs a singleton set.
  */
class expression_sgs:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_sgs();

    /**
      * The constructor.
      */
    expression_sgs(const expression::pointer &value);

protected:
    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    /**
      * The pre_order_traversal method is called by the traversal
      * method immediately before traversing the value.  The default
      * implementation does nothing.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called by the traversal
      * method immediately after traversing the value.  The default
      * implementation does nothing.
      */
    virtual void post_order_traversal(int pprec) const;

    /**
      * The get_value method may be used by derived classes to obtain
      * access to the value expression.
      */
    expression::pointer get_value() const { return value; }

private:
    /**
      * The value instance variable is used to remer the value of the
      * singleton to be the whole range of the new set.
      */
    expression::pointer value;

    /**
      * The tp instance variable is used to remember the type of the
      * result of evaluating this expression node.
      */
    type::pointer tp;

    /**
      * The default constructor.  Do not use.
      */
    expression_sgs();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_sgs(const expression_sgs &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_sgs &operator=(const expression_sgs &);
};

#endif // LIB_EXPRESSION_SGS_H
// vim: set ts=8 sw=4 et :

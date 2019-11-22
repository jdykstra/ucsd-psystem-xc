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

#ifndef LIB_EXPRESSION_DEREFERENCE_H
#define LIB_EXPRESSION_DEREFERENCE_H

#include <lib/expression.h>

/**
  * The expression_dereference class is used to represent an abstract
  * pointer dereference (indirect load).
  */
class expression_dereference:
    public expression
{
public:
    typedef boost::shared_ptr<expression_dereference> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_dereference();

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    expression::pointer strip_indirection(void) const;

    expression::pointer get_operand(void) const { return operand; }

protected:
    /**
      * The constructor.
      *
      * @param arg
      *     The operand of the dereference.
      */
    expression_dereference(const expression::pointer &arg);

    /**
      * The pre_order_traversal method is called by the traversal method
      * just before the operand is traversed.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called by the traversal method
      * just after the operand is traversed.
      */
    virtual void post_order_traversal(int pprec) const;

    bool is_explicit() const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The operand instance variable is used to remember the pointer
      * expression we are going to dereference.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_dereference();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_dereference(const expression_dereference &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_dereference &operator=(const expression_dereference &);
};

#endif // LIB_EXPRESSION_DEREFERENCE_H
// vim: set ts=8 sw=4 et :

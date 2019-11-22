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

#ifndef LIB_EXPRESSION_LOGICAL_NOT_H
#define LIB_EXPRESSION_LOGICAL_NOT_H

#include <lib/expression.h>

class translator; // forward

/**
  * The expression_logical_not class is used to represent and abstract
  * logical NOT expression.
  */
class expression_logical_not:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_logical_not();

    /**
      * The constructor.
      */
    expression_logical_not(const expression::pointer &operand,
        translator *cntxt);

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

protected:
    /**
      * The pre_order_traversal method is called before the operand is
      * traversed, by the traversal method.
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The post_order_traversal method is called after the operand is
      * traversed, by the traversal method.
      */
    virtual void post_order_traversal(int pprec) const;

    /**
      * The get_operand method may be used by derived classes to obtain
      * the operand.
      */
    expression::pointer get_operand() const { return operand; }

    // See base class for documentation.
    virtual void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The xlat instance variable is used to remember where to get
      * labels from.
      */
    translator &xlat;

    /**
      * The operand instance variable is used to remember the operand of
      * this logical NOT expression.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_logical_not();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_logical_not(const expression_logical_not &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_logical_not &operator=(const expression_logical_not &);
};

#endif // LIB_EXPRESSION_LOGICAL_NOT_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_LITTORAL_EXPRESSION_DOT_H
#define UCSDPSYS_LITTORAL_EXPRESSION_DOT_H

#include <lib/expression.h>

class translator_littoral; // forward

/**
  * The expression_dot class is used to represent
  * writing an "e1 DOT NAME" expression.
  */
class expression_dot:
    public expression
{
public:
    typedef boost::shared_ptr<expression_dot> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_dot();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side of the expression.
      * @param rhs
      *     The right hand side of the expression.
      * @param littoral
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &lhs,
        const symbol::pointer &rhs, translator_littoral *littoral);

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
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    expression::pointer assignment_expression_factory(
        const expression::pointer &rhs) const;

    // See base class for documentation.
    expression::pointer dot_expression_factory(const location &,
        const symbol::pointer &rhs) const;

    // See base class for documentation.
    bool is_constant(void) const;

    // See base class for documentation.
    expression::pointer array_index_expression_factory(
        const expression::pointer &rhs) const;

    // See base class for documentation.
    bool is_simple_with_stmt(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The left hand side of the expression.
      * @param rhs
      *     The right hand side of the expression.
      * @param littoral
      *     The translation context we are working within.
      */
    expression_dot(const expression::pointer &lhs, const symbol::pointer &rhs,
        translator_littoral *littoral);

    /**
      * The lhs instance variable is used to remember the left hand side
      * of the expression
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the right hand
      * side of the expression
      */
    symbol::pointer rhs;

    /**
      * The littoral instance variable is used to remember the
      * translation context we are working within.
      */
    translator_littoral &littoral;

    expression::pointer clone(void) const;

    /**
      * The default constructor.  Do not use.
      */
    expression_dot();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_dot(const expression_dot &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_dot &operator=(const expression_dot &rhs);
};

#endif // UCSDPSYS_LITTORAL_EXPRESSION_DOT_H
// vim: set ts=8 sw=4 et :

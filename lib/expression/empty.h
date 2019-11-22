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

#ifndef LIB_EXPRESSION_EMPTY_H
#define LIB_EXPRESSION_EMPTY_H

#include <lib/expression.h>

/**
  * The expression_empty class is used to represent an expression
  * that has no value, and does no work.  This is the result for some
  * optimizations of system procedures.
  */
class expression_empty:
    public expression
{
public:
    typedef boost::shared_ptr<expression_empty> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_empty();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &locn);

protected:
    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    void traversal(int pprec) const;

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
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    expression_empty(const location &locn);

    /**
      * The default constructor.
      */
    expression_empty();

    /**
      * The copy constructor.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_empty(const expression_empty &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_empty &operator=(const expression_empty &rhs);
};

#endif // LIB_EXPRESSION_EMPTY_H
// vim: set ts=8 sw=4 et :

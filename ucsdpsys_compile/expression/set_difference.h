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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_SET_DIFFERENCE_H
#define UCSDPSYS_COMPILE_EXPRESSION_SET_DIFFERENCE_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_set_difference class is used to represent a set
  * difference expression.
  */
class expression_set_difference:
    public expression
{
public:
    typedef boost::shared_ptr<expression_set_difference> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_set_difference();

private:
    /**
      * The constructor.
      */
    expression_set_difference(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

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
    rcstring lisp_representation(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The lhs instance variable is used to remember the left hand
      * expression.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the right hand
      * expression.
      */
    expression::pointer rhs;

    /**
      * The default constructor.
      */
    expression_set_difference();

    /**
      * The copy constructor.
      */
    expression_set_difference(const expression_set_difference &);

    /**
      * The assignment operator.
      */
    expression_set_difference &operator=(const expression_set_difference &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_SET_DIFFERENCE_H
// vim: set ts=8 sw=4 et :

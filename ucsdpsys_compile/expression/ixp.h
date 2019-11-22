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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_IXP_H
#define UCSDPSYS_COMPILE_EXPRESSION_IXP_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_ixp class is used to represent an expression which
  * indexes a packed array.
  */
class expression_ixp:
    public expression
{
public:
    typedef boost::shared_ptr<expression_ixp> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ixp();

private:
    /**
      * The constructor.
      *
      * @param lhs
      *     The left hand side of the array index, the pointer to the
      *     array itself.  This is an unpacked pointer.
      * @param rhs
      *     The right hand side of the array index, the element number.
      *     It has already been range checked, and had the lower bound
      *     subtracted if necessary.
      * @param cntxt
      *     Where to send our generated code.
      */
    expression_ixp(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side of the array index, the pointer to the
      *     array itself.  This is an unpacked pointer.
      * @param rhs
      *     The right hand side of the array index, the element number.
      *     It has already been range checked, and had the lower bound
      *     subtracted if necessary.
      * @param cntxt
      *     Where to send our generated code.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

    // See base class for documentation.
    expression::pointer get_byte_pointer(void) const;

protected:
    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

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

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The lhs instance variable is used to remember the left hand
      * expression, the array's base address.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the right hand
      * expression, the array member of interest (zero based).
      */
    expression::pointer rhs;

    /**
      * The tp instance variable is used to remember the type of the
      * value of this expression node.  This is mostly an optimization.
      */
    type::pointer tp;

    /**
      * The default constructor.  Do not use.
      */
    expression_ixp();

    /**
      * The copy constructor.  Do not use.
      */
    expression_ixp(const expression_ixp &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_ixp &operator=(const expression_ixp &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_IXP_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_ADDITION_REAL_H
#define UCSDPSYS_COMPILE_EXPRESSION_ADDITION_REAL_H

#include <lib/expression/addition.h>

class translator_compile; // forward

/**
  * The expression_addition_real class is used to represent code
  * generation for addition expressions.
  */
class expression_addition_real:
    public expression_addition
{
public:
    typedef boost::shared_ptr<expression_addition_real> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_addition_real();

private:
    /**
      * The constructor.
      */
    expression_addition_real(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

public:
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void post_order_traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_addition_real();

    /**
      * The copy constructor.  Do not use.
      */
    expression_addition_real(const expression_addition_real &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_addition_real &operator=(const expression_addition_real &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_ADDITION_REAL_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LOGICAL_NOT_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_LOGICAL_NOT_COMPILE_H

#include <lib/expression/logical_not.h>

class translator_compile; // forward

/**
  * The expression_logical_not_compile class is used to represent the
  * code generation for a logical NOT expression.
  */
class expression_logical_not_compile:
    public expression_logical_not
{
public:
    typedef boost::shared_ptr<expression_logical_not_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_logical_not_compile();

private:
    /**
      * The constructor.
      */
    expression_logical_not_compile(const expression::pointer &operand,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &operand,
        translator_compile *cntxt);

protected:
    // See base for documentation.
    void traversal(int pprec) const;

    // See base for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    expression::pointer ord_odd_hack(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.
      */
    expression_logical_not_compile();

    /**
      * The copy constructor.
      */
    expression_logical_not_compile(const expression_logical_not_compile &);

    /**
      * The assignment operator.
      */
    expression_logical_not_compile &operator=(
        const expression_logical_not_compile &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LOGICAL_NOT_COMPILE_H
// vim: set ts=8 sw=4 et :

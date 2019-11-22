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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_LINT_FROM_INTEGER_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_LINT_FROM_INTEGER_PRETTY_H

#include <lib/expression/lint_from_integer.h>

/**
  * The expression_lint_from_integer_littoral class is used to represent
  * littoral printing a cast from an INTEGER value to an INTEGER[*] value.
  */
class expression_lint_from_integer_littoral:
    public expression_lint_from_integer
{
public:
    typedef boost::shared_ptr<expression_lint_from_integer_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_lint_from_integer_littoral();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param operand
      *     The value to be cast.
      */
    static pointer create(const expression::pointer &operand);

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    bool is_constant(void) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param operand
      *     The value to be cast.
      */
    expression_lint_from_integer_littoral(const expression::pointer &operand);

    /**
      * The default constructor.  Do not use.
      */
    expression_lint_from_integer_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_lint_from_integer_littoral(
        const expression_lint_from_integer_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_lint_from_integer_littoral &operator=(
        const expression_lint_from_integer_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_LINT_FROM_INTEGER_PRETTY_H
// vim: set ts=8 sw=4 et :

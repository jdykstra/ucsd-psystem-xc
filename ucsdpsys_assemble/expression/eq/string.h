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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_EQ_STRING_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_EQ_STRING_H

#include <lib/expression/eq.h>

/**
  * The expression_eq_string class is used to represent
  * an equality expression, between two strings.
  */
class expression_eq_string:
    public expression_eq
{
public:
    typedef boost::shared_ptr<expression_eq_string> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_eq_string();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs);

protected:
    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      */
    expression_eq_string(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The default constructor.  Do not use.
      */
    expression_eq_string();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_eq_string(const expression_eq_string &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_eq_string &operator=(const expression_eq_string &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_EQ_STRING_H
// vim: set ts=8 sw=4 et :

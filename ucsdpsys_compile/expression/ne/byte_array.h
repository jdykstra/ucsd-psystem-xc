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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_NE_BYTE_ARRAY_H
#define UCSDPSYS_COMPILE_EXPRESSION_NE_BYTE_ARRAY_H

#include <lib/expression/ne.h>

class translator_compile; // forward

/**
  * The expression_ne_byte_array class is used to represent an inequality
  * comparison of byte arrays.
  */
class expression_ne_byte_array:
    public expression_ne
{
public:
    typedef boost::shared_ptr<expression_ne_byte_array> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ne_byte_array();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @param code
      *     The translation context we are working within.
      */
    expression_ne_byte_array(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

protected:
    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    void traversal(int pprec) const;

private:
    /**
      * The code instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_ne_byte_array();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_ne_byte_array(const expression_ne_byte_array &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_ne_byte_array &operator=(const expression_ne_byte_array &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_NE_BYTE_ARRAY_H
// vim: set ts=8 sw=4 et :

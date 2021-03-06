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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_NE_MULTI_WORD_H
#define UCSDPSYS_COMPILE_EXPRESSION_NE_MULTI_WORD_H

#include <lib/expression/ne.h>

class translator_compile; // forward

/**
  * The expression_ne_multi_word class is used to represent an in-equality
  * comparison of arrays (other than packed array of char) and record
  * types.
  */
class expression_ne_multi_word:
    public expression_ne
{
public:
    typedef boost::shared_ptr<expression_ne_multi_word> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ne_multi_word();

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
    expression_ne_multi_word(const expression::pointer &lhs,
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
    expression_ne_multi_word();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_ne_multi_word(const expression_ne_multi_word &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_ne_multi_word &operator=(const expression_ne_multi_word &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_NE_MULTI_WORD_H
// vim: set ts=8 sw=4 et :

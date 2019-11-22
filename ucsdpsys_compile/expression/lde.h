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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LDE_H
#define UCSDPSYS_COMPILE_EXPRESSION_LDE_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_lde class is used to represent an LDE (LoaD External
  * word) opcode.
  */
class expression_lde:
    public expression
{
public:
    typedef boost::shared_ptr<expression_lde> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_lde();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     source location of this code
      * @param ptype
      *     the type of expression resulting from this code
      * @param segnum
      *     The number of the segment containing the variable.
      * @param offset
      *     the segment offset of this variable, in words
      * @param context
      *     where to send our generated code
      */
    static pointer create(const location &locn, const type::pointer &ptype,
        int segnum, int offset, translator_compile *context);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

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
      * The constructor.
      *
      * @param locn
      *     source location of this code
      * @param ptype
      *     the type of expression resulting from this code
      * @param segnum
      *     The number of the segment containing the variable.
      * @param offset
      *     the segment offset of this variable, in words
      * @param context
      *     where to send our generated code
      */
    expression_lde(const location &locn, const type::pointer &ptype,
        int segnum, int offset, translator_compile *context);

    /**
      * The ptype instance variable is used to remember the type of this
      * expression.
      */
    type::pointer ptype;

    /**
      * The segnum instance variable is used to remember
      * the number of the segment containing the variable.
      */
    int segnum;

    /**
      * The offset instance variable is used to remember
      * the segment offset of this variable, in words, starting from 1.
      */
    int offset;

    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_lde();

    /**
      * The copy constructor.  Do not use.
      */
    expression_lde(const expression_lde &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_lde &operator=(const expression_lde &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LDE_H
// vim: set ts=8 sw=4 et :

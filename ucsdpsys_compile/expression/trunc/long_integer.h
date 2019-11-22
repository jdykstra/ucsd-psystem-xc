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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_TRUNC_LONG_INTEGER_H
#define UCSDPSYS_COMPILE_EXPRESSION_TRUNC_LONG_INTEGER_H

#include <ucsdpsys_compile/expression/trunc.h>

class translator_compile; // forward

/**
  * The expression_trunc_long_integer class is used to represent an a
  * call of the builtin "trunc" function.
  */
class expression_trunc_long_integer:
    public expression_trunc
{
public:
    typedef boost::shared_ptr<expression_trunc_long_integer> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_trunc_long_integer();

private:
    /**
      * The constructor.
      *
      * @param operand
      *     The value to be truncated.
      * @param code
      *     The translation context we are working within.
      */
    expression_trunc_long_integer(const expression::pointer &operand,
        translator_compile *code);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param operand
      *     The value to be truncated.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &operand,
        translator_compile *code);

protected:
    // See base class for documentation.
    void post_order_traversal(int pprec) const;

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
    expression_trunc_long_integer();

    /**
      * The copy constructor.  Do not use.
      */
    expression_trunc_long_integer(const expression_trunc_long_integer &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_trunc_long_integer &operator=(
        const expression_trunc_long_integer &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_TRUNC_LONG_INTEGER_H
// vim: set ts=8 sw=4 et :

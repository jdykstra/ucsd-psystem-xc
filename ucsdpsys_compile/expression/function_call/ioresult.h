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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_IORESULT_H
#define UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_IORESULT_H

#include <lib/expression/function_call.h>

class translator_compile; // forward

/**
  * The expression_function_call_ioresult class is used to represent an a
  * call of the builtin "ioresult" function.
  */
class expression_function_call_ioresult:
    public expression_function_call
{
public:
    typedef boost::shared_ptr<expression_function_call_ioresult> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_function_call_ioresult();

private:
    /**
      * The constructor.
      *
      * @param locn
      *     The source file location of this expression.
      * @param cntxt
      *     where to send our generated code.
      */
    expression_function_call_ioresult(const location &locn,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The source file location of this expression.
      * @param cntxt
      *     where to send our generated code.
      */
    static pointer create(const location &locn, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void post_order_traversal(int pprec, int argnum) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    type::pointer get_type() const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_function_call_ioresult();

    /**
      * The copy constructor.  Do not use.
      */
    expression_function_call_ioresult(
        const expression_function_call_ioresult &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_function_call_ioresult &operator=(
        const expression_function_call_ioresult &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_IORESULT_H
// vim: set ts=8 sw=4 et :

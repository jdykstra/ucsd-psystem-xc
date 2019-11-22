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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_USER_DEFINED_H
#define UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_USER_DEFINED_H

#include <lib/expression/function_call.h>

class symbol_function_user_defined; // forward
class translator_compile; // forward

/**
  * The expression_function_call_user_defined class is used to represent
  * a user defined function constructed by parsing the Pascal source
  * code.
  */
class expression_function_call_user_defined:
    public expression_function_call
{
public:
    typedef boost::shared_ptr<expression_function_call_user_defined> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_function_call_user_defined();

private:
    /**
      * The constructor.
      *
      * @param lhs
      *     The function to be called
      * @param rhs
      *     The parameters to the function to be called,
      *     including the function itself as the first parameter
      * @param cntxt
      *     The translation context we are woring within
      */
    expression_function_call_user_defined(
        const symbol_function_user_defined *lhs, const expression_list &rhs,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The function to be called
      * @param rhs
      *     The parameters to the function to be called,
      *     including the function itself as the first parameter
      * @param cntxt
      *     The translation context we are woring within
      */
    static pointer create(const symbol_function_user_defined *lhs,
        const expression_list &rhs, translator_compile *cntxt);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The parameters to the function to be called, including the function
      *     itself as the first parameter (and it had better be the right kind
      *     of function, or asserts will trigger).
      * @param cntxt
      *     The translation context we are woring within
      */
    static pointer create(const expression_list &rhs,
        translator_compile *cntxt);

protected:
    // See base class for documentation.
    void post_order_traversal(int pprec, int argnum) const;

    // See base class for documentation.
    type::pointer get_type() const;

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
      * The sp instance variable is used to remember the symbol this
      * function call is calling.
      */
    const symbol_function_user_defined *sp;

    /**
      * The default constructor.  Do not use.
      */
    expression_function_call_user_defined();

    /**
      * The copy constructor.  Do not use.
      */
    expression_function_call_user_defined(
        const expression_function_call_user_defined &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_function_call_user_defined &operator=(
        const expression_function_call_user_defined &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_USER_DEFINED_H
// vim: set ts=8 sw=4 et :

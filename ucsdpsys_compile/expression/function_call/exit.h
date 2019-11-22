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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_EXIT_H
#define UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_EXIT_H

#include <lib/expression/function_call.h>

class expression_list; // forward
class translator_compile; // forward

/**
  * The expression_function_call_exit class is used to represent the
  * code generation required for the EXIT built-in function.
  */
class expression_function_call_exit:
    public expression_function_call
{
public:
    typedef boost::shared_ptr<expression_function_call_exit> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_function_call_exit();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param args
      *     The (implicit) parameters to the function.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression_list &args,
        translator_compile *code);

protected:
    // See base class for documentation.
    void post_order_traversal(int pprec, int argnum) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    bool execution_flows_out(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

private:
    /**
      * The fubar instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param args
      *     The (implicit) parameters to the function.
      * @param code
      *     The translation context we are working within.
      */
    expression_function_call_exit(const expression_list &args,
        translator_compile *code);

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_function_call_exit(const expression_function_call_exit &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_function_call_exit &operator=(
        const expression_function_call_exit &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_EXIT_H
// vim: set ts=8 sw=4 et :

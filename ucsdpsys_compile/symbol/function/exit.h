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

#ifndef UCSDPSYS_COMPILE_SYMBOL_FUNCTION_EXIT_H
#define UCSDPSYS_COMPILE_SYMBOL_FUNCTION_EXIT_H

#include <lib/symbol/function.h>

class translator_compile; // forward

/**
  * The symbol_function_exit class is used to represent the builtin
  * "exit" function.  The three use cases are
  *
  *     EXIT(functionname)
  *     EXIT(programName)
  *     EXIT(PROGRAM)
  *
  * The first two cases are symbols in the symbol table, and translator::
  * name_expression() covers it without any changes.  There is also a builtin
  * "program" symbol, to make the third case turn into the second case (with
  * the help of a specific grammar production).
  */
class symbol_function_exit:
    public symbol_function
{
public:
    typedef boost::shared_ptr<symbol_function_exit> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_function_exit();

private:
    /**
      * The default constructor.
      */
    symbol_function_exit(translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(translator_compile *cntxt);

protected:
    // See base class for documentation.
    expression::pointer function_call_expression_factory(
        const expression_list &rhs) const;

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
     * The code instance variable is used to remember where to send our
     * generated code.
     */
    translator_compile &code;

    /**
      * The copy constructor.  Do not use.
      */
    symbol_function_exit(const symbol_function_exit &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_function_exit &operator=(const symbol_function_exit &);
};

#endif // UCSDPSYS_COMPILE_SYMBOL_FUNCTION_EXIT_H
// vim: set ts=8 sw=4 et :

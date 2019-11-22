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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_HALT_H
#define UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_HALT_H

#include <lib/expression/function_call.h>

class translator_compile; // forward

/**
  * The expression_function_call_halt class is used to represent an a
  * call of the builtin "halt" function.
  */
class expression_function_call_halt:
    public expression_function_call
{
public:
    typedef boost::shared_ptr<expression_function_call_halt> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_function_call_halt();

private:
    /**
      * The constructor.
      */
    expression_function_call_halt(const location &locn,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
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

    // See base class for documentation.
    bool execution_flows_out(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_function_call_halt();

    /**
      * The copy constructor.  Do not use.
      */
    expression_function_call_halt(const expression_function_call_halt &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_function_call_halt &operator=(
        const expression_function_call_halt &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_FUNCTION_CALL_HALT_H
// vim: set ts=8 sw=4 et :

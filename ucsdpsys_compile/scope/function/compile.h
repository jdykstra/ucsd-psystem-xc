//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#ifndef UCSDPSYS_COMPILE_SCOPE_FUNCTION_COMPILE_H
#define UCSDPSYS_COMPILE_SCOPE_FUNCTION_COMPILE_H

#include <lib/scope/function.h>

class translator_compile; // forward

/**
  * The scope_function_compile class is used to represent code generation
  * for a function.
  */
class scope_function_compile:
    public scope_function
{
public:
    typedef boost::shared_ptr<scope_function_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_function_compile();

    /**
      * The create class method is used to create dynamically allocated
      * isntances of this class.
      */
    static pointer create(const rcstring &name, bool is_func,
        bool seg_proc_flag, translator_compile *cntxt);

    /**
      * The get_segment_number method is used to obtain the segment
      * number of the function.
      */
    unsigned get_segment_number(void) const { return seg_num; }

    /**
      * The get_procedure_number method is used to obtain the procedure
      * number of the function.
      */
    unsigned get_procedure_number(void) const { return proc_num; }

protected:
    /**
      * The constructor.
      */
    scope_function_compile(const rcstring &name, bool is_func,
        bool seg_proc_flag, translator_compile *cntxt);

    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

    // See base class for documentation.
    void notify_function_parameters(const name_type_list &ntl);

    // See base class for documentation.
    void pre_order_traversal(void) const;

    // See base class for documentation.
    void post_order_traversal(void) const;

    // See base class for documentation.
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    // See base class for documentation.
    void variable_declaration_notify(const symbol_variable_p &svp);

    // See base class for documentation.
    symbol::pointer lookup(const rcstring &name) const;

    // See base class for documentation.
    symbol_variable::pointer variable_factory(const variable_name &vname,
        const type::pointer &vtype, int lex_level);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(const variable_name &vname,
        const type::pointer &vtype, int lex_level);

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The is_a_function instance variable is used to remember whether
      * this is a function (true) or a procedure (false).  This is
      * mostly redundant, because the return type can tell us much
      * the same thig, except for the brief time between parsing the
      * FUNCTION keyword and eventually parsing the return type.
      */
    bool is_a_function;

    /**
      * The is_a_segment_procedure instance variable is used to remember
      * whether or not this function is in a new segment (true) or in
      * the same segment as it's enclosing scope (false).
      */
    bool is_a_segment_procedure;

    /**
      * The seg_num instance variable is used to remember the segment
      * number of the function.
      */
    int seg_num;

    /**
      * The proc_num instance variable is used to remember the procedure
      * number of the function.
      */
    int proc_num;

    /**
      * The default constructor.
      */
    scope_function_compile();

    /**
      * The copy constructor.
      */
    scope_function_compile(const scope_function_compile &);

    /**
      * The assignment operator.
      */
    scope_function_compile &operator=(const scope_function_compile &);
};

#endif // UCSDPSYS_COMPILE_SCOPE_FUNCTION_COMPILE_H
// vim: set ts=8 sw=4 et :

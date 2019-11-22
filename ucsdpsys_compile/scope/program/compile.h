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

#ifndef UCSDPSYS_COMPILE_SCOPE_PROGRAM_COMPILE_H
#define UCSDPSYS_COMPILE_SCOPE_PROGRAM_COMPILE_H

#include <lib/scope/program.h>

class translator_compile; // forward

/**
  * The scope_program_compile class is used to represent code generation
  * for a program.
  */
class scope_program_compile:
    public scope_program
{
public:
    typedef boost::shared_ptr<scope_program_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_program_compile();

    /**
      * The create class method is used to create dynamically allocated
      * instances of this class.
      */
    static pointer create(const rcstring &name, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void post_order_traversal() const;

    // See base class for documentation.
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    // See base class for documentation.
    void variable_declaration_notify(const symbol_variable_p &svp);

    // See base class for documentation.
    void notify_function_parameters(const name_type_list &ntl);

    // See base class for documentation.
    symbol_variable::pointer variable_factory(const variable_name &vname,
        const type::pointer &vtype, int lex_level);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(const variable_name &vname,
        const type::pointer &vtype, int lex_level);

    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

private:
    /**
      * The constructor.
      */
    scope_program_compile(const rcstring &name, translator_compile *cntxt);

    /**
      * The code instance variable is used to remeber where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The proc_num instance variable is used to remember the procedure
      * number of the program.
      */
    int proc_num;

    /**
      * The default constructor.
      */
    scope_program_compile();

    /**
      * The copy constructor.
      */
    scope_program_compile(const scope_program_compile &);

    /**
      * The assignment operator.
      */
    scope_program_compile &operator=(const scope_program_compile &);
};

#endif // UCSDPSYS_COMPILE_SCOPE_PROGRAM_COMPILE_H
// vim: set ts=8 sw=4 et :

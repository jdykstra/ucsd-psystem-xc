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

#ifndef UCSDPSYS_COMPILE_SCOPE_TEMPORARY_COMPOUND_COMPILE_H
#define UCSDPSYS_COMPILE_SCOPE_TEMPORARY_COMPOUND_COMPILE_H

#include <lib/scope/temporary/compound.h>

class translator_compile; // forward

/**
  * The scope_temporary_compound_compile class is used to represent a
  * scope within the compiler for creating temporary variables with
  * compound statement scope.
  */
class scope_temporary_compound_compile:
    public scope_temporary_compound
{
public:
    typedef boost::shared_ptr<scope_temporary_compound_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_temporary_compound_compile();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(scope_stack &stk, translator_compile *context);

protected:
    // See base class for documentation.
    symbol_variable::pointer variable_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    scope_temporary_compound_compile(scope_stack &stk,
        translator_compile *context);

    /**
      * The context instance variable is used to remember
      */
    translator_compile *context;

    /**
      * The default constructor.  Do not use.
      */
    scope_temporary_compound_compile();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_temporary_compound_compile(
        const scope_temporary_compound_compile &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_temporary_compound_compile &operator=(
        const scope_temporary_compound_compile &rhs);
};

#endif // UCSDPSYS_COMPILE_SCOPE_TEMPORARY_COMPOUND_COMPILE_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_COMPILE_SYMBOL_VARIABLE_NORMAL_COMPILE_H
#define UCSDPSYS_COMPILE_SYMBOL_VARIABLE_NORMAL_COMPILE_H

#include <lib/symbol/variable/normal.h>

class translator_compile; // forward

/**
  * The symbol_variable_normal_compile class is used to represent
  * a local variable.
  */
class symbol_variable_normal_compile:
    public symbol_variable_normal
{
public:
    typedef boost::shared_ptr<symbol_variable_normal_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_variable_normal_compile();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the variable
      * @param tp
      *     The type of the variable
      * @param lex_level
      *     The lexical level of the variable
      * @param kind
      *     The kind of the variable
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const variable_name &name, const type::pointer &tp,
        int lex_level, kind_t kind, translator_compile *context);

protected:
    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int ref_lxlvl);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the variable
      * @param tp
      *     The type of the variable
      * @param lex_level
      *     The lexical level of the variable
      * @param kind
      *     The kind of the variable
      * @param context
      *     The translation context we are working within.
      */
    symbol_variable_normal_compile(const variable_name &name,
        const type::pointer &tp, int lex_level, kind_t kind,
        translator_compile *context);

    /**
      * The context instance variable is used to remember the
      * translation context we are working within.
      */
    translator_compile &context;

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_variable_normal_compile(const symbol_variable_normal_compile &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
     */
    symbol_variable_normal_compile &operator=(
        const symbol_variable_normal_compile &rhs);
};

#endif // UCSDPSYS_COMPILE_SYMBOL_VARIABLE_NORMAL_COMPILE_H
// vim: set ts=8 sw=4 et :

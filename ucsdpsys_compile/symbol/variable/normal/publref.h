//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#ifndef UCSDPSYS_COMPILE_SYMBOL_VARIABLE_NORMAL_PUBLREF_H
#define UCSDPSYS_COMPILE_SYMBOL_VARIABLE_NORMAL_PUBLREF_H

#include <lib/expression.h>
#include <lib/symbol/variable/normal.h>

class translator_compile; // forward

/**
  * The symbol_variable_normal_publref class is used to represent a
  * variable that is at the global scope, but not yet allocated; a
  * global variable that needs linking.
  *
  * Variables declared in the INTERFACE part of a UNIT are PUBLREF.
  * Variables declared in the IMPLEMENTATION part of a UNIT are PRIVREF.
  * Variables declared as a result of USES of a UNIT are PUBLDEF.
  *
  * These variables are used in non-intrinsic units if a variable is
  * declared at the global level in the unit.  They are used by the unit
  * defining the variable when the unit is compiled; they are also used
  * by the clients of any such units.  The actual offset for the global
  * variable is determined by the linker, and patched back into both the
  * refering program and the unit, when both are copied to create the
  * final executable.
  */
class symbol_variable_normal_publref:
    public symbol_variable_normal
{
public:
    typedef boost::shared_ptr<symbol_variable_normal_publref> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_variable_normal_publref();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the variable
      * @param tp
      *     The type of the variable
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const variable_name &name, const type::pointer &tp,
        translator_compile *context);

protected:
    // See base class for documentation.
    expression::pointer name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the variable
      * @param tp
      *     The type of the variable
      * @param context
      *     The translation context we are working within.
      */
    symbol_variable_normal_publref(const variable_name &name,
        const type::pointer &tp, translator_compile *context);

    /**
      * The context instance variable is used to remember
      * the translation context we are working within.
      */
    translator_compile *context;

    /**
      * The default constructor.  Do not use.
      */
    symbol_variable_normal_publref();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_variable_normal_publref(const symbol_variable_normal_publref &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_variable_normal_publref &operator=(
        const symbol_variable_normal_publref &rhs);
};

#endif // UCSDPSYS_COMPILE_SYMBOL_VARIABLE_NORMAL_PUBLREF_H
// vim: set ts=8 sw=4 et :

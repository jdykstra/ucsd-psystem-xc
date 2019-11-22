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

#ifndef UCSDPSYS_LITTORAL_SCOPE_BUILTIN_LITTORAL_H
#define UCSDPSYS_LITTORAL_SCOPE_BUILTIN_LITTORAL_H

#include <lib/scope/builtin.h>

class translator_littoral; // forward

/**
  * The scope_builtin_littoral class is used to represent
  * the scope containing built-in variables and functions.
  */
class scope_builtin_littoral:
    public scope_builtin
{
public:
    typedef boost::shared_ptr<scope_builtin_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_builtin_littoral();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(translator_littoral *context);

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
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param context
      *     The translation context we are working within.
      */
    scope_builtin_littoral(translator_littoral *context);

    /**
      * The context instance variable is used to remember
      * the translation context we are working within.
      */
    translator_littoral *context;

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_builtin_littoral(const scope_builtin_littoral &rhs);

    /**
      * The assignment operator.  Do no tuse.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_builtin_littoral &operator=(const scope_builtin_littoral &rhs);
};

#endif // UCSDPSYS_LITTORAL_SCOPE_BUILTIN_LITTORAL_H
// vim: set ts=8 sw=4 et :

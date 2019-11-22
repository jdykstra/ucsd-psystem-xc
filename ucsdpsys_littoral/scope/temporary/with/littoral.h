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

#ifndef UCSDPSYS_LITTORAL_SCOPE_TEMPORARY_WITH_LITTORAL_H
#define UCSDPSYS_LITTORAL_SCOPE_TEMPORARY_WITH_LITTORAL_H

#include <lib/scope/temporary/with.h>

class translator_littoral; // forward

/**
  * The scope_temporary_with_littoral class is used to represent
  * the WITH scope for writing C++, when referenced.
  */
class scope_temporary_with_littoral:
    public scope_temporary_with
{
public:
    typedef boost::shared_ptr<scope_temporary_with_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_temporary_with_littoral();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param ss
      *     The scope stack used to allocate and de-allocate.
      * @param base
      *     The implied lhs for the direct field references
      * @param assign
      *     The statement that builds the base value, or NULL if none required.
      * @param context
      *     the translator context we are working within
      */
    static pointer create(scope_stack &ss, const expression::pointer &base,
        const statement::pointer &assign, translator_littoral *context);

protected:
    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

    // See base class for documentation.
    symbol_variable::pointer variable_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param ss
      *     The scope stack used to allocate and de-allocate.
      * @param base
      *     The implied lhs for the direct field references
      * @param assign
      *     The statement that builds the base value, or NULL if none required.
      * @param context
      *     the translator context we are working within
      */
    scope_temporary_with_littoral(scope_stack &ss,
        const expression::pointer &base, const statement::pointer &assign,
        translator_littoral *context);

    /**
      * The context instance variable is used to remember
      * the translator context we are working within.
      */
    translator_littoral *context;

    /**
      * The copy constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_temporary_with_littoral(const scope_temporary_with_littoral &rhs);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    static pointer create(const scope_temporary_with_littoral &rhs);

    /**
      * The assignment operator.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_temporary_with_littoral &operator=(
        const scope_temporary_with_littoral &rhs);
};

#endif // UCSDPSYS_LITTORAL_SCOPE_TEMPORARY_WITH_LITTORAL_H
// vim: set ts=8 sw=4 et :

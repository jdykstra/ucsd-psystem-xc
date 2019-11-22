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

#ifndef UCSDPSYS_PRETTY_SCOPE_INTERFACE_PRETTY_H
#define UCSDPSYS_PRETTY_SCOPE_INTERFACE_PRETTY_H

#include <lib/scope/interface.h>

class translator_littoral; // forward

/**
  * The scope_interface_littoral class is used to represent the whole
  * lot of nothing that the littoral printer does with unit interface
  * definitions.
  */
class scope_interface_littoral:
    public scope_interface
{
public:
    typedef boost::shared_ptr<scope_interface_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_interface_littoral();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the scope
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, translator_littoral *context);

protected:
    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

    // See base class for documentation.
    symbol_variable::pointer variable_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(
        const variable_name &name, const type::pointer &tp, int lex_level);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the scope
      * @param context
      *     The translation context we are working within.
      */
    scope_interface_littoral(const rcstring &name,
        translator_littoral *context);

    /**
      * The context instance variable is used to remember
      * the translation context we are working within.
      */
    translator_littoral *context;

    /**
      * The default constructor.  Do not use.
      */
    scope_interface_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_interface_littoral(const scope_interface_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_interface_littoral &operator=(const scope_interface_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_SCOPE_INTERFACE_PRETTY_H
// vim: set ts=8 sw=4 et :

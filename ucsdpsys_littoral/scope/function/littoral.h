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

#ifndef UCSDPSYS_PRETTY_SCOPE_FUNCTION_PRETTY_H
#define UCSDPSYS_PRETTY_SCOPE_FUNCTION_PRETTY_H

#include <lib/scope/function.h>
#include <lib/name_type/list.h>

class translator_littoral; // forward

/**
  * The scope_function_littoral class is used to represent a littoral prining
  * of the outer-most function scope.
  */
class scope_function_littoral:
    public scope_function
{
public:
    typedef boost::shared_ptr<scope_function_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_function_littoral();

    /**
      * The create class method is used to create dynamically allocated
      * instances of this class.
      */
    static pointer create(const rcstring &name, translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void begin_notify(void);

    // See base class for documentation.
    void notify_function_parameters(const name_type_list &ntl);

    // See base class for documentation.
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    // See base class for documentation.
    void variable_declaration_notify(const symbol_variable_p &svp);

    // See base class for documentation.
    void traversal(void) const;

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
      * The constructor.
      */
    scope_function_littoral(const rcstring &name, translator_littoral *cntxt);

    /**
      * The littoral instance variable is used to remember where to print
      * our textual output.
      */
    translator_littoral &littoral;

    /**
      * The parameters instance variable is used to remember the
      * function parameters (names and types).
      */
    name_type_list parameters;

    /**
      * The variables instance variable is used to remember the
      * function variables (names and types).
      */
    name_type_list variables;

    /**
      * The default constructor.  Do not use.
      */
    scope_function_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    scope_function_littoral(const scope_function_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_function_littoral &operator=(const scope_function_littoral &);
};

#endif // UCSDPSYS_PRETTY_SCOPE_FUNCTION_PRETTY_H
// vim: set ts=8 sw=4 et :

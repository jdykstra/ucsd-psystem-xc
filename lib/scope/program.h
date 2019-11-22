//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#ifndef LIB_SCOPE_PROGRAM_H
#define LIB_SCOPE_PROGRAM_H

#include <lib/scope.h>

/**
  * The scope_program class is used to represent a simple outer-most-
  * level program scope, where global symbols are declared.
  */
class scope_program:
    public scope
{
public:
    typedef boost::shared_ptr<scope_program> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_program();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    scope_program(const rcstring &name);

protected:
    // See base class for documentation.
    void notify_function_parameters(const name_type_list &ntl);

    // See base class for documentation.
    int get_lex_level_height(void) const;

    // See base class for documentation.
    void label_declaration_notify(const variable_name_list &names);

private:
    /**
      * The default constructor.  Do not use.
      */
    scope_program();

    /**
      * The copy constructor.  Do not use.
      */
    scope_program(const scope_program &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_program &operator=(const scope_program &);
};

#endif // LIB_SCOPE_PROGRAM_H
// vim: set ts=8 sw=4 et :

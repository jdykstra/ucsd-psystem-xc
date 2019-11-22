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

#ifndef LIB_SCOPE_TEMPORARY_H
#define LIB_SCOPE_TEMPORARY_H

#include <lib/bit_address.h>
#include <lib/scope.h>

class scope_stack; // forward

/**
  * The scope_temporary class is used to represent the scope of
  * variables created in-line, usually by the compiler, for temporary
  * use and limited lifetimes.
  */
class scope_temporary:
    public scope
{
public:
    typedef boost::shared_ptr<scope_temporary> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_temporary();

protected:
    // See base class for documentation.
    int get_lex_level_height(void) const;

    // See base class for documentation.
    bool is_temporary(void) const;

    // See base class for documentation.
    void attach_return_type(const type::pointer &tp);

    // See base class for documentation.
    void attach_function_parameters(name_type_list *ntlp);

    // See base class for documentation.
    bool was_forward(void) const;

    // See base class for documentation.
    void set_forward(bool ign_seg0);

    // See base class for documentation.
    void drop_locals(void);

    // See base class for documentation.
    void label_declaration_notify(const variable_name_list &names);

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

    /**
      * The constructor.
      * For the use of derived classes only.
      *
      * @param ss
      *     The scope stack used to allocate and de-allocate.
      */
    scope_temporary(scope_stack &ss);

private:
    /**
      * The ss instance variable is used to remember the scope stack we
      * are allocating within.
      */
    scope_stack &ss;

    /**
      * The mark instance variable is used to remember the stack frame
      * size to restore when this scope ends.
      */
    bit_address mark;

    /**
      * The default constructor.  Do not use.
      */
    scope_temporary();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_temporary(const scope_temporary &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_temporary &operator=(const scope_temporary &rhs);
};

#endif // LIB_SCOPE_TEMPORARY_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_SCOPE_FUNCTION_H
#define LIB_SCOPE_FUNCTION_H

#include <lib/scope.h>


/**
  * The scope_function class is used to represent a function scope,
  * where local symbols are declared.
  */
class scope_function:
    public scope
{
public:
    typedef boost::shared_ptr<scope_function> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_function();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    scope_function(const rcstring &name);

protected:
    // See base class for documentation.
    void attach_function_parameters(name_type_list *ntlp);

    // See base class for documentation.
    void attach_return_type(const type::pointer &tp);

    // See base class for documentation.
    bool was_forward() const;

    // See base class for documentation.
    void set_forward(bool ign_seg0);

    // See base class for documentation.
    void drop_locals();

    // See base class for documentation.
    int get_lex_level_height(void) const;

    // See base class for documentation.
    void label_declaration_notify(const variable_name_list &names);

    // See base class for documentation.
    link_info::pointer get_link_info_extproc(void) const;

    type::pointer get_return_type(void) const { return return_type; }

private:
    /**
      * The return_type instance variable is used to remember the return
      * type of this function.
      */
    type::pointer return_type;

    /**
      * The default constructor.  Do not use.
      */
    scope_function();

    /**
      * The copy constructor.  Do not use.
      */
    scope_function(const scope_function &);

    /**
      * The assignment operator.  Do not use.
      */
    scope_function &operator=(const scope_function &);
};

#endif // LIB_SCOPE_FUNCTION_H
// vim: set ts=8 sw=4 et :

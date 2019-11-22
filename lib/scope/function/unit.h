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

#ifndef LIB_SCOPE_FUNCTION_UNIT_H
#define LIB_SCOPE_FUNCTION_UNIT_H

#include <lib/scope/function.h>

/**
  * The scope_function_unit class is used to represent the collected
  * portions of the INTERFACE portion of a compiled UNIT.
  */
class scope_function_unit:
    public scope_function
{
public:
    typedef boost::shared_ptr<scope_function_unit> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_function_unit();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param name
      *     The name of the procedure, function or unit.
      * @param segnum
      *     The segment number of the unit.
      */
    scope_function_unit(const rcstring &name, unsigned segnum);

    // See base class for documentation.
    int get_lex_level_height(void) const;

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
    unsigned get_segment_number(void) const;

    // See base class for documentation.
    void label_declaration_notify(const variable_name_list &names);

private:
    /**
      * The segnum instance variable is used to remember the segment
      * number of the unit.
      */
    unsigned segnum;

    /**
      * The next_proc_num instance variable is used to remember the next
      * available procedure number.
      */
    unsigned next_proc_num;

    /**
      * The default constructor.  Do not use.
      */
    scope_function_unit();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_function_unit(const scope_function_unit &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_function_unit &operator=(const scope_function_unit &rhs);
};

#endif // LIB_SCOPE_FUNCTION_UNIT_H
// vim: set ts=8 sw=4 et :

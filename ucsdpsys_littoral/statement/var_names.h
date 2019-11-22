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

#ifndef UCSDPSYS_PRETTY_STATEMENT_VAR_NAMES_H
#define UCSDPSYS_PRETTY_STATEMENT_VAR_NAMES_H

#include <lib/statement.h>
#include <lib/type.h>
#include <lib/variable/name/list.h>

class translator_littoral; // forward

/**
  * The statement_var_names class is used to represent a VAR statement,
  * allowing variables to be declared in-line with statements within
  * begin-end blocks.
  */
class statement_var_names:
    public statement
{
public:
    typedef boost::shared_ptr<statement_var_names> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_var_names();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param names
      *     The names of the variables to create.
      * @param tp
      *     The type of the variables.
      * @param littoral
      *     The translator context we are working within.
      */
    static pointer create(const variable_name_list &names,
        const type::pointer &tp, translator_littoral *littoral);

protected:
    // See base class for documentation.
    void traversal(void) const;

    // See base class for documentation.
    bool execution_flows_out(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param names
      *     The names of the variables to create.
      * @param tp
      *     The type of the variables.
      * @param littoral
      *     The translator context we are working within.
      */
    statement_var_names(const variable_name_list &names,
        const type::pointer &tp, translator_littoral *littoral);

    /**
      * The names instance variable is used to remember the names of the
      * variables to create.
      */
    variable_name_list names;

    /**
      * The tp instance variable is used to remember the type of the
      * variables.
      */
    type::pointer tp;

    /**
      * The littoral instance variable is used to remember the translator
      * context we are working within.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_var_names();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_var_names(const statement_var_names &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_var_names &operator=(const statement_var_names &rhs);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_VAR_NAMES_H
// vim: set ts=8 sw=4 et :

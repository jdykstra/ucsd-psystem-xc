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

#ifndef UCSDPSYS_PRETTY_STATEMENT_VAR_EXPRESSION_H
#define UCSDPSYS_PRETTY_STATEMENT_VAR_EXPRESSION_H

#include <lib/expression.h>
#include <lib/statement.h>
#include <lib/variable/name.h>

class translator_littoral; // forward

/**
  * The statement_var_expression class is used to represent an in-line
  * var statement that takes its type from, and is initialized by, an
  * expression.
  */
class statement_var_expression:
    public statement
{
public:
    typedef boost::shared_ptr<statement_var_expression> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_var_expression();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the variable being declared
      * @param ep
      *     The expression that initializes the variable.
      * @param littoral
      *     The translation context we are working within.
      */
    static pointer create(const variable_name &name,
        const expression::pointer &ep, translator_littoral *littoral);

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
      * @param name
      *     The name of the variable being declared
      * @param ep
      *     The expression that initializes the variable.
      * @param littoral
      *     The translation context we are working within.
      */
    statement_var_expression(const variable_name &name,
        const expression::pointer &ep, translator_littoral *littoral);

    /**
      * The name instance variable is used to remember the name of the
      * variable being declared
      */
    variable_name name;

    /**
      * The ep instance variable is used to remember the expression that
      * initializes the variable.
      */
    expression::pointer ep;

    /**
      * The ep instance variable is used to remember the translation
      * context we are working within.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_var_expression();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_var_expression(const statement_var_expression &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_var_expression &operator=(const statement_var_expression &rhs);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_VAR_EXPRESSION_H
// vim: set ts=8 sw=4 et :

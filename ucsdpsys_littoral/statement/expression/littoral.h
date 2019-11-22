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

#ifndef UCSDPSYS_LITTORAL_STATEMENT_EXPRESSION_LITTORAL_H
#define UCSDPSYS_LITTORAL_STATEMENT_EXPRESSION_LITTORAL_H

#include <lib/statement/expression.h>

class translator_littoral; // forward

/**
  * The statement_expression_littoral class is used to represent
  * printing an expression statement as C++.
  */
class statement_expression_littoral:
    public statement_expression
{
public:
    typedef boost::shared_ptr<statement_expression_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_expression_littoral();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param arg
      *     The expression to be printed.
      * @param littoral
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &arg,
        translator_littoral *littoral);

protected:
    // See base class for documentation.
    void traversal(void) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param arg
      *     The expression to be printed.
      * @param littoral
      *     The translation context we are working within.
      */
    statement_expression_littoral(const expression::pointer &arg,
        translator_littoral *littoral);

    /**
      * The littoral instance variable is used to remember the
      * translation context we are working within.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_expression_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_expression_littoral(const statement_expression_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_expression_littoral &operator=(
        const statement_expression_littoral &rhs);
};

#endif // UCSDPSYS_LITTORAL_STATEMENT_EXPRESSION_LITTORAL_H
// vim: set ts=8 sw=4 et :

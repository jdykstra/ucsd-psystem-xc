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

#ifndef LIB_STATEMENT_EXPRESSION_H
#define LIB_STATEMENT_EXPRESSION_H

#include <lib/expression.h>
#include <lib/statement.h>

/**
  * The statement_expression class is used to represent a statement
  * which evaluates and expression as its effect.
  */
class statement_expression:
    public statement
{
public:
    typedef boost::shared_ptr<statement_expression> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_expression();

protected:
    /**
      * The constructor.
      */
    statement_expression(const expression::pointer &arg);

public:
    static pointer create(const expression::pointer &arg);

protected:
    // See base class for documentation.
    bool execution_flows_out(void) const;

    // See base class for documentation.
    void traversal(void) const;

    // See base class for documentation.
    bool empty(void) const;

private:
    /**
      * The arg instance variable is used to remember the expression
      * which is this statement's body.
      */
    expression::pointer arg;

    /**
      * The default constructor.  Do not use.
      */
    statement_expression();

    /**
      * The copy constructor.  Do not use.
      */
    statement_expression(const statement_expression &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_expression &operator=(const statement_expression &);
};

#endif // LIB_STATEMENT_EXPRESSION_H
// vim: set ts=8 sw=4 et :

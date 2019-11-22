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

#ifndef LIB_STATEMENT_IF_H
#define LIB_STATEMENT_IF_H

#include <lib/expression.h>
#include <lib/statement.h>

class translator; // forward

/**
  * The statement_if class is used to represent an abstract "if"
  * statement, with optional else clause.
  */
class statement_if:
    public statement
{
public:
    /**
      * The destructor.
      */
    virtual ~statement_if();

    /**
      * The constructor.
      */
    statement_if(const expression::pointer &condition,
         const statement::pointer &then_clause,
         const statement::pointer &else_clause, translator *xlat);

protected:
    // See base class for documentation.
    void traversal() const;

    // See base class for documentation.
    bool empty() const;

    // See base class for documentation.
    bool execution_flows_out() const;

    /**
      * The pre_order_traversal method is called by the traversal method
      * before the condition is traversed.  The default implementation
      * does nothing.
      */
    virtual void pre_order_traversal() const;

    /**
      * The then_order_traversal method is called by the traversal
      * method after the condition is traversed and before the
      * then_clause is traversed.  The default implementation does
      * nothing.
      */
    virtual void then_order_traversal() const;

    /**
      * The else_order_traversal method is called by the traversal
      * method (but only if the else_clause is not null) after the
      * then_clause is traversed and before the else_clause is
      * traversed.  The default implementation does nothing.
      */
    virtual void else_order_traversal() const;

    /**
      * The post_order_traversal method is called by the traversal
      * method after the then_clause and else_clause have been
      * traversed.  The default implementation does nothing.
      */
    virtual void post_order_traversal() const;

    expression::pointer get_condition(void) const { return condition; }
    statement::pointer get_then_clause(void) const { return then_clause; }
    statement::pointer get_else_clause(void) const { return else_clause; }

private:
    /**
      * The xlat instance variable is used to reemeber where to get new
      * labels from.
      */
    translator &xlat;

    /**
      * The condition instance variable is used to remember the
      * controlling condition for this if statement.
      */
    expression::pointer condition;

    /**
      * The then_clause instance variable is used to remember the "then"
      * action for this if statement.
      */
    statement::pointer then_clause;

    /**
      * The else_clause instance variable is used to remember the "else"
      * action for this if statement.
      */
    statement::pointer else_clause;

    /**
      * The default constructor.
      */
    statement_if();

    /**
      * The copy constructor.
      */
    statement_if(const statement_if &);

    /**
      * The assignment operator.
      */
    statement_if &operator=(const statement_if &);
};

#endif // LIB_STATEMENT_IF_H
// vim: set ts=8 sw=4 et :

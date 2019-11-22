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

#ifndef LIB_STATEMENT_WRITELN_H
#define LIB_STATEMENT_WRITELN_H

#include <lib/statement.h>

#include <lib/expression/list.h>

/**
  * The statement_writeln class is used to represent a writeln
  * statement, which has wierd syntax and semantics compared to a more
  * usual Pascal procedure call.
  */
class statement_writeln:
    public statement
{
public:
    /**
      * The destructor.
      */
    virtual ~statement_writeln();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    statement_writeln(const expression_list &args);

public:
    // See base class for documentation.
    void traversal() const;

    // See base class for documentation.
    bool execution_flows_out() const;

protected:
    /**
      * The has_args method is used to determine whether or not this
      * call has arguemnts.
      */
    bool has_args() const { return !args.empty(); }

    /**
      * The pre_order_traversal method is called before any of the
      * expressions are traversed.  The default implementation does
      * nothing.
      */
    virtual void pre_order_traversal() const;

    /**
      * The infix_order_traversal method is called after traversing
      * each of the expressions.  The default implementation does
      * nothing.
      *
      * @param ep
      *     The expression just traversed.
      * @param last
      *     true if this is the last expression in the list
      */
    virtual void infix_order_traversal(const expression::pointer &ep,
        bool last) const;

    /**
      * The post_order_traversal method is called after all of the
      * expressions have been traversed.  The default implementation
      * does nothing.
      */
    virtual void post_order_traversal() const;

private:
    expression_list args;

    /**
      * The default constructor.  Do not use.
      */
    statement_writeln();

    /**
      * The copy constructor.  Do not use.
      */
    statement_writeln(const statement_writeln &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_writeln &operator=(const statement_writeln &);
};

#endif // LIB_STATEMENT_WRITELN_H
// vim: set ts=8 sw=4 et :

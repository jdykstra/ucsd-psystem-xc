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

#ifndef LIB_STATEMENT_LABEL_H
#define LIB_STATEMENT_LABEL_H

#include <lib/statement.h>
#include <lib/symbol/label.h>

/**
  * The statement_label class is used to represent a label and the
  * statement that follows it.
  */
class statement_label:
    public statement
{
public:
    typedef boost::shared_ptr<statement_label> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_label();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param sym
      *     The label symbol being defined
      * @param body
      *     The statement following the label.
      */
    statement_label(const symbol::pointer &sym, const statement::pointer &body);

protected:
    // See base class for documentation.
    void traversal(void) const;

    // See base class for documentation.
    bool execution_flows_out(void) const;

    // See base class for documentation.
    bool execution_flows_in(void) const;

    /**
      * The pre_order_traversal method is called by the #traversal
      * method before any code is generated.
      *
      * @param sym
      *     The label symbol being defined
      */
    virtual void pre_order_traversal(const symbol_label::pointer &sym) const;

    /**
      * The post_order_traversal method is called by the #traversal
      * method after the code is generated.
      */
    virtual void post_order_traversal(void) const;

private:
    /**
      * The sym instance variable is used to remember the label symbol
      * being defined.
      */
    symbol_label::pointer sym;

    /**
      * The body instance variable is used to remember the statement
      * following the label.
      */
    statement::pointer body;

    /**
      * The default constructor.  Do not use.
      */
    statement_label();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_label(const statement_label &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_label &operator=(const statement_label &rhs);
};

#endif // LIB_STATEMENT_LABEL_H
// vim: set ts=8 sw=4 et :

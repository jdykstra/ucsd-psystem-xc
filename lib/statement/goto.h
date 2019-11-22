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

#ifndef LIB_STATEMENT_GOTO_H
#define LIB_STATEMENT_GOTO_H

#include <lib/statement.h>
#include <lib/symbol/label.h>

/**
  * The statement_goto class is used to represent an abstract function
  * that performs a GOTO.
  */
class statement_goto:
    public statement
{
public:
    typedef boost::shared_ptr<statement_goto> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_goto();

    /**
      * The get_label method is used to obtain the label associated with
      * this statement.  This permits optimizations of the IF statement.
      */
    label::pointer get_label(void) const;

protected:
    /**
      * The constructor.
      * For the use of derived classes only.
      *
      * @param sp
      *     The label symbol to jump to.
      */
    statement_goto(const symbol::pointer &sp);

    // See base class for documentation;
    void traversal(void) const;

    // See base class for documentation;
    bool execution_flows_out(void) const;

    /**
      * The post_order_traversal method is called by the #traversal method
      * after it has finished all other processing of the label.
      */
    virtual void post_order_traversal(const symbol::pointer &sp) const;

private:
    /**
      * The sym instance variable is used to remember the slabel symbol
      * referred to.
      */
    symbol_label::pointer sym;

    /**
      * The default constructor.  Do not use.
      */
    statement_goto();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_goto(const statement_goto &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_goto &operator=(const statement_goto &rhs);
};

#endif // LIB_STATEMENT_GOTO_H
// vim: set ts=8 sw=4 et :

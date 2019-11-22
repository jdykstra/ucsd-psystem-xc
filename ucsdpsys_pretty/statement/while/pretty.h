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

#ifndef UCSDPSYS_PRETTY_STATEMENT_WHILE_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_WHILE_PRETTY_H

#include <lib/statement/while.h>

class translator_pretty; // forward

/**
  * The statement_while_pretty class is used to represent pretty
  * printing a while statement.
  */
class statement_while_pretty:
    public statement_while
{
public:
    typedef boost::shared_ptr<statement_while_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_while_pretty();

private:
    /**
      * The constructor.
      */
    statement_while_pretty(const expression::pointer &condition,
        const statement::pointer &body, translator_pretty *cntxt);

public:
    static pointer create(const expression::pointer &condition,
        const statement::pointer &body, translator_pretty *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void do_order_traversal() const;

    // See base class for documentation.
    void post_order_traversal() const;

private:
    /**
      * The pretty instance variable is used to remember where to send
      * our pretty text.
      */
    translator_pretty &pretty;

    /**
      * The default constructor.  Do not use.
      */
    statement_while_pretty();

    /**
      * The copy constructor.  Do not use.
      */
    statement_while_pretty(const statement_while_pretty &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_while_pretty &operator=(const statement_while_pretty &);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_WHILE_PRETTY_H
// vim: set ts=8 sw=4 et :

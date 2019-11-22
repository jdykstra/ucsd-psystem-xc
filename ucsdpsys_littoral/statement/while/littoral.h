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

class translator_littoral; // forward

/**
  * The statement_while_littoral class is used to represent littoral
  * printing a while statement.
  */
class statement_while_littoral:
    public statement_while
{
public:
    typedef boost::shared_ptr<statement_while_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_while_littoral();

private:
    /**
      * The constructor.
      */
    statement_while_littoral(const expression::pointer &condition,
        const statement::pointer &body, translator_littoral *cntxt);

public:
    static pointer create(const expression::pointer &condition,
        const statement::pointer &body, translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void do_order_traversal() const;

    // See base class for documentation.
    void post_order_traversal() const;

private:
    /**
      * The littoral instance variable is used to remember where to send
      * our littoral text.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_while_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    statement_while_littoral(const statement_while_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_while_littoral &operator=(const statement_while_littoral &);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_WHILE_PRETTY_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_PRETTY_STATEMENT_IF_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_IF_PRETTY_H

#include <lib/statement/if.h>

class translator_pretty; // forward

/**
  * The statement_if_pretty class is used to represent pretty printing
  * an "if" statement.
  */
class statement_if_pretty:
    public statement_if
{
public:
    typedef boost::shared_ptr<statement_if_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_if_pretty();

private:
    /**
      * The constructor.
      */
    statement_if_pretty(const expression::pointer &a_cond,
        const statement::pointer &a_thn,
        const statement::pointer &a_els, translator_pretty *cntxt);

public:
    static pointer create(const expression::pointer &a_cond,
        const statement::pointer &a_thn, const statement::pointer &a_els,
        translator_pretty *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void then_order_traversal() const;

    // See base class for documentation.
    void else_order_traversal() const;

    // See base class for documentation.
    void post_order_traversal() const;

private:
    /**
      * The pretty instance variable is used to remember where to semd
      * out pretty source code.
      */
    translator_pretty &pretty;

    /**
      * The default constructor.
      */
    statement_if_pretty();

    /**
      * The copy constructor.
      */
    statement_if_pretty(const statement_if_pretty &);

    /**
      * The assignment operator.
      */
    statement_if_pretty &operator=(const statement_if_pretty &);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_IF_PRETTY_H
// vim: set ts=8 sw=4 et :

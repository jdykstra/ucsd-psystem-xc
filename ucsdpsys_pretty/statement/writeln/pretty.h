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

#ifndef UCSDPSYS_PRETTY_STATEMENT_WRITELN_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_WRITELN_PRETTY_H

#include <lib/statement/writeln.h>

class translator_pretty; // forward

/**
  * The statement_writeln_pretty class is used to represent pretty
  * printing a writeln procedure call statement.
  */
class statement_writeln_pretty:
    public statement_writeln
{
public:
    typedef boost::shared_ptr<statement_writeln_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_writeln_pretty();

private:
    /**
      * The constructor.
      */
    statement_writeln_pretty(const expression_list &args, bool eoln,
        translator_pretty *cntxt);

public:
    static pointer create(const expression_list &args, bool eoln,
        translator_pretty *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void infix_order_traversal(const expression::pointer &ep, bool last) const;

    // See base class for documentation.
    void post_order_traversal() const;

private:
    /**
      * The eoln instance variable is used to remember if this is
      * writeln (true) or write (false).
      */
    bool eoln;

    /**
      * The pretty instance variable is used to remember where to write
      * our output.
      */
    translator_pretty &pretty;

    /**
      * The default constructor.  Do not use.
      */
    statement_writeln_pretty();

    /**
      * The copy constructor.  Do not use.
      */
    statement_writeln_pretty(const statement_writeln_pretty &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_writeln_pretty &operator=(const statement_writeln_pretty &);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_WRITELN_PRETTY_H
// vim: set ts=8 sw=4 et :

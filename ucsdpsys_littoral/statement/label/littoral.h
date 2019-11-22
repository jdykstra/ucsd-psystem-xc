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

#ifndef UCSDPSYS_PRETTY_STATEMENT_LABEL_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_LABEL_PRETTY_H

#include <lib/statement/label.h>

class translator_littoral; // forward

/**
  * The statement_label_littoral class is used to represent the prcessing
  * required to littoral print a label statement.
  */
class statement_label_littoral:
    public statement_label
{
public:
    typedef boost::shared_ptr<statement_label_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_label_littoral();

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param sym
      *     The label symbol being defined
      * @param body
      *     The statement following the label.
      * @param cntxt
      *     The translator context we are working within.
      */
    statement_label_littoral(const symbol::pointer &sym,
        const statement::pointer &body, translator_littoral *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param sym
      *     The label symbol being defined
      * @param body
      *     The statement following the label.
      * @param cntxt
      *     The translator context we are working within.
      */
    static pointer create(const symbol::pointer &sym,
        const statement::pointer &body, translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal(const symbol_label::pointer &sym) const;

private:
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_label_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_label_littoral(const statement_label_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_label_littoral &operator=(const statement_label_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_LABEL_PRETTY_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_DEREFERENCE_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_DEREFERENCE_PRETTY_H

#include <lib/expression/dereference.h>

class translator_pretty; // forward

/**
  * The expression_dereference_pretty class is used to represent pretty
  * printing of a dereference expression.
  */
class expression_dereference_pretty:
    public expression_dereference
{
public:
    typedef boost::shared_ptr<expression_dereference_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_dereference_pretty();

private:
    /**
      * The constructor.
      */
    expression_dereference_pretty(const expression::pointer &operand,
        translator_pretty *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &operand,
        translator_pretty *cntxt);

protected:
    // See base class for doumentation.
    void pre_order_traversal(int pprec) const;

    // See base class for doumentation.
    void post_order_traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The pretty instance variable is used to remember where to send
      * our pretty source code.
      */
    translator_pretty &pretty;

    /**
      * The default constructor.  Do not use.
      */
    expression_dereference_pretty();

    /**
      * The copy constructor.  Do not use.
      */
    expression_dereference_pretty(const expression_dereference_pretty &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_dereference_pretty &operator=(
        const expression_dereference_pretty &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_DEREFERENCE_PRETTY_H
// vim: set ts=8 sw=4 et :

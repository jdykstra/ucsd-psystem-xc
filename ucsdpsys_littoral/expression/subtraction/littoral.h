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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_SUBTRACTION_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_SUBTRACTION_PRETTY_H

#include <lib/expression/subtraction.h>

class translator_littoral; // forward

/**
  * The expression_subtraction_littoral class is used to represent littoral
  * printing a subtraction expression node.
  */
class expression_subtraction_littoral:
    public expression_subtraction
{
public:
    typedef boost::shared_ptr<expression_subtraction_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_subtraction_littoral();

private:
    /**
      * The constructor.
      */
    expression_subtraction_littoral(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_littoral *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_littoral *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal(int pprec) const;

    // See base class for documentation.
    void infix_order_traversal(int pprec) const;

    // See base class for documentation.
    void post_order_traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize() const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    bool is_constant(void) const;

private:
    /**
      * The littoral instance variable is used to remember where to send
      * our littoral printed text.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    expression_subtraction_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    expression_subtraction_littoral(const expression_subtraction_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_subtraction_littoral &operator=(
        const expression_subtraction_littoral &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_SUBTRACTION_PRETTY_H
// vim: set ts=8 sw=4 et :

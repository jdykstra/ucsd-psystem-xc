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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_IN_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_IN_PRETTY_H

#include <lib/expression/in.h>

class translator_littoral; // forward

/**
  * The expression_in_littoral class is used to represent the processing
  * necessary to littoral print an IN expression.
  */
class expression_in_littoral:
    public expression_in
{
public:
    typedef boost::shared_ptr<expression_in_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_in_littoral();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The left hand side of the IN expression (the value to test)
      * @param rhs
      *     The right hand side of the IN expression (the set)
      * @param littoral
      *     The translator context we are working within.
      */
    expression_in_littoral(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_littoral *littoral);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The left hand side of the IN expression (the value to test)
      * @param rhs
      *     The right hand side of the IN expression (the set)
      * @param littoral
      *     The translator context we are working within.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_littoral *littoral);

protected:
    // See base class for documentation.
    void pre_order_traversal(int pprec) const;

    // See base class for documentation.
    void infix_order_traversal(int pprec) const;

    // See base class for documentation.
    void post_order_traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The littoral instance variable is used to remember the translator
      * context we are working within.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    expression_in_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_in_littoral(const expression_in_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_in_littoral &operator=(const expression_in_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_IN_PRETTY_H
// vim: set ts=8 sw=4 et :

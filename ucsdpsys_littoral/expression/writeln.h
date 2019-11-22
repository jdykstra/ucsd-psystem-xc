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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_WRITELN_H
#define UCSDPSYS_PRETTY_EXPRESSION_WRITELN_H

#include <lib/expression.h>

class translator_littoral; // forward

/**
  * The expression_writeln class is used to represent
  * the arg:arg construct for writeln expressions.
  */
class expression_writeln:
    public expression
{
public:
    typedef boost::shared_ptr<expression_writeln> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_writeln();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param arg1
      *     The left hand parameter
      * @param arg2
      *     The right hand parameter
      * @param littoral
      *     The context we are working in.
      */
    expression_writeln(const expression::pointer &arg1,
        const expression::pointer &arg2, translator_littoral *littoral);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param arg1
      *     The left hand parameter
      * @param arg2
      *     The right hand parameter
      * @param littoral
      *     The context we are working in.
      */
    static pointer create(const expression::pointer &arg1,
        const expression::pointer &arg2, translator_littoral *littoral);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    expression::pointer lhs;
    expression::pointer rhs;
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    expression_writeln();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_writeln(const expression_writeln &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_writeln &operator=(const expression_writeln &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_WRITELN_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_NAME_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_NAME_PRETTY_H

#include <lib/expression/name.h>

class translator_pretty; // forward

/**
  * The expression_name_pretty class is used to represent pretty
  * printing a name expression node.
  */
class expression_name_pretty:
    public expression_name
{
public:
    typedef boost::shared_ptr<expression_name_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_name_pretty();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The location of the symbol reference
      * @param sp
      *     Pointer to the referenced symbol
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const location &locn, const symbol::pointer &sp,
        translator_pretty *context);

protected:
    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer assignment_expression_factory(
        const expression::pointer &rhs) const;

    // See base class for documentation.
    expression::pointer array_index_expression_factory(
        const expression::pointer &rhs) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The constructor.
      *
      * @param locn
      *     The location of the symbol reference
      * @param sp
      *     Pointer to the referenced symbol
      * @param context
      *     The translation context we are working within.
      */
    expression_name_pretty(const location &locn, const symbol::pointer &sp,
        translator_pretty *context);

    /**
      * The pretty instance variable is used to remember where to pretty
      * print this expression node.
      */
    translator_pretty &pretty;

    /**
      * The vtype instance variable is used to remember the type of this
      * name, but with any "reference to ..." stripped away.
      */
    type::pointer vtype;

    expression::pointer clone(void) const;

    /**
      * The default constructor.
      */
    expression_name_pretty();

    /**
      * The copy constructor.
      */
    expression_name_pretty(const expression_name_pretty &);

    /**
      * The assignment operator.
      */
    expression_name_pretty &operator=(const expression_name_pretty &);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_NAME_PRETTY_H
// vim: set ts=8 sw=4 et :

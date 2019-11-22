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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_CAST_H
#define UCSDPSYS_COMPILE_EXPRESSION_CAST_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_cast class is used to represent converting an
  * expression from one type to another, but where no opecodes are
  * necessary to accomplish this feat.
  */
class expression_cast:
    public expression
{
public:
    typedef boost::shared_ptr<expression_cast> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_cast();

private:
    /**
      * The constructor.
      *
      * @param to
      *     the data type to convert the expression to.
      * @param from
      *     the expression to be converted.
      * @param cntxt
      *     where to send our generated code ;-)
      */
    expression_cast(const type::pointer &to, const expression::pointer &from,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param to
      *     the data type to convert the expression to.
      * @param from
      *     the expression to be converted.
      * @param cntxt
      *     where to send our generated code ;-)
      */
    static pointer create(const type::pointer &to,
        const expression::pointer &from, translator_compile *cntxt);

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
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code ;-)
      */
    translator_compile &code;

    /**
      * The to instance variable is used to remember the data type to
      * convert the expression to.
      */
    type::pointer to;

    /**
      * The from instance variable is used to remember the expression to
      * be converted.
      */
    expression::pointer from;

    /**
      * The default constructor.  Do not use.
      */
    expression_cast();

    /**
      * The copy constructor.  Do not use.
      */
    expression_cast(const expression_cast &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_cast &operator=(const expression_cast &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_CAST_H
// vim: set ts=8 sw=4 et :

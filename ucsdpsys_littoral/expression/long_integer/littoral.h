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

#ifndef UCSDPSYS_PRETTY_EXPRESSION_LONG_INTEGER_PRETTY_H
#define UCSDPSYS_PRETTY_EXPRESSION_LONG_INTEGER_PRETTY_H

#include <lib/expression/long_integer.h>

class translator_littoral; // forward

/**
  * The expression_long_integer_littoral class is used to represent the
  * prcessing needed to littoral-print a long integer constant.
  */
class expression_long_integer_littoral:
    public expression_long_integer
{
public:
    typedef boost::shared_ptr<expression_long_integer_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_long_integer_littoral();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The location of this expression in the sourec code.
      * @param value
      *     The original text of the constant.
      * @param littoral
      *     The translation context we are working within.
      */
    static pointer create(const location &locn, const rcstring &value,
        translator_littoral *littoral);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param locn
      *     The location of this expression in the sourec code.
      * @param value
      *     The original text of the constant.
      * @param littoral
      *     The translation context we are working within.
      */
    expression_long_integer_littoral(const location &locn,
        const rcstring &value, translator_littoral *littoral);

    /**
      * The original isntance variable is used to remember the original
      * text of the constant, so that it can be preproduced exactly the
      * way it was written in the input.
      */
    rcstring original;

    /**
      * The littoral instance variable is used to remember the translation
      * context we are working within.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    expression_long_integer_littoral();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_long_integer_littoral(
        const expression_long_integer_littoral &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_long_integer_littoral &operator=(
        const expression_long_integer_littoral &rhs);
};

#endif // UCSDPSYS_PRETTY_EXPRESSION_LONG_INTEGER_PRETTY_H
// vim: set ts=8 sw=4 et :

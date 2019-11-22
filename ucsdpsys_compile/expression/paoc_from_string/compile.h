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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_PAOC_FROM_STRING_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_PAOC_FROM_STRING_COMPILE_H

#include <lib/expression/paoc_from_string.h>

class translator_compile; // forward

/**
  * The expression_paoc_from_string_compile class is used to represent
  * the code generation of a cast from a string constant to a packed
  * array of char constant.
  */
class expression_paoc_from_string_compile:
    public expression_paoc_from_string
{
public:
    typedef boost::shared_ptr<expression_paoc_from_string_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_paoc_from_string_compile();

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param rhs
      *     The value to cast.
      * @param tp
      *     The exact type of the result.
      * @param code
      *     The translation context we are working within.
      */
    expression_paoc_from_string_compile(const expression::pointer &rhs,
        const type::pointer &tp, translator_compile *code);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param rhs
      *     The value to cast.
      * @param tp
      *     The exact type of the result.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &rhs,
        const type::pointer &tp, translator_compile *code);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The code instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_paoc_from_string_compile();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_paoc_from_string_compile(
        const expression_paoc_from_string_compile &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_paoc_from_string_compile &operator=(
        const expression_paoc_from_string_compile &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_PAOC_FROM_STRING_COMPILE_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_STB_H
#define UCSDPSYS_COMPILE_EXPRESSION_STB_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_stb class is used to represent the STB (store byte)
  * opcode, used to assign characters by index within a string.
  */
class expression_stb:
    public expression
{
public:
    typedef boost::shared_ptr<expression_stb> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_stb();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param lhs
      *     The byte pointer to indirect store.
      * @param rhs
      *     The value to assign to that string index.
      * @param code
      *     The translation context we are working within.
      */
    static pointer create(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param lhs
      *     The byte pointer to indirect store into.
      * @param rhs
      *     The value to assign to that string index.
      * @param code
      *     The translation context we are working within.
      */
    expression_stb(const expression::pointer &lhs,
        const expression::pointer &rhs, translator_compile *code);

    /**
      * The lhs instance variable is used to remember the byte pointer
      * to indirect store into.
      */
    expression::pointer lhs;

    /**
      * The rhs instance variable is used to remember the value to
      * store intto the byte pointed to by the byte pointer.
      */
    expression::pointer rhs;

    /**
      * The code instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_stb();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_stb(const expression_stb &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_stb &operator=(const expression_stb &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_STB_H
// vim: set ts=8 sw=4 et :

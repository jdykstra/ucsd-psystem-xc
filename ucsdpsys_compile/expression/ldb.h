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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LDB_H
#define UCSDPSYS_COMPILE_EXPRESSION_LDB_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_ldb class is used to represent the LDB (load byte)
  * opcode, used to index into char arrays and byte arrays.
  */
class expression_ldb:
    public expression
{
public:
    typedef boost::shared_ptr<expression_ldb> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ldb();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param operand
      *     the byte pointer to indirect load
      * @param code
      *     the translation context we are working within.
      */
    static pointer create(const expression::pointer &operand,
        translator_compile *code);

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

    // See base class for documentation.
    expression::pointer assignment_expression_factory(
        const expression::pointer &rhs) const;

    // See base class for documentation.
    expression::pointer get_byte_pointer(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param operand
      *     the byte pointer to indirect load
      * @param code
      *     the translation context we are working within.
      */
    expression_ldb(const expression::pointer &operand,
        translator_compile *code);

    /**
      * The lhs instance variable is used to remember the byte pointer
      * to indirect load.
      */
    expression::pointer operand;

    /**
      * The code instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_ldb();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_ldb(const expression_ldb &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_ldb &operator=(const expression_ldb &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LDB_H
// vim: set ts=8 sw=4 et :

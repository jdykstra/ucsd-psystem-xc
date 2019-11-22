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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LPA_H
#define UCSDPSYS_COMPILE_EXPRESSION_LPA_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_lpa class is used to represent the code generation
  * required for the LPA (Load Packed Address) opcode.
  */
class expression_lpa:
    public expression
{
public:
    typedef boost::shared_ptr<expression_lpa> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_lpa();

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    expression_lpa(const void *data, size_t data_size,
        const location &locn, const type::pointer &atype,
        translator_compile *code);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const void *data, size_t data_size,
        const location &locn, const type::pointer &atype,
        translator_compile *code);

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
    unsigned char *data;
    size_t data_size;
    type::pointer atype;
    translator_compile &code;

    /**
      * The defualt constructor.  Do not use.
      */
    expression_lpa();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_lpa(const expression_lpa &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_lpa &operator=(const expression_lpa &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LPA_H
// vim: set ts=8 sw=4 et :

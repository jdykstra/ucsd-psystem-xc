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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LOAD_INDIRECT_PACKED_H
#define UCSDPSYS_COMPILE_EXPRESSION_LOAD_INDIRECT_PACKED_H

#include <ucsdpsys_compile/expression/load.h>

class translator_compile; // forward

/**
  * The expression_load_indirect_packed class is used to represent an
  * indirect load expression, via a packed pointer expression.
  */
class expression_load_indirect_packed:
    public expression_load
{
public:
    typedef boost::shared_ptr<expression_load_indirect_packed> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_load_indirect_packed();

private:
    /**
      * The constructor.
      */
    expression_load_indirect_packed(const expression::pointer &operand,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &operand,
        translator_compile *cntxt);

    // See base class for documentation.
    void post_order_traversal() const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer get_byte_pointer(void) const;

    // See base class for documentation.
    expression::pointer evaluate() const;

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    expression::pointer assignment_expression_factory(
        const expression::pointer &rhs) const;

    // See base class for documentation.
    expression::pointer strip_indirection() const;

    // See base class for documentation.
    symbol::pointer get_symbol_hint() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The operand instance variable is used to remember the address to
      * be indirected.
      */
    expression::pointer operand;

    /**
      * The default constructor.  Do not use.
      */
    expression_load_indirect_packed();

    /**
      * The copy constructor.  Do not use.
      */
    expression_load_indirect_packed(const expression_load_indirect_packed &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_load_indirect_packed &operator=(
        const expression_load_indirect_packed &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LOAD_INDIRECT_PACKED_H
// vim: set ts=8 sw=4 et :

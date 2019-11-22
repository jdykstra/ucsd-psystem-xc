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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LOAD_ADDRESS_H
#define UCSDPSYS_COMPILE_EXPRESSION_LOAD_ADDRESS_H

#include <lib/expression.h>

class symbol; // forward
class translator_compile; // forward

/**
  * The expression_load_address class is used to represent code
  * generation to load an address onto the stack.
  */
class expression_load_address:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_load_address();

protected:
    /**
      * The constructor.
      */
    expression_load_address(const location &locn, const symbol::pointer &sp,
        translator_compile *cntxt);

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
    rcstring lisp_representation(void) const;

private:
    translator_compile &code;
    symbol::pointer sp;

    /**
      * The default constructor.
      */
    expression_load_address();

    /**
      * The copy constructor.
      */
    expression_load_address(const expression_load_address &);

    /**
      * The assignment operator.
      */
    expression_load_address &operator=(const expression_load_address &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LOAD_ADDRESS_H
// vim: set ts=8 sw=4 et :

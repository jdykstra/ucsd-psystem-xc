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

#ifndef UCSDPSYS_ASSEMBLE_EXPRESSION_SPECIAL_H
#define UCSDPSYS_ASSEMBLE_EXPRESSION_SPECIAL_H

#include <lib/expression.h>

/**
  * The expression_special class is used to represent an assembler
  * special symbol.  This is normally used to CPU register names.
  */
class expression_special:
    public expression
{
public:
    typedef boost::shared_ptr<expression_special> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_special();

    /**
      * The get_name method may be used to obtain the name of the
      * special symbol.
      */
    virtual rcstring get_name(void) const = 0;

protected:
    /**
      * The constructor.
      * For use by derived classs only.
      *
      * @param locn
      *     source code location of the special symbol
      */
    expression_special(const location &locn);

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
    rcstring lisp_representation(void) const;

private:
    /**
      * The default constructor.  Do not use.
      */
    expression_special();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_special(const expression_special &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_special &operator=(const expression_special &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_EXPRESSION_SPECIAL_H
// vim: set ts=8 sw=4 et :

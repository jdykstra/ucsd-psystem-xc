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

#ifndef LIB_EXPRESSION_NAME_H
#define LIB_EXPRESSION_NAME_H

#include <lib/expression.h>

class symbol; // forward

/**
  * The expression_name class is used to represent an abstract
  * expression which references a named symbol.
  */
class expression_name:
    public expression
{
public:
    typedef boost::shared_ptr<expression_name> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_name();

    /**
      * The get_sp method is used to obtain a pointer to the symbol
      * referenced by this name.
      */
    symbol::pointer get_sp(void) const { return sp; }

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param locn
      *     The location of the symbol reference
      * @param sp
      *     Pointer to the referenced symbol
      */
    expression_name(const location &locn, const symbol::pointer &sp);

    // See base class for documentation.
    int get_precedence(void) const;

    // See base class for documentation.
    type::pointer get_type(void) const;

    // See base class for documentation.
    bool is_lvalue(void) const;

    // See base class for documentation.
    side_effect_t has_side_effect(void) const;

    // See base class for documentation.
    expression::pointer function_call_expression_factory(
        const expression_list &args) const;

    // See base class for documentation.
    bool is_constant(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    rcstring get_description(void) const;

private:
    /**
      * The sp instance variable is used to reember the symbol
      * referenced by this expression node.
      */
    symbol::pointer sp;

    /**
      * The copy constructor.
      */
    expression_name(const expression_name &);

    /**
      * The default constructor.  Do not use.
      */
    expression_name();

    /**
      * The assignment operator.  Do not use.
      */
    expression_name &operator=(const expression_name &);
};

#endif // LIB_EXPRESSION_NAME_H
// vim: set ts=8 sw=4 et :

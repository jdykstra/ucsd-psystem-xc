//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2012 Peter Miller
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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LOAD_H
#define UCSDPSYS_COMPILE_EXPRESSION_LOAD_H

#include <lib/expression.h>

/**
  * The expression_load class is used to represent an abstract load
  * expression.  Derivations take a variety of forms, including direct
  * and indirect loads, but not constants.
  */
class expression_load:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_load();

protected:
    /**
      * The constructor.
      * (You must derive from this class.)
      */
    expression_load(const location &locn, const type::pointer &operand);

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

private:
    /**
      * The var_type instance variable is used to remember the address of
      * the variable to be loaded.
      */
    type::pointer var_type;

    /**
      * The default constructor.  Do not use.
      */
    expression_load();

    /**
      * The copy constructor.  Do not use.
      */
    expression_load(const expression_load &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_load &operator=(const expression_load &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LOAD_H
// vim: set ts=8 sw=4 et :

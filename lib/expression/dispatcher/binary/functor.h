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

#ifndef LIB_EXPRESSION_DISPATCHER_BINARY_FUNCTOR_H
#define LIB_EXPRESSION_DISPATCHER_BINARY_FUNCTOR_H

#include <lib/expression.h>

/**
  * The expression_dispatcher_binary_functor class is used to represent
  * an action to perform when an expression_dispatcher_binary finds a match.
  */
class expression_dispatcher_binary_functor
{
public:
    typedef boost::shared_ptr<expression_dispatcher_binary_functor> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_dispatcher_binary_functor();

    /**
      * The action method is used to perform the action of the functor.
      *
      * @param lhs
      *     The left hand side of the expression.
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     pointer to a new pointer node in dynamic memory
      */
    virtual expression::pointer action(const expression::pointer &lhs,
        const expression::pointer &rhs) const = 0;

    /**
      * The function call operator may be used to perform the action of functor.
      * It looks like a function, that's why it's called a functor.
      *
      * @param lhs
      *     The left hand side of the expression.
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     pointer to a new pointer node in dynamic memory
      */
    expression::pointer
    operator()(const expression::pointer &lhs, const expression::pointer &rhs)
        const
    {
        return action(lhs, rhs);
    }

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    expression_dispatcher_binary_functor();

private:
    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_dispatcher_binary_functor(
        const expression_dispatcher_binary_functor &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_dispatcher_binary_functor &operator=(
        const expression_dispatcher_binary_functor &rhs);
};

#endif // LIB_EXPRESSION_DISPATCHER_BINARY_FUNCTOR_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_EXPRESSION_DISPATCHER_BINARY_H
#define LIB_EXPRESSION_DISPATCHER_BINARY_H

#include <lib/ac/list>

#include <lib/expression.h>
#include <lib/expression/dispatcher/binary/functor.h>

/**
  * The expression_dispatcher_binary class is used to represent a list
  * of expression factories, to be conditionally applied when a pair of
  * expressions is supplied.  The first match is used.
  */
class expression_dispatcher_binary
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_dispatcher_binary();

    /**
      * The constructor.
      */
    expression_dispatcher_binary(const char *operator_name);

    /**
      * The test_t type is a convenience typedef, to make the code
      * easier to read.  It is a pointer to a function.
      *
      * @param arg
      *     The expression to be tested.  Usually this test for type,
      *     but it could test for l-values, or anything else.
      * @returns
      *     bool; true if the test is fulfilled, false if not.
      */
    typedef bool (*test_t)(const expression::pointer &arg);

    /**
      * The push_back method is used to append another
      * {test,method,test} tuple to the end of the list of
      * possibilities.
      *
      * @param lhs_test
      *     The test to be applied to the left hand expression.
      * @param factory
      *     The translator factory method to be called if the left and
      *     right tests succeed.
      * @param rhs_test
      *     The test to be applied to the right hand expression.
      */
    void push_back(test_t lhs_test,
        const expression_dispatcher_binary_functor::pointer &factory,
        test_t rhs_test);

    /**
      * The dispatch method is used to manufacture a new expression node
      * instance, based on the dispatch table built by push_back method
      * calls, and the expressions given.
      *
      * If either of the left or right expressions are an error
      * expression, an error expression will be returned.
      *
      * If none of the rows in the dispatch table match, an error will
      * be reported via pascal_lex_error, and an error expression will
      * be returned.
      *
      * @param lhs
      *     The left hand expression of the new expression node.
      * @param rhs
      *     The right hand expression of the new expression node.
      * @returns
      *     a new expression node instance
      */
    expression::pointer dispatch(const expression::pointer &lhs,
        const expression::pointer &rhs) const;

    /**
      * The complain method is used to issue an error message about
      * the operands.  It is public because sometimes the functions we
      * dispatch too will make their own checks and eventually fail.
      *
      * @param lhs
      *     The left hand expression of the new expression node.
      * @param rhs
      *     The right hand expression of the new expression node.
      * @returns
      *     a new (error) expression node instance
      */
    expression::pointer complain(const expression::pointer &lhs,
        const expression::pointer &rhs) const;

private:
    /**
      * The dispatch_t class is used to represent a single row of the
      * dispatch table.
      */
    class dispatch_t
    {
    public:
        /**
          * The destructor.  It is not virtual; thou shalt not derive
          * from this class.
          */
        ~dispatch_t();

        /**
          * The default constructor.
          * May only be used by expression_dispatcher_binary::push_back
          */
        dispatch_t();

        /**
          * The constructor.
          *
          * @param lhs_test
          *     The condition that must be satisfied by the left hand
          *     side of the expression
          * @param factory
          *     what to do when both sides of the expression match
          * @param rhs_test
          *     The condition that must be satisfied by the right hand
          *     side of the expression
          */
        dispatch_t(test_t lhs_test,
            const expression_dispatcher_binary_functor::pointer &factory,
            test_t rhs_test);

        /**
          * The copy constructor.
          */
        dispatch_t(const dispatch_t &arg);

        /**
          * The assignment operator.
          */
        dispatch_t &operator=(const dispatch_t &arg);

        /**
          * The matches method is used to determine whether or not the
          * give pair of left and right expressions match this dispatch
          * table row.
          *
          * @param lhs
          *     The left hand expression to be matched.
          * @param rhs
          *     The right hand expression to be matched.
          * @returns
          *     bool; true if both expressions match, false if not
          */
        bool matches(const expression::pointer &lhs,
            const expression::pointer &rhs) const;

        /**
          * The dispatch method is used to manufacture a new expression
          * node instance from the supplied left and right operands.  It
          * is assumed that matches(lhs, rhs) has already returned true.
          *
          * @param lhs
          *     The left hand side of the expression to be manufactured.
          * @param rhs
          *     The right hand side of the expression to be manufactured.
          * @returns
          *     a new expression node instance
          */
        expression::pointer dispatch(const expression::pointer &lhs,
            const expression::pointer &rhs) const;

    private:
        /**
          * The lhs_test instance variable is used to remember the test
          * to be applied to the left hand expression.
          */
        test_t lhs_test;

        /**
          * The factory instance variable is used to remember the
          * translator method to be called to manufacture the new
          * expession instance.
          */
        expression_dispatcher_binary_functor::pointer factory;

        /**
          * The rhs_test instance variable is used to remember the test
          * to be applied to the right hand expression.
          */
        test_t rhs_test;
    };

    /**
      * The push_back method is used to add another row to the end of
      * the dispatch table.
      *
      * @param arg
      *     The row to be appended.
      */
    void push_back(const dispatch_t &arg);

    /**
      * The operator_name instance variable is used to remember the name
      * of the operator this dispatch table applies to.  This name will
      * be used for error messages.
      */
    const char *operator_name;

    typedef std::list<dispatch_t> items_t;

    /**
      * The items instance variable is used to remember the base
      * address of the dynamically allocated array of dispatch_t rows.
      */
    items_t items;

    /**
      * The default constructor.
      */
    expression_dispatcher_binary();

    /**
      * The copy constructor.
      */
    expression_dispatcher_binary(const expression_dispatcher_binary &);

    /**
      * The assignment operator.
      */
    expression_dispatcher_binary &operator=(
        const expression_dispatcher_binary &);
};

#endif // LIB_EXPRESSION_DISPATCHER_BINARY_H
// vim: set ts=8 sw=4 et :

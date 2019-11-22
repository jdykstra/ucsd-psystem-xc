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

#ifndef LIB_EXPRESSION_DISPATCHER_UNARY_H
#define LIB_EXPRESSION_DISPATCHER_UNARY_H


#include <lib/ac/stddef.h>

#include <lib/expression.h>
class translator; // forward

/**
  * The expression_dispatcher_unary class is used to represent a list
  * of expression factories, to be conditionally applied when a pair of
  * expressions is supplied.  The first match is used.
  */
class expression_dispatcher_unary
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_dispatcher_unary();

    /**
      * The constructor.
      */
    expression_dispatcher_unary(const char *operator_name);

    /**
      * The test_t type is a convenience typedef, to make the code
      * easier to read.  It is a pointer to a function.
      *
      * @param operand
      *     The expression to be tested.  Usually this test for type,
      *     but it could test for l-values, or anything else.
      * @returns
      *     bool; true if the test is fulfilled, false if not.
      */
    typedef bool (*test_t)(const expression::pointer &operand);

    /**
      * The factory_t is a convenience typedef, to make the code easier
      * to read.  It is a pointer to a class translator method.
      *
      * @param operand
      *     The operand expression of the new expression node.
      * @returns
      *     a new expression node instance
      */
    typedef expression::pointer (translator::*factory_t)(
        const expression::pointer &operand);

    /**
      * The push_back method is used to append another {test, method}
      * tuple to the end of the list of possibilities.
      *
      * @param operand_test
      *     The test to be applied to the left hand expression.
      * @param factory
      *     The translator factory method to be called if the left and
      *     right tests succeed.
      */
    void push_back(test_t operand_test, factory_t factory);

    /**
      * The dispatch method is used to manufacture a new expression node
      * instance, based on the dispatch table built by push_back method
      * calls, and the expression given.
      *
      * If either of the left or right expressions are an error
      * expression, an error expression will be returned.
      *
      * If none of the rows in the dispatch table match, an error will
      * be reported via pascal_lex_error, and an error expression will
      * be returned.
      *
      * @param operand
      *     The operand of the new expression node.
      * @param context
      *     The translator context for the new expression node.
      * @returns
      *     a new expression node instance
      */
    expression::pointer dispatch(const expression::pointer &operand,
        translator *context) const;

    /**
      * The complain method is used to emit an error message when an
      * undispatch-able argument is given.  It is public because it is
      * also usefil to the functions we dispatch to.
      *
      * @param operand
      *     The operand of the new expression node.
      * @returns
      *     a new (error) expression node instance
      */
    expression::pointer complain(const expression::pointer &operand) const;

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
          * May only be used by expression_dispatcher_unary::push_back
          */
        dispatch_t();

        /**
          * The constructor.
          */
        dispatch_t(test_t operand, factory_t factory);

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
          * @param operand
          *     The operand expression to be matched.
          * @returns
          *     bool; true if both expressions match, false if not
          */
        bool matches(const expression::pointer &operand) const;

        /**
          * The dispatch method is used to manufacture a new expression
          * node instance from the supplied left and right operands.  It
          * is assumed that matches(lhs, rhs) has already returned true.
          *
          * @param operand
          *     The operand of the expression to be manufactured.
          * @param context
          *     The translator context for the new expression node.
          * @returns
          *     a new expression node instance
          */
        expression::pointer dispatch(const expression::pointer &operand,
            translator *context) const;

    private:
        /**
          * The operand_test instance variable is used to remember the
          * test to be applied to the operand expression.
          */
        test_t operand_test;

        /**
          * The factory instance variable is used to remember the
          * translator method to be called to manufacture the new
          * expession intsance.
          */
        factory_t factory;
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

    /**
      * The length instance variable is used to remember how many
      * entries are present in the dispatch table.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remember how many
      * entries have been allocated in the dispatch table.
      */
    size_t maximum;

    /**
      * The operator_name instance variable is used to remember the base
      * address of the dynamically allocated array of dispatch_t rows.
      */
    dispatch_t *item;

    /**
      * The default constructor.
      */
    expression_dispatcher_unary();

    /**
      * The copy constructor.
      */
    expression_dispatcher_unary(const expression_dispatcher_unary &);

    /**
      * The assignment operator.
      */
    expression_dispatcher_unary &operator=(
        const expression_dispatcher_unary &);
};

#endif // LIB_EXPRESSION_DISPATCHER_UNARY_H
// vim: set ts=8 sw=4 et :

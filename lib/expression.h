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

#ifndef LIB_EXPRESSION_H
#define LIB_EXPRESSION_H

#include <boost/shared_ptr.hpp>

#include <lib/label.h>
#include <lib/location.h>
#include <lib/symbol.h>
#include <lib/type.h>

class expression_list; // forward
class variable_name; // forward

/**
  * The abstract expression base class is used to represent any type of
  * expression.
  */
class expression
{
public:
    /**
      * The pointer type is to be used for all pointers to statements.
      */
    typedef boost::shared_ptr<expression> pointer;

    /**
      * The destructor.
      */
    virtual ~expression();

    /**
      * The traversal method may be used to traverse each of the
      * declaractions in the list.
      *
      * @param pprec
      *     The precedence of the parent expression node.
      */
    virtual void traversal(int pprec) const = 0;

    /**
      * The logical_traversal method is used to traverse an expression,
      * with the intention of using to to implement a logical condition,
      * as would be found for an "if" statement or a "for" statement, etc.
      *
      * @param pprec
      *     The precedence of the parent expression node.
      * @param true_branch
      *     Where to branch to if the expression is true
      * @param false_branch
      *     Where to branch to if the expression is false
      * @param fall_through_preference
      *     This is true if true expressions should fall out the bottom,
      *     rather than branch to the true_branch.  This is false if
      *     false expressions should fall out the bottom, rather than
      *     branch to the false_branch.
      * @note
      *     The default implementation simply calls the traversal
      *     method.  Compilers implementing short-circuit boolean
      *     evaluation will need to impliment this more completely.
      */
    virtual void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    /**
      * The get_precedence method is used to obtain the precedence of
      * this expression node.
      *
      * The highest precedence has the highest absolute value, the
      * lowest precedence gets the lowest absolute value.
      */
    virtual int get_precedence(void) const = 0;

    /**
      * The get_type method is used to get the type of this expression.
      */
    virtual type::pointer get_type(void) const = 0;

    /**
      * The get_location method is used to obtain the location of the
      * expression.
      */
    const location &get_location(void) const { return loc; }

    /**
      * The get_description method is sued by error messages
      * to provide a general description of this expression.
      * It includes the type, and possibly the kind.
      */
    virtual rcstring get_description(void) const;

    /**
      * The is_lvalue method is used to determine whether or not this
      * expression is suitable to appear on the left hand side of an
      * assignment expression.
      */
    virtual bool is_lvalue(void) const = 0;

    /**
      * The assignment_expression_factory method is used by the
      * assignment_expression method to build an assignment expression
      * node.
      *
      * The default implementation prints and error saying that the left
      * hand side of the assignment is inappropriate.
      *
      * @param rhs
      *     The right hand side of the assignment ("this" is the left
      *     hand side).
      *
      * @note
      *     The parser will delete this expression tree once the
      *     assignment tree has been manufactored.
      */
    virtual expression::pointer assignment_expression_factory(
        const expression::pointer &rhs) const;

    /**
      * The function_call_expression_factory method is used by the
      * function_call_expression method to build a function call
      * expression node.
      *
      * The default implementation prints and error saying that the left
      * hand side of the function_call is inappropriate.
      *
      * @param parms
      *     All of the parameters of the function call, including the
      *     function name as the first parameter (usually [0] == this).
      *
      * @note
      *     The parser will delete this expression tree once the
      *     function_call tree has been manufactured.
      */
    virtual expression::pointer function_call_expression_factory(
        const expression_list &parms) const;

    /**
      * The dot_expression_factory method is used to build a record
      * member reference expression.
      *
      * The default implementation prints and error saying that the left
      * hand side of the dot is inappropriate.
      *
      * @param locn
      *     The source code location of the right hand side.
      * @param rhs
      *     The symbol information corresponding to the member to access
      * @returns
      *     pointer to an expression node
      */
    virtual expression::pointer dot_expression_factory(const location &locn,
        const symbol::pointer &rhs) const;

    /**
      * The array_index_expression_factory method is used to build an array
      * index expression.
      *
      * The default implementation prints and error saying that the left
      * hand side of the array index expression is inappropriate.
      *
      * @param rhs
      *     The value of the array element to be indexed
      * @returns
      *     pointer to an expression node
      */
    virtual expression::pointer array_index_expression_factory(
        const expression::pointer &rhs) const;

    /**
      * The strip_indirection method is used to obtain the address
      * expression of an abstract indirection expression.  This is used
      * to pass variables by reference to functions and procedures.
      *
      * The default implementation returns a NULL pointer, because the
      * error has to be printed by the caller.
      *
      * @returns
      *     new epression node, or NULL on error.
      */
    virtual expression::pointer strip_indirection(void) const;

    enum side_effect_t
    {
        side_effect_no,
        side_effect_partial,
        side_effect_yes
    };

    /**
      * The has_side_effect method is used to determine if an expression
      * has any effect.  It is used by the parse to produce "statement
      * with no effect" warnings.
      */
    virtual side_effect_t has_side_effect(void) const = 0;

    /**
      * The is_constant method may be used to determine whether this
      * expression node is a constant, or this tree would evaluate to a
      * constant.
      *
      * The default implementation is conservative, and always returns false.
      *
      * @returns
      *     bool; true if constant, false if not.
      */
    virtual bool is_constant(void) const;

    /**
      * The execution_flows_out method is used to determine whether an
      * expression ever finished executing.  This is usually true (and
      * the default implimentation returns true) but some functions (the
      * halt function, for example) do not.
      */
    virtual bool execution_flows_out(void) const;

    /**
      * The optimize method may be used to evaluate constant
      * expressions, or substitute more efficient expressions.
      *
      * This is intended to be called by the translator::
      * expression_statement method, or similar, where the expression is
      * in its final form, and will get no deeper.  In other words, when
      * all available information has been provided.
      *
      * There is no default implementation, even though many optimize
      * implementations return clone(), because this gives the compiler
      * no opportunity to remind the authors of new classes that they
      * have forgotten something.
      *
      * @returns
      *     If the expression tree is not constant, or it is already as
      *     reduced as it can get, it returns itself.  Otherwise, it
      *     returns a new expression node.
      */
    virtual expression::pointer optimize(void) const = 0;

    /**
      * The get_boolean_value method is used to obtain the boolean value
      * of an expression node.  It is an error, and the results are
      * undefined (probably plays electro punk music), if this is not a
      * constant boolean expression node.
      *
      * @note
      *     You may have to #optimize first, to fold integer constants,
      *     even when #is_constant returns true.  Otherwise it will
      *     simply return false.
      */
    virtual bool get_boolean_value(void) const;

    /**
      * The get_integer_value method is used to obtain the integer value
      * of an expression node.  It is an error, and the results are
      * undefined (probably rm -rf or worse), if this is not a constant
      * integer expression node.
      *
      * @note
      *     You may have to #optimize first, to fold integer constants,
      *     even when #is_constant returns true.
      */
    virtual long get_integer_value(void) const;

    /**
      * The get_real_value method is used to obtain the real value
      * of an expression node.  It is an error, and the results are
      * undefined (probably downloads pr0n), if this is not a constant
      * real expression node.
      *
      * @note
      *     You may have to #optimize first, to fold integer constants,
      *     even when #is_constant returns true.  Otherwise it will
      *     simply return zero.
      */
    virtual double get_real_value(void) const;

    /**
      * The get_string_value method is used to obtain the string value
      * of an expression node.  It is an error, and the results are
      * undefined (probably reboots into windows), if this is not a
      * constant string expression node.
      *
      * @note
      *     You may have to #optimize first, to fold integer constants,
      *     even when #is_constant returns true.  Otherwise it will
      *     simply return the empty string.
      */
    virtual rcstring get_string_value(void) const;

    /**
      * The get_symbol_hint method is used to get the symbol associated
      * with the expression.
      *
      * @returns
      *     pointer to a symbol, or NULL if there is no hint present
      */
    virtual symbol::pointer get_symbol_hint(void) const;

    /**
      * The is_error method is used to determine whether or not this
      * expression is an error (and that the error has already been
      * reported).
      */
    bool is_error(void) const;

    /**
      * The is_simple_with_stmt method is used to determine whether
      * or not an expression is the simple case for with statements,
      * where a temporary pointer variable is <i>not</i> required.
      * The default implementation returns false.
      */
    virtual bool is_simple_with_stmt(void) const;

    /**
      * The lisp_representation mthod is used to obtain a Lisp-like
      * representation of an expression tree.  This is often useful for
      * debugging.
      */
    virtual rcstring lisp_representation(void) const = 0;

    /**
      * The get_string_value mthod is used to obtain a strings of the
      * tree's value.  This is often useful for error messages.
      */
    virtual rcstring get_printable_value(void) const;

    /**
      * The is_empty_set method may be used to determine whether or
      * not an expression is the empty set constant.  The default
      * implementation returns false.
      */
    virtual bool is_empty_set(void) const;

    /**
      * The get_byte_pointer method is used to calculate the "pointer
      * and index" pair, used by a variety of IO procedures, and byte
      * array manipulation procedures, including strings.
      *
      * @returns
      *     a byte pointer if one is available, or the NULL pointer if not.
      *
      * @note
      *     The default implementation, used for most expressions,
      *     returns NULL.
      */
    virtual expression::pointer get_byte_pointer(void) const;

    /**
      * The clone method is used to make a duplicate of this expression.
      * This is used, for example, when the base expression of a WITH
      * statement is used in an implicit dot expression.
      *
      * @param locn
      *     When this cloing took place.
      * @returns
      *     a new expression node
      */
    virtual expression::pointer clone(const location &locn) const = 0;

    /**
      * The ord_odd_hack method is used to determine whether or not this
      * expression tree looks like an ord/odd hack to obtain access to
      * the bit-wise arithmetic opcodes.
      *
      * This is only called by the ord() implementation, to see if its
      * single parameter looks likely.
      *
      *   @code
      *   a, b, c: integer;
      *   a = ord(odd(b) and odd(c))
      *   @endcode
      *
      * however the cross compoiler doesn't need these hacks, because
      * it supports direct bit-wise expressions, and this becomes
      *
      *   @code
      *   a, b, c: integer;
      *   a = b and c
      *   @endcode
      *
      * Which is more readable, but will also survive the cross compiler's
      * logical expression short-circuit eveluation.
      *
      * @returns
      *     NULL if not relevant, or pointer to non-hack bit-wise
      *     arithmetic tree.
      */
    virtual expression::pointer ord_odd_hack(void) const;

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression(const expression &rhs);

    enum
    {
        prec_comma = 2,    // ,
        prec_assign = 4,   // :=
        prec_ternary = 6,  // ? :
        prec_eq = 18,      // = <>
        prec_rel = 20,     // < <= > >= IN
        prec_add = 24,     // + - or
        prec_mul = 26,     // * / and
        prec_unary = 28,   // - ! not
        prec_paren = 30,   // ( ) [ ] { }
        prec_id = 99
    };

    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param locn
      *     the source code location of the expression.
      */
    expression(const location &locn);

    /**
      * The need_parens function is used to calculate whether or not
      * parentheses are required.
      *
      * @param pprec
      *     The parent node prececence
      * @param cprec
      *     The child node precedence
      */
    bool need_parens(int pprec, int cprec) const;

    /**
      * The need_parens function is used to calculate whether or not
      * parentheses are required.
      *
      * @param pprec
      *     The parent node prececence
      */
    bool need_parens(int pprec) const;

    /**
      * The comparison_always_false_due_to_limited_range_of_data_type
      * method is used to print an warning message when a relational
      * expressions (<, <=, >, >=) is found to have a constant value
      * because of the limited range of type types involved.
      *
      * For example (true < boolean-expression) is always false.
      *
      * @param x
      *     The boolean value it always evauates to.
      */
    void comparison_always_same_due_to_limited_range_of_data_type(bool x) const;

private:
    /**
      * The loc instance variable is used to remember the location of
      * the expression.
      */
    location loc;

    /**
      * The default constructor.  Do not use.
      */
    expression();

    /**
      * The assignment operator.  Do not use.
      */
    expression &operator=(const expression &);
};

/**
  * The side_effect_t addition operator is used to combine side
  * effects for expression tree building.
  */
expression::side_effect_t operator+(expression::side_effect_t lhs,
    expression::side_effect_t rhs);

inline expression::side_effect_t
operator+=(expression::side_effect_t &lhs, expression::side_effect_t rhs)
{
    lhs = lhs + rhs;
    return lhs;
}

#endif // LIB_EXPRESSION_H
// vim: set ts=8 sw=4 et :

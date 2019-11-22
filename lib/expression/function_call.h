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

#ifndef LIB_EXPRESSION_FUNCTION_CALL_H
#define LIB_EXPRESSION_FUNCTION_CALL_H

#include <lib/expression.h>
#include <lib/expression/list.h>

/**
  * The expression_function_call class is used to represent an abstract
  * function call expression node.
  */
class expression_function_call:
    public expression
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_function_call();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param parms
      *     The parameters to the expression call.  In the general case,
      *     this will include the function name as the first parameter.
      *     However, not all usages need to abide by this convention.
      */
    expression_function_call(const expression_list &parms);

    /**
      * The constructor.
      * For use by derived classes only.
      * Use this form when the parameter list could be empty.
      *
      * @param locn
      *     The location of the function name within the source code.
      * @param parms
      *     The parameters to the function call.  In the general case,
      *     this will include the function name as the first parameter.
      *     However, not all usages need to abide by this convention.
      */
    expression_function_call(const location &locn,
        const expression_list &parms);

    /**
      * The constructor.
      * For use by derived classes only.
      * Use this form when the parameter list is empty.
      *
      * @param locn
      *     The location of the function name within the source code.
      */
    expression_function_call(const location &locn);

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

    /**
      * The pre_order_traversal method is called by the traversal method
      * just before any of the child expressions are traversed.
      *
      * @param pprec
      *     parent expression precedence
      */
    virtual void pre_order_traversal(int pprec) const;

    /**
      * The comma_order_traversal method is called by the traversal
      * method just after each of the child expressions has been
      * traversed, except the last one.
      *
      * @param pprec
      *     parent expression precedence
      * @param argnum
      *     the number of the parameter (0 based, 0 is the function name,
      *     usually)
      */
    virtual void comma_order_traversal(int pprec, int argnum) const;

    /**
      * The post_order_traversal method is called by the traversal
      * method just after the last child expression has been traversed.
      *
      * @param pprec
      *     parent expression precedence
      * @param argnum
      *     the number of the parameter (can be zero if only one child)
      *     usually)
      */
    virtual void post_order_traversal(int pprec, int argnum) const;

    size_t get_child_count() const { return children.size(); }

    expression::pointer
    get_child(size_t n)
        const
    {
        return children.get(n);
    }

    const expression_list &get_children() const { return children; }

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The children instance variable is used to remember the child
      * expressions of this function call.  Child [0] is the function name,
      * usually.
      */
    expression_list children;

    /**
      * The default constructor.
      */
    expression_function_call();

protected:
    /**
      * The copy constructor.
      * For use by derived classes only.
      */
    expression_function_call(const expression_function_call &);

private:
    /**
      * The assignment operator.
      */
    expression_function_call &operator=(const expression_function_call &);
};

#endif // LIB_EXPRESSION_FUNCTION_CALL_H
// vim: set ts=8 sw=4 et :

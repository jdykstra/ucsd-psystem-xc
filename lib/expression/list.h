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

#ifndef LIB_EXPRESSION_LIST_H
#define LIB_EXPRESSION_LIST_H

#include <lib/ac/stddef.h>

#include <lib/expression.h>

/**
  * The expression_list class is used to represent an ordered collection
  * of expressions.
  */
class expression_list
{
public:
    /**
      * The destructor.
      */
    virtual ~expression_list();

    /**
      * The default constructor.
      */
    expression_list();

    /**
      * The copy constructor.
      */
    expression_list(const expression_list &);

    /**
      * The assignment operator.
      */
    expression_list &operator=(const expression_list &);

    /**
      * The push_front method is used to add another expression to the
      * beginning of the list.
      *
      * This is an O(n) operation.
      */
    void push_front(const expression::pointer &esp);

    /**
      * The push_back method is used to add another expression to the
      * end of the list.
      *
      * This is an O(1) operation.
      */
    void push_back(const expression::pointer &esp);

    /**
      * The push_back method is used to push a whole list on the back of
      * this list.
      *
      * @param arg
      *     This is to be appended.
      */
    void push_back(const expression_list &arg);

    /**
      * The size method is used to obtain the number of elements in the
      * list.
      */
    size_t size() const { return length; }

    /**
      * The empty method is used to determine whether or not the list is
      * empty.
      */
    bool empty() const { return (length == 0); }

    /**
      * The get method is used to obtain a specific member of the list.
      */
    expression::pointer get(size_t n) const;

    /**
      * The subscript operator is used to obtain a specific member of
      * the list.
      */
    expression::pointer operator[](size_t n) const { return get(n); }

    /**
      * The front method is used to obtain the first item in the list.
      */
    expression::pointer
    front()
        const
    {
        return (length ? item[0] : expression::pointer());
    }

    /**
      * The back method is used to obtain the last item in the list.
      */
    expression::pointer
    back()
        const
    {
        return (length ? item[length - 1] : expression::pointer());
    }

    /**
      * The has_side_effect method is used to determine if an expression
      * has any effect.  It is used by the parse to produce "statement
      * with no effect" warnings.
      */
    expression::side_effect_t has_side_effect() const;

    /**
      * The pop_front method is used to remove the first element from
      * the list, and return it.  If the list is empty, the NULL pointer
      * is returned.
      */
    expression::pointer pop_front();

    /**
      * The pop_back method is used to remove the last element from the
      * list, and return it.  If the list is empty, the NULL pointer is
      * returned.
      */
    expression::pointer pop_back();

    /**
      * The clear method is used to discard all list contents.
      */
    void clear();

    /**
      * The get_location method is used to obtain the location of the
      * expression list.
      */
    location get_location() const;

    /**
      * The tail method is used to create a new expression list is the
      * value of this list, but woith the first item removed.
      */
    expression_list tail(void) const;

    /**
      * The optimize method is used to create a new expression list by
      * optimizing each of the parameters to this expression list.
      */
    expression_list optimize(void) const;

    /**
      * The clone method is used to create a new expression list by
      * cloning each of the parameters to this expression list.
      */
    expression_list clone(const location &locn) const;

protected:
    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The length instance variable is used to remember how many
      * elements of the \a item array have been used to date.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remember the allocation
      * size of the \a item array.
      */
    size_t maximum;

    /**
      * The item instance variable is used to remember the address of
      * the dynamically allocated array of expression pointers.
      */
    expression::pointer *item;

    /**
      * The grow method is used to increase the allocated storage, if necessary.
      *
      * @param by
      *     how many array elements to grow by.
      */
    void grow(size_t by);
};

#endif // LIB_EXPRESSION_LIST_H
// vim: set ts=8 sw=4 et :

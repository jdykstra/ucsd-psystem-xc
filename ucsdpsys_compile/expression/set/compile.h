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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_SET_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_SET_COMPILE_H

#include <lib/expression/set.h>

class translator_compile; // forward

/**
  * The expression_set_compile class is used to represent a constant
  * valued set expression node.
  */
class expression_set_compile:
    public expression_set
{
public:
    typedef boost::shared_ptr<expression_set_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_set_compile();

private:
    /**
      * The constructor.
      */
    expression_set_compile(const location &locn, const type::pointer &tp,
        const set &value, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &locn, const type::pointer &tp,
        const set &value, translator_compile *cntxt);

    /**
      * The minimum_subrange class method may be used to create a new
      * type that is a 'set of subrange' sufficiently large to hold the
      * set, but no larger.
      *
      * @param tp
      *     The indicative type of expression, 'type of {something}'
      * @param value
      *     The value of the set.
      */
    static type::pointer minimum_subrange(const type::pointer &tp,
        const set &value);

    /**
      * The get_bit method may be used to determine whether or not the
      * given bit has been set.  Useful for optimizing constant IN
      * expressions.
      *
      * @param nth
      *     The set member to test for.
      * @returns
      *     true if nth is a member of the set, false if not
      */
    bool get_bit(unsigned nth) const;

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The value instance variable is used to remember the value of the
      * set constant.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_set_compile();

    /**
      * The copy constructor.  Do not use.
      */
    expression_set_compile(const expression_set_compile &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_set_compile &operator=(const expression_set_compile &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_SET_COMPILE_H
// vim: set ts=8 sw=4 et :

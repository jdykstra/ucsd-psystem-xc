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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_SET2SET_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_SET2SET_COMPILE_H

#include <lib/expression/set2set.h>

class translator_compile; // forward

/**
  * The expression_set2set_compile class is used to represent the code
  * generation for a set cast expression.
  */
class expression_set2set_compile:
    public expression_set2set
{
public:
    typedef boost::shared_ptr<expression_set2set_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_set2set_compile();

private:
    /**
      * The constructor.
      */
    expression_set2set_compile(const type::pointer &to,
        const expression::pointer &from, translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static expression::pointer create(const type::pointer &to,
        const expression::pointer &from, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void post_order_traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_set2set_compile();

    /**
      * The copy constructor.  Do not use.
      */
    expression_set2set_compile(const expression_set2set_compile &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_set2set_compile &operator=(const expression_set2set_compile &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_SET2SET_COMPILE_H
// vim: set ts=8 sw=4 et :

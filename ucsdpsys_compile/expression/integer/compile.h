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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_INTEGER_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_INTEGER_COMPILE_H

#include <lib/expression/integer.h>

class translator_compile; // forward

/**
  * The expression_integer_compile class is used to represent code
  * generation of an integer valued constant.
  */
class expression_integer_compile:
    public expression_integer
{
public:
    typedef boost::shared_ptr<expression_integer_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_integer_compile();

private:
    /**
      * The constructor.
      */
    expression_integer_compile(const location &locn, const rcstring &arg,
        translator_compile *cntxt);

public:
    static pointer create(const location &locn, const rcstring &arg,
        translator_compile *cntxt);

protected:
    /**
      * The constructor.
      */
    expression_integer_compile(const location &locn, long arg,
        translator_compile *cntxt);

public:
    static pointer create(const location &locn, long arg,
        translator_compile *cntxt);

    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

private:
    /**
      * The code instance variabel is used to remember where to generate
      * our code to.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_integer_compile();

protected:
    translator_compile *get_code() const { return &code; }

    /**
      * The copy constructor.
      * For use by derived classes onlt.
      */
    expression_integer_compile(const expression_integer_compile &);

private:
    /**
      * The assignment operator.  Do not use.
      */
    expression_integer_compile &operator=(const expression_integer_compile &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_INTEGER_COMPILE_H
// vim: set ts=8 sw=4 et :

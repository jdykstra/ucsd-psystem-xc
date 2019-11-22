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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_NEGATE_REAL_H
#define UCSDPSYS_COMPILE_EXPRESSION_NEGATE_REAL_H

#include <lib/expression/negate.h>

class translator_compile; // forward

/**
  * The expression_negate_real class is used to represent the code
  * generation required for a floating point negate.
  */
class expression_negate_real:
    public expression_negate
{
public:
    typedef boost::shared_ptr<expression_negate_real> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_negate_real();

private:
    /**
      * The constructor.
      */
    expression_negate_real(const expression::pointer &arg,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &arg,
        translator_compile *cntxt);

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
      * The default constructor.
      */
    expression_negate_real();

    /**
      * The copy constructor.
      */
    expression_negate_real(const expression_negate_real &);

    /**
      * The assignment operator.
      */
    expression_negate_real &operator=(const expression_negate_real &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_NEGATE_REAL_H
// vim: set ts=8 sw=4 et :

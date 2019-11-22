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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_BOOLEAN_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_BOOLEAN_COMPILE_H

#include <lib/expression/boolean.h>

class translator_compile; // forward

/**
  * The expression_boolean_compile class is used to represent the code
  * generation necessary for constant boolean valued expression nodes.
  */
class expression_boolean_compile:
    public expression_boolean
{
public:
    typedef boost::shared_ptr<expression_boolean_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_boolean_compile();

private:
    /**
      * The constructor.
      */
    expression_boolean_compile(const location &where, bool value,
        translator_compile *cntxt);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const location &where, bool value,
        translator_compile *cntxt);

protected:
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
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.  Do not use.
      */
    expression_boolean_compile();

    /**
      * The copy constructor.  Do not use.
      */
    expression_boolean_compile(const expression_boolean_compile &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_boolean_compile &operator=(const expression_boolean_compile &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_BOOLEAN_COMPILE_H
// vim: set ts=8 sw=4 et :

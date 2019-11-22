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

#ifndef UCSDPSYS_COMPILE_STATEMENT_EXPRESSION_COMPILE_H
#define UCSDPSYS_COMPILE_STATEMENT_EXPRESSION_COMPILE_H

#include <lib/statement/expression.h>

class translator_compile; // forward

/**
  * The statement_expression_compile class is used to represent the code
  * generation necessary for a statement which consists of a single
  * expression.
  */
class statement_expression_compile:
    public statement_expression
{
public:
    typedef boost::shared_ptr<statement_expression_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_expression_compile();

private:
    /**
      * The constructor.
      */
    statement_expression_compile(const expression::pointer &arg,
        translator_compile *cntxt);

public:
    static pointer create(const expression::pointer &arg,
        translator_compile *cntxt);

protected:
    /**
      * The code instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The default constructor.
      */
    statement_expression_compile();

    /**
      * The copy constructor.
      */
    statement_expression_compile(const statement_expression_compile &);

    /**
      * The assignment operator.
      */
    statement_expression_compile &operator=(
        const statement_expression_compile &);
};

#endif // UCSDPSYS_COMPILE_STATEMENT_EXPRESSION_COMPILE_H
// vim: set ts=8 sw=4 et :

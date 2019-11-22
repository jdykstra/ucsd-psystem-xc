//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#ifndef UCSDPSYS_COMPILE_STATEMENT_CASE_COMPILE_H
#define UCSDPSYS_COMPILE_STATEMENT_CASE_COMPILE_H

#include <lib/statement/case.h>

class translator_compile; // forward

/**
  * The statement_case_compile class is used to represent the code
  * generation required for the CASE statement.
  */
class statement_case_compile:
    public statement_case
{
public:
    typedef boost::shared_ptr<statement_case_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_case_compile();

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    statement_case_compile(const expression::pointer &control,
        const case_list &cases, const statement::pointer &otherwise,
        translator_compile *code);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const expression::pointer &control,
        const case_list &cases, const statement::pointer &otherwise,
        translator_compile *code);

protected:
    // See base class for documentation.
    void traversal(void) const;

private:
    translator_compile &code;

    void traversal_old_style(void) const;

    void traversal_new_style(void) const;

    /**
      * The default constructor.  Do not use.
      */
    statement_case_compile();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    statement_case_compile(const statement_case_compile &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    statement_case_compile &operator=(const statement_case_compile &rhs);
};

#endif // UCSDPSYS_COMPILE_STATEMENT_CASE_COMPILE_H
// vim: set ts=8 sw=4 et :

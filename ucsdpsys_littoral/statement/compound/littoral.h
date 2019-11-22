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

#ifndef UCSDPSYS_PRETTY_STATEMENT_COMPOUND_PRETTY_H
#define UCSDPSYS_PRETTY_STATEMENT_COMPOUND_PRETTY_H

#include <lib/statement/compound.h>

class translator_littoral; // forward

/**
  * The statement_compound_littoral class is used to represent littoral
  * printing of a list of statements.
  */
class statement_compound_littoral:
    public statement_compound
{
public:
    typedef boost::shared_ptr<statement_compound_littoral> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_compound_littoral();

private:
    /**
      * The constructor.
      */
    statement_compound_littoral(const statement_list &body,
        translator_littoral *cntxt);

public:
    static pointer create(const statement_list &body,
        translator_littoral *cntxt);


protected:
    // See base class for documentation.
    void traversal_begin() const;

    // See base class for documentation.
    void traversal_semicolon() const;

    // See base class for documentation.
    void traversal_end() const;

private:
    /**
      * The littoral instance variable is used to remember how we print
      * ourself.
      */
    translator_littoral &littoral;

    /**
      * The default constructor.  Do not use.
      */
    statement_compound_littoral();

    /**
      * The copy constructor.  Do not use.
      */
    statement_compound_littoral(const statement_compound_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_compound_littoral &operator=(const statement_compound_littoral &);
};

#endif // UCSDPSYS_PRETTY_STATEMENT_COMPOUND_PRETTY_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_STATEMENT_COMPOUND_H
#define LIB_STATEMENT_COMPOUND_H

#include <lib/statement.h>
#include <lib/statement/list.h>

/**
  * The statement_compound class is used to represent a statement
  * composed of nested statements.
  */
class statement_compound:
    public statement
{
public:
    typedef boost::shared_ptr<statement_compound> pointer;

    /**
      * The destructor.
      */
    virtual ~statement_compound();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(void);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(const statement_list &rhs);

    // See base class for documentation.
    void traversal(void) const;

    // See base class for documentation.
    bool empty(void) const;

    // See base class for documentation.
    bool execution_flows_out(void) const;

    // See base class for documentation.
    bool execution_flows_in(void) const;

    /**
      * The get_content method may be used to obtain a reference to the
      * content of the statement list.
      */
    const statement_list &get_content(void) const { return content; }

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    statement_compound(const statement_list &rhs);

    // See base class for documentation.
    virtual void traversal_begin(void) const;

    // See base class for documentation.
    virtual void traversal_semicolon(void) const;

    // See base class for documentation.
    virtual void traversal_end(void) const;

    // See base class for documentation.
    bool is_compound(void) const;

private:
    /**
      * The default constructor.
      * It is private on purpose, use the #create class method instead.
      */
    statement_compound();

    /**
      * The content instance variable is used to remember the list of
      * statements in this compount statement.
      */
    statement_list content;

    /**
      * The copy constructor.  Do not use.
      */
    statement_compound(const statement_compound &);

    /**
      * The assignment operator.  Do not use.
      */
    statement_compound &operator=(const statement_compound &);
};

#endif // LIB_STATEMENT_COMPOUND_H
// vim: set ts=8 sw=4 et :

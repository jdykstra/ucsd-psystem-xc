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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_PRIVREF_H
#define UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_PRIVREF_H

#include <ucsdpsys_compile/expression/address.h>

class translator_compile; // forward

/**
  * The expression_address_privref class is used to represent the
  * address of a not-linked-yet global variable (but one that is private
  * to a UNIT's implementation).
  */
class expression_address_privref:
    public expression_address
{
public:
    typedef boost::shared_ptr<expression_address_privref> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_address_privref();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The source file location of this expression
      * @param type
      *     The type of variable this expression represents
      * @param sym
      *     The symbol related to this expression.
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const location &locn, const type::pointer &type,
        const symbol::pointer &sym, translator_compile *context);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    expression::pointer fast_load(void) const;

    // See base class for documentation.
    expression::pointer fast_store(const expression::pointer &rhs) const;

    // See base class for documentation.
    expression_address::pointer advance_by_words(long nwords,
        const type::pointer &new_type, const symbol::pointer &new_hint) const;

    // See base class for documentation.
    expression::pointer cast(const type::pointer &tp) const;

    // See base class for documentation.
    bool is_constant(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param locn
      *     The source file location of this expression
      * @param type
      *     The type of variable this expression represents
      * @param sym
      *     The symbol related to this expression.
      * @param context
      *     The translation context we are working within.
      */
    expression_address_privref(const location &locn, const type::pointer &type,
        const symbol::pointer &sym, translator_compile *context);

    /**
      * The context instance variabel is used to remember the
      * symbol related to this expression.
      */
    symbol::pointer sym;

    /**
      * The context instance variabel is used to remember the
      * translation context we are working within.
      */
    translator_compile *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_address_privref();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    expression_address_privref(const expression_address_privref &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    expression_address_privref &operator=(
        const expression_address_privref &rhs);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_ADDRESS_PRIVREF_H
// vim: set ts=8 sw=4 et :

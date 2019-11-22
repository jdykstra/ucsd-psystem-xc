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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_LDO_PRIVREF_H
#define UCSDPSYS_COMPILE_EXPRESSION_LDO_PRIVREF_H

#include <lib/expression.h>

class translator_compile; // forward

/**
  * The expression_ldo_privref class is used to represent an LDO (load global
  * word) opcode, but with a PRIVREF linkage record.
  */
class expression_ldo_privref:
    public expression
{
public:
    typedef boost::shared_ptr<expression_ldo_privref> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_ldo_privref();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param locn
      *     The source location of this code.
      * @param ptype
      *     The type of expression resulting from this code.
      * @param sym
      *     The symbol being referenced.
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const location &locn, const type::pointer &ptype,
        const symbol::pointer &sym, translator_compile *context);

protected:
    // See base class for documentation.
    void traversal(int pprec) const;

    // See base class for documentation.
    expression::pointer optimize(void) const;

    // See base class for documentation.
    expression::pointer clone(const location &locn) const;

    // See base class for documentation.
    void logical_traversal(int pprec, label::pointer &true_branch,
        label::pointer &false_branch, bool fall_through_preference) const;

    // See base class for documentation.
    int get_precedence() const;

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    bool is_lvalue() const;

    // See base class for documentation.
    side_effect_t has_side_effect() const;

    // See base class for documentation.
    rcstring lisp_representation(void) const;

private:
    /**
      * The constructor.
      *
      * @param locn
      *     The source location of this code.
      * @param ptype
      *     The type of expression resulting from this code.
      * @param sym
      *     The symbol being referenced.
      * @param context
      *     The translation context we are working within.
      */
    expression_ldo_privref(const location &locn, const type::pointer &ptype,
        const symbol::pointer &sym, translator_compile *context);

    /**
      * The ptype instance variable is used to remember the type of this
      * expression.
      */
    type::pointer ptype;

    /**
      * The sym instance variable is used to remember
      * the symbol being referenced.
      */
    symbol::pointer sym;

    /**
      * The context instance variable is used to remember where to send our
      * generated code.
      */
    translator_compile *context;

    /**
      * The default constructor.  Do not use.
      */
    expression_ldo_privref();

    /**
      * The copy constructor.  Do not use.
      */
    expression_ldo_privref(const expression_ldo_privref &);

    /**
      * The assignment operator.  Do not use.
      */
    expression_ldo_privref &operator=(const expression_ldo_privref &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_LDO_PRIVREF_H
// vim: set ts=8 sw=4 et :

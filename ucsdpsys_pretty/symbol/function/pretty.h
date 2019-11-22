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

#ifndef UCSDPSYS_PRETTY_SYMBOL_FUNCTION_PRETTY_H
#define UCSDPSYS_PRETTY_SYMBOL_FUNCTION_PRETTY_H

#include <lib/symbol/function.h>

class translator_pretty; // forward

/**
  * The symbol_function_pretty class is used to represent a callable
  * function symbol.  It exists to host the function call factory which
  * generates our pretty printable function call expression nodes.
  */
class symbol_function_pretty:
    public symbol_function
{
public:
    typedef boost::shared_ptr<symbol_function_pretty> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_function_pretty();

private:
    /**
      * The constructor.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param locals
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lex_level
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param cntxt
      *     The translation context we are working within.
      */
    symbol_function_pretty(const rcstring &name, const scope::pointer &locals,
        int lex_level, bool seg_proc, translator_pretty *cntxt);

    /**
      * The constructor.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param dec_loc
      *     The declaration location of the function.
      * @param locals
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lex_level
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param cntxt
      *     The translation context we are working within.
      */
    symbol_function_pretty(const rcstring &name, const location &dec_loc,
        const scope::pointer &locals, int lex_level, bool seg_proc,
        translator_pretty *cntxt);

public:
    /**
      * The create class method is sued to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param locals
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lex_level
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, const scope::pointer &locals,
        int lex_level, bool seg_proc, translator_pretty *cntxt);

    /**
      * The create class method is sued to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param dec_loc
      *     The declaration location of the function.
      * @param locals
      *     The scope that contains the function's parameters and local
      *     variables.
      * @param lex_level
      *     The lexical level at which the function is defined at.
      * @param seg_proc
      *     True if this is a segment procedure,
      *     false if is a normal procedure.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, const location &dec_loc,
        const scope::pointer &locals, int lex_level, bool seg_proc,
        translator_pretty *cntxt);

    /**
      * The create class method is sued to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the function (or procedure) being declared.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create_builtin(const rcstring &name,
        translator_pretty *cntxt);

    // See base class for documentation.
    expression::pointer function_call_expression_factory(
        const expression_list &rhs) const;

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The pretty instance variable is used to remember where to send
      * our pretty output source code.
      */
    translator_pretty &pretty;

    /**
      * The default constructor.  Do not use.
      */
    symbol_function_pretty();

    /**
      * The copy constructor.  Do not use.
      */
    symbol_function_pretty(const symbol_function_pretty &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol_function_pretty &operator=(const symbol_function_pretty &);
};

#endif // UCSDPSYS_PRETTY_SYMBOL_FUNCTION_PRETTY_H
// vim: set ts=8 sw=4 et :

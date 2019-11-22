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

#ifndef UCSDPSYS_COMPILE_EXPRESSION_STRING_COMPILE_H
#define UCSDPSYS_COMPILE_EXPRESSION_STRING_COMPILE_H

#include <lib/expression/string.h>

class translator_compile;

/**
  * The expression_string_compile class is used to represent code
  * generation for a string constant.
  */
class expression_string_compile:
    public expression_string
{
public:
    typedef boost::shared_ptr<expression_string_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~expression_string_compile();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * The expression will be of type string.
      *
      * @param locn
      *     The location of the string constant in the source code.
      * @param arg
      *     The text of the string literal.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const location &locn, const rcstring &arg,
        translator_compile *cntxt);

    /**
      * The create_ref class method is used to create new dynamically
      * allocated instances of this class.
      *
      * The expression will be of type reference to string.
      *
      * @param locn
      *     The location of the string constant in the source code.
      * @param arg
      *     The text of the string literal.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create_ref(const location &locn, const rcstring &arg,
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

    // See base class for documentation.
    type::pointer get_type() const;

    // See base class for documentation.
    expression::pointer strip_indirection() const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param locn
      *     The location of the string constant in the source code.
      * @param arg
      *     The text of the string literal.
      * @param whatsit
      *     The type of the string literal.  It could be a string, it
      *     could be a reference to a string... both are represented on
      *     the stack with a pointer.
      * @param cntxt
      *     The translation context we are working within.
      *
      * @note
      *     This can make a mess, because UCSD Pascal has no concept of
      *     a "const" reference.  So if a constant string literal is
      *     passed as a 'var foo: string' argument, the called function
      *     could change the string, thus changing the string CONSTANT.
      *
      *     Worse: a clever buffer overrun will modify the opcodes following
      *     the string constant.
      *
      *     Normally I would call that a BUG, except that system/globals.txt
      *     in the II.0 sources declares 'procedure fwritestr(var f: file;
      *     var value: string; width: integer);
      *
      *     FIXME: introduce a "constant reference" type, and use it for
      *     the fwritestr declaration in lib/translator/builtin.cc
      */
    expression_string_compile(const location &locn, const rcstring &arg,
        const type::pointer &whatsit, translator_compile *cntxt);

    /**
      * Build a new instance.  This is intentionally private.
      *
      * @param locn
      *     The location of the string constant in the source code.
      * @param arg
      *     The text of the string literal.
      * @param whatsit
      *     The type of the string literal.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const location &locn, const rcstring &arg,
        const type::pointer &whatsit, translator_compile *cntxt);

    type::pointer whatsit;

    /**
      * The code instance variable is used to remember wher to generate
      * code to.
      */
    translator_compile &code;

    /**
      * The default constructor.
      */
    expression_string_compile();

    /**
      * The copy constructor.
      */
    expression_string_compile(const expression_string_compile &);

    /**
      * The assignment operator.
      */
    expression_string_compile &operator=(const expression_string_compile &);
};

#endif // UCSDPSYS_COMPILE_EXPRESSION_STRING_COMPILE_H
// vim: set ts=8 sw=4 et :

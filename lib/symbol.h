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

#ifndef LIB_SYMBOL_H
#define LIB_SYMBOL_H

#include <lib/bit_address.h>
#include <lib/location.h>
#include <lib/rcstring.h>
#include <lib/type.h>

class expression; // forward
class variable_name; // forward
class expression; // forward
typedef boost::shared_ptr<expression> expression_pointer_t;

/**
  * The symbol abstract base class is used to represent a generic
  * symbol.  It could be a variable or a type or a function, etc.
  */
class symbol
{
public:
    typedef boost::shared_ptr<symbol> pointer;
    typedef boost::shared_ptr<expression> expression_pointer_t;

    /**
      * The destructor.
      */
    virtual ~symbol();

protected:
    /**
      * The constructor.
      *
      * @param name
      *     the name of the symbol.
      * @param typ
      *     the type of the symbol.
      * @param lex_level
      *     The function definition nesting level of this symbol.  (And,
      *     yes, I know this has the wrong name, but it's the one that
      *     the UCSD p-System documentation uses, so we are stuck with
      *     it.)
      */
    symbol(const variable_name &name, const type::pointer &typ,
        int lex_level);

    /**
      * The constructor.
      *
      * @param name
      *     the name of the symbol.
      * @param typ
      *     the type of the symbol.
      * @param lex_level
      *     The function definition nesting level of this symbol.  (And,
      *     yes, I know this has the wrong name, but it's the one that
      *     the UCSD p-System documentation uses, so we are stuck with
      *     it.)
      */
    symbol(const rcstring &name, const type::pointer &typ, int lex_level);

    /**
      * The constructor.
      *
      * @param name
      *     the name of the symbol.
      * @param dec_loc
      *     The declaration location of the new symbol.
      * @param typ
      *     the type of the symbol.
      * @param lex_level
      *     The function definition nesting level of this symbol.  (And,
      *     yes, I know this has the wrong name, but it's the one that
      *     the UCSD p-System documentation uses, so we are stuck with
      *     it.)
      */
    symbol(const rcstring &name, const location &dec_loc,
        const type::pointer &typ, int lex_level);

public:
    /**
      * The get_name method is used to obtain the name of the symbol.
      */
    rcstring get_name(void) const { return name; }

    /**
      * The get_type method is used to obtain the type of the symbol.
      * This is highly correlated with, but not the same as, the
      * expression type.
      */
    type::pointer get_type(void) const { return stype; }

    /**
      * The get_kind method is used to obtain what kind of symbol
      * this is.  For example, "function", "type name", "variable", etc.
      * It is used to construct better error messages.
      */
    virtual const char *get_kind(void) const = 0;

    /**
      * The get_bit_offset method is used to obtain the offset
      * of this variable in its local scope.
      */
    const bit_address &get_bit_offset(void) const { return bit_offset; }

    /**
      * The set_bit_offset method is used to set the offset, in bytes,
      * of a variable in its local scope.  Shall only be used by the
      * parser.
      *
      * @param boff
      *     The bit offset of this variable.
      * @param pkd
      *     The packed indicator; is this variable packed within its
      *     container.
      */
    void set_bit_offset(const bit_address &boff, bool pkd);

    /**
      * The get_size_in_bytes method is used to get the size of the
      * object in bytes.  This is only meaningful for variables, all
      * other symbols return zero.
      */
    virtual unsigned get_size_in_bytes(void) const;

    /**
      * The get_size_in_bits method is used to get the size of the
      * object in bits.  This is only meaningful for variables, all
      * other symbols return zero.
      */
    virtual unsigned get_size_in_bits(void) const;

    /**
      * The add_reference method is used to add a code location
      * reference to a variable, and even remembers the case of the name
      * at that point.  The default implementation does nothing.
      *
      * @param locn
      *     The source code location of the reference
      * @param name
      *     The name it was called when it was referenced.
      */
    void add_reference(const location &locn, const rcstring &name);

    /**
      * The add_reference method is used to add a code location
      * reference to a variable, and even remembers the case of the name
      * at that point.  The default implementation does nothing.
      */
    virtual void add_reference(const variable_name &name);

    /**
      * The get_lex_level method is used to obtain the function
      * definition nesting level of this symbol.
      */
    int get_lex_level(void) const { return lex_level; }

    /**
      * The get_declaration_location method is used to obtain the
      * location of a symbol's declaration.
      */
    const location &get_declaration_location(void) const { return locn; }

    /**
      * The is_packed method is used to determine whether or not this
      * symbol is packed into its container (scope, array or record).
      */
    bool is_packed(void) const;

    /**
      * The type_late binding method is used to perform late binding on
      * the symbol's type.  This will not change the size of variables.
      */
    virtual void type_late_binding(void);

    /**
      * The need_late_binding method may be used to determine whether or
      * not a type requires additional late binding.
      */
    virtual bool need_late_binding(void) const;

    /**
      * The check_defined method is used to check for undefined FORWARD
      * declared symbols.  A fatal error will be printed for any symbol
      * with an incomplete type (see #need_late_binding).
      */
    virtual void check_defined(void);

    /**
      * The name_expression_factory method is used to create an
      * expression when this symbol is referenced in the source code.
      *
      * @param sp
      *     The reference countered pointer
      *     assert(this == sp.get());
      * @param locn
      *     The sourec code location of the reference.
      * @param lxlvl
      *     The lexical level of the reference (this is used to
      *     determine whether an intermediate or local opcode is
      *     required).
      *
      * @note
      *     This method creates a load expression. if you want a store
      *     expression, the object returned has an assignment factory
      *     method for you to use.
      */
    virtual expression_pointer_t name_expression_factory(
        const symbol::pointer &sp, const location &locn, int lxlvl) = 0;

protected:
    /**
      * The change_type method is used to change the type of a symbol
      * after it has been defined.  The intended use is to attach the
      * function return type, because this is parsed long after the
      * function needs to be defined.
      *
      * @param tsp
      *     The type to use.
      */
    void change_type(const type::pointer &tsp);

private:
    /**
      * The name instance variable is used to remember the name of the
      * symbol.
      */
    rcstring name;

    /**
      * The locn instance variable is used to remember where the symbol
      * was first mentioned.
      */
    location locn;

    /**
      * The type instance variable is used to remember the type of
      * this variable, implying its size, and the operations which may
      * reasonably be performed on this symbol.
      */
    type::pointer stype;

    /**
      * The offset_in_bytes instance variable is used to remember the
      * offset, in bytes, of this variable in its local scope.
      */
    bit_address bit_offset;

    /**
      * The packed instance variable is used to remember whether or not
      * this symbol is packed into its container (scope, array or record).
      */
    bool packed;

    /**
      * The lex_level instance variable is used to remember the function
      * definition nesting level of this symbol.  (And, yes, I know this
      * has the wrong name, but it's the one that the UCSD p-System
      * documentation uses, so we are stuck with it.)
      */
    int lex_level;

    /**
      * The default constructor.  Do not use.
      */
    symbol();

    /**
      * The copy constructor.  Do not use.
      */
    symbol(const symbol &);

    /**
      * The assignment operator.  Do not use.
      */
    symbol &operator=(const symbol &);
};

#endif // LIB_SYMBOL_H
// vim: set ts=8 sw=4 et :

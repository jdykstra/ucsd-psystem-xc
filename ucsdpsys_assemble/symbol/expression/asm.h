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

#ifndef UCSDPSYS_ASSEMBLE_SYMBOL_EXPRESSION_ASM_H
#define UCSDPSYS_ASSEMBLE_SYMBOL_EXPRESSION_ASM_H

#include <lib/symbol/expression.h>

class assembler; // forward

/**
  * The symbol_expression_asm class is used to represent an assembler
  * symbol.  It could be a label, a constant, a register, etc.
  */
class symbol_expression_asm:
    public symbol_expression
{
public:
    typedef boost::shared_ptr<symbol_expression_asm> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_expression_asm();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the symbol.
      * @param tp
      *     The type of the symbol.
      * @param value
      *     The value of the symbol, or NULL if not yet defined.
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const variable_name &name,
        const type::pointer &tp, const expression::pointer &value,
        assembler *context);

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.  The value will be NULL.
      *
      * @param name
      *     The name of the symbol
      * @param tp
      *     The type of the symbol.
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const variable_name &name, const type::pointer &tp,
        assembler *context);

    /**
      * The set_value method is used to update the value attached to a
      * symbol.
      */
    void set_value(const expression::pointer &value);

    /**
      * The export_def method is used to indicate that this symbol
      * should be exported, in the form of a GLOBDEF linker record.
      */
    void export_def(void);

protected:
    // See base class for documentation.
    const char *get_kind(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the symbol
      * @param tp
      *     The type of the symbol.
      * @param value
      *     The value of the symbol, or NULL if not yet defined.
      * @param context
      *     The translation context we are working within.
      */
    symbol_expression_asm(const variable_name &name, const type::pointer &tp,
        const expression::pointer &value, assembler *context);

    /**
      * The context instance variable is used to remember the
      * translation context we are working within.
      */
    assembler &context;

    /**
      * The export_me instance variable is used to remember whether or
      * not this symbol is the subject of a .def directive.
      */
    bool export_me;

    /**
      * The export_done instancevariabl eis sued to remember whether or
      * not this symbol has been exported into the linkage information
      * (iff export_me is also true).
      */
    bool export_done;

    /**
      * The default constructor.  Do not use.
      */
    symbol_expression_asm();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_expression_asm(const symbol_expression_asm &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_expression_asm &operator=(const symbol_expression_asm &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_SYMBOL_EXPRESSION_ASM_H
// vim: set ts=8 sw=4 et :

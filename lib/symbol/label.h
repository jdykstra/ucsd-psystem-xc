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

#ifndef LIB_SYMBOL_LABEL_H
#define LIB_SYMBOL_LABEL_H

#include <lib/symbol.h>
#include <lib/label.h>
#include <lib/variable/name/list.h>

/**
  * The symbol_label class is used to represent a symbol representing a
  * goto label.
  */
class symbol_label:
    public symbol
{
public:
    typedef boost::shared_ptr<symbol_label> pointer;

    /**
      * The destructor.
      */
    virtual ~symbol_label();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     the name of the symbol.
      * @param typ
      *     the type of the symbol.
      * @param lex_level
      *     The function definition nesting level of this symbol.
      * @param lp
      *     The concrete label instance.
      */
    static pointer create(const variable_name &name, const type::pointer &typ,
        int lex_level, const label::pointer &lp);

    /**
      * The goto_from_here method is called at code generation time
      * indicate an unconditional branch is to be produced at the
      * current code location.
      */
    void goto_from_here(void);

    /**
      * The define_here method is used at code generation time to
      * indicate that the destination of the jump is at the current code
      * location.
      */
    void define_here(void);

    /**
      * The remember_definition method is used to remember the source
      * code location of a label definition.  If the label has already
      * been defined and error message will be printed, and false
      * returned.
      *
      * @param name
      *     The name of the label.
      */
    bool remember_definition(const variable_name &name);

    /**
      * The remember_reference method is used to remember the source
      * code location of a label reference.
      *
      * @param name
      *     The name of the label.
      */
    void remember_reference(const variable_name &name);

    /**
      * The get_label method is used to obtaion access to the label,
      * for optimizations that need it.
      */
    label::pointer get_label(void) const { return lp; }

protected:
    // See base class for documentation.
    const char *get_kind(void) const;

    // See base class for documentation.
    void check_defined(void);

    // See base class for documentation.
    expression_pointer_t name_expression_factory(const symbol::pointer &sp,
        const location &locn, int lxlvl);

private:
    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     the name of the symbol.
      * @param typ
      *     the type of the symbol.
      * @param lex_level
      *     The function definition nesting level of this symbol.
      * @param lp
      *     The concrete label instance.
      */
    symbol_label(const variable_name &name, const type::pointer &typ,
       int lex_level, const label::pointer &lp);

    /**
      * The lp instance variable is used to remember the concrete label
      * instance.
      */
    label::pointer lp;

    /**
      * The defined_here instance variable is used to remember the
      * source code location of the label definition (as opposed to the
      * declaration).
      */
    variable_name defined_here;

    /**
      * The referenced_here instance variable is used to remember the
      * source code location of all the references (goto) to this
      * label.
      */
    variable_name_list referenced_here;

    /**
      * The default constructor.  Do not use.
      */
    symbol_label();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    symbol_label(const symbol_label &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    symbol_label &operator=(const symbol_label &rhs);
};

#endif // LIB_SYMBOL_LABEL_H
// vim: set ts=8 sw=4 et :

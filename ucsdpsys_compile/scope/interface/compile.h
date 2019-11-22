//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
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

#ifndef UCSDPSYS_COMPILE_SCOPE_INTERFACE_COMPILE_H
#define UCSDPSYS_COMPILE_SCOPE_INTERFACE_COMPILE_H

#include <lib/scope/interface.h>
#include <lib/segkind.h>
#include <lib/segment/builder.h>

class translator_compile; // forward

/**
  * The scope_interface_compile class is used to represent
  * compiling the interface to a unit's interface.
  */
class scope_interface_compile:
    public scope_interface
{
public:
    typedef boost::shared_ptr<scope_interface_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_interface_compile();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the unit.
      * @param code_seg
      *     The segment number of the unit's code.
      * @param data_seg
      *     The segment number of the unit's data, or -1 if none.
      * @param segkind
      *     The kind of segment (UNITSEG, LINKED_INTRINS, etc).
      * @param context
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, int code_seg, int data_seg,
        segkind_t segkind, translator_compile *context);

    /**
      * The segment_builder_factory method is used to create a faux
      * segment builder, so that the compiler's segstack has an entry
      * on it, and the segment and function numbers for the defined
      * functions are in the correct segment.
      */
    segment_builder::pointer segment_builder_factory(void);

protected:
    // See base class for documentation.
    symbol::pointer constant_factory(const variable_name &name,
        int lex_level, const expression::pointer &value);

    // See base class for documentation.
    symbol_variable::pointer variable_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    // See base class for documentation.
    symbol_variable::pointer parameter_factory(const variable_name &name,
        const type::pointer &tp, int lex_level);

    // See base class for documentation.
    bit_address allocate_space(const type::pointer &tp);

    // See base class for documentation.
    bit_address get_next_bit_address(unsigned nbits);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the unit.
      * @param code_seg
      *     The segment number of the unit's code.
      * @param data_seg
      *     The segment number of the unit's data, or -1 if none.
      * @param segkind
      *     The kind of segment (UNITSEG, LINKED_INTRINS, etc).
      * @param context
      *     The translation context we are working within.
      */
    scope_interface_compile(const rcstring &name, int code_seg, int data_seg,
        segkind_t segkind, translator_compile *context);

    /**
      * The code_seg instance variable is used to remember the segment
      * number of a unit's code.
      */
    int code_seg;

    /**
      * The data_seg instance variable is used to remember the segment
      * number of an intrinsic unit's code.
      */
    int data_seg;

    /**
      * The segkind instance variable is used to remember the kind of
      * segment and unit this is.
      */
    segkind_t segkind;

    /**
      * The context instance variable is used to remember the translation
      * context we are working within.
      */
    translator_compile *context;

    /**
      * The dataseg_size_current instance variable is used to remember
      * the current size allocated to the separate external data segment
      * for intrinsic units.  Not used for non-instrinsic units.
      */
    bit_address dataseg_size_current;

    /**
      * The dataseg_size_highwater instance variable is used to remember
      * the maximum size allocated to the separate external data segment
      * for intrinsic units.  Not used for non-instrinsic units.
      */
    bit_address dataseg_size_highwater;

    /**
      * The default constructor.  Do not use.
      */
    scope_interface_compile();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    scope_interface_compile(const scope_interface_compile &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    scope_interface_compile &operator=(const scope_interface_compile &rhs);
};

#endif // UCSDPSYS_COMPILE_SCOPE_INTERFACE_COMPILE_H
// vim: set ts=8 sw=4 et :

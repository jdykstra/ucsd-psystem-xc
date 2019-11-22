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

#ifndef UCSDPSYS_COMPILE_SCOPE_UNIT_COMPILE_H
#define UCSDPSYS_COMPILE_SCOPE_UNIT_COMPILE_H

#include <lib/scope/unit.h>
#include <lib/segkind.h>

class translator_compile; // forward

/**
  * The scope_unit_compile class is used to represent code generation
  * for a unit.
  */
class scope_unit_compile:
    public scope_unit
{
public:
    typedef boost::shared_ptr<scope_unit_compile> pointer;

    /**
      * The destructor.
      */
    virtual ~scope_unit_compile();

    /**
      * The create class method is used to create dynamically allocated
      * instances of this class.
      *
      * @param name
      *     The name of the unit.
      * @param code_seg
      *     the INTRINSIC CODE segment number specified in the unit
      *     declaration.
      * @param data_seg
      *     the INTRINSIC DATA segment number specified in the unit
      *     declaration.
      * @param llht
      *     lex level height of the unit
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, int code_seg, int data_seg,
        int llht, translator_compile *cntxt);

    /**
      * The create class method is used to create dynamically allocated
      * instances of this class.
      *
      * @param name
      *     The name of the unit.
      * @param code_seg
      *     the INTRINSIC CODE segment number specified in the unit
      *     declaration.
      * @param data_seg
      *     the INTRINSIC DATA segment number specified in the unit
      *     declaration.
      * @param llht
      *     lex level height of the unit
      * @param segkind
      *     The kind of segment to be created.
      * @param cntxt
      *     The translation context we are working within.
      */
    static pointer create(const rcstring &name, int code_seg, int data_seg,
        int llht, segkind_t segkind, translator_compile *cntxt);

protected:
    // See base class for documentation.
    void pre_order_traversal() const;

    // See base class for documentation.
    void post_order_traversal() const;

    // See base class for documentation.
    void constant_declaration_notify(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    void type_declaration_notify(const variable_name &name,
        const type::pointer &tp);

    // See base class for documentation.
    void variable_declaration_notify(const symbol_variable_p &svp);

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
    void unit_interface_begin(void);

    // See base class for documentation.
    void unit_implementation_begin(void);

    // See base class for documentation.
    bit_address get_next_bit_address(unsigned bit_width);

private:
    /**
      * The constructor.
      *
      * @param name
      *     The name of the segment to be created
      * @param code_seg
      *     The segment number of the code part of the segment
      * @param data_seg
      *     The segment number of the data part of the segment,
      *     or -1 if not meaningful.
      * @param llht
      *     The lex level height of the segment.
      * @param segkind
      *     The segment kind for the new segment.
      * @param cntxt
      *     The translation context we are working within.
      */
    scope_unit_compile(const rcstring &name, int code_seg, int data_seg,
        int llht, segkind_t segkind, translator_compile *cntxt);

    /**
      * The code instance variable is used to remeber where to send our
      * generated code.
      */
    translator_compile &code;

    /**
      * The proc_num instance variable is used to remember the procedure
      * number of the unit initialization code.
      */
    int proc_num;

    /**
      * The implementation_phase instance variable is used to remember
      * whether or not we are in the IMPLEMENTATION phase of a UNIT
      * definition (the other phase being the INTERFACE phase).
      */
    bool implementation_phase;

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
      * The segkind instance variable is used to remember the kind of
      * segment being generated (or, initially, the fully linked form).
      */
    segkind_t segkind;

    /**
      * The default constructor.
      */
    scope_unit_compile();

    /**
      * The copy constructor.
      */
    scope_unit_compile(const scope_unit_compile &);

    /**
      * The assignment operator.
      */
    scope_unit_compile &operator=(const scope_unit_compile &);
};

#endif // UCSDPSYS_COMPILE_SCOPE_UNIT_COMPILE_H
// vim: set ts=8 sw=4 et :

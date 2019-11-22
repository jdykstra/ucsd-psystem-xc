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

#ifndef LIB_SEGMENT_BUILDER_INTERFACE_H
#define LIB_SEGMENT_BUILDER_INTERFACE_H

#include <lib/segment/builder.h>

/**
  * The segment_builder_interface class is used to represent the
  * processing (or rather, lack of processing) required to "code
  * generate" unit interface definitions.
  */
class segment_builder_interface:
    public segment_builder
{
public:
    typedef boost::shared_ptr<segment_builder_interface> pointer;

    /**
      * The destructor.
      */
    virtual ~segment_builder_interface();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param name
      *     The name of the segment interface.
      * @param number
      *     The number of the segment interface.
      */
    static pointer create(const rcstring &name, unsigned number);

protected:
    // See base class for documentation.
    bool emit_byte(unsigned char c);

    // See base class for documentation.
    bool emit_word(unsigned n);

    // See base class for documentation.
    bool emit_big(unsigned n);

    // See base class for documentation.
    bool emit_word_alignment(unsigned char value);

    // See base class for documentation.
    bool emit_odd_alignment(unsigned char value);

    // See base class for documentation.
    int next_procedure_number();

    // See base class for documentation.
    void procedure_parameter_size(unsigned nbytes);

    // See base class for documentation.
    void procedure_variable_size(unsigned nbytes);

    // See base class for documentation.
    void procedure_enter_ic();

    // See base class for documentation.
    void procedure_exit_ic();

    // See base class for documentation.
    void procedure_end(int procedure_number, int lex_level,
        const code_source_map::pointer &listing, mtype_t mtype);

    // See base class for documentation.
    void complete();

    // See base class for documentation.
    unsigned get_address() const;

    // See base class for documentation.
    void poke(unsigned address, unsigned char value);

    // See base class for documentation.
    void poke_word(unsigned address, unsigned value);

    // See base class for documentation.
    int new_jump_table_entry(unsigned address);

    // See base class for documentation.
    unsigned get_segment_number() const;

    // See base class for documentation.
    rcstring get_segment_name() const;

    // See base class for documentation.
    void unit_interface_text(const rcstring &text);

    // See base class for documentation.
    void segment_not_linked(const link_info::pointer &lip);

    // See base class for documentation.
    void link_globref_big(const rcstring &name);

    // See base class for documentation.
    void link_publref_big(const rcstring &name);

    // See base class for documentation.
    void link_publdef(const rcstring &name, int base_offset);

    // See base class for documentation.
    void link_privref_big(const rcstring &name, int nwords);

    // See base class for documentation.
    void reloc_type1_lcrel(unsigned address);

    // See base class for documentation.
    void reloc_type2_ref(unsigned address);

    // See base class for documentation.
    void reloc_type3_pubpriv(unsigned address);

    // See base class for documentation.
    bool is_an_interface(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param name
      *     The name of the segment interface.
      * @param number
      *     The number of the segment interface.
      */
    segment_builder_interface(const rcstring &name, unsigned number);

    /**
      * The name instance variable is used to remember the name of the
      * unit interface.
      */
    rcstring name;

    /**
      * The number instance variable is used to remember the number of
      * the unit interface.
      */
    unsigned number;

    /**
      * The next_proc_num instance variable is used to remember the next
      * procedure number to hand out.
      */
    int next_proc_num;

    /**
      * The default constructor.  Do not use.
      */
    segment_builder_interface();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    segment_builder_interface(const segment_builder_interface &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    segment_builder_interface &operator=(const segment_builder_interface &rhs);
};

#endif // LIB_SEGMENT_BUILDER_INTERFACE_H
// vim: set ts=8 sw=4 et :

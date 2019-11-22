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

#ifndef LIB_SEGMENT_BUILDER_CODE_H
#define LIB_SEGMENT_BUILDER_CODE_H

#include <lib/ac/list>

#include <lib/segment.h>
#include <lib/segment/builder.h>

class codefile; // forward

/**
  * The segment_builder_code class is used to represent the state of
  * segment construction, when constructing a segment in a codefile.
  */
class segment_builder_code:
    public segment_builder
{
public:
    typedef boost::shared_ptr<segment_builder_code> pointer;

    /**
      * The destructor.
      */
    virtual ~segment_builder_code();

    /**
      * The create class method is used to allocate new dynamicall
      * allocated instances of this class.
      *
      * @param sp
      *     The codefile segment being constructed.
      * @param cfp
      *     The codefile the segment is to reside in
      */
    static pointer create(const segment::pointer &sp, codefile *cfp);

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
    int next_procedure_number(void);

    // See base class for documentation.
    void procedure_parameter_size(unsigned nbytes);

    // See base class for documentation.
    void procedure_variable_size(unsigned nbytes);

    // See base class for documentation.
    void procedure_enter_ic(void);

    // See base class for documentation.
    void procedure_exit_ic(void);

    // See base class for documentation.
    void procedure_end(int procedure_number, int lex_level,
        const code_source_map::pointer &listing, mtype_t mtype);

    // See base class for documentation.
    void complete(void);

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
      * It is private on purpose, use the #create class method instead.
      *
      * @param sp
      *     The codefile segment being constructed.
      * @param cfp
      *     The codefile the segment is to reside in
      */
    segment_builder_code(const segment::pointer &sp, codefile *cfp);

    /**
      * The get_nextlc method is used to calculate the nextlc value to
      * be placed into an EOFMARK link information record.  It is 1 +
      * number of data words of procedure 1 in the segment, or simply 1
      * if that makes no sense for this kind of segment.
      *
      * assert(get_nextlc() > 0);
      * assert(get_nextlc() < 32768);
      */
    int get_nextlc(void) const;

    segment::pointer sp;
    unsigned char *data;
    unsigned address;
    unsigned high_water;

    /**
      * The maximum_procedure_number instance variable is used to
      * remeber the highest procedure number allocated to date.  (It is
      * also the count of procedure numbers allocated.)
      */
    int maximum_procedure_number;

    unsigned param_size;
    unsigned data_size;
    unsigned enter_ic;
    bool enter_ic_set;
    unsigned exit_ic;

    /**
      * The procedure instance variable is used to remember the absolute
      * addresses of the procedure attributes for each procedure.
      * The complete method will use this information to generate
      * self-relative pointers into the procedure dictionary.
      */
    unsigned procedure_dictionary[256];

    /**
      * The jump_table_size instance variable is used to remember how
      * many jump table entries have been used to date.
      */
    unsigned jump_table_size;

    /**
      * The jump_table instance variable is used to remeber the
      * *destination* of the jump in the jump table.  It will eventually
      * be stored self-relative, but that has to wait until we know the
      * code size of the current procedure.
      */
    unsigned jump_table[64];

    typedef std::list<unsigned> reloc_t;

    /**
      * The reloc instance variable is used to remember the relocation
      * data that must be emitted.
      *
      * [0] no idea (it's a II.1 thing?)
      * [1] relative to the code address
      * [2] .ref, .const
      * [3] .public, .private
      */
    reloc_t reloc[4];

    /**
      * The put_reloc method is used to emit the per-procedure
      * relocation data into the code stream.
      *
      * @param addrs
      *     The list of addresses to emit.
      */
    void put_reloc(reloc_t &addrs);

    /**
      * The cfp instance variable is used to remember the codefile this
      * segment will eventually reside in.
      */
    codefile *cfp;

    /**
      * The default constructor.  Do not use.
      */
    segment_builder_code();

    /**
      * The copy constructor.  Do not use.
      */
    segment_builder_code(const segment_builder_code &);

    /**
      * The assignment operator.  Do not use.
      */
    segment_builder_code &operator=(const segment_builder_code &);
};

#endif // LIB_SEGMENT_BUILDER_CODE_H
// vim: set ts=8 sw=4 et :

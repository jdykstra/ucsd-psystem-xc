//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010-2012 Peter Miller
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

#ifndef LIB_SEGMENT_BUILDER_H
#define LIB_SEGMENT_BUILDER_H

#include <lib/code_source_map.h>
#include <lib/link_info.h>
#include <lib/mtype.h>
#include <lib/rcstring.h>

/**
  * The segment_builder abstract base class is used to represent the
  * state of segment construction.
  */
class segment_builder
{
public:
    typedef boost::shared_ptr<segment_builder> pointer;

    /**
      * The destructor.
      */
    virtual ~segment_builder();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      */
    segment_builder();

public:
    /**
      * The emit_byte method is used to add a byte to the segment under
      * construction.  The data space needed to the segment for things
      * like the jump table and procedure dictionary are taken into
      * account when determining whether or not there is room in the
      * segment for the byte of data.
      *
      * @param c
      *     The byte value to insert.
      * @returns
      *     true if the byte will fit, or false if it will not fit
      */
    virtual bool emit_byte(unsigned char c) = 0;

    /**
      * The emit_word_alignment method is used to add a single filler
      * byte, if necessary, so that the next data in the code stream
      * will be word aligned.
      */
    virtual bool emit_word_alignment(unsigned char value = 0) = 0;

    /**
      * The emit_odd_alignment method is used to add a single filler
      * byte, if necessary, so that the next data in the code stream
      * will be word aligned.
      */
    virtual bool emit_odd_alignment(unsigned char value = 0) = 0;

    /**
      * The emit_word method is used to add a word to the segment under
      * construction.  It operates via the emit_byte method.
      *
      * @param n
      *     The word value to insert.
      * @returns
      *     true if the word will fit, or false if it will not fit
      */
    virtual bool emit_word(unsigned n) = 0;

    /**
      * The emit_big method is used to add a "big" to the segment under
      * construction.  It operates via the emit_byte method.  The "big"
      * values take one byte of they are < 128, otherwise they take two.
      *
      * @param n
      *     The "big" value to insert.
      * @returns
      *     true if the "big" will fit, or false if it will not fit
      */
    virtual bool emit_big(unsigned n) = 0;

    /**
      * The next_procedure_number method is used to obtain the next
      * available procedure number, and allocate space in the procedure
      * dictionary for it.
      *
      * @returns
      *     int; will always be >= 1 on success, or <0 on too many
      *     procedures per segment.
      */
    virtual int next_procedure_number(void) = 0;

    /**
      * The procedure_parameter_size method is used to cache the fact
      * that the current procedure under construction has the given
      * number of bytes of call parameters.
      *
      * @param nbytes
      *     The number of bytes of call parameters
      */
    virtual void procedure_parameter_size(unsigned nbytes) = 0;

    /**
      * The procedure_variable_size method is used to cache the fact
      * that the current procedure under construction has the given
      * number of bytes of local variables.
      *
      * @param nbytes
      *     The number of bytes of call parameters
      */
    virtual void procedure_variable_size(unsigned nbytes) = 0;

    /**
      * The procedure_enter_ic method is used to cache the fact that the
      * current procedure under construction begins at the current code
      * address.
      */
    virtual void procedure_enter_ic(void) = 0;

    /**
      * The procedure_exit_ic method is used to cache the fact that the
      * current procedure under construction's return code begins at the
      * current code address.
      */
    virtual void procedure_exit_ic(void) = 0;

    /**
      * The procedure_end method is used to build the procedure
      * attributes for the current procedure under construction, as weel
      * as the jump table and relocation data.
      *
      * @param procedure_number
      *     This is the value previously returned by the
      *     next_procedure_number method.
      * @param lex_level
      *     The function declaration nesting level (this is different than
      *     the run-time recursion level).
      * @param listing
      *     Where to print this listing, or NULL for no listing.
      * @param mtype
      *     The machine type of this procedure.
      */
    virtual void procedure_end(int procedure_number, int lex_level,
        const code_source_map::pointer &listing, mtype_t mtype) = 0;

    /**
      * The complete method is used to write the code segment data,
      * including the procedure dictionary, into the code file.
      */
    virtual void complete(void) = 0;

    /**
      * The get_address method is used to obtain the current code
      * address of the current segment.  This should only be used by the
      * lable_compile class.
      */
    virtual unsigned get_address(void) const = 0;

    /**
      * The poke method is used to insert a byte into the code stream.
      * This shall only be used by the label_compile class.
      *
      * @param address
      *     The address of the byte to poke, relative to the start of
      *     the segment's code.
      * @param value
      *     The value to poke into the byte.
      */
    virtual void poke(unsigned address, unsigned char value) = 0;

    /**
      * The poke_word method is used to insert a word into the code stream.
      * This shall only be used by the label_compile class.
      *
      * @param address
      *     The address of the word to poke, relative to the start of
      *     the segment's code.
      * @param value
      *     The value to poke into the word.
      */
    virtual void poke_word(unsigned address, unsigned value) = 0;

    /**
      * The new_jump_table_entry method is used to get another jump
      * table entry for the specified address.
      *
      * @param address
      *     The destination of the jump table entry (the actual value
      *     will be self relative, once the size of the jump table is
      *     known).
      */
    virtual int new_jump_table_entry(unsigned address) = 0;

    /**
      * The get_segment_number method is used to obtain the segment
      * number of the segment being built.
      */
    virtual unsigned get_segment_number(void) const = 0;

    /**
      * The get_segment_name method is used to obtain name name of the
      * segment being constructed.
      */
    virtual rcstring get_segment_name(void) const = 0;

    /**
      * The unit_interface_text method is used to write the text of the
      * INIT's interface to the codefile.
      *
      * @param text
      *     The text of the unit source, between the INTERFACE and
      *     IMPLEMENTATION keywords (includes neither keyword).
      */
    virtual void unit_interface_text(const rcstring &text) = 0;

    /**
      * The segment_not_linked method is used in reponse to the EXTERNAL
      * keyword to tell a segment under construction that it is to use
      * an unlinked segkind, rather than a linked segkind.
      *
      * @param lip
      *     The linker information required to resolve the symbol
      *     reference, when the linker is used to glue the different
      *     parts of the program together.
      */
    virtual void segment_not_linked(const link_info::pointer &lip) = 0;

    /**
      * The link_globref_big method is used to add GLOBREF BIG link
      * information to the segment being constructed, at the current
      * address.
      *
      * @param name
      *     The name of the symbol to be linked.
      */
    virtual void link_globref_big(const rcstring &name) = 0;

    /**
      * The link_publref_big method is used to add PUBLREF BIG link
      * information to the segment being constructed, at the current
      * address.
      *
      * @param name
      *     The name of the symbol to be linked.
      */
    virtual void link_publref_big(const rcstring &name) = 0;

    /**
      * The link_publdef method is used to add PUBLDEF link information
      * to the segment being constructed.
      *
      * @param name
      *     The name of the symbol to be linked.
      * @param base_offset
      *     The word offset of the symbol, relative to the stack frame
      *     of the current procedure.
      */
    virtual void link_publdef(const rcstring &name, int base_offset) = 0;

    /**
      * The link_privref_big method is used to add PRIVREF BIG link
      * information to the segment being constructed, at the current
      * address.
      *
      * @param name
      *     The name of the symbol to be linked.
      * @param nwords
      *     The size of the variable, in words.
      */
    virtual void link_privref_big(const rcstring &name, int nwords) = 0;

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 1 (address relative) relocation word data.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    virtual void reloc_type1_lcrel(unsigned address) = 0;

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 2 (.ref, .const) relocation word data.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    virtual void reloc_type2_ref(unsigned address) = 0;

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 3 (.public, .private) relocation word data.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    virtual void reloc_type3_pubpriv(unsigned address) = 0;

    /**
      * The is_an_interface method may be used to determine whether or
      * not a scope is an interface scope.
      */
    virtual bool is_an_interface(void) const = 0;

private:
    /**
      * The copy constructor.  Do not use.
      */
    segment_builder(const segment_builder &);

    /**
      * The assignment operator.  Do not use.
      */
    segment_builder &operator=(const segment_builder &);
};

#endif // LIB_SEGMENT_BUILDER_H
// vim: set ts=8 sw=4 et :

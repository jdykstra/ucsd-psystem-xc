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

#ifndef LIB_SEGMENT_BUILDER_STACK_H
#define LIB_SEGMENT_BUILDER_STACK_H

#include <lib/code_source_map.h>
#include <lib/segment/builder.h>

/**
  * The segment_builder_stack class is used to represent a stack of
  * segment builder instances.  Code is generated into the top-most
  * segment builder.
  */
class segment_builder_stack
{
public:
    /**
      * The destructor.
      */
    virtual ~segment_builder_stack();

    /**
      * The default constructor.
      */
    segment_builder_stack();

    /**
      * The push_back method is used to add another segment_builder
      * instance to the top of the stack.
      *
      * @param sbp
      *     The segment builder to push onto the top of the stack.
      */
    void push_back(segment_builder::pointer sbp);

    /**
      * The pop_back method is used to pop a segment off the stack.
      * It does <b>not</b> call the segment's complete() method.
      */
    void pop_back(void);

    /**
      * The emit_byte method is used to add another byte to the segment
      * on the top of the stack.
      *
      * @param value
      *     The value to add to the top-most segment
      * @returns
      *     bool; true if the byte fits, false if there is not enough room.
      */
    bool emit_byte(unsigned char value);

    /**
      * The emit_word_alignment method is used to add a single filler
      * byte, if necessary, so that the next data in the code stream
      * will be word aligned.
      */
    bool emit_word_alignment(unsigned char value = 0);

    /**
      * The emit_odd_alignment method is used to add a single filler
      * byte, if necessary, so that the next data in the code stream
      * will not be word aligned.
      */
    bool emit_odd_alignment(unsigned char value = 0);

    /**
      * The emit_word method is used to add another word to the segment
      * on the top of the stack.
      *
      * @param value
      *     The value to add to the top-most segment
      * @returns
      *     bool; true if the word fits, false if there is not enough room.
      */
    bool emit_word(unsigned value);

    /**
      * The next procedure_number method is used to obtain the next
      * available procedure within the current segment.
      *
      * @returns
      *     int; >0 on success, <0 when too many procedures
      */
    int next_procedure_number();

    /**
      * The procedure_parameter_size method is used to inform the
      * current segment under construction that the current procedure
      * under construction has the given number of bytes of call
      * parameters.
      *
      * @param nbytes
      *     The number of bytes of call parameters
      */
    void procedure_parameter_size(unsigned nbytes);

    /**
      * The procedure_variable_size method is used to inform the current
      * segment under construction that the current procedure under
      * construction has the given number of bytes of local variables.
      *
      * @param nbytes
      *     The number of bytes of call parameters
      */
    void procedure_variable_size(unsigned nbytes);

    /**
      * The procedure_enter_ic method is used to inform the current
      * segment under construction that the current procedure under
      * construction begins at the current code address (know to the
      * segemnt builder, but not to the caller).
      */
    void procedure_enter_ic();

    /**
      * The procedure_exit_ic method is used to inform the current
      * segment under construction that the current procedure under
      * construction's return code begins at the current code address
      * (know to the segemnt builder, but not to the caller).
      */
    void procedure_exit_ic();

    /**
      * The procedure_end method is used to inform the current segment
      * under construction that the current procedure under construction
      * is now complete, and thus the jump table and procedure attributes
      * may be written.  This also gives a concrete value for the
      * pointer in the procedure dictionary.
      *
      * @param procedure_number
      *     This is the value previously returned by the
      *     next_procedure_number method.
      * @param lex_level
      *     The lexical level of the procedure
      * @param listing
      *     When to send the (source code plus disassembly) listing.
      * @param mtype
      *     The machine type of the procedure.
      */
    void procedure_end(int procedure_number, int lex_level,
        const code_source_map::pointer &listing, mtype_t mtype);

    /**
      * The segment_end method is used to end the segment on the top of
      * the stack, and calls the #pop_back method when done.
      */
    void segment_end();

    /**
      * The get_address method is used to obtain the current code
      * address of the current segment.
      */
    unsigned get_address() const;

    /**
      * The poke method is used to insert a byte into the code stream.
      *
      * @param address
      *     The address of the byte to poke, relative to the start of
      *     the segment's code.
      * @param value
      *     The value to poke into the byte.
      */
    void poke(unsigned address, unsigned char value);

    /**
      * The poke_word method is used to insert a word into the code stream.
      *
      * @param address
      *     The address of the word to poke, relative to the start of
      *     the segment's code.
      * @param value
      *     The value to poke into the word.
      */
    void poke_word(unsigned address, unsigned value);

    /**
      * The new_jump_table_index method is used to get another jump
      * table entry for the specified address.
      *
      * @param address
      *     The destination of the jump table entry (the actual value
      *     will be self relative, once the size of the jump table is
      *     known).
      */
    int new_jump_table_entry(unsigned address);

    /**
      * The get_segment_number method is used to obtain the number of
      * the current (top most) segment.
      */
    unsigned get_segment_number() const;

    /**
      * The back method may be used to obtain a pointer to the segment
      * builder on the top of the stack.
      */
    segment_builder::pointer back(void) const;

    /**
      * The unit_interface_text method is used to write the segment
      * interface text into the codefile.
      *
      * @param text
      *     The text of the unit source, between the INTERFACE and
      *     IMPLEMENTATION keywords (includes nether keyword).
      */
    void unit_interface_text(const rcstring &text);

    /**
      * The segment_not_linked method is used in reponse to the EXTERNAL
      * keyword to tell a segment under construction that it is to use
      * an unlinked segkind, rather than a linked segkind.
      *
      * @param lip
      *     The accompanying link_info for use by the linker.
      *     It will either be EXTPROC or EXTFUNC.
      */
    void segment_not_linked(const link_info::pointer &lip);

    /**
      * The link_globref_big method is used to add GLOBREF BIG link
      * information to the segment being constructed, at the current
      * address.
      *
      * @param name
      *     The name of the symbol to be linked.
      */
    void link_globref_big(const rcstring &name);

    /**
      * The link_publref_big method is used to add PUBLREF BIG link
      * information to the segment being constructed, at the current
      * address.
      *
      * @param name
      *     The name of the symbol to be linked.
      */
    void link_publref_big(const rcstring &name);

    /**
      * The link_privref_big method is used to add PRIVREF BIG link
      * information to the segment being constructed, at the current
      * address.
      *
      * @param name
      *     The name of the symbol to be linked.
      * @param nwords
      *     The size of the variable, in words
      */
    void link_privref_big(const rcstring &name, int nwords);

    /**
      * The emit_publdef method is used to add a variable to the current
      * procedure's linker info, defining a public variable.
      *
      * @param name
      *     The name of the variable
      * @param base_offset
      *     The offset of the variable, relative to the current
      *     procedure's stack frame.
      */
    void link_publdef(const rcstring &name, int base_offset);

private:
    /**
      * The length instance variable is used to remember
      * how much of the allocated list array is in use.
      */
    size_t length;

    /**
      * The maximum instance variable is used to remember
      * how much space was allocated to the list array.
      */
    size_t maximum;

    /**
      * The list instance variable is used to remember the loction of
      * the dynamically allocated space.
      */
    segment_builder::pointer *list;

    /**
      * The representation method is used to obtain a simple string
      * describing the scope stack (a list of names).  This can be
      * useful for debugging.
      */
    rcstring representation(void) const;

    /**
      * The copy constructor.  Do not use.
      */
    segment_builder_stack(const segment_builder_stack &);

    /**
      * The assignment operator.  Do not use.
      */
    segment_builder_stack &operator=(const segment_builder_stack &);
};

#endif // LIB_SEGMENT_BUILDER_STACK_H
// vim: set ts=8 sw=4 et :

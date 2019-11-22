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

#ifndef UCSDPSYS_COMPILE_TRANSLATOR_COMPILE_H
#define UCSDPSYS_COMPILE_TRANSLATOR_COMPILE_H

#include <lib/codefile.h>
#include <lib/code_source_map.h>
#include <lib/translator.h>
#include <lib/segment/builder/stack.h>

/**
  * The translator_compile class is used to represent translation phase
  * from text to UCSD p-System code file.
  */
class translator_compile:
    public translator
{
public:
    /**
      * The destructor.
      */
    virtual ~translator_compile();

    /**
      * The constructor.
      */
    translator_compile(const rcstring &outfile);

    /**
      * The listing method is used to enable listing, and to specify the
      * filename to be listed to.
      */
    void listing_to_file(const rcstring &filename);

    // See base class for documentation.
    scope::pointer program_scope_factory(const rcstring &name);

    // See base class for documentation.
    scope::pointer builtin_scope_factory(void);

    // See base class for documentation.
    scope::pointer unit_scope_factory(const rcstring &name, int code_segnum,
        int data_segnum, int llht, segkind_t segkind);

    // See base class for documentation.
    scope::pointer function_scope_factory(const rcstring &name, bool is_func,
        bool new_seg);

    // See base class for documentation.
    scope::pointer compound_temporary_scope_factory(scope_stack &stk);

    // See base class for documentation.
    scope::pointer with_temporary_scope_factory(scope_stack &stk,
        const expression::pointer &base, const statement::pointer &assign);

    // See base class for documentation.
    statement::pointer expression_statement_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    statement::pointer if_statement_factory(
        const expression::pointer &condition,
        const statement::pointer &then_clause,
        const statement::pointer &else_clause);

    // See base class for documentation.
    statement::pointer while_statement_factory(
        const expression::pointer &condition, const statement::pointer &body);

    // See base class for documentation.
    statement::pointer until_statement_factory(const statement_list &body,
        const expression::pointer &condition);

    // See base class for documentation.
    statement::pointer infinite_loop_factory(const statement::pointer &body);

    // See base class for documentation.
    statement::pointer writeln_statement_factory(const expression_list &args,
        bool eoln);

    // See base class for documentation.
    expression::pointer boolean_expression_factory(const location &locn,
        bool value);

    // See base class for documentation.
    expression::pointer integer_plus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_plus_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_union_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_minus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_minus_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_difference_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set2set_expression_factory(
        const type::pointer &to, const expression::pointer &from);

    // See base class for documentation.
    expression::pointer integer_times_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_times_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_intersection_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_div_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer modulo_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_over_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_expression_factory(const location &locn,
        const rcstring &value);

    // See base class for documentation.
    expression::pointer lint_expression_factory(
        const location &locn, const rcstring &value);

    // See base class for documentation.
    expression::pointer real_expression_factory(const location &locn,
        const rcstring &value);

    // See base class for documentation.
    expression::pointer real_from_integer_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer string_expression_factory(const location &locn,
        const rcstring &value);

    // See base class for documentation.
    expression::pointer integer_negate_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer real_negate_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer lint_negate_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer boolean_eq_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_eq_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_eq_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer string_eq_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer array_eq_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer record_eq_record_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer record_ne_record_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_eq_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_ne_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_le_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_lt_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_ge_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_gt_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer boolean_ne_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_ne_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_ne_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer string_ne_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer boolean_lt_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_lt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_lt_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer string_lt_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer boolean_le_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_le_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_le_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer string_le_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer lint_le_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer boolean_gt_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_gt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_gt_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer string_gt_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer boolean_ge_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer integer_ge_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_ge_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer string_ge_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    void codefile_copyright(const rcstring &text);

    /**
      * The emit_byte method is used to emit a byte into the current
      * segment's code stream.
      *
      * @param value
      *     The value of the byte to be written.
      * @returns
      *     bool; true if the byte fits, false if we have run out of
      *     room in the current segment.
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
      * The emit_word method is used to emit a two byte word into the
      * current segment's code stream.
      *
      * @param value
      *     The value of the word to be written.
      * @returns
      *     bool; true if the word fits, false if we have run out of
      *     room in the current segment.
      */
    bool emit_word(unsigned value);

    /**
      * The emit_big method is used to emit a two byte "big" into the
      * current segment's code stream if the value is 128 or more, and a
      * single byte "big" if the value is less than 128..
      *
      * @param value
      *     The value of the word to be written.
      * @returns
      *     bool; true if the word fits, false if we have run out of
      *     room in the current segment.
      */
    bool emit_big(unsigned value);

    /**
      * The emit_big_globref method is used to emit a linkage request at
      * the current address for the given symbol name, and them emit a
      * 2-byte Big value equally zero to reserve space for it.
      *
      * A globref is a linkage term, meaning a reference to an
      * as-yet-unlinked global variable.  Its offset will be determined
      * by the linker, and patched in then.
      *
      * @param name
      *     The name of the symbol being referenced.
      */
    bool emit_big_globref(const rcstring &name);

    /**
      * The emit_big_publref method is used to emit a linkage request at
      * the current address for the given symbol name, and them emit a
      * 2-byte Big value equally zero to reserve space for it.
      *
      * A publref is a linkage term, meaning a reference to an
      * as-yet-unlinked global variable.  Its offset will be determined
      * by the linker, and patched in then.
      *
      * @param name
      *     The name of the symbol being referenced.
      */
    bool emit_big_publref(const rcstring &name);

    /**
      * The link_publdef method may be used to emit a public variable
      * defintiion into the current procedures linker information.
      *
      * @param name
      *     The name of the variable being defined.
      * @param base_offset
      *     The word offset of the variable, relative to the currently
      *     being-linked procedures stack frame.
      */
    void link_publdef(const rcstring &name, int base_offset);

    /**
      * The emit_big_privref method is used to emit a linkage request at
      * the current address for the given symbol name, and them emit a
      * 2-byte Big value equally zero to reserve space for it.
      *
      * A privref is a linkage term, meaning a reference to an
      * as-yet-unlinked global variable, but private to a unit's
      * implementation.  Its offset will be determined by the linker,
      * and patched in then.
      *
      * @param name
      *     The name of the symbol being referenced.
      * @param nwords
      *     The size of the variable, in words.
      */
    bool emit_big_privref(const rcstring &name, int nwords);

    /**
      * The emit_ldci method is used to push a constant integer value
      * onto the stack.  It optimizes for space, rather than speed.
      *
      * @param n
      *     the value of the constant to be pushed
      * @returns
      *     bool; true if the word fits, false if we have run out of
      *     room in the current segment.
      */
    bool emit_ldci(int n);

    /**
      * The ldci_nbytes method may be used to determine how many bytes
      * of code the translator will use to represent the given constant
      * value.
      */
    int ldci_nbytes(int n) const;

    /**
      * The emit_ldc method is used to emit an LCD (Load Constant)
      * opcode (or a functionally identical equalivalent) into the code
      * stream.
      *
      * LDC UB, /block/
      *     LoaD multiple-word Constant: Fetch word-aligned /block/
      *     of UB words in reverse order, and push the block onto the
      *     stack.
      *
      * @param data
      *     Pointer to bas eof array of word data.
      * @param data_size
      *     The size of the array, in words.
      * @returns
      *     true if there was room for the cde, false if the segment ran
      *     out of code space.
      */
    bool emit_ldc(const unsigned short *data, size_t data_size);

    /**
      * The error method is used to print error messages using the given
      * location.  If there are any errors, the compile will exit with a
      * failure exit status.
      *
      * @param fmt
      *     The format of the rest of the arguments.  See printf(3) for
      *     more information.
      */
    void error(const location &, const char *fmt, ...);

    /**
      * The new_segment method is used to create a new code segment to
      * hold code.  Any emit_byte calls will be delivered into the new
      * segments until (a) a segment procedure happens, or (b) scope_end is
      * called.
      *
      * @param name
      *     The name of the new segment.
      */
    void new_segment(const rcstring &name);

    /**
      * The new_segment method is used to create a new code segment to
      * hold code.  Any emit_byte calls will be delivered into the new
      * segments until scope_end is called.
      *
      * @param name
      *     The name of the new segment.
      * @param segnum
      *     The user-specified segment number.
      */
    void new_segment(const rcstring &name, int segnum);

    /**
      * The new_segment method is used to create a new code segment to
      * hold code.  Any emit_byte calls will be delivered into the new
      * segments until scope_end is called.
      *
      * @param name
      *     The name of the new segment.
      * @param segnum
      *     The user-specified segment number.
      * @param segkind
      *     The kind of segment to be created.
      */
    void new_segment(const rcstring &name, int segnum, segkind_t segkind);

    /**
      * The next_procedure_number method is used to obtain the next
      * available procedure number, and allocate space in the procedure
      * dictionary for it.
      *
      * @returns
      *     int; >0 on success, <0 when too many procedures
      */
    int next_procedure_number(void);

    /**
      * The procedure_parameter_size method is used to inform the
      * current segment under construction that the current procedure
      * under construction has the given number of bytes of call
      * parameters.
      *
      * @param nbytes
      *     The number of bytes of call arguments
      */
    void procedure_parameter_size(unsigned nbytes);

    /**
      * The procedure_variable_size method is used to inform the current
      * segment under construction that the current procedure under
      * construction has the given number of bytes of local variables.
      *
      * @param nbytes
      *     The number of bytes of call arguments
      */
    void procedure_variable_size(unsigned nbytes);

    /**
      * The procedure_enter_ic method is used to inform the current
      * segment under construction that the current procedure under
      * construction begins at the current code address (know to the
      * segemnt builder, but not to the caller).
      */
    void procedure_enter_ic(void);

    /**
      * The procedure_exit_ic method is used to inform the current
      * segment under construction that the current procedure under
      * construction's return code begins at the current code address
      * (know to the segemnt builder, but not to the caller).
      */
    void procedure_exit_ic(void);

    /**
      * The procedure_end method is used to inform the current segment
      * under construction that the current procedure under construction
      * is not complete, and the jumps table and procedure attributes
      * may be written.  This also gives a concrete value for the
      * pointer in the procedure dictionary.
      *
      * @param procedure_number
      *     This is the value previously returned by the
      *     next_procedure_number method.
      * @param lex_level
      *     The lexical level of the procedure -1 is system, 0 is user
      *     prog, more is nested functions).
      */
    void procedure_end(int procedure_number, int lex_level);

    /**
      * The segment_end method is used to complete the current segment.
      */
    void segment_end(void);

    /**
      * The get_address method is used to obtain the current code
      * address of the current segment.  This should only be used by the
      * lable_compile class.
      */
    unsigned get_address(void) const;

    /**
      * The poke method is used to insert a byte into the code stream,
      * overwriting an existing place-marker byte (jump offset).  This
      * shall only be used by the label_compile class.
      *
      * @param address
      *     The address of the byte to be poked.
      * @param value
      *     The value to be poked into the byte.
      */
    void poke(unsigned address, unsigned char value);

    /**
      * The poke_word method is used to insert a word into the code
      * stream, overwriting an existing place-marker byte (self relative
      * jump offset).  This shall only be used by the label_compile
      * class.
      *
      * @param address
      *     The address of the word to be poked.
      * @param value
      *     The value to be poked into the word.
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
      * The get_segment_number method is used to obtain the number
      * of the curremt segment.  This is needed generate correct
      * cross-segment calls.
      */
    unsigned get_segment_number(void) const;

    /**
      * The breakpoint method is used to (conditionally) insert a
      * breakpoint into the code.
      *
      * @param locn
      *     The location for the breakpoint.
      */
    void breakpoint(const location &locn);

    /**
      * The need_segment method is used to have the compiler remember
      * that a segment must be loaded, because the compiler will
      * generate code which calll procedures in it.
      *
      * @param segnum
      *     The number of the segment of interest.
      */
    void need_segment(unsigned segnum);

    /**
      * The segment_needed method is used to determine whether or not
      * code should be generated into the program main procedure to load
      * and unload a particular segment.
      *
      * @param segnum
      *     The number of the segment of interest.
      */
    bool segment_needed(unsigned segnum);

    /**
      * The range_check method is used to build a range check expression.
      *
      * @param value
      *     The value to be checked, and the result of evaluating this
      *     expression.
      * @param against
      *     the type to range check against
      */
    expression::pointer range_check(const expression::pointer &value,
        const type::pointer &against);

    /**
      * The range_check method is used to build a range check expression.
      *
      * @param value
      *     The value to be checked, and the result of evaluating this
      *     expression.
      * @param minimum
      *     The minimum acceptable value.
      * @param maximum
      *     The maximum acceptable value.
      */
    expression::pointer range_check(const expression::pointer &value,
        int minimum, int maximum);

    /**
      * The fix_binary_set_operands method may be used by the binary set
      * operators to ensure that both of their arguments are sized when
      * on the stack.
      *
      * @param lhs
      *     The left hand expression.  It may be changed on return.
      * @param rhs
      *     The right hand expression.  It may be changed on return.
      */
    void fix_binary_set_operands(expression::pointer &lhs,
        expression::pointer &rhs);

    /**
      * The segment_procedure_forward_suspend method is used by the
      * symbol_function_user_defined::notify_forward_suspend
      * method to take the appropriate segment off the segment stack.
      */
    segment_builder::pointer segment_procedure_forward_suspend(void);

    /**
      * The segment_procedure_forward_resume method is used by the
      * symbol_function_user_defined::notify_forward_resume
      * method to put the appropriate segment back on the segment stack.
      */
    void segment_procedure_forward_resume(const segment_builder::pointer &seg);

    /**
      * The value_traversal_for_logical_expression method is used to
      * calculate a boolean value (true or false) from an expression
      * tree that uses short-circuit evaluation.
      *
      * @param ep
      *     The expression tree to be traversed.
      */
    void value_traversal_for_logical_expression(const expression *ep);

    /**
      * The create_dataseg method is used to create the external DATASEG
      * associated with an intrinsic unit.
      *
      * @param name
      *     The name of the segment to be created.  It will be the same
      *     as the associated code segment.
      * @param segnum
      *     The number of the data segment.
      * @param size_in_words
      *     The size of the data segment to be created, in units of
      *     words (two bytes).
      */
    void create_dataseg(const rcstring &name, int segnum, int size_in_words);

protected:
    // See base class for documentation.
    void post_order_traversal(void) const;

    // See base class for documentation.
    label::pointer label_factory(void);

    // See base class for documentation.
    symbol::pointer function_symbol_factory(const rcstring &name,
        const scope::pointer &scp, int lex_level, bool new_seg);

    // See base class for documentation.
    expression::pointer dereference_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer address_of_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer logical_and_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer bitwise_and_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer logical_or_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer bitwise_or_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer logical_not_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer bitwise_not_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer sizeof_expression_factory(const location &locn,
        const type::pointer &arg);

    // See base class for documentation.
    expression::pointer char_expression_factory(const location &locn,
        unsigned char value);

    // See base class for documentation.
    expression::pointer empty_set_expression_factory(const location &locn,
        const type::pointer &tp);

    // See base class for documentation.
    expression::pointer set_expression_factory(
        const expression::pointer &singleton);

    // See base class for documentation.
    expression::pointer set_expression_factory(const expression::pointer &lo,
        const expression::pointer &hi);

    // See base class for docuemntation.
    virtual expression::pointer writeln1_expression_factory(
        const expression::pointer &value);

    // See base class for docuemntation.
    virtual expression::pointer writeln2_expression_factory(
        const expression::pointer &value, const expression::pointer &width);

    // See base class for docuemntation.
    virtual expression::pointer writeln3_expression_factory(
        const expression::pointer &value, const expression::pointer &width,
        const expression::pointer &precision);

    // See base class for documentation.
    symbol::pointer builtin_abs_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_arctan_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_atan_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_blockread_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_blockwrite_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_chr_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_close_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_concat_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_copy_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_cos_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_dec_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_delete_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_eof_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_eoln_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_exit_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_exp_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_fillchar_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_get_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_gotoxy_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_halt_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_idsearch_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_inc_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_insert_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_ioresult_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_length_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_ln_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_log_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_mark_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_memavail_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_moveleft_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_moveright_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_new_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_odd_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_opennew_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_openold_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_ord_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_page_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_pos_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_pred_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_put_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_pwroften_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_read_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_readln_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_release_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_reset_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_rewrite_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_round_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_seek_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_sin_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_sqr_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_sqrt_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_str_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_succ_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_time_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_trunc_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_treesearch_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_unitbusy_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_unitclear_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_unitread_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_unitstatus_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_unitwait_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_unitwrite_factory(void);

    // See base class for documentation.
    symbol_function::pointer builtin_syscall_factory(const rcstring &name,
        int seg_num, int proc_num);

    // See base class for documentation.
    scope::pointer interface_scope_factory(const rcstring &name,
        int code_seg, int data_seg, segkind_t segkind);

    // See base class for documentation.
    scope::pointer record_scope_factory(bool is_packed);

    // See base class for documentation.
    void uses_interface_notify_push(const scope::pointer &sp);

    // See base class for documentation.
    void uses_interface_notify_pop(const scope::pointer &sp);

    // See base class for documentation.
    statement::pointer with_statement_factory(const expression::pointer &ep,
        const statement::pointer &base_asign, const statement::pointer &body);

    // See base class for documentation.
    expression::pointer nil_expression_factory(void);

    // See base class for documentation.
    statement::pointer for_statement_factory(const expression::pointer &var,
        const expression::pointer &limit1, bool descending,
        const expression::pointer &limit2, const statement::pointer &body);

    // See base class for documentation.
    void case_statement(const expression::pointer &control,
        const case_list &cases, const statement::pointer &otherwise);

    // See base class for documentation.
    statement::pointer case_statement_factory(
        const expression::pointer &control, const case_list &cases,
        const statement::pointer &otherwise);

    // See base class for documentation.
    statement::pointer goto_statement_factory(const symbol::pointer &sym);

    // See base class for documentation.
    statement::pointer label_statement_factory(const symbol::pointer &sym,
        const statement::pointer &body);

    // See base class for documentation.
    expression::pointer in_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    // See base class for documentation.
    void codefile_bigendian(bool yesno);

    // See base class for documentation.
    expression::pointer pointer_eq_pointer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer pointer_ne_pointer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer paoc_from_string_expression_factory(
        const expression::pointer &arg, const type::pointer &tp);

    // See base class for documentation.
    expression::pointer array_ne_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer array_lt_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer lint_lt_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer array_le_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer array_gt_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer lint_gt_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer array_ge_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer lint_ge_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    statement::pointer var_statement_factory(const variable_name_list &names,
        const type::pointer &tp);

    // See base class for documentation.
    statement::pointer var_statement_factory(const variable_name &name,
        const expression::pointer &value);

    // See base class for documentation.
    statement::pointer compound_statement_factory(const statement_list &arg);

    // See base class for documentation.
    expression::pointer scan_expression_factory(
        const expression::pointer &maxdisp, int mask,
        const expression::pointer &value, const expression::pointer &start);

    // See base class for documentation.
    void scope_end(void);

    // See base class for documentation.
    void scope_end_external(void);

    // See base class for documentation.
    void program_end(void);

    // See base class for documentation.
    void fail_hook(void);

    // See base class for documentation.
    void unit_interface_text(const rcstring &text);

    // See base class for documentation.
    expression::pointer lint2lint_expression_factory(const type::pointer &to,
        const expression::pointer &from);

    // See base class for documentation.
    expression::pointer lint_from_integer_expression_factory(
        const expression::pointer &operand);

    // See base class for documentation.
    expression::pointer lint_plus_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer lint_minus_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer lint_times_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    expression::pointer lint_over_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer ternary_expression_factory(
        const expression::pointer &cond, const expression::pointer &then_branch,
        const expression::pointer &else_branch);

    // See base class for documentation.
    expression::pointer integer_from_lint_expression_factory(
        const expression::pointer &operand);

    // See base class for documentation.
    expression::pointer lint_eq_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer lint_ne_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    symbol::pointer enumerated_symbol_factory(const variable_name &name,
        const type_enumerated::pointer &parent, int lex_level, int value);

private:
    /**
      * The output_file_name is used to remember the name of the output
      * file.  Actually opening the is deferred until we know, from the
      * ($b*) comment, whether the file is to be little-endian (the
      * default) or big-endian.
      *
      * See the #create_code_file method for where this is done.
      */
    rcstring output_file_name;

    /**
      * The cfp instance variable is used to remember the code file to
      * receive the compiled p-code.
      *
      * See the #create_code_file method for where this is done.
      */
    codefile::pointer cfp;

    /**
      * The number_of_errors instance variable is used to remember the
      * number of code geneation errors.  If there are any errors, the
      * compile will exit with a failure exit status.
      */
    int number_of_errors;

    /**
      * The segstack instance variable is used to reemmber the push-down
      * stack of segments currently being built.
      */
    segment_builder_stack segstack;

    /**
      * The bpt_locn instance variable is used to remember the current
      * breakpoint location.
      */
    location bpt_locn;

    /**
      * The segment_flags instance variable is used to remember what
      * segments must be loaded, because the compiler will generate code
      * which calls procedures within them.
      */
    unsigned char segment_flags[8];

    /**
      * The create_code_file method is used to open a code file and
      * assign it to the #cfp instance variable.
      *
      * @param bs
      *     The byte sex of the codefile.
      */
    void create_code_file(byte_sex_t bs);

    /**
      * The create_code_file_default_byte_sex method is used to create
      * the code file, using little-endian byte sex.
      */
    void create_code_file_default_byte_sex(void);

    /**
      * The code_listing instance variable is used to remember the
      * destination for the code listing, or NULL for no listing.
      */
    code_source_map::pointer listing;

    // See base class for documentation.
    int uses_unitseg(const rcstring &segname, int segnum);

    /**
      * The default constructor.  Do not use.
      */
    translator_compile();

    /**
      * The copy constructor.  Do not use.
      */
    translator_compile(const translator_compile &);

    /**
      * The assignment operator.  Do not use.
      */
    translator_compile &operator=(const translator_compile &);
};

#endif // UCSDPSYS_COMPILE_TRANSLATOR_COMPILE_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_ASSEMBLE_ASSEMBLER_H
#define UCSDPSYS_ASSEMBLE_ASSEMBLER_H

#include <lib/ac/vector>

#include <lib/codefile.h>
#include <lib/expression.h>
#include <lib/expression/dispatcher/binary.h>
#include <lib/output.h>
#include <lib/rcstring.h>
#include <lib/segment.h>
#include <lib/segment/builder.h>
#include <lib/symbol.h>
#include <lib/variable/name.h>

#include <ucsdpsys_assemble/listing.h>
#include <ucsdpsys_assemble/patch.h>
#include <ucsdpsys_assemble/personality.h>

class rcstring_list; // forward

/**
  * The assembler class is used to represent an abstract assembler code
  * destination.
  */
class assembler
{
public:
    /**
      * The destructor.
      */
    virtual ~assembler();

    /**
      * The constructor.
      *
      * @param code_file
      *     The coee file to be created.
      * @param listing_file
      *     The output to accept the assembler listing.
      */
    assembler(const rcstring &code_file, const output::pointer &listing_file);

    /**
      * The parse method is used to read and parse an assembler source
      * file, and write the result to the codefile.
      *
      * @param source_file
      *     The name of the source file.
      */
    void parse(const rcstring &source_file);

    /**
      * The integer_expression_factory method is used to create a new
      * expression_integer instance.
      *
      * @param locn
      *     where the integer occurred in the source.
      * @param value
      *     The integer value of the expression.
      * @returns
      *     a new expression instance
      */
    expression::pointer integer_expression_factory(const location &locn,
        long value);

    /**
      * The string_expression_factory method is used to create a new
      * expression_string instance.
      *
      * @param locn
      *     where the integer occurred in the source.
      * @param value
      *     The string value of the expression.
      * @returns
      *     a new expression instance
      */
    expression::pointer string_expression_factory(const location &locn,
        const rcstring &value);

    /**
      * The name_expression_factory method is used to create a new
      * expression_name instance.
      *
      * @param name
      *     The name of the symbol, and the location of the name within
      *     the source code.
      * @returns
      *     a new expression instance
      */
    expression::pointer name_expression_factory(const variable_name &name);

    /**
      * The negate_expression_factory method is used to create a new
      * expression_negate instance.
      *
      * @param arg
      *     The expression to be negated.
      * @returns
      *     a new expression instance
      */
    expression::pointer negate_expression_factory(
        const expression::pointer &arg);

    /**
      * The unary_plus_expression_factory method is used to create a
      * new "positive expressions" instance (not to be confused with
      * absolute value).  Basically, a check that the expression is an
      * integer.
      *
      * @param arg
      *     The expression to be processed.
      * @returns
      *     a new expression instance
      */
    expression::pointer unary_plus_expression_factory(
        const expression::pointer &arg);

    /**
      * The bitwise_not_expression_factory method is used to create a new
      * expression_bitwise_not instance.
      *
      * @param arg
      *     The expression to be negated.
      * @returns
      *     a new expression instance
      */
    expression::pointer bitwise_not_expression_factory(
        const expression::pointer &arg);

    /**
      * The dollar_expression_factory method is used to create an
      * expression who's value is the current address (usually a segment
      * relative address).
      *
      * @param locn
      *     The location of the label in the source code.
      * @param offset
      *     The offset from the curent code location in the segment,
      *     usually zero.
      */
    expression::pointer dollar_expression_factory(const location &locn,
        int offset = 0);

    /**
      * The enter_ic_expression_factory method is used to create an
      * expression who's value is the segment relative address of the
      * beginnign of the current procedure.
      *
      * @param locn
      *     The location of the label in the source code.
      */
    expression::pointer enter_ic_expression_factory(const location &locn);

    /**
      * The define_label method is used to define a label at the current
      * code position.
      *
      * @param name
      *     The name of label, includes the location in the source code
      *     that the name occurred at.
      * @param dot_def
      *     Treat this defintion as if it was the subject of a .def pseudo-op.
      */
    void define_label(const variable_name &name, bool dot_def = false);

    /**
      * The define_symbol method is called by the grammar when a label
      * is defined or an .equ pseudo-op is seen.
      *
      * If the name is not temporary (does not start with '$') it will
      * cause all of the temprary labels to be dropped, after opcodes
      * refering to them have been patched, and errors issued for
      * undefined temporary names.
      *
      * @param name
      *     The name of the symbol, usually a label.
      * @param value
      *     The value of the symbol, usually an integer (an address).
      * @param dot_def
      *     Treat this defintion as if it was the subject of a .def pseudo-op.
      */
    void define_symbol(const variable_name &name,
        const expression::pointer &value, bool dot_def = false);

    /**
      * The define_macro method is called by the grammar
      * when a macro is defined (when a .macro pseudo-op is seen).
      *
      * @param name
      *     The name of the macro.
      * @param contents
      *     The text of the macro, including all newlines, and even the
      *     comments.
      */
    void define_macro(const variable_name &name, const rcstring_list &contents);

    /**
      * The lookup_opcode method is used (indirectly) by the lexer to
      * figure out if a symbol it is looking at is the name of an opcode
      * or not.
      *
      * @param name
      *     The name of the opcode to lookup
      * @returns
      *     A pointer to the opcode on success, or NULL if the name is
      *     not that of an opcode.
      */
    opcode::pointer lookup_opcode(const rcstring &name);

    /**
      * The lookup_special method is used (indirectly) by the lexer to
      * figure out if a symbol it is looking at is the name of a special
      * symbol or not.
      *
      * @param name
      *     The name of the special symbol to lookup
      * @returns
      *     A pointer to the special symbols's value on success, or NULL
      *     if the name is not that of a special symbol.
      */
    expression::pointer lookup_special(const rcstring &name);

    /**
      * The emit_byte method is used to emit another byte to the code stream.
      *
      * @param value
      *     The value to place into the code stream.
      */
    void emit_byte(unsigned char value);

    /**
      * The emit_byte method is used to emit another byte to the code
      * stream.  It may need to be deferred until later.
      *
      * @param value
      *     The value to place into the code stream.
      */
    void emit_byte(const expression::pointer &value);

    /**
      * The emit_word method is used to emit a two-byte word to the code
      * stream.
      *
      * @param value
      *     The value to place into the code stream.
      */
    void emit_word(unsigned short value);

    /**
      * The emit_word method is used to emit a two-byte word to the code
      * stream.  It may need to be deferred until later.
      *
      * @param value
      *     The value to place into the code stream.
      */
    void emit_word(const expression::pointer &value);

    /**
      * The emit_big method is used to emit a one-or-two-byte word to
      * the code stream.
      *
      * @param value
      *     The value to place into the code stream.
      */
    void emit_big(unsigned short value);

    /**
      * The emit_big method is used to emit a one-or-two-byte word to
      * the code stream.  It may need to be deferred until later.
      *
      * @param value
      *     The value to place into the code stream.
      */
    void emit_big(const expression::pointer &value);

    void emit_even_alignment(void);

    /**
      * The emit_one_byte_nop method is used to emit a filler byte
      * equal to the processor's one byte NOP opcode.  This is used as
      * filler for unknown byte values, before the are patched by symbol
      * resolution.
      */
    void emit_one_byte_nop(void);

    /**
      * The emit_globref method is used to add a GLOBREF link into
      * record into the segment's link info.
      *
      * @param name
      *     The name of the linker symbol
      * @param nwords
      *     The word width of the symbol.
      */
    void emit_globref(const rcstring &name, int nwords = 1);

    /**
      * The write_code_file method is used to write out the final
      * contents of the codefile being assembled.
      */
    void write_code_file(void);

    /**
      * The dot_architecture method is used by the ".arch" pseudo-op to
      * change the machine architecture the following assembler code is for.
      *
      * @param name
      *     The architecture namne, e.g. "p-code-le" or "6502", etc.
      */
    void dot_architecture(const rcstring &name);

    /**
      * The dot_host method is used by the ".host" pseudo-op to change
      * the machine architecture the following assembler code is for.
      *
      * @param name
      *     The host system name, e.g. "apple", etc.
      */
    void dot_host(const rcstring &name);

    /**
      * The proc_begin method is called by the grammar to indicated that
      * the named procedure has started.
      *
      * @param name
      *     The name of the procedure about to start.
      * @param nparam
      *     The number of words of parameters that are expected
      * @param is_func
      *     Whether this is a function (true) or a procedure (false).
      */
    void proc_begin(const variable_name &name,
        const expression::pointer &nparam = expression::pointer(),
        bool is_func = false);

    /**
      * The proc_end method is called by the grammar to indicate that
      * a procedure has completed (either an new procedure is about
      * to start, or .end is about to be processed, or end of file is
      * imminent).
      */
    void proc_end(void);

    /**
      * The dot_end method is called by the grammar to indicate that end
      * of input has been reached.
      */
    void dot_end(void);

    /**
      * The dot_if method is used to handle the .IF pseudo-op .
      *
      * @param arg
      *     The allegedly boolean expression controlling the conditional
      *     assembler of the following text.
      */
    void dot_if(const expression::pointer &arg);

    /**
      * The dot_title method is used to handle the .TITLE pseudo-op.
      * This sets the first title line on the listing.
      *
      * @param text
      *     The new text to use as the first line of the page title in
      *     the assembly listing.
      */
    void dot_title(const rcstring &text);

    /**
      * The dot_sbttl method is used to handle the .SBTTL pseudo-op.
      * This sets the second title line on the listing.
      *
      * @param text
      *     The new text to use as the first line of the page title in
      *     the assembly listing.
      */
    void dot_sbttl(const rcstring &text);

    /**
      * The get_address method is used to obtain the current address in
      * the code generation.
      */
    unsigned get_address(void) const;

    /**
      * The dereference_expression_factory method is called by the
      * grammar to build a dereference (unary star) expression tree
      * node.
      *
      * @param operand
      *     The address to be dereferenced.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer dereference_expression_factory(
        const expression::pointer &operand);

    /**
      * The addition_expression_factory method is called by the grammar
      * to build an addition expression tree node.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer addition_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The subtraction_expression_factory method is called by the
      * grammar to build an subtraction expression tree node.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer subtraction_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The multiplication_expression_factory method is called by the
      * grammar to build an multiplication expression tree node.
      *
      * @param lhs
      *     The left hand side of the multiplication.
      * @param rhs
      *     The right hand side of the multiplication.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer multiplication_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The division_expression_factory method is called by the grammar
      * to build an division expression tree node.
      *
      * @param lhs
      *     The left hand side of the division.
      * @param rhs
      *     The right hand side of the division.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer division_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The modulo_expression_factory method is called by the grammar
      * to build an modulo expression tree node.
      *
      * @param lhs
      *     The left hand side of the modulo.
      * @param rhs
      *     The right hand side of the modulo.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer modulo_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The bitwise_and_expression_factory method is called by the grammar
      * to build a bit-wise AND expression tree node.
      *
      * @param lhs
      *     The left hand side of the expression.
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer bitwise_and_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The bitwise_or_expression_factory method is called by the grammar
      * to build a bit-wise OR expression tree node.
      *
      * @param lhs
      *     The left hand side of the expression.
      * @param rhs
      *     The right hand side of the expression.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer bitwise_or_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The xor_expression_factory method is called by the grammar
      * to build a bit-wise exclusive-or expression tree node.
      *
      * @param lhs
      *     The left hand side of the xor.
      * @param rhs
      *     The right hand side of the xor.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer xor_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The eq_expression_factory method is called by the grammar
      * to build an equal-to expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer eq_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The ne_expression_factory method is called by the grammar
      * to build a not-equal-to expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer ne_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The le_expression_factory method is called by the grammar
      * to build a less-than-or-equal-to expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer le_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The lt_expression_factory method is called by the grammar
      * to build a less-than expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer lt_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The ge_expression_factory method is called by the grammar
      * to build a greater-than-or-equal-to expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer ge_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The gt_expression_factory method is called by the grammar
      * to build a greater-than expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer gt_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    /**
      * The set_host method is used to set the host from
      * the command line.
      *
      * @param name
      *     The host type name (e.g. "apple")
      */
    void set_host(const char *name);

    /**
      * The set_architecture method is used to set the architecture from
      * the command line.
      *
      * @param name
      *     The machine type name (e.g. "6502")
      */
    void set_architecture(const char *name);

    /**
      * The export_def method is used to export a symbol into the linkage
      * information attached to the segment.
      *
      * @param name
      *     The name of the symbol to be exported.
      * @param value
      *     The value of the symbol to be exported.
      */
    void export_def(const rcstring &name, const expression::pointer &value);

    /**
      * The export_def method is used to export a symbol into the linkage
      * information attached to the segment.
      *
      * @param name
      *     The name of the symbol to be exported.
      * @param value
      *     The value of the symbol to be exported.
      */
    void export_def(const rcstring &name, unsigned short value);

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 1 (address relative) relocation word data.
      * Passed to segment builder via #sbp.
      *
      * @param address
      *     The address of the word within the segment (segment relative
      *     NOT procedure relative).
      */
    void reloc_type1_lcrel(unsigned address);

    /**
      * The reloc_type1_lcrel method is used to add the current address to
      * the Type 1 (pc relative) relocation word data.
      * Passed to segment builder via #sbp.
      */
    void reloc_type1_lcrel(void);

    /**
      * The reloc_type1_lcrel method is used to add another address to
      * the Type 2 (.ref, .const) relocation word data.
      * Passed to segment builder via #sbp.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    void reloc_type2_ref(unsigned address);

    /**
      * The reloc_type3_pubpriv method is used to add another address to
      * the Type 3 (.public, .private) relocation word data.
      * Passed to segment builder via #sbp.
      *
      * @param address
      *     The addres sof the word within the segment.
      */
    void reloc_type3_pubpriv(unsigned address);

    void listing_eoln();

    static output::pointer get_source_column(void);

private:
    /**
      * The integer_plus_integer_expression_factory method is called by
      * the #addition_expression_factory to build an integer addition
      * expression tree node.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_plus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The segrel_plus_integer_expression_factory method is called by
      * the #addition_expression_factory to build a segment relative
      * addition expression tree node.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer segrel_plus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_plus_segrel_expression_factory method is called by
      * the #addition_expression_factory to build a segment relative
      * addition expression tree node.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_plus_segrel_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_plus_linksym_expression_factory method is called by
      * the #addition_expression_factory to build a segment relative
      * addition expression tree node.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_plus_linksym_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The linksym_plus_integer_expression_factory method is called by
      * the #addition_expression_factory to build a segment relative
      * addition expression tree node.
      *
      * @param lhs
      *     The left hand side of the addition.
      * @param rhs
      *     The right hand side of the addition.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer linksym_plus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The integer_minus_integer_expression_factory method is called
      * by the #subtraction_expression_factory to build an integer
      * subtraction expression tree node.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_minus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The segrel_minus_integer_expression_factory method is called
      * by the #subtraction_expression_factory to build an integer
      * subtraction expression tree node.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer segrel_minus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The segrel_minus_segrel_expression_factory method is called
      * by the #subtraction_expression_factory to build an segrel
      * subtraction expression tree node.
      *
      * @param lhs
      *     The left hand side of the subtraction.
      * @param rhs
      *     The right hand side of the subtraction.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer segrel_minus_segrel_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The code_file_name is used to remember the name of the code file
      * to produce.
      */
    rcstring code_file_name;

    /**
      * The lout instance variable is used to remember where to send the
      * assembler listing.
      */
    listing lout;

    /**
      * The mpseudo instance variable is used to remember
      * the set of pseudo-ops the assembler supports.
      */
    personality::pointer mpseudo;

    /**
      * The cfp instance variable is used to remember
      * the locfation of the codefile we are building.
      */
    codefile::pointer cfp;

    /**
      * The seg instance variable is used to remember
      * the current segment we are building.
      */
    segment::pointer seg;

    /**
      * The sbp instance variable is used to remember the segment
      * builder for the current segment.  All code is accumulated here,
      * the transfered to the segment procedure by procedure.
      */
    segment_builder::pointer sbp;

    /**
      * The machine instance variable is used to remember the opcodes
      * supported by the machine language we are assembling.  This can
      * change between procedures, so that you can assemble more than
      * one kind of machine code in the one pass.
      */
    personality::pointer machine;

    /**
      * The procnum instance variable is used to remember the procedure
      * number of the procedure currently being assembled.
      */
    int procnum;

    unsigned enter_ic;

    bool exit_ic_set;

    void delayed_open(const rcstring &segname);

    typedef symtab<symbol::pointer> symbols_t;

    /**
      * The symbols instance variable is used to remember the symbols
      * (usually labels) that have been defined to date.  It is
      * populated by the #define_symbol method.
      */
    symbols_t symbols;

    /**
      * The temporary_symbol_names instance variable is used to track
      * the presently defined or referenced temporary symbols.  (We
      * could extract the from #symbols, but this will generally be
      * faster.)
      */
    rcstring_list temporary_symbol_names;

    /**
      * The local_symbol_names instance variable is used to track the
      * presently defined or referenced local symbols.  This is used to
      * distinguish symbols defined before the first .proc or .func from
      * symbols defined after (to be dropped at the end of the current
      * procedure).
      */
    rcstring_list local_symbol_names;

    typedef std::vector<patch::pointer> patches_t;

    /**
      * The patches instance variable is used to remember the locations
      * in the code that need to be repaired once symbols have been
      * defined.
      */
    patches_t patches;

    /**
      * The op_addition instance variable is used to remember all of the
      * possible legal addition operand combinations.
      */
    expression_dispatcher_binary op_addition;

    /**
      * The op_subtraction instance variable is used to remember all of
      * the possible legal subtraction operand combinations.
      */
    expression_dispatcher_binary op_subtraction;

    /**
      * The op_eq instance variable is used to remember all of the
      * possible legal eq operand combinations.
      */
    expression_dispatcher_binary op_eq;

    /**
      * The integer_eq_integer_expression_factory method is called
      * by the #eq_expression_factory method to build a integer
      * eq expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_eq_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_eq_string_expression_factory method is called
      * by the #eq_expression_factory method to build a string
      * eq expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer string_eq_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The boolean_eq_boolean_expression_factory method is called
      * by the #eq_expression_factory method to build a boolean
      * eq expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer boolean_eq_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The op_ne instance variable is used to remember all of the
      * possible legal ne operand combinations.
      */
    expression_dispatcher_binary op_ne;

    /**
      * The integer_ne_integer_expression_factory method is called
      * by the #ne_expression_factory method to build a integer
      * eq expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_ne_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_ne_string_expression_factory method is called
      * by the #ne_expression_factory method to build a string
      * eq expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer string_ne_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The boolean_ne_boolean_expression_factory method is called
      * by the #ne_expression_factory method to build a boolean
      * eq expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer boolean_ne_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The op_lt instance variable is used to remember all of the
      * possible legal lt operand combinations.
      */
    expression_dispatcher_binary op_lt;

    /**
      * The integer_lt_integer_expression_factory method is called
      * by the #lt_expression_factory method to build a integer
      * lt expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_lt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_lt_string_expression_factory method is called
      * by the #lt_expression_factory method to build a string
      * lt expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer string_lt_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The boolean_lt_boolean_expression_factory method is called
      * by the #lt_expression_factory method to build a boolean
      * lt expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer boolean_lt_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The op_le instance variable is used to remember all of the
      * possible legal le operand combinations.
      */
    expression_dispatcher_binary op_le;

    /**
      * The integer_le_integer_expression_factory method is called
      * by the #le_expression_factory method to build a integer
      * le expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_le_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_le_string_expression_factory method is called
      * by the #le_expression_factory method to build a string
      * le expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer string_le_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The boolean_le_boolean_expression_factory method is called
      * by the #le_expression_factory method to build a boolean
      * le expression tree node.
      *
      * @param lhs
      *     The left hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer boolean_le_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The op_gt instance variabge is used to remember all of the
      * possibge gegal gt operand combinations.
      */
    expression_dispatcher_binary op_gt;

    /**
      * The integer_gt_integer_expression_factory method is calged
      * by the #gt_expression_factory method to build a integer
      * gt expression tree node.
      *
      * @param lhs
      *     The geft hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer integer_gt_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_gt_string_expression_factory method is calged
      * by the #gt_expression_factory method to build a string
      * gt expression tree node.
      *
      * @param lhs
      *     The geft hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer string_gt_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The boolean_gt_boolean_expression_factory method is calged
      * by the #gt_expression_factory method to build a boolean
      * gt expression tree node.
      *
      * @param lhs
      *     The geft hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer boolean_gt_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The op_ge instance variabge is used to remember all of the
      * possibge gegal ge operand combinations.
      */
    expression_dispatcher_binary op_ge;

    /**
      * The integer_ge_integer_expression_factory method is calged
      * by the #ge_expression_factory method to build a integer
      * eq expression tree node.
      *
      * @param lhs
      *     The geft hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     gew expression tree node in dynamic memory
      */
    expression::pointer integer_ge_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The string_ge_string_expression_factory method is calged
      * by the #ge_expression_factory method to build a string
      * eq expression tree node.
      *
      * @param lhs
      *     The geft hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     gew expression tree node in dynamic memory
      */
    expression::pointer string_ge_string_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    /**
      * The boolean_ge_boolean_expression_factory method is calged
      * by the #ge_expression_factory method to build a boolean
      * eq expression tree node.
      *
      * @param lhs
      *     The geft hand side of the comparison.
      * @param rhs
      *     The right hand side of the comparison.
      * @returns
      *     new expression tree node in dynamic memory
      */
    expression::pointer boolean_ge_boolean_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    output::pointer get_source_column_inner(void);

    /**
      * The default constructor.  Do not use.
      */
    assembler();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    assembler(const assembler &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    assembler &operator=(const assembler &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_ASSEMBLER_H
// vim: set ts=8 sw=4 et :

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

#include <lib/config.h>
#include <lib/ac/assert.h>
#include <lib/ac/string.h>
#include <libexplain/output.h>

#include <lib/codefile/file.h>
#include <lib/debug.h>
#include <lib/expression/error.h>
#include <lib/pascal/lex.h>
#include <lib/pcode.h>
#include <lib/segment/builder/code.h>
#include <lib/statement/list.h>
#include <lib/symbol/expression/constant.h>
#include <lib/symbol/variable.h>
#include <lib/type/array.h>
#include <lib/type/file.h>
#include <lib/type/long_integer/sized.h>
#include <lib/type/nothing.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/reference.h>
#include <lib/type/set/sized.h>
#include <lib/type/set/unsized.h>
#include <lib/type/string.h>
#include <lib/type/subrange.h>

#include <ucsdpsys_compile/expression/addition/integer.h>
#include <ucsdpsys_compile/expression/addition/long_integer.h>
#include <ucsdpsys_compile/expression/addition/real.h>
#include <ucsdpsys_compile/expression/address/global.h>
#include <ucsdpsys_compile/expression/address/intermediate.h>
#include <ucsdpsys_compile/expression/address/local.h>
#include <ucsdpsys_compile/expression/bitwise_and/compile.h>
#include <ucsdpsys_compile/expression/bitwise_not/compile.h>
#include <ucsdpsys_compile/expression/bitwise_or/compile.h>
#include <ucsdpsys_compile/expression/boolean/compile.h>
#include <ucsdpsys_compile/expression/cast.h>
#include <ucsdpsys_compile/expression/char/compile.h>
#include <ucsdpsys_compile/expression/check_range.h>
#include <ucsdpsys_compile/expression/eq/boolean.h>
#include <ucsdpsys_compile/expression/eq/byte_array.h>
#include <ucsdpsys_compile/expression/eq/integer.h>
#include <ucsdpsys_compile/expression/eq/long_integer.h>
#include <ucsdpsys_compile/expression/eq/multi_word.h>
#include <ucsdpsys_compile/expression/eq/real.h>
#include <ucsdpsys_compile/expression/eq/set.h>
#include <ucsdpsys_compile/expression/eq/string.h>
#include <ucsdpsys_compile/expression/function.h>
#include <ucsdpsys_compile/expression/ge/boolean.h>
#include <ucsdpsys_compile/expression/ge/byte_array.h>
#include <ucsdpsys_compile/expression/ge/integer.h>
#include <ucsdpsys_compile/expression/ge/long_integer.h>
#include <ucsdpsys_compile/expression/ge/real.h>
#include <ucsdpsys_compile/expression/ge/set.h>
#include <ucsdpsys_compile/expression/ge/string.h>
#include <ucsdpsys_compile/expression/gt/boolean.h>
#include <ucsdpsys_compile/expression/gt/byte_array.h>
#include <ucsdpsys_compile/expression/gt/integer.h>
#include <ucsdpsys_compile/expression/gt/long_integer.h>
#include <ucsdpsys_compile/expression/gt/real.h>
#include <ucsdpsys_compile/expression/gt/set.h>
#include <ucsdpsys_compile/expression/gt/string.h>
#include <ucsdpsys_compile/expression/in/compile.h>
#include <ucsdpsys_compile/expression/integer/compile.h>
#include <ucsdpsys_compile/expression/integer_division/compile.h>
#include <ucsdpsys_compile/expression/integer_division/long_integer.h>
#include <ucsdpsys_compile/expression/le/boolean.h>
#include <ucsdpsys_compile/expression/le/byte_array.h>
#include <ucsdpsys_compile/expression/le/integer.h>
#include <ucsdpsys_compile/expression/le/long_integer.h>
#include <ucsdpsys_compile/expression/le/real.h>
#include <ucsdpsys_compile/expression/le/set.h>
#include <ucsdpsys_compile/expression/le/string.h>
#include <ucsdpsys_compile/expression/lint2lint/compile.h>
#include <ucsdpsys_compile/expression/lint_from_integer/compile.h>
#include <ucsdpsys_compile/expression/load/indirect.h>
#include <ucsdpsys_compile/expression/load/string.h>
#include <ucsdpsys_compile/expression/logical_and/compile.h>
#include <ucsdpsys_compile/expression/logical_not/compile.h>
#include <ucsdpsys_compile/expression/logical_or/compile.h>
#include <ucsdpsys_compile/expression/lt/boolean.h>
#include <ucsdpsys_compile/expression/lt/byte_array.h>
#include <ucsdpsys_compile/expression/lt/integer.h>
#include <ucsdpsys_compile/expression/lt/long_integer.h>
#include <ucsdpsys_compile/expression/lt/real.h>
#include <ucsdpsys_compile/expression/lt/set.h>
#include <ucsdpsys_compile/expression/lt/string.h>
#include <ucsdpsys_compile/expression/long_integer/compile.h>
#include <ucsdpsys_compile/expression/modulo/compile.h>
#include <ucsdpsys_compile/expression/multiplication/integer.h>
#include <ucsdpsys_compile/expression/multiplication/long_integer.h>
#include <ucsdpsys_compile/expression/multiplication/real.h>
#include <ucsdpsys_compile/expression/ne/boolean.h>
#include <ucsdpsys_compile/expression/ne/byte_array.h>
#include <ucsdpsys_compile/expression/ne/integer.h>
#include <ucsdpsys_compile/expression/ne/long_integer.h>
#include <ucsdpsys_compile/expression/ne/multi_word.h>
#include <ucsdpsys_compile/expression/ne/real.h>
#include <ucsdpsys_compile/expression/ne/set.h>
#include <ucsdpsys_compile/expression/ne/string.h>
#include <ucsdpsys_compile/expression/negate/integer.h>
#include <ucsdpsys_compile/expression/negate/long_integer.h>
#include <ucsdpsys_compile/expression/negate/real.h>
#include <ucsdpsys_compile/expression/nil/compile.h>
#include <ucsdpsys_compile/expression/paoc_from_string/compile.h>
#include <ucsdpsys_compile/expression/real/compile.h>
#include <ucsdpsys_compile/expression/real_division/compile.h>
#include <ucsdpsys_compile/expression/real_from_integer/compile.h>
#include <ucsdpsys_compile/expression/scan/compile.h>
#include <ucsdpsys_compile/expression/set/compile.h>
#include <ucsdpsys_compile/expression/set2set/compile.h>
#include <ucsdpsys_compile/expression/set_difference.h>
#include <ucsdpsys_compile/expression/set_intersection.h>
#include <ucsdpsys_compile/expression/set_union.h>
#include <ucsdpsys_compile/expression/sgs/compile.h>
#include <ucsdpsys_compile/expression/srs/compile.h>
#include <ucsdpsys_compile/expression/string/compile.h>
#include <ucsdpsys_compile/expression/subtraction/integer.h>
#include <ucsdpsys_compile/expression/subtraction/long_integer.h>
#include <ucsdpsys_compile/expression/subtraction/real.h>
#include <ucsdpsys_compile/expression/ternary/compile.h>
#include <ucsdpsys_compile/expression/trunc/long_integer.h>
#include <ucsdpsys_compile/expression/writeln.h>
#include <ucsdpsys_compile/label/compile.h>
#include <ucsdpsys_compile/scope/builtin/compile.h>
#include <ucsdpsys_compile/scope/function/compile.h>
#include <ucsdpsys_compile/scope/interface/compile.h>
#include <ucsdpsys_compile/scope/interface/unitseg.h>
#include <ucsdpsys_compile/scope/program/compile.h>
#include <ucsdpsys_compile/scope/record/compile.h>
#include <ucsdpsys_compile/scope/temporary/compound/compile.h>
#include <ucsdpsys_compile/scope/temporary/with/compile.h>
#include <ucsdpsys_compile/scope/unit/compile.h>
#include <ucsdpsys_compile/statement/case/compile.h>
#include <ucsdpsys_compile/statement/expression/compile.h>
#include <ucsdpsys_compile/statement/for/compile.h>
#include <ucsdpsys_compile/statement/goto/compile.h>
#include <ucsdpsys_compile/statement/if/compile.h>
#include <ucsdpsys_compile/statement/infinite_loop/compile.h>
#include <ucsdpsys_compile/statement/label/compile.h>
#include <ucsdpsys_compile/statement/until/compile.h>
#include <ucsdpsys_compile/statement/while/compile.h>
#include <ucsdpsys_compile/symbol/enumerated/compile.h>
#include <ucsdpsys_compile/symbol/function/abs.h>
#include <ucsdpsys_compile/symbol/function/atan.h>
#include <ucsdpsys_compile/symbol/function/blockread.h>
#include <ucsdpsys_compile/symbol/function/blockwrite.h>
#include <ucsdpsys_compile/symbol/function/chr.h>
#include <ucsdpsys_compile/symbol/function/close.h>
#include <ucsdpsys_compile/symbol/function/concat.h>
#include <ucsdpsys_compile/symbol/function/copy.h>
#include <ucsdpsys_compile/symbol/function/cos.h>
#include <ucsdpsys_compile/symbol/function/delete.h>
#include <ucsdpsys_compile/symbol/function/eof.h>
#include <ucsdpsys_compile/symbol/function/eoln.h>
#include <ucsdpsys_compile/symbol/function/exit.h>
#include <ucsdpsys_compile/symbol/function/exp.h>
#include <ucsdpsys_compile/symbol/function/gotoxy.h>
#include <ucsdpsys_compile/symbol/function/fillchar.h>
#include <ucsdpsys_compile/symbol/function/get.h>
#include <ucsdpsys_compile/symbol/function/halt.h>
#include <ucsdpsys_compile/symbol/function/idsearch.h>
#include <ucsdpsys_compile/symbol/function/insert.h>
#include <ucsdpsys_compile/symbol/function/ioresult.h>
#include <ucsdpsys_compile/symbol/function/length.h>
#include <ucsdpsys_compile/symbol/function/ln.h>
#include <ucsdpsys_compile/symbol/function/log.h>
#include <ucsdpsys_compile/symbol/function/mark.h>
#include <ucsdpsys_compile/symbol/function/memavail.h>
#include <ucsdpsys_compile/symbol/function/moveleft.h>
#include <ucsdpsys_compile/symbol/function/moveright.h>
#include <ucsdpsys_compile/symbol/function/new.h>
#include <ucsdpsys_compile/symbol/function/odd.h>
#include <ucsdpsys_compile/symbol/function/opennew.h>
#include <ucsdpsys_compile/symbol/function/openold.h>
#include <ucsdpsys_compile/symbol/function/ord.h>
#include <ucsdpsys_compile/symbol/function/page.h>
#include <ucsdpsys_compile/symbol/function/pos.h>
#include <ucsdpsys_compile/symbol/function/pred.h>
#include <ucsdpsys_compile/symbol/function/put.h>
#include <ucsdpsys_compile/symbol/function/pwroften.h>
#include <ucsdpsys_compile/symbol/function/read.h>
#include <ucsdpsys_compile/symbol/function/release.h>
#include <ucsdpsys_compile/symbol/function/reset.h>
#include <ucsdpsys_compile/symbol/function/rewrite.h>
#include <ucsdpsys_compile/symbol/function/round.h>
#include <ucsdpsys_compile/symbol/function/seek.h>
#include <ucsdpsys_compile/symbol/function/sin.h>
#include <ucsdpsys_compile/symbol/function/sqr.h>
#include <ucsdpsys_compile/symbol/function/sqrt.h>
#include <ucsdpsys_compile/symbol/function/str.h>
#include <ucsdpsys_compile/symbol/function/succ.h>
#include <ucsdpsys_compile/symbol/function/time.h>
#include <ucsdpsys_compile/symbol/function/treesearch.h>
#include <ucsdpsys_compile/symbol/function/trunc.h>
#include <ucsdpsys_compile/symbol/function/unitbusy.h>
#include <ucsdpsys_compile/symbol/function/unitclear.h>
#include <ucsdpsys_compile/symbol/function/unitread.h>
#include <ucsdpsys_compile/symbol/function/unitstatus.h>
#include <ucsdpsys_compile/symbol/function/unitwait.h>
#include <ucsdpsys_compile/symbol/function/unitwrite.h>
#include <ucsdpsys_compile/symbol/function/user_defined.h>
#include <ucsdpsys_compile/translator/compile.h>


translator_compile::~translator_compile()
{
    if (number_of_errors > 0)
        explain_output_error_and_die("Found %d fatal errors", number_of_errors);
}


translator_compile::translator_compile(const rcstring &ofn) :
    output_file_name(ofn),
    number_of_errors(0)
{
    memset(segment_flags, 0, sizeof(segment_flags));
}


scope::pointer
translator_compile::program_scope_factory(const rcstring &name)
{
    return scope_program_compile::create(name, this);
}


scope::pointer
translator_compile::builtin_scope_factory(void)
{
    return scope_builtin_compile::create(this);
}


scope::pointer
translator_compile::unit_scope_factory(const rcstring &name, int code_seg,
    int data_seg, int llht, segkind_t segkind)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (code_seg < 0)
    {
        create_code_file_default_byte_sex();
        DEBUG(2, "cfp = %p", cfp.get());
        code_seg = cfp->next_free_segment_number(false, true);
    }
    return
        scope_unit_compile::create
        (
            name,
            code_seg,
            data_seg,
            llht,
            segkind,
            this
        );
}


void
translator_compile::unit_interface_text(const rcstring &text)
{
    segstack.unit_interface_text(text);
}


scope::pointer
translator_compile::function_scope_factory(const rcstring &name, bool is_func,
    bool new_seg)
{
    return scope_function_compile::create(name, is_func, new_seg, this);
}


statement::pointer
translator_compile::expression_statement_factory(
    const expression::pointer &arg)
{
    return statement_expression_compile::create(arg, this);
}


expression::pointer
translator_compile::integer_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_integer_compile::create(locn, arg, this);
}


expression::pointer
translator_compile::lint_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_long_integer_compile::create(locn, arg, this);
}


expression::pointer
translator_compile::integer_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_integer::create(arg, this);
}


expression::pointer
translator_compile::real_negate_expression_factory(
    const expression::pointer &arg)
{
    return expression_negate_real::create(arg, this);
}


expression::pointer
translator_compile::lint_negate_expression_factory(
    const expression::pointer &arg)
{
    type::pointer tp = type_long_integer_sized::create();
    expression::pointer ep = lint2lint_expression_factory(tp, arg);
    return expression_negate_long_integer::create(ep, this);
}


expression::pointer
translator_compile::real_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_real_compile::create(locn, arg, this);
}


expression::pointer
translator_compile::string_expression_factory(const location &locn,
    const rcstring &arg)
{
    return expression_string_compile::create(locn, arg, this);
}


void
translator_compile::error(const location &loc, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    rcstring s = rcstring::vprintf(fmt, ap);
    va_end(ap);
    explain_output_error
    (
        "%s: %d: %s",
        loc.get_file_name().c_str(),
        loc.get_line_number(),
        s.c_str()
    );
    if (fmt[0] != '.')
        ++number_of_errors;
}


void
translator_compile::new_segment(const rcstring &name)
{
    create_code_file_default_byte_sex();
    segment::pointer sp = cfp->new_segment(name, is_a_sysprog());
    segment_builder::pointer sbp = segment_builder_code::create(sp, cfp.get());
    segstack.push_back(sbp);
}


void
translator_compile::new_segment(const rcstring &name, int segnum,
    segkind_t segkind)
{
    create_code_file_default_byte_sex();
    mtype_t mtype = mtype_pcode_from_byte_sex(cfp->get_byte_sex());
    segversion_t version = segversion_3_0;
    segment::pointer sp =
        cfp->new_segment(name, segnum, segkind, mtype, version);
    segment_builder::pointer sbp = segment_builder_code::create(sp, cfp.get());
    segstack.push_back(sbp);
}


segment_builder::pointer
translator_compile::segment_procedure_forward_suspend(void)
{
    segment_builder::pointer sbp = segstack.back();
    // Do not segstack.segment_end() because we haven't even begun, let
    // alone reached the point where the segment is complete.
    segstack.pop_back();
    return sbp;
}


void
translator_compile::segment_procedure_forward_resume(
    const segment_builder::pointer &sbp)
{
    segstack.push_back(sbp);
}


bool
translator_compile::emit_byte(unsigned char byte)
{
    return segstack.emit_byte(byte);
}


bool
translator_compile::emit_word_alignment(unsigned char value)
{
    return segstack.emit_word_alignment(value);
}


bool
translator_compile::emit_odd_alignment(unsigned char value)
{
    return segstack.emit_odd_alignment(value);
}


bool
translator_compile::emit_big(unsigned value)
{
    assert(value < (1u << 15));
    if (value < 128)
        return segstack.emit_byte(value);
    return
        (
            segstack.emit_byte((value >> 8) | 0x80)
        &&
            segstack.emit_byte(value)
        );
}


bool
translator_compile::emit_big_globref(const rcstring &name)
{
    segstack.link_globref_big(name);
    return (segstack.emit_byte(0x80) && segstack.emit_byte(0));
}


bool
translator_compile::emit_big_publref(const rcstring &name)
{
    segstack.link_publref_big(name);
    return (segstack.emit_byte(0x80) && segstack.emit_byte(0));
}


bool
translator_compile::emit_big_privref(const rcstring &name, int nwords)
{
    segstack.link_privref_big(name, nwords);
    return (segstack.emit_byte(0x80) && segstack.emit_byte(0));
}


bool
translator_compile::emit_word(unsigned value)
{
    return segstack.emit_word(value);
}


bool
translator_compile::emit_ldci(int ival)
{
    assert(ival >= -32768);
    assert(ival < 65536);
    short val = ival;
    if (val < -127)
    {
        goto three;
    }
    if (val < 0)
    {
        return emit_byte(SLDC_0 - val) && emit_byte(NGI);
    }
    if (val < 128)
    {
        return emit_byte(SLDC_0 + val);
    }

    // This is always little-endian, even on big-endian hosts
    three:
    return emit_byte(LDCI) && emit_byte(val) && emit_byte(val >> 8);
}


int
translator_compile::ldci_nbytes(int i)
    const
{
    // This code, and the emit_ldci function, above, must Must MUST agree.
    short n = i;
    if (n < -127)
        return 3;
    if (n < 0)
        return 2;
    if (n < 128)
        return 1;
    return 3;
}


int
translator_compile::next_procedure_number()
{
    return segstack.next_procedure_number();
}


void
translator_compile::procedure_parameter_size(unsigned nbytes)
{
    segstack.procedure_parameter_size(nbytes);
}


void
translator_compile::procedure_variable_size(unsigned nbytes)
{
    segstack.procedure_variable_size(nbytes);
}


void
translator_compile::procedure_enter_ic()
{
    segstack.procedure_enter_ic();
}


void
translator_compile::procedure_exit_ic()
{
    segstack.procedure_exit_ic();
}


void
translator_compile::procedure_end(int proc_num, int lex_level)
{
    mtype_t mtype = mtype_pcode_from_byte_sex(cfp->get_byte_sex());
    segstack.procedure_end(proc_num, lex_level, listing, mtype);
}


void
translator_compile::program_end(void)
{
    translator::program_end();
    if (listing)
        scope_top_of_stack_list_symbols(*listing);
}


void
translator_compile::segment_end()
{
    segstack.segment_end();
}


unsigned
translator_compile::get_address()
    const
{
    return segstack.get_address();
}


void
translator_compile::poke(unsigned address, unsigned char value)
{
    segstack.poke(address, value);
}


void
translator_compile::poke_word(unsigned address, unsigned value)
{
    segstack.poke_word(address, value);
}


int
translator_compile::new_jump_table_entry(unsigned address)
{
    return segstack.new_jump_table_entry(address);
}


void
translator_compile::post_order_traversal()
    const
{
    assert(cfp);
    cfp->write_segment_dictionary();
}


expression::pointer
translator_compile::integer_plus_integer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_addition_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_plus_real_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_addition_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_plus_lint_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_addition_long_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_union_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    type::pointer lhs_to_type =
        type_set_sized::create(lhs->get_type()->get_subtype());
    type::pointer rhs_to_type =
        type_set_sized::create(rhs->get_type()->get_subtype());
    return
        expression_set_union::create
        (
            expression_set2set_compile::create(lhs_to_type, lhs, this),
            expression_set2set_compile::create(rhs_to_type, rhs, this),
            this
        );
}


expression::pointer
translator_compile::real_from_integer_expression_factory(
    const expression::pointer &arg)
{
    return expression_real_from_integer_compile::create(arg, this);
}


expression::pointer
translator_compile::integer_minus_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_minus_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_subtraction_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_difference_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    type::pointer lhs_to_type =
        type_set_sized::create(lhs->get_type()->get_subtype());
    type::pointer rhs_to_type =
        type_set_sized::create(rhs->get_type()->get_subtype());
    return
        expression_set_difference::create
        (
            expression_set2set_compile::create(lhs_to_type, lhs, this),
            expression_set2set_compile::create(rhs_to_type, rhs, this),
            this
        );
}


expression::pointer
translator_compile::lint_minus_lint_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_subtraction_long_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::integer_times_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_times_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_times_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_multiplication_long_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_over_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_integer_division_long_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_intersection_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    type::pointer lhs_to_type =
        type_set_sized::create(lhs->get_type()->get_subtype());
    type::pointer rhs_to_type =
        type_set_sized::create(rhs->get_type()->get_subtype());
    return
        expression_set_intersection::create
        (
            expression_set2set_compile::create(lhs_to_type, lhs, this),
            expression_set2set_compile::create(rhs_to_type, rhs, this),
            this
        );
}


expression::pointer
translator_compile::integer_div_integer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_integer_division_compile::create(lhs, rhs, this);
}


expression::pointer
translator_compile::modulo_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_modulo_compile::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_over_real_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_real_division_compile::create(lhs, rhs, this);
}


expression::pointer
translator_compile::boolean_expression_factory(const location &locn, bool value)
{
    return expression_boolean_compile::create(locn, value, this);
}


expression::pointer
translator_compile::boolean_eq_boolean_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_boolean::create(lhs, rhs, this);
}


expression::pointer
translator_compile::integer_eq_integer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::pointer_eq_pointer_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_eq_real_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::string_eq_string_expression_factory(
    const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_eq_string::create(lhs, rhs, this);
}


expression::pointer
translator_compile::array_eq_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing arrays requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    if (!tap->is_packed_array_of_char())
    {
        return expression_eq_multi_word::create(lhs, rhs, this);
    }
    return expression_eq_byte_array::create(lhs, rhs, this);
}


expression::pointer
translator_compile::record_eq_record_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing records requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    return expression_eq_multi_word::create(lhs, rhs, this);
}


expression::pointer
translator_compile::record_ne_record_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing records requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    return expression_ne_multi_word::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_eq_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    return expression_eq_set::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_ne_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    return expression_ne_set::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_le_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    return expression_le_set::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_lt_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    return expression_lt_set::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_ge_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    return expression_ge_set::create(lhs, rhs, this);
}


expression::pointer
translator_compile::set_gt_set_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    return expression_gt_set::create(lhs, rhs, this);
}


expression::pointer
translator_compile::array_ne_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());

    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing arrays requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    if (!tap->is_packed_array_of_char())
    {
        return expression_ne_multi_word::create(lhs, rhs, this);
    }

    return expression_ne_byte_array::create(lhs, rhs, this);
}


expression::pointer
translator_compile::boolean_ne_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_boolean::create(lhs, rhs, this);
}


expression::pointer
translator_compile::integer_ne_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::pointer_ne_pointer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_ne_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::string_ne_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_string::create(lhs, rhs, this);
}


expression::pointer
translator_compile::boolean_lt_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_boolean::create(lhs, rhs, this);
}


expression::pointer
translator_compile::integer_lt_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_lt_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::string_lt_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_string::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_lt_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_lt_long_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::boolean_le_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_boolean::create(lhs, rhs, this);
}


expression::pointer
translator_compile::integer_le_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_le_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_long_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_le_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::string_le_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_le_string::create(lhs, rhs, this);
}


expression::pointer
translator_compile::boolean_gt_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_boolean::create(lhs, rhs, this);
}


expression::pointer
translator_compile::integer_gt_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_gt_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::string_gt_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_string::create(lhs, rhs, this);
}


expression::pointer
translator_compile::boolean_ge_boolean_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_boolean::create(lhs, rhs, this);
}


expression::pointer
translator_compile::integer_ge_integer_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::real_ge_real_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_real::create(lhs, rhs, this);
}


expression::pointer
translator_compile::string_ge_string_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_string::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_ge_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ge_long_integer::create(lhs, rhs, this);
}


statement::pointer
translator_compile::if_statement_factory(const expression::pointer &cnd,
    const statement::pointer &thn, const statement::pointer &els)

{
    return statement_if_compile::create(cnd, thn, els, this);
}


label::pointer
translator_compile::label_factory()
{
    return label_compile::create(this);
}


symbol::pointer
translator_compile::function_symbol_factory(rcstring const &name,
    const scope::pointer &scp, int lex_level, bool new_seg)
{
    assert(scp);
    scope_function_compile::pointer sfcp =
        boost::dynamic_pointer_cast<scope_function_compile>(scp);
    assert(sfcp);
    int seg_num = sfcp->get_segment_number();
    int proc_num = sfcp->get_procedure_number();
    return
        symbol_function_user_defined::create
        (
            name,
            scp,
            lex_level,
            new_seg,
            seg_num,
            proc_num,
            this
        );
}


symbol::pointer
translator_compile::enumerated_symbol_factory(const variable_name &vname,
    const type_enumerated::pointer &vtype, int lex_level, int value)
{
    return
        symbol_enumerated_compile::create(vname, vtype, lex_level, value, this);
}


unsigned
translator_compile::get_segment_number()
    const
{
    return segstack.get_segment_number();
}


expression::pointer
translator_compile::dereference_expression_factory(
    const expression::pointer &operand)
{
    if (operand->is_error())
        return operand;

    if (type_file::is_a(operand->get_type()))
    {
        //
        // The p-System implements files as
        //
        //   TYPE FIB =
        //     RECORD
        //       FWINDOW: WINDOWP; (* USER WINDOW...F^, USED BY GET-PUT *)
        //       blah
        //       blah
        //     END;
        //
        // which makes the address of the file variable also the address
        // of a pointer to the next file record.
        //
        expression::pointer ep1 = operand->strip_indirection();
        assert(ep1);
        type::pointer ep4t = operand->get_type()->get_subtype();
        assert(!type_nothing::is_a(ep4t));
        type::pointer ep3t = type_pointer_unpacked::create(ep4t);
        type::pointer ep2t = type_pointer_unpacked::create(ep3t);
        expression::pointer ep2 = expression_cast::create(ep2t, ep1, this);
        expression::pointer ep3 = expression_load_indirect::create(ep2, this);
        expression::pointer ep4 = expression_load_indirect::create(ep3, this);
        assert(ep4t == ep4->get_type());
        return ep4;
    }

    assert(type_pointer::is_a(operand->get_type()));
    // Strings are different...
    if (type_string::is_a(operand->get_type()->get_subtype()))
        return expression_load_string::create(operand, this);
    return expression_load_indirect::create(operand, this);
}


void
translator_compile::breakpoint(const location &locn)
{
    if (listing)
    {
        listing->associate
        (
            segstack.get_segment_number(),
            segstack.get_address(),
            locn
        );
    }

    if (issue_breakpoint() && locn != bpt_locn)
    {
        int line_number = locn.get_line_number();
        if (line_number > 255)
            line_number = 255;
        emit_byte(BPT);
        emit_byte(line_number);
        bpt_locn = locn;
    }
}


statement::pointer
translator_compile::while_statement_factory(
    const expression::pointer &condition, const statement::pointer &body)
{
    return statement_while_compile::create(condition, body, this);
}


statement::pointer
translator_compile::until_statement_factory(const statement_list &body,
    const expression::pointer &condition)
{
    return statement_until_compile::create(body, condition, this);
}


statement::pointer
translator_compile::infinite_loop_factory(const statement::pointer &body)
{
    return statement_infinite_loop_compile::create(body, this);
}


expression::pointer
translator_compile::logical_and_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_and_compile::create(lhs, rhs, this);
}


expression::pointer
translator_compile::bitwise_and_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_bitwise_and_compile::create(lhs, rhs, this);
}


expression::pointer
translator_compile::logical_or_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_logical_or_compile::create(lhs, rhs, this);
}


expression::pointer
translator_compile::bitwise_or_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_bitwise_or_compile::create(lhs, rhs, this);
}


expression::pointer
translator_compile::logical_not_expression_factory(
    const expression::pointer &arg)
{
    return expression_logical_not_compile::create(arg, this);
}


expression::pointer
translator_compile::bitwise_not_expression_factory(
    const expression::pointer &arg)
{
    return expression_bitwise_not_compile::create(arg, this);
}


expression::pointer
translator_compile::sizeof_expression_factory(const location &locn,
    const type::pointer &arg)
{
    return
        expression_integer_compile::create
        (
            locn,
            arg->get_size_in_bytes(),
            this
        );
}


symbol::pointer
translator_compile::builtin_abs_factory()
{
    return symbol_function_abs::create(this);
}


symbol::pointer
translator_compile::builtin_arctan_factory()
{
    return symbol_function_atan::create("arctan", this);
}


symbol::pointer
translator_compile::builtin_atan_factory()
{
    return symbol_function_atan::create("atan", this);
}


symbol::pointer
translator_compile::builtin_blockread_factory()
{
    return symbol_function_blockread::create(this);
}


symbol::pointer
translator_compile::builtin_blockwrite_factory()
{
    return symbol_function_blockwrite::create(this);
}


symbol::pointer
translator_compile::builtin_chr_factory()
{
    return symbol_function_chr::create(this);
}


symbol::pointer
translator_compile::builtin_close_factory()
{
    return symbol_function_close::create(this);
}


symbol::pointer
translator_compile::builtin_concat_factory()
{
    return symbol_function_concat::create(this);
}


symbol::pointer
translator_compile::builtin_copy_factory()
{
    return symbol_function_copy::create(this);
}


symbol::pointer
translator_compile::builtin_cos_factory()
{
    return symbol_function_cos::create(this);
}


symbol::pointer
translator_compile::builtin_dec_factory()
{
    return symbol_function_pred::create("dec", this);
}


symbol::pointer
translator_compile::builtin_delete_factory()
{
    return symbol_function_delete::create(this);
}


symbol::pointer
translator_compile::builtin_eof_factory()
{
    return symbol_function_eof::create(this);
}


symbol::pointer
translator_compile::builtin_eoln_factory()
{
    return symbol_function_eoln::create(this);
}


symbol::pointer
translator_compile::builtin_exit_factory()
{
    return symbol_function_exit::create(this);
}


symbol::pointer
translator_compile::builtin_exp_factory()
{
    return symbol_function_exp::create(this);
}


symbol::pointer
translator_compile::builtin_gotoxy_factory()
{
    return symbol_function_gotoxy::create(this);
}


symbol::pointer
translator_compile::builtin_get_factory()
{
    return symbol_function_get::create(this);
}


symbol::pointer
translator_compile::builtin_fillchar_factory()
{
    return symbol_function_fillchar::create(this);
}


symbol::pointer
translator_compile::builtin_halt_factory()
{
    return symbol_function_halt::create(this);
}


symbol::pointer
translator_compile::builtin_idsearch_factory()
{
    return symbol_function_idsearch::create(this);
}


symbol::pointer
translator_compile::builtin_inc_factory()
{
    return symbol_function_succ::create("inc", this);
}


symbol::pointer
translator_compile::builtin_insert_factory()
{
    return symbol_function_insert::create(this);
}


symbol::pointer
translator_compile::builtin_ioresult_factory()
{
    return symbol_function_ioresult::create(this);
}


symbol::pointer
translator_compile::builtin_length_factory()
{
    return symbol_function_length::create(this);
}


symbol::pointer
translator_compile::builtin_ln_factory()
{
    return symbol_function_ln::create(this);
}


symbol::pointer
translator_compile::builtin_log_factory()
{
    return symbol_function_log::create(this);
}


symbol::pointer
translator_compile::builtin_mark_factory()
{
    return symbol_function_mark::create(this);
}


symbol::pointer
translator_compile::builtin_memavail_factory()
{
    return symbol_function_memavail::create(this);
}


symbol::pointer
translator_compile::builtin_moveleft_factory()
{
    return symbol_function_moveleft::create(this);
}


symbol::pointer
translator_compile::builtin_moveright_factory()
{
    return symbol_function_moveright::create(this);
}


symbol::pointer
translator_compile::builtin_new_factory()
{
    return symbol_function_new::create(this);
}


symbol::pointer
translator_compile::builtin_odd_factory()
{
    return symbol_function_odd::create(this);
}


symbol::pointer
translator_compile::builtin_opennew_factory()
{
    return symbol_function_opennew::create(this);
}


symbol::pointer
translator_compile::builtin_openold_factory()
{
    return symbol_function_openold::create(this);
}


symbol::pointer
translator_compile::builtin_ord_factory()
{
    return symbol_function_ord::create(this);
}


symbol::pointer
translator_compile::builtin_page_factory()
{
    return symbol_function_page::create(this);
}


symbol::pointer
translator_compile::builtin_pos_factory()
{
    return symbol_function_pos::create(this);
}


symbol::pointer
translator_compile::builtin_pred_factory()
{
    return symbol_function_pred::create("pred", this);
}


symbol::pointer
translator_compile::builtin_put_factory()
{
    return symbol_function_put::create(this);
}


symbol::pointer
translator_compile::builtin_pwroften_factory()
{
    return symbol_function_pwroften::create(this);
}


symbol::pointer
translator_compile::builtin_read_factory()
{
    return symbol_function_read::create(false, this);
}


symbol::pointer
translator_compile::builtin_readln_factory()
{
    return symbol_function_read::create(true, this);
}


symbol::pointer
translator_compile::builtin_release_factory()
{
    return symbol_function_release::create(this);
}


symbol::pointer
translator_compile::builtin_reset_factory()
{
    return symbol_function_reset::create(this);
}


symbol::pointer
translator_compile::builtin_rewrite_factory()
{
    return symbol_function_rewrite::create(this);
}


symbol::pointer
translator_compile::builtin_round_factory()
{
    return symbol_function_round::create(this);
}


symbol::pointer
translator_compile::builtin_seek_factory()
{
    return symbol_function_seek::create(this);
}


symbol::pointer
translator_compile::builtin_sin_factory()
{
    return symbol_function_sin::create(this);
}


symbol::pointer
translator_compile::builtin_sqr_factory()
{
    return symbol_function_sqr::create(this);
}


symbol::pointer
translator_compile::builtin_sqrt_factory()
{
    return symbol_function_sqrt::create(this);
}


symbol::pointer
translator_compile::builtin_str_factory()
{
    return symbol_function_str::create(this);
}


symbol::pointer
translator_compile::builtin_succ_factory()
{
    return symbol_function_succ::create("succ", this);
}


symbol::pointer
translator_compile::builtin_time_factory()
{
    return symbol_function_time::create(this);
}


symbol::pointer
translator_compile::builtin_treesearch_factory()
{
    return symbol_function_treesearch::create(this);
}


symbol::pointer
translator_compile::builtin_trunc_factory()
{
    return symbol_function_trunc::create(this);
}


symbol::pointer
translator_compile::builtin_unitbusy_factory()
{
    return symbol_function_unitbusy::create(this);
}


symbol::pointer
translator_compile::builtin_unitclear_factory()
{
    return symbol_function_unitclear::create(this);
}


symbol::pointer
translator_compile::builtin_unitread_factory()
{
    return symbol_function_unitread::create(this);
}


symbol::pointer
translator_compile::builtin_unitstatus_factory()
{
    return symbol_function_unitstatus::create(this);
}


symbol::pointer
translator_compile::builtin_unitwait_factory()
{
    return symbol_function_unitwait::create(this);
}


symbol::pointer
translator_compile::builtin_unitwrite_factory()
{
    return symbol_function_unitwrite::create(this);
}


symbol_function::pointer
translator_compile::builtin_syscall_factory(const rcstring &name, int seg_num,
    int proc_num)
{
    return
        symbol_function_user_defined::create
        (
            name,
            location::builtin(),
            scope::pointer(),
            -1, // lexical level
            false, // new_seg
            seg_num,
            proc_num,
            this
        );
}


expression::pointer
translator_compile::set2set_expression_factory(const type::pointer &to,
    const expression::pointer &from)
{
    return expression_set2set_compile::create(to, from, this);
}


expression::pointer
translator_compile::empty_set_expression_factory(const location &locn,
    const type::pointer &tp)
{
    return expression_set_compile::create(locn, tp, set(), this);
}


expression::pointer
translator_compile::set_expression_factory(const expression::pointer &num)
{
    return expression_sgs_compile::create(num, this);
}


expression::pointer
translator_compile::set_expression_factory(const expression::pointer &lo,
    const expression::pointer &hi)
{
    return expression_srs_compile::create(lo, hi, this);
}


expression::pointer
translator_compile::char_expression_factory(location const &locn,
    unsigned char value)
{
    return expression_char_compile::create(locn, value, this);
}


expression::pointer
translator_compile::range_check(const expression::pointer &value,
    const type::pointer &against)
{
#if 1
    const type_subrange::pointer stp =
        boost::dynamic_pointer_cast<type_subrange>(against);
    if (stp)
    {
        int min = stp->get_minimum_value();
        int max = stp->get_maximum_value();
        return range_check(value, min, max);
    }
#else
    if (against->may_be_used_as_array_index())
    {
        int min = against->get_minimum_value();
        int max = against->get_maximum_value();
        return range_check(value, min, max);
    }
#endif
    return value;
}


expression::pointer
translator_compile::range_check(const expression::pointer &value,
    int minimum, int maximum)
{
    if (!issue_range_check() && !value->is_constant())
        return value;
    expression::pointer lo =
        expression_integer_compile::create
        (
            value->get_location(),
            minimum,
            this
        );
    expression::pointer hi =
        expression_integer_compile::create
        (
            value->get_location(),
            maximum,
            this
        );
    return expression_check_range::create(value, lo, hi, this);
}


expression::pointer
translator_compile::writeln1_expression_factory(
    const expression::pointer &value)
{
    // If the parameter is of type FILE just return the parameter.  This
    // is used to isolate the file parameter that can optionally appear
    // at the start of the parameter list.  And you can't print files,
    // on files anyway.
    if (type_file::is_a(value->get_type()))
        return value;

    //
    // Usually the width is zero if not specified, but for some reason
    // packed arrays of char are an exception.  Go figure.
    //
    int w = 0;
    {
        type_array::pointer tap =
            boost::dynamic_pointer_cast<type_array>(value->get_type());
        if (tap && tap->is_packed_array_of_char())
            w = tap->get_width();
    }
    expression::pointer width =
        expression_integer_compile::create(value->get_location(), w, this);

    return
        expression_writeln::create
        (
            value,
            width,
            this
        );
}


expression::pointer
translator_compile::writeln2_expression_factory(
    const expression::pointer &value, const expression::pointer &width)
{
    return
        expression_writeln::create
        (
            value,
            width,
            this
        );
}


expression::pointer
translator_compile::writeln3_expression_factory(
    const expression::pointer &value,
    const expression::pointer &width,
    const expression::pointer &precision)
{
    return
        expression_writeln::create
        (
            value,
            width,
            precision,
            this
        );
}


void
translator_compile::codefile_copyright(const rcstring &atext)
{
    rcstring text(atext);
    if (text.substring(0, 3).downcase() == "opy")
        text = "C" + text;
    // FIXME: By IV.0 this was cut to 77, so that a byte sex indicator
    // could be added.  Should we make a similer restriction?
    if (text.size() > 79)
    {
        text = text.substring(0, 79);
        pascal_lex_error
        (
            pascal_lex_location(),
            "the copyright notice is limited to 79 characters, your "
                "notice has been truncated to %s",
            text.quote_c().c_str()
        );
    }
    create_code_file_default_byte_sex();
    cfp->set_copyright_notice(text);
}


scope::pointer
translator_compile::interface_scope_factory(const rcstring &name, int code_seg,
    int data_seg, segkind_t segkind)
{
    assert(code_seg > 0);
    need_segment(code_seg);
    switch (segkind)
    {
    case UNLINKED_INTRINS:
    case DATASEG:
        assert(!"this shouldn't happen");
        // Fall through...

    case LINKED_INTRINS:
        if (data_seg > 0)
            need_segment(data_seg);
        return
            scope_interface_compile::create
            (
                name,
                code_seg,
                data_seg,
                segkind,
                this
            );

    default:
    case SEGPROC:
    case SEPRTSEG:
    case LINKED:
    case HOSTSEG:
        assert(!"this shouldn't happen");
        // Fall through...

    case UNITSEG:
        assert(data_seg < 0);
        return
            scope_interface_unitseg::create
            (
                name,
                code_seg,
                data_seg,
                top_scope(),
                this
            );
    }
}


scope::pointer
translator_compile::compound_temporary_scope_factory(scope_stack &stk)
{
    return scope_temporary_compound_compile::create(stk, this);
}


scope::pointer
translator_compile::with_temporary_scope_factory(scope_stack &stk,
    const expression::pointer &base, const statement::pointer &assign)
{
    return scope_temporary_with_compile::create(stk, base, assign, this);
}


scope::pointer
translator_compile::record_scope_factory(bool is_packed)
{
    return scope_record_compile::create(is_packed, this);
}


void
translator_compile::uses_interface_notify_push(const scope::pointer &sp)
{
    // FIXME: refactor these classes so they have a common base class
    // with a pure virtual method.
    {
        scope_interface_unitseg::pointer siup =
            boost::dynamic_pointer_cast<scope_interface_unitseg>(sp);
        if (siup)
        {
            segstack.push_back(siup->segment_builder_factory());
            return;
        }
    }

    {
        scope_interface_compile::pointer sicp =
            boost::dynamic_pointer_cast<scope_interface_compile>(sp);
        if(sicp)
        {
            segstack.push_back(sicp->segment_builder_factory());
            return;
        }
    }

    assert(!"scope_interface derived class not understood");
}


void
translator_compile::uses_interface_notify_pop(const scope::pointer &sp)
{
    scope_interface::pointer sip =
        boost::dynamic_pointer_cast<scope_interface>(sp);
    assert(sip);

    segstack.segment_end();
}


void
translator_compile::need_segment(unsigned segnum)
{
    unsigned byte_num = segnum >> 3;
    assert(byte_num < sizeof(segment_flags));
    if (byte_num < sizeof(segment_flags))
    {
        unsigned bit = 1 << (segnum & 7);
        segment_flags[byte_num] |= bit;
    }
}


bool
translator_compile::segment_needed(unsigned segnum)
{
    unsigned byte_num = segnum >> 3;
    assert(byte_num < sizeof(segment_flags));
    if (byte_num < sizeof(segment_flags))
    {
        unsigned bit = 1 << (segnum & 7);
        if (segment_flags[byte_num] & bit)
            return true;
    }
    return false;
}


statement::pointer
translator_compile::with_statement_factory(const expression::pointer &,
    const statement::pointer &base_asign, const statement::pointer &body)
{
    if (!base_asign)
        return body;
    statement_list slp;
    slp.push_back(base_asign);
    slp.push_back(body);
    return compound_statement_factory(slp);
}


expression::pointer
translator_compile::nil_expression_factory(void)
{
    return expression_nil_compile::create(pascal_lex_location(), this);
}


statement::pointer
translator_compile::for_statement_factory(const expression::pointer &var,
    const expression::pointer &limit1, bool descending,
    const expression::pointer &limit2, const statement::pointer &body)
{
    return
        statement_for_compile::create
        (
            var,
            limit1,
            descending,
            limit2,
            body,
            this
        );
}


statement::pointer
translator_compile::case_statement_factory(const expression::pointer &control,
    const case_list &cases, const statement::pointer &otherwise)
{
    return statement_case_compile::create(control, cases, otherwise, this);
}


statement::pointer
translator_compile::goto_statement_factory(const symbol::pointer &sym)
{
    return statement_goto_compile::create(sym, this);
}


statement::pointer
translator_compile::label_statement_factory(const symbol::pointer &sym,
    const statement::pointer &body)
{
    return statement_label_compile::create(sym, body, this);
}


expression::pointer
translator_compile::in_expression_factory(const expression::pointer &lhs,
    const expression::pointer &rhs)
{
    return expression_in_compile::create(lhs, rhs, this);
}


void
translator_compile::codefile_bigendian(bool yesno)
{
    if (cfp)
        pascal_grammar_error("too late to change byte sex of codefile");
    else
        create_code_file(yesno ? big_endian : little_endian);
}


void
translator_compile::create_code_file_default_byte_sex()
{
    create_code_file(little_endian);
}


void
translator_compile::create_code_file(byte_sex_t bs)
{
    if (!cfp)
    {
        cfp = codefile_file::create(output_file_name, bs);
    }
}


expression::pointer
translator_compile::paoc_from_string_expression_factory(
    const expression::pointer &arg, const type::pointer &tp)
{
    expression::pointer ep =
        expression_paoc_from_string_compile::create(arg, tp, this);
    DEBUG(2, "%s", ep->lisp_representation().c_str());
    return ep->optimize();
}


expression::pointer
translator_compile::array_lt_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    if (!tap->is_packed_array_of_char())
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "only 'packed array of char' can be compared for less-than"
        );
        return expression_error::create(locn);
    }

    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing arrays requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    return expression_lt_byte_array::create(lhs, rhs, this);
}


expression::pointer
translator_compile::array_le_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    if (!tap->is_packed_array_of_char())
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "only 'packed array of char' can be compared for less-than-or-equal"
        );
        return expression_error::create(locn);
    }

    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing arrays requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    return expression_le_byte_array::create(lhs, rhs, this);
}


expression::pointer
translator_compile::array_ge_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    if (!tap->is_packed_array_of_char())
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "only 'packed array of char' can be compared for "
            "greater-than-or-equal"
        );
        return expression_error::create(locn);
    }

    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing arrays requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    return expression_ge_byte_array::create(lhs, rhs, this);
}


expression::pointer
translator_compile::array_gt_array_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    assert(lhs->get_type() == rhs->get_type());
    type_array::pointer tap =
        boost::dynamic_pointer_cast<type_array>(lhs->get_type());
    assert(tap);
    if (!tap->is_packed_array_of_char())
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "only 'packed array of char' can be compared for greater-than"
        );
        return expression_error::create(locn);
    }

    expression::pointer lhs2 = lhs->strip_indirection();
    expression::pointer rhs2 = rhs->strip_indirection();
    if (!lhs2 || !rhs2)
    {
        location locn(lhs->get_location() + rhs->get_location());
        pascal_lex_error
        (
            locn,
            "comparing arrays requires two variables, not a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(locn);
    }

    return expression_gt_byte_array::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_gt_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_gt_long_integer::create(lhs, rhs, this);
}


void
translator_compile::listing_to_file(const rcstring &filename)
{
    listing = code_source_map::create(filename);
}


statement::pointer
translator_compile::var_statement_factory(variable_name_list const &,
    const type::pointer &)
{
    return empty_statement_factory();
}


statement::pointer
translator_compile::var_statement_factory(const variable_name &name,
    const expression::pointer &rhs)
{
    expression::pointer lhs =
        name_expression(name.get_location(), name.get_name());
    expression::pointer ep = lhs->assignment_expression_factory(rhs);
    return expression_statement(ep);
}


statement::pointer
translator_compile::compound_statement_factory(const statement_list &arg)
{
    if (warning_unreachable())
    {
        for (size_t j = 1; j < arg.size(); ++j)
        {
            if (!arg[j - 1]->execution_flows_out())
            {
                statement::pointer sp = arg[j];
                if (!sp->execution_flows_in())
                {
                    pascal_lex_warning
                    (
                        sp->get_location(),
                        "statement unreachable"
                    );
                }
            }
        }
    }
    return translator::compound_statement_factory(arg);
}


expression::pointer
translator_compile::scan_expression_factory(const expression::pointer &maxdisp,
    int mask, const expression::pointer &value,
    const expression::pointer &start)
{
    return expression_scan_compile::create(maxdisp, mask, value, start, this);
}


void
translator_compile::scope_end(void)
{
    if (listing)
        scope_top_of_stack_list_symbols(*listing);
    translator::scope_end();
}


void
translator_compile::scope_end_external(void)
{
    link_info::pointer lip = scope_end_external_link_info();
    segstack.segment_not_linked(lip);
    translator::scope_end_external();
}


void
translator_compile::fail_hook(void)
{
    unlink(output_file_name.c_str());
}


void
translator_compile::value_traversal_for_logical_expression(const expression *ep)
{
    label::pointer true_branch = label_factory();
    label::pointer false_branch = label_factory();
    label::pointer end_branch = label_factory();
    ep->logical_traversal(0, true_branch, false_branch, true);

    true_branch->define_here();
    expression::pointer e1 =
        expression_boolean_compile::create(ep->get_location(), true, this);
    e1->traversal(0);
    end_branch->goto_from_here();

    false_branch->define_here();
    expression::pointer e2 =
        expression_boolean_compile::create(ep->get_location(), false, this);
    e2->traversal(0);

    end_branch->define_here();
}


expression::pointer
translator_compile::lint2lint_expression_factory(const type::pointer &to,
    const expression::pointer &from)
{
    return expression_lint2lint_compile::create(to, from, this);
}


expression::pointer
translator_compile::lint_from_integer_expression_factory(
    const expression::pointer &operand)
{
    return expression_lint_from_integer_compile::create(operand, this);
}


expression::pointer
translator_compile::ternary_expression_factory(const expression::pointer &cond,
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ternary_compile::create(cond, lhs, rhs, this);
}


bool
translator_compile::emit_ldc(const unsigned short *data, size_t data_size)
{
    //
    // Note: The following code optimizes for space.  If we were
    //       optimizing for speed we would use LDC every time.
    //
    DEBUG(2, "data_size = %d", (int)data_size);
    assert(data_size < 256);

    // LDC n w...w
    int ldc_num_bytes = 2 + data_size * 2;
    DEBUG(2, "ldc_num_bytes = %d", ldc_num_bytes);
    if (segstack.get_address() & 1)
        ++ldc_num_bytes;

    // LDCI*n
    int ldci_num_bytes = 0;
    for (size_t j = 0; j < data_size; ++j)
        ldci_num_bytes += ldci_nbytes(data[j]);
    DEBUG(2, "ldci_num_bytes = %d", ldci_num_bytes);

    bool ok = true;
    if (ldc_num_bytes < ldci_num_bytes)
    {
        DEBUG(2, "LDC wins");
        ok &= emit_word_alignment(NOP);
        ok &= emit_byte(LDC);
        ok &= emit_byte(data_size);
        for (size_t jj = 0; jj < data_size; ++jj)
        {
            size_t j = data_size - 1 - jj;
            int n = data[j];
            DEBUG(2, "word %d = 0x%04X", int(j), n);
            ok &= emit_word(n);
        }
    }
    else
    {
        DEBUG(2, "LDCI wins");
        for (size_t jj = 0; jj < data_size; ++jj)
        {
            size_t j = data_size - 1 - jj;
            int n = data[j];
            DEBUG(2, "word %d = 0x%04X", int(j), n);
            ok &= emit_ldci(n);
        }
    }
    return ok;
}


expression::pointer
translator_compile::integer_from_lint_expression_factory(
    const expression::pointer &operand)
{
    return expression_trunc_long_integer::create(operand, this);
}


expression::pointer
translator_compile::lint_eq_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_eq_long_integer::create(lhs, rhs, this);
}


expression::pointer
translator_compile::lint_ne_lint_expression_factory(
    const expression::pointer &lhs, const expression::pointer &rhs)
{
    return expression_ne_long_integer::create(lhs, rhs, this);
}


void
translator_compile::create_dataseg(const rcstring &name, int segnum,
    int size_in_words)
{
    cfp->create_dataseg(name, segnum, size_in_words);
}


int
translator_compile::uses_unitseg(const rcstring &segname, int segnum)
{
    //
    // If the given segment number is in use,
    // allocate a new one.
    //
    if (cfp->get_segment_by_number(segnum))
        segnum = cfp->next_free_segment_number(is_a_sysprog(), true);

    //
    // Create a zero-length segment in the code file,
    // to tell the linker we need to have it filled in.
    //
    cfp->new_segment(segname, segnum, UNITSEG);

    //
    // Let the linker know this is a unit to be linked to other programs.
    //
    link_info::pointer lip = scope_end_external_link_info();
    segstack.segment_not_linked(lip);

    //
    // Let the caller know which segent we have allocated.
    //
    return segnum;
}


void
translator_compile::link_publdef(const rcstring &name, int base_offset)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(base_offset >= 0);
    assert(base_offset < 32768);
    segstack.link_publdef(name, base_offset);
}


// vim: set ts=8 sw=4 et :

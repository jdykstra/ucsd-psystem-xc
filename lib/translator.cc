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

#include <lib/ac/assert.h>

#include <lib/debug.h>
#include <lib/expression/dispatcher/binary/functor/method.h>
#include <lib/expression/eq.h>
#include <lib/expression/error.h>
#include <lib/expression/list.h>
#include <lib/expression/real_from_integer.h>
#include <lib/label/placebo.h>
#include <lib/name_type/list.h>
#include <lib/pascal/grammar.h>
#include <lib/pascal/lex.h>
#include <lib/scope/builtin.h>
#include <lib/scope/program.h>
#include <lib/set.h>
#include <lib/statement/compound.h>
#include <lib/statement/empty.h>
#include <lib/statement/expression.h>
#include <lib/symbol/expression/constant.h>
#include <lib/symbol/function.h>
#include <lib/symbol/program.h>
#include <lib/symbol/typedef.h>
#include <lib/symbol/variable/normal.h>
#include <lib/translator.h>
#include <lib/type/anything.h>
#include <lib/type/array.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/enumerated.h>
#include <lib/type/error.h>
#include <lib/type/file.h>
#include <lib/type/function.h>
#include <lib/type/integer.h>
#include <lib/type/list.h>
#include <lib/type/long_integer/sized.h>
#include <lib/type/nothing.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/program.h>
#include <lib/type/real.h>
#include <lib/type/record.h>
#include <lib/type/reference.h>
#include <lib/type/set/unsized.h>
#include <lib/type/string.h>
#include <lib/type/subrange.h>
#include <lib/variable/name/list.h>


int translator::max_id_len = 8;
bool translator::feature_underscore_significant = false;


translator::~translator()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


translator::translator() :
    debug(false),
    syscomp(false),
    no_more_codefile_options(false),
    iocheck(true),
    breakpoints_flag(false),
    goto_warning(true),
    feature_range_check_flag(true),
    feature_chr_range_check_flag(false),
    empty_parens_warning(true),
    op_addition("addition"),
    op_subtraction("subtraction"),
    op_multiplication("multiplication"),
    op_div("integer division"),
    op_division("division"),
    op_eq("equal to"),
    op_ne("not equal to"),
    op_lt("less than"),
    op_le("less than or equal to"),
    op_gt("greater than"),
    op_ge("greater than or equal to"),
    op_negate("negative"),
    op_and("and"),
    op_or("or"),
    op_not("not"),
    address_of_warning(true),
    tiny(false),
    short_case_flag(false),
    warnings_are_silent_flag(false),
    warnings_are_errors_flag(false),
    warning_named_label_flag(true),
    warning_shadow_flag(true),
    warning_hex_constants_flag(true),
    warning_binary_constants_flag(true),
    warning_set_comparisons_flag(true),
    feature_extra_set_comparisons_flag(false),
    warning_constant_branch_flag(true),
    warning_parameter_string_size_flag(true),
    warning_otherwise_flag(true),
    warning_ternary_expression_flag(true),
    warning_unreachable_flag(true),
    feature_ignore_undefined_segment_zero_flag(false),
    feature_inline_math_flag(false),
    feature_string_escapes_flag(false),
    feature_short_with_flag(true),
    feature_efj_nfj_flag(true),
    feature_long_integer_constants_flag(true),
    feature_long_integer_extensions_flag(false),
    feature_swapping_flag(false),
    unit_segnum_code(-1),
    unit_segnum_data(-1),
    unit_segkind(UNITSEG)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);

#define glue2(x) \
    expression_dispatcher_binary_functor_method<translator>::create( \
        this, &translator::x)

    op_addition.push_back(type_integer::is_a,
        glue2(integer_plus_integer_expression_factory),
        type_integer::is_a);
    op_addition.push_back(type_integer::is_a,
        glue2(integer_plus_real_expression_factory),
        type_real::is_a);
    op_addition.push_back(type_real::is_a,
        glue2(real_plus_integer_expression_factory),
        type_integer::is_a);
    op_addition.push_back(type_real::is_a,
        glue2(real_plus_real_expression_factory),
        type_real::is_a);
    op_addition.push_back(type_set::is_a,
        glue2(set_union_expression_factory),
        type_set::is_a);
    op_addition.push_back(type_long_integer::is_a,
        glue2(lint_plus_lint_expression_factory),
        type_long_integer::is_a);
    op_addition.push_back(type_long_integer::is_a,
        glue2(lint_plus_int_expression),
        type_integer::is_a);
    op_addition.push_back(type_integer::is_a,
        glue2(int_plus_lint_expression),
        type_long_integer::is_a);

    op_subtraction.push_back(type_integer::is_a,
        glue2(integer_minus_integer_expression_factory),
        type_integer::is_a);
    op_subtraction.push_back(type_integer::is_a,
        glue2(integer_minus_real_expression_factory),
        type_real::is_a);
    op_subtraction.push_back(type_real::is_a,
        glue2(real_minus_integer_expression_factory),
        type_integer::is_a);
    op_subtraction.push_back(type_real::is_a,
        glue2(real_minus_real_expression_factory),
        type_real::is_a);
    op_subtraction.push_back(type_set::is_a,
        glue2(set_difference_expression_factory),
        type_set::is_a);
    op_subtraction.push_back(type_long_integer::is_a,
        glue2(lint_minus_lint_expression_factory),
        type_long_integer::is_a);
    op_subtraction.push_back(type_long_integer::is_a,
        glue2(lint_minus_int_expression),
        type_integer::is_a);
    op_subtraction.push_back(type_integer::is_a,
        glue2(int_minus_lint_expression),
        type_long_integer::is_a);

    op_multiplication.push_back(type_integer::is_a,
        glue2(integer_times_integer_expression_factory),
        type_integer::is_a);
    op_multiplication.push_back(type_integer::is_a,
        glue2(integer_times_real_expression_factory),
        type_real::is_a);
    op_multiplication.push_back(type_real::is_a,
        glue2(real_times_integer_expression_factory),
        type_integer::is_a);
    op_multiplication.push_back(type_real::is_a,
        glue2(real_times_real_expression_factory),
        type_real::is_a);
    op_multiplication.push_back(type_set::is_a,
        glue2(set_intersection_expression_factory),
        type_set::is_a);
    op_multiplication.push_back(type_long_integer::is_a,
        glue2(lint_times_lint_expression_factory),
        type_long_integer::is_a);
    op_multiplication.push_back(type_integer::is_a,
        glue2(int_times_lint_expression),
        type_long_integer::is_a);
    op_multiplication.push_back(type_long_integer::is_a,
        glue2(lint_times_int_expression),
        type_integer::is_a);

    op_div.push_back(type_integer::is_a,
        glue2(integer_div_integer_expression_factory),
        type_integer::is_a);
    op_div.push_back(type_long_integer::is_a,
        glue2(lint_over_lint_expression_factory),
        type_long_integer::is_a);
    op_div.push_back(type_long_integer::is_a,
        glue2(lint_over_int_expression),
        type_integer::is_a);
    op_div.push_back(type_integer::is_a,
        glue2(int_over_lint_expression),
        type_long_integer::is_a);

    op_division.push_back(type_integer::is_a,
        glue2(integer_over_integer_expression_factory),
        type_integer::is_a);
    op_division.push_back(type_integer::is_a,
        glue2(integer_over_real_expression_factory),
        type_real::is_a);
    op_division.push_back(type_real::is_a,
        glue2(real_over_integer_expression_factory),
        type_integer::is_a);
    op_division.push_back(type_real::is_a,
        glue2(real_over_real_expression_factory),
        type_real::is_a);

    op_eq.push_back(type_boolean::is_a,
        glue2(boolean_eq_boolean_expression_factory),
        type_boolean::is_a);
    op_eq.push_back(type_char::is_a,
        glue2(char_eq_char_expression),
        type_char::is_a);
    op_eq.push_back(type_char::is_a,
        glue2(char_eq_string_expression),
        type_string::is_a);
    op_eq.push_back(type_string::is_a,
        glue2(string_eq_char_expression),
        type_char::is_a);
    op_eq.push_back(type_enumerated::is_a,
        glue2(enum_eq_enum_expression_factory),
        type_enumerated::is_a);
    op_eq.push_back(type_integer::is_a,
        glue2(integer_eq_integer_expression_factory),
        type_integer::is_a);
    op_eq.push_back(type_integer::is_a,
        glue2(integer_eq_real_expression_factory),
        type_real::is_a);
    op_eq.push_back(type_real::is_a,
        glue2(real_eq_integer_expression_factory),
        type_integer::is_a);
    op_eq.push_back(type_real::is_a,
        glue2(real_eq_real_expression_factory),
        type_real::is_a);
    op_eq.push_back(type_string::is_a,
        glue2(string_eq_string_expression_factory),
        type_string::is_a);
    op_eq.push_back(type_pointer::is_a,
        glue2(pointer_eq_pointer_expression),
        type_pointer::is_a);
    op_eq.push_back(type_array::is_a,
        glue2(array_eq_array_expression),
        type_array::is_a);
    op_eq.push_back(type_array::is_a,
        glue2(array_eq_string_expression),
        type_string::is_a);
    op_eq.push_back(type_string::is_a,
        glue2(string_eq_array_expression),
        type_array::is_a);
    op_eq.push_back(type_record::is_a,
        glue2(record_eq_record_expression),
        type_record::is_a);
    op_eq.push_back(type_set::is_a,
        glue2(set_eq_set_expression),
        type_set::is_a);
    op_eq.push_back(type_long_integer::is_a,
        glue2(lint_eq_lint_expression_factory),
        type_long_integer::is_a);
    op_eq.push_back(type_integer::is_a,
        glue2(int_eq_lint_expression),
        type_long_integer::is_a);
    op_eq.push_back(type_long_integer::is_a,
        glue2(lint_eq_int_expression),
        type_integer::is_a);

    op_ne.push_back(type_boolean::is_a,
        glue2(boolean_ne_boolean_expression_factory),
        type_boolean::is_a);
    op_ne.push_back(type_char::is_a,
        glue2(char_ne_char_expression),
        type_char::is_a);
    op_ne.push_back(type_char::is_a,
        glue2(char_ne_string_expression),
        type_string::is_a);
    op_ne.push_back(type_string::is_a,
        glue2(string_ne_char_expression),
        type_char::is_a);
    op_ne.push_back(type_enumerated::is_a,
        glue2(enum_ne_enum_expression_factory),
        type_enumerated::is_a);
    op_ne.push_back(type_enumerated::is_a,
        glue2(integer_ne_integer_expression_factory),
        type_enumerated::is_a);
    op_ne.push_back(type_integer::is_a,
        glue2(integer_ne_integer_expression_factory),
        type_integer::is_a);
    op_ne.push_back(type_integer::is_a,
        glue2(integer_ne_real_expression_factory),
        type_real::is_a);
    op_ne.push_back(type_real::is_a,
        glue2(real_ne_integer_expression_factory),
        type_integer::is_a);
    op_ne.push_back(type_real::is_a,
        glue2(real_ne_real_expression_factory),
        type_real::is_a);
    op_ne.push_back(type_string::is_a,
        glue2(string_ne_string_expression_factory),
        type_string::is_a);
    op_ne.push_back(type_pointer::is_a,
        glue2(pointer_ne_pointer_expression),
        type_pointer::is_a);
    op_ne.push_back(type_array::is_a,
        glue2(array_ne_array_expression),
        type_array::is_a);
    op_ne.push_back(type_array::is_a,
        glue2(array_ne_string_expression),
        type_string::is_a);
    op_ne.push_back(type_string::is_a,
        glue2(string_ne_array_expression),
        type_array::is_a);
    op_ne.push_back(type_record::is_a,
        glue2(record_ne_record_expression),
        type_record::is_a);
    op_ne.push_back(type_set::is_a,
        glue2(set_ne_set_expression),
        type_set::is_a);
    op_ne.push_back(type_long_integer::is_a,
        glue2(lint_ne_lint_expression_factory),
        type_long_integer::is_a);
    op_ne.push_back(type_long_integer::is_a,
        glue2(lint_ne_int_expression),
        type_integer::is_a);
    op_ne.push_back(type_integer::is_a,
        glue2(int_ne_lint_expression),
        type_long_integer::is_a);

    op_lt.push_back(type_boolean::is_a,
        glue2(boolean_lt_boolean_expression_factory),
        type_boolean::is_a);
    op_lt.push_back(type_char::is_a,
        glue2(char_lt_char_expression),
        type_char::is_a);
    op_lt.push_back(type_char::is_a,
        glue2(char_lt_string_expression),
        type_string::is_a);
    op_lt.push_back(type_string::is_a,
        glue2(string_lt_char_expression),
        type_char::is_a);
    op_lt.push_back(type_enumerated::is_a,
        glue2(enum_lt_enum_expression_factory),
        type_enumerated::is_a);
    op_lt.push_back(type_integer::is_a,
        glue2(integer_lt_integer_expression_factory),
        type_integer::is_a);
    op_lt.push_back(type_integer::is_a,
        glue2(integer_lt_real_expression_factory),
        type_real::is_a);
    op_lt.push_back(type_real::is_a,
        glue2(real_lt_integer_expression_factory),
        type_integer::is_a);
    op_lt.push_back(type_real::is_a,
        glue2(real_lt_real_expression_factory),
        type_real::is_a);
    op_lt.push_back(type_string::is_a,
        glue2(string_lt_string_expression_factory),
        type_string::is_a);
    op_lt.push_back(type_array::is_a,
        glue2(array_lt_array_expression),
        type_array::is_a);
    op_lt.push_back(type_array::is_a,
        glue2(array_lt_string_expression),
        type_string::is_a);
    op_lt.push_back(type_string::is_a,
        glue2(string_lt_array_expression),
        type_array::is_a);
    op_lt.push_back(type_set::is_a,
        glue2(set_lt_set_expression),
        type_set::is_a);
    op_lt.push_back(type_long_integer::is_a,
        glue2(lint_lt_lint_expression_factory),
        type_long_integer::is_a);
    op_lt.push_back(type_integer::is_a,
        glue2(int_lt_lint_expression),
        type_long_integer::is_a);
    op_lt.push_back(type_long_integer::is_a,
        glue2(lint_lt_int_expression),
        type_integer::is_a);

    op_le.push_back(type_boolean::is_a,
        glue2(boolean_le_boolean_expression_factory),
        type_boolean::is_a);
    op_le.push_back(type_char::is_a,
        glue2(char_le_char_expression),
        type_char::is_a);
    op_le.push_back(type_string::is_a,
        glue2(string_le_char_expression),
        type_char::is_a);
    op_le.push_back(type_char::is_a,
        glue2(char_le_string_expression),
        type_string::is_a);
    op_le.push_back(type_enumerated::is_a,
        glue2(enum_le_enum_expression_factory),
        type_enumerated::is_a);
    op_le.push_back(type_integer::is_a,
        glue2(integer_le_integer_expression_factory),
        type_integer::is_a);
    op_le.push_back(type_integer::is_a,
        glue2(integer_le_real_expression_factory),
        type_real::is_a);
    op_le.push_back(type_real::is_a,
        glue2(real_le_integer_expression_factory),
        type_integer::is_a);
    op_le.push_back(type_real::is_a,
        glue2(real_le_real_expression_factory),
        type_real::is_a);
    op_le.push_back(type_string::is_a,
        glue2(string_le_string_expression_factory),
        type_string::is_a);
    op_le.push_back(type_array::is_a,
        glue2(array_le_array_expression),
        type_array::is_a);
    op_le.push_back(type_array::is_a,
        glue2(array_le_string_expression),
        type_string::is_a);
    op_le.push_back(type_string::is_a,
        glue2(string_le_array_expression),
        type_array::is_a);
    op_le.push_back(type_set::is_a,
        glue2(set_le_set_expression),
        type_set::is_a);
    op_le.push_back(type_long_integer::is_a,
        glue2(lint_le_lint_expression_factory),
        type_long_integer::is_a);
    op_le.push_back(type_integer::is_a,
        glue2(int_le_lint_expression),
        type_long_integer::is_a);
    op_le.push_back(type_long_integer::is_a,
        glue2(lint_le_int_expression),
        type_integer::is_a);

    op_gt.push_back(type_boolean::is_a,
        glue2(boolean_gt_boolean_expression_factory),
        type_boolean::is_a);
    op_gt.push_back(type_char::is_a,
        glue2(char_gt_char_expression),
        type_char::is_a);
    op_gt.push_back(type_char::is_a,
        glue2(char_gt_string_expression),
        type_string::is_a);
    op_gt.push_back(type_string::is_a,
        glue2(string_gt_char_expression),
        type_char::is_a);
    op_gt.push_back(type_enumerated::is_a,
        glue2(enum_gt_enum_expression_factory),
        type_enumerated::is_a);
    op_gt.push_back(type_integer::is_a,
        glue2(integer_gt_integer_expression_factory),
        type_integer::is_a);
    op_gt.push_back(type_integer::is_a,
        glue2(integer_gt_real_expression_factory),
        type_real::is_a);
    op_gt.push_back(type_real::is_a,
        glue2(real_gt_integer_expression_factory),
        type_integer::is_a);
    op_gt.push_back(type_real::is_a,
        glue2(real_gt_real_expression_factory),
        type_real::is_a);
    op_gt.push_back(type_string::is_a,
        glue2(string_gt_string_expression_factory),
        type_string::is_a);
    op_gt.push_back(type_array::is_a,
        glue2(array_gt_array_expression),
        type_array::is_a);
    op_gt.push_back(type_array::is_a,
        glue2(array_gt_string_expression),
        type_string::is_a);
    op_gt.push_back(type_string::is_a,
        glue2(string_gt_array_expression),
        type_array::is_a);
    op_gt.push_back(type_set::is_a,
        glue2(set_gt_set_expression),
        type_set::is_a);
    op_gt.push_back(type_long_integer::is_a,
        glue2(lint_gt_lint_expression_factory),
        type_long_integer::is_a);
    op_gt.push_back(type_integer::is_a,
        glue2(int_gt_lint_expression),
        type_long_integer::is_a);
    op_gt.push_back(type_long_integer::is_a,
        glue2(lint_gt_int_expression),
        type_integer::is_a);

    op_ge.push_back(type_boolean::is_a,
        glue2(boolean_ge_boolean_expression_factory),
        type_boolean::is_a);
    op_ge.push_back(type_char::is_a,
        glue2(char_ge_char_expression),
        type_char::is_a);
    op_ge.push_back(type_char::is_a,
        glue2(char_ge_string_expression),
        type_string::is_a);
    op_ge.push_back(type_string::is_a,
        glue2(string_ge_char_expression),
        type_char::is_a);
    op_ge.push_back(type_enumerated::is_a,
        glue2(enum_ge_enum_expression_factory),
        type_enumerated::is_a);
    op_ge.push_back(type_integer::is_a,
        glue2(integer_ge_integer_expression_factory),
        type_integer::is_a);
    op_ge.push_back(type_integer::is_a,
        glue2(integer_ge_real_expression_factory),
        type_real::is_a);
    op_ge.push_back(type_real::is_a,
        glue2(real_ge_integer_expression_factory),
        type_integer::is_a);
    op_ge.push_back(type_real::is_a,
        glue2(real_ge_real_expression_factory),
        type_real::is_a);
    op_ge.push_back(type_string::is_a,
        glue2(string_ge_string_expression_factory),
        type_string::is_a);
    op_ge.push_back(type_array::is_a,
        glue2(array_ge_array_expression),
        type_array::is_a);
    op_ge.push_back(type_array::is_a,
        glue2(array_ge_string_expression),
        type_string::is_a);
    op_ge.push_back(type_string::is_a,
        glue2(string_ge_array_expression),
        type_array::is_a);
    op_ge.push_back(type_set::is_a,
        glue2(set_ge_set_expression),
        type_set::is_a);
    op_ge.push_back(type_long_integer::is_a,
        glue2(lint_ge_lint_expression_factory),
        type_long_integer::is_a);
    op_ge.push_back(type_integer::is_a,
        glue2(int_ge_lint_expression),
        type_long_integer::is_a);
    op_ge.push_back(type_long_integer::is_a,
        glue2(lint_ge_int_expression),
        type_integer::is_a);

    op_and.push_back(type_boolean::is_a,
        glue2(logical_and_expression_factory),
        type_boolean::is_a);
    op_and.push_back(type_integer::is_a,
        glue2(bitwise_and_expression_factory),
        type_integer::is_a);

    op_or.push_back(type_boolean::is_a,
        glue2(logical_or_expression_factory),
        type_boolean::is_a);
    op_or.push_back(type_integer::is_a,
        glue2(bitwise_or_expression_factory),
        type_integer::is_a);

    op_negate.push_back(type_integer::is_a,
        &translator::integer_negate_expression_factory);
    op_negate.push_back(type_real::is_a,
        &translator::real_negate_expression_factory);
    op_negate.push_back(type_long_integer::is_a,
        &translator::lint_negate_expression_factory);

    op_not.push_back(type_boolean::is_a,
        &translator::logical_not_expression_factory);
    op_not.push_back(type_integer::is_a,
        &translator::bitwise_not_expression_factory);
    DEBUG(1, "}");
}


void
translator::parse(const rcstring &filename)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    pascal_grammar_debug_set(debug);
    pascal_parse(this, filename);
    DEBUG(1, "}");
}


void
translator::enable_grammar_debug(void)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    debug = true;
}


void
translator::pre_order_traversal(void)
    const
{
    // Do nothing.
}


void
translator::post_order_traversal(void)
    const
{
    // Do nothing.
}


void
translator::traversal(void)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);

    //
    // Check that the program doesn't have any undefined FORWARD declared
    // functions in the outermost scope.
    //
    DEBUG(2, "check_defined");
    scopes.check_defined();

    pre_order_traversal();
    scopes.traversal();
    post_order_traversal();
    DEBUG(1, "}");
}


scope::pointer
translator::program_scope(const rcstring &name)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // After this point, the user may not use the B, C, L, S or U
    // control comments.
    //
    no_more_codefile_options = true;

    if (warning_shadow_flag)
    {
        symbol::pointer osp = scopes.lookup(name);
        if (osp)
        {
            pascal_lex_warning
            (
                "program name %s shadows the %s %s",
                name.quote_c().c_str(),
                osp->get_name().quote_c().c_str(),
                osp->get_kind()
            );
        }
    }

    scope::pointer scp = program_scope_factory(name);
    scopes.push_back(scp);
    symbol::pointer sp = symbol_program::create(name, scp, get_lex_level());
    scp->attach_symbol(sp);
    scopes.define(sp, false);

    if (!syscomp)
    {
        //
        // The UCSD p-System calls user programs with two implicit parameters.
        // See system/syssegs.a.text, line 37.
        //
        //     SEGMENT PROCEDURE USERPROGRAM(INPUT, OUTPUT: FIBP);
        //
        // Of course, this means they are defined backwards (sheesh).
        //
        // BUT, see system/system.c.text in the II.0 source where it
        // calls
        //
        //     USERPROGRAM(NIL,NIL);
        //
        // Which makes INPUT useless for READLN et al and OUTPUT useless
        // for WRITELN et al.
        //
        // These aren't the droids you are looking for.
        // Input and output are defined in lib/translator/builtin.cc
        //
        type::pointer tp =
            type_reference::create(type_file::create(type_char::create()));
        assert(tp->get_size_in_bytes() == 2);
        scopes.define
        (
            scopes.parameter_factory
            (
                variable_name("bogus:out"),
                tp,
                get_lex_level()
            ),
            true
        );
        scopes.define
        (
            scopes.parameter_factory
            (
                variable_name("bogus:in"),
                tp,
                get_lex_level()
            ),
            true
        );
    }

    DEBUG(1, "}");
    return scp;
}


bool
translator::current_function_definition_was_forward(void)
    const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    return scopes.current_function_definition_was_forward();
}


scope::pointer
translator::function_scope(const rcstring &name, bool is_func, bool new_seg)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // Make sure the name hasn't already been used.
    //
    scope::pointer scp;
    scope::pointer sp_scope;
    symbol::pointer sp = scopes.lookup(name, &sp_scope);
    if (sp)
    {
        symbol_function::pointer sfp =
            boost::dynamic_pointer_cast<symbol_function>(sp);
        if
        (
            sfp
        &&
            sfp->is_forward()
        &&
            sfp->get_lex_level() == get_lex_level() + 1
        )
        {
            // It is an error if new_seg and the symbol disagree.
            if (new_seg != sfp->is_segment_procedure())
            {
                const char *kind = sfp->get_kind();
                if (new_seg)
                {
                    pascal_lex_error
                    (
                        sfp->get_declaration_location(),
                        "may not declare a non-segment %s forward...",
                        kind
                    );
                    pascal_lex_error
                    (
                        pascal_lex_location(),
                        "... and later declare it a segment %s",
                        kind
                    );
                }
                else
                {
                    pascal_lex_error
                    (
                        sfp->get_declaration_location(),
                        "may not declare a segment %s forward...",
                        kind
                    );
                    pascal_lex_error
                    (
                        pascal_lex_location(),
                        "... and later declare it a non-segment %s",
                        kind
                    );
                }
            }

            // Get the scope from the function symbol,
            // rather than creating a new one.
            scp = sfp->get_locals();
            assert(scp);
        }
        else
        {
            if (sp_scope == scopes.back())
            {
                pascal_lex_error
                (
                    pascal_lex_location(),
                    "function %s conflicts with an earlier symbol...",
                    name.quote_c().c_str()
                );
                pascal_lex_error
                (
                    sp->get_declaration_location(),
                    "... here is the earlier %s %s declaration",
                    sp->get_name().quote_c().c_str(),
                    sp->get_kind()
                );
            }
            else if (warning_shadow_flag)
            {
                pascal_lex_warning
                (
                    "function %s shadows an earlier symbol...",
                    name.quote_c().c_str()
                );
                pascal_lex_warning
                (
                    sp->get_declaration_location(),
                    "... here is the earlier %s %s declaration",
                    sp->get_name().quote_c().c_str(),
                    sp->get_kind()
                );
            }
            sp.reset();
            assert(!scp);
        }
    }
    if (sp)
    {
        assert(scp);
        scopes.push_back(scp);
    }
    else
    {
        if (new_seg && scopes.within_a_unit())
        {
            pascal_lex_error
            (
                pascal_lex_location(),
                "may not have a segment %s within a unit",
                (is_func ? "function" : "procedure")
            );
        }

        assert(!scp);
        scp = function_scope_factory(name, is_func, new_seg);
        assert(scp);
        sp = function_symbol_factory(name, scp, get_lex_level() + 1, new_seg);
        scp->attach_symbol(sp);

        //
        // The ordering here is different than the program_scope case,
        // because the function is defined in the enclosing scope, not
        // in its own scope.
        //
        scopes.define(sp, false);
        scopes.push_back(scp);
    }
    DEBUG(1, "}");
    return scp;
}


void
translator::scope_to_be_continued(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    // This happens for FORWARD function declarations.
    scopes.set_forward(feature_ignore_undefined_segment_zero_flag);
    scopes.pop_back();
    DEBUG(2, "stack depth = %d", int(scopes.size()));
    DEBUG(1, "}");
}


void
translator::scope_part_begin(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    scopes.begin_notify();
    DEBUG(1, "}");
}


void
translator::scope_end(void)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);

    //
    // Check that the function doesn't have any undefined FORWARD declared
    // functions or labels in the outermost scope.
    //
    DEBUG(2, "check_defined");
    scopes.check_defined();

    // Generate code for the function scope on the top of the scope stack.
    scopes.traversal();

    scopes.drop_locals();
    scopes.pop_back();
    DEBUG(2, "stack depth = %d", int(scopes.size()));
    DEBUG(1, "}");
}


link_info::pointer
translator::scope_end_external_link_info(void)
    const
{
    return scopes.get_link_info_extproc();
}


void
translator::scope_end_external(void)
{
    //
    // Check that the function doesn't have any undefined FORWARD declared
    // functions or labels in the outermost scope.
    //
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    scopes.drop_locals();
    scopes.pop_back();
    DEBUG(2, "stack depth = %d", int(scopes.size()));
    DEBUG(1, "}");
}


void
translator::scope_attach_function_parameters(name_type_list *ntlp)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // We have to declare each of the parameters BACKWARDS, which is why
    // we are given them all at once.  The UCSD native compiler implemented
    // a grammar that kept them all on the parse stack, but we need to
    // play both backwards and forwards games.
    //
    // The reason that we declare them backwards is because the value
    // stack grows downwards, causing the last parameter pushed onto
    // the stack to have the lowest address.  The variable layout in
    // our stack frame must therefore match if we are to access the
    // parameters.
    //
    for (size_t j = 0; j < ntlp->size(); ++j)
    {
        DEBUG(2, "j=%d", int(j));
        const name_type &nt = ntlp->get(ntlp->size() - 1 - j);
        if
        (
            warning_parameter_string_size_flag
        &&
            type_string::is_a(nt.get_type())
        )
        {
            type_string::pointer tsp =
                boost::dynamic_pointer_cast<type_string>(nt.get_type());
            assert(tsp);
            if (tsp->get_size() != type_string::default_size)
            {
                pascal_lex_warning
                (
                    nt.get_name(0).get_location(),
                    "the UCSD native Pascal compiler did not allow "
                    "specification of string lengths for parameters"
                );
            }
        }
        for (size_t k = 0; k < nt.size(); ++k)
        {
            DEBUG(2, "k=%d", (int)k);
            const variable_name &vn = nt.get_name(nt.size() - 1 - k);
            rcstring name = vn.get_name();
            DEBUG(2, "name=%s", name.quote_c().c_str());

            //
            // Make sure this name has not been defined already.
            //
            scope::pointer sp_scope;
            symbol::pointer sp = scopes.lookup(name, &sp_scope);
            if (sp)
            {
                DEBUG(2, "sp=%p", sp.get());
                if (sp_scope == scopes.back())
                {
                    pascal_lex_error
                    (
                        vn.get_location(),
                        "parameter %s conflicts with an earlier symbol...",
                        name.quote_c().c_str()
                    );
                    pascal_lex_error
                    (
                        sp->get_declaration_location(),
                        "... here is the earlier %s %s declaration",
                        sp->get_name().quote_c().c_str(),
                        sp->get_kind()
                    );
                }
                else if (warning_shadow_flag)
                {
                    pascal_lex_warning
                    (
                        vn.get_location(),
                        "parameter %s shadows an earlier symbol...",
                        name.quote_c().c_str()
                    );
                    pascal_lex_warning
                    (
                        sp->get_declaration_location(),
                        "... here is the earlier %s %s declaration",
                        sp->get_name().quote_c().c_str(),
                        sp->get_kind()
                    );
                }
            }

            //
            // Define the symbol.
            //
            sp = scopes.parameter_factory(vn, nt.get_type(), get_lex_level());
            scopes.define(sp, true);
            DEBUG(2, "sp=%p %s %s", sp.get(), sp->get_kind(),
                sp->get_name().quote_c().c_str());
        }
    }

    //
    // Let the translator specific action happen, if any are needed.
    //
    DEBUG(2, "mark");
    scopes.attach_function_parameters(ntlp);

    //
    // Not needed any longer.
    //
    delete ntlp;
    DEBUG(1, "}");
}


void
translator::scope_attach_return_type(const type::pointer &tp)
{
    scopes.attach_return_type(tp);
}


void
translator::scope_attach_body(const statement::pointer &sp)
{
    scopes.attach_body(sp);
}


statement::pointer
translator::compound_statement_factory(const statement_list &arg)
{
    return statement_compound::create(arg);
}


void
translator::constant_declaration(const variable_name &name,
    const expression::pointer &ep)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    //
    // Make sure the expression they gave is actually a constant.
    //
    expression::pointer value = ep->optimize();
    if (!value->is_constant())
    {
        pascal_lex_error
        (
            name.get_location(),
            "constant %s is not constant",
            name.get_name().c_str()
        );
    }

    //
    // Define a symbol with this constant value.
    //
    symbol::pointer sp =
        scopes.constant_factory(name, get_lex_level(), value);
    scopes.define(sp, false);
    DEBUG(2, "sp=%p %s %s", sp.get(), sp->get_kind(),
        sp->get_name().quote_c().c_str());

    //
    // And we need to let the pretty printer know, too.
    //
    scopes.constant_declaration_notify(name, ep);
    DEBUG(1, "}");
}


void
translator::type_declaration(const variable_name &name,
    const type::pointer &tp)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    //
    // Define a symbol with this type.
    //
    symbol::pointer sp = symbol_typedef::create(name, tp, get_lex_level());
    scopes.define(sp, false);

    //
    // And we need to let the pretty printer know, too.
    //
    scopes.type_declaration_notify(name, tp);
}


void
translator::variable_declaration(const variable_name_list &names,
    const type::pointer &tp)
{
    DEBUG(1, "%s\n", __PRETTY_FUNCTION__);
    //
    // The UCSD compiler declares them backwards, probably because right
    // recursion was used in the grammar.  That uses the parse stack for
    // storage, rather than the explicit list you see here.
    //
    for (size_t j = 0; j < names.size(); ++j)
    {
        // We define the names backwards, to mimic the order used by the
        // UCSD Pascal compiler.
        size_t k = names.size() - 1 - j;
        rcstring name = names[k].get_name();
        DEBUG(2, "name=%s\n", name.quote_c().c_str());

        scope::pointer sp_scope;
        symbol::pointer sp = scopes.lookup(name, &sp_scope);
        if (sp)
        {
            if (sp_scope == scopes.back())
            {
                pascal_lex_error
                (
                    names[k].get_location(),
                    "variable %s conflicts with an earlier symbol...",
                    name.quote_c().c_str()
                );
                pascal_lex_error
                (
                    sp->get_declaration_location(),
                    "... here is the earlier %s %s declaration",
                    sp->get_name().quote_c().c_str(),
                    sp->get_kind()
                );
            }
            else if (warning_shadow_flag)
            {
                pascal_lex_warning
                (
                    names[k].get_location(),
                    "variable %s shadows an earlier symbol...",
                    name.quote_c().c_str()
                );
                pascal_lex_warning
                (
                    sp->get_declaration_location(),
                    "... here is the earlier %s %s declaration",
                    sp->get_name().quote_c().c_str(),
                    sp->get_kind()
                );
            }
        }

        symbol_variable::pointer svp =
            scopes.variable_factory(name, tp, get_lex_level());
        scopes.define(svp, false);
        DEBUG(2, "name=%s, offset=%s\n", svp->get_name().quote_c().c_str(),
            svp->get_bit_offset().representation().c_str());
        DEBUG(2, "type=%s\n", typeid(*svp).name());
        scopes.variable_declaration_notify(svp);
    }
}


expression::pointer
translator::name_expression(const location &locn, const rcstring &name)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    symbol::pointer sp = scopes.lookup(name);
    if (!sp)
    {
        sp = scopes.lookup_fuzzy(name);
        if (!sp)
        {
            pascal_lex_error(locn, "symbol %s unknown", name.quote_c().c_str());
            return expression_error::create(pascal_lex_location());
        }
        pascal_lex_error(locn, "symbol %s unknown...", name.quote_c().c_str());
        pascal_lex_error
        (
            sp->get_declaration_location(),
            "... guessing you meant the %s %s instead",
            sp->get_name().quote_c().c_str(),
            sp->get_kind()
        );
    }
    assert(sp);

    // Quietly turn program name into "program" procedure.
    // This is all so that EXIT(programName) works correctly.
    if (type_program::is_a(sp->get_type()))
    {
        sp = scopes.lookup("program");
        assert(sp);
    }

    sp->add_reference(locn, name);
    expression::pointer result =
        sp->name_expression_factory(sp, locn, get_lex_level());
    DEBUG(2, "return %s", result->lisp_representation().c_str());
    DEBUG(1, "}");
    return result;
}


expression::pointer
translator::function_call_expression(const expression::pointer &lhs,
    const expression_list &rhs)
{
    //
    // We don't do routine expression promotions here because some of
    // the system functions are *real* funky.  Let each do the necessary
    // promotions themselves.
    //
    expression_list args(rhs);
    args.push_front(lhs);
    return lhs->function_call_expression_factory(args);
}


statement::pointer
translator::expression_statement(const expression::pointer &ro_arg)
{
    //
    // Suppress secondary errors.
    //
    if (ro_arg->is_error())
        return statement_empty::create();

    //
    // Promote function expressions into function call expressions with
    // no parameters.
    //
    expression::pointer arg = ro_arg;
    if (type_function::is_a(arg->get_type()))
    {
        arg = function_call_expression(arg, expression_list());
    }
    if (arg->is_error())
        return statement_empty::create();

    //
    // Make the expression as efficient as possible.
    //
    arg = arg->optimize();
    if (arg->is_error())
        return statement_empty::create();

    //
    // Make sure we like the type of the expression.
    //
    if (type_nothing::is_a(arg->get_type()))
    {
        switch (arg->has_side_effect())
        {
        case expression::side_effect_yes:
            break;

        case expression::side_effect_partial:
            pascal_lex_warning
            (
                arg->get_location(),
                "statement with partial effect"
            );
            break;

        case expression::side_effect_no:
            pascal_lex_warning(arg->get_location(), "statement with no effect");
            break;
        }
    }
    else
    {
        if
        (
            type_boolean::is_a(arg->get_type())
        &&
            dynamic_cast<expression_eq *>(arg.operator->())
        )
        {
            pascal_lex_error
            (
                arg->get_location(),
                "statement expression is a %s, it should be nothing; "
                "did you mean to use an assignment (written \":=\") instead "
                "of an equality test (written \"=\")?",
                arg->get_description().c_str()
            );
        }
        else
        {
            pascal_lex_error
            (
                arg->get_location(),
                "statement expression is a %s, it should be nothing",
                arg->get_description().c_str()
            );
        }
    }
    return expression_statement_factory(arg);
}


statement::pointer
translator::expression_statement_factory(const expression::pointer &arg)
{
    // Nothing special happens here, because
    // (a) it all happens when the expression is traversed, and
    // (b) we have already issued an error if the expression is non-void,
    //     because there are no "pop" opcodes to discard value stack entries.
    return statement_expression::create(arg);
}


expression::pointer
translator::dereference_expression(const expression::pointer &arg)
{
    if (arg->is_error())
        return arg;

    if (type_file::is_a(arg->get_type()))
    {
        if (type_nothing::is_a(arg->get_type()->get_subtype()))
        {
            pascal_lex_error
            (
                arg->get_location(),
                "raw files do not have a ^ variable"
            );
            return expression_error::create(arg->get_location());
        }
    }
    else if (!type_pointer::is_a(arg->get_type()))
    {
        pascal_lex_error
        (
            arg->get_location(),
            "unable to dereference a %s",
            arg->get_description().c_str()
        );
        return expression_error::create(arg->get_location());
    }

    return dereference_expression_factory(arg);
}


expression::pointer
translator::call_noparam_func_if_necessary(const expression::pointer &arg)
{
    expression::pointer ep = arg;
    if (ep->is_error())
        return ep;
    if (type_function::is_a(ep->get_type()))
        ep = function_call_expression(ep, expression_list());
    return ep;
}


statement::pointer
translator::empty_statement_factory(void)
{
    return statement_empty::create();
}


label::pointer
translator::label_factory(void)
{
    return label_placebo::create();
}


int
translator::get_lex_level(void)
    const
{
    //
    // The shortest the scope stack can be is two deep: one for the
    // builtins, and one for the program or unit.
    //
    // Except when defining the builtins, when it is only one.
    //
    assert(scopes.size() >= 1);
    int n = scopes.get_lex_level();

    //
    // System programs flagged by the contol comment (*$U-*) start at
    // lex level -1.  User programs start at lex level 0.
    //
    if (syscomp)
        --n;
    return n;
}


bool
translator::is_current_function(const symbol::pointer &sp)
    const
{
    return scopes.is_top_symbol(sp);
}


bool
translator::function_is_on_scope_stack(const symbol::pointer &sp)
    const
{
    return scopes.function_is_on_scope_stack(sp);
}


bool
translator::is_a_type_name(const rcstring &name)
    const
{
    return !!get_type_by_name(name);
}


type::pointer
translator::get_type_by_name(const rcstring &name)
    const
{
    symbol::pointer sp = scopes.lookup(name);
    if (!sp)
        return type::pointer();
    symbol_typedef::pointer stp =
        boost::dynamic_pointer_cast<symbol_typedef>(sp);
    if (!stp)
        return type::pointer();
    return stp->get_type();
}


type::pointer
translator::subrange_type(const expression::pointer &minimum,
    const expression::pointer &maximum)
{
    if (minimum->is_error())
        return minimum->get_type();
    if (maximum->is_error())
        return maximum->get_type();

    expression::pointer e1 = minimum->optimize();
    if (!e1->is_constant())
    {
        pascal_lex_error
        (
            e1->get_location(),
            "subrange lower bound is not constant"
        );
        oops:
        return type_subrange::create(0, 127, type_integer::create());
    }
    expression::pointer e2 = maximum->optimize();
    if (!e2->is_constant())
    {
        pascal_lex_error
        (
            e2->get_location(),
            "subrange upper bound is not constant"
        );
        goto oops;
    }

    //
    // String constants of length one are char constants in this context.
    //
    {
        expression::pointer e1c = char_from_string_of_length_one(e1);
        if (e1c)
            e1 = e1c;
    }
    {
        expression::pointer e2c = char_from_string_of_length_one(e2);
        if (e2c)
            e2 = e2c;
    }

    // You can have subranges of integers... this is the commonest case.
    // You can have subranges of enumerated types.
    // You can have subranges of char.
    if (minimum->get_type() != maximum->get_type())
    {
        pascal_lex_error
        (
            minimum->get_location(),
            "subrange limits must be the same type, not %s and %s types",
            minimum->get_type()->get_name().c_str(),
            maximum->get_type()->get_name().c_str()
        );
        goto oops;
    }

    if
    (
        !type_integer::is_a(e1)
    &&
        !type_enumerated::is_a(e1)
    &&
        !type_char::is_a(e1)
    )
    {
        pascal_lex_error
        (
            e1->get_location(),
            "unable to subrange %s type",
            e1->get_type()->get_name().c_str()
        );
        goto oops;
    }

    // already optimized
    int c1 = e1->get_integer_value();
    int c2 = e2->get_integer_value();
    type::pointer tp = e1->get_type();
    if (c1 > c2)
    {
        pascal_lex_error
        (
            e1->get_location(),
            "invalid subrange bounds, the minimum (%s) must be less than "
                "the maximum (%s)",
            tp->get_human_readable_representation_of_value(c1).c_str(),
            tp->get_human_readable_representation_of_value(c2).c_str()
        );
        int x = c1;
        c1 = c2;
        c2 = x;
    }
    return type_subrange::create(c1, c2, tp);
}


type::pointer
translator::set_type(const type::pointer &subtype)
{
    if
    (
        !type_subrange::is_a(subtype)
    &&
        !type_boolean::is_a(subtype)
    &&
        !type_enumerated::is_a(subtype)
    &&
        !type_char::is_a(subtype)
    )
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "invalid SET type declaration, "
                "you may not create a set from a \"%s\" type",
            subtype->get_name().c_str()
        );
        return type_error::create();
    }

    //
    // Sets are limited to 255 16-bit words.
    // (The ADJ opcode takes a one-byte size parameter.)
    //
    if (subtype->get_maximum_value() > set::range_maximum)
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "set too large, maximum value must be %d or less, not %d",
            set::range_maximum,
            subtype->get_maximum_value()
        );
        return type_error::create();
    }

    return type_set_unsized::create(subtype);
}


type::pointer
translator::new_array_type(bool packed, const type::pointer &a_array_index_type,
        const type::pointer &array_element_type)
{
    type::pointer array_index_type = a_array_index_type;
    if (array_element_type->get_size_in_bits() > 8)
        packed = false;
    if (!array_index_type->may_be_used_as_array_index())
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "array index declaration must be an enumerated or subrange type, "
                "not %s type",
            array_index_type->get_name().c_str()
        );
        array_index_type = type_boolean::create();
    }
    return type_array::create(packed, array_index_type, array_element_type);
}


type::pointer
translator::new_array_type(bool packed, const type_list &index_types,
        const type::pointer &array_element_type)
{
    assert(!index_types.empty());
    type::pointer tp = array_element_type;
    for (size_t jj = 0; jj < index_types.size(); ++jj)
    {
        // we will work right-to-left
        // of, course, we may be wrong
        size_t j = index_types.size() - 1 - jj;
        tp = new_array_type(packed, index_types[j], tp);
    }
    return tp;
}


bit_address
translator::get_data_mark(void)
    const
{
    return scopes.get_data_mark();
}


void
translator::set_data_mark(const bit_address &arg)
{
    scopes.set_data_mark(arg);
}


void
translator::complain_about_empty_parens(void)
{
    if (empty_parens_warning)
    {
        pascal_lex_warning
        (
            "this is Pascal, not C, empty parentheses are implicit, "
            "to silence this message use (*$warning empty-parentheses false*)"
        );
        empty_parens_warning = false;
    }
}


void
translator::complain_about_address_of(void)
{
    if (address_of_warning)
    {
        pascal_lex_warning
        (
            "the address-of operator was not present in UCSD Pascal, to "
            "silence this message use (*$warning address-of false*)"
        );
        address_of_warning = false;
    }
}


expression::pointer
translator::implicit_dot_expression(const expression::pointer &lhs,
    const variable_name &rhs)
{
    return dot_expression(lhs, rhs);
}


void
translator::complain_about_named_label(const variable_name &name)
{
    if (warning_named_label_flag)
    {
        pascal_lex_warning
        (
            name.get_location(),
            "label %s is not a number, named labels are not available in "
                "UCSD Pascal; to silence this message use "
                "(*$warning named-label false*)",
            name.get_name().quote_c().c_str()
        );
        warning_named_label_flag = false;
    }
}


void
translator::complain_about_hex_constant(void)
{
    if (warning_hex_constants_flag)
    {
        pascal_lex_warning
        (
            "hexadecimal constants were not available in UCSD Pascal; "
            "to silence this message use (*$warning hex-constants false*)"
        );
        warning_hex_constants_flag = false;
    }
}


void
translator::complain_about_binary_constant(void)
{
    if (warning_binary_constants_flag)
    {
        pascal_lex_warning
        (
            "binary constants were not available in UCSD Pascal; "
            "to silence this message use (*$warning binary-constants false*)"
        );
        warning_binary_constants_flag = false;
    }
}


void
translator::complain_about_otherwise(void)
{
    if (warning_otherwise_flag)
    {
        pascal_lex_warning
        (
            "the original UCSD Pascal did not support the OTHERWISE clause; "
            "to silence this message use the (*$warning otherwise false*) "
            "control comment"
        );
        warning_otherwise_flag = false;
    }
}


bool
translator::warning_set_comparisons(void)
{
    bool result = warning_set_comparisons_flag;
    // only issue this warning once
    warning_set_comparisons_flag = false;
    return result;
}


bool
translator::feature_extra_set_comparisons(void)
    const
{
    return feature_extra_set_comparisons_flag;
}


bool
translator::warning_constant_branch(void)
    const
{
    return warning_constant_branch_flag;
}


void
translator::scope_top_of_stack_list_symbols(code_source_map &where)
    const
{
    scopes.list_symbols(where);
}


void
translator::program_end(void)
{
    scopes.check_defined();
}


void
translator::fail_hook(void)
{
}


expression::pointer
translator::unary_plus_expression(const expression::pointer &ep)
{
    if (ep->is_error())
        return ep;
    if
    (
        !type_integer::is_a(ep)
    &&
        !type_long_integer::is_a(ep)
    &&
        !type_real::is_a(ep))
    {
        pascal_lex_error
        (
            ep->get_location(),
            "unary plus requires an integer or a real, not a %s",
            ep->get_description().c_str()
        );
        return expression_error::create(ep->get_location());
    }
    return ep;
}


bool
translator::feature_string_escapes(void)
    const
{
    return feature_string_escapes_flag;
}


bool
translator::feature_long_integer_constants(void)
    const
{
    return feature_long_integer_constants_flag;
}


bool
translator::feature_long_integer_extensions(void)
    const
{
    return feature_long_integer_extensions_flag;
}


expression::pointer
translator::ternary_expression(const expression::pointer &a_cond,
    const expression::pointer &a_lhs, const expression::pointer &a_rhs)
{
    expression::pointer cond = call_noparam_func_if_necessary(a_cond);
    if (cond->is_error())
        return cond;

    expression::pointer lhs = call_noparam_func_if_necessary(a_lhs);
    if (lhs->is_error())
        return lhs;

    expression::pointer rhs = call_noparam_func_if_necessary(a_rhs);
    if (rhs->is_error())
        return rhs;

    if (!type_boolean::is_a(cond))
    {
        pascal_lex_error
        (
            cond->get_location(),
            "ternary condition must be a boolean value, not a %s",
            cond->get_description().c_str()
        );
        return expression_error::create(cond->get_location());
    }

    //
    // Quietly promote integers to long integers when needed.
    //
    if (type_long_integer::is_a(lhs) && type_integer::is_a(rhs))
    {
        rhs = lint_from_integer_expression_factory(rhs);
    }
    if (type_integer::is_a(lhs) && type_long_integer::is_a(rhs))
    {
        lhs = lint_from_integer_expression_factory(lhs);
    }
    if
    (
        type_long_integer::is_a(lhs)
    &&
        type_long_integer::is_a(rhs)
    )
    {
        type::pointer tp = type_long_integer_sized::create();
        lhs = lint2lint_expression_factory(tp, lhs);
        rhs = lint2lint_expression_factory(tp, rhs);
    }

    //
    // Quietly promote integers to reals when needed.
    //
    if (type_integer::is_a(lhs) && type_real::is_a(rhs))
    {
        lhs = real_from_integer_expression_factory(lhs);
    }
    if (type_real::is_a(lhs) && type_integer::is_a(rhs))
    {
        rhs = real_from_integer_expression_factory(rhs);
    }

    //
    // make sure the value types match
    //
    if (lhs->get_type() != rhs->get_type())
    {
        pascal_lex_error
        (
            lhs->get_location(),
            "ternary expression type mismatch, given a %s and a %s",
            lhs->get_description().c_str(),
            rhs->get_description().c_str()
        );
        return expression_error::create(lhs->get_location());
    }

    if (warning_ternary_expression_flag)
    {
        pascal_lex_warning
        (
            cond->get_location(),
            "the ternary operator was not available in the UCSD native "
            "compiler; use (*$warning ternary-expression false*) to silence "
            "this warning"
        );
        warning_ternary_expression_flag = false;
    }

    //
    // Looks OK so far.
    //
    return ternary_expression_factory(cond, lhs, rhs);
}


void
translator::complain_if_not_swapping(void)
    const
{
    if (!feature_swapping_flag)
    {
        pascal_lex_warning
        (
            "you need to enable (*$S+*) when compiling units with the "
            "UCSD native compiler"
        );
    }
}


void
translator::complain_if_not_syscomp(void)
    const
{
    if (!syscomp)
    {
        pascal_lex_warning
        (
            "you need to select (*$U-*) when compiling sub-units with the "
            "UCSD native compiler"
        );
    }
}


void
translator::unit_interface_begin(void)
{
    scopes.unit_interface_begin();
}


void
translator::unit_interface_end(void)
{
    scopes.unit_interface_end();
}


void
translator::unit_implementation_begin(void)
{
    scopes.unit_implementation_begin();
}


void
translator::unit_implementation_end(void)
{
    scopes.unit_implementation_end();
}


scope::pointer
translator::top_scope(void)
    const
{
    return scopes.back();
}


// vim: set ts=8 sw=4 et :

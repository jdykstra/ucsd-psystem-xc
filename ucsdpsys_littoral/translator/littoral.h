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

#ifndef UCSDPSYS_PRETTY_TRANSLATOR_PRETTY_H
#define UCSDPSYS_PRETTY_TRANSLATOR_PRETTY_H

#include <lib/output.h>
#include <lib/rcstring/list.h>
#include <lib/translator.h>

/**
  * The translator_littoral class is used to represent a littoral printer
  * for UCSD Pascal.
  */
class translator_littoral:
    public translator
{
public:
    /**
      * The destructor.
      */
    virtual ~translator_littoral();

    /**
      * The default constructor.
      */
    translator_littoral(const rcstring &filename = "-");

protected:
    // See base class for documentation.
    scope::pointer program_scope_factory(const rcstring &name);

    // See base class for documentation.
    scope::pointer builtin_scope_factory(void);

    // See base class for documentation.
    scope::pointer unit_scope_factory(const rcstring &name, int code_seg,
        int data_seg, int llht, segkind_t segkind);

    // See base class for documentation.
    scope::pointer function_scope_factory(const rcstring &name, bool is_func,
        bool new_seg);

    // See base class for documentation.
    scope::pointer compound_temporary_scope_factory(scope_stack &stk);

    // See base class for documentation.
    scope::pointer with_temporary_scope_factory(scope_stack &stk,
        const expression::pointer &base, const statement::pointer &assign);

    // See base class for documentation.
    statement::pointer if_statement_factory(
        const expression::pointer &condition,
        const statement::pointer &then_clause,
        const statement::pointer &else_clause);

    // See base class for documentation.
    statement::pointer compound_statement_factory(const statement_list &arg);

    // See base class for documentation.
    statement::pointer while_statement_factory(
        const expression::pointer &condition, const statement::pointer &body);

    // See base class for documentation.
    statement::pointer with_statement_factory(const expression::pointer &ep,
        const statement::pointer &base_asign, const statement::pointer &body);

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
    expression::pointer integer_minus_integer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer real_minus_real_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_difference_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for documentation.
    expression::pointer set_union_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

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
    expression::pointer lint_expression_factory(const location &locn,
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
    expression::pointer real_expression_factory(const location &locn,
        const rcstring &value);

    // See base class for documentation.
    expression::pointer string_expression_factory( const location &locn,
        const rcstring &value);

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
    expression::pointer real_from_integer_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer sizeof_expression_factory(
        const expression::pointer &arg);

    // See base class for documentation.
    expression::pointer sizeof_expression_factory(const location &locn,
        const type::pointer &arg);

    // See base class for documentation.
    symbol::pointer builtin_abs_factory();

    // See base class for documentation.
    symbol::pointer builtin_arctan_factory();

    // See base class for documentation.
    symbol::pointer builtin_atan_factory();

    // See base class for documentation.
    symbol::pointer builtin_blockread_factory();

    // See base class for documentation.
    symbol::pointer builtin_blockwrite_factory();

    // See base class for documentation.
    symbol::pointer builtin_chr_factory();

    // See base class for documentation.
    symbol::pointer builtin_close_factory();

    // See base class for documentation.
    symbol::pointer builtin_concat_factory();

    // See base class for documentation.
    symbol::pointer builtin_copy_factory();

    // See base class for documentation.
    symbol::pointer builtin_cos_factory();

    // See base class for documentation.
    symbol::pointer builtin_dec_factory();

    // See base class for documentation.
    symbol::pointer builtin_delete_factory();

    // See base class for documentation.
    symbol::pointer builtin_eof_factory();

    // See base class for documentation.
    symbol::pointer builtin_eoln_factory();

    // See base class for documentation.
    symbol::pointer builtin_exit_factory();

    // See base class for documentation.
    symbol::pointer builtin_exp_factory();

    // See base class for documentation.
    symbol::pointer builtin_fillchar_factory();

    // See base class for documentation.
    symbol::pointer builtin_get_factory();

    // See base class for documentation.
    symbol::pointer builtin_gotoxy_factory();

    // See base class for documentation.
    symbol::pointer builtin_halt_factory();

    // See base class for documentation.
    symbol::pointer builtin_idsearch_factory();

    // See base class for documentation.
    symbol::pointer builtin_inc_factory();

    // See base class for documentation.
    symbol::pointer builtin_insert_factory();

    // See base class for documentation.
    symbol::pointer builtin_ioresult_factory();

    // See base class for documentation.
    symbol::pointer builtin_length_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_ln_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_log_factory(void);

    // See base class for docuemntation.
    symbol::pointer builtin_mark_factory(void);

    // See base class for docuemntation.
    symbol::pointer builtin_memavail_factory(void);

    // See base class for documentation.
    symbol::pointer builtin_moveleft_factory();

    // See base class for documentation.
    symbol::pointer builtin_moveright_factory();

    // See base class for documentation.
    symbol::pointer builtin_new_factory();

    // See base class for documentation.
    symbol::pointer builtin_odd_factory();

    // See base class for documentation.
    symbol::pointer builtin_opennew_factory();

    // See base class for documentation.
    symbol::pointer builtin_openold_factory();

    // See base class for documentation.
    symbol::pointer builtin_ord_factory();

    // See base class for documentation.
    symbol::pointer builtin_page_factory();

    // See base class for documentation.
    symbol::pointer builtin_pos_factory();

    // See base class for documentation.
    symbol::pointer builtin_pred_factory();

    // See base class for documentation.
    symbol::pointer builtin_put_factory();

    // See base class for documentation.
    symbol::pointer builtin_pwroften_factory();

    // See base class for documentation.
    symbol::pointer builtin_read_factory();

    // See base class for documentation.
    symbol::pointer builtin_readln_factory();

    // See base class for documentation.
    symbol::pointer builtin_release_factory();

    // See base class for documentation.
    symbol::pointer builtin_reset_factory();

    // See base class for documentation.
    symbol::pointer builtin_rewrite_factory();

    // See base class for documentation.
    symbol::pointer builtin_round_factory();

    // See base class for documentation.
    symbol::pointer builtin_seek_factory();

    // See base class for documentation.
    symbol::pointer builtin_sin_factory();

    // See base class for documentation.
    symbol::pointer builtin_sqr_factory();

    // See base class for documentation.
    symbol::pointer builtin_sqrt_factory();

    // See base class for documentation.
    symbol::pointer builtin_str_factory();

    // See base class for documentation.
    symbol::pointer builtin_succ_factory();

    // See base class for documentation.
    symbol::pointer builtin_time_factory();

    // See base class for documentation.
    symbol::pointer builtin_trunc_factory();

    // See base class for documentation.
    symbol::pointer builtin_treesearch_factory();

    // See base class for documentation.
    symbol::pointer builtin_unitbusy_factory();

    // See base class for documentation.
    symbol::pointer builtin_unitclear_factory();

    // See base class for documentation.
    symbol::pointer builtin_unitread_factory();

    // See base class for documentation.
    symbol::pointer builtin_unitstatus_factory();

    // See base class for documentation.
    symbol::pointer builtin_unitwait_factory();

    // See base class for documentation.
    symbol::pointer builtin_unitwrite_factory();

    // See base class for documentation.
    symbol_function::pointer builtin_syscall_factory(const rcstring &name,
        int seg_num, int proc_num);

    // See base class for documentation.
    expression::pointer set2set_expression_factory(const type::pointer &to,
        const expression::pointer &from);

    // See base class for documentation.
    expression::pointer lint2lint_expression_factory(const type::pointer &to,
        const expression::pointer &from);

    // See base class for documentation.
    expression::pointer lint_from_integer_expression_factory(
        const expression::pointer &operand);

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
    expression::pointer writeln1_expression_factory(
        const expression::pointer &value);

    // See base class for docuemntation.
    expression::pointer writeln2_expression_factory(
        const expression::pointer &value, const expression::pointer &width);

    // See base class for docuemntation.
    expression::pointer writeln3_expression_factory(
        const expression::pointer &value, const expression::pointer &width,
        const expression::pointer &precision);

    // See base class for docuemntation.
    scope::pointer interface_scope_factory(const rcstring &name,
        int code_seg, int data_seg, segkind_t segkind);

    // See base class for docuemntation.
    scope::pointer record_scope_factory(bool is_packed);

    // See base class for docuemntation.
    expression::pointer nil_expression_factory(void);

    // See base class for docuemntation.
    statement::pointer for_statement_factory(
        const expression::pointer &var, const expression::pointer &limit1,
        bool descending, const expression::pointer &limit2,
        const statement::pointer &body);

    // See base class for docuemntation.
    statement::pointer case_statement_factory(
        const expression::pointer &control, const case_list &cases,
        const statement::pointer &otherwise);

    // See base class for docuemntation.
    statement::pointer goto_statement_factory(const symbol::pointer &sym);

    // See base class for docuemntation.
    statement::pointer label_statement_factory(const symbol::pointer &sym,
        const statement::pointer &body);

    // See base class for docuemntation.
    expression::pointer in_expression_factory(const expression::pointer &lhs,
        const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer pointer_eq_pointer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer array_eq_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer pointer_ne_pointer_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer paoc_from_string_expression_factory(
        const expression::pointer &arg, const type::pointer &tp);

    // See base class for docuemntation.
    expression::pointer array_ne_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer array_lt_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer array_le_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer array_gt_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer array_ge_array_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer record_eq_record_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer record_ne_record_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer set_eq_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer set_ne_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer set_le_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer set_lt_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer lint_lt_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer set_ge_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer lint_ge_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer set_gt_set_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer lint_gt_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    statement::pointer var_statement_factory(const variable_name_list &names,
        const type::pointer &tp);

    // See base class for docuemntation.
    statement::pointer var_statement_factory(const variable_name &name,
        const expression::pointer &value);

    // See base class for docuemntation.
    expression::pointer scan_expression_factory(
        const expression::pointer &maxdisp, int mask,
        const expression::pointer &value, const expression::pointer &start);

    // See base class for docuemntation.
    void unit_interface_text(const rcstring &text);

    // See base class for docuemntation.
    expression::pointer lint_plus_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer lint_minus_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer lint_times_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer lint_over_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer ternary_expression_factory(
        const expression::pointer &cond, const expression::pointer &then_branch,
        const expression::pointer &else_branch);

    // See base class for docuemntation.
    expression::pointer integer_from_lint_expression_factory(
        const expression::pointer &operand);

    // See base class for docuemntation.
    expression::pointer lint_eq_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    expression::pointer lint_ne_lint_expression_factory(
        const expression::pointer &lhs, const expression::pointer &rhs);

    // See base class for docuemntation.
    statement::pointer expression_statement_factory(
        const expression::pointer &arg);

    // See base class for docuemntation.
    statement::pointer empty_statement_factory(void);

    // See base class for docuemntation.
    symbol::pointer enumerated_symbol_factory(const variable_name &name,
        const type_enumerated::pointer &parent, int lex_level, int value);

public:
    /**
      * The print_token method is used to littoral print an output token.
      * It can be anything other than a space or a newline.  No line
      * wrapping occurs within a token.
      *
      * @param text
      *     The text of the token to be printed.
      */
    void print_token(const rcstring &text);

    /**
      * The print_space method is used to add a space to the list of
      * output tokens.  Line wrapping prefers to occur at spaces.
      */
    void print_space();

    /**
      * The print_eoln method is used to indicate a newline is to be
      * printed.  Line wrapping may occur at this point, for over-length
      * lines.  The right-most white space is the preferred place to
      * wrap, but failing that, the right-most inter-token point will be
      * used.
      *
      * @param extra_wrap_indent
      *     Usually, over-length lines are indented by one indent
      *     width.  This agument indicates how much *more* indenting
      *     should occur.  This is no way to request no indenting for
      *     over-length lines.
      */
    void print_eoln(int extra_wrap_indent = 0);

    /**
      * The indent_more method is used to indicate that nesting has
      * occurred, and that the indent depth should increase by the
      * indent width.
      */
    void indent_more();

    /**
      * The indent_less method is used to indicate that nesting has
      * finished, and that the indent depth should decrease by the
      * indent width.
      */
    void indent_less();

    /**
      * The indent reset is used to indicate the indenting should return
      * to zero, hard against the left margin.  Use sparingly.
      */
    void indent_reset();

private:
    /**
      * The os instance variable is used to remember the output stream on
      * which to write the formatted text.
      */
    output::pointer os;

    /**
      * The page_width instance variable is used to remember the maximum
      * number of characters wide the output is allowed to be.
      */
    int page_width;

    /**
      * The indent_width instance variable is used to remember how many
      * column to more right each time the indent increases.  Common
      * values are 2, 3, 4, 5 and 8.  Research suggests that 2, 3 or 4
      * are optimal.
      */
    int indent_width;

    /**
      * The indent_depth instance variable is used to remember the
      * current number of characters of indent that have accumulated.
      * Always a multiple of indent_width.
      */
    int indent_depth;

    /**
      * The line instance variable is used to remember the tokens which
      * comprise the current line being littoral printed.
      */
    rcstring_list line;

    /**
      * The undent instance variable is used to remember how many times
      * to "undent" at the next end-of-line.  This is deferred because
      * the code gets much easier to read when it happens tis way, plus
      * humans expect it.
      */
    int undent;

    bool have_printed_var_keyword;

    /**
      * The copy constructor.  Do not use.
      */
    translator_littoral(const translator_littoral &);

    /**
      * The assignment operator.  Do not use.
      */
    translator_littoral &operator=(const translator_littoral &);
};

#endif // UCSDPSYS_PRETTY_TRANSLATOR_PRETTY_H
// vim: set ts=8 sw=4 et :

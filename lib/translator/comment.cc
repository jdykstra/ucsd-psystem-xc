//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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
#include <lib/ac/ctype.h>
#include <lib/ac/string.h>

#include <lib/cardinal.h>
#include <lib/debug.h>
#include <lib/mtype.h>
#include <lib/pascal/lex.h>
#include <lib/rcstring/list.h>
#include <lib/sizeof.h>
#include <lib/tcontrol/bool/ambiguous.h>
#include <lib/tcontrol/bool/ignore.h>
#include <lib/tcontrol/bool/instance_variable.h>
#include <lib/tcontrol/bool/instance_variable_implicit.h>
#include <lib/tcontrol/bool/inst_var_not.h>
#include <lib/tcontrol/bool/method.h>
#include <lib/tcontrol/bool/method_implicit.h>
#include <lib/tcontrol/bool/method_not.h>
#include <lib/tcontrol/bool/method_not_implicit.h>
#include <lib/tcontrol/bool/variable.h>
#include <lib/tcontrol/int8min.h>
#include <lib/tcontrol/list.h>
#include <lib/tcontrol/noarg/ignore.h>
#include <lib/tcontrol/string/function.h>
#include <lib/tcontrol/string/ignore.h>
#include <lib/tcontrol/string/method.h>
#include <lib/tcontrol/too_late.h>
#include <lib/translator.h>


void
translator::comment(const rcstring &text)
{
    comment_notify(text);
    if (text[0] == '$')
        interpret_comment(text);
}


void
translator::interpret_comment(const rcstring &arg)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    assert(arg[0] == '$');
    comment_dispatcher_init();

    const char *cp = arg.c_str();
    const char *end = cp + arg.size();

    // skip the '$' character
    ++cp;

    // only as far as the first newline
    {
        const char *nl = (const char *)memchr(cp, '\n', end - cp);
        if (nl)
            end = nl;
    }
    rcstring text(cp, end - cp);
    DEBUG(1, "text = %s", text.quote_c().c_str());

    //
    // The copyright comment is an exception, because it can contain
    // comma characters.  But we try to treat it as unexceptionally as
    // possible.
    //
    if (cp[0] == 'c' || cp[0] == 'C')
    {
        ++cp;
        text = rcstring(cp, end - cp).trim();

        rcstring_list args;
        args.push_back("$");
        args.push_back("copyright");
        args.push_back(text);
        comment_dispatcher->set(args);
        DEBUG(1, "}");
        return;
    }

    rcstring_list controls;
    controls.split(text, ",", true);
    for (size_t j = 0; j < controls.size(); ++j)
    {
        rcstring cntl = controls[j];
        rcstring_list args;
        if
        (
            isalpha((unsigned char)cntl[0])
        &&
            ispunct((unsigned char)cntl[1])
        )
        {
            args.push_back(cntl.substring(0, 1));
            args.push_back(cntl.substring(1, cntl.size() - 1));
        }
        else
        {
            args.split(cntl);
        }
        args.push_front("$");
        comment_dispatcher->set(args);
    }
    DEBUG(1, "}");
}


void
translator::codefile_bigendian(bool)
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
translator::codefile_copyright(const rcstring &)
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
translator::comment_notify(const rcstring &)
{
    // Do nothing.
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
}


void
translator::interpret_comment_syscomp(bool yesno)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);

    //
    // This option sets the boolean variable SYSCOMP in the compiler
    // which is used by the compiler to determine whether this
    // compilation is a user program compilation, or a compilation of a
    // system program.
    //
    // The default is $U+ meaning "user program".
    //
    //
    // This gem is from the original User Manual:
    //
    //     The $U- option will generate programs that will not behave
    //     as expected.  Not recommended for non-systems work without
    //     knowing its method of operation.
    //
    // Completely uninformative.  The $U- mode is for compiling the
    // SYSTEM.PASCAL file.  This provides runtime support, and it
    // includes the command shell as well.  See the II.0 source code for
    // more information.
    //
    if (!yesno)
    {
        syscomp = false;
    }
    else
    {
        syscomp = true;
        feature_range_check_flag = false;
        goto_warning = false;
        iocheck = false;
    }
}


void
translator::comment_dispatcher_init(void)
{
    if (comment_dispatcher)
        return;
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    tcontrol_list::pointer tcl = tcontrol_list::create();
    comment_dispatcher = tcl;

    tcontrol::pointer big_endian =
        tcontrol_bool_method_implicit<translator>::create
        (
            this,
            &translator::codefile_bigendian
        );
    tcl->register_command("b", big_endian);
    tcl->register_command("big-endian", big_endian);

    tcontrol::pointer copyright_tcp =
        tcontrol_string_method<translator>::create
        (
            this,
            &translator::codefile_copyright
        );
    tcl->register_command("c", copyright_tcp);
    tcl->register_command("copyright", copyright_tcp);

    tcontrol::pointer breakpoints =
        tcontrol_too_late::create
        (
            this,
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::breakpoints_flag
            )
        );
    tcl->register_command("d", breakpoints);

    tcl->register_command("f", big_endian);

    tcontrol::pointer goto_n_tcp =
        tcontrol_bool_inst_var_not<translator>::create
        (
            this,
            &translator::goto_warning
        );
    tcl->register_command("g", goto_n_tcp);

    tcontrol::pointer iocheck_tcp =
        tcontrol_bool_instance_variable<translator>::create
        (
            this,
            &translator::iocheck
        );
    tcontrol::pointer include_tcp =
        tcontrol_string_function::create(pascal_lex_include);
    tcl->register_command("i",
        tcontrol_bool_ambiguous::create(iocheck_tcp, include_tcp));
    tcl->register_command("include", include_tcp);

    tcontrol::pointer listing_tcp = tcontrol_bool_ignore::create();
    tcl->register_command("l", listing_tcp);

    tcontrol::pointer noload_tcp = tcontrol_string_ignore::create();
    tcl->register_command("n", noload_tcp);
    tcl->register_command("noload", noload_tcp);
    tcontrol::pointer page_tcp = tcontrol_noarg_ignore::create();
    tcl->register_command("p", page_tcp);
    tcl->register_command("page", page_tcp);

    tcontrol::pointer quiet_tcp = tcontrol_bool_ignore::create();
    tcl->register_command("q", quiet_tcp);

    tcontrol::pointer resident_tcp = tcontrol_string_ignore::create();
    tcl->register_command("resident", resident_tcp);
    tcontrol::pointer range_check_tcp =
        tcontrol_bool_instance_variable<translator>::create
        (
            this,
            &translator::feature_range_check_flag
        );
    tcl->register_command("range-check", range_check_tcp);
    tcl->register_command("r",
        tcontrol_bool_ambiguous::create(range_check_tcp, resident_tcp));

    tcontrol::pointer swap_tcp =
        tcontrol_bool_instance_variable<translator>::create
        (
            this,
            &translator::feature_swapping_flag
        );
    tcl->register_command("s", swap_tcp);
    tcl->register_command("swap", swap_tcp);
    tcl->register_command("swapping", swap_tcp);

    tcontrol::pointer tiny_tcp =
        tcontrol_bool_instance_variable<translator>::create
        (
            this,
            &translator::tiny
        );
    tcl->register_command("t", tiny_tcp);

    tcontrol::pointer uses_tcp =
        tcontrol_string_method<translator>::create
        (
            this,
            &translator::codefile_library
        );
    tcl->register_command("uses", uses_tcp);
    tcontrol::pointer user_mode_tcp =
        tcontrol_bool_method_not<translator>::create
        (
            this,
            &translator::interpret_comment_syscomp
        );
    tcl->register_command("u",
        tcontrol_bool_ambiguous::create(user_mode_tcp, uses_tcp));

    //
    // These are the (*$feature ... *) control comments.
    //
    {
        tcontrol_list::pointer feature = tcontrol_list::create();
        tcl->register_command("feature", feature);

        // you can set the endian-ness from the mtype
        {
            tcontrol::pointer host_tc =
                tcontrol_string_method<translator>::create
                (
                    this,
                    &translator::comment_host
                );
            host_tc = tcontrol_too_late::create(this, host_tc);
            feature->register_command("host", host_tc);
        }
        {
            tcontrol::pointer arch_tc =
                tcontrol_string_method<translator>::create
                (
                    this,
                    &translator::comment_arch
                );
            arch_tc = tcontrol_too_late::create(this, arch_tc);
            feature->register_command("architecture", arch_tc);
            feature->register_command("machine", arch_tc);
        }

        feature->register_command("big-endian", big_endian);
        feature->register_command("breakpoints", breakpoints);
        feature->register_command
        (
            "chr-range-check",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_chr_range_check_flag
            )
        );
        feature->register_command("copyright", copyright_tcp);
        feature->register_command("debug", breakpoints);

        feature->register_command
        (
            "efj-nfj",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_efj_nfj_flag
            )
        );

        feature->register_command
        (
            "extra-set-comparisons",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_extra_set_comparisons_flag
            )
        );

        feature->register_command("flip-bytes", big_endian);

        feature->register_command
        (
            "ignore-undefined-segment-zero",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_ignore_undefined_segment_zero_flag
            )
        );

        feature->register_command
        (
            "inline-math",
            tcontrol_too_late::create
            (
                this,
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::feature_inline_math_flag
                )
            )
        );

        feature->register_command("iocheck", iocheck_tcp);
        feature->register_command("listing", listing_tcp);

        tcontrol::pointer little_endian =
            tcontrol_too_late::create
            (
                this,
                tcontrol_bool_method_not_implicit<translator>::create
                (
                    this,
                    &translator::codefile_bigendian
                )
            );
        feature->register_command("little-endian", little_endian);

        feature->register_command
        (
            "long-integer-constants",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_long_integer_constants_flag
            )
        );

        feature->register_command
        (
            "long-integer-extensions",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_long_integer_extensions_flag
            )
        );

        feature->register_command
        (
            "maximum-name-length",
            tcontrol_too_late::create
            (
                this,
                tcontrol_int8min::create(max_id_len)
            )
        );

        feature->register_command("quiet", quiet_tcp);
        feature->register_command("range-check", range_check_tcp);
        feature->register_command
        (
            "short-case",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::short_case_flag
            )
        );
        feature->register_command
        (
            "short-with",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_short_with_flag
            )
        );
        feature->register_command
        (
            "string-escapes",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::feature_string_escapes_flag
            )
        );
        feature->register_command("swapping", swap_tcp);
        feature->register_command("tiny", tiny_tcp);
        tcontrol::pointer syscomp_tcp =
            tcontrol_bool_method<translator>::create
            (
                this,
                &translator::interpret_comment_syscomp
            );
        feature->register_command("syscomp", syscomp_tcp);
        feature->register_command("system-compile", syscomp_tcp);

        feature->register_command
        (
            "underscore-significant",
            tcontrol_too_late::create
            (
                this,
                tcontrol_bool_variable::create(feature_underscore_significant)
            )
        );

        feature->register_command("user-mode", user_mode_tcp);
    }

    //
    // These are the (*$warning ... *) control comments.
    //
    {
        tcontrol_list::pointer warning = tcontrol_list::create();
        tcl->register_command("warning", warning);

        warning->register_command
        (
            "address-of",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::address_of_warning
            )
        );

        warning->register_command
        (
            "constant-branch",
            tcontrol_bool_instance_variable<translator>::create
            (
                this,
                &translator::warning_constant_branch_flag
            )
        );

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable_implicit<translator>::create
                (
                    this,
                    &translator::warnings_are_errors_flag
                );
            warning->register_command("error", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::empty_parens_warning
                );
            warning->register_command("empty-parentheses", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::warning_binary_constants_flag
                );
            warning->register_command("binary-constants", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::goto_warning
                );
            warning->register_command("goto", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::warning_hex_constants_flag
                );
            warning->register_command("hex-constants", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::warning_named_label_flag
                );
            warning->register_command("named-labels", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::warning_otherwise_flag
                );
            warning->register_command("otherwise", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::warning_parameter_string_size_flag
                );
            warning->register_command("parameter-string-size", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::warning_set_comparisons_flag
                );
            warning->register_command("set-comparisons", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable<translator>::create
                (
                    this,
                    &translator::warning_shadow_flag
                );
            warning->register_command("shadow", tcp);
        }

        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable_implicit<translator>::create
                (
                    this,
                    &translator::warnings_are_silent_flag
                );
            warning->register_command("silent", tcp);
        }
        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable_implicit<translator>::create
                (
                    this,
                    &translator::warning_ternary_expression_flag
                );
            warning->register_command("ternary-expression", tcp);
        }
        {
            tcontrol::pointer tcp =
                tcontrol_bool_instance_variable_implicit<translator>::create
                (
                    this,
                    &translator::warning_unreachable_flag
                );
            warning->register_command("unreachable", tcp);
        }
    }
    DEBUG(1, "}");
}


void
translator::comment_arch(const rcstring &value)
{
    mtype_t mt = mtype_from_name(value);
    if (mt == mtype_undefined)
    {
        mt = mtype_from_name_fuzzy(value);
        if (mt == mtype_undefined)
        {
            pascal_lex_error
            (
                pascal_lex_location(),
                "architecture %s unknown",
                value.quote_c().c_str()
            );
            return;
        }
        pascal_lex_error
        (
            pascal_lex_location(),
            "architecture %s unknown, guessing you meant %s instead",
            value.quote_c().c_str(),
            mtype_name(mt).quote_c().c_str()
        );
        return;
    }

    //
    // If the machine type is big_endian, let the translator know.
    //
    // The code file only needs to know if it is to be big-endian or
    // little-endian, it doesn't need the actual mtype, because it
    // doesn't emit any native code procedures.
    //
    codefile_bigendian(byte_sex_from_mtype(mt) == big_endian);
}


void
translator::comment_host(const rcstring &value)
{
    mtype_t mt = mtype_from_host(value);
    if (mt == mtype_undefined)
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "host %s unknown",
            value.quote_c().c_str()
        );
        return;
    }

    //
    // If the machine type is big_endian, let the translator know.
    //
    // The code file only needs to know if it is to be big-endian or
    // little-endian, it doesn't need the actual mtype, because it
    // doesn't emit any native code procedures.
    //
    codefile_bigendian(byte_sex_from_mtype(mt) == big_endian);
}


// vim: set ts=8 sw=4 et :

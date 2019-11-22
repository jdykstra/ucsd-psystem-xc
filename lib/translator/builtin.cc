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

#include <lib/bit_address.h>
#include <lib/pcode.h>
#include <lib/scope.h>
#include <lib/symbol/function.h>
#include <lib/translator.h>
#include <lib/type/anything.h>
#include <lib/type/boolean.h>
#include <lib/type/char.h>
#include <lib/type/file.h>
#include <lib/type/integer.h>
#include <lib/type/long_integer/unsized.h>
#include <lib/type/nothing.h>
#include <lib/type/pointer/unpacked.h>
#include <lib/type/real.h>
#include <lib/type/reference.h>
#include <lib/type/string.h>


void
translator::define_builtin_symbols(void)
{
    // This can get called if there is an embedded unit defined
    // within a (usually dummy) outer shell (*$U-*) program.
    if (builtins)
    {
        assert(scopes.size() >= 1);
        return;
    }
    builtins = builtin_scope_factory();
    assert(builtins);
    assert(scopes.size() == 0);
    scopes.push_back(builtins);

    location nowhere(location::builtin());

    class restore_flag
    {
    public:
        ~restore_flag() { flag = orig; }
        restore_flag(bool &a_flag) : flag(a_flag), orig(a_flag) { }
    private:
        bool &flag;
        bool orig;
    };

    // No matter how we leave this function, syscomp will always be
    // restored.  It's even exception safe.
    restore_flag on_the_way_out(syscomp);

    // By setting syscomp to true, all the symbols defined here get a
    // lex-level of -1.  Meaning the user will get a warning, not an
    // error, if they define symbols over the top of them.
    syscomp = true;
    assert(get_lex_level() == -1);

    //
    // By defining these as symbols, we can catch simple mis-spellings
    // and provide better error messages.  The ones in the lexer and
    // grammar will always take precedence, however.
    //

    //
    // Types
    //
    // Keep these sorted by name.
    //
    type_declaration(variable_name("boolean", nowhere),
        type_boolean::create());
    type_declaration(variable_name("char", nowhere),
        type_char::create());
    type_declaration(variable_name("integer", nowhere),
        type_integer::create());
    type_declaration(variable_name("interactive", nowhere),
        type_file::create(type_char::create_interactive()));
    type_declaration(variable_name("real", nowhere),
        type_real::create());
    type_declaration(variable_name("string", nowhere),
        type_string::create());
    type_declaration(variable_name("text", nowhere),
        type_file::create(type_char::create()));

    {
        //
        // The UCSD compiler has a disgusting hack for the CLOSE
        // function.  If there is a second parameter it looks for four
        // specific identifiers.  That is, it fakes an enumerated type.
        // See compiler/bodypart.b.text around line 290.
        //
        // Rather than clutter the grammar further (and all the fall-out
        // that would entail) we don't fake it, we create a real
        // enumerated type.
        //
        // Yes, it is possible for users to re-define any of these four
        // symbols, potentially rendering a "valid" program invalid, but
        // if that happens
        //   1. a shadow warning will be emitted
        //   2. close will emit an error message
        // Which is to say, it will not be silently broken, and a good
        // indication of how to fix the problem is produced.
        //
        // See ucsdpsys_compile/symbol/function/close.cc for where this
        // enumerated type is checked.
        //
        type::pointer tp = enumerated_begin();
        enumerated_tag(variable_name("normal", nowhere));
        enumerated_tag(variable_name("lock", nowhere));
        enumerated_tag(variable_name("purge", nowhere));
        enumerated_tag(variable_name("crunch", nowhere));
        enumerated_end();
        type_declaration(variable_name("sys:closetype", nowhere), tp);
    }

    //
    // Constants
    //
    // Keep these sorted by name.
    //
    expression::pointer ep = boolean_expression_factory(nowhere, false);
    constant_declaration(variable_name("false", nowhere), ep);

    ep = integer_expression_factory(nowhere, "32767");
    constant_declaration(variable_name("maxint", nowhere), ep);

    ep = nil_expression_factory();
    constant_declaration(variable_name("nil", nowhere), ep);

    ep = boolean_expression_factory(nowhere, true);
    constant_declaration(variable_name("true", nowhere), ep);

    //
    // Builtins that turn into opcodes
    //
    // Keep these sorted by name.
    //
    builtins->define(builtin_abs_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_arctan_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_atan_factory());
    builtins->define(builtin_blockread_factory());
    builtins->define(builtin_blockwrite_factory());
    builtins->define(builtin_chr_factory());
    builtins->define(builtin_close_factory());
    builtins->define(builtin_concat_factory());
    if (!tiny)
        builtins->define(builtin_copy_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_cos_factory());
    builtins->define(builtin_dec_factory());
    if (!tiny)
        builtins->define(builtin_delete_factory());
    builtins->define(builtin_eof_factory());
    builtins->define(builtin_eoln_factory());
    builtins->define(builtin_exit_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_exp_factory());
    builtins->define(builtin_fillchar_factory());
    if (!tiny)
        builtins->define(builtin_get_factory());
    if (!tiny)
        builtins->define(builtin_gotoxy_factory());
    builtins->define(builtin_halt_factory());
    builtins->define(builtin_idsearch_factory());
    builtins->define(builtin_inc_factory());
    if (!tiny)
        builtins->define(builtin_insert_factory());
    builtins->define(builtin_ioresult_factory());
    builtins->define(builtin_length_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_ln_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_log_factory());
    builtins->define(builtin_mark_factory());
    builtins->define(builtin_memavail_factory());
    builtins->define(builtin_moveleft_factory());
    builtins->define(builtin_moveright_factory());
    builtins->define(builtin_new_factory());
    builtins->define(builtin_odd_factory());
    builtins->define(builtin_opennew_factory());
    builtins->define(builtin_openold_factory());
    builtins->define(builtin_ord_factory());
    if (!tiny)
        builtins->define(builtin_page_factory());
    if (!tiny)
        builtins->define(builtin_pos_factory());
    if (!tiny)
        builtins->define(builtin_pred_factory());
    if (!tiny)
        builtins->define(builtin_put_factory());
    builtins->define(builtin_pwroften_factory());
    builtins->define(builtin_read_factory());
    if (!tiny)
        builtins->define(builtin_readln_factory());
    builtins->define(builtin_release_factory());
    builtins->define(builtin_reset_factory());
    builtins->define(builtin_rewrite_factory());
    builtins->define(builtin_round_factory());
    if (!tiny)
        builtins->define(builtin_seek_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_sin_factory());
    if (!tiny)
        builtins->define(builtin_sqr_factory());
    if (feature_inline_math_flag)
        builtins->define(builtin_sqrt_factory());
    if (!tiny)
        builtins->define(builtin_str_factory());
    if (!tiny)
        builtins->define(builtin_succ_factory());
    builtins->define(builtin_time_factory());
    builtins->define(builtin_treesearch_factory());
    builtins->define(builtin_trunc_factory());
    builtins->define(builtin_unitbusy_factory());
    builtins->define(builtin_unitclear_factory());
    if (!tiny)
        builtins->define(builtin_unitread_factory());
    if (!tiny)
        builtins->define(builtin_unitstatus_factory());
    if (!tiny)
        builtins->define(builtin_unitwait_factory());
    builtins->define(builtin_unitwrite_factory());

    //
    // If you thought CLOSE was fubar, you haven't met EXIT yet.
    //
    // This particular built-in symbol supports the gobsmackingly
    // inconsistent
    //
    //     EXIT(PROGRAM)
    //
    // statement.  It amazes me how much crap they managed to stuff
    // into the grammar without feeling dirty.  Elegance comes from
    // simplicity, guys: simplicity.
    //
    {
        rcstring name = "program";
        int seg_num = 1;
        int proc_num = 1;

        name_type_list args;
        type::pointer tp =
            type_pointer_unpacked::create(type_anything::create());
        variable_name_list n1;
        n1.push_back(variable_name("ignore1", nowhere));
        args.push_back(name_type(n1, tp));
        variable_name_list n2;
        n2.push_back(variable_name("ignore2", nowhere));
        args.push_back(name_type(n2, tp));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        builtins->define(sp);
    }

    //
    // The implicit first parameters of READLN and WRITELN are not the
    // two implicit parameters to the program, INPUT and OUTPUT as
    // documented.  They are still called INPUT and OUTPUT, but they are
    // not the parameters to the program.
    //
    // The UCSD compiler actually references the system variables at lex
    // level -1.  Look in system/globals.text (near line 374) in the
    // II.0 sources and you will find the following declarations...
    //
    //   VAR
    //     SYSCOM: ^SYSCOMREC;
    //     GFILES: ARRAY [0..5] OF FIBP; (*GLOBAL FILES, 0=INPUT, 1=OUTPUT*)
    //
    // It's an array of pointers, so each is 1 word wide.  This has the
    // nice property that a compiler doesn't need to accurately know
    // the sizeof(FIB)... all we do know is it's <= sizeof(file of char)
    //
    {
        // Actually filler so the next two are in the right place.
        int lex_level = -1;
        type::pointer tp =
            type_pointer_unpacked::create(type_anything::create());
        symbol_variable::pointer sp =
            builtins->variable_factory
            (
                variable_name("sys:com", nowhere),
                tp,
                lex_level
            );
        builtins->define(sp);
    }
    {
        int lex_level = -1;
        type::pointer tp =
            type_reference::create(type_file::create(type_char::create()));
        symbol_variable::pointer sp =
            builtins->variable_factory
            (
                variable_name("input", nowhere),
                tp,
                lex_level
            );
        builtins->define(sp);
    }
    {
        int lex_level = -1;
        type::pointer tp =
            type_reference::create(type_file::create(type_char::create()));
        symbol_variable::pointer sp =
            builtins->variable_factory
            (
                variable_name("output", nowhere),
                tp,
                lex_level
            );
        builtins->define(sp);
    }
    {
        int lex_level = -1;
        type::pointer tp =
            type_reference::create
            (
                type_file::create(type_char::create_interactive())
            );
        symbol_variable::pointer sp =
            builtins->variable_factory
            (
                variable_name("keyboard", nowhere),
                tp,
                lex_level
            );
        builtins->define(sp);
    }

    //
    // procedure execerror(*void*);
    // (Prints the most recent execerror on the terminal.)
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:execerror";
        int seg_num = 0;
        int proc_num = CXP_0_EXECERROR;
        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure finit(var f: file; window: ^void; recwords: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:finit";
        int seg_num = 0;
        int proc_num = CXP_0_FINIT;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("window", nowhere));
        type::pointer t2 = type_reference::create(type_anything::create());
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("recwords", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure freset(var f: file);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:freset";
        int seg_num = 0;
        int proc_num = CXP_0_FRESET;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fopen(var f: file; var ftitle: string; fopenold: boolean;
    //     junk: ^anything);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fopen";
        int seg_num = 0;
        int proc_num = CXP_0_FOPEN;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("ftitle", nowhere));
        type::pointer t2 = type_reference::create(type_string::create(255));
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("fopenold", nowhere));
        type::pointer t3 = type_boolean::create();
        args.push_back(name_type(n3, t3));

        variable_name_list n4;
        n4.push_back(variable_name("junk", nowhere));
        type::pointer t4 =
            type_pointer_unpacked::create(type_anything::create());
        args.push_back(name_type(n4, t4));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fclose(var f: file; ftype: closetype);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fclose";
        int seg_num = 0;
        int proc_num = CXP_0_FCLOSE;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        // See the II.0 sources.  This is actually "ftype: closetype"
        // but that enumeration, and its members, are not available.
        variable_name_list n2;
        n2.push_back(variable_name("ftype", nowhere));
        type::pointer t2 = type_integer::create();
        args.push_back(name_type(n2, t2));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fget(var f: file of anything);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fget";
        int seg_num = 0;
        int proc_num = CXP_0_FGET;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fput(var f: file);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fput";
        int seg_num = 0;
        int proc_num = CXP_0_FPUT;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure xseek(*void*);
    //
    // This one was made into a dummy, so that the floating-point stuff
    // could be moved to segment 31.
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:xseek";
        int seg_num = 0;
        int proc_num = CXP_0_XSEEK;

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fseek(var f: file; recnum: integer);
    //
    // See pio_unit/main.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fseek";
        int seg_num = 31;
        int proc_num = CXP_31_FSEEK;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("recnum", nowhere));
        type::pointer t2 = type_integer::create();
        args.push_back(name_type(n2, t2));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // function feof(var f: file): boolean;
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:feof";
        int seg_num = 0;
        int proc_num = CXP_0_FEOF;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        sp->attach_return_type(type_boolean::create());
        builtins->define(sp);
    }

    //
    // function feoln(var f: file): boolean;
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:feoln";
        int seg_num = 0;
        int proc_num = CXP_0_FEOLN;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        sp->attach_return_type(type_boolean::create());
        builtins->define(sp);
    }

    //
    // procedure freaddec(var f: file; var value; size: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:freaddec";
        int seg_num = DECOPS_SEG_NUM;
        int proc_num = DECOPS_FREADDEC;
        name_type_list args;

        {
            variable_name_list n1;
            n1.push_back(variable_name("f", nowhere));
            type::pointer t1 =
                type_reference::create
                (
                    type_file::create(type_anything::create())
                );
            args.push_back(name_type(n1, t1));
        }

        {
            variable_name_list n2;
            n2.push_back(variable_name("value", nowhere));
            type::pointer t2 = type_reference::create(type_anything::create());
            args.push_back(name_type(n2, t2));
        }

        {
            variable_name_list n3;
            n3.push_back(variable_name("size", nowhere));
            type::pointer t3 = type_integer::create();
            args.push_back(name_type(n3, t3));
        }

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure freadint(var f: file; var value: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:freadint";
        int seg_num = 0;
        int proc_num = CXP_0_FREADINT;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_reference::create(type_integer::create());
        args.push_back(name_type(n2, t2));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fwriteint(var f: file; value: integer; width: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fwriteint";
        int seg_num = 0;
        int proc_num = CXP_0_FWRITEINT;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_integer::create();
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("width", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fwritedec(var f: file; value: integer[36]; width: integer);
    //
    // See long_integers/pascalio.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fwritedec";
        int seg_num = DECOPS_SEG_NUM;
        int proc_num = DECOPS_FWRITEDEC;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_long_integer_unsized::create(36);
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("width", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure xreadreal(*void*);
    //
    // This one was made into a dummy, so that the floating-point stuff
    // could be moved to segment 31.
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:xreadreal";
        int seg_num = 0;
        int proc_num = CXP_0_XREADREAL;

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure freadreal(var f: file; var value: real);
    //
    // See pio_unit/main.text in the II.0 sources.
    //
    {
        rcstring name = "sys:freadreal";
        int seg_num = 31;
        int proc_num = CXP_31_FREADREAL;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_reference::create(type_real::create());
        args.push_back(name_type(n2, t2));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure xwritereal(*void*);
    //
    // This one was made into a dummy, so that the floating-point stuff
    // could be moved to segment 31.
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:xwritereal";
        int seg_num = 0;
        int proc_num = CXP_0_XWRITEREAL;

       symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fwritereal(var f: file; value: real; width: integer;
    //     precision: integer);
    //
    // See pio_unit/main.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fwritereal";
        int seg_num = 31;
        int proc_num = CXP_31_FWRITEREAL;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_real::create();
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("width", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        variable_name_list n4;
        n4.push_back(variable_name("precision", nowhere));
        type::pointer t4 = type_integer::create();
        args.push_back(name_type(n4, t4));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure freadchar(var f: file; var value: char);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:freadchar";
        int seg_num = 0;
        int proc_num = CXP_0_FREADCHAR;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_reference::create(type_char::create());
        args.push_back(name_type(n2, t2));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fwritechar(var f: file; value: char; width: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fwritechar";
        int seg_num = 0;
        int proc_num = CXP_0_FWRITECHAR;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_char::create();
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("width", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure freadstring(var f: file; var s: string; sleng: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:freadstring";
        int seg_num = 0;
        int proc_num = CXP_0_FREADSTRING;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("s", nowhere));
        type::pointer t2 = type_reference::create(type_string::create(255));
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("sleng", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fwritestring(var f: file; var value: string; width: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fwritestring";
        int seg_num = 0;
        int proc_num = CXP_0_FWRITESTRING;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("value", nowhere));
        type::pointer t2 = type_string::create(255);
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("width", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fwritebytes(var f: file; var a: anything; rleng: integer;
    //     aleng: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fwritebytes";
        int seg_num = 0;
        int proc_num = CXP_0_FWRITEBYTES;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("a", nowhere));
        type::pointer t2 = type_reference::create(type_anything::create());
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("rleng", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        variable_name_list n4;
        n4.push_back(variable_name("aleng", nowhere));
        type::pointer t4 = type_integer::create();
        args.push_back(name_type(n4, t4));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure freadln(var f: file);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:freadln";
        int seg_num = 0;
        int proc_num = CXP_0_FREADLN;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure fwriteln(var f: file);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:fwriteln";
        int seg_num = 0;
        int proc_num = CXP_0_FWRITELN;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure sconcat(var dest: string; var src: string; destleng: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:sconcat";
        int seg_num = 0;
        int proc_num = CXP_0_SCONCAT;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("dest", nowhere));
        type::pointer t1 = type_reference::create(type_string::create(255));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("src", nowhere));
        type::pointer t2 = t1;
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("destleng", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure sinsert(var src: string; var dest: string; destleng: integer;
    //     insinx: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:sinsert";
        int seg_num = 0;
        int proc_num = CXP_0_SINSERT;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("src", nowhere));
        type::pointer t1 = type_reference::create(type_string::create(255));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("dest", nowhere));
        type::pointer t2 = t1;
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("destleng", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        variable_name_list n4;
        n4.push_back(variable_name("insinx", nowhere));
        type::pointer t4 = type_integer::create();
        args.push_back(name_type(n4, t4));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure scopy(var src: string; var dest: string; srcinx: integer;
    //     copyleng: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:scopy";
        int seg_num = 0;
        int proc_num = CXP_0_SCOPY;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("src", nowhere));
        type::pointer t1 = type_reference::create(type_string::create(255));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("dest", nowhere));
        type::pointer t2 = t1;
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("srcinx", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        variable_name_list n4;
        n4.push_back(variable_name("copylength", nowhere));
        type::pointer t4 = t3;
        args.push_back(name_type(n4, t4));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // procedure sdelete(var dest: string; delinx: integer; delleng: integer);
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:sdelete";
        int seg_num = 0;
        int proc_num = CXP_0_SDELETE;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("dest", nowhere));
        type::pointer t1 = type_reference::create(type_string::create(255));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("delinx", nowhere));
        type::pointer t2 = type_integer::create();
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("delleng", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }

    //
    // function spos(var target: string; var src: string): integer;
    //
    // See system/globals.text in the II.0 sources.
    //
    {
        rcstring name = "sys:spos";
        int seg_num = 0;
        int proc_num = CXP_0_SPOS;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("target", nowhere));
        type::pointer t1 = type_reference::create(type_string::create(255));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("src", nowhere));
        type::pointer t2 = t1;
        args.push_back(name_type(n2, t2));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        sp->attach_return_type(type_integer::create());
        builtins->define(sp);
    }

    //
    // function fblockio(var f: file; var a: ^void; i: integer;
    //     nblocks: integer; rblock: integer; doread: boolean): integer;
    //
    {
        rcstring name = "sys:fblockio";
        int seg_num = 0;
        int proc_num = CXP_0_FBLOCKIO;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("f", nowhere));
        type::pointer t1 =
            type_reference::create(type_file::create(type_anything::create()));
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("a", nowhere));
        type::pointer t2 = type_reference::create(type_anything::create());
        args.push_back(name_type(n2, t2));

        variable_name_list n3;
        n3.push_back(variable_name("i", nowhere));
        type::pointer t3 = type_integer::create();
        args.push_back(name_type(n3, t3));

        variable_name_list n4;
        n4.push_back(variable_name("nblocks", nowhere));
        type::pointer t4 = type_integer::create();
        args.push_back(name_type(n4, t4));

        variable_name_list n5;
        n5.push_back(variable_name("rblock", nowhere));
        type::pointer t5 = type_integer::create();
        args.push_back(name_type(n5, t5));

        variable_name_list n6;
        n6.push_back(variable_name("doread", nowhere));
        type::pointer t6 = type_boolean::create();
        args.push_back(name_type(n6, t6));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        sp->attach_return_type(type_integer::create());
        builtins->define(sp);
    }

    //
    // procedure fgotoxy(x: integer; y: integer);
    //
    {
        rcstring name = "sys:fgotoxy";
        int seg_num = 0;
        int proc_num = CXP_0_FGOTOXY;
        name_type_list args;

        variable_name_list n1;
        n1.push_back(variable_name("x", nowhere));
        type::pointer t1 = type_integer::create();
        args.push_back(name_type(n1, t1));

        variable_name_list n2;
        n2.push_back(variable_name("y", nowhere));
        type::pointer t2 = t1;
        args.push_back(name_type(n2, t2));

        symbol_function::pointer sp =
            builtin_syscall_factory(name, seg_num, proc_num);
        sp->attach_function_parameters(args);
        // sp->attach_return_type(type_nothing::create());
        builtins->define(sp);
    }
}


// vim: set ts=8 sw=4 et :

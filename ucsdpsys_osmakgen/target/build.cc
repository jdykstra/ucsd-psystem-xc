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

#include <lib/ac/stdarg.h>
#include <lib/ac/string.h>
#include <libexplain/output.h>

#include <lib/cardinal.h>
#include <lib/pascal_id.h>
#include <lib/path_join.h>
#include <lib/sizeof.h>

#include <ucsdpsys_osmakgen/collect.h>
#include <ucsdpsys_osmakgen/target/build.h>


target_build::~target_build()
{
}


struct libseg_table_t
{
    const char *name;
    int number;
};

static const libseg_table_t libseg_table[] =
{
    { "apple_stuff", 22 },
    { "chain_stuff", 28 },
    { "long_integer", 30 },
    { "long_intiger", 30 }, // spelling mistake in original system
    { "pascal_io", 31 },
    { "transcendental", 29 },
    { "turtle_graphics", 20 },
    { "turtle_graphics", 21 }, // data segment
};


static int
library_segnum(const rcstring &name)
{
    for
    (
        const libseg_table_t *tp = libseg_table;
        tp < ENDOF(libseg_table);
        ++tp
    )
    {
        if (pascal_id(name) == pascal_id(tp->name))
            return tp->number;
    }
    return -1;
}


target_build::target_build(const rcstring &outfile) :
    target(outfile),
    blurb(true),
    default_host("klebsch"),
    max_stage(1)
{
    // DO NOT do this to system.code or diagnostic.code, they are
    // required to have system segments in them.
    remove_system_segments.push_back("assembler");
    remove_system_segments.push_back("basic");
    remove_system_segments.push_back("compiler");
    remove_system_segments.push_back("editor");
    remove_system_segments.push_back("execerror");
    remove_system_segments.push_back("filer");
    remove_system_segments.push_back("librarian");
    remove_system_segments.push_back("linker");
    remove_system_segments.push_back("long_integer");
    remove_system_segments.push_back("pascalio");
    remove_system_segments.push_back("setup");
    remove_system_segments.push_back("yaloe");

    system_candidates.push_back("interpreter"); // system.$(host)
    system_candidates.push_back("assembler");   // system.assmbler
    // part of the assembler                    // $(arch).errors
    // part of the assembler                    // $(arch).opcodes
    // part of (turtle) graphics                // system.charset
    system_candidates.push_back("compiler");    // system.compiler
    system_candidates.push_back("editor");      // system.editor
    system_candidates.push_back("filer");       // system.filer
    // partially done                           // system.library    <--
    system_candidates.push_back("linker");      // system.linker
    // done                                     // system.miscinfo
    system_candidates.push_back("system");      // system.pascal
    // part of the compiler                     // system.syntax
    // FIXME: part of the disassembler          // opcodes.ii.0      <--

    for
    (
        const libseg_table_t *tp = libseg_table;
        tp < ENDOF(libseg_table);
        ++tp
    )
    {
        library_candidates.push_back_unique(tp->name);
        library_candidates.push_back_unique(pascal_id(tp->name));
    }

    remember_dist_clean_file("config.log");
    remember_dist_clean_file("config.status");
}


void
target_build::no_blurb(void)
{
    blurb = false;
}


void
target_build::set_default_host(const rcstring &text)
{
    default_host = text;
}


void
target_build::set_default_arch(const rcstring &text)
{
    default_arch = text;
}


void
target_build::set_notice(const rcstring &text)
{
    notice = text;
}


static rcstring
name_mangle(const rcstring &stem)
{
    rcstring name = stem.basename();
    if (name == "interpreter.code")
        return "system.$(host)";
    if (name == "assembler.code")
        return "system.assmbler"; // note abbeviation
    if (name == "compiler.code")
        return "system.compiler";
    if (name == "editor.code")
        return "system.editor";
    if (name == "filer.code")
        return "system.filer";
    if (name == "linker.code")
        return "system.linker";
    if (name == "system.code")
        return "system.pascal";
    if (name.length() > 15)
    {
        const char *s = name.c_str();
        const char *dot = strrchr(s, '.');
        if (!dot || s + name.size() - dot > 15)
            return name.substring(0, 15);
        rcstring lhs(s, dot - s);
        rcstring rhs(dot);
        return lhs.substring(0, 15 - rhs.size()) + rhs;
    }
    return name;
}


bool
target_build::mkdir_p(const rcstring &)
{
    return true;
}


void
target_build::clean_file(const rcstring &path)
{
    clean_files.push_back_unique(path);
}


void
target_build::clean_dir(const rcstring &path)
{
    clean_dirs.push_back_unique(path);
}


void
target_build::stage1_compile(const rcstring &prog, bool maybe_globals,
    bool has_asm)
{
    remember_dist_clean_dir("stage1");

    rcstring src = prog + "/main.text";
    rcstring obj =
        "stage1/" + load("host") + "/codefiles/" + prog + "/main.code";
    rcstring pas_obj =
        "stage1/" + load("host") + "/codefiles/" + prog + "/main.pas.code";
    rcstring asm_obj =
        "stage1/" + load("host") + "/codefiles/" + prog + "/main.asm.code";

    // rule target
    rcstring rtarget = (has_asm ? pas_obj : obj);
    // rule ingreedients
    rcstring_list ingredients;
    {
        rcstring command = "ucsdpsys_depends -I" + prog;
        command += " -J. -Jbl -Jblbl -Jblblbl";
        command += " -L -r " + prog + "/main.text";
        collect_output_of(command, ingredients);
    }
    ingredients.push_back_unique(src);
    ingredients.sort();

    // rule body
    rcstring_list body;
    bool rss = remove_system_segments.member(prog.basename());

    rcstring cmd = "ucsdpsys_compile " + load("XCFLAGS");
    if (rss)
        cmd += " -fignore-undefined-segment-zero";
    rcstring eif = extra_include_flags();
    if (!eif.empty())
        cmd += " " + eif;
    cmd += " -I" + prog + " -o " + rtarget + " " + resolve(src);
    body.push_back(cmd);
    if (rss)
    {
        cmd = "ucsdpsys_librarian --file=" + rtarget
            + " --remove-system-segments";
        body.push_back(cmd);
    }

    rcstring host;
    if (prog.starts_with("arch/"))
    {
        rcstring_list components;
        components.split(prog, "/");
        host = components[1];
    }

    // write the rule
    if (!host.empty())
    {
        op->fputc('\n');
        cond_if_eq(load("host"), host);
    }
    write_rule(rtarget, ingredients, body);
    if (!host.empty())
        cond_endif();
    clean_file(rtarget);

    if (ingredients.member(prog + "/vstamp.inc.text"))
    {
        // This file is a copy of the project version stamp, found
        // in the "etc" directory.
        rcstring f1 = "etc/vstamp.inc.text";
        rcstring f2 = prog + "/vstamp.inc.text";
        simple_copy_rule(f1, f2);
    }

    //
    // Rather than have multiple copies of globals.text, have the
    // Makefile make the copies, and use just the one master file.
    //
    if (maybe_globals && ingredients.member(prog + "/globals.text"))
    {
        rcstring f1 = "system/globals.text";
        rcstring f2 = prog + "/globals.text";
        simple_copy_rule(f1, f2);
    }

    if (has_asm)
    {
        rtarget = obj;
        ingredients.clear();
        ingredients.push_back(asm_obj);
        ingredients.push_back(pas_obj);
        body.clear();
        cmd = "ucsdpsys_link -o " + obj + " " + resolve(pas_obj) + " "
            + resolve(asm_obj);
        body.push_back(cmd);
        write_rule(rtarget, ingredients, body);
        clean_file(rtarget);
    }
}


void
target_build::stage1_assemble(const rcstring &prog)
{
    rcstring progbase = prog.basename();
    rcstring src = prog + "/main.asm.text";
    rcstring obj = "stage1/" + load("host") + "/codefiles/" + progbase
        + "/main.asm.code";

    // Track down the include dependencies.
    rcstring command = "ucsdpsys_depends --assemble -a -I" + prog;
    command += " -J. -Jbl -Jblbl -Jblblbl";
    command += " -L -r " + prog + "/main.asm.text";
    rcstring_list depends;
    collect_output_of(command, depends);
    depends.push_back_unique(src);
    depends.sort();

    // Write the rule
    {
        rcstring host;
        if (prog.starts_with("arch/"))
        {
            rcstring_list components;
            components.split(prog, "/");
            host = components[1];
        }

        rcstring_list body;
        rcstring cmd = "ucsdpsys_assemble " + load("XAFLAGS") + " -I" + prog;
        rcstring eif = extra_include_flags();
        if (!eif.empty())
            cmd += " " + eif;
        cmd += " -o " + obj + " " + resolve(src);
        body.push_back(cmd);

        if (!host.empty())
        {
            op->fputc('\n');
            cond_if_eq(load("host"), host);
        }
        write_rule(obj, depends, body);
        if (!host.empty())
            cond_endif();

        clean_file(obj);
    }

    if (depends.member(prog + "/vstamp.asm.text"))
    {
        // This file is a copy of the project version stamp, found
        // in the "etc" directory.
        rcstring f1 = "etc/vstamp.asm.text";
        rcstring f2 = prog + "/vstamp.asm.text";
        simple_copy_rule(f1, f2);
    }
}


void
target_build::simple_copy_rule(const rcstring &src, const rcstring &dst)
{
    rcstring dst_dir = dst.dirname();
    rcstring_list ingredients;
    ingredients.push_back(src);
    rcstring_list body;
    body.push_back("cp " + resolve(src) + " " + dst);
    write_rule(dst, ingredients, body);
    clean_file(dst);
}


void
target_build::simple_install_rule(const rcstring &src, const rcstring &dst)
{
    rcstring dst_dir = dst.dirname();
    rcstring_list ingredients;
    ingredients.push_back(src);
    rcstring_list body;
    body.push_back("cp " + resolve(src) + " " + dst);
    write_rule(dst, ingredients, body);
    uninstall_files.push_back_unique(dst);
}


static rcstring
finder(const rcstring_list &filenames, const rcstring_list &dir_list,
    const rcstring_list &base_list, const rcstring &extn)
{
    for (size_t j = 0; j < base_list.size(); ++j)
    {
        rcstring base0 = base_list[j];
        rcstring base = base0 + extn;
        if (base0 == "*")
        {
            for (size_t k = 0; k < dir_list.size(); ++k)
            {
                rcstring pattern = path_join(dir_list[k], base);
                for (size_t m = 0; m < filenames.size(); ++m)
                {
                    rcstring fn = filenames[m];
                    if (fn.gmatch(pattern))
                        return fn;
                }
            }
        }
        else
        {
            for (size_t k = 0; k < dir_list.size(); ++k)
            {
                rcstring path = path_join(dir_list[k], base);
                if (filenames.member(path))
                    return path;
            }
        }
    }
    return rcstring();
}


void
target_build::generate(const rcstring_list &filenames)
{
    //
    // Build lists of known hosts and architcures, for use later.
    //
    rcstring_list known_arch;
    rcstring_list known_host;
    {
        // note: 'filenames' is not guaranteed to be sorted
        for (size_t j = 0; j < filenames.size(); ++j)
        {
            rcstring fn = filenames[j];
            rcstring_list components;
            components.split(fn, "/");
            if (components.size() >= 3)
            {
                if (components[0] == "arch")
                    known_arch.push_back_unique(components[1]);
                if (components[0] == "host")
                    known_host.push_back_unique(components[1]);
            }
        }
        known_arch.sort();
        known_host.sort();
    }

    rcstring_list host_neutral_install;

    for (size_t j = 0; j < filenames.size(); ++j)
        no_make_dir.push_back_unique(filenames[j].dirname());

    bool have_system_globals_text = filenames.member("system/globals.text");

    // bool stage2_compile = filenames.member("etc/stage2-compile.sh");
    // bool stage2_assemble = filenames.member("etc/stage2-assemble.sh");
    // bool stage3_compile = filenames.member("etc/stage3-compile.sh");
    // bool stage3_assemble = filenames.member("etc/stage3-assemble.sh");

    wrap_comment
    (
        "\n"
        "This file is generated by the ucsdpsys_osmakgen(1) command, from the "
        "ucsd-psystem-xc project.  The ucsd-psystem-xc project is distributed "
        "under the terms and conditions of the GNU GPL version 3.  As a "
        "special exception, the contents of this file are not subject to those "
        "terms and conditions.\n"
        "\n"
    );
    extra_file_header();
    if (blurb)
    {
        wrap_comment
        (
            "\n"
            "Note well:  This file is generated.  See the 'filenames "
            "=' assignment, below, for more information.\n"
            "\n"
        );
    }

    op->fputc('\n');
    if (!blurb)
    {
        wrap_comment("values provided by the ./configure script\n");
    }
    else
    {
        wrap_comment
        (
            "\n"
            "The " + load("prefix") + " macro is used by ./configure to define "
            "all the other installation paths.\n"
            "\n"
            "If you need to change the prefix, it is best to re-run "
            "./configure using the --prefix=PATH option, as this will re-write "
            "all of the files which contain the prefix, not just this one.\n"
            "\n"
        );
    }
    wrap_assignment("prefix", "@prefix@");
    wrap_assignment("exec_prefix", "@exec_prefix@");

    if (blurb)
    {
        op->fputc('\n');
        wrap_comment
        (
            "\n"
            "The " + load("datadir") + " is where to put the data "
            "directories.\n"
            "\n"
            "The " + load("datadir") + " is for architecture-neutral files.  "
            "On a network, this would be shared between all machines on the "
            "network.  It can be mounted read-only.\n"
            "\n"
            "This is where shell scripts are parked.  "
            "This is where report scripts are parked.  "
            "This is where documents are parked.\n"
            "\n"
            "By default, the macro uses the " + load("prefix") + " macro to "
            "simplify the number of things you have to supply on the "
            "./configure command line.\n"
            "\n"
        );
    }
    wrap_assignment("datadir", load("DESTDIR") + "@datadir@");
    wrap_assignment("datarootdir", load("DESTDIR") + "@datarootdir@");

    if (filenames.gmatch("*.1"))
    {
        if (blurb)
        {
            op->fputc('\n');
            wrap_comment
            (
                "\n"
                "The " + load("mandir") + " is where to put the on-line "
                "manuals.  On a network, this would be shared between all "
                "machines on the network.  It can be mounted read-only.\n\n"
                "By default, the macro uses the " + load("prefix") + " macro "
                "to simplify the number of things you have to supply on the "
                "./configure command line.\n"
                "\n"
            );
        }
        wrap_assignment("mandir", load("DESTDIR") + "@mandir@");
    }

    if (blurb)
    {
        op->fputc('\n');
        wrap_comment
        (
            "\n"
            "The " + load("sysconfdir") + " is where to put the system "
            "configuration directory.\n"
            "\n"
            "\n"
            "By default, the macro uses the " + load("prefix") + " macro to "
            "simplify the number of things you have to supply on the "
            "./configure command line.  It is common to explicitly give "
            "./configure --sysconfdir=/etc on the command line.\n"
        );
    }
    wrap_assignment("sysconfdir", load("DESTDIR") + "@sysconfdir@");

    if (blurb)
    {
        op->fputc('\n');
        wrap_comment
        (
            "\n"
            "This makefile builds the UCSD p-System system in three stages.\n"
            "\n"
            "The first stage is to use the ucsdpsys_compile command (from "
            "the ucsd-psystem-xc project) to build the executables of the "
            "system, compiler, assembler, linker and utilities.\n"
            "\n"
            "The first stage files are then combined, using the "
            "ucsdpsys_mkfs and ucsdpsys_disk commands from the "
            "ucsd-psystem-fs project, to build disk images suitable for "
            "use with a UCSD p-System emulator.\n"
            "\n"
            "Stage two involves using the ucsdpsys_vm emulator, from the "
            "ucsd-psystem-vm project, to use the disk images created by "
            "stage one to compile the sources once again, this time using "
            "the compiler built by stage one, in order to have a natively "
            "compiled system.\n"
            "\n"
            "The second stage files are then combined, using the "
            "ucsdpsys_mkfs and ucsdpsys_disk commands, to build a "
            "second set of disk images.  In theory the system is now "
            "boot-strapped and ready to use.\n"
            "\n"
            "Stage three once again uses the emulator, but this time with "
            "the disk images created by stage two.  The codefiles produced "
            "by stage three should be identical to those produced by stage "
            "two.  If they differ, you have probably uncovered a bug.\n"
            "\n"
        );
    }
    {
        rcstring rtarget = "all";
        rcstring_list ingredients;
        for (int j = 1; j <= max_stage; ++j)
        {
            ingredients.push_back(rcstring::printf("stage%d.disks", j));
        }
        rcstring_list body;
        write_rule(rtarget, ingredients, body);
    }
    if (blurb && max_stage < 3)
    {
        op->fputc('\n');
        wrap_comment
        (
            "Only stage " + cardinal(max_stage) + " is working, so far."
        );
    }

    op->fputc('\n');
    if (blurb)
    {
        wrap_comment
        (
            "\n"
            "The host is the system (often a brand name) that will "
            "eventually host the boot-strapped system.  This controls "
            "the choice of certain system alternatives.  Look in the "
            "'host' directory for the known host types (brand names).  "
            "It also implies the kind of microprocessor being used.\n"
            "\n"
        );
        if (default_host == "klebsch")
        {
            wrap_comment
            (
                "The default is 'klebsch' in reference to the "
                "ucsd-psystem-vm project (the emulator was written by Mario "
                "Klebsch).\n"
                "\n"
            );
        }
    }
    // There is only room for 8 characters in the filename "system.$(host)"
    wrap_assignment("host", default_host.substring(0, 8).downcase());

    op->fputc('\n');
    if (blurb)
    {
        wrap_comment
        (
            "\n"
            "The arch is the microprocessor that the boot-strapped "
            "system will eventually execute on.  This controls the "
            "choice of interpreter, assembler and long integer support "
            "to be built.\n"
            "\n"
            "Look in the 'arch' directory for the known microprocessor "
            "types.  We have yet to locate all of the historical source "
            "code for all known architectures, so the coverage may be "
            "uneven.  If you have source code to one of the missing "
            "pieces, it would be a most welcome contribution to this "
            "project.\n"
            "\n"
        );
    }
    if (default_arch.empty())
    {
        default_arch =
            shell("ucsdpsys_osmakgen --arch-from-host " + load("host"));
        wrap_assignment("arch", default_arch, true);
    }
    else
    {
        // There is only room for 8 characters in the filename "system.$(arch)"
        wrap_assignment("arch", default_arch.substring(0, 8).downcase());
    }

    bool host_disk_kb = filenames.member("etc/host-disk-kb.sh");
    if (host_disk_kb)
    {
        op->fputc('\n');
        if (blurb)
        {
            wrap_comment
            (
                "\n"
                "The size of the disks typically used by this host, in "
                "1024-byte (1kB) blocks.  The included shell script is used "
                "to determine this automatically.  You should rarely need to "
                "override this manually.\n"
                "\n"
            );
        }
        wrap_assignment
        (
            "host_disk_kb",
            shell("sh " + resolve("etc/host-disk-kb.sh") + " " + load("host")),
            true
        );
    }

    op->fputc('\n');
    if (blurb)
    {
        wrap_comment
        (
            "\n"
            "The Cross Compiler Flags\n"
            "\n"
            "The -fno-efj-nfj option tells the cross compiler not to use the "
            "EFJ and NFJ opcodes.  This is necessary for interpreters that do "
            "not implement these opcodes; Apple, for example.\n"
            // FIXME: can we detect this from the $(host) setting?
            "\n"
            "The -finline-math option tells the cross compiler to use the CSP "
            "math opcodes, rather than calling the TRANSCEN intrinsic unit.\n"
            // FIXME: can we detect this from the $(host) setting?
            "\n"
        );
    }
    else
        wrap_comment("cross compiler flags");

    {
        rcstring_list flags;
        flags.push_back("--arch=" + load("arch"));
        flags.push_back("-Wno-shadow");
        wrap_assignment("XCFLAGS", flags);
    }

    if (filenames.gmatch("*/main.asm.text"))
    {
        op->fputc('\n');
        if (blurb)
            wrap_comment("\nThe Cross Assembler Flags\n\n");
        else
            wrap_comment("cross assembler flags");
        rcstring_list flags;
        flags.push_back("--arch=" + load("arch"));
        wrap_assignment("XAFLAGS", flags);
    }

    op->fputc('\n');
    if (blurb)
    {
        wrap_comment
        (
            "\n"
            "Note well: This file is generated.\n"
            "\n"
            "This variable holds the list of files from which this file "
            "was generated.  By changeing it, and running the 'make "
            "rebuild-makefile' command, you can manipulate it in various "
            "ways.\n"
            "\n"
            "1. If you only need to rebuild the include dependencies, "
            "use the 'make rebuild-makefile' command.\n"
            "2. If you need to add a source file, add it to the 'filenames' "
            "variable, and then rebuild the Makefile (see above).\n"
            "3. If you want to add a new utility, the file must be "
            "named <utility>/main.text (plus as many include files as "
            "you like in that directory) for it to be recognised as a "
            "program to be compiled; then proceed as if you were "
            "adding a new file or files (see above).\n"
            "4. If you need a more structural change to this file, "
            "you actually need to change the ucsdpsys_osmakgen command "
            "in the cross compiler ucsd-psystem-xc project.\n"
            "\n"
            "See the ucsdpsys_osmakgen(1) man page for more information "
            "about the directory structure.\n"
            "\n"
        );
    }
    wrap_assignment("filenames", filenames);

    //
    // Walk the list of file names, and build everything we know how to
    // build.
    //
    rcstring_list system_programs;
    rcstring_list library_programs;
    rcstring_list utility_programs;
    rcstring_list all_charsets;
    bool have_diagnostic = false;
    bool have_miscinfo = false;
    bool have_assembler = false;
    bool have_interpreter = false;
    bool have_charset = false;
    for (size_t j = 0; j < filenames.size(); ++j)
    {
        rcstring fn = filenames[j];
        if (fn.ends_with("/main.text"))
        {
            rcstring prog = fn.substring(0, fn.size() - 10);
            rcstring pbase = prog.basename();
            if (prog == "diagnostic")
                have_diagnostic = true;
            else if (pbase == "assembler")
            {
                if (!fn.starts_with("arch/"))
                {
                    explain_output_error_and_die
                    (
                        "assemblers must be located at "
                            "arch/$(arch)/assembler/main.text, not %s",
                        fn.c_str()
                    );
                }
                have_assembler = true;
            }
            else if (system_candidates.member(prog))
                system_programs.push_back(pbase);
            else if (library_candidates.member(pascal_id(pbase)))
                library_programs.push_back(pbase);
            else
                utility_programs.push_back(pbase);

            bool maybe_globals =
                (
                    have_system_globals_text
                &&
                    !filenames.member(prog + "/globals.text")
                );
            bool maybe_asm =
                filenames.gmatch("*/" + prog + "/main.asm.text");
            stage1_compile(prog, maybe_globals, maybe_asm);
        }
        else if (fn.ends_with("/main.asm.text"))
        {
            rcstring prog = fn.substring(0, fn.size() - 14);
            rcstring pbase = prog.basename();
            if (pbase == "interpreter" || pbase == "interp")
            {
                if (!fn.starts_with("arch/") && !fn.starts_with("host/"))
                {
                    explain_output_error_and_die
                    (
                        "interpreters must be located at "
                            "arch/$(arch)/interpreter/main.asm.text, not %s",
                        fn.c_str()
                    );
                }
                have_interpreter = true;
            }
            stage1_assemble(prog);
        }
        else if (fn.gmatch("arch/*/assembler/error-messages.text"))
        {
            rcstring d = fn.dirname().dirname();
            rcstring arch = d.basename();
            for (int n = 1; n <= 3; ++n)
            {
                rcstring stage = rcstring::printf("stage%d", n);
                rcstring obj =
                   stage + "/codefiles/" + d + "/assembler/" + arch + ".errors";

                rcstring_list ingredients;
                ingredients.push_back(fn);
                rcstring_list body;
                rcstring cmd = "ucsdpsys_errors --arch=" + arch + " "
                    + resolve(fn) + " " + obj;
                body.push_back(cmd);
                write_rule(obj, ingredients, body);
                clean_file(obj);
            }
            have_assembler = true;
        }
        else if (fn.gmatch("arch/*/assembler/opcode-data.text"))
        {
            rcstring d = fn.dirname().dirname();
            rcstring arch = d.basename();
            for (int stage_n = 1; stage_n <= 3; ++stage_n)
            {
                rcstring stage = rcstring::printf("stage%d", stage_n);
                rcstring obj = stage + "/" + load("host")
                    + "/codefiles/assembler/" + arch + ".opcodes";
                rcstring_list ingredients;
                ingredients.push_back(fn);
                rcstring cmd = "ucsdpsys_opcodes --arch=" + arch + " "
                    + resolve(fn) + " " + obj;
                rcstring_list body;
                body.push_back(cmd);
                write_rule(obj, ingredients, body);
                clean_file(obj);
            }
            have_assembler = true;
        }
        else if (fn.gmatch("host/*/miscinfo.text"))
        {
            for (int stage_n = 1; stage_n <= 3; ++stage_n)
            {
                rcstring stage = rcstring::printf("stage%d", stage_n);
                rcstring data = stage + "/" + load("host") + "/codefiles/"
                    + fn.dirname() + "/miscinfo.data";
                rcstring data_dir = data.dirname();

                rcstring_list ingredients;
                ingredients.push_back(fn);
                rcstring_list body;
                rcstring cmd = "ucsdpsys_setup --arch=" + load("arch")
                    + " --encode " + resolve(fn) + " " + data;
                body.push_back(cmd);
                write_rule(data, ingredients, body);

                clean_file(data);
            }
            have_miscinfo = true;
        }
        else if (fn.gmatch("host/*/charset/*.text"))
        {
            rcstring charset = fn.trim_suffix() + ".charset";
            rcstring base = fn.basename().trim_suffix();
            bool is_system = (base == "system");

            //
            // From the Terak Manual (section 4.4):
            // "Bytes 2049-3072: If the character set file is 5 blocks in
            // length, these bytes contain the graphics logo displayed on the
            // monochrome monitor at boot time."
            //
            // The file must be at least 5 blocks, not exactly 5 blocks.  The
            // Terak manual fails to mention that the image is 64x64 pixels,
            // 8 bytes per row, least significant bit of each byte is on the
            // left, top row first.  The pixel values are inverted, compared
            // to modern conventions.
            //
            // look for {base}.png, system.png, triton.png, *.png
            // first in "host/{name}/charset/", then in "charset/"
            //
            rcstring png;
            {
                rcstring_list base_list;
                base_list.push_back(base);
                if (is_system)
                    base_list.push_back("system");
                base_list.push_back("triton"); // ucsd mascot
                base_list.push_back("*");
                rcstring_list dir_list;
                dir_list.push_back(fn.dirname());
                dir_list.push_back("charset");
                png = finder(filenames, dir_list, base_list, ".png");
            }

            rcstring_list ingredients;
            ingredients.push_back(fn);
            rcstring_list body;
            rcstring cmd = "ucsdpsys_charset --host=" + fn.field('/', 1) + " "
                "--encode " + resolve(fn) + " " + charset;
            if (!png.empty())
            {
                ingredients.push_back(png);
                cmd += " " + resolve(png);
            }
            body.push_back(cmd);
            write_rule(charset, ingredients, body);

            all_charsets.push_back(charset);
            clean_file(charset);
            if (is_system)
                have_charset = true;
        }
        else if (fn.gmatch("charset/*.text"))
        {
            rcstring charset = fn.trim_suffix() + ".charset";
            rcstring base = fn.basename().trim_suffix();
            bool is_system = (base == "system");

            // look for {base}.png, system.png, triton.png, *.png
            rcstring png;
            {
                rcstring_list base_list;
                base_list.push_back(base);
                if (is_system)
                    base_list.push_back("system");
                base_list.push_back("triton"); // ucsd mascot
                base_list.push_back("*");
                rcstring_list dir_list;
                assert(fn.dirname() == "charset");
                dir_list.push_back("charset");
                png = finder(filenames, dir_list, base_list, ".png");
            }

            op->fputc('\n');
            cond_if_not_exists("host/$(host)/charset/system.text");
            rcstring_list ingredients;
            ingredients.push_back(fn);
            rcstring_list body;
            rcstring ofn = path_join("host", load("host"), charset);
            rcstring cmd =
                "ucsdpsys_charset --host=$(host) --encode " + resolve(fn) +
                    " " + ofn;
            if (filenames.member(png))
            {
                ingredients.push_back(png);
                cmd += " " + resolve(png);
            }
            body.push_back(cmd);
            write_rule(ofn, ingredients, body);
            op->fputc('\n');
            cond_endif();

            all_charsets.push_back(ofn);
            clean_file(ofn);
            if (is_system)
                have_charset = true;
        }
        else if (fn.gmatch("man/man1/*.1"))
        {
            rcstring base = fn.basename();
            rcstring dst = load("mandir") + "/man1/" + base;
            simple_install_rule(fn, dst);
            host_neutral_install.push_back(dst);
        }
    }

    if (!all_charsets.empty())
    {
        all_charsets.sort();
        write_rule("all", all_charsets, rcstring_list());
    }

    bool have_system_syntax = filenames.member("compiler/error-messages.text");
    bool have_system_vol =
        (
            !system_programs.empty()
        ||
            !library_programs.empty()
        ||
            have_system_syntax
        ||
            have_miscinfo
        ||
            have_assembler
        ||
            have_charset
        );

    for (int state_n = 1; state_n <= 3; ++state_n)
    {
        rcstring stage = rcstring::printf("stage%d", state_n);
        rcstring rtarget = stage + ".disks";
        rcstring_list ingredients;
        if (have_diagnostic)
        {
            rcstring f = stage + "/" + load("host") + "/diagnostic.vol";
            ingredients.push_back(f);
        }
        if (have_system_vol)
        {
            rcstring f = stage + "/" + load("host") + "/system.vol";
            ingredients.push_back(f);
        }
        if (!utility_programs.empty())
        {
            rcstring f = stage + "/" + load("host") + "/utility.vol";
            ingredients.push_back(f);
        }
        rcstring_list body;
        write_rule(rtarget, ingredients, body);
    }

    if (have_interpreter)
    {
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring f1 = stage + "/" + load("host")
                + "/codefiles/interpreter/main.asm.code";
            rcstring f2 = stage + "/" + load("host") + "/diagnostic/system."
                + load("host");
            simple_copy_rule(f1, f2);

            f2 = stage + "/" + load("host") + "/system/system." + load("host");
            simple_copy_rule(f1, f2);
        }

        op->fputc('\n');
        cond_if_exists("host/" + load("host") + "/interpreter/main.asm.text");
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);

            rcstring name = "interpreter_" + stage + "_diagnostic_code";
            rcstring value = stage + "/" + load("host") + "/diagnostic/system."
                + load("host");
            wrap_assignment(name, value);

            name = "interpreter_" + stage + "_system_code";
            value = stage + "/" + load("host") + "/system/system."
                + load("host");
            wrap_assignment(name, value);
        }
        cond_else();
        cond_if_exists("arch/" + load("arch") + "/interpreter/main.asm.text");
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);

            rcstring name = "interpreter_" + stage + "_diagnostic_code";
            rcstring value = stage + "/" + load("host") + "/diagnostic/system."
                + load("host");
            wrap_assignment(name, value);

            name = "interpreter_" + stage + "_system_code";
            value = stage + "/" + load("host") + "/system/system."
                + load("host");
            wrap_assignment(name, value);
        }
        cond_else();
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring name = "interpreter_" + stage + "_diagnostic_code";
            rcstring_list value;
            wrap_assignment(name, value);
            name = "interpreter_" + stage + "_system_code";
            wrap_assignment(name, value);
        }
        cond_endif();
        cond_endif();
    }

    if (have_assembler)
    {
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring f1 = stage + "/codefiles/arch/" + load("arch")
                + "/assembler/main.code";
            rcstring f2 = stage + "/system/system.assmbler";
            simple_copy_rule(f1, f2);
        }

        op->fputc('\n');
        cond_if_exists("arch/" + load("arch") + "/assembler/main.text");
        for (int n = 1; n <= 3; ++n)
        {
            rcstring stage = rcstring::printf("stage%d", n);
            rcstring name = stage + "_assembler_code";
            rcstring value = stage + "/" + load("host")
                + "/system/system.assmbler";
            wrap_assignment(name, value);
        }
        cond_else();
        for (int state_n = 1; state_n <= 3; ++state_n)
        {
            rcstring stage = rcstring::printf("stage%d", state_n);
            rcstring name = stage + "_assembler_code";
            rcstring_list value;
            wrap_assignment(name, value);
        }
        cond_endif();

        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring f1 = stage + "/" + load("host")
                + "/codefiles/assembler/" + load("arch") + ".errors";
            rcstring f2 = stage + "/" + load("host") + "/system/"
                + load("arch") + ".errors";
            simple_copy_rule(f1, f2);
        }

        op->fputc('\n');
        cond_if_exists("arch/" + load("arch")
            + "/assembler/error-messages.text");
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring name = stage + "_assembler_error_data";
            rcstring value = stage + "/system/" + load("arch") + ".errors";
            wrap_assignment(name, value);
        }
        cond_else();
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring name = stage + "_assembler_error_data";
            rcstring_list value;
            wrap_assignment(name, value);
        }
        cond_endif();

        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring f1 = stage + "/codefiles/arch/" + load("arch")
                + "/assembler/" + load("arch") + ".opcodes";
            rcstring f2 = stage + "/system/" + load("arch") + ".opcodes";
            simple_copy_rule(f1, f2);
        }

        op->fputc('\n');
        cond_if_exists("arch/" + load("arch")
            + "/assembler/opcode-data.text");
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring name = stage + "_assembler_opcode_data";
            rcstring value = stage + "/system/" + load("arch") + ".opcodes";
            wrap_assignment(name, value);
        }
        cond_else();
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring name = stage + "_assembler_opcode_data";
            rcstring_list value;
            wrap_assignment(name, value);
        }
        cond_endif();
    }

    if (have_diagnostic)
    {
        //
        // We build a stage1/diagnostic/ directory, full of the files we
        // want, and then we use a single --put to build it.
        //
        rcstring stage = "stage1/" + load("host");

        {
            rcstring f1 = stage + "/codefiles/diagnostic/main.code";
            rcstring f2 = stage + "/diagnostic/system.pascal";
            simple_copy_rule(f1, f2);
        }

        op->fputc('\n');
        if (blurb)
        {
            wrap_comment
            (
                "\n"
                "The Diagnostic Disk image is used to test the p-machine "
                "(interpreter, virtual machine) implementation.  It works by "
                "booting straight into the test program, and running the test "
                "program instead of the normal system.\n"
                "\n"
                "This provides a way of separating the diagnosis of p-machine "
                "problems from the diagnosis of system problems, when "
                "boot-strapping a new host.\n"
                "\n"
            );
        }
        else
            wrap_comment("build diagnostic disk image");

        // write the rule
        rcstring rtarget = stage + "/diagnostic.vol";
        rcstring_list ingredients;
        rcstring src = stage + "/diagnostic/system.pascal";
        ingredients.push_back(src);
        if (have_interpreter)
            ingredients.push_back(load("interpreter_stage1_diagnostic_code"));
        rcstring_list body;
        rcstring cmd = "ucsdpsys_mkfs --arch=" + load("arch") + " -B";
        cmd += (host_disk_kb ? load("host_disk_kb") : "256");
        cmd += " --label=diagnostic " + rtarget;
        body.push_back(cmd);
        rcstring files = stage + "/diagnostic/system.pascal";
        if (have_interpreter)
            files += " " + load("interpreter_stage1_diagnostic_code");
        cmd = "ucsdpsys_disk -f " + rtarget + " --put " + resolve(files);
        body.push_back(cmd);
        cmd = "ucsdpsys_disk -f " + rtarget + " --crunch --list --sort=name";
        body.push_back(cmd);
        write_rule(rtarget, ingredients, body);
        clean_file(rtarget);

        // FIXME: how do we add the boot sectors?
        // cmd = "ucsdpsys_disk -f " + rtarget + " --put --boot=" name of file
    }

    //
    // Build the system library, if its source files are present.
    //
    if (!library_programs.empty())
    {
        rcstring stage = "stage1/" + load("host");

        rcstring_list ingredients;
        for (size_t j = 0; j < library_programs.size(); ++j)
        {
            rcstring prog = library_programs[j];
            rcstring obj = stage + "/codefiles/" + prog + "/main.code";
            ingredients.push_back(obj);
        }

        op->fputc('\n');
        wrap_comment("build system library");

        // write the rule
        rcstring rtarget = stage + "/system/system.library";
        rcstring_list body;
        rcstring cmd = "ucsdpsys_librarian --create=" + rtarget;
        if (!notice.empty())
        {
            cmd += " --copyright=";
            cmd += notice.replace(" ", "_").quote_shell();
        }
        for (size_t j = 0; j < ingredients.size(); ++j)
        {
            rcstring obj = ingredients[j];
            cmd += " --copy " + resolve(obj) + " --segment ";
            rcstring segname = pascal_id(obj.dirname().basename());
            cmd += segname;

            int n = library_segnum(segname);
            if (n > 0)
                cmd += rcstring::printf("=%d", n);
        }
        body.push_back(cmd);
        write_rule(rtarget, ingredients, body);

        clean_file(rtarget);
    }

    if (have_miscinfo)
    {
        for (int n = 1; n <= 3; ++n)
        {
            rcstring stage = rcstring::printf("stage%d", n);
            stage += "/" + load("host");
            rcstring f1 =
                stage + "/codefiles/host/" + load("host") + "/miscinfo.data";
            rcstring f2 = stage + "/system/system.miscinfo";
            simple_copy_rule(f1, f2);
        }
        op->fputc('\n');
        cond_if_exists("host/" + load("host") + "/miscinfo.text");
        for (int n = 1; n <= 3; ++n)
        {
            rcstring stage = rcstring::printf("stage%d", n);
            rcstring name = stage + "_system_miscinfo";
            stage += "/" + load("host");
            rcstring value = stage + "/system/system.miscinfo";
            wrap_assignment(name, value);
        }
        cond_else();
        for (int n = 1; n <= 3; ++n)
        {
            rcstring stage = rcstring::printf("stage%d", n);
            rcstring name = stage + "_system_miscinfo";
            rcstring value;
            wrap_assignment(name, value);
        }
        cond_endif();
    }

    if (have_charset)
    {
        op->fputc('\n');
        cond_if_exists("host/" + load("host") + "/charset/system.text");
        for (int n = 1; n <= 3; ++n)
        {
            rcstring stage = rcstring::printf("stage%d", n);
            rcstring name = stage + "_system_charset";
            rcstring value = "host/" + load("host") + "/system/system.charset";
            wrap_assignment(name, value);
        }
        cond_else();
        cond_if_exists("charset/system.text");
        for (int n = 1; n <= 3; ++n)
        {
            // yes, it is the same as above
            rcstring stage = rcstring::printf("stage%d", n);
            rcstring name = stage + "_system_charset";
            rcstring value = "host/" + load("host") + "/system/system.charset";
            wrap_assignment(name, value);
        }
        cond_else();
        for (int n = 1; n <= 3; ++n)
        {
            rcstring stage = rcstring::printf("stage%d", n);
            rcstring name = stage + "_system_charset";
            rcstring value;
            wrap_assignment(name, value);
        }
        cond_endif();
        cond_endif();

        // OK to always define, but sometimes not used.
        {
            rcstring f1 = "host/" + load("host") + "/charset/system.charset";
            rcstring f2 = "host/" + load("host") + "/system/system.charset";
            simple_copy_rule(f1, f2);
        }
    }

    //
    // Specific programs go on the system disk.
    //
    if (have_system_vol)
    {
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            rcstring stage_host = stage + "/" + load("host");
            rcstring_list ingredients;

            for (size_t j = 0; j < system_programs.size(); ++j)
            {
                rcstring prog = system_programs[j];
                rcstring sysfile = name_mangle(prog + ".code");
                rcstring f1 = stage_host + "/codefiles/" + prog + "/main.code";
                rcstring f2 = stage_host + "/system/" + sysfile;
                simple_copy_rule(f1, f2);
                ingredients.push_back(f2);
            }

            if (have_system_syntax)
            {
                rcstring f1 = "compiler/error-messages.text";
                rcstring f2 = stage_host + "/system/system.syntax";
                simple_copy_rule(f1, f2);
                ingredients.push_back(f2);
            }

            if (!library_programs.empty())
                ingredients.push_back(stage_host + "/system/system.library");
            if (have_miscinfo)
                ingredients.push_back(load(stage + "_system_miscinfo"));
            if (have_charset)
                ingredients.push_back(load(stage + "_system_charset"));
            if (have_interpreter)
            {
                ingredients.push_back
                (
                    load("interpreter_" + stage + "_system_code")
                );
            }
            if (have_assembler)
            {
                ingredients.push_back(load(stage + "_assembler_code"));
                ingredients.push_back(load(stage + "_assembler_error_data"));
                ingredients.push_back(load(stage + "_assembler_opcode_data"));
            }

            op->fputc('\n');
            if (blurb)
            {
                rcstring Stage = "Stage " + cardinal(stage_n).capitalize();
                wrap_comment
                (
                    "\n"
                    "The " + Stage + " System Disk image contains the files "
                    "needed to boot the UCSD p-System, and the files that "
                    "provide the fundamental operations of the system.  "
                    "It also includes the programs necessary to build other "
                    "programs and libraries.\n"
                    "\n"
                );
                if (stage_n < 3)
                {
                    wrap_comment
                    (
                        "The " + Stage + " System Disk image is also used when "
                        "compiling the next stage of the build.\n"
                        "\n"
                    );
                }
            }
            else
            {
                wrap_comment
                (
                    "build stage " + cardinal(stage_n) + " system disk image"
                );
            }

            // Write the rule
            rcstring rtarget = stage_host + "/system.vol";
            ingredients.sort();
            rcstring_list body;
            rcstring cmd = "ucsdpsys_mkfs --arch=" + load("arch") + " -B";
            cmd += (host_disk_kb ? load("host_disk_kb") : "256");
            cmd += " --label=system " + stage_host + "/system.vol";
            body.push_back(cmd);

            cmd = "ucsdpsys_disk --file " + stage_host + "/system.vol --text "
                "--put " + resolve(ingredients.unsplit());
            body.push_back(cmd);

            cmd = "ucsdpsys_disk --file " + stage_host + "/system.vol --crunch "
                "--list --sort=name";
            body.push_back(cmd);
            write_rule(rtarget, ingredients, body);

            clean_file(rtarget);

            // FIXME: how do we add the boot sectors?
        }
    }

    // All other programs go on the utility disk.
    if (!utility_programs.empty())
    {
        for (int stage_n = 1; stage_n <= 3; ++stage_n)
        {
            rcstring stage = rcstring::printf("stage%d", stage_n);
            stage += "/" + load("host");
            rcstring_list ingredients;

            for (size_t j = 0; j < utility_programs.size(); ++j)
            {
                rcstring prog = utility_programs[j];
                rcstring sysfile = name_mangle(prog + ".code");
                rcstring f1 = stage + "/codefiles/" + prog + "/main.code";
                rcstring f2 = stage + "/utility/" + sysfile;
                simple_copy_rule(f1, f2);
                ingredients.push_back(f2);
            }

            op->fputc('\n');
            rcstring Stage = "Stage " + cardinal(stage_n).capitalize();
            if (blurb)
            {
                wrap_comment
                (
                    "\n"
                    "The " + Stage + " Utility Disk image contains all of the "
                    "programs that are part of the UCSD p-System, but not "
                    "essential to boot and run the system, and not essential "
                    "to build other programs and libraries.\n"
                    "\n"
                );
                if (stage_n <= 2)
                {
                    wrap_comment
                    (
                        "The " + Stage + " Utility Disk image is *not* used "
                        "when compiling the next stage of the build.\n"
                        "\n"
                    );
                }
            }
            else
            {
                wrap_comment
                (
                    "build stage " + cardinal(stage_n) + " utility disk image"
                );
            }

            // Write the rule
            rcstring rtarget = stage + "/utility.vol";
            ingredients.sort();
            rcstring_list body;
            rcstring cmd = "ucsdpsys_mkfs --arch=" + load("arch") + " -B";
            cmd += (host_disk_kb ? load("host_disk_kb") : "256");
            cmd += " --label=utility " + stage + "/utility.vol";
            body.push_back(cmd);
            cmd = "ucsdpsys_disk --file " + stage + "/utility.vol --text "
                "--put " + resolve(ingredients.unsplit());
            body.push_back(cmd);
            cmd = "ucsdpsys_disk --file " + stage + "/utility.vol --crunch "
                "--list --sort=name";
            body.push_back(cmd);
            write_rule(rtarget, ingredients, body);

            clean_file(rtarget);
        }
    }

    // the "clean" rules
    {
        rcstring lhs = "clean";
        rcstring_list rhs;
        if (!clean_files.empty())
            rhs.push_back("clean-files");
        if (!clean_dirs.empty())
            rhs.push_back("clean-dirs");
        rcstring_list body;
        write_rule(lhs, rhs, body);
    }
    if (!clean_files.empty())
    {
        clean_files.sort();
        rcstring_list rhs;
        write_clean_files_rule("clean-files", rhs, clean_files);
    }
    if (!clean_dirs.empty())
    {
        clean_dirs.sort();
        rcstring_list rhs;
        write_clean_dirs_rule("clean-dirs", rhs, clean_dirs);
    }

    // the "distclean" rules
    {
        rcstring lhs = "distclean";
        rcstring_list rhs;
        rhs.push_back("clean");
        if (!dist_clean_files.empty())
            rhs.push_back("dist-clean-files");
        if (!dist_clean_dirs.empty())
            rhs.push_back("dist-clean-dirs");
        rcstring_list body;
        write_rule(lhs, rhs, body);
    }
    if (!dist_clean_files.empty())
    {
        dist_clean_files.sort();
        rcstring_list rhs;
        write_clean_files_rule("dist-clean-files", rhs, dist_clean_files);
    }
    if (!dist_clean_dirs.empty())
    {
        dist_clean_dirs.sort();
        rcstring_list rhs;
        write_clean_dirs_rule("dist-clean-dirs", rhs, dist_clean_dirs);
    }

    if (blurb)
    {
        op->fputc('\n');
        wrap_comment
        (
            "\n"
            "Note well:  This file is generated.  This rule can be used "
            "to rebuild it.  See the 'filenames =' assignment, above, "
            "for more information.\n"
            "\n"
        );
    }
    {
        rcstring rtarget = "rebuild-makefile";
        rcstring_list ingredients;
        rcstring_list body;
        // deliberately not calling resolve()
        body.push_back("ucsdpsys_osmakgen " + load("filenames"));
        write_rule(rtarget, ingredients, body);
    }

    // Should "make check" run some tests?  If so, what?
    // Didn't stage 2 and stage 3 take care of that?

    if (blurb)
    {
        op->fputc('\n');
        rcstring stage = "Stage " + cardinal(max_stage).capitalize();
        wrap_comment
        (
            "\n"
            "We have \"make install\" install both the " + stage + " disk "
            "images and the " + stage + " disk directories into " +
            load("datadir") + ".\n"
            "\n"
            "We install both the directories and the disk images so that the "
            "cross compiler can read the " + load("datadir") +
            "/ucsd-psystem-os/system/system.library file when cross "
            "compiling programs that use units, without adding code to the "
            "cross compiler for groping the private parts of disk images.  It "
            "also makes it a simple task to remix the files when users want to "
            "make custom disk images.\n"
            "\n"
        );
    }

    {
        rcstring rtarget = "install";
        rcstring_list ingredients;
        ingredients.push_back(rcstring::printf("stage%d.install", max_stage));
        ingredients.push_back(host_neutral_install);
        rcstring_list body;
        write_rule(rtarget, ingredients, body);
    }

    for (int j = 1; j <= 3; ++j)
    {
        rcstring stage = rcstring::printf("stage%d", j);
        rcstring stage_host = stage + "/" + load("host");
        rcstring rtarget = stage + ".install";
        rcstring_list ingredients;
        ingredients.push_back(stage + ".disks");
        rcstring_list body;
        // FIXME: what if there is more than one utility disk?
        static const char *disk_names[] =
        {
            "diagnostic",
            "system",
            "utility",
        };
        for (size_t k = 0; k < SIZEOF(disk_names); ++k)
        {
            rcstring disk_name = disk_names[k];
            if (k == 0)
            {
                rcstring cmd =
                    "mkdir -p " + load("datadir") + "/ucsd-psystem-os";
                body.push_back(cmd);
            }
            // FIXME: explicit list
            rcstring cmd = "cp -r " + stage_host + "/" + disk_name + " "
                + load("datadir") + "/ucsd-psystem-os/" + disk_name;
            body.push_back(cmd);
            cmd = "cp " + resolve(stage_host + "/" + disk_name + ".vol") + " " +
                load("datadir") + "/ucsd-psystem-os/" + disk_name + ".vol";
            body.push_back(cmd);
        }
        write_rule(rtarget, ingredients, body);
    }

    if (!uninstall_files.empty())
    {
        rcstring rtarget("uninstall");
        rcstring_list ingredients;
        rcstring_list body;
        uninstall_files.sort();
        for (size_t j = 0; j < uninstall_files.size(); ++j)
            body.push_back("rm -f " + uninstall_files[j]);
        write_rule(rtarget, ingredients, body);
    }
}


void
target_build::wrap_assignment(const rcstring &name, const rcstring &value,
    bool immed)
{
    rcstring_list values;
    if (!value.empty())
        values.push_back(value);
    wrap_assignment(name, values, immed);
}


rcstring
target_build::resolve(const rcstring &name)
    const
{
    return name;
}


rcstring
target_build::extra_include_flags(void)
    const
{
    return rcstring();
}


void
target_build::extra_file_header(void)
{
    // Do nothing.
}


void
target_build::remember_dist_clean_file(const rcstring &fn)
{
    dist_clean_files.push_back_unique(fn);
}


void
target_build::remember_dist_clean_dir(const rcstring &dir)
{
    dist_clean_dirs.push_back_unique(dir);
}


// vim: set ts=8 sw=4 et :

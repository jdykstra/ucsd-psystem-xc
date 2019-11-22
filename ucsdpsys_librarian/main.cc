//
// UCSD p-System cross compiler
// Copyright (C) 2010-2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
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

#include <lib/ac/stdio.h>
#include <lib/ac/string.h>
#include <lib/ac/getopt.h>
#include <unistd.h>
#include <libexplain/output.h>
#include <libexplain/program_name.h>
#include <libexplain/rename.h>

#include <lib/codefile/file.h>
#include <lib/codefile/filter/insert.h>
#include <lib/codefile/filter/notice.h>
#include <lib/codefile/filter/remove_by_name.h>
#include <lib/codefile/filter/remove_by_number.h>
#include <lib/debug.h>
#include <lib/output/stdout.h>
#include <lib/path_join.h>
#include <lib/p_machine.h>
#include <lib/rcstring.h>
#include <lib/segment/filter/renumber.h>
#include <lib/version.h>


static void
usage(void)
{
    const char *prog = explain_program_name_get();
    fprintf(stderr, "Usage: %s [ <option>... ] <filename>...\n", prog);
    fprintf(stderr, "       %s -V\n", prog);
    exit(1);
}


static void
must_use_file_option_first(void)
{
    explain_output_error_and_die("you must use the --file option first");
}


static void
cat(const codefile::pointer &ifp, const rcstring &ofn)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "ofn = %s", ofn.quote_c().c_str());
    codefile::pointer ofp =
        (
            p_machine_set_explicitly()
        ?
            codefile_file::create(ofn, ifp->get_byte_sex())
        :
            ifp->new_file_factory(ofn)
        );
    ofp->copy_from(ifp);
    ofp->write_segment_dictionary();
    DEBUG(1, "}");
}


static bool
is_a_number(const char *s, long &n)
{
    assert(s);
    char *ep = 0;
    n = strtol(s, &ep, 10);
    assert(ep);
    return (ep != s && !*ep);
}


int
main(int argc, char **argv)
{
    explain_program_name_set(argv[0]);
    explain_option_hanging_indent_set(4);
    codefile::pointer infile;
    codefile::pointer alternate;
    bool emit_new_file = false;
    rcstring output_filename;
    for (;;)
    {
        static struct option options[] =
        {
            { "alternate", 1, 0, 'a' },
            { "copy", 1, 0, 'a' },
            { "copyright", 1, 0, 'n' },
            { "create", 1, 0, 'c' },
            { "debug", 0, 0, 'd' },
            { "file", 1, 0, 'f' },
            { "force-remove", 1, 0, 'R' },
            { "listing", 0, 0, 'l' },
            { "notice", 1, 0, 'n' },
            { "output", 1, 0, 'o' },
            { "p-machine", 1, 0, 'P' },
            { "remove", 1, 0, 'r' },
            { "remove-system-segments", 0, 0, 'X' },
            { "segment", 1, 0, 's' },
            { "version", 0, 0, 'V' },
            { 0, 0, 0, 0 },
        };
        int c = getopt_long(argc, argv, "a:c:df:ln:o:R:r:s:VX", options, 0);
        if (c == EOF)
            break;
        DEBUG(1, "c = '%c'", c);
        switch (c)
        {
        case 'a':
            DEBUG(1, "alternate");
            if (!infile)
                must_use_file_option_first();
            alternate = codefile_file::create(optarg);
            if (infile->get_byte_sex() != alternate->get_byte_sex())
            {
                explain_output_error_and_die
                (
                    "all codefiles must have the same byte sex"
                );
            }
            break;

        case 'c':
            DEBUG(1, "create");
            if (infile)
                goto too_many_file;
            infile = codefile_file::create(optarg, little_endian);
            emit_new_file = true;
            break;

        case 'd':
            DEBUG(1, "debug");
            ++debug_level;
            break;

        case 'f':
            DEBUG(1, "file");
            if (infile)
            {
                too_many_file:
                explain_output_error_and_die
                (
                    "you may only use one --file or --create option"
                );
            }
            infile = codefile_file::create(optarg);
            break;

        case 'l':
            {
                DEBUG(1, "list");
                if (!infile)
                    must_use_file_option_first();
                output::pointer os = output_stdout::create();
                infile->print_library_map(os);
            }
            break;

        case 'n':
            {
                DEBUG(1, "notice");
                if (!infile)
                    must_use_file_option_first();
                rcstring text(optarg);
                text = text.replace("_", " ");
                infile = codefile_filter_notice::create(infile, text);
                emit_new_file = true;
            }
            break;

        case 'o':
            if (!output_filename.empty())
                explain_output_error_and_die("may only use --output once");
            output_filename = optarg;
            break;

        case 'P':
            // The p-machine release dictates the shape of the codefile.
            p_machine_set(optarg);
            break;

        case 'r':
            {
                DEBUG(1, "remove");
                rcstring segname(optarg);
                if (!infile)
                    must_use_file_option_first();
                if (!infile->get_segment_by_name(segname))
                {
                    explain_output_error_and_die
                    (
                        "%s: segment %s unknown",
                        infile->get_filename().c_str(),
                        segname.quote_c().c_str()
                    );
                }
                infile =
                    codefile_filter_remove_by_name::create(infile, segname);
                emit_new_file = true;
            }
            break;

        case 'R':
            {
                DEBUG(1, "remove");
                char *ep = 0;
                long n = strtol(optarg, &ep, 10);
                if (ep != optarg && !*ep)
                {
                    infile =
                        codefile_filter_remove_by_number::create(infile, n);
                }
                else
                {
                    infile =
                        codefile_filter_remove_by_name::create(infile, optarg);
                }
                emit_new_file = true;
            }
            break;

        case 's':
            {
                DEBUG(1, "segment");
                rcstring segname(optarg);
                if (!infile)
                    must_use_file_option_first();
                if (!alternate)
                {
                    explain_output_error_and_die
                    (
                        "you must use the --copy option before "
                            "the --segment option"
                    );
                }

                //
                // we need to be able to re-number segments.
                // for situations like
                //     --copy pascalio.code --segment pascalio=31
                //
                int segnum = -1;
                {
                    const char *s = segname.c_str();
                    const char *eq = strchr(s, '=');
                    if (eq)
                    {
                        long n = 0;
                        if (!is_a_number(eq + 1, n))
                        {
                            explain_output_error_and_die
                            (
                                "new segment number %s invalid",
                                rcstring(eq + 1).quote_c().c_str()
                            );
                        }
                        if (n < 0 || n >= 64)
                        {
                            explain_output_error_and_die
                            (
                                "new segment number %ld not in range 0..63",
                                n
                            );
                        }
                        segname = rcstring(s, eq - s);
                        segnum = n;
                    }
                }

                segment::pointer sp = alternate->get_segment_by_name(segname);
                if (!sp)
                {
                    explain_output_error_and_die
                    (
                        "%s: segment %s unknown",
                        alternate->get_filename().c_str(),
                        segname.quote_c().c_str()
                    );
                }
                if (segnum >= 0)
                    sp = segment_filter_renumber::create(sp, segnum);
                infile = codefile_filter_insert::create(infile, alternate, sp);
                emit_new_file = true;
            }
            break;

        case 'V':
            DEBUG(1, "version");
            version_print();
            return 0;

        case 'X':
            DEBUG(1, "remove-system-segments");
            infile = codefile_filter_remove_by_number::create(infile, 0);
            infile = codefile_filter_remove_by_number::create(infile, 2);
            infile = codefile_filter_remove_by_number::create(infile, 3);
            infile = codefile_filter_remove_by_number::create(infile, 4);
            infile = codefile_filter_remove_by_number::create(infile, 5);
            infile = codefile_filter_remove_by_number::create(infile, 6);
            emit_new_file = true;
            break;

        default:
            DEBUG(1, "idiot");
            usage();
        }
    }
    if (optind < argc)
        usage();
    alternate.reset();

    if (emit_new_file)
    {
        if (output_filename.empty())
        {
            rcstring ifn = infile->get_filename();
            output_filename =
                path_join
                (
                    ifn.dirname(),
                    "." + ifn.basename(".code") + ".tmp.code"
                );
            cat(infile, output_filename);
            explain_rename_or_die(output_filename.c_str(), ifn.c_str());
        }
        else
        {
            cat(infile, output_filename);
        }
    }
    return 0;
}


// vim: set ts=8 sw=4 et :

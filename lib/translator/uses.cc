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

#include <lib/ac/sys/stat.h>

#include <lib/codefile/file.h>
#include <lib/libdir.h>
#include <lib/pascal/lex.h>
#include <lib/pascal_id.h>
#include <lib/path_join.h>
#include <lib/translator.h>


static bool
exists_and_is_a_regular_file(const rcstring &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) < 0)
        return false;
    return S_ISREG(st.st_mode);
}


void
translator::codefile_library(const rcstring &filename)
{
    unit_library_init();
    unit_library_add(filename, true);
}


bool
translator::unit_library_adder(const rcstring &filename)
{
    if (exists_and_is_a_regular_file(filename))
    {
        unit_libraries.push_back(filename);
        return true;
    }
    if (!filename.downcase().ends_with(".code"))
    {
        rcstring fn2 = filename + ".code";
        if (exists_and_is_a_regular_file(fn2))
        {
            unit_libraries.push_back(fn2);
            return true;
        }
    }
    if (!filename.downcase().ends_with(".library"))
    {
        rcstring fn3 = filename + ".library";
        if (exists_and_is_a_regular_file(fn3))
        {
            unit_libraries.push_back(fn3);
            return true;
        }
    }

    return false;
}


void
translator::unit_library_init(void)
{
    if (unit_libraries.empty())
    {
        unit_library_add("system", false);
    }
}


void
translator::unit_library_add(const rcstring &filename, bool insist)
{
    if (unit_library_adder(filename))
        return;

    //
    // Add the system directories to the search path.
    //
    if (!library_search_path_env)
    {
        // Only do this once.
        library_search_path_env = false;

        library_search_path.push_back(".");

        const char *paths = getenv("UCSD_PSYSTEM_XC_LIBRARY_PATH");
        if (paths && *paths)
            library_search_path.split(paths, ":;");

        rcstring dir = configured_datadir();
        rcstring path = path_join(dir, "ucsd-psystem-xc", filename);
        library_search_path.push_back(path);

        path = path_join(dir, "ucsd-psystem-xc", "system", filename);
        library_search_path.push_back(path);

        dir = configured_datarootdir();
        path = path_join(dir, "ucsd-psystem-os", filename);
        library_search_path.push_back(path);

        path = path_join(dir, "ucsd-psystem-os", "system", filename);
        library_search_path.push_back(path);
    }

    if (filename[0] != '/')
    {
        //
        // Look along the search path.
        //
        for (size_t j = 0; j < library_search_path.size(); ++j)
        {
            rcstring dir = library_search_path[j];
            if (dir.empty())
                dir = ".";
            rcstring fn2 = path_join(dir, filename);
            if (unit_library_adder(fn2))
                return;
        }
    }

    //
    // Could not find the named file.
    //
    if (insist)
        unit_libraries.push_back(filename);
}


void
translator::uses(const variable_name_list &names)
{
    for (size_t j = 0; j < names.size(); ++j)
        uses(names[j]);
}


void
translator::uses(const variable_name &name)
{
    unit_library_init();

    rcstring pname = pascal_id(name.get_name());

    //
    // In the interests of better error messages, we are going to keep
    // track of the ones we find, so that we can do a fuzzy match at the
    // end, in order to suggest alternatives.
    //
    symtab<rcstring> interface_names_by_pascal_id;

    for
    (
        unit_libraries_t::const_iterator it = unit_libraries.begin();
        it != unit_libraries.end();
        ++it
    )
    {
        rcstring code_file_name = *it;
        codefile::pointer cf = codefile_file::create(code_file_name);
        for (unsigned sn = 0; ; ++sn)
        {
            segment::pointer sp = cf->get_segment(sn);
            if (!sp)
                break;
            rcstring segpname = pascal_id(sp->get_name());
            interface_names_by_pascal_id.assign(segpname, segpname);
            if (pname != segpname)
                continue;
            switch (sp->get_kind())
            {
            case DATASEG:
            case HOSTSEG:
            case LINKED:
            case SEGPROC:
            case SEPRTSEG:
            case UNLINKED_INTRINS:
                break;

            case LINKED_INTRINS:
                {
                    int code_seg = sp->get_segment_number();

                    // check to see if there is an accompanying data segment
                    int data_seg = -1;
                    {
                        segment::pointer dsp =
                            cf->get_segment_by_name(sp->get_name(), DATASEG);
                        if (dsp)
                            data_seg = dsp->get_segment_number();
                    }

                    rcstring text = sp->get_interface_text();
                    pascal_lex_include_unit
                    (
                        code_file_name,
                        name.get_name(),
                        code_seg,
                        data_seg,
                        sp->get_kind(),
                        text
                    );
                    return;
                }

            case UNITSEG:
                if (sp->get_code_length() > 0)
                {
                    // Use the segment number of the segment IF that segment
                    // number is still free ELSE grab next free segment number.
                    int code_seg =
                        uses_unitseg(sp->get_name(), sp->get_segment_number());
                    int data_seg = -1;

                    rcstring text = sp->get_interface_text();

                    pascal_lex_include_unit
                    (
                        code_file_name,
                        name.get_name(),
                        code_seg,
                        data_seg,
                        sp->get_kind(),
                        text
                    );
                    return;
                }
                break;
            }
        }
    }

    //
    // If the unit isn't there (for example, when bootstrapping a new
    // system) we insert some text of our own.
    //
    interface_names_by_pascal_id.assign("turtlegr", "turtle_graphics");
    if (pname == "turtlegr")
    {
        rcstring code_file_name = "system.library";
        int code_seg = 20;
        int data_seg = 21;
        segkind_t segkind = LINKED_INTRINS;
        pascal_lex_include_unit
        (
            code_file_name,
            name.get_name(),
            code_seg,
            data_seg,
            segkind,
            "type screencolor = (none, white, black, reverse, radar,\n"
            "    black1, green, violet, white1, black2, orange, blue,\n"
            "    white2);\n"
            "procedure initturtle;\n"
            "procedure turn(angle: integer);\n"
            "procedure turnto(angle: integer);\n"
            "procedure move(dist: integer);\n"
            "procedure moveto(x, y: integer);\n"
            "procedure pencolor(penmode: screencolor);\n"
            "procedure textmode;\n"
            "procedure grafmode;\n"
            "procedure fillscreen(fillcolor: screencolor);\n"
            "procedure viewport(left, right, bottom, top: integer);\n"
            "function turtlex: integer;\n"
            "function turtley: integer;\n"
            "function turtleang: integer;\n"
            "function screenbit(x, y: integer): boolean;\n"
            "procedure drawblock(var source; rowsize, xskip, yskip,\n"
            "    width, height, xscreen, yscreen, mode: integer);\n"
            "procedure wchar(ch: char);\n"
            "procedure wstring(s: string);\n"
            "procedure chartype(mode: integer);\n"
        );
        return;
    }
    interface_names_by_pascal_id.assign("applestu", "apple_stuff");
    if (pname == "applestu")
    {
        rcstring code_file_name = "system.library";
        int code_seg = 20;
        int data_seg = -1;
        segkind_t segkind = LINKED_INTRINS;
        pascal_lex_include_unit
        (
            code_file_name,
            name.get_name(),
            code_seg,
            data_seg,
            segkind,
            "function paddle(select: integer): integer;\n"
            "function button(select: integer): boolean;\n"
            "procedure ttlout(select: integer; data: boolean);\n"
            "function keypress: boolean;\n"
            "function random: integer;\n"
            "procedure randomize;\n"
            "procedure note(pitch, duration: integer);\n"
        );
        return;
    }
    interface_names_by_pascal_id.assign("chainstu", "chain_stuff");
    if (pname == "chainstu")
    {
        rcstring code_file_name = "system.library";
        int code_seg = 28;
        int data_seg = -1;
        segkind_t segkind = LINKED_INTRINS;
        pascal_lex_include_unit
        (
            code_file_name,
            name.get_name(),
            code_seg,
            data_seg,
            segkind,
            "procedure setchain(tytle: string);\n"
            "procedure setcval(val: string);\n"
            "procedure getcval(var val: string);\n"
            "procedure swapon;\n"
            "procedure swapoff;\n"
        );
        return;
    }
    interface_names_by_pascal_id.assign("transcen", "transcendental");
    if (pname == "transcen")
    {
        rcstring code_file_name = "system.library";
        int code_seg = 29;
        int data_seg = -1;
        segkind_t segkind = LINKED_INTRINS;
        pascal_lex_include_unit
        (
            code_file_name,
            name.get_name(),
            code_seg,
            data_seg,
            segkind,
            "function sin(x: real): real;\n"
            "function cos(x: real): real;\n"
            "function exp(x: real): real;\n"
            "function atan(x: real): real;\n"
            "function ln(x: real): real;\n"
            "function log(x: real): real;\n"
            "function sqrt(x: real): real;\n"
        );
        return;
    }
    interface_names_by_pascal_id.assign("longinti", "longintio");
    if (pname == "longinti")
    {
        rcstring code_file_name = "system.library";
        int code_seg = 30;
        int data_seg = -1;
        segkind_t segkind = LINKED_INTRINS;
        pascal_lex_include_unit
        (
            code_file_name,
            name.get_name(),
            code_seg,
            data_seg,
            segkind,
            "type\n"
            "    decmax = integer[36];\n"
            "    stunt =\n"
            "        record case integer of\n"
            "            2: (w2: integer[4]);\n"
            "            3: (w3: integer[8]);\n"
            "            4: (w4: integer[12]);\n"
            "            5: (w5: integer[16]);\n"
            "            6: (w6: integer[20]);\n"
            "            7: (w7: integer[24]);\n"
            "            8: (w8: integer[28]);\n"
            "            9: (w9: integer[32]);\n"
            "            10: (w10: integer[36])\n"
            "        end;\n"
            "\n"
            "procedure freaddec(var f: fib; var d: stunt; l: integer);\n"
            "procedure fwritedec(var f: fib; d: decmax; rleng: integer);\n"
        );
        return;
    }
    interface_names_by_pascal_id.assign("pascalio", "pascalio");
    if (pname == "pascalio")
    {
        rcstring code_file_name = "system.library";
        int code_seg = 31;
        int data_seg = -1;
        segkind_t segkind = LINKED_INTRINS;
        pascal_lex_include_unit
        (
            code_file_name,
            name.get_name(),
            code_seg,
            data_seg,
            segkind,
            "procedure fseek(var f: fib; recnum: integer);\n"
            "procedure freadreal(var f: fib; var x: real);\n"
            "procedure fwritereal(var f: fib; x: real; w, d: integer);\n"
        );
        return;
    }

    //
    // I give up.
    //
    rcstring fpname = interface_names_by_pascal_id.query_fuzzy(pname);
    if (!fpname.empty())
    {
        pascal_lex_error
        (
            name.get_location(),
            "unit %s unknown, but %s is similar",
            name.get_name().quote_c().c_str(),
            fpname.quote_c().c_str()
        );
    }
    else
    {
        pascal_lex_error
        (
            name.get_location(),
            "unit %s unknown",
            name.get_name().quote_c().c_str()
        );
    }
}


int
translator::uses_unitseg(const rcstring &, int segnum)
{
    //
    // In a compiler (or something that must behave like a compiler)
    // we have to use a different segment number if the given segment
    // number has been used.
    //
    // As a default, return the same segment number.
    //
    return segnum;
}


void
translator::uses_interface_push(const rcstring &name, int code_seg,
    int data_seg, int isegkind)
{
    assert(code_seg > 0);
    assert(data_seg > 0 || data_seg == -1);
    segkind_t segkind = (segkind_t)isegkind;
    if (data_seg >= 64)
    {
        pascal_lex_error
        (
            pascal_lex_location(),
            "segment number %d out of range",
            code_seg
        );
    }

    scope::pointer sp =
        interface_scope_factory(name, code_seg, data_seg, segkind);
    scopes.push_back(sp);

    uses_interface_notify_push(sp);
}


void
translator::uses_interface_notify_push(const scope::pointer &)
{
}


void
translator::uses_interface_pop()
{
    scope::pointer sp = scopes.back();
    scopes.pop_back();
    assert(sp->get_lex_level_height() == 0);
    scopes.push_front(sp);

    uses_interface_notify_pop(sp);
}


void
translator::uses_interface_notify_pop(const scope::pointer &)
{
}


rcstring_list translator::library_search_path;
bool translator::library_search_path_env = false;


void
translator::library_path(const rcstring &path)
{
    assert(!library_search_path_env);
    library_search_path.push_back(path);
}


// vim: set ts=8 sw=4 et :

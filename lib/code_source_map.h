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

#ifndef LIB_CODE_SOURCE_MAP_H
#define LIB_CODE_SOURCE_MAP_H

#include <lib/discolumns.h>
#include <lib/input.h>
#include <lib/location.h>
#include <lib/mtype.h>
#include <lib/symtab/template.h>

class codefile; // forward
class symbol; // forward

/**
  * The code_source_map class is used to represent the mapping from a
  * code address to a source code location.  This also assists with
  * disassembling code, including annotated compiler listing with source
  * code and opcodes.
  */
class code_source_map
{
public:
    typedef boost::shared_ptr<code_source_map> pointer;

    /**
      * The destructor.
      */
    virtual ~code_source_map();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param filename
      *     The name of the file to print the listing to.
      */
    static pointer create(const rcstring &filename);

    /**
      * The associate method is used to associate a code address (within
      * a segment) with a source file location (by name and line number).
      *
      * @param segnum
      *     The segment number of the code
      * @param address
      *     The address of the code, within the segment.
      * @param locn
      *     The source code location (filename and line number) of the
      *     code at that location.
      */
    void associate(unsigned segnum, unsigned address, const location &locn);

    /**
      * The list_to_here method is used to copy source code to the
      * listing.  It's called by the disassembler just before it prints
      * the address and the label.
      *
      * @param segnum
      *     The segment number of the code
      * @param address
      *     The address of the code, within the segment.
      */
    void list_to_here(unsigned segnum, unsigned address);

    /**
      * The disassemble method is used to disassemble the code for the
      * given procedure, interleaving sourec code as appropriate.
      *
      * @param mtype
      *     The machine type to be disassembled
      * @param data
      *     The base of the array containg segment code.
      * @param enter_ic
      *     The start of the procedure within the code array.
      * @param data_size
      *     The end of the procedure attributes within the code array.
      * @param seg_num
      *     The segment of the segment being constructed.
      * @param proc_number
      *     The procedure number of the procedure just compiled.
      * @param cfp
      *     The containing codefile
      */
    void disassemble(mtype_t mtype, const unsigned char *data,
        unsigned enter_ic, unsigned data_size, int seg_num,
        unsigned proc_number, codefile *cfp);

    /**
      * The forget_associations method is used to clear the association
      * data.  This can be used if the list is generated procedure by
      * procedure, rather than after all code has been generated, but
      * that uses more memory.
      */
    void forget_associations(void);

    /**
      * The symbol_listing method is used to produce a listing for the
      * given symbol.
      */
    void symbol_listing(const symbol &sym);

private:
    /**
      * The report instance variable is used to remember the columnar
      * output for writing out the disassembly.
      */
    discolumns report;

    typedef boost::shared_ptr<location> location_p;

    symtab<location_p> location_by_name_and_line;

    symtab<location_p> location_by_segnum_and_addr;

    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param report
      *     The columnar output for writing out the disassembled code.
      */
    code_source_map(discolumns &report);

    /**
      * The progress class is used to represent the progress to date in
      * printing a source file.
      */
    class progress
    {
    public:
        typedef boost::shared_ptr<progress> pointer;
        ~progress();
        static pointer create(const rcstring &file_name);
        int get_line_number(void);
        void list_one_line(discolumns &report);
    private:
        progress(const rcstring &file_name);
        rcstring base;
        int line_number;
        input::pointer ip;
    };

    symtab<progress::pointer> progress_by_filename;

    /**
      * The default constructor.  Do not use.
      */
    code_source_map();

    /**
      * The copy constructor.  Do not use.
      *
      * @param filename
      *     The file to write the listing to.
      */
    code_source_map(const rcstring &filename);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    code_source_map &operator=(const code_source_map &rhs);
};

#endif // LIB_CODE_SOURCE_MAP_H
// vim: set ts=8 sw=4 et :

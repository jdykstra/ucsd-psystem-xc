//
// UCSD p-System cross compiler
// Copyright (C) 2011, 2012 Peter Miller
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

#ifndef LIB_DISASSEMBLER_PCODE_I_3_H
#define LIB_DISASSEMBLER_PCODE_I_3_H

#include <lib/disassembler/pcode.h>

/**
  * The disassembler_pcode_i_3 class is used to represent the processing
  * required to disassemble I.3 p-code procedures within a code segment.
  */
class disassembler_pcode_i_3:
    public disassembler_pcode
{
public:
    typedef boost::shared_ptr<disassembler_pcode_i_3> pointer;

    /**
      * The destructor.
      */
    virtual ~disassembler_pcode_i_3();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(byte_sex_t bs, dislabel &where, discolumns &report,
        int seg_num, int proc_num, int proc_base, code_source_map *listing);

protected:
    // See base class for documentation.
    unsigned disassemble_one_instruction(const unsigned char *data,
        unsigned address, size_t data_size, int procedure_number);

    // See base class for documentation.
    void preprocess(const unsigned char *data, unsigned entry_ic,
        unsigned exit_ic, size_t data_size);

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    disassembler_pcode_i_3(byte_sex_t bs, dislabel &where, discolumns &report,
        int seg_num, int proc_num, int proc_base, code_source_map *listing);

private:
    unsigned entry_ic;
    unsigned exit_ic;
    unsigned jump_table_ic;
    unsigned proc_attr_table_offset;

    /**
      * The default constructor.  Do not use.
      */
    disassembler_pcode_i_3();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    disassembler_pcode_i_3(const disassembler_pcode_i_3 &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    disassembler_pcode_i_3 &operator=(const disassembler_pcode_i_3 &rhs);
};

#endif // LIB_DISASSEMBLER_PCODE_I_3_H
// vim: set ts=8 sw=4 et :

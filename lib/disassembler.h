//
// UCSD p-System cross compiler
// Copyright (C) 2010, 2012 Peter Miller
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

#ifndef LIB_DISASSEMBLER_H
#define LIB_DISASSEMBLER_H

#include <boost/shared_ptr.hpp>

#include <lib/byte_sex.h>
#include <lib/discolumns.h>
#include <lib/dislabel.h>
#include <lib/mtype.h>

class code_source_map; // forward

/**
  * The disassembler class is used to represent an abstract machine code
  * disassembler.  It could be p-code, it could be 6502 code, etc.
  */
class disassembler
{
public:
    typedef boost::shared_ptr<disassembler> pointer;

    /**
      * The destructor.
      */
    virtual ~disassembler();

    /**
      * The factory class method is used to create a new dynamically
      * allocated disassember instance, based on the machine type of the
      * segment.
      *
      * @param mtype
      *     The machine type, used to determine which class to use to
      *     instancaite a concrete disassembler instance.
      * @param where
      *     A database of addresses and corresponding label names.
      * @param report
      *     The columnar output for writing out the disassembled code.
      * @param seg_num
      *     The segment number of the segment being disassembled.
      * @param proc_num
      *     The procedure number of the procedure being disassembled.
      * @param proc_base
      *     The base address of the procedure being disassembled.
      * @param listing
      *     the distination for the listing, or NULL if no source code
      *     to be interleaved.
      */
    static pointer factory(mtype_t mtype, dislabel &where, discolumns &report,
        int seg_num, int proc_num, int proc_base, code_source_map *listing);

public:
    /**
      * The process method is used to disassemble an entire procedure.
      *
      * @param data
      *     Pointer to the base of the segment data
      * @param address
      *     The address of the start of the code within the segment
      *     data, in bytes.
      * @param data_size
      *     The size of the procedure, in bytes, including the procedure
      *     attributes.
      * @param procedure_number
      *     The number of the procedure being disassembled.
      */
    void process(const unsigned char *data, unsigned &address, size_t data_size,
        unsigned procedure_number);

    /**
      * The print_addresses method is used to select whether
      * or not to print addresses in the address column of the listing.
      * Default to true if you cont call this method.
      *
      * @param yesno
      *     true for addresses rto be printed, false if not
      */
    void print_addresses(bool yesno);

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      *
      * @param byte_sex
      *     The endian-ness of this code segment
      * @param where
      *     A database of addresses and corresponding label names.
      * @param report
      *     The columnar output for writing out the disassemble.
      * @param segnum
      *     The segment number the code is in.
      * @param proc_num
      *     The procedure number of the procedure being disassembled.
      * @param proc_base
      *     The base address of the procedure being disassembled.
      * @param listing
      *     where to get the source code from, to interleave with the
      *     disassembly.
      */
    disassembler(byte_sex_t byte_sex, dislabel &where, discolumns &report,
        int segnum, int proc_num, int proc_base, code_source_map *listing);

    /**
      * The disassemble_one_instruction method is used to actually disassemble.
      *
      * @param data
      *     pointer to the data for this procedure or function.
      *     It is <b>not</b> all the data for the segment.
      * @param address
      *     The address, relative to data, of the instruction of interest.
      * @param data_size
      *     The size of the data segment, including the jump table, and
      *     including the procedure attributes.
      * @param procedure_number
      *     The number of the procedure (1..255) being disassembled
      * @returns
      *     The number of bytes in the instruction, or 0 if not valid an
      *     nothing printed.
      */
    virtual unsigned disassemble_one_instruction(const unsigned char *data,
        unsigned address, size_t data_size, int procedure_number) = 0;

    /**
      * The get_word method is used to obtain a 16-bit word value,
      * taking the byte sex into account.
      *
      * @param data
      *     Pointer to the two bytes to be decoded
      * @returns
      *     16-bit decoded value
      */
    unsigned get_word(const unsigned char *data) const;

    /**
      * The is_native method is sued to determine whether or not this is
      * a native code disassembler (meaning: not p-code).
      */
    virtual bool is_native(void) const = 0;

    /**
      * The where instance variable is used to remember the database of
      * addresses and corresponding label names.
      */
    dislabel &where;

    /**
      * The report instance variable is used to remember the columnar
      * output for writing out the disassembly.
      */
    discolumns &report;

private:
    /**
      * The byte_sex instance variable is used to remember the
      * endian-ness of this code segment.
      */
    byte_sex_t byte_sex;

    /**
      * The no_addr instance variable is used to remember whether or not
      * to print addresses in the address column.
      */
    bool no_addr;

    /**
      * The segnum instance variable is used to remember the segment
      * number of the segment being disassembled.
      */
    int segnum;

    /**
      * The proc_num instance variable is used to remember the procedure
      * number of the procedure being disassembled.
      */
    int proc_num;

    /**
      * The proc_base instance variable is used to remember the base
      * address (enter_ic) of the procedure being disassembled.
      */
    int proc_base;

    /**
      * The listing instance variable is used to remember the listing
      * destination, or NULL if no listing.
      */
    code_source_map *listing;

    /**
      * The print_address method is used to print the addrss given in
      * the address column (heeding the #no_addr flag.  It also prints
      * the label, if one should be present.
      *
      * @param addr
      *     The address to be printed in the address column.
      */
    void print_address(unsigned addr) const;

    /**
      * The preprocess method is used to make a first pass over the
      * code, looking for the locations of the labels within the code.
      *
      * @param data
      *     The base of the data for the segment
      * @param entry_ic
      *     The offset of the first byte of the procedure code, in
      *     bytes, relative to data.
      * @param exit_ic
      *     The offset of the first byte beyond the end of the procedure
      *     code (machine code), or the address of the exit code
      *     (p-code), relative to data.
      * @param data_size
      *     The size of the data (actually, the address, relative to
      *     data, of the first byte beywond the procedure attributes).
      */
    virtual void preprocess(const unsigned char *data, unsigned entry_ic,
        unsigned exit_ic, size_t data_size) = 0;

private:
    void find_reloc(const unsigned char *data, unsigned lo, unsigned &hi,
        unsigned &r) const;

    void find_relocs(const unsigned char *data, unsigned lo, unsigned hi,
        unsigned r[4]) const;

    /**
      * The default constructor.  Do not use.
      */
    disassembler();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    disassembler(const disassembler &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    disassembler &operator=(const disassembler &rhs);
};

#endif // LIB_DISASSEMBLER_H
// vim: set ts=8 sw=4 et :

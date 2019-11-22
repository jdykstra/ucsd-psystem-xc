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

#ifndef LIB_DISASSEMBLER_PDP11_H
#define LIB_DISASSEMBLER_PDP11_H

#include <lib/disassembler.h>

/**
  * The disassembler_pdp11 class is used to represent the processing
  * required to disassemble PDP-11 (LSI-11) procedures within code
  * segments.
  */
class disassembler_pdp11:
    public disassembler
{
public:
    typedef boost::shared_ptr<disassembler_pdp11> pointer;

    /**
      * The destructor.
      */
    virtual ~disassembler_pdp11();

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    disassembler_pdp11(dislabel &where, discolumns &report, int segnum,
        int proc_num, int proc_base, code_source_map *listing);

public:
    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(dislabel &where, discolumns &report, int segnum,
        int proc_num, int proc_base, code_source_map *listing);

protected:
    // See base class for documentation.
    unsigned disassemble_one_instruction(const unsigned char *data,
        unsigned address, size_t data_size, int procedure_number);

    // See base class for documentation.
    void preprocess(const unsigned char *data, unsigned entry_ic,
        unsigned exit_ic, size_t data_size);

    // See base class for documentation.
    bool is_native(void) const;

private:
    unsigned highwater;
    unsigned short pc;
    const unsigned char *memory;

    unsigned short get_next_word(void);

    void pcoper(const output::pointer &op, unsigned short reg);
    void pfoper(const output::pointer &op, unsigned short reg);
    void prac(const output::pointer &op, unsigned short reg);
    void prfpac(const output::pointer &op, unsigned short reg);
    void prlab(const output::pointer &op, unsigned short addr);
    void prop1(const output::pointer &op, unsigned short reg);
    void proper(const output::pointer &op, unsigned short reg);
    void prreg(const output::pointer &op, unsigned short reg);
    void prrgp(const output::pointer &op, unsigned short reg);

    void prep_pcoper(unsigned short reg);
    void prep_pfoper(unsigned short reg);
    void prep_prfpac(unsigned short reg);
    void prep_prlab(unsigned short addr);
    void prep_prop1(unsigned short reg);
    void prep_proper(unsigned short reg);

    /**
      * The default constructor.  Do not use.
      */
    disassembler_pdp11();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    disassembler_pdp11(const disassembler_pdp11 &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    disassembler_pdp11 &operator=(const disassembler_pdp11 &rhs);
};

#endif // LIB_DISASSEMBLER_PDP11_H
// vim: set ts=8 sw=4 et :

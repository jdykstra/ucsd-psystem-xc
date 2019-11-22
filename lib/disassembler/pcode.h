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

#ifndef LIB_DISASSEMBLER_PCODE_H
#define LIB_DISASSEMBLER_PCODE_H

#include <lib/disassembler.h>
#include <lib/p_machine.h>

class code_source_map; // forward

/**
  * The disassembler_pcode class is used to represent the processing
  * required to disassemble p-code procedures within a code segment.
  */
class disassembler_pcode:
    public disassembler
{
public:
    typedef boost::shared_ptr<disassembler_pcode> pointer;

    /**
      * The destructor.
      */
    virtual ~disassembler_pcode();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class (well, a derived class, actually).
      */
    static pointer create(byte_sex_t bs, dislabel &where, discolumns &report,
        int seg_num, int proc_num, int proc_base, code_source_map *listing,
        p_machine_t pm);

protected:
    /**
      * The constructor.
      * For use by derived classes only
      */
    disassembler_pcode(byte_sex_t bs, dislabel &where, discolumns &report,
        int seg_num, int proc_num, int proc_base, code_source_map *listing);

    // See base class for documentation.
    bool is_native(void) const;

private:
    /**
      * The default constructor.  Do not use.
      */
    disassembler_pcode();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    disassembler_pcode(const disassembler_pcode &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    disassembler_pcode &operator=(const disassembler_pcode &rhs);
};

#endif // LIB_DISASSEMBLER_PCODE_H
// vim: set ts=8 sw=4 et :

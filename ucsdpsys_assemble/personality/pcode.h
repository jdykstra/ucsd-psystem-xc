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

#ifndef UCSDPSYS_ASSEMBLE_PERSONALITY_PCODE_H
#define UCSDPSYS_ASSEMBLE_PERSONALITY_PCODE_H

#include <ucsdpsys_assemble/personality.h>

/**
  * The personality_pcode class is used to represent
  * the set of p-machine opcodes.
  */
class personality_pcode:
    public personality
{
public:
    typedef boost::shared_ptr<personality_pcode> pointer;

    /**
      * The destructor.
      */
    virtual ~personality_pcode();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      *
      * @param context
      *     The assembly context we are working within.
      * @param bs
      *     The desired byte sex.  Defaults to little-endian.
      */
    static pointer create(assembler *context, byte_sex_t bs = little_endian);
    static personality::pointer create_le(assembler *context);
    static personality::pointer create_be(assembler *context);

protected:
    // See base class for documentation.
    byte_sex_t get_byte_sex(void) const;

    // See base class for documentation.
    mtype_t get_mtype(void) const;

    // See base class for documentation.
    int get_default_radix(void) const;

    // See base class for documentation.
    unsigned char get_one_byte_nop(void) const;

private:
    /**
      * The context instance variable is used to remember
      * the assembly context we are working within.
      */
    assembler *context;

    /**
      * The endian instance variable is used to rememeber the byte sex
      * requested.  Most CPU architectures are either big-endian or
      * little-endian, but p-code can be either, because that way the
      * host interpreter can do some operations natively, for greater
      * speed.
      */
    byte_sex_t endian;

    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      *
      * @param context
      *     The assembly context we are working within.
      * @param bs
      *     The desired byte sex.  Defaults to little-endian.
      */
    personality_pcode(assembler *context, byte_sex_t bs);

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    personality_pcode(const personality_pcode &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    personality_pcode &operator=(const personality_pcode &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_PERSONALITY_PCODE_H
// vim: set ts=8 sw=4 et :

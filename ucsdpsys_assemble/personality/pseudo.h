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

#ifndef UCSDPSYS_ASSEMBLE_PERSONALITY_PSEUDO_H
#define UCSDPSYS_ASSEMBLE_PERSONALITY_PSEUDO_H

#include <ucsdpsys_assemble/personality.h>

class assembler; // forward

/**
  * The personality_pseudo class is used to represent
  * the set of all of the machine-neutral pseudo opcodes.
  */
class personality_pseudo:
    public personality
{
public:
    typedef boost::shared_ptr<personality_pseudo> pointer;

    /**
      * The destructor.
      */
    virtual ~personality_pseudo();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static pointer create(assembler *context);

protected:
    // See base class for documentation.
    byte_sex_t get_byte_sex(void) const;

    // See base class for documentation.
    mtype_t get_mtype(void) const;

    // See base class for documentation.
    unsigned char get_one_byte_nop(void) const;

private:
    /**
      * The constructor.
      * It is private on purpose, use a #create class method instead.
      */
    personality_pseudo(assembler *context);

    /**
      * The default constructor.  Do not use.
      */
    personality_pseudo();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    personality_pseudo(const personality_pseudo &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    personality_pseudo &operator=(const personality_pseudo &rhs);
};

#endif // UCSDPSYS_ASSEMBLE_PERSONALITY_PSEUDO_H
// vim: set ts=8 sw=4 et :

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

#ifndef UCSDPSYS_DEPENDS_LANGUAGE_ASSEMBLER_H
#define UCSDPSYS_DEPENDS_LANGUAGE_ASSEMBLER_H

#include <ucsdpsys_depends/language.h>

/**
  * The language_assembler class is used to represent the processing
  * required to parse assembler files looking for include dependencies.
  */
class language_assembler:
    public language
{
public:
    typedef boost::shared_ptr<language_assembler> pointer;

    /**
      * The destructor.
      */
    virtual ~language_assembler();

    /**
      * The create class method is used to create new dynamically
      * allocated instances of this class.
      */
    static language::pointer create(void);

    // See base class for documentation.
    void depends(const rcstring &infile);

private:
    void look_for_include(const rcstring &line);

    /**
      * The default constructor.
      * It is private on purpose, use a #create class method instead.
      */
    language_assembler();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    language_assembler(const language_assembler &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    language_assembler &operator=(const language_assembler &rhs);
};

#endif // UCSDPSYS_DEPENDS_LANGUAGE_ASSEMBLER_H
// vim: set ts=8 sw=4 et :

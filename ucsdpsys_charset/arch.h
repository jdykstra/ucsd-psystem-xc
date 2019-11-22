//
// UCSD p-System cross compiler
// Copyright (C) 2012 Peter Miller
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

#ifndef UCSDPSYS_CHARSET_ARCH_H
#define UCSDPSYS_CHARSET_ARCH_H

#include <boost/shared_ptr.hpp>

#include <lib/mtype.h>
#include <lib/rcstring.h>

/**
  * The arch class is used to represent architecture-specific actions to
  * be performed by the command.  There are a frew different formats to
  * the SYSTEM.CHARSET file, and this is how they are selected for.
  */
class arch
{
public:
    typedef boost::shared_ptr<arch> pointer;

    /**
      * The destructor.
      */
    virtual ~arch();

    /**
      * The factory_by_arch class method is used to manufacture suitable
      * derived instances, based on the name of the architecture given.
      *
      * @param name
      *     The name of the architecture or interest (e.g. "6502").
      *     If error, will exit via fatal error message.
      * @returns
      *     pointer to class instance, never NULL.
      */
    static pointer factory_by_arch(const rcstring &name);

    /**
      * The factory_by_host class method is used to manufacture suitable
      * derived instances, based on the brand name of the host given.
      *
      * @param name
      *     The name of the host or interest (e.g. "apple").
      *     If error, will exit via fatal error message.
      * @returns
      *     pointer to class instance, never NULL.
     */
    static pointer factory_by_host(const rcstring &name);

    virtual rcstring get_binary_format_name(void) const;

    virtual rcstring get_text_format_name(void) const;

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param mtype
      *     The machine type we are working within.
      */
    arch(mtype_t mtype);

private:
    /**
      * The mtype instance variable is used to remember the machine type
      * we are working within.
      */
    mtype_t mtype;

    /**
      * The factory_by_mtype class method is used to manufacture
      * suitable derived instances.  The #factory_by_arch and
      * #factory_by_host methods both come here.
      *
      * @param mtype
      *     The machine type.
      *     If error, will exit via fatal error message.
      * @returns
      *     pointer to class instance, never NULL.
      */
    static pointer factory_by_mtype(mtype_t mtype);

    /**
      * The default constructor.
      * Do not use.
      */
    arch();

    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    arch(const arch &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    arch &operator=(const arch &rhs);
};

#endif // UCSDPSYS_CHARSET_ARCH_H
// vim: set ts=8 sw=4 et :

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

#ifndef LIB_OUTPUT_FILTER_H
#define LIB_OUTPUT_FILTER_H

#include <lib/output.h>

/**
  * The output_filter class is used to represent a generic filter that
  * passes eveything to the "deeper" stream.
  */
class output_filter:
    public output
{
public:
    typedef boost::shared_ptr<output_filter> pointer;

    /**
      * The destructor.
      */
    virtual ~output_filter();

protected:
    /**
      * The constructor.
      * For use by derived classes only.
      *
      * @param deeper
      *     The deeper stream to write our filtered output to.
      */
    output_filter(const output::pointer &deeper);

    // See base class for documentation.
    rcstring filename(void);

    // See base class for documentation.
    void utime(struct utimbuf &utb);

    // See base class for documentation.
    unsigned get_width(void) const;

    // See base class for documentation.
    unsigned get_height(void) const;

    // See base class for documentation.
    void write_inner(const void *data, size_t length);

    // See base class for documentation.
    void flush_inner(void);

    // See base class for documentation.
    void set_heading1(const rcstring &text);

    // See base class for documentation.
    void set_heading2(const rcstring &text);

    void deeper_putc(char c) { deeper->fputc(c); }

    void deeper_puts(const char *text) { deeper->fputs(text); }

    void deeper_puts(const rcstring &text) { deeper->fputs(text); }

    void
    deeper_write(const void *data, size_t length)
    {
        deeper->write(data, length);
    }

private:
    /**
      * The deeper instance variable is used to remember the deeper
      * stream to write our filtered output to.
      */
    output::pointer deeper;

    /**
      * The default constructor.  Do not use.
      */
    output_filter();

    /**
      * The copy constructor.  Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    output_filter(const output_filter &rhs);

    /**
      * The assignment operator.  Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    output_filter &operator=(const output_filter &rhs);
};

#endif // LIB_OUTPUT_FILTER_H
// vim: set ts=8 sw=4 et :

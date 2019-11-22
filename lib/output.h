//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
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

#ifndef LIB_OUTPUT_H
#define LIB_OUTPUT_H

#include <lib/ac/stdarg.h>
#include <lib/ac/stddef.h>
#include <boost/shared_ptr.hpp>

#include <lib/format_printf.h>

class rcstring; // forward


/**
  * The output class is used to describe the interface to an
  * arbitrary output destination.  It could be a file, it could be a
  * string, or many other things, including several filters.
  *
  * Abstract output instances are usually referred to by output::pointer
  * instances.
  */
class output
{
public:
    typedef boost::shared_ptr<output> pointer;

    /**
      * The destructor.
      */
    virtual ~output();

protected:
    /**
      * The default constructor.
      *
      * The default constructor is protected, because only derived
      * classes may be created by users of this interface.
      */
    output();

public:
    /**
      * The factory class method is used to open a file or the standard
      * output file.
      *
      * @param file_name
      *     The name of the file to open, or "-" if the standard output
      *     is to be used.
      * @returns
      *     smart pointer to the output stream, used to automagically
      *     close the file when finished.
      */
    static output::pointer factory(const rcstring &file_name);

    /**
      * The filename method is used to obtain the filename of this output.
      */
    virtual rcstring filename(void) = 0;

    /**
      * The write method is used to write date to the output.
      */
    void write(const void *data, size_t nbytes);

    /**
      * The flush method is used to ensure that any buffered data is
      * written to the output.
      */
    void flush(void);

    /**
      * The fputc method is used to write a single character to the
      * output.  The output is buffered.
      */
    void
    fputc(char c)
    {
        if (buffer_position < buffer_end)
            *buffer_position++ = c;
        else
            overflow(c);
    }

    /**
      * The fputs method is used to write a NUL terminated string to the
      * output stream.
      *
      * @param str
      *     The string to be written out.
      */
    void fputs(const char *str);

    /**
      * The fputs method is used to write a string to the output stream.
      *
      * @param str
      *     The string to be written out.
      */
    void fputs(const rcstring &str);

    /**
      * The fprintf method produces output according to
      * a format as described in the printf(3) man page.
      *
      * \param fmt
      *     This method writes the output under the control of a format
      *     string that specifies how subsequent arguments (or arguments
      *     accessed via the variable-length argument facilities of
      *     stdarg(3)) are converted for output.
      */
    void fprintf(const char *fmt, ...)                      FORMAT_PRINTF(2, 3);

    /**
      * The vfprintf method is equivalent to the fprintf method, except
      * that it is called with a va_list instead of a variable number
      * of arguments.  This method does not call the va_end macro.
      * Consequently, the value of ap is undefined after the call.  The
      * application should call va_end(ap) itself afterwards.
      */
    void vfprintf(const char *fmt, va_list)                 FORMAT_PRINTF(2, 0);

    /**
      * The utime method mau be used to change the access and
      * modification times of this file.
      *
      * @param utb
      *     The buffer containing the times to set.
      */
    virtual void utime(struct utimbuf &utb) = 0;

    /**
      * The get_width method is used to obtain the width of the output,
      * in character columns.
      */
    virtual unsigned get_width(void) const = 0;

    /**
      * The get_height method is used to obtain the height of the
      * output, in lines per page.
      */
    virtual unsigned get_height(void) const = 0;

    /**
      * The set_heading1 method is used to set the first line of the
      * header, if there is a header
      *
      * @param text
      *     The text of the first line.
      */
    virtual void set_heading1(const rcstring &text);

    /**
      * The set_heading2 method is used to set the second line of the
      * header, if there is a header.
      *
      * @param text
      *     The text of the second line.
      */
    virtual void set_heading2(const rcstring &text);

private:
    /**
      * The write_inner method is used write data to the output, without
      * taking the buffering into account.
      */
    virtual void write_inner(const void *data, size_t length) = 0;

    /**
      * The flush_inner method is called by the flush method once all
      * the data has been written.
      */
    virtual void flush_inner(void) = 0;

    /**
      * The overflow mwthod is used by the fputc method when the buffer
      * is full.  It write the buffer to the output, and then adds the
      * single character to the buffer.
      */
    void overflow(char c);

    /**
      * The buffer instance variable is used to remember the base of a
      * dynamically allocated array of characters used to buffer the
      * data, to minimize the number of systems calls required.
      */
    unsigned char *buffer;

    /**
      * The buffer_size instance variable is used to remember the number
      * of characters allocated in the buffer array.
      */
    size_t buffer_size;

    /**
      * The buffer_position instance variable is used to remember the
      * current output position withing the output buffer.
      */
    unsigned char *buffer_position;

    /**
      * The buffer_end instance variable is used to remember the end of
      * the dynamically allocated buffer array.
      */
    unsigned char *buffer_end;

    /**
      * The copy constructor.  Do not use.
      */
    output(const output &);

    /**
      * The assignment operator.  Do not use.
      */
    output &operator=(const output &);
};

#endif // LIB_OUTPUT_H
// vim: set ts=8 sw=4 et :

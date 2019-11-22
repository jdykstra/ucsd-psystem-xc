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

#ifndef LIB_INPUT_H
#define LIB_INPUT_H

#include <boost/shared_ptr.hpp>

#include <lib/rcstring.h>

/**
  * The input abstract class is used to represent a generic input source.
  *
  * Usually pointers to input instances are managed using an
  * input::pointer object.
  */
class input
{
public:
    typedef boost::shared_ptr<input> pointer;

    /**
      * The destructor is used to close the given input, and delete all
      * resources associated with it.  Once this returns, the given
      * input is no longer available for *any* use.  (Think of this
      * method as "close" if it helps.)
      */
    virtual ~input();

protected:
    /**
      * The default constructor.
      * Only for the use of derived classes.
      */
    input();

public:
    /**
      * The factory class method is used to open the given path.  It
      * understands "-" for stdin.  It can detect raw UCSD p-system text
      * files, too.
      *
      * @param path
      *     The pathname of the file to be opened.
      * @returns
      *     pointer to new file instance
      */
    static input::pointer factory(const rcstring &path);

    /**
      * The read method is used to read buffered data from the given
      * input stream.  At most \a size bytes will be read into
      * \a buffer.  The number of bytes actually read will be returned.
      * At end-of-file, a value <=0 will be returned, and \a buffer will
      * be unchanged.  All file read errors or format errors are fatal,
      * and will cause the method to not return.
      *
      * @param data
      *     Where to put the results of the read.
      * @param nbytes
      *     The maximum number of bytes to read.
      * @returns
      *     The actual number of bytes read, or zero for end-of-file.
      */
    long read(void *data, size_t nbytes);

    /**
      * The read_strictest method is used to read data from the given
      * input stream.  Exactly \a size bytes will be read into
      * \a buffer.  If there are less than \a size bytes available, a
      * fatal error will result.
      *
      * @param data
      *     Where to put the results of the read.
      * @param size
      *     The number of bytes to read.
      */
    void read_strictest(void *data, size_t size);

    /**
      * The read_strict method is used to read data from the given input
      * stream.  Exactly \a size bytes will be read into \a buffer, or
      * zero bytes at end-of-file.  If there are less than \a size bytes
      * available, a fatal error will result.
      *
      * @param data
      *     Where to put the results of the read.
      * @param size
      *     The number of bytes to read.
      * @returns
      *     bool; true if data was read, false if end-of-file
      */
    bool read_strict(void *data, size_t size);

    /**
      * The skip method is used to read data from the given input
      * stream and discard it.  Exactly \a size bytes will be read.  If
      * there are less than \a size bytes available, a fatal error will
      * result.
      *
      * @param size
      *     The number of bytes to discard.
      */
    void skip(size_t size);

    /**
      * The fatal_error method is used to report a fatal error on
      * an input stream.  This method does not return.
      */
    void fatal_error(const char *msg);

    /**
      * The one_line method is used to read one line from the input
      * (up to the next newline character or end of input).
      *
      * @param result
      *    Where the text is stored.  The newline is not included in the
      *    returned string.
      * @returns
      *    bool; true if any text was read, false if end-of-file is reached.
      */
    bool one_line(rcstring &result);

    /**
      * The name method is used to determine the name of the input.
      */
    virtual rcstring name(void) = 0;

    /**
      * The length method is used to determine the length of the input.
      * May return -1 if the length is unknown.
      */
    virtual long length(void) = 0;

    /**
      * The getch method is used to get the next character from
      * the input.  Returns a value<=0 at end-of-file.
      */
    int
    getch(void)
    {
        if (buffer_position < buffer_end)
            return *buffer_position++;
        return getc_complicated();
    }

    /**
      * The ungetc method is used to push back a character of input.
      * Think of it as undoing the effects of the getch method.
      */
    void
    ungetc(int c)
    {
        if (c >= 0)
        {
            if (buffer_position > buffer)
                *--buffer_position = c;
            else
                ungetc_complicated(c);
        }
    }

    /**
      * The peek method is used to obtain the value of the next input
      * character, without advancing the read position.
      *
      * @returns
      *     the next character, or -1 if the end of file has been reached.
      */
    int
    peek(void)
    {
        int c = getch();
        ungetc(c);
        return c;
    }

    /**
      * The pushback_transfer method is used by input filter classes'
      * destructors to return unused buffeered input.
      */
    void pushback_transfer(input &from);

    /**
      * The pullback_transfer method is used by input filter classes'
      * destructors to return unused buffeered input.
      */
    void pullback_transfer(input *to);

    /**
      * The pullback_transfer method is used by input filter classes'
      * destructors to return unused buffeered input.
      */
    void pullback_transfer(input &to);

    /**
      * The unread method may be used to reverse the effects of the read
      * method.  The data is pushed into the buffer.  Think of it as a
      * whole bunhs of ungetc calls (backwards).
      *
      * @param data
      *     The base of the array of bytes to be returned.
      * @param nbytes
      *     The number of bytes to be returned.
      */
    void unread(const void *data, size_t nbytes);

    /**
      * The at_end method is used to determine whether or not
      * this input stream is at the end of input.
      */
    bool at_end(void);

    /**
      * The is_remote method is used to determine whether or not an
      * input stream is from a local file or a remote source.  This is
      * only intended to be a generally informative thing, to provide
      * information to the user, it isn't (and can't be) utterly
      * precise.
      */
    virtual bool is_remote(void) const;

    virtual int fpathconf_name_max(void) = 0;
    virtual void fstat(struct stat &) = 0;

protected:
    /**
      * The read_inner method is used to read unbuffered data from the
      * given input stream.  At most \a nbytes bytes will be read into
      * \a data.  The number of bytes actually read will be returned.
      * At end-of-file, a value <= 0 will be returned, and \a data will
      * be unchanged.  All file read errors or format errors are fatal,
      * and will cause the method to not return.
      *
      * @param data
      *     Where to put the results of the read.
      * @param nbytes
      *     The maximum number of bytes to read.
      * @returns
      *     The actual number of bytes read, or zero for end-of-file.
      */
    virtual long read_inner(void *data, size_t nbytes) = 0;

    /**
      * The getc_complicated method is used to get a character from the
      * input.  Usually users do not call this method directly, but
      * use the getch method instead.
      */
    int getc_complicated(void);

    /**
      * The ungetc_complicated method is used to push a character back
      * onto an input.  Usually users do not call this method directly,
      * but use the input_ungetc macro instead.
      */
    void ungetc_complicated(int c);

private:
    /**
      * The buffer instance variable is used to remember the base of
      * a dynamically allocated array used to buffer the data for
      * buffered input.
      */
    unsigned char *buffer;

    /**
      * The buffer_size instance variable is used to remember the
      * allocation size of the dynamically allocated buffer.
      */
    size_t buffer_size;

    /**
      * The buffer_position instance variable is used to remember the
      * read position of the data within the buffer.
      */
    unsigned char *buffer_position;

    /**
      * The buffer_end instance variable is used to remember the
      * highwater mark of data stored in the buffer.
      */
    unsigned char *buffer_end;
};

#endif // LIB_INPUT_H
// vim: set ts=8 sw=4 et :

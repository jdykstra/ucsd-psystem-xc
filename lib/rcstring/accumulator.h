//
// UCSD p-System cross compiler
// Copyright (C) 2006, 2007, 2010, 2012 Peter Miller
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or (at
// your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//

#ifndef LIB_RCSTRING_ACCUMULATOR_H
#define LIB_RCSTRING_ACCUMULATOR_H

#include <lib/rcstring.h>

/**
  * The rcstring_accumulator class is used to accumulate strings into a buffer.
  */
class rcstring_accumulator
{
public:
    /**
      * The destructor.  Thou shalt not derive from this class because
      * it isn't virtual.
      */
    ~rcstring_accumulator();

    /**
      * The default constructor.
      */
    rcstring_accumulator();

    /**
      * The copy constructor.
      */
    rcstring_accumulator(const rcstring_accumulator &);

    /**
      * The assignment operator.
      */
    rcstring_accumulator &operator=(const rcstring_accumulator &);

    /**
      * The mkstr method is used to take the current contents of the
      * accumulated buffer and turn it into a string.
      */
    rcstring mkstr() const;

    /**
      * The push_back method is used to append another character to the
      * end of the accumulator.
      *
      * \param c
      *     The character to be appended to the buffer.
      */
    void
    push_back(char c)
    {
        //
        // The rcstring_accumulator::push_back(char) method shows up
        // in the profiles as occupying 10% of the time it takes to
        // parse the database files.  By making it an inline, things go
        // measurably faster.
        //
        if (length < maximum)
            buffer[length++] = c;
        else
            overflow(c);
    }

    /**
      * The push_back method is used to append an array of characters to
      * the end of the accumulator.
      *
      * \param data
      *     The data to be appended to the buffer.
      * \param len
      *     The number of bytes of data.
      */
    void push_back(const void *data, size_t len);

    /**
      * The push_back method is used to append a string to the end of
      * the accumulator.
      *
      * \param arg
      *     The string to be appended to the buffer.
      */
    void
    push_back(const rcstring &arg)
    {
        push_back(arg.c_str(), arg.size());
    }

    /**
      * The push_back method is used to append a NUL terminated string
      * to the end of the accumulator.
      *
      * \param data
      *     The string to be appended to the buffer.
      */
    void push_back(const char *data);

    /**
      * The push_back method is used to append another string
      * accumulator to the end of this accumulator.
      *
      * \param data
      *     The string to be appended to the buffer.
      */
    void push_back(const rcstring_accumulator &data);

    /**
      * The clear method is used to reset the length of the accumulated
      * string to zero.
      */
    void clear() { length = 0; }

    /**
      * The size method is used to obtain the current size of the buffer
      * in characters.
      */
    size_t size() const { return length; }

    /**
      * The empty method is used to determine if the buffer is empty
      * (has a size of zero).
      */
    bool empty() const { return (length == 0); }

    /**
      * The pop_back method is used to remove the last character from
      * the buffer.
      */
    void pop_back() { if (length) --length; }

    /**
      * The pop_back method is used to remove the first n characters from
      * the buffer.
      *
      * @param n
      *     The number of characters to remove.  Defaults to one.
      */
    void pop_front(size_t n = 1);

    /**
      * The back method may be used to obtain the last character in the
      * buffer, or NUL if the buffer is empty.
      */
    char back() const { return (length ? buffer[length - 1] : '\0'); }

    /**
      * The ends_with method is used to determine whether or not the
      * buffer ends with a particular string.
      */
    bool ends_with(const char *text) const;

    /**
      * The get_data method is used to obtain a pointer to the base of
      * the array of characters being accumulated.
      *
      * \note
      *     The pointer is only garanteed to be valid until the next
      *     push_back method call.
      * \note
      *     Please use this methdo as little as possible.
      */
    const char *get_data() const { return (buffer ? buffer : ""); }

    /**
      * The array index operator is used to obtain thr nth character in
      * the buffer.
      *
      * \note
      *     No array bounds checking is performed.  If you really stuff
      *     up, it will segfault.  Caveat emptor.
      */
    char operator[](size_t n) { return buffer[n]; }

    /**
      * The printf method is used to appand a formatted string to the
      * buffer.  See printf(3) for mor einformation.
      */
    void printf(const char *fmt, ...)                       FORMAT_PRINTF(2, 3);

private:
    /**
      * The overflow method is used to append another character to the
      * end of the accumulator.  It is called by the putch method when
      * it get too too hard (and a new buffer is needed).
      *
      * \param c
      *     The character to be appended to the buffer.
      */
    void overflow(char c);

    /**
      * The length instance variable is used to remember how many
      * characters in the buffer are significant.
      */
    size_t length;

    /**
      * The length instance variable is used to remember the allocated
      * size of the buffer.
      */
    size_t maximum;

    /**
      * The length instance variable is used to remember the base of an
      * array of characters in the heap.
      */
    char *buffer;
};

#endif // LIB_RCSTRING_ACCUMULATOR_H
// vim: set ts=8 sw=4 et :

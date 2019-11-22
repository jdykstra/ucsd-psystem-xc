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

#include <lib/ac/assert.h>
#include <lib/ac/string.h>

#include <lib/input.h>


input::~input()
{
    assert(buffer);
    delete [] buffer;
    buffer = 0;
    buffer_position = 0;
    buffer_end = 0;
    buffer_size = 0;
}


input::input() :
    buffer(0),
    buffer_size(0),
    buffer_position(0),
    buffer_end(0)
{
    buffer_size = (size_t)1 << 14;
    buffer = new unsigned char [buffer_size];
    buffer_position = buffer;
    buffer_end = buffer;
}


long
input::read(void *data, size_t len)
{
    //
    // If they asked for nothing, give them nothing.
    //
    if (len <= 0)
        return 0;

    //
    // If there is anything in the buffer, return the contents of
    // the buffer.
    //
    if (buffer_position < buffer_end)
    {
        size_t nbytes = buffer_end - buffer_position;
        if (nbytes > len)
            nbytes = len;
        memcpy(data, buffer_position, nbytes);
        buffer_position += nbytes;
        return nbytes;
    }

    //
    // The buffer is empty.  Read the data directly into the
    // destination.  There is no profit in double handling.
    //
    return read_inner(data, len);
}


int
input::getc_complicated()
{
    //
    // If there is anything in the buffer, return the first byte of the
    // buffer.  This should never happen, because the inline getch method
    // is supposed to make it go away.
    //
    if (buffer_position < buffer_end)
    {
        assert(0);
        return *buffer_position++;
    }

    //
    // Fill the buffer with data, and then return the first byte of
    // the new buffer.
    //
    long nbytes = read_inner(buffer, buffer_size);
    if (nbytes <= 0)
        return (-1);
    buffer_position = buffer;
    buffer_end = buffer + nbytes;
    return *buffer_position++;
}


void
input::ungetc_complicated(int c)
{
    if (c < 0)
    {
        //
        // Toss the end-of-file indicator.
        //
        return;
    }
    if (buffer_position > buffer)
    {
        //
        // If there is room in the buffer, back up and put the character
        // in the buffer.  The inline ungetc method is supposed to have
        // taken care of this already.
        //
        assert(0);
    }
    else if (buffer_position >= buffer_end)
    {
        //
        // If the buffer is empty, just mangle the pointers to make it
        // possible to push the character back.
        //
        buffer_end = buffer + buffer_size;
        buffer_position = buffer_end;
    }
    else
    {
        //
        // Double the size of the buffer, moving the current
        // data into the second half.  That way, there will
        // always be enough room for the old data and always
        // enough room for the character to be pushed back.
        //
        // By doubling the buffer size every time (and halving the
        // probability we will need to grow again) it is still O(1)
        // overall.
        //
        buffer_size *= 2;
        unsigned char *tmp = new unsigned char [buffer_size];
        size_t nbytes = buffer_end - buffer_position;
        memcpy(tmp + buffer_size - nbytes, buffer_position, nbytes);
        delete [] buffer;
        buffer = tmp;
        buffer_end = buffer + buffer_size;
        buffer_position = buffer_end - nbytes;
    }

    //
    // The character goes before the current pointer,
    // and the current pointer is moved back by one.
    //
    *--buffer_position = c;
}


void
input::unread(const void *data, size_t len)
{
    if (buffer_position >= buffer_end)
    {
        //
        // If the buffer is empty, just mangle the pointers to
        // make it possible to push the characters back.
        //
        buffer_end = buffer + buffer_size;
        buffer_position = buffer_end - len;
        memcpy(buffer_position, data, len);
        return;
    }

    //
    // Make the buffer large enough to hold the pushback.
    //
    while (buffer_position - len < buffer)
    {
        //
        // Double the size of the buffer, moving the current
        // data into the second half.  That way, there will
        // always be enough room for the old data and always
        // enough room for the character to be pushed back.
        //
        // By doubling the buffer size every time (and halving the
        // probability we will need to grow again) it is still O(1)
        // overall.
        //
        buffer_size *= 2;
        unsigned char *tmp = new unsigned char [buffer_size];
        size_t nbytes = buffer_end - buffer_position;
        memcpy(tmp + buffer_size - nbytes, buffer_position, nbytes);
        delete [] buffer;
        buffer = tmp;
        buffer_end = buffer + buffer_size;
        buffer_position = buffer_end - nbytes;
    }

    //
    // Copy the data into the buffer, before the current position.
    //
    buffer_position -= len;
    memcpy(buffer_position, data, len);
}


bool
input::at_end()
{
    return (peek() < 0);
}


bool
input::is_remote()
    const
{
    return false;
}


// vim: set ts=8 sw=4 et :

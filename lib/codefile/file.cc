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

#include <lib/ac/errno.h>
#include <lib/ac/string.h>
#include <fcntl.h>
#include <libexplain/close.h>
#include <libexplain/fstat.h>
#include <libexplain/lseek.h>
#include <libexplain/open.h>
#include <libexplain/output.h>
#include <libexplain/read.h>
#include <libexplain/write.h>

#include <lib/codefile/file.h>
#include <lib/codefile/file/i_3.h>
#include <lib/codefile/file/i_5.h>
#include <lib/codefile/file/ii_0.h>
#include <lib/codefile/file/ii_1.h>
#include <lib/debug.h>
#include <lib/interval.h>
#include <lib/pascal_id.h>
#include <lib/p_machine.h>
#include <lib/seginfo.h>
#include <lib/segment/concrete.h>


codefile_file::~codefile_file()
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    if (fd >= 0)
        close(fd);
    fd = -1;
}


bool
codefile_file::is_valid_segment_name(const char *data)
{
    DEBUG(1, "checking %s", rcstring(data, 8).quote_c().c_str());
    // Spaces are only valid on the right (as padding).
    int len = 8;
    for (; len > 0; --len)
        if (data[len - 1] != ' ')
            break;
    for (int j = 0; j < len; ++j)
    {
        unsigned char c = *data++;
        switch (c)
        {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            break;

        default:
            DEBUG(2, "not valid segment name");
            return false;
        }
    }
    return true;
}


codefile_file::codefile_file(
    const rcstring &a_filename
) :
    filename(a_filename),
    fd(-1),
    byte_sex(little_endian),
    may_set_byte_sex(true),
    size_in_blocks(0)
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    fd = ::open(filename.c_str(), O_RDONLY);
    if (fd < 0 && errno == ENOENT && !filename.ends_with(".code"))
    {
        filename += ".code";
        fd = ::open(filename.c_str(), O_RDONLY);
    }
    if (fd < 0)
    {
        explain_output_error_and_die
        (
            "%s",
            explain_open(filename.c_str(), O_RDONLY, 0)
        );
    }

    //
    // Work out the size of the file.
    //
    struct stat st;
    explain_fstat_or_die(fd, &st);
    if (st.st_size == 0 || (st.st_size % sizeof_block) != 0)
    {
        yuck:
        explain_output_error_and_die
        (
            "the %s file does not look like a UCSD p-System codefile, "
                "it is an inappropriate size",
            get_filename().quote_c().c_str()
        );
    }
    size_in_blocks = st.st_size / sizeof_block;
    DEBUG(1, "size_in_blocks = %d", (int)size_in_blocks);

    //
    // The largest possible codefile_file has a 1 block segment dictionary,
    // plus 64KiB for each slot in the segment dictionary.  In reality, they
    // are ALWAYS smaller than this, because UCSD p-System was aimed at early
    // 8-bit microprocessors with very little RAM.
    //
    // The largest possible codefile_file size works out to be just over 1MiB,
    // and since the disks back then were almost always less than this in size
    // (e.g. 140KiB), this is hugely optimistic.
    //
    if (size_in_blocks * sizeof_block > 512 + 16 * (1L << 16))
        goto yuck;
    DEBUG(1, "}");
}


static bool
all_bytes_zero(const char *data, size_t data_size)
{
    while (data_size > 0)
    {
        if (*data != 0)
            return false;
        ++data;
        --data_size;
    }
    return true;
}


bool
codefile_file::kinda_close(const char *buffer)
{
    for (int j = 0; j < 16; ++j)
    {
        if (buffer[32 + 8 * j] == ' ')
        {
            if (!all_bytes_zero(buffer + j * 4, 4))
                return false;
        }
        if (!codefile_file::is_valid_segment_name(buffer + 32 + 8 * j))
            return false;
    }
    return true;
}


p_machine_t
codefile_file::guess_the_release(const rcstring &fn)
{
    int fildes = explain_open_or_die(fn.c_str(), O_RDONLY, 0);
    char buffer[512];
    ssize_t n = explain_read_or_die(fildes, buffer, sizeof(buffer));
    if (n != sizeof(buffer))
        goto oops;
    explain_close_or_die(fildes);

    if (codefile_file_ii_1::candidate(buffer))
        return p_machine_ii_1;
    if (codefile_file_ii_0::candidate(buffer))
        return p_machine_ii_0;
    if (codefile_file_i_5::candidate(buffer))
        return p_machine_i_5;
    if (codefile_file_i_3::candidate(buffer))
        return p_machine_i_3;

    oops:
    if (kinda_close(buffer))
    {
        explain_output_error_and_die
        (
            "the %s file looks similar to a valid UCSD Pascal codefile, "
                "but the exact p-machine release could not be determined",
            fn.quote_c().c_str()
        );
    }
    else
    {
        explain_output_error_and_die
        (
            "the %s file does not look like a valid UCSD Pascal codefile",
            fn.quote_c().c_str()
        );
    }
    return p_machine_ii_1;
}


codefile_file::pointer
codefile_file::create(const rcstring &a_filename)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);

    //
    // We are reading an existing file, not creating a new one, so we
    // ignore the system p-machine setting for the moment.
    //
    // We have to grope the file's segment dictionary, to see if it has
    // enough information that we can guess which p-machine release it
    // corresponds to.
    //
    p_machine_t guess = guess_the_release(a_filename);

    switch (guess)
    {
    case p_machine_i_3:
        DEBUG(2, "looks like I.3 codefile");
        return codefile_file_i_3::create(a_filename);

    case p_machine_i_5:
        DEBUG(2, "looks like I.5 codefile");
        return codefile_file_i_5::create(a_filename);

    case p_machine_ii_0:
        DEBUG(2, "looks like II.0 codefile");
        return codefile_file_ii_0::create(a_filename);

    case p_machine_ii_1:
        DEBUG(2, "looks like II.1 codefile");
        return codefile_file_ii_1::create(a_filename);

    default:
        assert(!"you need to add support for another p-machine release");
    }
}


codefile_file::codefile_file(
    const rcstring &a_filename,
    byte_sex_t a_byte_sex
) :
    filename(a_filename),
    fd(-1),
    byte_sex(a_byte_sex),
    may_set_byte_sex(false),
    size_in_blocks(1)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    int mode = O_RDWR | O_CREAT | O_TRUNC;
#ifdef O_BINARY
    mode |= O_BINARY;
#endif
    fd = explain_open_or_die(filename.c_str(), mode, 0666);
}


codefile_file::pointer
codefile_file::create(const rcstring &a_filename, byte_sex_t a_byte_sex)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    switch (p_machine_get())
    {
    case p_machine_i_3:
        return codefile_file_i_3::create(a_filename, a_byte_sex);

    case p_machine_i_5:
        return codefile_file_i_5::create(a_filename, a_byte_sex);

    case p_machine_ii_0:
        return codefile_file_ii_0::create(a_filename, a_byte_sex);

    case p_machine_ii_1:
        return codefile_file_ii_1::create(a_filename, a_byte_sex);

    default:
        assert(!"you need to add support for another p-machine release");
        return codefile_file_ii_1::create(a_filename, a_byte_sex);
    }
}


unsigned
codefile_file::allocate(unsigned nblocks)
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    DEBUG(2, "nblocks = %u", nblocks);
    unsigned addr = size_in_blocks;
    DEBUG(2, "return %u", addr);
    size_in_blocks += nblocks;
    return addr;
}


void
codefile_file::set_copyright_notice(const rcstring &text)
{
    notice = text;
}


rcstring
codefile_file::get_copyright_notice(void)
    const
{
    return notice;
}


void
codefile_file::read_block(unsigned block_number, void *data, unsigned nblocks)
    const
{
    DEBUG(1, "%s {", __PRETTY_FUNCTION__);
    DEBUG(2, "block_number = %u", block_number);
    DEBUG(2, "nblocks = %u", nblocks);
    assert(fd >= 0);
    off_t offset = off_t(block_number) * sizeof_block;
    explain_lseek_or_die(fd, offset, SEEK_SET);

    size_t nbytes = size_t(nblocks) * sizeof_block;
    ssize_t nr = explain_read_or_die(fd, data, nbytes);
    if ((size_t)nr != nbytes)
    {
        explain_output_error_and_die
        (
            "read %s: return the wrong size (expected %ld, got %ld)",
            filename.quote_c().c_str(),
            long(nbytes),
            long(nr)
        );
    }
    DEBUG(1, "}");
}

void
codefile_file::write_block(unsigned block_number, const void *data,
    unsigned nblocks) const
{
    DEBUG(1, "%s", __PRETTY_FUNCTION__);
    assert(fd >= 0);
    off_t offset = block_number * sizeof_block;
    explain_lseek_or_die(fd, offset, SEEK_SET);

    size_t nbytes = nblocks * sizeof_block;
    ssize_t nr = explain_write_or_die(fd, data, nbytes);
    if ((size_t)nr != nbytes)
    {
        explain_output_error_and_die
        (
            "write %s: return the wrong size (expected %ld, got %ld)",
            filename.quote_c().c_str(),
            long(nbytes),
            long(nr)
        );
    }
}


byte_sex_t
codefile_file::get_byte_sex(void)
    const
{
    may_set_byte_sex = false;
    return byte_sex;
}


void
codefile_file::set_byte_sex(byte_sex_t value)
{
    assert(may_set_byte_sex);
    byte_sex = value;
    may_set_byte_sex = false;
}


rcstring
codefile_file::get_filename(void)
    const
{
    return filename;
}


bool
codefile_file::all_spaces(const char *data, size_t data_size)
{
    while (data_size > 0)
    {
        if (*data != ' ')
            return false;
        ++data;
        --data_size;
    }
    return true;
}


// vim: set ts=8 sw=4 et :

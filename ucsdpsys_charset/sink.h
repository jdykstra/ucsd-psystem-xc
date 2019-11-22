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

#ifndef UCSDPSYS_CHARSET_SINK_H
#define UCSDPSYS_CHARSET_SINK_H

#include <lib/ac/stdio.h>
#include <lib/rcstring.h>

#include <ucsdpsys_charset/font.h>

/**
  * The sink class is used to represent places that a font file can be
  * written to.  This will usually be a file format, but conceivably
  * could be more exotic.
  */
class sink
{
public:
    typedef boost::shared_ptr<sink> pointer;

    /**
      * The destructor.
      */
    virtual ~sink();

    /**
      * The factory class method is used to manufacture new instances
      * of derived sink class.  The format name is guessed from the
      * filename, and can often be innaccurate.
      *
      * @param filename
      *     The name of the file to be written.
      * @returns
      *     managed pointer to new sink instance
      */
    static pointer factory(const rcstring &filename);

    /**
      * The factory_by_name class method is used to manufacture new instances
      * of derived sink class.
      *
      * @param format_name
      *     the name of the desired output format.
      * @param filename
      *     The name of the file to be written.
      * @returns
      *     managed pointer to new sink instance
      */
    static pointer factory_by_name(const rcstring &format_name,
        const rcstring &filename);

    /**
      * The write_whole_font method is used to write font data to the
      * file that was named in the constructor.
      *
      * @param fp
      *     the font to be written to the file
      */
    void write_whole_font(const font::pointer &fp);

    /**
      * The put_boot_logo method is used to write the boot icon into the
      * charset file.
      *
      * Whoever thought of this crap needs a slap.  The logo should have been
      * given it's own filename; it would have consumed an identical amount of
      * disk space, and not exhaust the file count, either.
      *
      * @param boot_logo
      *     the 512-byte image data (64x64x1)
      * @returns
      *     true if format supports boot logo and the logo has been written,
      *     false if the format does <b>not</b> support a boot logo.
      */
    virtual bool put_boot_logo(const unsigned char *boot_logo);

    /**
      * The is_binary method is used to determine whether or not this
      * object is a binary (non-text) file format.
      */
    virtual bool is_binary(void) const;

protected:
    /**
      * The default constructor.
      * For use by derived classes only.
      */
    sink();

    /**
      * The write_whole_font_begin method is used to notify the derived class
      * that #write_whole_font is about to start calling #write_one_glyph
      * repeatedly.  The default implementation does nothing.
      */
    virtual void write_whole_font_begin(void);

    /**
      * The write_one_glyph method is used to encode a single glyph to a
      * font definition file; it could be text, and it could be binary.
      *
      * @param gp
      *     pointer to the glyph to be written.
      */
    virtual void write_one_glyph(const glyph::pointer &gp) = 0;

    /**
      * The write_whole_font_end method is used to notify the
      * derived class that #write_whole_font is has finished calling
      * #write_one_glyph.  The default implementation does nothing.
      */
    virtual void write_whole_font_end(void);

    /**
      * The print_decode_header is used to print a textual header into a
      * non-binary file,
      */
    static void print_decode_header(FILE *fp);

    static bool looks_like_a_stdout_synonym(const rcstring &filename);

private:
    /**
      * The copy constructor.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the initialisation.
      */
    sink(const sink &rhs);

    /**
      * The assignment operator.
      * Do not use.
      *
      * @param rhs
      *     The right hand side of the assignment.
      */
    sink &operator=(const sink &rhs);
};

#endif // UCSDPSYS_CHARSET_SINK_H
// vim: set ts=8 sw=4 et :

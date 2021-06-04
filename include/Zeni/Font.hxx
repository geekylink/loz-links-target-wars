/* This file is part of the Zenipex Library.
* Copyleft (C) 2008 Mitchell Keith Bloch a.k.a. bazald
*
* The Zenipex Library is free software; you can redistribute it and/or 
* modify it under the terms of the GNU General Public License as 
* published by the Free Software Foundation; either version 2 of the 
* License, or (at your option) any later version.
*
* The Zenipex Library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License 
* along with the Zenipex Library; if not, write to the Free Software 
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 
* 02110-1301 USA.
*
* As a special exception, you may use this file as part of a free software
* library without restriction.  Specifically, if other files instantiate
* templates or use macros or inline functions from this file, or you compile
* this file and link it with other files to produce an executable, this
* file does not by itself cause the resulting executable to be covered by
* the GNU General Public License.  This exception does not however
* invalidate any other reasons why the executable file might be covered by
* the GNU General Public License.
*/

#ifndef ZENI_FONT_HXX
#define ZENI_FONT_HXX

#include <Zeni/Font.h>

namespace Zeni {

  bool Font::is_bold() const {
    return m_bold;
  }
  
  const std::string & Font::get_font_name() const {
    return m_font_name;
  }

  bool Font::is_italic() const {
    return m_italic;
  }

  int Font::get_text_height() const {
    return m_glyph_height;
  }

#ifndef DISABLE_GL

  int Font_GL::Glyph::get_glyph_width() const {
    return m_glyph_width;
  }

#endif

}

#endif

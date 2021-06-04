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

#ifndef ZENI_COLOR_HXX
#define ZENI_COLOR_HXX

#include <Zeni/Color.h>

namespace Zeni {

  const float & Color::r() const {
    return rgba[0];
  }

  const float & Color::g() const {
    return rgba[1];
  }

  const float & Color::b() const {
    return rgba[2];
  }

  const float & Color::a() const {
    return rgba[3];
  }

  unsigned char Color::r_ub() const {
    return static_cast<unsigned char>(rgba[0]*0xFF);
  }

  unsigned char Color::g_ub() const {
    return static_cast<unsigned char>(rgba[1]*0xFF);
  }

  unsigned char Color::b_ub() const {
    return static_cast<unsigned char>(rgba[2]*0xFF);
  }

  unsigned char Color::a_ub() const {
    return static_cast<unsigned char>(rgba[3]*0xFF);
  }

  Uint32 Color::get_rgba() const {
    return (r_ub() << 24) | (g_ub() << 16) | (b_ub() << 8) | a_ub();
  }

  Uint32 Color::get_argb() const {
    return (a_ub() << 24) | (r_ub() << 16) | (g_ub() << 8) | b_ub();
  }

}

#endif

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

#ifndef ZENI_VERTEX3F_HXX
#define ZENI_VERTEX3F_HXX

#include <Zeni/Vertex3f.h>

namespace Zeni {

  void Vertex3f::set_position(const Point3f &position) {
    m_position = position;
  }

  void Vertex3f::set_normal(const Vector3f &normal) {
    m_normal = normal;
  }

  void * Vertex3f::get_address() const {
    return const_cast<Point3f *>(&m_position);
  }

#ifdef _WINDOWS
  unsigned long long Vertex3f::get_offset() const {
    return reinterpret_cast<unsigned long long>(&m_position) - reinterpret_cast<unsigned long long>(this);
  }
#else
  unsigned long Vertex3f::get_offset() const {
    return reinterpret_cast<unsigned long>(&m_position) - reinterpret_cast<unsigned long>(this);
  }
#endif

  const Uint32 & Vertex3f_Color::get_color() const {
    return m_argb;
  }

  void Vertex3f_Color::set_color(const Color &color) {
    m_argb = color.get_argb();
  }

  void Vertex3f_Color::set_color(const Uint32 &argb) {
    m_argb = argb;
  }

  Point2f Vertex3f_Texture::get_texture_coordinate() const {
    return m_texture_coordinate;
  }

  void Vertex3f_Texture::set_texture_coordinate(const Point2f &texture_coordinate) {
    m_texture_coordinate = texture_coordinate;
  }

}

#endif

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

#include <Zeni/Vertex2f.h>

#include <Zeni/Coordinate.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Video_DX9.hxx>

#include <cassert>

#ifndef DISABLE_GL
#include <GL/gl.h>
#endif

namespace Zeni {

  Vertex2f::Vertex2f(const Point2f &position)
    : m_position(position.x, position.y, position.z)
  {
  }

  Vertex2f::~Vertex2f() {
  }

  Point3f Vertex2f::get_position() const {
    return m_position;
  }

  Vertex2f * Vertex2f_Color::interpolate_to(const float &rhs_part, const Vertex2f_Color &rhs) const {
    const Point3f position = get_position().interpolate_to(rhs_part, rhs.get_position());
    return new Vertex2f_Color(Point2f(position.x, position.y), 
      Color(m_argb).interpolate_to(rhs_part, rhs.m_argb).get_argb());
  }

  Vertex2f_Color::Vertex2f_Color(const Point2f &position, const Color &color)
    : Vertex2f(position),
    m_argb(color.get_argb())
  {
  }

  Vertex2f_Color::Vertex2f_Color(const Point2f &position, const Uint32 &argb)
    : Vertex2f(position),
    m_argb(argb)
  {
  }

  Point3f Vertex2f_Color::get_position() const {
    return Vertex2f::get_position();
  }

#ifndef DISABLE_GL
  void Vertex2f_Color::render_to(Video_GL &screen) const {
    glBegin(GL_POINTS);
    subrender_to(screen);
    glEnd();
  }

  void Vertex2f_Color::subrender_to(Video_GL &) const {
    glColor4ub(GLubyte((m_argb >> 16) & 0xFF), 
      GLubyte((m_argb >> 8) & 0xFF), 
      GLubyte(m_argb & 0xFF), 
      GLubyte((m_argb >> 24) & 0xFF));
    glVertex2f(get_position().x, get_position().y);
  }
#endif

#ifndef DISABLE_DX9
  void Vertex2f_Color::render_to(Video_DX9 &screen) const {
    screen.get_d3d_device()->DrawPrimitiveUP(D3DPT_POINTLIST, 1, this, sizeof(Vertex2f_Color));
  }
#endif

  Vertex2f_Texture::Vertex2f_Texture(const Point2f &position, const Point2f &texture_coordinate)
    : Vertex2f(position),
    m_texture_coordinate(texture_coordinate)
  {
  }

  Vertex2f * Vertex2f_Texture::interpolate_to(const float &rhs_part, const Vertex2f_Texture &rhs) const {
    const Point3f position = get_position().interpolate_to(rhs_part, rhs.get_position());
    return new Vertex2f_Texture(Point2f(position.x, position.y), 
      m_texture_coordinate.interpolate_to(rhs_part, rhs.m_texture_coordinate));
  }

  Point3f Vertex2f_Texture::get_position() const {
    return Vertex2f::get_position();
  }

#ifndef DISABLE_GL
  void Vertex2f_Texture::render_to(Video_GL &) const {
    assert(false);
  }

  void Vertex2f_Texture::subrender_to(Video_GL &) const {
    glTexCoord2f(m_texture_coordinate.x, m_texture_coordinate.y);
    glVertex2f(get_position().x, get_position().y);
  }
#endif

#ifndef DISABLE_DX9
  void Vertex2f_Texture::render_to(Video_DX9 &) const {
    assert(false);
  }
#endif

}

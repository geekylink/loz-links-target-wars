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

/**
 * \class Zeni::Quadrilateral<VERTEX>
 *
 * \ingroup Zenilib
 *
 * \brief An Abstraction of a Quadrilateral
 *
 * I call it a Quadrilateral, but there is nothing forcing you to use it as such.
 * Really it is a triangle fan consisting of four vertices.
 *
 * \note Use a Texture_Render_Wrapper to avoid having to manually set and unset a texture each time the Quadrilateral is rendered.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_Quadrilateral_H
#define ZENI_Quadrilateral_H

#include <Zeni/Render_Wrapper.h>
#include <Zeni/Triangle.h>

namespace Zeni {

  template <typename VERTEX>
  class Quadrilateral : public Renderable {
  public:
    /// The best way to create a Quadrilateral
    Quadrilateral(const VERTEX &vertex0 = VERTEX(), const VERTEX &vertex1 = VERTEX(), 
      const VERTEX &vertex2 = VERTEX(), const VERTEX &vertex3 = VERTEX(), 
      Render_Wrapper *render_wrapper = new Render_Wrapper());
    ~Quadrilateral();

    Quadrilateral(const Quadrilateral<VERTEX> &rhs);
    Quadrilateral<VERTEX> & operator=(const Quadrilateral<VERTEX> &rhs);

    const VERTEX & get_vertex(const int &index) const; ///< Get a vertex
    void set_vertex(const int &index, const VERTEX &vertex); ///< Set a vertex

    // The "position" is the average of the three vertices
    virtual Point3f get_position() const; ///< Get the aveage of all vertices

#ifndef DISABLE_GL
    virtual void render_to(Video_GL &screen) const;
#endif

#ifndef DISABLE_DX9
    virtual void render_to(Video_DX9 &screen) const;
#endif

    const Render_Wrapper * get_render_wrapper() const; ///< Get the current Render_Wrapper
    void set_render_wrapper(Render_Wrapper * const render_wrapper); ///< Set the current Render_Wrapper
    Quadrilateral<VERTEX> * get_duplicate() const; ///< Get a duplicate of the Quadrilateral

    Triangle<VERTEX> * get_duplicate_t0() const; ///< Get the first half of the Quadrilateral
    Triangle<VERTEX> * get_duplicate_t1() const; ///< Get the second half of the Quadrilateral

  private:
    VERTEX m_vertex[4];
    Render_Wrapper * m_render_wrapper;
  };

}

#endif

#ifdef ZENI_INLINES
#include <Zeni/Quadrilateral.hxx>
#endif

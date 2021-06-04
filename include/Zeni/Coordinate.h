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
 * \class Zeni::Point2i
 *
 * \ingroup Zenilib
 *
 * \brief A 2D Point represented with integers
 *
 * \note Point2f is often a better choice.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Point2f
 *
 * \ingroup Zenilib
 *
 * \brief A 2D Point represented with floats
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Point3i
 *
 * \ingroup Zenilib
 *
 * \brief A 3D Point represented with integers
 *
 * \note Point3f is often a better choice.
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

/**
 * \class Zeni::Point3f
 *
 * \ingroup Zenilib
 *
 * \brief A 3D Point represented with floats
 *
 * \author bazald
 *
 * Contact: bazald@zenipex.com
 */

#ifndef ZENI_COORDINATE_H
#define ZENI_COORDINATE_H

#include <SDL/SDL_stdinc.h>

namespace Zeni {

  struct Point2i;
  struct Point2f;
  struct Point3i;
  struct Point3f;

  struct Point2i {
    inline Point2i();
    inline Point2i(const int &x_, const int &y_);
    inline explicit Point2i(const Point3i &rhs); ///< z in rhs is ignored.

    Point2i interpolate_to(const float &rhs_part, const Point2i &rhs) const; ///< Get a point inbetween this point and another point of the same type.

    Sint32 x, y;
    static const Sint32 z;
  };

  struct Point2f {
    inline Point2f();
    inline Point2f(const float &x_, const float &y_);
    inline explicit Point2f(const Point3f &rhs); ///< z in rhs is ignored.

    Point2f interpolate_to(const float &rhs_part, const Point2f &rhs) const; ///< Get a point inbetween this point and another point of the same type.

    float x, y;
    static const float z;
  };

  struct Point3i {
    inline Point3i();
    inline Point3i(const int &x_, const int &y_, const int &z_);
    inline explicit Point3i(const Point2i &rhs); ///< z is set to 0

    Point3i interpolate_to(const float &rhs_part, const Point3i &rhs) const; ///< Get a point inbetween this point and another point of the same type.

    Sint32 x, y, z;
  };

  struct Point3f {
    inline Point3f();
    inline Point3f(const float &x_, const float &y_, const float &z_);
    inline explicit Point3f(const Point2f &rhs); ///< z is set to 0.0f

    Point3f interpolate_to(const float &rhs_part, const Point3f &rhs) const; ///< Get a point inbetween this point and another point of the same type.

    float x, y, z;
  };

}

#endif

#ifdef ZENI_INLINES
#include <Zeni/Coordinate.hxx>
#endif

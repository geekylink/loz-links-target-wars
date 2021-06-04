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

#include <Zeni/Coordinate.hxx>

namespace Zeni {

  Point2i Point2i::interpolate_to(const float &rhs_part, const Point2i &rhs) const {
    float lhs_part = 1.0f - rhs_part;
    return Point2i(int(lhs_part * x + rhs_part * rhs.x), 
      int(lhs_part * y + rhs_part * rhs.y));
  }

  Point2f Point2f::interpolate_to(const float &rhs_part, const Point2f &rhs) const {
    float lhs_part = 1.0f - rhs_part;
    return Point2f(lhs_part * x + rhs_part * rhs.x, 
      lhs_part * y + rhs_part * rhs.y);
  }

  Point3i Point3i::interpolate_to(const float &rhs_part, const Point3i &rhs) const {
    float lhs_part = 1.0f - rhs_part;
    return Point3i(int(lhs_part * x + rhs_part * rhs.x), 
      int(lhs_part * y + rhs_part * rhs.y), 
      int(lhs_part * z + rhs_part * rhs.z));
  }

  Point3f Point3f::interpolate_to(const float &rhs_part, const Point3f &rhs) const {
    float lhs_part = 1.0f - rhs_part;
    return Point3f(lhs_part * x + rhs_part * rhs.x, 
      lhs_part * y + rhs_part * rhs.y, 
      lhs_part * z + rhs_part * rhs.z);
  }

  const int Point2i::z = 0;
  const float Point2f::z = 0;

}

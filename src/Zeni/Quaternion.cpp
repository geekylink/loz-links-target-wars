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

#include <Zeni/Quaternion.hxx>

#include <cmath>

namespace Zeni {

  Quaternion::Quaternion(const float &t, const Vector3f &s)
    : time(t),
    space(s)
  {
  }

  Quaternion::Quaternion(const Vector3f &v, const float &theta) {
    const float half_theta = 0.5f * theta;

    time = cos(half_theta);
    space = sin(half_theta) * v;
  }

  Quaternion::Quaternion(const float &yaw, const float &pitch, const float &roll) {
    const float half_yaw = 0.5f * yaw;
    const float half_pitch = 0.5f * pitch;
    const float half_roll = 0.5f * roll;
   
	  const float shy = sin(half_yaw);
	  const float shp = sin(half_pitch);
	  const float shr = sin(half_roll);
	  const float chy = cos(half_yaw);
	  const float chp = cos(half_pitch);
	  const float chr = cos(half_roll);
   
	  time = chr * chp * chy + shr * shp * shy;
	  space.i = shr * chp * chy - chr * shp * shy;
	  space.j = chr * shp * chy + shr * chp * shy;
	  space.k = chr * chp * shy - shr * shp * chy;
   
	  *this = norm();
  }

  Quaternion::Quaternion(const Quaternion &rhs)
  {
    *this = rhs;
  }

}

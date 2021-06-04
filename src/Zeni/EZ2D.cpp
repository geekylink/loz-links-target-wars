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

#include <Zeni/EZ2D.h>

#include <Zeni/Color.hxx>
#include <Zeni/Coordinate.hxx>
#include <Zeni/Material.hxx>
#include <Zeni/Textures.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Vertex2f.hxx>
#include <Zeni/Video.h>
#include <Zeni/Quadrilateral.hxx>

namespace Zeni {

  void render_image(
    const std::string &image_name,
    const Point2f &upper_left,
    const Point2f &lower_right,
    const bool &horizontally_flipped,
    const Color &color_filter) {

      float
        tx0 = horizontally_flipped ? 1.0f : 0.0f,
        tx1 = 1.0f - tx0;

      Material material(image_name);
      material.set_ambient(color_filter);
      material.set_diffuse(color_filter);

      Quadrilateral<Vertex2f_Texture> q(
        Vertex2f_Texture(Point2f(upper_left.x, upper_left.y), Point2f(tx0, 0.0f)),
        Vertex2f_Texture(Point2f(upper_left.x, lower_right.y), Point2f(tx0, 1.0f)),
        Vertex2f_Texture(Point2f(lower_right.x, lower_right.y), Point2f(tx1, 1.0f)),
        Vertex2f_Texture(Point2f(lower_right.x, upper_left.y), Point2f(tx1, 0.0f)),
        new Material_Render_Wrapper(material));

      Video::get_reference().render(q);
  }


  void render_image(
    const std::string &image_name,
    const Point2f &upper_left,
    const Point2f &lower_right,
    const float &radians_cw,
    const float &scaling_factor,
    const Point2f &about,
    const bool &horizontally_flipped,
    const Color &color_filter) {

      const Point3f about3 = Point3f(about);

      Vector3f 
        ulv = Point3f(upper_left) - about3,
        llv = Point3f(upper_left.x, lower_right.y, 0.0f) - about3,
        lrv = Point3f(lower_right) - about3,
        urv = Point3f(lower_right.x, upper_left.y, 0.0f) - about3;

      ulv.set_spherical(ulv.theta() + radians_cw, ulv.phi(), ulv.magnitude() * scaling_factor);
      llv.set_spherical(llv.theta() + radians_cw, llv.phi(), llv.magnitude() * scaling_factor);
      lrv.set_spherical(lrv.theta() + radians_cw, lrv.phi(), lrv.magnitude() * scaling_factor);
      urv.set_spherical(urv.theta() + radians_cw, urv.phi(), urv.magnitude() * scaling_factor);

      const float
        tx0 = horizontally_flipped ? 1.0f : 0.0f,
        tx1 = 1.0f - tx0;

      Material material(image_name);
      material.set_ambient(color_filter);
      material.set_diffuse(color_filter);

      Quadrilateral<Vertex2f_Texture> q(
        Vertex2f_Texture(Point2f(about3 + ulv), Point2f(tx0, 0.0f)),
        Vertex2f_Texture(Point2f(about3 + llv), Point2f(tx0, 1.0f)),
        Vertex2f_Texture(Point2f(about3 + lrv), Point2f(tx1, 1.0f)),
        Vertex2f_Texture(Point2f(about3 + urv), Point2f(tx1, 0.0f)),
        new Material_Render_Wrapper(material));

      Video::get_reference().render(q);
  }

  bool is_sprite(
    const std::string &sprite) {

      return Textures::get_reference().is_sprite(
        Textures::get_reference().get_texture_id(sprite));
  }

  int sprite_num_frames(
    const std::string &sprite) {

      return Textures::get_reference().get_num_frames(
        Textures::get_reference().get_texture_id(sprite));
  }

  void set_sprite_frame(
    const std::string &sprite,
    const int &frame_number) {

      return Textures::get_reference().set_current_frame(
        Textures::get_reference().get_texture_id(sprite),
        frame_number);
  }

  void increment_sprite_frame(
    const std::string &sprite_name) {

      Texture * const texture = Textures::get_reference().get_texture(
        Textures::get_reference().get_texture_id(sprite_name));

      Sprite * const sprite = dynamic_cast<Sprite * const>(texture);

      if(!sprite)
        throw Sprite_Function_Misapplied();

      const unsigned int
        frameno = sprite->get_current_frame(),
        framecap = sprite->get_num_frames();

      if(frameno == framecap - 1)
        sprite->set_current_frame(0);
      else
        sprite->set_current_frame(frameno + 1);
  }

  void decrement_sprite_frame(
    const std::string &sprite_name) {

      Texture * const texture = Textures::get_reference().get_texture(
        Textures::get_reference().get_texture_id(sprite_name));

      Sprite * const sprite = dynamic_cast<Sprite * const>(texture);

      if(!sprite)
        throw Sprite_Function_Misapplied();

      const unsigned int
        frameno = sprite->get_current_frame(),
        framecap = sprite->get_num_frames();

      if(frameno)
        sprite->set_current_frame(frameno - 1);
      else
        sprite->set_current_frame(framecap - 1);
  }

}

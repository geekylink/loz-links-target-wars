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

#ifndef ZENI_TEXTURE_HXX
#define ZENI_TEXTURE_HXX

#include <Zeni/Texture.h>

#include <Zeni/Textures.h>
#include <Zeni/Textures.hxx>
#include <Zeni/Video.hxx>
#include <Zeni/Video_DX9.hxx>

namespace Zeni {

#ifndef DISABLE_DX9
#ifndef DISABLE_GL

#define \
  TEXTURE_IV_FCN_CALL(member_function, ...) { \
    switch(vtype()) { \
      case Texture_Base::VTYPE_GL: return reinterpret_cast<Texture_GL *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_DX9: return reinterpret_cast<Texture_DX9 *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_SPRITE: return reinterpret_cast<Sprite *>(this)->member_function(__VA_ARGS__); \
    } \
  }

#define \
  TEXTURE_IV_FCN_CALL_CONST(member_function, ...) { \
    switch(vtype()) { \
      case Texture_Base::VTYPE_GL: return reinterpret_cast<const Texture_GL *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_DX9: return reinterpret_cast<const Texture_DX9 *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_SPRITE: return reinterpret_cast<const Sprite *>(this)->member_function(__VA_ARGS__); \
    } \
  }

#else

#define \
  TEXTURE_IV_FCN_CALL(member_function, ...) { \
    switch(vtype()) { \
      case Texture_Base::VTYPE_DX9: return reinterpret_cast<Texture_DX9 *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_SPRITE: return reinterpret_cast<Sprite *>(this)->member_function(__VA_ARGS__); \
    } \
  }

#define \
  TEXTURE_IV_FCN_CALL_CONST(member_function, ...) { \
    switch(vtype()) { \
      case Texture_Base::VTYPE_DX9: return reinterpret_cast<const Texture_DX9 *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_SPRITE: return reinterpret_cast<const Sprite *>(this)->member_function(__VA_ARGS__); \
    } \
  }

#endif
#else

#define \
  TEXTURE_IV_FCN_CALL(member_function, ...) { \
    switch(vtype()) { \
      case Texture_Base::VTYPE_GL: return reinterpret_cast<Texture_GL *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_SPRITE: return reinterpret_cast<Sprite *>(this)->member_function(__VA_ARGS__); \
    } \
  }

#define \
  TEXTURE_IV_FCN_CALL_CONST(member_function, ...) { \
    switch(vtype()) { \
      case Texture_Base::VTYPE_GL: return reinterpret_cast<const Texture_GL *>(this)->member_function(__VA_ARGS__); \
      case Texture_Base::VTYPE_SPRITE: return reinterpret_cast<const Sprite *>(this)->member_function(__VA_ARGS__); \
    } \
  }

#endif

  void Sprite::apply_texture_impl() const {
    static bool no_recurse = true;

    if(no_recurse)
      no_recurse = false;
    else
      throw Sprite_Containing_Sprite();

    if(m_frames.empty() || m_frame < 0 || m_frame >= int(m_frames.size()))
      throw Frame_Out_of_Range();

    try {
      Textures::get_reference().apply_texture(m_frames[m_frame].second);
    }
    catch(Texture_Not_Found &) {
      try {
        m_frames[m_frame].second = Textures::get_reference().get_texture_id(m_frames[m_frame].first);
        Textures::get_reference().apply_texture(m_frames[m_frame].second);
      }
      catch(...) {
        no_recurse = true;
        throw;
      }
    }
    catch(...) {
      no_recurse = true;
      throw;
    }
    no_recurse = true;
  }

#ifndef DISABLE_GL
  void Texture_GL::apply_texture_impl() const {
    if(!m_texture_id)
      load(m_filename, m_repeat);
    
    glBindTexture(GL_TEXTURE_2D, m_texture_id);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  }
#endif

#ifndef DISABLE_DX9
  void Texture_DX9::apply_texture_impl() const {
    Video_DX9 &vdx = reinterpret_cast<Video_DX9 &>(Video::get_reference());

    vdx.get_d3d_device()->SetTexture(0, m_texture);

    if(vdx.get_lighting()) {
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    }
    else {
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
      vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);
    }

    vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    vdx.get_d3d_device()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
  }
#endif

  void Texture::apply_texture() const {
    TEXTURE_IV_FCN_CALL_CONST(apply_texture_impl, );
  }

#undef TEXTURE_IV_FCN_CALL
#undef TEXTURE_IV_FCN_CALL_CONST

}

#endif

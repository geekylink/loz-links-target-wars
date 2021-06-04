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

#include <Zeni/Video_DX9.hxx>

#ifndef DISABLE_DX9

#include <Zeni/Camera.hxx>
#include <Zeni/Color.hxx>
#include <Zeni/Game.hxx>
#include <Zeni/Gamestate.hxx>
#include <Zeni/Matrix4f.hxx>
#include <Zeni/Vector3f.hxx>
#include <Zeni/Video.hxx>

#include <SDL/SDL_syswm.h>

#include <cassert>
#include <iostream>

using namespace std;

namespace Zeni {

  Video_DX9::Video_DX9()
    : Video(Video_Base::ZENI_VIDEO_DX9),
    m_d3d(0),
    m_d3d_device(0),
    m_matrix_stack(0),
    m_ambient_color(1.0f, 1.0f, 1.0f, 1.0f),
    m_textured(false),
    m_3d(false)
  {
    init();
  }

  Video_DX9::~Video_DX9() {
    uninit();
  }

  void Video_DX9::render_all() {
    static bool reset = false;

    if(reset) {
      const HRESULT result = m_d3d_device->TestCooperativeLevel();
      
      if(result == D3DERR_DEVICELOST)
        return;
      else if(result == D3DERR_DRIVERINTERNALERROR)
        throw Video_Device_Failure();
      
      if(result == D3DERR_DEVICENOTRESET) {
        if(FAILED(m_d3d_device->Reset(&m_d3d_parameters)))
          throw Video_Device_Failure();
      
        reset = false;

        init_context();
        
        Textures::get_reference().reload();
        Fonts::get_reference().reload();
      }
    }
    
    HRESULT result = m_d3d_device->Present(0, 0, 0, 0);
    
    if(result == S_OK) {
      D3DVIEWPORT9 vp = {0, 0, get_screen_width(), get_screen_height(), 0, 1};
      m_d3d_device->SetViewport(&vp);
      m_d3d_device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(get_clear_color().r_ub(), get_clear_color().g_ub(), get_clear_color().b_ub()), 1.0f, 0);
      m_d3d_device->BeginScene();

      Game::get_reference().render();
      
      m_d3d_device->EndScene();
    }
    else if(result == D3DERR_DEVICELOST) {
      reset = true;
      return;
    }
    else if(result == D3DERR_DEVICEREMOVED) {
      throw Video_Device_Failure();
    }
  }

  void Video_DX9::init() {
    set_opengl_flag(false);
    Video::init();
    
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWMInfo(&wmInfo);

    cout << "Initializing DirectX 9" << endl;

    m_d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if(!m_d3d) {
      uninit();
      throw Video_Init_Failure();
    }

    m_d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_d3d_capabilities);

    m_dpi = GetDeviceCaps(GetDC(wmInfo.window), LOGPIXELSY);

    ZeroMemory(&m_d3d_parameters, sizeof(m_d3d_parameters));

    m_d3d_parameters.hDeviceWindow = wmInfo.window;
    
    m_d3d_parameters.Windowed = !is_fullscreen();
    m_d3d_parameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

    m_d3d_parameters.BackBufferCount = 1;
    m_d3d_parameters.BackBufferWidth = get_screen_width();
    m_d3d_parameters.BackBufferHeight = get_screen_height();
    m_d3d_parameters.BackBufferFormat = m_d3d_parameters.Windowed ? D3DFMT_UNKNOWN : D3DFMT_A8R8G8B8;
    m_d3d_parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    m_d3d_parameters.PresentationInterval = get_vertical_sync() ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
    m_d3d_parameters.Flags = 0;//D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

    m_d3d_parameters.EnableAutoDepthStencil = true;
    m_d3d_parameters.AutoDepthStencilFormat = D3DFMT_D16;

    if(get_multisampling() > 1)
      switch(get_multisampling()) {
      case 2: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
      case 3: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; break;
      case 4: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
      case 5: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; break;
      case 6: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; break;
      case 7: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; break;
      case 8: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; break;
      case 9: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; break;
      case 10: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; break;
      case 11: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; break;
      case 12: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; break;
      case 13: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; break;
      case 14: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; break;
      case 15: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; break;
      case 16:
      default: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES; break;
      }
    else if(get_multisampling() == -1)
      m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_16_SAMPLES;
    else
      m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
    m_d3d_parameters.MultiSampleQuality = 0;

    // Initialize the D3D device
    if(!init_device())
      throw Video_Init_Failure();

    // Initialize the rendering context
    init_context();
  }
  
  bool Video_DX9::init_device() {
    DWORD num_quality_levels;
    while(FAILED(m_d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, D3DFMT_A8R8G8B8, m_d3d_parameters.Windowed, m_d3d_parameters.MultiSampleType, &num_quality_levels))) {
      if(m_d3d_parameters.MultiSampleType <= D3DMULTISAMPLE_2_SAMPLES) {
        m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        break;
      }

      switch(m_d3d_parameters.MultiSampleType) {
      case D3DMULTISAMPLE_3_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_2_SAMPLES; break;
      case D3DMULTISAMPLE_4_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_3_SAMPLES; break;
      case D3DMULTISAMPLE_5_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_4_SAMPLES; break;
      case D3DMULTISAMPLE_6_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_5_SAMPLES; break;
      case D3DMULTISAMPLE_7_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_6_SAMPLES; break;
      case D3DMULTISAMPLE_8_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_7_SAMPLES; break;
      case D3DMULTISAMPLE_9_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES; break;
      case D3DMULTISAMPLE_10_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_9_SAMPLES; break;
      case D3DMULTISAMPLE_11_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_10_SAMPLES; break;
      case D3DMULTISAMPLE_12_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_11_SAMPLES; break;
      case D3DMULTISAMPLE_13_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_12_SAMPLES; break;
      case D3DMULTISAMPLE_14_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_13_SAMPLES; break;
      case D3DMULTISAMPLE_15_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_14_SAMPLES; break;
      case D3DMULTISAMPLE_16_SAMPLES: m_d3d_parameters.MultiSampleType = D3DMULTISAMPLE_15_SAMPLES; break;
      default: return false;
      }
    }

    if(FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters.hDeviceWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &m_d3d_parameters, &m_d3d_device)) &&
      FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters.hDeviceWindow, D3DCREATE_MIXED_VERTEXPROCESSING, &m_d3d_parameters, &m_d3d_device)) &&
      FAILED(m_d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_d3d_parameters.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &m_d3d_parameters, &m_d3d_device))) {

        // HARDWARE, MIXED, and SOFTWARE all failed

        return false;
    }

    // Set Up Matrix Stack
    D3DXCreateMatrixStack(0, &m_matrix_stack);
    m_matrix_stack->LoadIdentity();

    return true;
  }

  void Video_DX9::init_context() {
    // Enable Alpha Blitting
    m_d3d_device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
    m_d3d_device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_d3d_device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

    // Configure Texture Stages
    m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
    m_d3d_device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);

    // Set Lighting Variables
    m_d3d_device->SetRenderState(D3DRS_NORMALIZENORMALS, true);

    // Multisampling
    m_d3d_device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, get_multisampling() > 1);

    // More basic stuff
    set_2d();
    set_color(get_color());
    set_clear_color(get_clear_color());
    set_backface_culling(get_backface_culling());
    set_lighting(get_lighting());
    set_ambient_lighting(m_ambient_color);
  }

  void Video_DX9::destroy_device() {
    if(m_matrix_stack)
      m_matrix_stack->Release();
    m_matrix_stack = 0;

    if(m_d3d_device)
      m_d3d_device->Release();
    m_d3d_device = 0;
  }

  void Video_DX9::set_fvf() {
    DWORD fvf = D3DFVF_XYZ;

    if(m_3d)
      fvf |= D3DFVF_NORMAL;

    if(m_textured)
      fvf |= D3DFVF_TEX1;
    else if(!get_lighting())
      fvf |= D3DFVF_DIFFUSE;

    m_d3d_device->SetFVF(fvf);
  }
  
  bool Video_DX9::get_3d() const {
    return m_3d;
  }

  void Video_DX9::set_3d(const bool &on) {
    m_3d = on;

    set_fvf();
  }

}

#else

namespace Zeni {
  void * this_pointer_is_dead_beef = (void *)0xDEADBEEF;
}

#endif

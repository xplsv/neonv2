//---------------------------------------------------------------------------//
// Neon v2.5
// Copyright (C) 2006,2008 Jordi Ros <shine.3p@gmail.com>
// www.neonv2.com / www.xplsv.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program called license.txt
// If not, see <http://www.gnu.org/licenses/>
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include <d3d9.h>
#include <dxerr9.h>
#include "GEDisplayDevice.h"
#include "GECustomVertex.h"
#include "GEGestorMateriales.h"
#include "GETextura.h"
#include "GEMotor3D.h"
#include "GEEffects.h"

//---------------------------------------------------------------------------//

#define INVALID_BST       ((unsigned)-1)
#ifndef offsetof
  #define offsetof(s,m)   (size_t)&(((s *)0)->m)
#endif

//---------------------------------------------------------------------------//
// Static Vertex Buffers
//---------------------------------------------------------------------------//
struct TVertexBufferInfo
{
  LPDIRECT3DVERTEXBUFFER9      pVertexBuffer;
  LPDIRECT3DVERTEXDECLARATION9 pDecl;
  LPD3DVERTEXELEMENT9          pElements;
  unsigned                     uSize;
  unsigned                     uNumVertex;
  unsigned                     uLockIndex;
  unsigned                     uDrawIndex;
};

static D3DVERTEXELEMENT9 s_HCV_XYZ_F_Elements[] =
{
  {0, offsetof(TVertex_HCV_XYZ_F, Pos),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, offsetof(TVertex_HCV_XYZ_F, Normal),   D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, offsetof(TVertex_HCV_XYZ_F, Diffuse),  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
  {0, offsetof(TVertex_HCV_XYZ_F, UV0),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  {0, offsetof(TVertex_HCV_XYZ_F, UV1),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
  D3DDECL_END(),
};
static D3DVERTEXELEMENT9 s_HCV_XYZ_P_Elements[] =
{
  {0, offsetof(TVertex_HCV_XYZ_P, Pos),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
  {0, offsetof(TVertex_HCV_XYZ_P, Normal),   D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
  {0, offsetof(TVertex_HCV_XYZ_P, Tangent),  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
  {0, offsetof(TVertex_HCV_XYZ_P, Binormal), D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
  {0, offsetof(TVertex_HCV_XYZ_P, Diffuse),  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,    0},
  {0, offsetof(TVertex_HCV_XYZ_P, UV0),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
  {0, offsetof(TVertex_HCV_XYZ_P, UV1),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
  {0, offsetof(TVertex_HCV_XYZ_P, UV2),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
  {0, offsetof(TVertex_HCV_XYZ_P, UV3),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
  D3DDECL_END(),
};
static D3DVERTEXELEMENT9 s_HCV_XYZ_S_Elements[] =
{
  {0, offsetof(TVertex_HCV_XYZ_S, Pos),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,  0},
  {0, offsetof(TVertex_HCV_XYZ_S, PSize),    D3DDECLTYPE_FLOAT1,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE,     0},
  {0, offsetof(TVertex_HCV_XYZ_S, Diffuse),  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0},
  {0, offsetof(TVertex_HCV_XYZ_S, UV0),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0},
  {0, offsetof(TVertex_HCV_XYZ_S, UV1),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1},
  {0, offsetof(TVertex_HCV_XYZ_S, UV2),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  2},
  {0, offsetof(TVertex_HCV_XYZ_S, UV3),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  3},
  D3DDECL_END(),
};
static D3DVERTEXELEMENT9 s_HCV_XYZ_T_Elements[] =
{
  {0, offsetof(TVertex_HCV_XYZ_T, Pos),      D3DDECLTYPE_FLOAT4,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
  {0, offsetof(TVertex_HCV_XYZ_T, PSize),    D3DDECLTYPE_FLOAT1,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_PSIZE,     0},
  {0, offsetof(TVertex_HCV_XYZ_T, Diffuse),  D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0},
  {0, offsetof(TVertex_HCV_XYZ_T, UV0),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0},
  {0, offsetof(TVertex_HCV_XYZ_T, UV1),      D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1},
  D3DDECL_END(),
};

// La cantidad de vertices es algo estadistico. Cuando se necesita mas se incrementa el buffer automaticamente
static TVertexBufferInfo s_VBInfo[HCV_MAX_CUSTOM_VERTEX] =
{
  {NULL, NULL, s_HCV_XYZ_F_Elements, sizeof(TVertex_HCV_XYZ_F), 50000, 0,0},
  {NULL, NULL, s_HCV_XYZ_P_Elements, sizeof(TVertex_HCV_XYZ_P), 50000, 0,0},
  {NULL, NULL, s_HCV_XYZ_S_Elements, sizeof(TVertex_HCV_XYZ_S),  1000, 0,0},
  {NULL, NULL, s_HCV_XYZ_T_Elements, sizeof(TVertex_HCV_XYZ_T),  5000, 0,0},
};

//---------------------------------------------------------------------------//
// Static Index Buffers
//---------------------------------------------------------------------------//
struct TIndexBufferInfo
{
  IDirect3DIndexBuffer9 *pIndexBuffer;
  unsigned               uFormat;
  unsigned               uSize;
  unsigned               uNumIndex;
  unsigned               uLockIndex;
  unsigned               uDrawIndex;
  unsigned               uMaxLocked;
};

static TIndexBufferInfo  s_IBInfo = {NULL, D3DFMT_INDEX16, sizeof(ushort), 5000, 0,0,0};

//---------------------------------------------------------------------------//
// Static Vars
//---------------------------------------------------------------------------//
static unsigned          s_aTextureFormats[HARD_MAX_TEXTURE_FORMATS] =
{
  HARD_TEX_FORMAT_P8,
  HARD_TEX_FORMAT_A8,
  HARD_TEX_FORMAT_A8L8,
  HARD_TEX_FORMAT_A4R4G4B4,
  HARD_TEX_FORMAT_R5G6B5,
  HARD_TEX_FORMAT_A1R5G5B5,
  HARD_TEX_FORMAT_A8R8G8B8,
};

#define D3D_ERROR(String,hRet)                              \
        {                                                   \
          char pBuffer[2048];                               \
          sprintf_s(pBuffer, 2048, String                   \
                            "\nDXErrorStr  = %s",           \
                            DXGetErrorString9(hRet));       \
          ASSERTM (false, pBuffer);                         \
          GLOG    ((pBuffer));                              \
        }

/*
#define D3D_ERROR(String,hRet)                              \
        {                                                   \
          char pBuffer[2048];                               \
          sprintf (pBuffer, String                          \
                            "\nDXErrorStr  = %s"            \
                            "\nDXErrorDesc = %s",           \
                            DXGetErrorString8(hRet),        \
                            DXGetErrorDescription8(hRet));  \
          ASSERTM (false, pBuffer);                         \
        }

*/

CGestorMateriales *g_pGestorMateriales;

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CDisplayDevice::Init(TModoGrafico const &Modo, int hWnd)
{
  End();
  BorraVars();

  // Crear el objeto D3D9
  m_hWnd = hWnd;
  m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
  if (!m_pD3D)
  {
    GLOG(("ERR: Direct3D9 not found\n"));
    return false;
  }

  m_bOk = true;

  // Inicializar el dispositivo
  Reset  (Modo);
  Restore();

  m_pGestorMateriales = NEW CGestorMateriales;
  m_pGestorMateriales->Init();
  g_pGestorMateriales = m_pGestorMateriales;
  CMotor3D::Init();
  CEffects::Init();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CDisplayDevice::End()
{
  if (IsOk())
  {
    // Liberar D3D
    Release();
    CMotor3D::End();
    CEffects::End();
    DISPOSE(m_pGestorMateriales);
    HARD_RELEASE(m_pD3DDevice);
    HARD_RELEASE(m_pD3D);
    // Destruir la ventana
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// BorraVars
//
//---------------------------------------------------------------------------//
void CDisplayDevice::BorraVars()
{
  m_pD3D              = NULL;
  m_pD3DDevice        = NULL;
  m_pGestorMateriales = NULL;
  m_bAvailable        = false;
  m_bDrawingScene     = false;
  m_bVertexLock       = false;
  m_Viewport          = TViewport(0,0,0,0);
  D3DXMatrixIdentity(&m_MatIdentity);
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CDisplayDevice::LiberaVars()
{
  if (IsOk())
  {
    m_pD3D       = NULL;
    m_pD3DDevice = NULL;
  }
}


//---------------------------------------------------------------------------//
// PageFlip
//
//---------------------------------------------------------------------------//
void CDisplayDevice::PageFlip()
{
  m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//---------------------------------------------------------------------------//
// Release
// Funcion: Se llama cuando se ha perdido el contexto del dispositivo
// (minimizar, alt+tab)
//---------------------------------------------------------------------------//
void CDisplayDevice::Release()
{
  m_pGestorMateriales->Release();
  HARD_RELEASE(m_pBasicBST);
  HARD_RELEASE(m_pColorBuffer);
  HARD_RELEASE(m_pDepthBuffer);
  HARD_RELEASE(m_pDepthSurface);
  LiberaVertexBuffers();
  LiberaIndexBuffers ();
}


//---------------------------------------------------------------------------//
// Restore
// Funcion: Se llama cuando hay que restaurar el objeto direct3d debido
// a un cambio de contexto (minimizar/maximizar, alt+tab en fullscreen)
// Hay que volver a crear los buffers y las texturas
//---------------------------------------------------------------------------//
void CDisplayDevice::Restore()
{
  // Para el render normal
  HRESULT hRet;
  hRet = m_pD3DDevice->GetBackBuffer (0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pColorBuffer);
  if (FAILED(hRet))
    D3D_ERROR("Error: Can't get BackBuffer surface", hRet);
  hRet = m_pD3DDevice->GetDepthStencilSurface (&m_pDepthBuffer);
  if (FAILED(hRet))
    D3D_ERROR("Error: Can't get DepthStencil surface", hRet);

  int size = 512;
  if      (m_Modo.iWidth >= 1024 || m_Modo.iHeight >= 1024) size = 2048;
  else if (m_Modo.iWidth >= 512  || m_Modo.iHeight >= 512 ) size = 1024;
  hRet = m_pD3DDevice->CreateDepthStencilSurface(size, size, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pDepthSurface, NULL);
  if (FAILED(hRet))
    D3D_ERROR("Error: Can't create the offscreen DepthStencil buffer", hRet);

  CreaVertexBuffers();
  CreaIndexBuffers ();
  CreaBasicShader  ();

  // Valores de render por defecto
  m_pD3DDevice->SetRenderState(D3DRS_CLIPPING, TRUE);

  if (m_pGestorMateriales)
    m_pGestorMateriales->Restore();

  SetRenderTarget(-1);
}


//---------------------------------------------------------------------------//
// Reset
// Reinicializa el dispositivo con los nuevos datos
//---------------------------------------------------------------------------//
void CDisplayDevice::Reset(TModoGrafico const &Modo)
{
  m_bAvailable = false;
  m_Modo       = Modo;
  int iDevice  = Modo.iDevice ? Modo.iDevice : D3DADAPTER_DEFAULT;

  m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_D3DDisplayMode);

  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory (&d3dpp, sizeof(d3dpp));
  d3dpp.Windowed               =!Modo.bFullScreen;
  d3dpp.BackBufferFormat       = Modo.bFullScreen ? D3DFMT_X8R8G8B8 : m_D3DDisplayMode.Format;
  d3dpp.BackBufferCount        = 1;
  d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.Flags                  = Modo.bLockable ? D3DPRESENTFLAG_LOCKABLE_BACKBUFFER : 0;
  d3dpp.BackBufferWidth        = Modo.iWidth;
  d3dpp.BackBufferHeight       = Modo.iHeight;
  d3dpp.hDeviceWindow          = (HWND)m_hWnd;
  d3dpp.PresentationInterval   = Modo.bFullScreen ? D3DPRESENT_INTERVAL_ONE : 0;
  d3dpp.FullScreen_RefreshRateInHz = Modo.bFullScreen ? Modo.iRefresh : 0;

  D3DCAPS9 Caps;
  m_iPixelShaderVersion = m_pD3D->GetDeviceCaps(iDevice, D3DDEVTYPE_HAL, &Caps);

  int ps_major = D3DSHADER_VERSION_MAJOR(Caps.PixelShaderVersion);
  int ps_minor = D3DSHADER_VERSION_MINOR(Caps.PixelShaderVersion);
  switch (ps_major)
  {
    case 1:
        switch (ps_minor)
        {
            case 0:  m_iPixelShaderVersion = PS_1_0; break;
            case 3:  m_iPixelShaderVersion = PS_1_3; break;
            case 4:  m_iPixelShaderVersion = PS_1_4; break;
            default: m_iPixelShaderVersion = PS_1_0; break;
        }
        break;
    case 2:
        m_iPixelShaderVersion = PS_2_0;
        break;
    case 3:
        m_iPixelShaderVersion = PS_3_0;
        break;
    default:
        m_iPixelShaderVersion = PS_NONE;
        break;
  }

  // Si aun no hay dispositivo, lo creamos
  if (!m_pD3DDevice)
  {
    HRESULT hRet = m_pD3D->CreateDevice (iDevice,
                                         D3DDEVTYPE_HAL, HWND(m_hWnd),
                                         D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                         &d3dpp,
                                         &m_pD3DDevice);
    if (FAILED(hRet))
      D3D_ERROR("ERR: Failed to create Direct3D9 device", hRet)
  }
  else
  {
    Release();
    if (FAILED(m_pD3DDevice->Reset(&d3dpp)))
    {
      // A las malas, si el Reset ha fallado (por un LostDevice) volvemos a crear el Dispositivo
      SAFE_RELEASE(m_pD3DDevice);
      HRESULT hRet = m_pD3D->CreateDevice (iDevice,
                                           D3DDEVTYPE_HAL, HWND(m_hWnd),
                                           D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_FPU_PRESERVE,
                                           &d3dpp,
                                           &m_pD3DDevice);
      if (FAILED(hRet))
        D3D_ERROR("ERR: Failed to create Direct3D9 device", hRet)
    }
    Restore();
  }

  m_bAvailable = true;
}


//---------------------------------------------------------------------------//
// SetViewport
//
//---------------------------------------------------------------------------//
void CDisplayDevice::SetViewport(TRect const &Viewport, float fMinZ, float fMaxZ)
{
  ASSERT (IsOk());

  TViewport NewViewport = TViewport(Viewport.x,Viewport.y, Viewport.x+Viewport.w, Viewport.y+Viewport.h);
  if (fMinZ != m_fMinZViewport || fMaxZ != m_fMaxZViewport || NewViewport.x0 != m_Viewport.x0 || NewViewport.y0 != m_Viewport.y0 || NewViewport.x1 != m_Viewport.x1 || NewViewport.y1 != m_Viewport.y1)
  {
    D3DVIEWPORT9 Vp;
    m_Viewport= NewViewport;
    Vp.X      = Viewport.x;
    Vp.Y      = Viewport.y;
    Vp.Width  = Viewport.w;
    Vp.Height = Viewport.h;
    Vp.MinZ   = fMinZ;
    Vp.MaxZ   = fMaxZ;
    m_fAspectRatio = (float)m_iTargetWidth/(float)m_iTargetHeight;
    HRESULT hRet = m_pD3DDevice->SetViewport(&Vp);
    ASSERT (SUCCEEDED (hRet));
  }
}


//---------------------------------------------------------------------------//
// SetRenderTarget
//
//---------------------------------------------------------------------------//
void CDisplayDevice::SetRenderTarget(int iIdTexturaRenderTarget)
{
  ASSERT (IsOk());

  D3DSURFACE *pSurfaceRenderTarget = NULL;
  D3DSURFACE *pSurfaceDepthStencil = NULL;

  if (iIdTexturaRenderTarget != m_iIdRenderingToSurface)
  {
    bool bBegin = m_bDrawingScene;
    if (m_bDrawingScene)
      EndScene();
    
    /////////////////////////////////////////////////////////////
    CTextura *pTex = g_pGestorMateriales->GetTextura(iIdTexturaRenderTarget);
    if (iIdTexturaRenderTarget != -1)
      pSurfaceRenderTarget = pTex->GetSurfaceD3D();
    else
      pSurfaceRenderTarget = m_pColorBuffer;

    /////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////
    if (iIdTexturaRenderTarget != -1)
    {
      if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pDepthSurface)))
        GLOG(("ERR:: Can't set stencil target for texture %d\n", iIdTexturaRenderTarget));
    }
    else
    {
      if (FAILED(m_pD3DDevice->SetDepthStencilSurface(m_pDepthBuffer)))
        GLOG(("ERR:: Can't set stencil target for texture %d\n", iIdTexturaRenderTarget));
    }

    /*
    if (iIdTexturaDepthStencil != -1)
    {
      //CHardwareMaterial *pMaterial = m_pGestorMateriales->GetMaterial(iIdTexturaDepthStencil);
      //pSurfaceDepthStencil = pMaterial->m_pSurface;
    }
    else
    {
      //HRESULT hRet = m_pD3DDevice->GetDepthStencilSurface (&pSurfaceDepthStencil);
      //ASSERT (SUCCEEDED(hRet));
    }
    */

    m_iIdRenderingToSurface = iIdTexturaRenderTarget;
    //ASSERT (pSurfaceRenderTarget && pSurfaceDepthStencil);
    if (FAILED(m_pD3DDevice->SetRenderTarget (0, pSurfaceRenderTarget)))
      GLOG(("ERR: Can't set render target for texture %d\n", iIdTexturaRenderTarget));

    // Viewport
    if (m_iIdRenderingToSurface == -1)
    {
      m_iTargetWidth  = m_Modo.iWidth;
      m_iTargetHeight = m_Modo.iHeight;
    }
    else
    {
      m_iTargetWidth  = pTex->GetWidth ();
      m_iTargetHeight = pTex->GetHeight();
    }
    SetViewport(TRect(0,0,m_iTargetWidth,m_iTargetHeight), 0.f, 1.f);
    float f;
    f = 1.f / m_iTargetWidth;
    CMotor3D::SetRegisterData(CMotor3D::V_INVSCREENW, &f);
    f = 1.f / m_iTargetHeight;
    CMotor3D::SetRegisterData(CMotor3D::V_INVSCREENH, &f);

    if (bBegin)
      BeginScene();
  }
}


//---------------------------------------------------------------------------//
// SetBilinearFiltering
//
//---------------------------------------------------------------------------//
void CDisplayDevice::SetBilinearFiltering(int iStage, bool bEnable)
{
  ASSERT (IsOk());

  if (bEnable)
  {
    m_pD3DDevice->SetSamplerState(iStage, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pD3DDevice->SetSamplerState(iStage, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  }
  else
  {
    m_pD3DDevice->SetSamplerState(iStage, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    m_pD3DDevice->SetSamplerState(iStage, D3DSAMP_MINFILTER, D3DTEXF_POINT);
  }
}


//---------------------------------------------------------------------------//
// DesactivaStage
//
//---------------------------------------------------------------------------//
void CDisplayDevice::DesactivaStage(int iStage)
{
  m_pD3DDevice->SetTextureStageState(iStage, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
  m_pD3DDevice->SetTextureStageState(iStage, D3DTSS_COLOROP, D3DTOP_DISABLE);
  m_pD3DDevice->SetTexture(iStage, NULL);
}


//---------------------------------------------------------------------------//
// SetIdentity
//
//---------------------------------------------------------------------------//
void CDisplayDevice::SetIdentity()
{
  m_pD3DDevice->SetTransform(D3DTS_VIEW,       &m_MatIdentity);
  m_pD3DDevice->SetTransform(D3DTS_WORLD,      &m_MatIdentity);
  m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &m_MatIdentity); 
}

//---------------------------------------------------------------------------//
// ResetVideoMemory
//
//---------------------------------------------------------------------------//
void CDisplayDevice::ResetVideoMemory()
{
  ASSERT (IsOk());

  if (m_pD3DDevice)
  {
    //HRESULT hRet = m_pD3DDevice->ResourceManagerDiscardBytes(0);
    //if (FAILED(hRet))
    //  D3D_ERROR("Error al llamar al ResourceManagerDiscardBytes", hRet);
  }
}


//---------------------------------------------------------------------------//
// Clear
//
//---------------------------------------------------------------------------//
void CDisplayDevice::Clear(bool bClearTarget, bool bClearZBuffer, unsigned uColor, float fZ)
{
  ASSERT (IsOk());

  unsigned uFlags = 0;
  uFlags|= bClearTarget  ? D3DCLEAR_TARGET  : 0;
  uFlags|= bClearZBuffer ? D3DCLEAR_ZBUFFER : 0;

  HRESULT hRet = m_pD3DDevice->Clear(0, NULL, uFlags, uColor, fZ, 0);
  ASSERT (SUCCEEDED (hRet));
}


//---------------------------------------------------------------------------//
// BeginFrame
//
//---------------------------------------------------------------------------//
void CDisplayDevice::BeginFrame()
{
  ASSERT (IsOk());

  // Reset de los VertexBuffer
  for (unsigned uVB = 0; uVB < HCV_MAX_CUSTOM_VERTEX; uVB++)
  {
    TVertexBufferInfo *pVBInfo = &s_VBInfo[uVB];
    pVBInfo->uLockIndex = 0;
    pVBInfo->uDrawIndex = 0;
  }

  // Reset del IndexBuffer
  s_IBInfo.uLockIndex = 0;
  s_IBInfo.uDrawIndex = 0;

  SetRenderTarget(-1);
}


//---------------------------------------------------------------------------//
// EndFrame
//
//---------------------------------------------------------------------------//
void CDisplayDevice::EndFrame()
{
}


//---------------------------------------------------------------------------//
// BeginScene
//
//---------------------------------------------------------------------------//
void CDisplayDevice::BeginScene()
{
  ASSERT (IsOk());
  ASSERTM(!m_bDrawingScene, "CDisplayDevice::BeginScene()");

  HRESULT hRet = m_pD3DDevice->BeginScene();
  ASSERT (SUCCEEDED (hRet));
  m_bDrawingScene = true;
}


//---------------------------------------------------------------------------//
// EndScene
//
//---------------------------------------------------------------------------//
void CDisplayDevice::EndScene()
{
  ASSERT (IsOk());
  ASSERTM(m_bDrawingScene, "CDisplayDevice::EndScene()");

  HRESULT hRet = m_pD3DDevice->EndScene();
  ASSERT (SUCCEEDED (hRet));
  m_bDrawingScene = false;
}


//---------------------------------------------------------------------------//
// DrawPrimitive
//
//---------------------------------------------------------------------------//
void CDisplayDevice::DrawPrimitive(unsigned uCustomVertex, int iPrimitive, int iNumPrimitives)
{
  ASSERT (IsOk());
  ASSERT (uCustomVertex < HCV_MAX_CUSTOM_VERTEX);
  TVertexBufferInfo *pVBInfo = &s_VBInfo[uCustomVertex];

  m_pD3DDevice->SetVertexDeclaration(pVBInfo->pDecl);
  m_pD3DDevice->SetStreamSource     (0, pVBInfo->pVertexBuffer, 0, pVBInfo->uSize);
  m_pD3DDevice->DrawPrimitive       ((D3DPRIMITIVETYPE)iPrimitive, pVBInfo->uDrawIndex, iNumPrimitives);
}


//---------------------------------------------------------------------------//
// BeginFrame
//
//---------------------------------------------------------------------------//
void CDisplayDevice::DrawIndexedPrimitive(unsigned uCustomVertex, int iPrimitive, int iNumPrimitives)
{
  ASSERT (IsOk());
  ASSERT (uCustomVertex < HCV_MAX_CUSTOM_VERTEX);
  TVertexBufferInfo *pVBInfo = &s_VBInfo[uCustomVertex];
  TIndexBufferInfo  *pIBInfo = &s_IBInfo;

  m_pD3DDevice->SetVertexDeclaration(pVBInfo->pDecl);
  m_pD3DDevice->SetIndices          (pIBInfo->pIndexBuffer);
  m_pD3DDevice->SetStreamSource     (0, pVBInfo->pVertexBuffer, 0, pVBInfo->uSize);
  m_pD3DDevice->DrawIndexedPrimitive((D3DPRIMITIVETYPE)iPrimitive, pVBInfo->uDrawIndex, 0, pVBInfo->uLockIndex - pVBInfo->uDrawIndex, pIBInfo->uDrawIndex, iNumPrimitives);
}


//---------------------------------------------------------------------------//
// ProcessVertices
//
//---------------------------------------------------------------------------//
void CDisplayDevice::ProcessVertices(unsigned uCustomVertex, int iNumVertices)
{
  ASSERT (IsOk());
  ASSERT (uCustomVertex < HCV_MAX_CUSTOM_VERTEX);
  TVertexBufferInfo *pVBInfo = &s_VBInfo[uCustomVertex];

  m_pD3DDevice->SetVertexDeclaration(pVBInfo->pDecl);
  m_pD3DDevice->SetStreamSource     (0, pVBInfo->pVertexBuffer, 0, pVBInfo->uSize);
  m_pD3DDevice->ProcessVertices     (pVBInfo->uDrawIndex, pVBInfo->uLockIndex, iNumVertices, pVBInfo->pVertexBuffer, NULL, D3DPV_DONOTCOPYDATA);
}


//---------------------------------------------------------------------------//
// LockVertexBuffer
//
//---------------------------------------------------------------------------//
void *CDisplayDevice::LockVertexBuffer(unsigned uCustomVertex, unsigned uNumVertex)
{
  ASSERT (!m_bVertexLock);
  ASSERT (IsOk());
  ASSERT (uCustomVertex < HCV_MAX_CUSTOM_VERTEX);
  TVertexBufferInfo *pVBInfo = &s_VBInfo[uCustomVertex];

  // Gestion para que crezcan los vertices
  if (uNumVertex >= pVBInfo->uNumVertex)
  {
    EndScene();
    BeginScene();

    GLOG (("INF: uNumVertex(%d) > LockVertexBuffer(%d), growing buffer\n", uNumVertex, pVBInfo->uNumVertex));
    pVBInfo->uNumVertex = uNumVertex + 100;
    SAFE_RELEASE(pVBInfo->pVertexBuffer);
    HRESULT hRet = m_pD3DDevice->CreateVertexBuffer (pVBInfo->uNumVertex * pVBInfo->uSize,
                                                     D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
                                                     0,
                                                     D3DPOOL_DEFAULT,
                                                     &pVBInfo->pVertexBuffer,
                                                     NULL);
    pVBInfo->uLockIndex = 0;
  }

  void *pVertices = NULL;
  if ((pVBInfo->uLockIndex+uNumVertex) >= pVBInfo->uNumVertex)
    pVBInfo->uLockIndex = 0;

  HRESULT     hRet    = pVBInfo->pVertexBuffer->Lock(pVBInfo->uSize * pVBInfo->uLockIndex, pVBInfo->uSize * uNumVertex, (void **)&pVertices, pVBInfo->uLockIndex ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
  pVBInfo->uDrawIndex = pVBInfo->uLockIndex;
  pVBInfo->uLockIndex+= uNumVertex;
  ASSERT (SUCCEEDED (hRet));
  m_bVertexLock = true;

  return pVertices;
}


//---------------------------------------------------------------------------//
// UnlockVertexBuffer
//
//---------------------------------------------------------------------------//
void CDisplayDevice::UnlockVertexBuffer(unsigned uCustomVertex)
{
  ASSERT (m_bVertexLock);
  ASSERT (IsOk());
  ASSERT (uCustomVertex < HCV_MAX_CUSTOM_VERTEX);
  TVertexBufferInfo *pVBInfo = &s_VBInfo[uCustomVertex];

  HRESULT hRet = pVBInfo->pVertexBuffer->Unlock();
  ASSERT (SUCCEEDED (hRet));

  m_bVertexLock = false;
}


//---------------------------------------------------------------------------//
// LockIndexBuffer
//
//---------------------------------------------------------------------------//
ushort *CDisplayDevice::LockIndexBuffer(unsigned uNumIndex)
{
  ASSERT (IsOk());
  TIndexBufferInfo *pIBInfo = &s_IBInfo;
  ASSERT (uNumIndex > 0 && uNumIndex < pIBInfo->uNumIndex);

  ushort   *pIndices  = NULL;
  unsigned  uSize     = uNumIndex * pIBInfo->uSize;
  unsigned  uFlags    = D3DLOCK_NOOVERWRITE;
  if ((pIBInfo->uLockIndex+uNumIndex) >= pIBInfo->uNumIndex)
  {
    uFlags = D3DLOCK_DISCARD;
    pIBInfo->uLockIndex = 0;
  }
  HRESULT     hRet    = pIBInfo->pIndexBuffer->Lock(pIBInfo->uLockIndex * pIBInfo->uSize, uSize, (void **)&pIndices, uFlags);
  pIBInfo->uDrawIndex = pIBInfo->uLockIndex;
  pIBInfo->uLockIndex+= uNumIndex;
  ASSERT (SUCCEEDED (hRet));

  return pIndices;
}


//---------------------------------------------------------------------------//
// UnlockIndexBuffer
//
//---------------------------------------------------------------------------//
void CDisplayDevice::UnlockIndexBuffer()
{
  ASSERT (IsOk());
  TIndexBufferInfo *pIBInfo = &s_IBInfo;

  HRESULT hRet = pIBInfo->pIndexBuffer->Unlock();
  ASSERT (SUCCEEDED (hRet));
}


//---------------------------------------------------------------------------//
// CreaVertexBuffers
//
//---------------------------------------------------------------------------//
void CDisplayDevice::CreaVertexBuffers()
{
  ASSERT (IsOk());
  
  for (int i = 0; i < HCV_MAX_CUSTOM_VERTEX; i++)
  {
    TVertexBufferInfo *pVBInfo = &s_VBInfo[i];
    HRESULT hRet = m_pD3DDevice->CreateVertexBuffer (pVBInfo->uNumVertex * pVBInfo->uSize,
                                                     D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
                                                     0,
                                                     D3DPOOL_DEFAULT,
                                                     &pVBInfo->pVertexBuffer,
                                                     NULL);
    m_pD3DDevice->CreateVertexDeclaration(pVBInfo->pElements, &pVBInfo->pDecl);
    ASSERT (SUCCEEDED (hRet));
  }
}


//---------------------------------------------------------------------------//
// LiberaVertexBuffers
//
//---------------------------------------------------------------------------//
void CDisplayDevice::LiberaVertexBuffers()
{
  ASSERT (!m_bVertexLock);
  ASSERT (IsOk());

  for (int i = 0; i < HCV_MAX_CUSTOM_VERTEX; i++)
  {
    TVertexBufferInfo *pVBInfo = &s_VBInfo[i];
    SAFE_RELEASE(pVBInfo->pVertexBuffer);
    SAFE_RELEASE(pVBInfo->pDecl);
  }
}


//---------------------------------------------------------------------------//
// CreaIndexBuffers
//
//---------------------------------------------------------------------------//
void CDisplayDevice::CreaIndexBuffers()
{
  ASSERT (IsOk());
  
  // Crear todos los IndexBuffer
  TIndexBufferInfo *pIBInfo = &s_IBInfo;
  HRESULT hRet = m_pD3DDevice->CreateIndexBuffer (pIBInfo->uNumIndex * pIBInfo->uSize,
                                                  D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
                                                  (D3DFORMAT)pIBInfo->uFormat,
                                                  D3DPOOL_DEFAULT,
                                                  &pIBInfo->pIndexBuffer,
                                                  NULL);
  ASSERT (SUCCEEDED (hRet));
}


//----------------------------------------------------------------------
// LiberaIndexBuffers
//
//----------------------------------------------------------------------
void CDisplayDevice::LiberaIndexBuffers()
{
  ASSERT (IsOk());

  // Liberar todos los IndexBuffer
  SAFE_RELEASE(s_IBInfo.pIndexBuffer);
}


//---------------------------------------------------------------------------//
// ApplyBasicShader
//
//---------------------------------------------------------------------------//
void CDisplayDevice::ApplyBasicShader()
{
  m_pBasicBST->Apply();
}


//---------------------------------------------------------------------------//
// CreaBasicShader
//
//---------------------------------------------------------------------------//
void CDisplayDevice::CreaBasicShader()
{
  m_pD3DDevice->BeginStateBlock();

  // Fill Mode
  m_pD3DDevice->SetRenderState(D3DRS_FILLMODE,          D3DFILL_SOLID);
  m_pD3DDevice->SetRenderState(D3DRS_CULLMODE,          D3DCULL_NONE);
  m_pD3DDevice->SetRenderState(D3DRS_CLIPPING,          TRUE);
  m_pD3DDevice->SetRenderState(D3DRS_ZENABLE,           D3DZB_TRUE);
  m_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE,      D3DZB_TRUE);
  m_pD3DDevice->SetRenderState(D3DRS_ZFUNC,             D3DCMP_LESSEQUAL);
  m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC,         D3DCMP_ALWAYS);
  m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,           D3DBLENDOP_ADD);
  m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,  FALSE);
  m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE,   FALSE);
  m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE,         FALSE);
  m_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
  m_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE,  FALSE);
  m_pD3DDevice->SetRenderState(D3DRS_LIGHTING,          FALSE);
  
  for (int i = 0; i < 4; i++)
  {
    m_pD3DDevice->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    m_pD3DDevice->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    m_pD3DDevice->SetSamplerState(i, D3DSAMP_ADDRESSU,  D3DTADDRESS_WRAP);
    m_pD3DDevice->SetSamplerState(i, D3DSAMP_ADDRESSV,  D3DTADDRESS_WRAP);
    m_pD3DDevice->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, i);
  }

  m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
  m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

  m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
  m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

  m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
  m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

  m_pD3DDevice->SetVertexShader(NULL);
  m_pD3DDevice->SetPixelShader (NULL);

  // Fin del bloque
  m_pD3DDevice->EndStateBlock(&m_pBasicBST);
}


//---------------------------------------------------------------------------//
// SetBlendMode
//
//---------------------------------------------------------------------------//
void CDisplayDevice::SetBlendMode(int aBlend, float fAlpha)
{
  unsigned uAlpha = (unsigned)(fAlpha * 255);
  switch (aBlend)
  {
      case BL_ALPHABLEND:
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        break;
      case BL_ADDITIVE:
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        break;
      case BL_SUBTRACTIVE:
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        break;
      case BL_INVERT:
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SUBTRACT);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        break;
      case BL_INVERTDEST:
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_SUBTRACT);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        break;
      case BL_MASK:
        m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB(255-uAlpha, 255,255,255));
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_ADD);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_BOTHINVSRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        break;
      case BL_MULTIPLY:
        m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR, HARD_COLOR_ARGB(uAlpha, 255,255,255));
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SUBTRACT);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        break;
      case BL_INVMULTIPLY:
        m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,   HARD_COLOR_ARGB(uAlpha, 255,255,255));
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        break;
      case BL_COLORMULTIPLY:
        m_pD3DDevice->SetRenderState(D3DRS_TEXTUREFACTOR,   HARD_COLOR_ARGB(uAlpha, 255,255,255));
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SUBTRACT);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
        m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        m_pD3DDevice->SetRenderState(D3DRS_BLENDOP,   D3DBLENDOP_REVSUBTRACT);
        m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCCOLOR);
        m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
        break;
  }
}
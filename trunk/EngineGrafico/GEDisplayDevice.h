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

#ifndef GE_DISPLAYDEVICE_H
#define GE_DISPLAYDEVICE_H

#include "Base.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "GEVector.h"
#include "GECustomVertex.h"

//---------------------------------------------------------------------------//
typedef IDirect3D9               D3D;
typedef IDirect3DDevice9         D3DDEVICE;
typedef IDirect3DVertexShader9   D3DVS;
typedef IDirect3DPixelShader9    D3DPS;
typedef IDirect3DTexture9        D3DTEXTURE;
typedef IDirect3DSurface9        D3DSURFACE;
typedef IDirect3DStateBlock9     D3DSTATEBLOCK;

//---------------------------------------------------------------------------//
struct TModoGrafico
{
  TModoGrafico() { iDevice = iWidth = iHeight = iDepth = iRefresh = 0; bFullScreen = false; bLockable = false; }
  TModoGrafico(int iDev, int iW, int iH, int iD, int iR, bool bF, bool bL) { iDevice = iDev; iWidth = iW; iHeight = iH; iDepth = iD; iRefresh = iR; bFullScreen = bF; bLockable = bL; }
  int  iDevice;
  int  iWidth;
  int  iHeight;
  int  iDepth;
  int  iRefresh;
  bool bFullScreen;
  bool bLockable;
};

//---------------------------------------------------------------------------//
#define  HARD_RELEASE(p)  { if (p) (p)->Release(); (p) = NULL; }

//---------------------------------------------------------------------------//
#define  HARD_COLOR_ARGB(a,r,g,b)                 D3DCOLOR_ARGB(a,r,g,b)
#define  HARD_COLOR_XRGB(r,g,b)                   D3DCOLOR_XRGB(r,g,b)
#define  HARD_COLOR_GET_A(color)                  ((color>>24) & 0xFF)
#define  HARD_COLOR_GET_R(color)                  ((color>>16) & 0xFF)
#define  HARD_COLOR_GET_G(color)                  ((color>> 8) & 0xFF)
#define  HARD_COLOR_GET_B(color)                  ((color    ) & 0xFF)
#define  HARD_COLOR_SET_A(color,v)                (color = (color&0x00FFFFFF) | ((v)<<24))
#define  HARD_COLOR_SET_R(color,v)                (color = (color&0xFF00FFFF) | ((v)<<16))
#define  HARD_COLOR_SET_G(color,v)                (color = (color&0xFFFF00FF) | ((v)<< 8))
#define  HARD_COLOR_SET_B(color,v)                (color = (color&0xFFFFFF00) | ((v)    ))
#define  HARD_PALETTE_TO_COLOR(PaleteEntryColor)  (*(unsigned *)&PaleteEntryColor)
#define  HARD_COLOR_A1R5G5B5(a,r,g,b)             ((a<<15) | (r<<10) | (g<<5) | (b))

// Tipos de primitivas hardware para el renderizado
enum
{
  HARD_PRIM_POINTLIST      = D3DPT_POINTLIST,
  HARD_PRIM_LINELIST       = D3DPT_LINELIST,
  HARD_PRIM_LINESTRIP      = D3DPT_LINESTRIP,
  HARD_PRIM_TRIANGLELIST   = D3DPT_TRIANGLELIST,
  HARD_PRIM_TRIANGLESTRIP  = D3DPT_TRIANGLESTRIP,
  HARD_PRIM_TRIANGLEFAN    = D3DPT_TRIANGLEFAN,
};

// Tipos de formato para una texura hardware
enum
{
  HARD_TEX_FORMAT_P8       = D3DFMT_P8,
  HARD_TEX_FORMAT_A8       = D3DFMT_A8,
  HARD_TEX_FORMAT_A8L8     = D3DFMT_A8L8,
  HARD_TEX_FORMAT_A4R4G4B4 = D3DFMT_A4R4G4B4,
  HARD_TEX_FORMAT_R5G6B5   = D3DFMT_R5G6B5,
  HARD_TEX_FORMAT_A1R5G5B5 = D3DFMT_A1R5G5B5,
  HARD_TEX_FORMAT_A8R8G8B8 = D3DFMT_A8R8G8B8,
  HARD_TEX_FORMAT_R8G8B8   = D3DFMT_R8G8B8,
  HARD_TEX_FORMAT_D16      = D3DFMT_D16,
  HARD_TEX_FORMAT_D24S8    = D3DFMT_D24S8,

  HARD_MAX_TEXTURE_FORMATS = 7,
};

// Flags para las texturas hardware
enum
{
  HARD_TEX_FLAG_RENDERTARGET = 1<<0,
  HARD_TEX_FLAG_DEPTHSTENCIL = 1<<1,
};

//---------------------------------------------------------------------------//
enum
{
  BL_ALPHABLEND,
  BL_ADDITIVE,
  BL_SUBTRACTIVE,
  BL_INVERT,
  BL_INVERTDEST,
  BL_MASK,
  BL_MULTIPLY,
  BL_INVMULTIPLY,
  BL_COLORMULTIPLY,
};

enum
{
  PS_NONE,
  PS_1_0,
  PS_1_3,
  PS_1_4,
  PS_2_0,
  PS_3_0,
};

class CGestorMateriales;

//---------------------------------------------------------------------------//
// La clase CDisplayDevice
//---------------------------------------------------------------------------//
class CDisplayDevice
{
  public:
                        CDisplayDevice       () { m_bOk = false; }
                       ~CDisplayDevice       () { End(); }
                                             
    bool                Init                 (TModoGrafico const &Modo, int hWnd);
    void                End                  ();
    bool                IsOk                 () const { return m_bOk; }

    void                Clear                (bool bClearTarget, bool bClearZBuffer, unsigned uColor, float fZ);
    void                Present              ();
    void                Reset                (TModoGrafico const &Modo);
    void                Release              ();
    void                Restore              ();
    void                ResetVideoMemory     ();
                                             
    void                PageFlip             ();
                      
    // Scene          
    void                BeginFrame           ();
    void                EndFrame             ();
    void                BeginScene           ();
    void                EndScene             ();
    bool                IsDrawingScene       () const { return m_bDrawingScene; }
    bool                IsAvailable          () const { return m_bAvailable; }

    // Draws                          
    void                DrawPrimitive        (unsigned uCustomVertex, int iPrimitive, int iNumPrimitives);
    void                DrawIndexedPrimitive (unsigned uCustomVertex, int iPrimitive, int iNumPrimitives);
    // PV
    void                ProcessVertices      (unsigned uCustomVertex, int iNumVertices);

    // Vertex/Index buffers           
    void               *LockVertexBuffer     (unsigned uCustomVertex, unsigned uNumVertex);
    void                UnlockVertexBuffer   (unsigned uCustomVertex);

    ushort             *LockIndexBuffer      (unsigned uNumIndex);
    void                UnlockIndexBuffer    ();

    // Vertex/Index buffers
    void                CreaVertexBuffers    ();
    void                LiberaVertexBuffers  ();

    void                CreaIndexBuffers     ();
    void                LiberaIndexBuffers   ();

    // Miscelanea util                
    void                SetViewport          (TRect const &Viewport, float fMinZ, float fMaxZ);
    TViewport    const &GetViewport          () const { return m_Viewport;      }
    TModoGrafico const &GetModoGrafico       () const { return m_Modo;          }
    int                 ViewportWidth        () const { return m_Viewport.x1-m_Viewport.x0; }
    int                 ViewportHeight       () const { return m_Viewport.y1-m_Viewport.y0; }
    int                 Width                () const { return m_iTargetWidth;  }
    int                 Height               () const { return m_iTargetHeight; }
    void                SetRenderTarget      (int iIdTexturaRenderTArget);
    void                SetTextureFactor     (unsigned uColor);
    void                SetBilinearFiltering (int iStage, bool bEnable);
    void                SetAlphaTest         (bool bEnable);
    void                DesactivaStage       (int iStage);
    void                SetAspectRatio       (float fAspectRatio) { m_fAspectRatio = fAspectRatio; }
    float               GetAspectRatio       () const { return m_fAspectRatio; }
    void                SetIdentity          ();
    void                SetBlendMode         (int aBlend, float fAlpha);
    
    // Gets
    D3D                *GetD3D               () { return m_pD3D; }
    D3DDEVICE          *GetD3DDevice         () { return m_pD3DDevice; }
    int                 GetRenderTarget      () const { return m_iIdRenderingToSurface; }
    D3DSURFACE         *GetBackBufferSurface () const { return m_pColorBuffer; }
    int                 GetPixelShaderVersion() const { return m_iPixelShaderVersion; }

    // Shader      
    void                ApplyBasicShader     ();

  private:              

    void                BorraVars            ();
    void                LiberaVars           ();

    void                CreaBasicShader      ();

  private:

    bool                m_bOk;
    int                 m_hWnd;
    D3D                *m_pD3D;
    D3DDEVICE          *m_pD3DDevice;
    D3DDISPLAYMODE      m_D3DDisplayMode;

    TModoGrafico        m_Modo;
    int                 m_iPixelShaderVersion;
    bool                m_bVertexLock;
    bool                m_bDrawingScene;
    bool                m_bAvailable;
    TViewport           m_Viewport;
    float               m_fAspectRatio;
    float               m_fMinZViewport;
    float               m_fMaxZViewport;
    int                 m_iTargetWidth;
    int                 m_iTargetHeight;

    int                 m_iIdRenderingToSurface;
    D3DSURFACE         *m_pColorBuffer;
    D3DSURFACE         *m_pDepthBuffer;
    D3DSURFACE         *m_pDepthSurface;
    D3DXMATRIX          m_MatIdentity;

    CGestorMateriales  *m_pGestorMateriales;

    D3DSTATEBLOCK      *m_pBasicBST;
};

#endif

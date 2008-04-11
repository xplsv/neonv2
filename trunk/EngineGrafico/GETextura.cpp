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
#include "GEEngineGrafico.h"
#include "Tga.h"


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CTextura::Init(int iWidth, int iHeight, int iFormat, bool bDynamic, bool bRenderTarget, bool bDepthStencil, unsigned uFlags)
{
  // Datos
  m_bLocked  = false;
  m_pTextura = NULL;
  m_pSurface = NULL;
  m_iCount   = 0;
  m_iWidth   = iWidth;
  m_iHeight  = iHeight;
  m_iFormat  = iFormat;
  m_bDynamic = bDynamic;
  m_uFlags   = uFlags;
  m_uUsage   = (bDepthStencil  ? D3DUSAGE_DEPTHSTENCIL : 0);
  m_uUsage  |= (bRenderTarget  ? D3DUSAGE_RENDERTARGET : 0);
  m_uUsage  |= (bDynamic       ? D3DUSAGE_DYNAMIC      : 0);
  m_uPool    = (m_uUsage != 0) ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
  m_uPool    = ((uFlags & SYSTEMMEM) ? D3DPOOL_SYSTEMMEM : m_uPool);
  Restore();
  if (m_pTextura)
    m_bOk = true;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CTextura::Init(const string &sFile, unsigned uFlags)
{
  // Nombre
  GetCurrentDirectory(MAX_PATH, m_Dir);
  m_Filename = sFile;
  m_pTextura = NULL;
  m_pSurface = NULL;
  m_iCount   = 0;
  m_uFlags   = uFlags;
  Restore();
  if (m_pTextura)
    m_bOk = true;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CTextura::End()
{
  if (IsOk())
  {
    ASSERTM(!m_iCount, "Textura con referencias no liberadas");
    Release();
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// Release
//
//---------------------------------------------------------------------------//
void CTextura::Release()
{
  HARD_RELEASE(m_pSurface);
  HARD_RELEASE(m_pTextura);
}


//---------------------------------------------------------------------------//
// Restore
//
//---------------------------------------------------------------------------//
void CTextura::Restore()
{
  LPDIRECT3DDEVICE9 pD3D = g_DisplayDevice.GetD3DDevice();

  bool bCube = (m_uFlags & CUBE) != 0;
  if (m_Filename == "")
  {
    D3DFORMAT uFormat;
    switch (m_iFormat)
    {
      case HARD_TEX_FORMAT_A8R8G8B8: uFormat = D3DFMT_A8R8G8B8; break;
      case HARD_TEX_FORMAT_A4R4G4B4: uFormat = D3DFMT_A4R4G4B4; break;
      case HARD_TEX_FORMAT_R5G6B5:   uFormat = D3DFMT_R5G6B5;   break;
      case HARD_TEX_FORMAT_A1R5G5B5: uFormat = D3DFMT_A1R5G5B5; break;
    }
    // Create Texture
    //if (bCube)
    //  pD3D->CreateCubeTexture(1, 1, m_uUsage, uFormat, (D3DPOOL)m_uPool, &m_pTextura, NULL);
    //else
      pD3D->CreateTexture(m_iWidth, m_iHeight, 1, m_uUsage, uFormat, (D3DPOOL)m_uPool, &m_pTextura, NULL);

    if (m_pTextura)
    {
      m_pTextura->GetSurfaceLevel(0, &m_pSurface);
      if (m_pSurface)
      {
        pD3D->ColorFill(m_pSurface, NULL, 0);
      }
    }
  }
  else
  {
    PushDir(m_Dir);
    CFichero File;
    if (File.Open(m_Filename.c_str(), "rb"))
    {
      int   iSize = File.GetLength();
      byte *pData = NEW_ARRAY(byte, iSize);
      File.Read (pData, iSize, 1);
      File.Close();
  
      HRESULT hRes = D3DXCreateTextureFromFileInMemory(g_DisplayDevice.GetD3DDevice(), pData, iSize, &m_pTextura);
      if (SUCCEEDED(hRes))
      {
        D3DXIMAGE_INFO  ImageInfo;
        D3DXGetImageInfoFromFileInMemory(pData, iSize, &ImageInfo);
        m_pTextura->GetSurfaceLevel(0, &m_pSurface);
        m_iWidth   = ImageInfo.Width;
        m_iHeight  = ImageInfo.Height;
        m_bDynamic = false;
        /*
        D3DSURFACE_DESC Desc;
        m_pTextura->GetLevelDesc(0, &Desc);
        m_pSurface = NULL;
        m_iWidth   = Desc.Width;
        m_iHeight  = Desc.Height;
        m_bDynamic = false;
        */
      }
      DISPOSE_ARRAY(pData);
    }
    PopDir();
  }
}


//---------------------------------------------------------------------------//
// Lock
//
//---------------------------------------------------------------------------//
bool CTextura::Lock(TRect const *pRect, TSurfaceDesc &Desc)
{
  ASSERT(!m_bLocked);
  if (m_bLocked)
    return false;

  bool bRes = false;
  LPDIRECT3DSURFACE9 pSurface;
  if (SUCCEEDED(m_pTextura->GetSurfaceLevel(0, &pSurface)))
  {
    D3DLOCKED_RECT LockInfo;
    RECT LockRect;
    // Si nos pasan el rectangulo lo usamos
    HRESULT hr;
    if (pRect)
    {
      LockRect.left   = pRect->x;
      LockRect.top    = pRect->y;
      LockRect.right  = pRect->w - pRect->x;
      LockRect.bottom = pRect->h - pRect->y;
    }
    else
    {
      LockRect.left   = 0;
      LockRect.top    = 0;
      LockRect.right  = m_iWidth;
      LockRect.bottom = m_iHeight;
    }
    hr = pSurface->LockRect(&LockInfo, &LockRect, 0);
    // Todo ok?
    if (SUCCEEDED(hr))
    {
      Desc.iWidth = m_iWidth;
      Desc.iHeight= m_iHeight;
      Desc.iPitch = LockInfo.Pitch;
      Desc.pBits  = LockInfo.pBits;
      m_bLocked   = true;
      bRes = true;
    }
    HARD_RELEASE(pSurface);
  }
  return bRes;
}


//---------------------------------------------------------------------------//
// Unlock
//
//---------------------------------------------------------------------------//
bool CTextura::Unlock()
{
  ASSERT(m_bLocked);
  if (!m_bLocked)
    return false;

  bool bRes = false;
  LPDIRECT3DSURFACE9 pSurface;
  if (SUCCEEDED(m_pTextura->GetSurfaceLevel(0, &pSurface)))
  {
    if (SUCCEEDED(pSurface->UnlockRect()))
    {
      m_bLocked = false;
      bRes = true;
    }
    HARD_RELEASE(pSurface);
  }
  return bRes;
}


//---------------------------------------------------------------------------//
// Set
//
//---------------------------------------------------------------------------//
void CTextura::Set(int iStage)
{
  if (m_pTextura)
    g_DisplayDevice.GetD3DDevice()->SetTexture(iStage, m_pTextura);
}

//---------------------------------------------------------------------------//
// LoadFromTGA
//
//---------------------------------------------------------------------------//
/*
bool CTextura::LoadFromTGA(const char *pszFilename)
{
  CFichero   File;
  TGAHeader  Hdr;

  bool bRes = false;
  if (!File.Open(pszFilename))
    return false;

  File.Read(&Hdr, sizeof(TGAHeader), 1);
  if (Hdr.PixelDepth < 24)
    return false;
  
  if (!Init(Hdr.ImageWidth, Hdr.ImageHeight, HARD_TEX_FORMAT_A8R8G8B8))
    return false;

  // Nombre
  m_pFilename = AllocCopyString(pszFilename);

  TSurfaceDesc Desc;
  if (Lock(NULL, Desc))
  {
    int   iSize = m_iWidth*m_iHeight*Hdr.PixelDepth;
    byte *pTemp = NEW_ARRAY(byte, iSize);
    File.Read (pTemp, iSize, 1);
    File.Close();

    //-- Leemos la imagen (de abajo a arriba) --//
    unsigned *pDst   = (unsigned *)Desc.pBits;
    int       iPitch = (Desc.iPitch/4) - m_iWidth;
    if (Hdr.PixelDepth == 32)
    {
      // 32bpp
      for (int y = m_iHeight; y; y--)
      {
        byte *pSrc = pTemp + (y-1)*m_iWidth*4;
        for (int x = m_iHeight; x; x--)
        {
          byte b = *pSrc++;
          byte g = *pSrc++;
          byte r = *pSrc++;
          byte a = *pSrc++;
          *pDst++ = HARD_COLOR_ARGB(a,r,g,b);
        }
      }
    }
    else if (Hdr.PixelDepth == 24)
    {
      // 24bpp
      for (int y = m_iHeight; y; y--)
      {
        byte *pSrc = pTemp + (y-1)*m_iWidth*4;
        for (int x = m_iHeight; x; x--)
        {
          byte b = *pSrc++;
          byte g = *pSrc++;
          byte r = *pSrc++;
          *pDst++ = HARD_COLOR_ARGB(255,r,g,b);
        }
      }
    }
    DISPOSE_ARRAY(pTemp);
    bRes = true;
  }

  return bRes;
}
*/

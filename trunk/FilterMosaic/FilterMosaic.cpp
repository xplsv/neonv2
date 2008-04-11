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
// FilterMosaic
// DLL Filter example for Neon v2.5
//----------------------------------------------------------------------------

#include "N2_Utils.h"
#include <vector>
#include <string>

using std::string;
using std::vector;
using std::pair;

//----------------------------------------------------------------------------
// Main entry point for this DLL
//----------------------------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
  }
  return TRUE;
}

//----------------------------------------------------------------------------
enum
{
  STRETCH_WH,
  STRETCH_W,
  STRETCH_H,
};

/// My Vars
#define MIN_WIDTH   8
#define MIN_HEIGHT  6
#define MAX_WIDTH   40
#define MAX_HEIGHT  30
#define MAX_BLOCKS  MAX_WIDTH*MAX_HEIGHT

//----------------------------------------------------------------------------
// Vars for the GUI
static TCtrlVar m_Vars[] =
{
  {TCtrlVar::COMBO_BOX, 0, "Type",   false, 3, {"Stretch WH", "Stretch W", "Stretch H"}},
  {TCtrlVar::SLIDER,    1, "Intens", true,  0, {NULL}},
  {TCtrlVar::SLIDER,    2, "Size",   true,  0, {NULL}},
  {TCtrlVar::CHECK_BOX, 3, "Alpha",  true,  0, {NULL}},
  {TCtrlVar::INVALID},
};

//----------------------------------------------------------------------------
class CFilter
{
  public:
  
    // Init
    bool Init(void *pFilterInstance, IDirect3DDevice9 *pD3DDevice, vector< pair<string,string> > const &aOptions)
    {
      m_fIntens = 0.f;
      m_fSize   = 0.2f;
      m_iType   = STRETCH_WH;
      m_bAlpha  = false;
      m_pUtils  = new CUtils;
      m_pUtils->Init(pD3DDevice, MAX_BLOCKS*6);
      m_pFilterInstance = pFilterInstance;
      UpdateBlocks();
      return true;
    }
    // End
    void End()
    {
      m_pUtils->End();
      delete m_pUtils;
    }

    // GetVarCtrls
    TCtrlVar *GetVarCtrls()
    {
      return m_Vars;
    }

    // SetVar
    void SetVar(int iVar, void *pData)
    {
      switch (iVar)
      {
        case 0: m_iType   = *(int   *)pData; break;
        case 1: m_fIntens = *(float *)pData; break;
        case 2: m_fSize   = *(float *)pData; UpdateBlocks(); break;
        case 3: m_bAlpha  = *(bool  *)pData; break;
      }
    }

    // GetVar
    void *GetVar(int iVar)
    {
      switch (iVar)
      {
        case 0: return &m_iType;
        case 1: return &m_fIntens;
        case 2: return &m_fSize;
        case 3: return &m_bAlpha;
      }
      return NULL;
    }

    // SetTime
    void SetTime(float fTime)
    {
    }

    // Draw
    bool Apply(TFilterTgt *pFilterTgt)
    {
      IDirect3DDevice9 *m_pD3DDevice = m_pUtils->GetD3DDevice();
      // Fill Mode
      D3DVIEWPORT9 vp;
      m_pD3DDevice->GetViewport(&vp);
      m_fBlockW     = (vp.Width * m_fInvW);
      m_fBlockH     = (vp.Height * m_fInvH);
      m_fHalfBlockW = (m_fBlockW*0.5f);
      m_fHalfBlockH = (m_fBlockH*0.5f);

      // Set the source texture, the blend mode, and then draw a screen quad
      IDirect3DSurface9 *pSurface;
      pFilterTgt->pTexTgt->GetSurfaceLevel(0, &pSurface);
      m_pD3DDevice->SetRenderTarget(0, pSurface);
      m_pD3DDevice->SetTexture(0, pFilterTgt->pTexSrc);
      m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0, 0, 0);

      ApplyBasicShader();
      TVertex_XYZRHW *pVertices = m_pUtils->LockVertexBuffer(m_iTotal * 6);
      for (int j = 0; j < m_iHeight; j++)
      {
        for (int i = 0; i < m_iWidth; i++)
        {
          FillQuad(pVertices, i,j, vp.Width, vp.Height);
          pVertices+=6;
        }
      }
      m_pUtils->UnlockVertexBuffer();
      m_pUtils->DrawPrimitive     (D3DPT_TRIANGLELIST, m_iTotal * 2);
      // Return true if we used TexTgt as target for the filter, false if we used pTexSrc
      return true;
    }

    void ApplyBasicShader()
    {
      IDirect3DDevice9 *m_pD3DDevice = m_pUtils->GetD3DDevice();
      m_pD3DDevice->SetRenderState      (D3DRS_CULLMODE,         D3DCULL_NONE);
      m_pD3DDevice->SetRenderState      (D3DRS_ZENABLE,          D3DZB_FALSE);
      m_pD3DDevice->SetRenderState      (D3DRS_ALPHABLENDENABLE, TRUE);
      m_pD3DDevice->SetRenderState      (D3DRS_BLENDOP,          D3DBLENDOP_ADD);
      m_pD3DDevice->SetRenderState      (D3DRS_ALPHAFUNC,        D3DCMP_ALWAYS);
      m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
      m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
      m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
      m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
      m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
      m_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
      m_pD3DDevice->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);
    }

  private:

    void UpdateBlocks();
    void FillQuad    (TVertex_XYZRHW *pVertices, int iX, int iY, int sW, int sH);

  private:

    void     *m_pFilterInstance;
    CUtils   *m_pUtils;
    float     m_aBlocks[MAX_BLOCKS];
    float     m_fIntens;
    float     m_fSize;
    int       m_iType;
    bool      m_bAlpha;
    int       m_iWidth;
    int       m_iHeight;
    int       m_iTotal;
    float     m_fInvW;
    float     m_fInvH;
    float     m_fBlockW;
    float     m_fBlockH;
    float     m_fHalfBlockW;
    float     m_fHalfBlockH;
};


//----------------------------------------------------------------------------
// Filter_Init
// 
// Called when the program wants an instance of this filter
//----------------------------------------------------------------------------
EXPORT_DLL int Filter_Init(unsigned uVersion, void *pFilterInstance, IDirect3DDevice9 *pD3DDevice, vector< pair<string,string> > const &aOptions, unsigned *pID)
{
  int eRet = 0;
  if (uVersion == FILTER_VERSION)
  {
    CFilter *pFilter = new CFilter;
    if (pFilter->Init(pFilterInstance, pD3DDevice, aOptions))
    {
      *pID = (unsigned)pFilter;
    }
    else
      eRet = -2;
  }
  else
    eRet = -1;

  return eRet;
}


//----------------------------------------------------------------------------
// Filter_End
// 
// Called when the filter is closed
//----------------------------------------------------------------------------
EXPORT_DLL void Filter_End(unsigned uInstance)
{
  CFilter *pFilter = (CFilter *)uInstance;
  pFilter->End();
  delete pFilter;
}


//----------------------------------------------------------------------------
// Filter_GetVarCtrls
//
// Called when the GUI wants the specs of all vars available
//----------------------------------------------------------------------------
EXPORT_DLL TCtrlVar *Filter_GetVarCtrls(unsigned uID)
{
  return ((CFilter *)uID)->GetVarCtrls();
}


//----------------------------------------------------------------------------
// Filter_SetVar
//
// Called when the GUI sets a var value
//----------------------------------------------------------------------------
EXPORT_DLL void Filter_SetVar(unsigned uID, int iVar, void *pData)
{
  ((CFilter *)uID)->SetVar(iVar, pData);
}


//----------------------------------------------------------------------------
// Filter_GetVar
//
// Called when the GUI wants a var value
//----------------------------------------------------------------------------
EXPORT_DLL void *Filter_GetVar(unsigned uID, int iVar)
{
  return ((CFilter *)uID)->GetVar(iVar);
}



//----------------------------------------------------------------------------
// Filter_SetTime
//
// Called to set the time on each frame
//----------------------------------------------------------------------------
EXPORT_DLL void Filter_SetTime(unsigned uID, float fTime)
{
  ((CFilter *)uID)->SetTime(fTime);
}


//----------------------------------------------------------------------------
// Filter_Apply
//
// Apply the filter to the filter source texture or the target texture
//----------------------------------------------------------------------------
EXPORT_DLL bool Filter_Apply(unsigned uID, TFilterTgt *pFilterTgt)
{
  return ((CFilter *)uID)->Apply(pFilterTgt);
}


//----------------------------------------------------------------------------
// UpdateBlocks
//
//----------------------------------------------------------------------------
void CFilter::UpdateBlocks()
{
  m_iWidth  = (int)((MAX_WIDTH -MIN_WIDTH ) * m_fSize) + MIN_WIDTH;
  m_iHeight = (int)((MAX_HEIGHT-MIN_HEIGHT) * m_fSize) + MIN_HEIGHT;
  m_fInvW   = 1.f / m_iWidth;
  m_fInvH   = 1.f / m_iHeight;
  m_iTotal  = m_iWidth * m_iHeight;
  for (int i = 0; i < m_iTotal; i++)
  {
    m_aBlocks[i] = (float)(rand() * (1.f / RAND_MAX));
  }
}

//---------------------------------------------------------------------------//
// FillQuad
//
//---------------------------------------------------------------------------//
void CFilter::FillQuad(TVertex_XYZRHW *pVertices, int iX, int iY, int sW, int sH)
{
  TVector2 v0, v1, uv0, uv1;

  float fPosIni = m_aBlocks[iX + iY*m_iWidth] * 0.7f;
  float fPosEnd = fPosIni + 0.3f;
  float fSize;
  if (m_fIntens > fPosIni)
    fSize = (m_fIntens <= fPosEnd) ? (fPosEnd - m_fIntens) / 0.3f : 0.f;
  else
    fSize = 1.f;

  unsigned uColor = 0xFFFFFFFF;
  if (m_bAlpha)
    uColor = (int(fSize * 255.f) << 24) | 0xFFFFFF;

  float fCX = iX * m_fBlockW + m_fHalfBlockW;
  float fCY = iY * m_fBlockH + m_fHalfBlockH;
  switch (m_iType)
  {
    case STRETCH_WH:
      v0.x = fCX - fSize * m_fHalfBlockW;
      v0.y = fCY - fSize * m_fHalfBlockH;
      v1.x = fCX + fSize * m_fHalfBlockW;
      v1.y = fCY + fSize * m_fHalfBlockH;
      break;
    case STRETCH_W:
      v0.x = fCX - fSize * m_fHalfBlockW;
      v0.y = fCY - 1.f   * m_fHalfBlockH;
      v1.x = fCX + fSize * m_fHalfBlockW;
      v1.y = fCY + 1.f   * m_fHalfBlockH;
      break;
    case STRETCH_H:
      v0.x = fCX - 1.f   * m_fHalfBlockW;
      v0.y = fCY - fSize * m_fHalfBlockH;
      v1.x = fCX + 1.f   * m_fHalfBlockW;
      v1.y = fCY + fSize * m_fHalfBlockH;
      break;
  }
  uv0.x= (fCX - m_fHalfBlockW) / (float)sW;
  uv0.y= (fCY - m_fHalfBlockH) / (float)sH;
  uv1.x= (fCX + m_fHalfBlockW) / (float)sW;
  uv1.y= (fCY + m_fHalfBlockH) / (float)sH;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZRHW(pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, uv0.x, uv0.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZRHW(pVertices, v1.x-0.5f, v0.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, uv1.x, uv0.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZRHW(pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, uv1.x, uv1.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZRHW(pVertices, v0.x-0.5f, v0.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, uv0.x, uv0.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZRHW(pVertices, v1.x-0.5f, v1.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, uv1.x, uv1.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, uColor);
  HCV_SET_XYZRHW(pVertices, v0.x-0.5f, v1.y-0.5f, 0.0f, 1.0f);
  HCV_SET_UV0   (pVertices, uv0.x, uv1.y);
  pVertices++;
}

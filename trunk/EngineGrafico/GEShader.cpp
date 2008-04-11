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
#include "Xml.h"
#include <d3dx9.h>

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CShader::Init(const string &sFile)
{
  BorraVars();

  // Nombre
  GetCurrentDirectory(MAX_PATH, m_Dir);
  m_Filename = sFile;
  m_bOk = Restore();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// LoadShaderNeon
//
// Usage: Carga un shader en formato propio (Xml)
//---------------------------------------------------------------------------//
bool CShader::LoadShaderNeon(const string &sFile)
{
  const char *pValue;
  unsigned    iValue;
  float       fValue;

  // XML Document
  TiXmlDocument XMLDoc(sFile.c_str());
  if (!XMLDoc.LoadFile() || !XMLDoc.FirstChild("shader"))
    return false;

  m_iType = NEON;

  TiXmlNode    *pNode   = XMLDoc.FirstChild("shader");
  TiXmlElement *pParent = pNode->ToElement();

  LPDIRECT3DDEVICE9 pD3D = g_DisplayDevice.GetD3DDevice();

  // Crear el StateBlock
  pD3D->BeginStateBlock();

  // ZEnable
  pValue = SafeString(pParent, "ZENABLE");
  if (pValue)
  {
    if (!Stricmp(pValue, "FALSE"))
      pD3D->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
    else if (!Stricmp(pValue, "USEW"))
      pD3D->SetRenderState(D3DRS_ZENABLE, D3DZB_USEW);
    else
      pD3D->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
  }

  // ZFunc
  pValue = SafeString(pParent, "ZFUNC");
  if (pValue)
  {
    if (!Stricmp(pValue, "NEVER"))
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);
    else if (!Stricmp(pValue, "LESS"))
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
    else if (!Stricmp(pValue, "EQUAL"))
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_EQUAL);
    else if (!Stricmp(pValue, "LESSEQUAL"))
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
    else if (!Stricmp(pValue, "GREATER"))
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
    else if (!Stricmp(pValue, "NOTEQUAL"))
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_NOTEQUAL);
    else if (!Stricmp(pValue, "GREATEREQUAL"))
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATEREQUAL);
    else
      pD3D->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
  }

  // TextureFactor
  pValue = SafeString(pParent, "TEXTUREFACTOR", "FFFFFFFF");
  iValue = strtoul(pValue, 0, 16);
  pD3D->SetRenderState(D3DRS_TEXTUREFACTOR, iValue);

  // BlendFactor
  pValue = SafeString(pParent, "BLENDFACTOR", "FFFFFFFF");
  iValue = strtoul(pValue, 0, 16);
  pD3D->SetRenderState(D3DRS_BLENDFACTOR, iValue);

  // Fill Mode
  pValue = SafeString(pParent, "FILLMODE", "SOLID");
  if (!Stricmp(pValue, "POINT"))
    pD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_POINT);
  else if (!Stricmp(pValue, "WIREFRAME"))
    pD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
  else
    pD3D->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

  // Cull Mode
  pValue = SafeString(pParent, "CULLMODE", "NONE");
  if (!Stricmp(pValue, "CCW"))
    pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
  else if (!Stricmp(pValue, "CW"))
    pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
  else
    pD3D->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

  // Shade Mode
  pValue = SafeString(pParent, "SHADEMODE");
  if (pValue)
  {
    if (!Stricmp(pValue, "FLAT"))
      pD3D->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
    else
      pD3D->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
  }

  // AlphaBlendEnable
  pValue = SafeString(pParent, "ALPHABLENDENABLE");
  if (pValue)
  {
    if (!Stricmp(pValue, "TRUE"))
      pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    else
      pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  }

  // AlphaTestEnable
  pValue = SafeString(pParent, "ALPHATESTENABLE");
  if (pValue)
  {
    if (!Stricmp(pValue, "TRUE"))
      pD3D->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    else
      pD3D->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
  }

  // AlphaRef
  pD3D->SetRenderState(D3DRS_ALPHAREF, atoi(SafeString(pParent, "ALPHAREF", "0")));

  // AlphaFunc
  pValue = SafeString(pParent, "ALPHAFUNC");
  if (pValue)
  {
    if (!Stricmp(pValue, "NEVER"))
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NEVER);
    else if (!Stricmp(pValue, "LESS"))
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESS);
    else if (!Stricmp(pValue, "EQUAL"))
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_EQUAL);
    else if (!Stricmp(pValue, "LESSEQUAL"))
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESSEQUAL);
    else if (!Stricmp(pValue, "GREATER"))
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
    else if (!Stricmp(pValue, "NOTEQUAL"))
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_NOTEQUAL);
    else if (!Stricmp(pValue, "GREATEREQUAL"))
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    else
      pD3D->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
  }
  
  // SourceBlend
  pValue = SafeString(pParent, "SRCBLEND");
  if (pValue)
  {
    if (!Stricmp(pValue, "ZERO"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
    else if (!Stricmp(pValue, "SRCCOLOR"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
    else if (!Stricmp(pValue, "INVSRCCOLOR"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
    else if (!Stricmp(pValue, "SRCALPHA"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    else if (!Stricmp(pValue, "INVSRCALPHA"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCALPHA);
    else if (!Stricmp(pValue, "DESTALPHA"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTALPHA);
    else if (!Stricmp(pValue, "INVDESTALPHA"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTALPHA);
    else if (!Stricmp(pValue, "DESTCOLOR"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
    else if (!Stricmp(pValue, "INVDESTCOLOR"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
    else if (!Stricmp(pValue, "SRCALPHASAT"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHASAT);
    else if (!Stricmp(pValue, "BOTHSRCALPHA"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHSRCALPHA);
    else if (!Stricmp(pValue, "BOTHINVSRCALPHA"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BOTHINVSRCALPHA);
    else if (!Stricmp(pValue, "BLENDFACTOR "))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
    else if (!Stricmp(pValue, "INVBLENDFACTOR"))
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVBLENDFACTOR);
    else
      pD3D->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
  }

  // DestBlend
  pValue = SafeString(pParent, "DESTBLEND");
  if (pValue)
  {
    if (!Stricmp(pValue, "ZERO"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
    else if (!Stricmp(pValue, "SRCCOLOR"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);
    else if (!Stricmp(pValue, "INVSRCCOLOR"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCCOLOR);
    else if (!Stricmp(pValue, "SRCALPHA"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
    else if (!Stricmp(pValue, "INVSRCALPHA"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    else if (!Stricmp(pValue, "DESTALPHA"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTALPHA);
    else if (!Stricmp(pValue, "INVDESTALPHA"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTALPHA);
    else if (!Stricmp(pValue, "DESTCOLOR"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_DESTCOLOR);
    else if (!Stricmp(pValue, "INVDESTCOLOR"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVDESTCOLOR);
    else if (!Stricmp(pValue, "SRCALPHASAT"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
    else if (!Stricmp(pValue, "BOTHSRCALPHA"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHSRCALPHA);
    else if (!Stricmp(pValue, "BOTHINVSRCALPHA"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BOTHINVSRCALPHA);
    else if (!Stricmp(pValue, "BLENDFACTOR "))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_BLENDFACTOR);
    else if (!Stricmp(pValue, "INVBLENDFACTOR"))
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
    else
      pD3D->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
  }

  // BlendOp
  pValue = SafeString(pParent, "BLENDOP");
  if (pValue)
  {
    if (!Stricmp(pValue, "SUBTRACT"))
      pD3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
    else if (!Stricmp(pValue, "REVSUBTRACT"))
      pD3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
    else if (!Stricmp(pValue, "MIN"))
      pD3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MIN);
    else if (!Stricmp(pValue, "MAX"))
      pD3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_MAX);
    else
      pD3D->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
  }
  
  // PointSpriteEnable
  pValue = SafeString(pParent, "POINTSPRITEENABLE");
  if (pValue)
  {
    if (!Stricmp(pValue, "TRUE"))
      pD3D->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
    else
      pD3D->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
  }

  // PointScaleEnable
  pValue = SafeString(pParent, "POINTSCALEENABLE");
  if (pValue)
  {
    if (!Stricmp(pValue, "TRUE"))
      pD3D->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
    else
      pD3D->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
  }

  // PointSize
  pValue = SafeString(pParent, "POINTSIZE");
  if (pValue)
  {
    fValue = (float)strtofloat(pValue);
    pD3D->SetRenderState(D3DRS_POINTSIZE, *((DWORD*)&fValue));
  }

  // PointSizeMin
  pValue = SafeString(pParent, "POINTSIZE_MIN");
  if (pValue)
  {
    fValue = (float)strtofloat(pValue);
    pD3D->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&fValue));
  }

  // PointScaleA
  pValue = SafeString(pParent, "POINTSCALE_A");
  if (pValue)
  {
    fValue = (float)strtofloat(pValue);
    pD3D->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*)&fValue));
  }

  // PointScaleB
  pValue = SafeString(pParent, "POINTSCALE_B");
  if (pValue)
  {
    fValue = (float)strtofloat(pValue);
    pD3D->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&fValue));
  }

  // PointScaleC
  pValue = SafeString(pParent, "POINTSCALE_C");
  if (pValue)
  {
    fValue = (float)strtofloat(pValue);
    pD3D->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&fValue));
  }

  // Stages
  bool bAnyStage = false;
  for (int i = 0; i < 4; i++)
  {
    char pBuffer[50];
    sprintf_s(pBuffer, 50, "STAGE%d", i);
    TiXmlElement *pElem = pNode->FirstChildElement(pBuffer);
    if (pElem)
    {
      // MinFilter
      pValue = SafeString(pElem, "MINFILTER", "LINEAR");
      if (!Stricmp(pValue, "NONE"))
        pD3D->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_NONE);
      else if (!Stricmp(pValue, "POINT"))
        pD3D->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_POINT);
      else if (!Stricmp(pValue, "ANISOTROPIC"))
        pD3D->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
      else
        pD3D->SetSamplerState(i, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

      // MagFilter
      pValue = SafeString(pElem, "MAGFILTER", "LINEAR");
      if (!Stricmp(pValue, "NONE"))
        pD3D->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
      else if (!Stricmp(pValue, "POINT"))
        pD3D->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
      else if (!Stricmp(pValue, "ANISOTROPIC"))
        pD3D->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
      else
        pD3D->SetSamplerState(i, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

      // AddressU
      pValue = SafeString(pElem, "ADDRESSU", "WRAP");
      if (!Stricmp(pValue, "MIRROR"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR);
      else if (!Stricmp(pValue, "CLAMP"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
      else if (!Stricmp(pValue, "BORDER"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER);
      else if (!Stricmp(pValue, "MIRRORONCE"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRRORONCE);
      else
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);

      // AddressV
      pValue = SafeString(pElem, "ADDRESSV", "WRAP");
      if (!Stricmp(pValue, "MIRROR"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR);
      else if (!Stricmp(pValue, "CLAMP"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
      else if (!Stricmp(pValue, "BORDER"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER);
      else if (!Stricmp(pValue, "MIRRORONCE"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRRORONCE);
      else
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

      // AddressW
      pValue = SafeString(pElem, "ADDRESSW", "WRAP");
      if (!Stricmp(pValue, "MIRROR"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_MIRROR);
      else if (!Stricmp(pValue, "CLAMP"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
      else if (!Stricmp(pValue, "BORDER"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_BORDER);
      else if (!Stricmp(pValue, "MIRRORONCE"))
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_MIRRORONCE);
      else
        pD3D->SetSamplerState(i, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);

      // BorderColor
      pValue = SafeString(pElem, "BORDERCOLOR");
      if (pValue)
      {
        iValue = strtoul(pValue, 0, 16);
        pD3D->SetSamplerState(i, D3DSAMP_BORDERCOLOR, iValue);
      }

      // ColorOp
      pD3D->SetTextureStageState(i, D3DTSS_COLOROP,   GetTextureOp (SafeString(pElem, "COLOROP")));
      pD3D->SetTextureStageState(i, D3DTSS_COLORARG1, GetTextureArg(SafeString(pElem, "COLORARG1")));
      pD3D->SetTextureStageState(i, D3DTSS_COLORARG2, GetTextureArg(SafeString(pElem, "COLORARG2")));
      pD3D->SetTextureStageState(i, D3DTSS_COLORARG0, GetTextureArg(SafeString(pElem, "COLORARG0")));

      pD3D->SetTextureStageState(i, D3DTSS_ALPHAOP,   GetTextureOp (SafeString(pElem, "ALPHAOP")));
      pD3D->SetTextureStageState(i, D3DTSS_ALPHAARG1, GetTextureArg(SafeString(pElem, "ALPHAARG1")));
      pD3D->SetTextureStageState(i, D3DTSS_ALPHAARG2, GetTextureArg(SafeString(pElem, "ALPHAARG2")));
      pD3D->SetTextureStageState(i, D3DTSS_ALPHAARG0, GetTextureArg(SafeString(pElem, "ALPHAARG0")));

      // BumpEnvMat00
      pValue = SafeString(pElem, "BUMPENVMAT00");
      if (pValue)
      {
        fValue = (float)strtofloat(pValue);
        pD3D->SetTextureStageState(i, D3DTSS_BUMPENVMAT00, *((DWORD*)&fValue));
      }
      // BumpEnvMat01
      pValue = SafeString(pElem, "BUMPENVMAT01");
      if (pValue)
      {
        fValue = (float)strtofloat(pValue);
        pD3D->SetTextureStageState(i, D3DTSS_BUMPENVMAT01, *((DWORD*)&fValue));
      }
      // BumpEnvMat10
      pValue = SafeString(pElem, "BUMPENVMAT10");
      if (pValue)
      {
        fValue = (float)strtofloat(pValue);
        pD3D->SetTextureStageState(i, D3DTSS_BUMPENVMAT10, *((DWORD*)&fValue));
      }
      // BumpEnvMat11
      pValue = SafeString(pElem, "BUMPENVMAT11");
      if (pValue)
      {
        fValue = (float)strtofloat(pValue);
        pD3D->SetTextureStageState(i, D3DTSS_BUMPENVMAT11, *((DWORD*)&fValue));
      }
      
      // BumpEnvLScale
      pValue = SafeString(pElem, "BUMPENVLSCALE");
      if (pValue)
      {
        fValue = (float)strtofloat(pValue);
        pD3D->SetTextureStageState(i, D3DTSS_BUMPENVLSCALE, *((DWORD*)&fValue));
      }

      // BumpEnvLScale
      pValue = SafeString(pElem, "BUMPENVLOFFSET");
      if (pValue)
      {
        fValue = (float)strtofloat(pValue);
        pD3D->SetTextureStageState(i, D3DTSS_BUMPENVLOFFSET, *((DWORD*)&fValue));
      }

      // TexCoordIndex
      pD3D->SetTextureStageState(i, D3DTSS_TEXCOORDINDEX, atoi(SafeString(pElem, "TEXCOORDINDEX", "0")));

      bAnyStage = true;
    }
  }
  // Si no hay ninguna stage especificada, aplicamos un color op por defecto
  if (!bAnyStage)
  {
    pD3D->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pD3D->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

    pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
    pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  }

  // Fin del bloque
  pD3D->EndStateBlock(&m_pStateBlock);

  // Vertex Shader
  TiXmlElement *pElem = pNode->FirstChildElement("vertexshader");
  if (pElem)
  {
    const char *pFile     = pElem->Attribute("file");
    const char *pVersion  = pElem->Attribute("version");
    const char *pFunction = pElem->Attribute("function");
    LPD3DXBUFFER pBuffer;
    if (SUCCEEDED(D3DXCompileShaderFromFile(pFile, NULL, NULL, pFunction, pVersion, 0, &pBuffer, NULL, NULL)))
    {
      g_DisplayDevice.GetD3DDevice()->CreateVertexShader((DWORD *)pBuffer->GetBufferPointer(), &m_pVS);
      HARD_RELEASE(pBuffer);
    }
    else
      GLOG(("ERR: Can't load VertexShader from %s\n", pFile));
  }

  // Pixel Shader
  pElem = pNode->FirstChildElement("pixelshader");
  if (pElem)
  {
    const char *pFile     = pElem->Attribute("file");
    const char *pVersion  = pElem->Attribute("version");
    const char *pFunction = pElem->Attribute("function");
    LPD3DXBUFFER pBuffer;
    if (SUCCEEDED(D3DXCompileShaderFromFile(pFile, NULL, NULL, pFunction, pVersion, 0, &pBuffer, NULL, NULL)))
    {
      g_DisplayDevice.GetD3DDevice()->CreatePixelShader((DWORD *)pBuffer->GetBufferPointer(), &m_pPS);
      HARD_RELEASE(pBuffer);
    }
    else
      GLOG(("ERR: Can't load PixelShader from %s\n", pFile));
  }

  return true;
}



//---------------------------------------------------------------------------//
// LoadShaderD3D
//
// Usage: Carga un shader en formato D3DEffect
//---------------------------------------------------------------------------//
bool CShader::LoadShaderD3D(const string &sFile)
{
  // Load effects file
  if (FAILED (D3DXCreateEffectFromFile(g_DisplayDevice.GetD3DDevice(), sFile.c_str(), NULL, NULL, 0, NULL, &m_pD3DEffect, NULL)))
  {
    GLOG(("ERR: Can't load D3D Effect file %s\n", sFile.c_str()));
    return false;
  }

  m_iType = D3DEFFECT;

  // Buscar tecnicas
  m_iTechniques = 0;
  D3DXHANDLE Next    = NULL;
  D3DXHANDLE Current = NULL;
  do
  {
    m_pD3DEffect->FindNextValidTechnique(Current, &Next);
    if (Next)
    {
      D3DXTECHNIQUE_DESC Desc;
      m_pD3DEffect->GetTechniqueDesc(Next, &Desc);
      m_pTechniques[m_iTechniques] = Next;
      strncpy_s(m_pTechniquesName[m_iTechniques], Desc.Name, 32);
      Current = Next;
      m_iTechniques++;
    }
  } while ((m_iTechniques < 16) && Next);

  // Buscar el numero de parametros conocidos
  D3DXHANDLE hParameter = NULL;
  m_pParameters    = NULL;
  m_iNumParameters = 0;
  //m_iNumTexturas   = 0;
  do
  {
    hParameter = m_pD3DEffect->GetParameter(NULL, m_iNumParameters);
    if (hParameter)
      m_iNumParameters++;
  } while (hParameter);

  // Cargar los identificadores de los parametros
  if (m_iNumParameters > 0)
  {
    m_pParameters = NEW_ARRAY(TParam, m_iNumParameters);
    for (int i = 0; i < m_iNumParameters; i++)
    {
      D3DXPARAMETER_DESC Desc;
      m_pParameters[i].hParam    = m_pD3DEffect->GetParameter(NULL, i);
      m_pParameters[i].iRegister = -1;
      m_pD3DEffect->GetParameterDesc(m_pParameters[i].hParam, &Desc);
      switch (Desc.Type)
      {
        // 2D Texture
        case D3DXPT_TEXTURE2D:
        {
          // Blackpawn guarda las texturas en el FX
          D3DXHANDLE hFile = m_pD3DEffect->GetAnnotationByName(m_pParameters[i].hParam, "SasResourceAddress");
          const char *pFile;
          HRESULT hRes = m_pD3DEffect->GetString(hFile, &pFile);
          if (FAILED(hRes))
            GLOG(("ERR: Can't find SasResourceAddress on FX %s", sFile.c_str()));
        
          // 2D Texture
          LPDIRECT3DTEXTURE9 pTex;
          hRes = D3DXCreateTextureFromFile(g_DisplayDevice.GetD3DDevice(), pFile, &pTex);
          if (SUCCEEDED(hRes))
            m_pD3DEffect->SetTexture(m_pParameters[i].hParam, pTex);
          else
            GLOG(("ERR: Can't load texture %s from FX %s", pFile, sFile.c_str()));
          pTex->Release();
          break;
        }

        // CUBE Texture
        case D3DXPT_TEXTURECUBE:
        {
          // Blackpawn guarda las texturas en el FX
          D3DXHANDLE hFile = m_pD3DEffect->GetAnnotationByName(m_pParameters[i].hParam, "SasResourceAddress");
          const char *pFile;
          HRESULT hRes = m_pD3DEffect->GetString(hFile, &pFile);
          if (FAILED(hRes))
            GLOG(("ERR: Can't find SasResourceAddress on FX %s", sFile.c_str()));
        
          LPDIRECT3DCUBETEXTURE9 pTex;
          hRes = D3DXCreateCubeTextureFromFile(g_DisplayDevice.GetD3DDevice(), pFile, &pTex);
          if (SUCCEEDED(hRes))
            m_pD3DEffect->SetTexture(m_pParameters[i].hParam, pTex);
          else
            GLOG(("ERR: Can't load texture %s from FX %s", pFile, sFile.c_str()));
          pTex->Release();
          break;
        }

        // Standarrd Parameter
        default:
          m_pParameters[i].iRegister = Desc.Semantic ? CMotor3D::GetRegisterByName(Desc.Semantic) : CMotor3D::GetRegisterByName(Desc.Name);
          break;
      }
    }
    // El block state
    LPDIRECT3DDEVICE9 pD3D = g_DisplayDevice.GetD3DDevice();
    
    pD3D->BeginStateBlock();

    pD3D->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    pD3D->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
    pD3D->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 2);
    pD3D->SetTextureStageState(3, D3DTSS_TEXCOORDINDEX, 3);

    pD3D->EndStateBlock(&m_pStateBlock);
  }

  //m_pD3DEffect->SetTechnique(GetTechnique(0));

  return true;
}


//---------------------------------------------------------------------------//
// GetTextureOp
//
//---------------------------------------------------------------------------//
unsigned CShader::GetTextureOp(const char *pValue)
{
  unsigned uValue = D3DTOP_SELECTARG1;
  if (pValue)
  {
    if(!Stricmp(pValue, "DISABLE"))
      uValue = D3DTOP_DISABLE;
    else if(!Stricmp(pValue, "SELECTARG1"))
      uValue = D3DTOP_SELECTARG1;
    else if(!Stricmp(pValue, "SELECTARG2"))
      uValue = D3DTOP_SELECTARG2;
    else if(!Stricmp(pValue, "MODULATE"))
      uValue = D3DTOP_MODULATE;
    else if(!Stricmp(pValue, "MODULATE2X"))
      uValue = D3DTOP_MODULATE2X;
    else if(!Stricmp(pValue, "MODULATE4X"))
      uValue = D3DTOP_MODULATE4X;
    else if(!Stricmp(pValue, "ADD"))
      uValue = D3DTOP_ADD;
    else if(!Stricmp(pValue, "ADDSIGNED"))
      uValue = D3DTOP_ADDSIGNED;
    else if(!Stricmp(pValue, "ADDSIGNED2X"))
      uValue = D3DTOP_ADDSIGNED2X;
    else if(!Stricmp(pValue, "SUBTRACT"))
      uValue = D3DTOP_SUBTRACT;
    else if(!Stricmp(pValue, "ADDSMOOTH"))
      uValue = D3DTOP_ADDSMOOTH;
    else if(!Stricmp(pValue, "BLENDDIFFUSEALPHA"))
      uValue = D3DTOP_BLENDDIFFUSEALPHA;
    else if(!Stricmp(pValue, "BLENDTEXTUREALPHA"))
      uValue = D3DTOP_BLENDTEXTUREALPHA;
    else if(!Stricmp(pValue, "BLENDFACTORALPHA"))
      uValue = D3DTOP_BLENDFACTORALPHA;
    else if(!Stricmp(pValue, "BLENDTEXTUREALPHAPM"))
      uValue = D3DTOP_BLENDTEXTUREALPHAPM;
    else if(!Stricmp(pValue, "BLENDCURRENTALPHA"))
      uValue = D3DTOP_BLENDCURRENTALPHA;
    else if(!Stricmp(pValue, "PREMODULATE"))
      uValue = D3DTOP_PREMODULATE;
    else if(!Stricmp(pValue, "MODULATEALPHA_ADDCOLOR"))
      uValue = D3DTOP_MODULATEALPHA_ADDCOLOR;
    else if(!Stricmp(pValue, "MODULATECOLOR_ADDALPHA"))
      uValue = D3DTOP_MODULATECOLOR_ADDALPHA;
    else if(!Stricmp(pValue, "MODULATEINVALPHA_ADDCOLOR"))
      uValue = D3DTOP_MODULATEINVALPHA_ADDCOLOR;
    else if(!Stricmp(pValue, "MODULATEINVCOLOR_ADDALPHA"))
      uValue = D3DTOP_MODULATEINVCOLOR_ADDALPHA;
    else if(!Stricmp(pValue, "BUMPENVMAP"))
      uValue = D3DTOP_BUMPENVMAP;
    else if(!Stricmp(pValue, "BUMPENVMAPLUMINANCE"))
      uValue = D3DTOP_BUMPENVMAPLUMINANCE;
    else if(!Stricmp(pValue, "DOTPRODUCT3"))
      uValue = D3DTOP_DOTPRODUCT3;
    else if(!Stricmp(pValue, "MULTIPLYADD"))
      uValue = D3DTOP_MULTIPLYADD;
    else if(!Stricmp(pValue, "LERP"))
      uValue = D3DTOP_LERP;
  }
  return uValue;
}


//---------------------------------------------------------------------------//
// GetTextureArg
//
//---------------------------------------------------------------------------//
unsigned CShader::GetTextureArg(const char *pValue)
{
  unsigned uValue = D3DTA_TEXTURE;
  if (pValue)
  {
    uValue = 0;
    // Flags
    if (pValue[0] == 'I')
    {
      uValue = D3DTA_COMPLEMENT;
      pValue++;
    }
    else if (pValue[0] == 'A')
    {
      uValue = D3DTA_ALPHAREPLICATE;
      pValue++;
    }
    // Ahora el FLAG en si
    if(!Stricmp(pValue, "CONSTANT"))
      uValue|= D3DTA_CONSTANT;
    else if(!Stricmp(pValue, "CURRENT"))
      uValue|= D3DTA_CURRENT;
    else if(!Stricmp(pValue, "DIFFUSE"))
      uValue|= D3DTA_DIFFUSE;
    else if(!Stricmp(pValue, "SELECTMASK"))
      uValue|= D3DTA_SELECTMASK;
    else if(!Stricmp(pValue, "SPECULAR"))
      uValue|= D3DTA_SPECULAR;
    else if(!Stricmp(pValue, "TEMP"))
      uValue|= D3DTA_TEMP;
    else if(!Stricmp(pValue, "TEXTURE"))
      uValue|= D3DTA_TEXTURE;
    else if(!Stricmp(pValue, "TFACTOR"))
      uValue|= D3DTA_TFACTOR;
  }
  return uValue;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CShader::End()
{
  if (IsOk())
  {
    LiberaVars();
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// BorraVars
//
//---------------------------------------------------------------------------//
void CShader::BorraVars()
{
  m_pStateBlock = NULL;
  m_pVS         = NULL;
  m_pPS         = NULL;
  m_pD3DEffect  = NULL;
  m_pParameters = NULL;
  m_Filename    = "";
  m_iCount      = 0;
  m_iNumParameters = 0;
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CShader::LiberaVars()
{
  Release();
}


//---------------------------------------------------------------------------//
// Release
//
//---------------------------------------------------------------------------//
void CShader::Release()
{
  switch (m_iType)
  {
    case NEON:
      HARD_RELEASE (m_pStateBlock);
      HARD_RELEASE (m_pVS);
      HARD_RELEASE (m_pPS);
      break;
    case D3DEFFECT:
      HARD_RELEASE (m_pStateBlock);
      HARD_RELEASE (m_pD3DEffect);
      DISPOSE_ARRAY(m_pParameters);
      break;
  }
}


//---------------------------------------------------------------------------//
// Restore
//
//---------------------------------------------------------------------------//
bool CShader::Restore()
{
  PushDir(m_Dir);
  bool bRes = LoadShaderNeon(m_Filename) || LoadShaderD3D(m_Filename);
  PopDir();
  return bRes;
}


//---------------------------------------------------------------------------//
// BeginDraw
//
//---------------------------------------------------------------------------//
unsigned CShader::BeginDraw(unsigned uTech)
{
  int i;
  unsigned uPasses = 0;
  if (IsOk())
  {
    switch (m_iType)
    {
      case NEON:
        uPasses = 1;
        break;
      case D3DEFFECT:
        // Set technique
        m_pD3DEffect->SetTechnique(GetTechnique(uTech));
        // Set Parameters for this effect
        for (i = 0; i < m_iNumParameters; i++)
        {
          if (m_pParameters[i].iRegister >= 0)
            m_pD3DEffect->SetValue(m_pParameters[i].hParam, CMotor3D::GetRegisterPtr(m_pParameters[i].iRegister), D3DX_DEFAULT);
        }
        m_pD3DEffect->Begin(&uPasses, NULL);
        break;
      default:
        break;
    }
  }
  return uPasses;
}


//---------------------------------------------------------------------------//
// EndDraw
//
//---------------------------------------------------------------------------//
void CShader::EndDraw()
{
  if (IsOk())
  {
    switch (m_iType)
    {
      case NEON:
        break;
      case D3DEFFECT:
        m_pD3DEffect->End();
        break;
    }
  }
}


//---------------------------------------------------------------------------//
// BeginPass
//
//---------------------------------------------------------------------------//
void CShader::BeginPass(unsigned uPass)
{
  if (IsOk())
  {
    switch (m_iType)
    {
      case NEON:
        m_pStateBlock->Apply();
        g_DisplayDevice.GetD3DDevice()->SetVertexShader(m_pVS);
        g_DisplayDevice.GetD3DDevice()->SetPixelShader (m_pPS);
        break;
      case D3DEFFECT:
        m_pStateBlock->Apply();
        m_pD3DEffect->BeginPass(uPass);
        break;
    }
  }
}


//---------------------------------------------------------------------------//
// EndPass
//
//---------------------------------------------------------------------------//
void CShader::EndPass()
{
  if (IsOk())
  {
    switch (m_iType)
    {
      case NEON:
        break;
      case D3DEFFECT:
        m_pD3DEffect->EndPass();
        break;
    }
  }
}


//---------------------------------------------------------------------------//
// FixedPipeline
//
//---------------------------------------------------------------------------//
bool CShader::FixedPipeline() const
{
  bool bRes = true;
  if (IsOk())
  {
    switch (m_iType)
    {
      case NEON:
        bRes = (m_pVS == NULL);
        break;
      case D3DEFFECT:
      {
        D3DXHANDLE hPass = m_pD3DEffect->GetPass(m_pD3DEffect->GetTechnique(0), 0);
        D3DXPASS_DESC Desc;
        m_pD3DEffect->GetPassDesc(hPass, &Desc);
        bRes = (Desc.pVertexShaderFunction == NULL);
      }
    }
  }
  return (bRes);
}

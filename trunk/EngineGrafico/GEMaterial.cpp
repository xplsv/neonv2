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

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CMaterial::Init(const string &sFile)
{
  BorraVars();
  
  m_bOk      = true;
  m_Filename = sFile;

  size_t iLen = sFile.length();
  if (sFile[iLen-2] == 'f' && sFile[iLen-1] == 'x')
    m_bOk = LoadMaterialFX(sFile);
  else if (sFile[iLen-3] == 'n' && sFile[iLen-2] == 's' && sFile[iLen-1] == 'h')
    m_bOk = LoadMaterialNeon(sFile);
  else if (sFile[iLen-3] == 'x' && sFile[iLen-2] == 'm' && sFile[iLen-1] == 'l')
    m_bOk = LoadMaterialNeon(sFile);
  else
    m_bOk = LoadMaterialTex(sFile);

  if (!IsOk())
  {
    GLOG(("Can't load material %s\n", sFile.c_str()));
    LiberaVars();
  }

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CMaterial::End()
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
void CMaterial::BorraVars()
{
  m_bMaterial    = false;
  m_iCount       = 0;
  m_iNumTexturas = 0;
  m_iShader      = -1;
  for (int i = 0; i < MAX_TEXTURES; i++)
    m_pTexturas[i] = -1;
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CMaterial::LiberaVars()
{
  // Texturas
  for (int i = 0; i < m_iNumTexturas; i++)
    g_pGestorMateriales->RemoveTextura(m_pTexturas[i]);
  // Shaders
  g_pGestorMateriales->RemoveShader(m_iShader);
}


//---------------------------------------------------------------------------//
// LoadMaterialNeon
//
//---------------------------------------------------------------------------//
bool CMaterial::LoadMaterialNeon(const string &sFile)
{
  bool bRes = false;
  // XML Document
  TiXmlDocument XMLDoc(sFile.c_str());
  if (XMLDoc.LoadFile() && XMLDoc.FirstChild("material"))
  {
    // Material completo
    bRes        = true;
    m_bMaterial = true;

    TiXmlNode *pNode = XMLDoc.FirstChild("material");

    // NumTexturas
    if (GetNumSameChilds(pNode, "texture") > MAX_TEXTURES)
      GLOG(("WAR: Material %s specified more than %d textures\n", sFile.c_str(), MAX_TEXTURES));
    // Texturas
    TiXmlNode *pIter = NULL;
    int i = 0;
    do
    {
      pIter = pNode->IterateChildren("texture", pIter);
      if (pIter)
      {
        const char *pValue = pIter->ToElement()->Attribute("file");
        if (pValue && pValue[0])
        {
          // Añadir al gestor materiales
          m_pTexturas[i] = g_pGestorMateriales->AddTextura(pValue, 0);
          if (m_pTexturas[i] < 0)
            GLOG(("ERR: Can't load texture %s on Material %s\n", pValue, sFile.c_str()));
        }
        else
          GLOG(("ERR: Texture file not specified on Material %s\n", sFile.c_str()));
        m_iNumTexturas++;
      }
      i++;
    } while (pIter && i < MAX_TEXTURES);

    // Shader
    pIter = pNode->FirstChild("shader");
    if (pIter)
    {
      const char *pValue = pIter->ToElement()->Attribute("file");
      if (pValue && pValue[0])
      {
        m_iShader = g_pGestorMateriales->AddShader(pValue);
        if (m_iShader  < 0)
          GLOG(("ERR: Can't load Shader %s on Material %s\n", pValue, sFile.c_str()));
      }
      else
        GLOG(("ERR: Shader file not specified on Material %s\n", sFile.c_str()));
    }
  }
  return bRes;
}


//---------------------------------------------------------------------------//
// LoadMaterialFX
//
//---------------------------------------------------------------------------//
bool CMaterial::LoadMaterialFX(const string &sFile)
{
    // Shader
    m_iShader = g_pGestorMateriales->AddShader(sFile);
    if (m_iShader >= 0)
    {
      return true;
    }
    else
    {
      GLOG(("ERR: Can't load shader %s\n", sFile.c_str()));
      return false;
    }
}


//---------------------------------------------------------------------------//
// LoadMaterialTex
//
//---------------------------------------------------------------------------//
bool CMaterial::LoadMaterialTex(const string &sFile)
{
    m_iNumTexturas = 1;
    m_pTexturas[0] = g_pGestorMateriales->AddTextura(sFile);
    return (m_pTexturas[0] >= 0);
}


//---------------------------------------------------------------------------//
// BeginDraw
//
//---------------------------------------------------------------------------//
unsigned CMaterial::BeginDraw(unsigned uTech)
{
  unsigned uPasses = 1;
  D3DDEVICE *pD3D = g_DisplayDevice.GetD3DDevice();

  // Texturas
  g_pGestorMateriales->SetTextura(NULL, 0);
  for (int i = 0; i < m_iNumTexturas; i++)
  {
    if (m_pTexturas[i] >= 0)
      g_pGestorMateriales->SetTextura(m_pTexturas[i], i);
    else
      g_pGestorMateriales->SetTextura(NULL, i);
  }
  // Shader
  if (m_iShader >= 0)
    uPasses = g_pGestorMateriales->GetShader(m_iShader)->BeginDraw(uTech);
  return uPasses;
}


//---------------------------------------------------------------------------//
// EndDraw
//
//---------------------------------------------------------------------------//
void CMaterial::EndDraw()
{
  // Shader
  if (m_iShader >= 0)
    g_pGestorMateriales->GetShader(m_iShader)->EndDraw();
}


//---------------------------------------------------------------------------//
// BeginPass
//
//---------------------------------------------------------------------------//
void CMaterial::BeginPass(unsigned uPass)
{
  // Shader
  if (m_iShader >= 0)
    g_pGestorMateriales->GetShader(m_iShader)->BeginPass(uPass);
  else
  {
    D3DDEVICE *pD3D = g_DisplayDevice.GetD3DDevice();
    pD3D->SetRenderState(D3DRS_FILLMODE,         D3DFILL_SOLID);
    pD3D->SetRenderState(D3DRS_CULLMODE,         D3DCULL_NONE);
    pD3D->SetRenderState(D3DRS_CLIPPING,         TRUE);
    pD3D->SetRenderState(D3DRS_ZENABLE,          D3DZB_TRUE);
    pD3D->SetRenderState(D3DRS_ZWRITEENABLE,     D3DZB_TRUE);
    pD3D->SetRenderState(D3DRS_ZFUNC,            D3DCMP_LESSEQUAL);
    pD3D->SetRenderState(D3DRS_BLENDOP,          D3DBLENDOP_ADD);
    pD3D->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pD3D->SetRenderState(D3DRS_ALPHATESTENABLE,  TRUE);
    pD3D->SetRenderState(D3DRS_SRCBLEND,         D3DBLEND_SRCALPHA);
		pD3D->SetRenderState(D3DRS_DESTBLEND,        D3DBLEND_INVSRCALPHA);
    pD3D->SetRenderState(D3DRS_ALPHAFUNC,        D3DCMP_GREATEREQUAL);
    pD3D->SetRenderState(D3DRS_ALPHAREF,         (DWORD)8);

    pD3D->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pD3D->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

    pD3D->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
    pD3D->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pD3D->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

    pD3D->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_DISABLE);
    pD3D->SetTextureStageState(1, D3DTSS_ALPHAOP,   D3DTOP_DISABLE);

    pD3D->SetVertexShader(NULL);
    pD3D->SetPixelShader (NULL);  
  }
}


//---------------------------------------------------------------------------//
// EndPass
//
//---------------------------------------------------------------------------//
void CMaterial::EndPass()
{
  // Shader
  if (m_iShader >= 0)
    g_pGestorMateriales->GetShader(m_iShader)->EndPass();
}


//---------------------------------------------------------------------------//
// FixedPipeline
//
//---------------------------------------------------------------------------//
bool CMaterial::FixedPipeline() const
{
  bool bRes = true;
  // Shader
  if (m_iShader >= 0)
    bRes = g_pGestorMateriales->GetShader(m_iShader)->FixedPipeline();
  return bRes;
}

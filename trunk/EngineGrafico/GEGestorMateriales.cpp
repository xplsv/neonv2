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

#include <Base.h>
#include "GEEngineGrafico.h"

//#define CACHE_RESOURCES 

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CGestorMateriales::Init()
{
  m_bOk = true;

  m_iIDNextMaterial = 0;
  m_iIDNextTextura  = 0;
  m_iIDNextShader   = 0;
  m_ListaMateriales.Init();
  m_ListaTexturas.Init();
  m_ListaShaders.Init();

  // Default materials
  //PushDir("resources");
  //m_iIDFlatMaterial = AddMaterial("flat.nmt");
  //m_iIDWireMaterial = AddMaterial("wire.nmt");
  //PopDir("..");

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CGestorMateriales::End()
{
  if (IsOk())
  {
    // Default materials
    RemoveMaterial(m_iIDFlatMaterial);
    RemoveMaterial(m_iIDWireMaterial);
    // Liberar todo
    m_ListaMateriales.VaciaConDispose();
    m_ListaTexturas.VaciaConDispose();
    m_ListaShaders.VaciaConDispose();
    m_ListaMateriales.End();
    m_ListaTexturas.End();
    m_ListaShaders.End();
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// Release
//
//---------------------------------------------------------------------------//
void CGestorMateriales::Release()
{
  // Shaders
  CListaIter<CShader *> IterShader(m_ListaShaders);
  while (!IterShader.EsFinal())
  {
    CShader *pShader = IterShader;
    pShader->Release();
    IterShader++;
  }
  IterShader.End();

  // Texturas
  CListaIter<CTextura *> IterTex(m_ListaTexturas);
  while (!IterTex.EsFinal())
  {
    CTextura *pTex = IterTex;
    pTex->Release();
    IterTex++;
  }
  IterTex.End();
}


//---------------------------------------------------------------------------//
// Restore
//
//---------------------------------------------------------------------------//
void CGestorMateriales::Restore()
{
  // Restore
  CListaIter<CShader *> IterShader(m_ListaShaders);
  while (!IterShader.EsFinal())
  {
    CShader *pShader = IterShader;
    pShader->Restore();
    IterShader++;
  }
  IterShader.End();

  // Texturas
  CListaIter<CTextura *> IterTex(m_ListaTexturas);
  while (!IterTex.EsFinal())
  {
    CTextura *pTex = IterTex;
    pTex->Restore();
    IterTex++;
  }
  IterTex.End();
}


//---------------------------------------------------------------------------//
// AddMaterial
//
//---------------------------------------------------------------------------//
int CGestorMateriales::AddMaterial(const string &sFilename)
{
  CMaterial *pMat = NULL;
  int iMat = -1;

#ifdef CACHE_RESOURCES
  CListaIter<CMaterial *> Iter(m_ListaMateriales);
  while (!Iter.EsFinal())
  {
    pMat = Iter;
    if (pMat->GetFilename() && !Stricmp(pszFilename, pMat->GetFilename()))
    {
      iMat = pMat->GetID();
      break;
    }
    Iter++;
  }
  Iter.End();
#endif

  // Si no estaba cargada, la cargamos
  if (iMat == -1)
  {
    pMat = NEW CMaterial;
    if (pMat->Init(sFilename))
    {
      iMat = m_iIDNextMaterial++;
      pMat->SetID(iMat);
      m_ListaMateriales.InsertaUltimo(pMat);
    }
    else
      DISPOSE(pMat);
  }

  // Añadir una referencia (sea nueva o no)
  if (pMat)
    pMat->AddRef();

  return iMat;
}


//---------------------------------------------------------------------------//
// RemoveMaterial
//
//---------------------------------------------------------------------------//
void CGestorMateriales::RemoveMaterial(int iMat)
{
  CMaterial *pMat = GetMaterial(iMat);
  if (pMat && !pMat->SubRef())
  {
    m_ListaMateriales.Saca(pMat);
    DISPOSE(pMat);
  }
}


//---------------------------------------------------------------------------//
// GetMaterial
//
//---------------------------------------------------------------------------//
CMaterial *CGestorMateriales::GetMaterial(int iMat)
{
  if (iMat >= 0)
  {
    CListaIter<CMaterial *> Iter(m_ListaMateriales);
    while (!Iter.EsFinal())
    {
      CMaterial *pMat = Iter;
      if (pMat->GetID() == iMat)
        return pMat;
      Iter++;
    }
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// AddTextura
//
//---------------------------------------------------------------------------//
int CGestorMateriales::AddTextura(const string &sFilename, unsigned uFlags)
{
  CTextura *pTex = NULL;
  int iTex = -1;

#ifdef CACHE_RESOURCES
  CListaIter<CTextura *> Iter(m_ListaTexturas);
  while (!Iter.EsFinal())
  {
    pTex = Iter;
    if (pTex->GetFilename() && !Stricmp(pszFilename, pTex->GetFilename()))
    {
      iTex = pTex->GetID();
      break;
    }
    Iter++;
  }
  Iter.End();
#endif

  // Si no estaba cargada, la cargamos
  if (iTex == -1)
  {
    pTex = NEW CTextura;
    if (pTex->Init(sFilename, uFlags))
    {
      iTex = m_iIDNextTextura++;
      pTex->SetID(iTex);
      m_ListaTexturas.InsertaUltimo(pTex);
    }
    else
      DISPOSE (pTex);
  }

  // Añadir una referencia (sea nueva o no)
  if (pTex)
    pTex->AddRef();

  return iTex;
}


//---------------------------------------------------------------------------//
// AddTextura
//
//---------------------------------------------------------------------------//
int CGestorMateriales::AddTextura(int iWidth, int iHeight, int iFormat, bool bDynamic, bool bRenderTarget, bool bDepthStencil, unsigned uFlags)
{
  CTextura *pTex = NEW CTextura;
  int iTex = -1;
  if (pTex->Init(iWidth, iHeight, iFormat, bDynamic, bRenderTarget, bDepthStencil, uFlags))
  {
    iTex = m_iIDNextTextura++;
    pTex->SetID(iTex);
    m_ListaTexturas.InsertaUltimo(pTex);
  }

  if (pTex)
    pTex->AddRef();

  return iTex;
}


//---------------------------------------------------------------------------//
// RemoveTextura
//
//---------------------------------------------------------------------------//
void CGestorMateriales::RemoveTextura(int iTex)
{
  CTextura *pTex = GetTextura(iTex);
  if (pTex && !pTex->SubRef())
  {
    m_ListaTexturas.Saca(pTex);
    DISPOSE(pTex);
  }
}


//---------------------------------------------------------------------------//
// GetTextura
//
//---------------------------------------------------------------------------//
CTextura *CGestorMateriales::GetTextura(int iTex)
{
  if (iTex >= 0)
  {
    CListaIter<CTextura *> Iter(m_ListaTexturas);
    while (!Iter.EsFinal())
    {
      CTextura *pTex = Iter;
      if (pTex->GetID() == iTex)
        return pTex;
      Iter++;
    }
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// AddShader
//
//---------------------------------------------------------------------------//
int CGestorMateriales::AddShader(const string &sFilename)
{
  CShader *pShader = NULL;
  int iShader = -1;

#ifdef CACHE_RESOURCES
  CListaIter<CShader *> Iter(m_ListaShaders);
  while (!Iter.EsFinal())
  {
    pShader = Iter;
    if (pShader->GetFilename() && !Stricmp(pszFilename, pShader->GetFilename()))
    {
      iShader = pShader->GetID();
      break;
    }
    Iter++;
  }
  Iter.End();
#endif

  // Si no estaba cargado, la cargamos
  if (iShader == -1)
  {
    pShader = NEW CShader;
    if (pShader->Init(sFilename))
    {
      iShader = m_iIDNextShader++;
      pShader->SetID(iShader);
      m_ListaShaders.InsertaUltimo(pShader);
    }
    else
      DISPOSE(pShader);
  }

  // Añadir una referencia (sea nueva o no)
  if (pShader)
    pShader->AddRef();

  return iShader;
}


//---------------------------------------------------------------------------//
// RemoveShader
//
//---------------------------------------------------------------------------//
void CGestorMateriales::RemoveShader(int iTex)
{
  CShader *pShader = GetShader(iTex);
  if (pShader && !pShader->SubRef())
  {
    m_ListaShaders.Saca(pShader);
    DISPOSE(pShader);
  }
}


//---------------------------------------------------------------------------//
// GetShader
//
//---------------------------------------------------------------------------//
CShader *CGestorMateriales::GetShader(int iShader)
{
  if (iShader >= 0)
  {
    CListaIter<CShader *> Iter(m_ListaShaders);
    while (!Iter.EsFinal())
    {
      CShader *pShader = Iter;
      if (pShader->GetID() == iShader)
        return pShader;
      Iter++;
    }
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// SetTextura
//
//---------------------------------------------------------------------------//
void CGestorMateriales::SetTextura(int iTex, int iStage)
{
  if (iTex >= 0)
  {
    CTextura *pTex = GetTextura(iTex);
    if (pTex)
      pTex->Set(iStage);
  }
  else
    g_DisplayDevice.GetD3DDevice()->SetTexture(iStage, NULL);
}

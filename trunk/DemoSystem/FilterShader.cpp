//---------------------------------------------------------------------------//
// File: FilterShader.cpp
//
//---------------------------------------------------------------------------//

#include "DemoSystemPch.h"
#include "FilterShader.h"
#include "CtrlVar.h"
#include "CustomVar.h"


//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFilterShader::GetVarCtrls()
{
  return m_pCtrlVars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFilterShader::SetVar(int iVar, void *pData)
{
  m_pVars[iVar].SetValue(pData);
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFilterShader::GetVar(int iVar)
{
  return m_pVars[iVar].GetValue();
}


//---------------------------------------------------------------------------//
// InitCustomVar
//
//---------------------------------------------------------------------------//
bool CFilterShader::InitCtrlVar(int iVar, const string &sVar)
{
  int iReg  = sVar.find(",", 0);       if (iReg   <= 0) return false;
  int iName = sVar.find(",", iReg +1); if (iName  <= 0) return false;
  int iValue= sVar.find(",", iName+1); if (iValue <= 0) return false;
  int j = 0;

  switch (GetCtrlVarType(sVar.substr(0,iReg)))
  {
    // CHECK BOX
    case TCtrlVar::CHECK_BOX:
      m_pCtrlVars[iVar].iType       = TCtrlVar::CHECK_BOX;
      m_pCtrlVars[iVar].iNum        = iVar;
      m_pCtrlVars[iVar].bLinkable   = true;
      m_pCtrlVars[iVar].iNumOptions = 0;
      strcpy(m_pCtrlVars[iVar].pszName, sVar.substr(iName+1,iValue-iName-1).c_str());
      m_pVars[iVar].Init(strtobool(sVar.substr(iValue+1,sVar.size()-iValue-1)), CMotor3D::GetRegisterByName(sVar.substr(iReg+1,iName-iReg-1)));
      break;
    // COMBO_BOX
    case TCtrlVar::COMBO_BOX:
      m_pCtrlVars[iVar].iType       = TCtrlVar::COMBO_BOX;
      m_pCtrlVars[iVar].iNum        = iVar;
      m_pCtrlVars[iVar].bLinkable   = true;
      m_pCtrlVars[iVar].iNumOptions = 0;
      strcpy(m_pCtrlVars[iVar].pszName, sVar.substr(iName+1,iValue-iName-1).c_str());
      do 
      {
        int iValueAnt = iValue;
        iValue = sVar.find(",", iValue+1);
        m_pCtrlVars[iVar].iNumOptions++;
        if (iValue <= 0)
            strcpy(m_pCtrlVars[iVar].pszOptions[j], sVar.substr(iValueAnt+1,sVar.size()-iValueAnt-1).c_str());
        else
            strcpy(m_pCtrlVars[iVar].pszOptions[j], sVar.substr(iValueAnt+1,iValue-iValueAnt-1).c_str());
        j++;
      } while (iValue > 0);
      m_pVars[iVar].Init(0, CMotor3D::GetRegisterByName(sVar.substr(iReg+1,iName-iReg-1)));
      break;
    // SLIDER: CustomVar=Float
    case TCtrlVar::SLIDER:
      m_pCtrlVars[iVar].iType       = TCtrlVar::SLIDER;
      m_pCtrlVars[iVar].iNum        = iVar;
      m_pCtrlVars[iVar].bLinkable   = true;
      m_pCtrlVars[iVar].iNumOptions = 0;
      strcpy(m_pCtrlVars[iVar].pszName, sVar.substr(iName+1,iValue-iName-1).c_str());
      m_pVars[iVar].Init(strtofloat(sVar.substr(iValue+1,sVar.size()-iValue-1)), CMotor3D::GetRegisterByName(sVar.substr(iReg+1,iName-iReg-1)));
      break;
  }
  return true;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFilterShader::Init(COptions &aOptions)
{
  const string &sFile = aOptions.Option("shader");
  if (sFile != "")
  {
    m_iShader = g_pGestorMateriales->AddShader(sFile);
    if (m_iShader >= 0)
    {
      // Load textures
      for (int i = 0; i < 3; i++)
      {
        m_pTexturas[i] = -1;
        string sTexture;
        switch (i)
        {
          case 0: sTexture = aOptions.Option("texture0"); break;
          case 1: sTexture = aOptions.Option("texture1"); break;
          case 2: sTexture = aOptions.Option("texture2"); break;
        }
        if (sTexture != "")
        {
          // Añadir al gestor materiales
          m_pTexturas[i] = g_pGestorMateriales->AddTextura(sTexture, 0);
          if (m_pTexturas[i] < 0)
            GLOG(("ERR: Can't load texture %s from Filter file %s\n", sTexture.c_str(), sFile.c_str()));
        }
      }

      // Load vars
      m_iNumVars = 0;
      for (vector< pair<string,string> >::const_iterator lIter = aOptions.Options().begin(); lIter != aOptions.Options().end(); lIter++)
      {
        if ((*lIter).first == "var")
          m_iNumVars++;
      }
      m_pCtrlVars = NEW_ARRAY(TCtrlVar,   m_iNumVars+2);
      m_pVars     = NEW_ARRAY(TCustomVar, m_iNumVars+1);

      // Var 1 = techniques
      CShader *pShader = g_pGestorMateriales->GetShader(m_iShader);
      m_pCtrlVars[0].iType = TCtrlVar::COMBO_BOX;
      m_pCtrlVars[0].iNum  = 0;
      m_pCtrlVars[0].iNumOptions = pShader->GetNumTechniques();
      m_pCtrlVars[0].bLinkable   = false;
      strcpy(m_pCtrlVars[0].pszName, "Technique");
      for (int i = 0; i < m_pCtrlVars[0].iNumOptions; i++)
        strcpy(m_pCtrlVars[0].pszOptions[i], pShader->GetTechniqueName(i));
      m_pVars[0].Init(0, -1);

      // Custom Vars
      int iVar = 1;
      for (vector< pair<string,string> >::const_iterator lIter = aOptions.Options().begin(); lIter != aOptions.Options().end(); lIter++)
      {
        if ((*lIter).first == "var")
        {
          InitCtrlVar(iVar, (*lIter).second);
          iVar++;
        }
      }
      // Ultimo elemento
      m_pCtrlVars[m_iNumVars+1].iType=TCtrlVar::INVALID;
      m_Ok = true;
    }
  }
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFilterShader::End()
{
  if (IsOk())
  {
    for (int i = 0; i < 3; i++)
      g_pGestorMateriales->RemoveTextura(m_pTexturas[i]);
    g_pGestorMateriales->RemoveShader(m_iShader);
    DISPOSE_ARRAY(m_pCtrlVars)
    DISPOSE_ARRAY(m_pVars);
    CFilter::End();
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFilterShader::Run(float fTime)
{
  CFilter::Run(fTime);
}


//---------------------------------------------------------------------------//
// Apply
//
//---------------------------------------------------------------------------//
bool CFilterShader::Apply(CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt)
{
  // CustomVars para el shader
  unsigned uTechnique = m_pVars[0].iValue;
  for (int i = 1; i <= m_iNumVars; i++)
  {
    if (m_pVars[i].iRegister)
    {
      float f = m_pVars[i].GetFloat();
      CMotor3D::SetRegisterData(m_pVars[i].iRegister, &f);
    }
  }

  // Draw shader to target
  pDD->ApplyBasicShader    ();
  pDD->SetRenderTarget     (iTexTgt);
  pDD->SetBilinearFiltering(0, false);
  pDD->SetBilinearFiltering(1, false);
  pDD->SetBilinearFiltering(2, false);
  pDD->SetBilinearFiltering(3, false);

  CShader *pShader = g_pGestorMateriales->GetShader(m_iShader);
  unsigned uNumPasses = pShader->BeginDraw(uTechnique);
  for (unsigned uPass = 0; uPass < uNumPasses; uPass++)
  {
    g_pGestorMateriales->SetTextura(pRenderTgt->iTexture, 0);
    for (int i = 0; i < 3; i++)
      g_pGestorMateriales->SetTextura(m_pTexturas[i], i+1);
    pShader->BeginPass(uPass);
    Draw(pDD, TVector2(0,0), TVector2(1,1));
    pShader->EndPass();
  }
  pShader->EndDraw();

  return true;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFilterShader::Draw(CDisplayDevice *pDD, TVector2 const &uv0, TVector2 const &uv1)
{
  pDD->SetIdentity();
  // Screen aligned quad, para que asi el filtro pueda tener un VS
  TVector2 v0 = TVector2(-1, 1);
  TVector2 v1 = TVector2( 1,-1);
  TVertex_HCV_XYZ_S *pVertices = (TVertex_HCV_XYZ_S *)pDD->LockVertexBuffer(HCV_XYZ_S, 4);

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZ   (pVertices, v0.x,  v0.y, 0.0f);
  HCV_SET_UV0   (pVertices, uv0.x, uv0.y);
  HCV_SET_UV1   (pVertices, uv0.x, uv0.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZ   (pVertices, v1.x,  v0.y, 0.0f);
  HCV_SET_UV0   (pVertices, uv1.x, uv0.y);
  HCV_SET_UV1   (pVertices, uv1.x, uv0.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZ   (pVertices, v1.x,  v1.y, 0.0f);
  HCV_SET_UV0   (pVertices, uv1.x, uv1.y);
  HCV_SET_UV1   (pVertices, uv1.x, uv1.y);
  pVertices++;

  HCV_SET_COLOR (pVertices, 0xFFFFFFFF);
  HCV_SET_XYZ   (pVertices, v0.x,  v1.y, 0.0f);
  HCV_SET_UV0   (pVertices, uv0.x, uv1.y);
  HCV_SET_UV1   (pVertices, uv0.x, uv1.y);
  pVertices++;
  
  pDD->UnlockVertexBuffer(HCV_XYZ_S);
  pDD->DrawPrimitive(HCV_XYZ_S, HARD_PRIM_TRIANGLEFAN, 2);
}

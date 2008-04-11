//---------------------------------------------------------------------------//
// File: GECamara3D.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "GECamara3D.h"
#include "Ase.h"
#include "Path.h"


//---------------------------------------------------------------------------//
// LoadASE
//
//---------------------------------------------------------------------------//
bool CCamara3D::LoadASE(CAseFile *pFile, float fFrameEnd)
{
  End();
  BorraVars();

  char pTemp[256];

  // Solo aceptamos camara con target
  if (pFile->IntoKey("CAMERA_TYPE"))
  {
    sscanf(pFile->GetData(), "%s", pTemp);
    if (Stricmp(pTemp, "Target"))
    {
      GLOG(("ERR: Camera type %s not supported\n", pTemp));
      return false;
    }
  }

  // Source Pos
  if (pFile->IntoChild("NODE_TM"))
  {
    if (pFile->IntoKey("TM_POS"))
      sscanf(pFile->GetData(), "%f %f %f", &m_vSrc.x, &m_vSrc.z, &m_vSrc.y); 
    pFile->OutOfChild();
  }

  // Target Pos
  if (pFile->IntoChild("NODE_TM"))
  {
    if (pFile->IntoKey("TM_POS"))
      sscanf(pFile->GetData(), "%f %f %f", &m_vTgt.x, &m_vTgt.z, &m_vTgt.y); 
    pFile->OutOfChild();
  }
  
  // Camera Settings
  if (pFile->IntoChild("CAMERA_SETTINGS"))
  {
    if (pFile->IntoKey("CAMERA_FOV"))
      sscanf(pFile->GetData(), "%f", &m_fFov); 
    pFile->OutOfChild();
  }

  // Camera Animation
  if (pFile->IntoChild("CAMERA_ANIMATION"))
  {
    m_pPathFov = NEW CPath;
    m_pPathFov->Init(CPath::PATH_1D, pFile->CountKeys("CAMERA_SETTINGS")+1);
    while (pFile->IntoChild("CAMERA_SETTINGS"))
    {
      // Nodo Settings
      float fTime;
      if (pFile->IntoKey("TIMEVALUE"))
      {
        sscanf(pFile->GetData(), "%f", &fTime);
        if (pFile->IntoKey("CAMERA_FOV"))
        {
          float fFov;
          sscanf(pFile->GetData(), "%f", &fFov);
          m_pPathFov->AddKey(fTime, fFov);
          if (m_pPathFov->GetNumKeys() == (m_pPathFov->GetMaxKeys()-1))
          {
            if (fTime < fFrameEnd)
            {
              TPathKey1 *pIni = m_pPathFov->GetKeyValue1(0);
              m_pPathFov->AddKey(fFrameEnd, pIni->f0);
            }
          }
        }
      }
      pFile->OutOfChild();
    }
    pFile->OutOfChild();
  }

  // Source Animation
  if (pFile->IntoChild("TM_ANIMATION"))
  {
    if (pFile->IntoChild("CONTROL_POS_TRACK"))
    {
      m_pPathSrc = NEW CPath;
      m_pPathSrc->Init(CPath::PATH_3D, pFile->CountKeys("CONTROL_POS_SAMPLE")+1);
      while (pFile->IntoKey("CONTROL_POS_SAMPLE"))
      {
        // Nodo Settings
        float    fTime;
        TVector3 vPos;
        sscanf(pFile->GetData(), "%f %f %f %f", &fTime, &vPos.x, &vPos.z, &vPos.y);
        m_pPathSrc->AddKey(fTime, vPos);
        if (m_pPathSrc->GetNumKeys() == (m_pPathSrc->GetMaxKeys()-1))
        {
          if (fTime < fFrameEnd)
          {
            TPathKey3 *pIni = m_pPathSrc->GetKeyValue3(0);
            m_pPathSrc->AddKey(fFrameEnd, TVector3(pIni->f0,pIni->f1,pIni->f2));
          }
        }
      }
      pFile->OutOfChild();
    }
    pFile->OutOfChild();
  }

  // Target Animation
  if (pFile->IntoChild("TM_ANIMATION"))
  {
    if (pFile->IntoChild("CONTROL_POS_TRACK"))
    {
      m_pPathTgt = NEW CPath;
      m_pPathTgt->Init(CPath::PATH_3D, pFile->CountKeys("CONTROL_POS_SAMPLE")+1);
      while (pFile->IntoKey("CONTROL_POS_SAMPLE"))
      {
        // Nodo Settings
        float    fTime;
        TVector3 vPos;
        sscanf(pFile->GetData(), "%f %f %f %f", &fTime, &vPos.x, &vPos.z, &vPos.y);
        m_pPathTgt->AddKey(fTime, vPos);
        if (m_pPathTgt->GetNumKeys() == (m_pPathTgt->GetMaxKeys()-1))
        {
          if (fTime < fFrameEnd)
          {
            TPathKey3 *pIni = m_pPathTgt->GetKeyValue3(0);
            m_pPathTgt->AddKey(fFrameEnd, TVector3(pIni->f0,pIni->f1,pIni->f2));
          }
        }
      }
      pFile->OutOfChild();
    }
    pFile->OutOfChild();
  }
  
  m_bOk = true;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CCamara3D::End()
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
void CCamara3D::BorraVars()
{
  m_fFov       = 0.7f;
  m_vSrc.x     = m_vSrc.y = m_vSrc.z = 0.f;
  m_vTgt.x     = m_vTgt.y = m_vTgt.z = 0.f;
  m_pPathFov   = NULL;
  m_pPathSrc   = NULL;
  m_pPathTgt   = NULL;
  m_pNombre    = NULL;
  m_bFovStatic = true;
  m_bSrcStatic = true;
  m_bTgtStatic = true;
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CCamara3D::LiberaVars()
{
  DISPOSE(m_pPathFov);
  DISPOSE(m_pPathSrc);
  DISPOSE(m_pPathTgt);
  DISPOSE_ARRAY(m_pNombre);
}


//---------------------------------------------------------------------------//
// SetTime
//
//---------------------------------------------------------------------------//
void CCamara3D::SetTime(float fTime)
{
  // Runs
  if (m_pPathFov)
  {
    m_pPathFov->SetTime(fTime);
    m_pPathFov->GetKeyValue(m_fFov);
  }
  if (m_pPathSrc)
  {
    m_pPathSrc->SetTime(fTime);
    m_pPathSrc->GetKeyValue(m_vSrc);
  }
  if (m_pPathTgt)
  {
    m_pPathTgt->SetTime(fTime);
    m_pPathTgt->GetKeyValue(m_vTgt);
  }
}


//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CCamara3D::Run(float fRunTime)
{
  // Runs
  if (m_pPathFov)
  {
    m_pPathFov->Run(fRunTime);
    m_pPathFov->GetKeyValue(m_fFov);
  }
  if (m_pPathSrc)
  {
    m_pPathSrc->Run(fRunTime);
    m_pPathSrc->GetKeyValue(m_vSrc);
  }
  if (m_pPathTgt)
  {
    m_pPathTgt->Run(fRunTime);
    m_pPathTgt->GetKeyValue(m_vTgt);
  }
}

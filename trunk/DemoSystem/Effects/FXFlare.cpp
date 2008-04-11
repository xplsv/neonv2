//---------------------------------------------------------------------------//
// File: FXFlare.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "FXFlare.h"
#include "GEGestorMateriales.h"
#include "GEMaterial.h"
#include "GEMotor3D.h"
#include "Xml.h"


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXFlare::Init(TiXmlElement *pElem)
{
  CEffect::Init(pElem);

  const char *pszFile = pElem->Attribute("fxfile");
  m_iMaterial = g_pGestorMateriales->AddMaterial(pszFile);
  m_iRays = SafeInteger(pElem, "rays", MAX_RAYS);
  for (int i = 0; i < m_iRays; i++)
    m_fRandoms[i] = float(rand())/float(RAND_MAX);
  m_bLengthWave = SafeInteger(pElem, "lengthwave", 0) ? true : false;
  m_bIntensWave = SafeInteger(pElem, "intenswave", 0) ? true : false;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXFlare::End()
{
  if (IsOk())
  {
    g_pGestorMateriales->RemoveMaterial(m_iMaterial);
    CEffect::End();
  }
}


//---------------------------------------------------------------------------//
// OnReset
//
//---------------------------------------------------------------------------//
void CFXFlare::OnReset()
{
  CEffect::OnReset();
}


//---------------------------------------------------------------------------//
// OnCommand
//
//---------------------------------------------------------------------------//
void CFXFlare::OnCommand(TCommand *pComm)
{
  CEffect::OnCommand(pComm);
}
   

//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXFlare::Run(float fTime)
{
  CEffect::Run(fTime);
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXFlare::Draw(CDisplayDevice *pDD)
{
  pDD->SetRenderTarget(m_RenderTarget.iTexture);
  if (m_bClear)
    pDD->Clear(true, true, 0, 1.f);

  float fW = (float)pDD->GetViewport().w;
  float fH = (float)pDD->GetViewport().h;

  float w = 4.0f/fW;
  float h =-4.0f/fH;
  float dw=-2.0f;
  float dh= 2.0f;
    
  float fFar =2.0f;
  float fNear=0.001f;
  float Q =fFar/(fFar-fNear);

  D3DXMATRIX identity;
  D3DXMATRIX m;

  m._11=w   ; m._12=0.0f; m._13=0.0f    ; m._14=0.0f;
  m._21=0.0f; m._22=h   ; m._23=0.0f    ; m._24=0.0f;
  m._31=0.0f; m._32=0.0f; m._33=Q       ; m._34=1.0f;
  m._41=dw  ; m._42=dh  ; m._43=-Q*fNear; m._44=1.0f;
  
  D3DDEVICE *pD3D = pDD->GetD3DDevice();
  D3DXMatrixIdentity(&identity);
  pD3D->SetTransform(D3DTS_VIEW, &identity);
  pD3D->SetTransform(D3DTS_WORLD, &identity);
  pD3D->SetTransform(D3DTS_PROJECTION, &m); 

  TVertex_HCV_XYZ_F *pVertices = (TVertex_HCV_XYZ_F *)pDD->LockVertexBuffer(HCV_XYZ_F, m_iRays*3);

  pDD->ApplyBasicShader();
  // Strength
  float fStrength = 1.f;
  if (m_bLengthWave)
  {
    CMotor3D::GetRegisterData(CMotor3D::V_WAVE, &fStrength);
    fStrength = 0.3f + 1.0f * fStrength;
  }
  // Intens
  float fIntens = 0.3f;
  if (m_bIntensWave)
  {
    CMotor3D::GetRegisterData(CMotor3D::V_WAVE, &fIntens);
    fIntens = + 1.0f * fIntens;
    if (fIntens > 1.f)
      fIntens = 1.f;
  }
  unsigned uColor = HARD_COLOR_ARGB(255, (uchar)(fIntens * 0xAF), (uchar)(fIntens * 0xAF), (uchar)(fIntens * 0x8F));

  for (int i = 0; i < m_iRays; i++)
  {
    float fRayLength = m_fRandoms[i] * (240.0f + 200.0f * (float)cosf(3.0f*m_fTime+i*i)) * fStrength;
    float fRayArc    = m_fRandoms[i] * (0.2f + 0.01f * (float)sinf(m_fTime+i));
    float fRayOffset = m_fRandoms[i] * (0.1f*m_fTime + 6.23f * (float)cosf(13.0f*i));
    float fAngle     = cosf(fRayOffset);
    fRayOffset = fabsf(fmodf(fRayOffset, 3.1416f*2.0f));
    if (fRayOffset>=3.1416f && fRayOffset<2.0f*3.1416f)
    {
      fRayLength*=fabsf(fAngle);
    }

    float fx = 320.0f;
    float fy = 240.0f;
    
    HCV_SET_XYZ(pVertices, fx, fy, 1.f);
    HCV_SET_UV0(pVertices, 0.f, 1.f);
    HCV_SET_COLOR(pVertices, uColor)
    pVertices++;

    HCV_SET_XYZ(pVertices, fx+fRayLength*cosf(fRayOffset), fy+fRayLength*sinf(fRayOffset), 1.f);
    HCV_SET_UV0(pVertices, 0.f, 0.f);
    HCV_SET_COLOR(pVertices, 0)
    pVertices++;

    HCV_SET_XYZ(pVertices, fx+fRayLength*cosf(fRayOffset+fRayArc), fy+fRayLength*sinf(fRayOffset+fRayArc), 1.f);
    HCV_SET_UV0(pVertices, 0.f, 0.f);
    HCV_SET_COLOR(pVertices, 0)
    pVertices++;
  }
  pDD->UnlockVertexBuffer(HCV_XYZ_F);

  CMaterial *pMat = g_pGestorMateriales->GetMaterial(m_iMaterial);
  unsigned uPasses = pMat->BeginDraw(0);
  for (unsigned uPass = 0; uPass < uPasses; uPass++)
  {
    pMat->BeginPass(uPass);
    pDD->DrawPrimitive(HCV_XYZ_F, HARD_PRIM_TRIANGLELIST, m_iRays);
    pMat->EndPass();
  }
  pMat->EndDraw();
}

//---------------------------------------------------------------------------//
// File: FXMessage.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "FXMessage.h"
#include "GEEngineGrafico.h"
#include "GEMotor3D.h"
#include "Command.h"
#include "CtrlVar.h"
#include "Xml.h"

static TCtrlVar s_Vars[] =
{
  {TCtrlVar::EDIT_BOX, 0, "Text", false, 0, {NULL}},
  {TCtrlVar::INVALID},
};

//---------------------------------------------------------------------------//
// GetVars
//
//---------------------------------------------------------------------------//
TCtrlVar *CFXMessage::GetVars(int iScope) const
{
  return s_Vars;
}


//---------------------------------------------------------------------------//
// SetVar
//
//---------------------------------------------------------------------------//
void CFXMessage::SetVar(int iScope, int iObj, int iVar, void *pData)
{
  switch (iVar)
  {
    case 0:
      m_pMessage[0]=0;
      if (pData)
        strncpy(m_pMessage, (const char *)pData, 256);
      break;
  }
}


//---------------------------------------------------------------------------//
// GetVar
//
//---------------------------------------------------------------------------//
void *CFXMessage::GetVar(int iScope, int iObj, int iVar)
{
  switch (iVar)
  {
    case 0: return (m_pMessage);
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CFXMessage::Init(TiXmlElement *pElem)
{
  CEffect::Init(pElem);

  const char *pszFile = SafeString(pElem, "fxfile");
  m_iTexFondo = -1;
  if (pszFile && pszFile[0])
    m_iTexFondo = g_pGestorMateriales->AddTextura(pszFile);
  SetEstado     (ST_ACTIVO);
  memset        (m_pMessage, 0, MAX_MESSAGE);
  D3DXCreateFont(g_DisplayDevice.GetD3DDevice(), 24, 0, FW_BOLD, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"), &m_pFont);
  m_fPosYActivo   = g_DisplayDevice.ViewportHeight() - 100.f;
  m_fPosYInactivo = g_DisplayDevice.ViewportHeight() +  10.f;
  m_fPosY         = m_fPosYActivo;
  m_fVelY         = 0.f;
  m_fVelYDeseada  = 0.f;
  
  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CFXMessage::End()
{
  if (IsOk())
  {
    HARD_RELEASE(m_pFont);
    g_pGestorMateriales->RemoveTextura(m_iTexFondo);
    CEffect::End();
  }
}


//---------------------------------------------------------------------------//
// OnReset
//
//---------------------------------------------------------------------------//
void CFXMessage::OnReset()
{
  CEffect::OnReset();
}


//---------------------------------------------------------------------------//
// OnCommand
//
//---------------------------------------------------------------------------//
void CFXMessage::OnCommand(TCommand *pComm)
{
  CEffect::OnCommand(pComm);

  if (!Stricmp(pComm->pComm, "Down"))
    SetEstado(ST_BAJANDO);
  else if (!Stricmp(pComm->pComm, "Up"))
    SetEstado(ST_SUBIENDO);
}
   

//---------------------------------------------------------------------------//
// Run
//
//---------------------------------------------------------------------------//
void CFXMessage::Run(float fTime)
{
  CEffect::Run(fTime);

  if (m_fVelY < m_fVelYDeseada)
  {
    m_fVelY+= 0.011f * m_fFrameTime * 1000.f;
    if (m_fVelY > m_fVelYDeseada)
      m_fVelY = m_fVelYDeseada;
  }
  else if (m_fVelY > m_fVelYDeseada)
  {
    m_fVelY-= 0.011f * m_fFrameTime * 1000.f;
    if (m_fVelY < m_fVelYDeseada)
      m_fVelY = m_fVelYDeseada;
  }
  m_fPosY+=m_fVelY;
  switch (m_iEstado)
  {
    case ST_SUBIENDO:
      if (m_fPosY < m_fPosYActivo)
      {
        m_fVelYDeseada = 0.f;
        m_fVelY   = 0.f;
        m_iEstado = ST_ACTIVO;
      }
      break;
    case ST_BAJANDO:
      if (m_fPosY > m_fPosYInactivo)
      {
        m_fVelYDeseada = 0.f;
        m_fVelY   = 0.f;
        m_iEstado = ST_INACTIVO;
      }
      break;
  }
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CFXMessage::Draw(CDisplayDevice *pDD)
{
  pDD->SetRenderTarget(m_RenderTarget.iTexture);
  if (m_bFirstClear || m_bClear)
  {
    pDD->Clear(true, true, 0, 1.f);
    m_bFirstClear = false;
  }

  float fW = (float)pDD->ViewportWidth ();
  float fH = (float)pDD->ViewportHeight();

  // Create a rectangle to indicate where on the screen it should be drawn
  RECT rct;
  rct.left  = 30;
  rct.right = (int)fW - 80;
  rct.top   = (int)m_fPosY;
  rct.bottom= rct.top + 80;
  RECT rctSombra = rct;
  rctSombra.left  +=2;
  rctSombra.right +=2;
  rctSombra.top   +=2;
  rctSombra.bottom+=2;
 
  pDD->ApplyBasicShader();
  if (m_iTexFondo != -1)
  {
    D3DDEVICE *pD3D = pDD->GetD3DDevice();
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
    pD3D->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pDD->SetBilinearFiltering(0, false);
    CTextura *pTex = g_pGestorMateriales->GetTextura(m_iTexFondo);
    g_pGestorMateriales->SetTextura(m_iTexFondo, 0);
    DrawQuadUV(pDD, TVector2(rct.left-10.f, rct.top-5.f), TVector2(rct.right+10.f, rct.bottom+5.f),
                    TVector2(0.0f, 0.0f), TVector2((float)(rct.right - rct.left + 1) / pTex->GetWidth(), (float)(rct.bottom - rct.top + 1) / pTex->GetHeight()));
  }
  else
    DrawQuadFlat(pDD, TVector2(rct.left-10.f, rct.top-5.f), TVector2(rct.right+10.f, rct.bottom+5.f), HARD_COLOR_ARGB(255,255,96,64), 0.f);
  m_pFont->DrawText(NULL, m_pMessage, -1, &rctSombra, DT_WORDBREAK, HARD_COLOR_ARGB(255,0,0,0));
  m_pFont->DrawText(NULL, m_pMessage, -1, &rct, DT_WORDBREAK, HARD_COLOR_ARGB(255,255,255,255));
}


//---------------------------------------------------------------------------//
// SetEstado
//
//---------------------------------------------------------------------------//
void CFXMessage::SetEstado(int iEstado)
{
  switch (iEstado)
  {
    case ST_INACTIVO:
      m_fPosY = m_fPosYInactivo;
      m_iEstado = iEstado;
      break;
    case ST_SUBIENDO:
      if (m_iEstado == ST_INACTIVO)
      {
        m_fVelYDeseada =-5.f;
        m_fVelY        = 0.f;
        m_iEstado = iEstado;
      }
      break;
    case ST_BAJANDO:
      if (m_iEstado == ST_ACTIVO)
      {
        m_fVelYDeseada = 5.f;
        m_fVelY        = 0.f;
        m_iEstado = iEstado;
      }
      break;
    case ST_ACTIVO:
      m_fVelYDeseada = 0.f;
      m_fVelY        = 0.f;
      m_iEstado = iEstado;
      break;
  }
}

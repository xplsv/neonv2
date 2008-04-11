 //---------------------------------------------------------------------------//
// File: FXMessage.h
//
//---------------------------------------------------------------------------//

#ifndef FXMESSAGE_H
#define FXMESSAGE_H

#include "Effect.h"

class CDisplayDevice;
class TiXmlElement;

class CFXMessage : public CEffect
{
  public:

    virtual          ~CFXMessage() { End(); }

    virtual bool      Init      (TiXmlElement *pElem);
    virtual void      End       ();

    virtual void      OnReset   ();
    virtual void      OnCommand (TCommand *pComm);

    virtual void      Run       (float fTime);
    virtual void      Draw      (CDisplayDevice *pDD);

    virtual void      SetVar    (int iScope, int iObj, int iVar, void *pData);
    virtual void     *GetVar    (int iScope, int iObj, int iVar);
    virtual TCtrlVar *GetVars   (int iScope) const;

  private:

            void      SetEstado (int iEstado);

  protected:

    enum 
    {
      MAX_MESSAGE = 256,
      ST_INACTIVO = 0,
      ST_SUBIENDO,
      ST_ACTIVO,
      ST_BAJANDO,
    };
    int          m_iEstado;
    float        m_fVelY;
    float        m_fVelYDeseada;
    float        m_fPosY;
    float        m_fPosYActivo;
    float        m_fPosYInactivo;
    unsigned     m_uColor;
    char         m_pMessage[MAX_MESSAGE];
    LPD3DXFONT   m_pFont;
    int          m_iTexFondo;
};

#endif

 //---------------------------------------------------------------------------//
// File: FXFlare.h
//
//---------------------------------------------------------------------------//

#ifndef FXFLARE_H
#define FXFLARE_H

#include "Effect.h"

class CDisplayDevice;
class TiXmlElement;

class CFXFlare : public CEffect
{
  public:

    virtual         ~CFXFlare        () { End(); }

    virtual bool     Init            (TiXmlElement *pElem);
    virtual void     End             ();

    virtual void     OnReset         ();
    virtual void     OnCommand       (TCommand *pComm);

    virtual void     Run             (float fTime);
    virtual void     Draw            (CDisplayDevice *pDD);

  protected:

    enum
    {
      MAX_RAYS = 500,
    };
    int     m_iRays;
    float   m_fRandoms[MAX_RAYS];
    int     m_iMaterial;
    bool    m_bLengthWave;
    bool    m_bIntensWave;
};

#endif

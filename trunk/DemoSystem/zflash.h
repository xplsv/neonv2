//---------------------------------------------------------------------------//
// File: ZFlash.h
//
//---------------------------------------------------------------------------//

#ifndef ZFLASH__INCLUDED_
#define ZFLASH__INCLUDED_

#include <stdlib.h>
#include <atlbase.h>
#include <atlwin.h>

#include "zflash_flash.h"

class CFlash
{
  public:
    bool   Init              (int version_needed);
    void   End               ();
    void   LoadMovie         (char *movie_name);

    void   Play              ();
    void   Stop              ();
    long   GetBackgroundColor();
    bool   SetBackgroundColor(long bg);
    void   SetLoop           (bool bLoop);
    bool   GetLoop           ();
    void   SetQuality        (int iQuality);
    float  GetLength         () { return m_fLength; }

    void   GotoFrame         (long frame);
    void   DrawFrame         (HDC hdc);
    
    CLSID const &GetClsid()
    {
      static CLSID const clsid = { 0xd27cdb6e, 0xae6d, 0x11cf, { 0x96, 0xb8, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };
      return clsid;
    }
    IID const &GetIid()
    {
      static IID const iid = { 0xD27CDB6C, 0xAE6D, 0x11CF, { 0x96, 0xB8, 0x44, 0x45, 0x53, 0x54, 0x0, 0x0 } };
      return iid;
    }

  private:

    void   ReleaseInterfaces ();

  private:
    int              m_iInt;
    long             lastframe;
    float            m_fLength;

    IShockwaveFlash *iflash;
    IUnknown        *unk;
    CAxWindow       *pcw;
    IViewObjectEx   *viewobject;
};

#endif
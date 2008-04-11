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

#ifndef _VJEFFECT_H
#define _VJEFFECT_H

#include "Base.h"
#include "Lista.h"
#include "VJStructs.h"

class CEffect;
class CFilter;
class CDisplayDevice;

class CVJEffect
{
  public:
    enum EState
    {
      NORMAL,
      FADING_IN,
      FADING_OUT,
    };
    struct TVJSnapshot
    {
      enum
      {
        WIDTH  = 128,
        HEIGHT = 128,
      };
      int       iTexRender;
      int       iTexSnapshot;
      unsigned  pData[WIDTH*HEIGHT];
    };
    enum
    {
      MAX_FILTERS = 5,
    };
  public:
                      CVJEffect       () { m_Ok = false; }
                     ~CVJEffect       () { End(); }

    bool              Load            (const string &sClass, const string &sFile, int iID, TVJSnapshot *pSnapshot, void const *iTexturasRender);
    void              End             ();
    bool              IsOk            () const { return m_Ok; }

    int               GetID           () const { return m_ID;    }
    float             GetTime         () const { return m_Time;  }
    EState            GetState        () const { return m_State; }

    void              SetTime         (float fTime);
    void              Render          (CDisplayDevice *pDD);
    void              Draw            (CDisplayDevice *pDD);

    void              Attach          (float fTime);
    void              Unattach        ();
    void              FadeIn          (float fTime);
    void              FadeOut         (float fTime);
    bool              IsVisible       () const { return (m_Alpha * m_FadeAlpha) > 0; }
    float             GetFadeAlpha    () const { return m_FadeAlpha; }

    CEffect          *GetEffect       ()              { return m_Effect; }
    CFilter          *GetFilterBySlot (int iSlot)     { ASSERT(iSlot >= 0 && iSlot < MAX_FILTERS); return m_Filters[iSlot].pFilter; }
    CFilter          *GetFilterByID   (int iIDFilter);
    int               GetFilterSlot   (int iIDFilter);
    int               GetFilterID     (int iSlot)     { ASSERT(iSlot >= 0 && iSlot < MAX_FILTERS); return m_Filters[iSlot].iID; }

    // Vars
    void              SetAlpha        (float fAlpha) { m_Alpha = fAlpha; }
    float             GetAlpha        () const { return m_Alpha; }
    void              SetBlend        (int iBlend) { m_Blend = iBlend; }
    int               GetBlend        () const { return m_Blend; }
    void              SetSpeed        (float fSpeed) { m_Speed = fSpeed; }
    float             GetSpeed        () const { return m_Speed; }
    void              SetFadeLength   (float fFadeLength) { m_FadeLength = fFadeLength; }
    float             GetFadeLength   () const { return m_FadeLength; }
    void              SetResetOnAttach(bool bReset) { m_ResetOnAttach = bReset; }
    float             GetResetOnAttach() const { return m_ResetOnAttach; }
    void              SetQuality      (int iQuality) { m_Quality = iQuality; }
    int               GetQuality      () const { return m_Quality; }

    int               AddFilter       (int iSlot, const string &sFilter);
    void              RemoveFilter    (int iIDFilter);
    void              SwapFilter      (int iIDFilterSrc, int iIDFilterDst);

  private:

    void              CreaSnapshot    (TVJSnapshot *pSnapshot);

  private:
    struct TVJFilter 
    {
      int      iID;
      CFilter *pFilter;
    };
  private:
    bool              m_Ok;
    int               m_ID;
    int               m_Blend;
    int               m_Quality;
    bool              m_FadeIn;
    float             m_FadeAlpha;
    float             m_Alpha;
    float             m_Time;
    float             m_LastRealTime;
    float             m_Speed;
    float             m_FadeValue;
    float             m_FadeLength;
    float             m_FadeTime;
    int               m_TexturasRender[Q_QUALITIES][2];
    bool              m_ResetOnAttach;
    int               m_CurrentFilterID;
    EState            m_State;
    CEffect          *m_Effect;
    TVJFilter         m_Filters[MAX_FILTERS];
};

#endif

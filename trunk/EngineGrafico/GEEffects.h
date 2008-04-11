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

#ifndef GEEFFECTS_H
#define GEEFFECTS_H

#include "GEVector.h"

namespace CEffects
{
  enum
  {
    TEX_TARGET,
    TEX_GLOW_MIPMAP_0,
    TEX_GLOW_PS_0,
    TEX_GLOW_PS_1,
    TEX_MOTION_BLUR,
    TEX_BLUR_0,
    TEX_BLUR_1,
    TEX_TEMP_0,
    TEX_SPRITER,
    TEX_NUM,
  };
  enum
  {
    SH_BLURXGLOW,
    SH_BLURX,
    SH_BLURY,
    SH_HEAT1,
    SH_HEAT2,
    SH_NUM,
  };

  void  Init             ();
  void  End              ();
  void  Restore          ();

  int   GetTexture       (int iTexEnum);
  int   GetShader        (int iShader);

  void  DoRadialBlur     (CDisplayDevice *pDD, int iIDTexSrc, int iRadio2, float fIntens, TVector2 const &vPos);
  void  DoRadialBlurSteps(CDisplayDevice *pDD, int iIDTexSrc, float fRadio, float fIntens, int nSteps, TVector2 const &vPos);
  void  DoBoxBlur        (CDisplayDevice *pDD, int iIDTexSrc, float fRadio);
  void  DoHorzBlur       (CDisplayDevice *pDD, int iIDTexSrc, float fRadio);
  void  DoVertBlur       (CDisplayDevice *pDD, int iIDTexSrc, float fRadio);
  void  DoHighlights     (CDisplayDevice *pDD, int iIDTexSrc, int iRadio, float fUmbral, float fMul);
  void  DoHighlightsPS   (CDisplayDevice *pDD, int iIDTexSrc, int iRadio, float fUmbral, float fMul);
  void  DoWhiteLightsPS  (CDisplayDevice *pDD, int iIDTexSrc, int iRadio, float fUmbral, float fMul);
  void  DoGlowMipmap     (CDisplayDevice *pDD, int iIDTexSrc, float fIntens);
  void  DoGlowPS         (CDisplayDevice *pDD, int iIDTexSrc, float fIntens);
  void  DoBlur           (CDisplayDevice *pDD, int iIDTexSrc, float fIntens);
  int   DoPixelate       (CDisplayDevice *pDD, int iIDTexSrc, float fIntens, float &fU, float &fV);
  void  DoPsychoEffect   (CDisplayDevice *pDD, int iIDTexSrc, float fIntens);
  void  DoMotionBlurPass1(CDisplayDevice *pDD, int iIDTexSrc);
  void  DoMotionBlurPass2(CDisplayDevice *pDD, int iIDTexSrc, float fIntens);
  void  DoSpriter        (CDisplayDevice *pDD, int iIDTexSrc, int iIDTexTgt, int iSprite, float fResolution, float fMinSize, float fMaxSize);
};

#endif

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

#ifndef ITEM

#include "FXColor.h"
#include "FXEscena3D.h"
#include "FXImagen.h"
#include "FXScreen.h"
#include "FXAviVideo.h"

#else

ITEM("FXColor",      FXCOLOR,   CFXColor)
ITEM("FX3DScene",    FX3D,      CFXEscena3D)
ITEM("FXEscena3D",   FX3D,      CFXEscena3D)
ITEM("FXImage",      FXIMAGE,   CFXImagen)
ITEM("FXScreen",     FXSCREEN,  CFXScreen)
ITEM("FXAviVideo",   FXAVI16,   CFXAviVideo(16))
ITEM("FXAviVideo32", FXAVI32,   CFXAviVideo(32))

#endif

/*
#include "FXMpgVideo.h"
#include "FXVideo.h"
#include "FXFlashPlayer.h"
#include "FXCapture.h"
#include "FXMessage.h"

#else

#ifdef USE_MPG_VIDEO
ITEM("FXMpgVideo",   FXMPG,     CFXMpgVideo)
#endif
ITEM("FXVideo",      FXVIDEO,   CFXVideo)
ITEM("FXFlash",      FXFLASH,   CFXFlash)
ITEM("FXCapture",    FXCAPTURE, CFXCapture)
ITEM("FXMessage",    FXMESSAGE, CFXMessage)

#endif
*/

//---------------------------------------------------------------------------//
// File: EnumEffects.h
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
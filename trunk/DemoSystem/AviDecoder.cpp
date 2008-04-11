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

#include "DemoSystemPch.h"
#include "AviDecoder.h"

//---------------------------------------------------------------------------//
// InitSubsytem
//
//---------------------------------------------------------------------------//
void CAviDecoder::InitSubsystem()
{
  AVIFileInit();
}


//----------------- .shnshn ----------------------------------------------------------//
// ShutdownSubsystem
//
//---------------------------------------------------------------------------//
void CAviDecoder::EndSubsystem()
{
  AVIFileExit();
}


//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CAviDecoder::Init(const char *pszFilename, unsigned uBpp)
{
  m_pBInfoIn  = NULL;
  m_pBInfoOut = NULL;

  if (AVIStreamOpenFromFile(&m_Avi, pszFilename, streamtypeVIDEO, 0, OF_READ, NULL) != 0)
  {
    GLOG(("ERR: AVIStreamOpenFromFile failed for %s\n", pszFilename));
    return false;
  }
  AVISTREAMINFO streamInfo;
  AVIStreamInfo(m_Avi, &streamInfo, sizeof(streamInfo));  

  m_Bpp = uBpp;
  if (uBpp > 0)
  {
    AVISTREAMINFO psi;
  	AVIStreamInfo(m_Avi, &psi, sizeof(psi));						// Reads Information About The Stream Into psi
	  m_iWidth =psi.rcFrame.right-psi.rcFrame.left;					// Width Is Right Side Of Frame Minus Left
	  m_iHeight=psi.rcFrame.bottom-psi.rcFrame.top;					// Height Is Bottom Of Frame Minus Top

    BITMAPINFOHEADER	bmih;
    memset(&bmih, 0, sizeof(BITMAPINFOHEADER));
	  bmih.biSize = sizeof (BITMAPINFOHEADER);					// Size Of The BitmapInfoHeader
	  bmih.biPlanes = 1;											// Bitplanes	
	  bmih.biBitCount = m_Bpp;										// Bits Format We Want (24 Bit, 3 Bytes)
	  bmih.biWidth = m_iWidth;								// Width We Want (rX Pixels)
	  bmih.biHeight = m_iHeight;              // Height We Want (rX Pixels)
	  bmih.biCompression = BI_RGB;      // Requested Mode = RGB

    if (!(m_Frame = AVIStreamGetFrameOpen(m_Avi, &bmih)))
    {
      GLOG(("ERR: AVIStreamGetFrameOpen failed to load in %dbpp for %s\n", uBpp, pszFilename));
      if (!(m_Frame = AVIStreamGetFrameOpen(m_Avi, NULL)))
      {
        GLOG(("ERR: AVIStreamLength failed for %s\n", pszFilename));
        End();
        return false;
      }
    }
  }
  else
  {
    if (!(m_Frame = AVIStreamGetFrameOpen(m_Avi, NULL)))
    {
      GLOG(("ERR: AVIStreamLength failed for %s\n", pszFilename));
      End();
      return false;
    }
  }

  LPBITMAPINFOHEADER pBitmap = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_Frame, 0);
  m_iWidth  = pBitmap->biWidth;
  m_iHeight = pBitmap->biHeight;

  m_iLength = AVIStreamLength(m_Avi);
  if (m_iLength == -1)
  {
		GLOG(("ERR: AVIStreamLength failed for %s\n", pszFilename));
    End();
    return false;
  }

  return true;
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CAviDecoder::End()
{
  if (m_Frame)
  {
    AVIStreamGetFrameClose(m_Frame);
    m_Frame = NULL;
  }

  if (m_Avi) 
  {
    AVIStreamRelease(m_Avi);
    m_Avi = NULL;
  }
}


//---------------------------------------------------------------------------//
// TimeToFrame (devuelve el frame que hay que poner en el tiempo dado)
//
//---------------------------------------------------------------------------//
int CAviDecoder::TimeToFrame(float fTime)
{
  return AVIStreamTimeToSample(m_Avi, int(fTime*1000.0f));
}


//---------------------------------------------------------------------------//
// Length
//
//---------------------------------------------------------------------------//
float CAviDecoder::Length()
{
  int iFrames = AVIStreamLength(m_Avi);
  int iTime   = AVIStreamSampleToTime(m_Avi, iFrames);
  
  return (float(iTime)*0.001f);
}


//---------------------------------------------------------------------------//
// GetFrame
//
//---------------------------------------------------------------------------//
bool CAviDecoder::GetFrame(int iFrame, CTextura *pTextura)
{
  bool bRes = false;
  BITMAPINFOHEADER *pBitmap = (LPBITMAPINFOHEADER)AVIStreamGetFrame(m_Frame, iFrame);
  if (pBitmap)
  {
	  char *pData = (char *)pBitmap + pBitmap->biSize + pBitmap->biClrUsed * 2;	
    bRes = true;
    TSurfaceDesc Desc;
    TRect Rect(0,0,m_iWidth,m_iHeight);
    if (pTextura->Lock(&Rect, Desc))
    {
      ASSERT(pBitmap->biBitCount == m_Bpp);
      memcpy(Desc.pBits, pData, m_iWidth*m_iHeight*(m_Bpp == 32 ? 4 : 2));
      pTextura->Unlock();
    }
  }
  return bRes;
}

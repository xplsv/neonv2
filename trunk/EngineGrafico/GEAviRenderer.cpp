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

#include "Base.h"
#include "AppWindow.h"
#include "GEDisplayDevice.h"
#include "GEAviRenderer.h"
#include "GEGestorMateriales.h"
#include "GETextura.h"

#define AVIIF_KEYFRAME	0x00000010L


//---------------------------------------------------------------------------//
// 
//
//---------------------------------------------------------------------------//
CAviRenderer::CAviRenderer()
{
	m_uFrameCount = 0;
	m_bIsWorking  = false;

  m_pAviFile             = NULL;
  m_pAviStream           = NULL;
  m_pAviStreamCompressed = NULL;
  m_pAviText             = NULL;
  m_pAviOptions[0]       = &m_aviOptions;
}


//---------------------------------------------------------------------------//
// 
//
//---------------------------------------------------------------------------//
CAviRenderer::~CAviRenderer()
{
	StopRender();
}


//---------------------------------------------------------------------------//
// 
//
//---------------------------------------------------------------------------//
bool CAviRenderer::StartRender(CDisplayDevice *pDD, const char *pszFile, unsigned nFramerate)
{
	m_bIsWorking = false;

	// Check VFW version.
	WORD wVer = HIWORD(VideoForWindowsVersion());

	if(wVer < 0x010A)
	{
		return false;
	}

	// Init library
	AVIFileInit();

  // Get an image and stuff it into a bitmap.
	HRESULT hr;
  HBITMAP bmp;

	if((bmp = CaptureScreen(pDD)) == NULL)
	{
		return false;
	}

	LPBITMAPINFOHEADER lpInfoHeader = (LPBITMAPINFOHEADER)MakeDib(bmp, 32);
  DeleteObject(bmp);

	if(lpInfoHeader == NULL)
	{
		return false;
	}

	m_uWidth  = (int)lpInfoHeader->biWidth;
	m_uHeight = (int)lpInfoHeader->biHeight;

	// Open an avi file for writing
	hr = AVIFileOpen(&m_pAviFile, pszFile, OF_WRITE | OF_CREATE, NULL);
	if (hr != AVIERR_OK)
	{
		DISPOSE(lpInfoHeader);
		return false;
	}

	// Configure the stream
	memset(&m_aviStreamInfo, 0, sizeof(m_aviStreamInfo));
	m_aviStreamInfo.fccType                = streamtypeVIDEO; // stream type
	m_aviStreamInfo.fccHandler             = 0;
	m_aviStreamInfo.dwScale                = 1;
	m_aviStreamInfo.dwRate                 = nFramerate;
	m_aviStreamInfo.dwSuggestedBufferSize  = lpInfoHeader->biSizeImage;
	SetRect(&m_aviStreamInfo.rcFrame, 0, 0, (int)lpInfoHeader->biWidth, (int)lpInfoHeader->biHeight);

	// And create the stream
	hr = AVIFileCreateStream(m_pAviFile, &m_pAviStream,	&m_aviStreamInfo);
	if(hr != AVIERR_OK)
	{
		DISPOSE(lpInfoHeader);
		return false;
	}

	// Get save options (prompt dialog)
	memset(&m_aviOptions, 0, sizeof(m_aviOptions));
	if(!AVISaveOptions((HWND)g_AppWindow.GetHandle(), 0, 1, &m_pAviStream, (LPAVICOMPRESSOPTIONS FAR *) &m_pAviOptions))
	{
		DISPOSE(lpInfoHeader);
		return false;
	}

	// Create compressed stream
	hr = AVIMakeCompressedStream(&m_pAviStreamCompressed, m_pAviStream, &m_aviOptions, NULL);
	if(hr != AVIERR_OK)
	{
		DISPOSE(lpInfoHeader);
		return false;
	}

	// Set it's format
	hr = AVIStreamSetFormat(m_pAviStreamCompressed, 0, lpInfoHeader, lpInfoHeader->biSize +  lpInfoHeader->biClrUsed * sizeof(RGBQUAD));
	if(hr != AVIERR_OK)
	{
		DISPOSE(lpInfoHeader);
		return false;
	}
	m_bIsWorking = true;

	return true;
}

// --[  Method  ]---------------------------------------------------------------
//
//  - Class     : CAviRenderer
//  - prototype : bool StopRender()
//
//  - Purpose   : Stops capturing and closes the file.
//
// -----------------------------------------------------------------------------
bool CAviRenderer::StopRender()
{
	// Close/free stuff

	if(m_pAviStream)
	{
		AVIStreamClose(m_pAviStream);
		m_pAviStream = NULL;
	}

	if(m_pAviStreamCompressed)
	{
		AVIStreamClose(m_pAviStreamCompressed);
		m_pAviStreamCompressed = NULL;
	}

	if(m_pAviText)
	{
		AVIStreamClose(m_pAviText);
		m_pAviText = NULL;
	}

	if(m_pAviFile)
	{
		AVIFileClose(m_pAviFile);
		m_pAviFile = NULL;
	}

	WORD wVer = HIWORD(VideoForWindowsVersion());

	if(wVer >= 0x010A)
	{
		AVIFileExit();
	}

	m_bIsWorking = false;

	return true;
}


//---------------------------------------------------------------------------//
// SaveFrame
//
//---------------------------------------------------------------------------//
bool CAviRenderer::SaveFrame(CDisplayDevice *pDD)
{
  /*
  D3DSURFACE *pSurf = pDD->GetBackBufferSurface();
  D3DLOCKED_RECT Desc;
  pSurf->LockRect(&Desc, NULL, 0);

	HRESULT hr = AVIStreamWrite(m_pAviStreamCompressed,
						                  m_uFrameCount,
						                  1,
						                  Desc.pBits,
                              640*480*4,
						                  AVIIF_KEYFRAME,
						                  NULL,
						                  NULL);

  pSurf->UnlockRect();
  
	if(hr != AVIERR_OK)
		return false;
	m_uFrameCount++;

  /*
	if(!IsValid())
	{
		return false;
	}

  // Get an image and stuff it into a bitmap.
	HRESULT hr;
  HBITMAP bmp;

  if((bmp = CaptureScreen(pDD)) == NULL)
	{
		return false;
	}

	LPBITMAPINFOHEADER lpInfoHeader = (LPBITMAPINFOHEADER)MakeDib(bmp, 32);
  DeleteObject(bmp);

	if(lpInfoHeader == NULL)
	{
		return false;
	}

	if(m_uWidth != (unsigned)lpInfoHeader->biWidth)
	{
		DISPOSE(lpInfoHeader);
		return false;
	}

	if(m_uHeight != (unsigned)lpInfoHeader->biHeight)
	{
		DISPOSE(lpInfoHeader);
		return false;
	}

	// Save frame

	hr = AVIStreamWrite(m_pAviStreamCompressed,
						          m_uFrameCount,
						          1,
						          (LPBYTE) lpInfoHeader +
						          lpInfoHeader->biSize +
						          lpInfoHeader->biClrUsed * sizeof(RGBQUAD),
						          lpInfoHeader->biSizeImage,
						          AVIIF_KEYFRAME,
						          NULL,
						          NULL);

	if(hr != AVIERR_OK)
	{
		DISPOSE(lpInfoHeader);
		return false;
	}

	DISPOSE(lpInfoHeader);
	m_uFrameCount++;
  */

	return true;
}


//---------------------------------------------------------------------------//
// MakeDib
//
//---------------------------------------------------------------------------//
HANDLE CAviRenderer::MakeDib(HBITMAP hBitmap, UINT uBits)
{
	HANDLE              hdib;
	HDC                 hdc;
	BITMAP              bitmap;
	UINT                wLineLen;
	DWORD               dwSize;
	DWORD               wColSize;
	LPBITMAPINFOHEADER  lpbi;
	LPBYTE              lpBits;

	GetObject(hBitmap, sizeof(BITMAP), &bitmap) ;

	// DWORD align the width of the DIB.
	// Figure out the size of the colour table
	// Calculate the size of the DIB


	wLineLen = (bitmap.bmWidth * uBits + 31) / 32 * 4;
	wColSize = sizeof(RGBQUAD) * ((uBits <= 8) ? 1 << uBits : 0);
	dwSize   = sizeof(BITMAPINFOHEADER) + wColSize + (DWORD)(UINT)wLineLen * (DWORD)(UINT)bitmap.bmHeight;

	// Allocate room for a DIB and set the LPBI fields

	hdib = (HANDLE)NEW_ARRAY(char, dwSize);

	if(!hdib)
	{
		return hdib;
	}

	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib) ;

	lpbi->biSize          = sizeof(BITMAPINFOHEADER) ;
	lpbi->biWidth         = bitmap.bmWidth ;
	lpbi->biHeight        = bitmap.bmHeight ;
	lpbi->biPlanes        = 1 ;
	lpbi->biBitCount      = (WORD) uBits ;
	lpbi->biCompression   = BI_RGB ;
	lpbi->biSizeImage     = dwSize - sizeof(BITMAPINFOHEADER) - wColSize ;
	lpbi->biXPelsPerMeter = 0 ;
	lpbi->biYPelsPerMeter = 0 ;
	lpbi->biClrUsed       = (uBits <= 8) ? 1 << uBits : 0;
	lpbi->biClrImportant  = 0 ;

	// Get the bits from the bitmap and stuff them after the LPBI

	lpBits = (LPBYTE)(lpbi + 1) + wColSize ;

	hdc = CreateCompatibleDC(NULL) ;

	GetDIBits(hdc, hBitmap, 0, bitmap. bmHeight, lpBits, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	// Fix this if GetDIBits messed it up...

	lpbi->biClrUsed = (uBits <= 8) ? 1<<uBits : 0;

	DeleteDC(hdc) ;
	GlobalUnlock(hdib);

	return hdib ;
}


//---------------------------------------------------------------------------//
// CaptureScreen
//
//---------------------------------------------------------------------------//
HBITMAP CAviRenderer::CaptureScreen(CDisplayDevice *pDD)
{
  HDC hdcScreen;
  pDD->GetBackBufferSurface()->GetDC(&hdcScreen);
  HDC hdcCompatible = CreateCompatibleDC(hdcScreen); 
 
  // Create a compatible bitmap for hdcScreen. 
  HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, pDD->GetModoGrafico().iWidth, pDD->GetModoGrafico().iHeight);
  if(hbmScreen == 0)
	{
		return 0;
	}
 
  // Select the bitmaps into the compatible DC. 
  if(!SelectObject(hdcCompatible, hbmScreen))
	{
    return 0;
	}
 
  if(!BitBlt(hdcCompatible,
			       0,0, pDD->GetModoGrafico().iWidth, pDD->GetModoGrafico().iHeight,
			       hdcScreen,
			       0, 0,
			       SRCCOPY))
	{
		return 0;
	}

  pDD->GetBackBufferSurface()->ReleaseDC(hdcScreen);
  ShowWindow((HWND)g_AppWindow.GetHandle(), SW_SHOW); 
  DeleteDC(hdcCompatible);
  return(hbmScreen);
}


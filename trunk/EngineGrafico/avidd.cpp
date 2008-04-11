 // THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
// FITNESS FOR A PARTICULAR PURPOSE.
//
// Copyright © 1996  Microsoft Corporation.  All Rights Reserved.
//

/*
heavily hacked:
works in normal instead of exclusive
doesn't use "flip" or back buffer.
decompresses to 555 (can't do 565)
assumes primary is 565!
*/

#define NAME "DDExample with AVI's"
#define TITLE "Direct Draw Example for 320x240 and smaller AVI's"

// Link with WINMM.LIB and VFW32.LIB

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <ddraw.h>
#include <stdlib.h>
#include <stdarg.h>
#include <mmsystem.h>
#include <vfw.h>
#include "resource.h"

CHAR szName[MAX_PATH];

#define WM_STARTAVI WM_USER+1

class AVISample
{
public:
  AVISample()
    : lpDD(0)
    , lpDDSPrimary(0)
    , lpDDSBack(0)
    , lpDDSSource(0)
#if HAVE_PALETTE
    , lpDDPal(0)
#endif
    , pas(0)
    , lpB(0)
    , lpI(0)
    , haveBack(0)
  {
  }
  unsigned sw,sh;
  unsigned ww,wh;
  LPDIRECTDRAW            lpDD;           // DirectDraw object
  LPDIRECTDRAWSURFACE     lpDDSPrimary;   // DirectDraw primary surface
  LPDIRECTDRAWSURFACE     lpDDSBack;      // DirectDraw back surface
  LPDIRECTDRAWSURFACE     lpDDSSource;    // DirectDraw source AVI surface
  #if HAVE_PALETTE
  LPDIRECTDRAWPALETTE     lpDDPal;        // DirectDraw palette
  PALETTEENTRY            ape[256];
  #endif
  DWORD                   dwWidth, dwHeight;
  HWND                hwnd;
  MSG         msg;
  BOOL haveBack;

  // -- Multimedia/AVI globals
  PAVISTREAM pas;
//  LONG lFmtLength;
  LPBITMAPINFOHEADER lpSrcFmt;
  LPBITMAPINFOHEADER lpTarget;
  LONG lLength;
  AVISTREAMINFO si;
  LPBYTE lpB;
  LPBYTE lpI;
  LONG lIndex;
  LONG lFrames;
  HIC hic;
  OPENFILENAME ofn;
  HINSTANCE ghInst;
void Sample(LPSTR szFileName);
BOOL SampleInit (const LPBITMAPINFO);
void SampleDraw (const LPBYTE);
int SampleRun ();
void SampleStream ();
BOOL GetOpenAVIFile();
void finiObjects( void );

HRESULT restoreAll( void );
static long FAR PASCAL WindowProc( HWND hWnd, UINT message, 
                            WPARAM wParam, LPARAM lParam );
BOOL initFail( HWND hwnd );
BOOL doInit( HINSTANCE hInstance, int nCmdShow );
};

#define ERR_IMAGE_SIZE_TOO_LARGE -13
#define ERR_NO_DECOMPRESS_TO_8BIT -14


/*
 * finiObjects
 *
 * finished with all objects we use; release them
 */
void AVISample::finiObjects( void )
{
  OutputDebugString(" starting to fini objects\n");
  if( lpDD != NULL )
  {
    if( lpDDSSource != NULL )
    {
      lpDDSSource->Release();
      lpDDSSource = NULL;
    }
    if( lpDDSPrimary != NULL )
    {
      lpDDSPrimary->Release();
      lpDDSPrimary = NULL;
    }
#if HAVE_PALETTE
    if( lpDDPal != NULL )
    {
      lpDDPal->Release();
      lpDDPal = NULL;
    }
#endif
    lpDD->RestoreDisplayMode();
    lpDD->Release();
    lpDD = NULL;
  }
    OutputDebugString(" objects finied\n");
} /* finiObjects */
/*
 * restoreAll
 *
 * restore all lost objects
 */
HRESULT AVISample::restoreAll( void )
{
  HRESULT     ddrval;

  if (lpDDSPrimary)
  {
  ddrval = lpDDSPrimary->Restore();
  return ddrval;
  }
  else
  {
    return 0;
  }
} /* restoreAll */

long FAR PASCAL AVISample::WindowProc( HWND hWnd, UINT message, 
                            WPARAM wParam, LPARAM lParam )
{
  static AVISample* it=0;
  switch( message )
  {
  case WM_CREATE:
    it = (AVISample*)((CREATESTRUCT*)lParam)->lpCreateParams;
    break;
  case WM_SETCURSOR:
    SetCursor(NULL);
    return TRUE;

  case WM_STARTAVI:
    it->SampleStream();
    break;

  case WM_KEYDOWN:
    switch( wParam )
    {
    case VK_ESCAPE:
    case VK_F12:
      PostMessage(hWnd, WM_CLOSE, 0, 0);
      break;
    }
    break;

  case WM_DESTROY:
    it->finiObjects();
    PostQuitMessage( 0 );
    break;
  }

  return DefWindowProc(hWnd, message, wParam, lParam);

} /* WindowProc */

  /*
  This function is called if the initialization function fails
*/
BOOL AVISample::initFail( HWND hwnd )
{
  OutputDebugString(" initFail entered\n");
  finiObjects();
  MessageBox( hwnd, "DirectDraw Init FAILED", TITLE, MB_OK );
  if (hwnd)
    DestroyWindow( hwnd );
  return FALSE;

} /* initFail */

/*
doInit - do work required for every instance of the application:
 *                create the window, initialize data
 */
BOOL AVISample::doInit( HINSTANCE hInstance, int nCmdShow )
{
  WNDCLASS            wc;
  DDSURFACEDESC       ddsd;
  DDSCAPS             ddscaps;
  HRESULT             ddrval;

  /*
  * set up and register window class
  */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon( hInstance, IDI_APPLICATION );
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  wc.hbrBackground = 0;//(HBRUSH)GetStockObject(BLACK_BRUSH);
  wc.lpszMenuName = NAME;
  wc.lpszClassName = NAME;
  RegisterClass( &wc );

  sw = GetSystemMetrics(SM_CXSCREEN);
  sh = GetSystemMetrics(SM_CYSCREEN);
  ww = 640;
  wh = 480;
  /*
  * create a window
  */
  hwnd = CreateWindowEx(
    0,
    NAME,
    TITLE,
    WS_POPUP,
    (sw-ww)/2, (sh-wh)/2,
    ww, wh,
    NULL,
    NULL,
    ghInst = hInstance,
    this );

  if( !hwnd )
  {
    return FALSE;
  }

  ShowWindow( hwnd, nCmdShow );
  UpdateWindow( hwnd );

  if (!GetOpenAVIFile())
    return FALSE;   // Cancel pressed

  DWORD dwResult;
  dwResult = SampleRun();

  if (dwResult)
  {
    switch(dwResult)

    {   // Problem with file encountered -- shut down gracefully

    case ERR_IMAGE_SIZE_TOO_LARGE:

      OutputDebugString("image2big\n");
      LoadString(ghInst, IDS_IMAGE2BIG, szName, MAX_PATH);
      MessageBox(GetDesktopWindow(), szName, NAME, MB_ICONHAND);
      return FALSE;
      break;

    case ERR_NO_DECOMPRESS_TO_8BIT:

      OutputDebugString("cant setup\n");
      LoadString(ghInst, IDS_NODECOMPRESS, szName, MAX_PATH);
      MessageBox(GetDesktopWindow(), szName, NAME, MB_ICONHAND);
      return FALSE;
      break;
    case -15:
      OutputDebugString("code 15\n");
      return FALSE;
    }   // Problem with file encountered -- shut down gracefully
  }
  /*
  * create the main DirectDraw object
  */
  ddrval = DirectDrawCreate( NULL, &lpDD, NULL );
  if( ddrval != DD_OK )
  {
    OutputDebugString("DDCreate failed\n");
    return initFail(hwnd);
  }

  // Get exclusive mode
//  ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX );
  ddrval = lpDD->SetCooperativeLevel( hwnd, DDSCL_NORMAL );
  if( ddrval != DD_OK )
  {
    OutputDebugString("SetCooperativeLevel failed\n");
    return initFail(hwnd);
  }

  // Set the video mode to 640x480x8
  //ddrval = lpDD->SetDisplayMode( 320, 240, 8);
#ifdef FULL_COLOR
  ddrval = lpDD->SetDisplayMode( 640, 480, 32);
#else
//  ddrval = lpDD->SetDisplayMode( 640, 480, 16);
#endif

  if( ddrval != DD_OK )
  {
    OutputDebugString("setdisplaymode\n");
    return initFail(hwnd);
  }

  // Create the primary surface with 1 back buffer
  ddsd.dwSize = sizeof( ddsd );
  ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
  ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
  ddsd.dwBackBufferCount = 1;
  ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
  if( ddrval != DD_OK )
  {
    OutputDebugString("create surface");
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
    if( ddrval != DD_OK )
    {
      OutputDebugString("create surface w/o backbuffer");
      return initFail(hwnd);
    }
  }
  else
  {
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
    ddrval = lpDDSPrimary->GetAttachedSurface(&ddscaps, &lpDDSBack);
    if( ddrval != DD_OK )
    {
      OutputDebugString("Getattached");
      return initFail(hwnd);
    }
  }

#if HAVE_PALETTE
    // Create a Direct Draw Palette and associate it with the front buffer
    lpDD->CreatePalette(DDPCAPS_8BIT, ape, &lpDDPal, NULL);

    if (lpDDPal)
      lpDDSPrimary->SetPalette( lpDDPal );
#endif
    OutputDebugString("start avi posted\n");
    PostMessage(hwnd, WM_STARTAVI, 0, 0);
    return TRUE;
} /* doInit */

/*
 * WinMain - initialization, message loop
 */
int PASCAL WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                        LPSTR lpCmdLine, int nCmdShow)
{
    OSVERSIONINFO osv;
    AVISample app;

    osv.dwOSVersionInfoSize = sizeof OSVERSIONINFO;
    GetVersionEx(&osv);

    if (osv.dwMajorVersion != 4 && osv.dwMinorVersion != 0) {
      OutputDebugString("os version\n");
      MessageBox(GetDesktopWindow(), "Samples is designed for Windows 95!", NAME, MB_ICONHAND);
      return 0;
    }

    lpCmdLine = lpCmdLine;
    hPrevInstance = hPrevInstance;

    if( !app.doInit( hInstance, nCmdShow ) )
    {
      // Funnel all exits through the same code path
      PostMessage(app.hwnd, WM_KEYDOWN, VK_ESCAPE, 0);
    }

    while( GetMessage( &app.msg, NULL, 0, 0 ) )
    {
      TranslateMessage(&app.msg);
      DispatchMessage(&app.msg);
    }

    return app.msg.wParam;

} /* WinMain */

#if HAVE_PALETTE
void GetPaletteFromDIB( LPBITMAPINFO lpbmi)
{
    int                 i;  // loop index, number of colors in color table

    // get pointer to BITMAPINFO
    if (lpbmi)
    {
      // store RGB triples 
      // into palette
      for (i = 0; i < 256; i++)
      {
        ape[i].peRed = lpbmi->bmiColors[i].rgbRed;
        ape[i].peGreen = lpbmi->bmiColors[i].rgbGreen;
        ape[i].peBlue = lpbmi->bmiColors[i].rgbBlue;
        ape[i].peFlags = 0;
      }
    }
}
#endif

BOOL AVISample::SampleInit (const LPBITMAPINFO bmpFmt)

{   // SampleInit //
#if 0
  //Let's set the palette.
  GetPaletteFromDIB(bmpFmt);
  lpDDPal->SetEntries(0, 0, 256, ape);
#endif
  OutputDebugString("SampleInit\n");
  dwWidth  = bmpFmt->bmiHeader.biWidth;
  dwHeight = bmpFmt->bmiHeader.biHeight-1;

  DDSURFACEDESC       ddsd;
  HRESULT             ddrval;

  ddsd.dwSize = sizeof( ddsd );
  ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
  ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
  ddsd.dwHeight = dwHeight;
  ddsd.dwWidth = dwWidth;
  ddrval = lpDD->CreateSurface( &ddsd, &lpDDSSource, NULL );
  if( ddrval != DD_OK )
  {
    OutputDebugString("Create DDSSource failed (plain)\n");
    ddsd.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;
    ddrval = lpDD->CreateSurface(&ddsd, &lpDDSSource, NULL);
    if (ddrval != DD_OK)
    {
      OutputDebugString("Create DDSSource failed (SYSTEM)\n");
      initFail(NULL);
      return FALSE;
    }
  }
  OutputDebugString("sampleInit success\n");
  return TRUE;
}   // SampleInit //

void AVISample::SampleDraw (const LPBYTE pbBits)
{   // SampleDraw //
  HRESULT ddrval;
  DDSURFACEDESC ddsd;
  BYTE *src, *dst;
  int y;

  if (pbBits)
  {
#if 1
    RECT dr;
    SetRect(&dr, (sw-ww)/2, (sh-wh)/2, ww+(sw-ww)/2, wh+(sh-wh)/2);
    ddsd.dwSize = sizeof(ddsd);
    ddrval = lpDDSPrimary->Lock(&dr, &ddsd, DDLOCK_WAIT, NULL);
    if (ddrval == DD_OK)
    {
      // 16-BIT PIXELS direct to primary.
      src = (BYTE *)pbBits + dwWidth*2 * dwHeight;

      dst = (BYTE *)ddsd.lpSurface;
#if 1
      for (y=0; y<(int)dwHeight;y++)
      {
        WORD* s = (WORD*)src;
        WORD* d = (WORD*)dst;
        for (int x=0; x<(int)dwWidth; x++)
        {
          *d++ = *s;
          *d++ = *s++;
        }
        dst += ddsd.lPitch;
        d = (WORD*)dst;
        s = (WORD*)src;
        for (x=0; x<(int)dwWidth; x++)
        {
          *d++ = *s;
          *d++ = *s++;
        }
        dst += ddsd.lPitch;
        src -= dwWidth*2;
      }
#else
      for( y=0; y<(int)dwHeight;y++ )
      {
        memcpy(dst, src, dwWidth*2);
        dst += ddsd.lPitch;
        src -= dwWidth*2;
      }
#endif
      lpDDSPrimary->Unlock(NULL);
    }
#else
    ddsd.dwSize = sizeof(ddsd);
    ddrval = lpDDSSource->Lock(NULL, &ddsd, DDLOCK_WAIT, NULL);
    if( ddrval == DD_OK )
    {
      __try
      {
#ifdef FULL_COLOR
        src = (BYTE *)pbBits + dwWidth*4 * dwHeight;

        dst = (BYTE *)ddsd.lpSurface;
        for( y=0; y<(int)dwHeight;y++ )
        {
          memcpy(dst, src, dwWidth*4);
          dst += ddsd.lPitch;
          src -= dwWidth*4;
        }
#else
#if USE_555
        src = (BYTE *)pbBits + dwWidth*2 * dwHeight;

        dst = (BYTE *)ddsd.lpSurface;
        for( y=0; y<(int)dwHeight;y++ )
        {
          //memcpy(dst, src, dwWidth*2);
          WORD* sl = (WORD*)src;
          WORD* dl = (WORD*)dst;
          for (int x=0; x<dwWidth; x++)
          {
            *dl = *sl + (*sl & 0x7fe0);
            dl++;
            sl++;
          }
          dst += ddsd.lPitch;
          src -= dwWidth*2;
        }
#else
        src = (BYTE *)pbBits + dwWidth*2 * dwHeight;

        dst = (BYTE *)ddsd.lpSurface;
        for( y=0; y<(int)dwHeight;y++ )
        {
          memcpy(dst, src, dwWidth*2);
          dst += ddsd.lPitch;
          src -= dwWidth*2;
        }
#endif
#endif
      }
      __except(EXCEPTION_EXECUTE_HANDLER)
      {
        OutputDebugString("exception in sample draw\n");
      }
      lpDDSSource->Unlock(NULL);
    }
  }

  RECT rc;
  RECT rcSrc;
  //SetRect(&rc,0,0,320,240);
  //SetRect(&rc,0,0,dwWidth, dwHeight);
  SetRect(&rc,0,0,dwWidth*2,dwHeight*2);
  SetRect(&rcSrc,0,0,dwWidth,dwHeight);
  if (!haveBack)
  {
    lpDDSPrimary->Blt(&rc, lpDDSSource, &rcSrc, DDBLT_WAIT, NULL);
  }
  else
  {
    lpDDSBack->Blt(&rc, lpDDSSource, &rcSrc, DDBLT_WAIT, NULL);

    // Flip the surfaces
    while( 1 )
    {
      ddrval = lpDDSPrimary->Flip( NULL, 0 );
      if( ddrval == DD_OK )
      {
        break;
      }
      if( ddrval == DDERR_SURFACELOST )
      {
        ddrval = restoreAll();
        if( ddrval != DD_OK )
        {
          break;
        }
      }
      if( ddrval != DDERR_WASSTILLDRAWING )
      {
        break;
      }
    }
#endif
  }
}   // SampleDraw //

//
// Display the open dialog box to retrieve the user-selected AVI
//
BOOL AVISample::GetOpenAVIFile()
{	// GetOpenAVIFile //

  ofn.lStructSize       = sizeof(OPENFILENAME);
  ofn.hwndOwner         = GetDesktopWindow();
  ofn.lpstrFilter       = NULL;
  ofn.lpstrFilter       = "Video (*.avi)\0*.AVI\0All Files (*.*)\0*.*\0";
  ofn.lpstrCustomFilter = NULL;
  ofn.nFilterIndex      = 1;
  ofn.lpstrFile         = (LPSTR)szName;
  ofn.nMaxFile          = 128;
  ofn.lpstrInitialDir   = NULL;
  ofn.lpstrTitle        = NULL;
  ofn.lpstrFileTitle    = NULL;
  ofn.lpstrDefExt       = NULL;
  ofn.Flags             = OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST;
  return GetOpenFileName((LPOPENFILENAME)&ofn);

}	// GetOpenAVIFile //

int AVISample::SampleRun ()
{   // SampleRun //

  AVIFileInit();

  AVIStreamOpenFromFile(&pas
    , ofn.lpstrFile
    , streamtypeVIDEO
    , 0
    , OF_READ | OF_SHARE_EXCLUSIVE
    , NULL
    );
  if (pas==0)
  {
    return -15;
  }

  LONG lFmtLength;
  AVIStreamFormatSize(pas, 0, &lFmtLength);

  lpSrcFmt = (LPBITMAPINFOHEADER)calloc(1, lFmtLength);
  memset(lpSrcFmt, 0, lFmtLength);
  lpTarget = (LPBITMAPINFOHEADER)calloc(1, lFmtLength+(sizeof(RGBQUAD)*256));

  AVIStreamReadFormat(pas, 0, lpSrcFmt, &lFmtLength);

  if ((lpSrcFmt->biWidth > 320) || (lpSrcFmt->biHeight > 240))
  {   // Image dimensions outside of maximum limit

    AVIStreamRelease(pas);
    AVIFileExit();
    return ERR_IMAGE_SIZE_TOO_LARGE;

  }   // Image dimensions outside of maximum limit

  lFrames = AVIStreamLength(pas);

  memcpy(lpTarget, lpSrcFmt, lFmtLength);
#ifdef FULL_COLOR
  lpTarget->biBitCount = 32;//8;
  lpTarget->biCompression = BI_RGB;
  lpTarget->biSizeImage = lpTarget->biHeight * lpTarget->biWidth* 4;
#else
#if USE_555
  lpTarget->biBitCount = 16;
  lpTarget->biCompression = BI_RGB;
  lpTarget->biSizeImage = lpTarget->biHeight * lpTarget->biWidth* 2;
  lpTarget->biPlanes = 1;
#else
  lpTarget->biBitCount = 16;
  lpTarget->biCompression = BI_BITFIELDS;
  lpTarget->biSizeImage = lpTarget->biHeight * lpTarget->biWidth* 2;
      DWORD* masks = (DWORD*)&((BITMAPINFO*)lpTarget)->bmiColors[0];
#define RED565 0xf800
#define GRN565 0x07e0
#define BLU565 0x001f
      masks[0] = RED565;
      masks[1] = GRN565;
      masks[2] = BLU565;
      lpTarget->biCompression = BI_BITFIELDS;
      lpTarget->biPlanes = 1;
      lpTarget->biBitCount = 16;
#endif
#endif

  AVIStreamInfo(pas, &si, sizeof AVISTREAMINFO);

  lLength = (ULONG)lpTarget->biSizeImage * (ULONG)(lpSrcFmt->biBitCount >> 3);

  if (si.dwSuggestedBufferSize)
  {
    if ((LONG)si.dwSuggestedBufferSize < lLength)
      lLength = (LONG)si.dwSuggestedBufferSize;
  }

  lpB = (LPBYTE)calloc(1, lLength);
  lpI = (LPBYTE)calloc(1, lpTarget->biSizeImage);

  hic = ICDecompressOpen(ICTYPE_VIDEO, si.fccHandler, lpSrcFmt, lpTarget);

  if (!hic)
  {   // Image dimensions outside of maximum limit

    OutputDebugString("can't icdecompressopen\n");
    free( lpB);
    free( lpI);
    free(lpSrcFmt);
    free( lpTarget);
    AVIStreamRelease(pas);
    AVIFileExit();
    return ERR_NO_DECOMPRESS_TO_8BIT;

  }   // Image dimensions outside of maximum limit


  BITMAPINFOHEADER* newFmt = (BITMAPINFOHEADER*) new BYTE[lFmtLength+256*sizeof(RGBQUAD)];
  memcpy(newFmt, lpSrcFmt, lFmtLength);
  delete [] lpSrcFmt;
  lpSrcFmt = newFmt;
//  lpSrcFmt = (LPBITMAPINFOHEADER)realloc(lpSrcFmt, lFmtLength+(sizeof(RGBQUAD)*256));

  ICDecompressGetPalette(hic, lpSrcFmt, lpTarget);

  OutputDebugString("SampleRun succeeds\n");
  return 0;

}   // SampleRun //

void AVISample::SampleStream ()
{   // SampleStream //

  if (SampleInit((LPBITMAPINFO)lpTarget))
  {
    OutputDebugString("about to start decompressing\n");
    lIndex = 0;

    BOOL done=0;
    while (!done)
    {	// Extract individual frame!


      DWORD ch = AVIStreamRead(pas, lIndex, 1, lpB, lLength, NULL, NULL);
      if (ch)
      {
        OutputDebugString("read failed\n");
        break;
      }
      ch = ICDecompress(hic, 0, lpSrcFmt, lpB, lpTarget, lpI);
      if (ch != ICERR_OK)
      {
        OutputDebugString("decompress failed\n");
        break;
      }
      SampleDraw(lpI);
      if (PeekMessage(&msg, NULL, WM_KEYDOWN, WM_KEYDOWN, PM_NOREMOVE))
      {
        done=1;
        break;
      }
      lIndex++;
      if (lIndex>=lFrames) lIndex = 0;
    }	// Extract individual frame!
  }
  OutputDebugString("Done decompressing\n");

  ICClose(hic);
  AVIStreamRelease(pas);
  AVIFileExit();
  PostMessage(hwnd, WM_KEYDOWN, VK_ESCAPE, 0);

}   // SampleStream //

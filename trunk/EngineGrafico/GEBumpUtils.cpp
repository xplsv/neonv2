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
#include <d3d9.h>
#include <d3dx9.h>

#define SBYTE signed char

#define FORMAT_END_SENTINEL 0xffffffff
#define MAX_USAGE_BITS 9


static char *sg_ResourceTypeName[10]={
  "",
    "D3DRTYPE_SURFACE",         
    "D3DRTYPE_VOLUME",               
    "D3DRTYPE_TEXTURE",                
    "D3DRTYPE_VOLUMETEXTURE",        
    "D3DRTYPE_CUBETEXTURE",           
    "D3DRTYPE_VERTEXBUFFER",          
    "D3DRTYPE_INDEXBUFFER",           
};


//note these are names of bits e.g. D3DUSAGE_DEPTHSTENCIL = 1<<1,
static char *sg_UsageName[MAX_USAGE_BITS]={
  "D3DUSAGE_RENDERTARGET",
    "D3DUSAGE_DEPTHSTENCIL",
    "D3DUSAGE_LOADONCE",
    "D3DUSAGE_WRITEONLY",
    "D3DUSAGE_SOFTWAREPROCESSING",
    "D3DUSAGE_DONOTCLIP",
    "D3DUSAGE_POINTS",
    "D3DUSAGE_HOSURFACES",
    "D3DUSAGE_NPATCHES",
};


//number of each format
static int sg_FormatIndices[]={
  0,1,2,3,4,5,6,7,8,9,
    20,21,22,23,24,25,26,27,28,29,30,
    40,41,
    50,51,52,
    60,61,62,
    'UYVY','YUY2','DXT1','DXT2','DXT3','DXT4','DXT5',
    70,
    90,91,92,93,94,
    100,101,102, FORMAT_END_SENTINEL,
};


//number of bytes needed to represent a pixel
static int sg_CorrespondingFormatPixelDepth[]={
  4,1,1,1,2,2,2,3,4,4,
    3,4,4,2,2,2,2,1,1,2,2,
    2,1,
    1,1,2,
    2,4,2,
    4,4,4,4,4,4,4,
    2,
    4,4,4,4,4,
    4,2,4, FORMAT_END_SENTINEL,
};


static char *sg_CorrespondingFormatName[]={
  "D3DFMT_UNKNOWN",
    "D3DFMT_UNKNOWN_C5",           
    "D3DFMT_UNKNOWN_C5A1",        
    "D3DFMT_UNKNOWN_C8",          
    "D3DFMT_UNKNOWN_C8A8",         
    "D3DFMT_UNKNOWN_D16",          
    "D3DFMT_UNKNOWN_D15S1",       
    "D3DFMT_UNKNOWN_D24",         
    "D3DFMT_UNKNOWN_D24S8",        
    "D3DFMT_UNKNOWN_D32",          
    
    "D3DFMT_R8G8B8",           
    "D3DFMT_A8R8G8B8",            
    "D3DFMT_X8R8G8B8",             
    "D3DFMT_R5G6B5",              
    "D3DFMT_X1R5G5B5",             
    "D3DFMT_A1R5G5B5",             
    "D3DFMT_A4R4G4B4",            
    "D3DFMT_R3G3B2",              
    "D3DFMT_A8",               
    "D3DFMT_A8R3G3B2",           
    "D3DFMT_X4R4G4B4",            
    
    "D3DFMT_A8P8",                
    "D3DFMT_P8",                
    
    "D3DFMT_L8",                 
    "D3DFMT_A8L8",               
    "D3DFMT_A4L4",  
    
    "D3DFMT_V8U8",                
    "D3DFMT_L6V5U5",               
    "D3DFMT_X8L8V8U8",            
    
    "D3DFMT_UYVY",            
    "D3DFMT_YUY2",             
    "D3DFMT_DXT1",             
    "D3DFMT_DXT2",             
    "D3DFMT_DXT3",             
    "D3DFMT_DXT4",               
    "D3DFMT_DXT5", 
    
    "D3DFMT_D16",               
    
    "D3DFMT_DXV1",                
    "D3DFMT_DXV2",               
    "D3DFMT_DXV3",               
    "D3DFMT_DXV4",                
    "D3DFMT_DXV5",                
    "D3DFMT_VERTEXDATA",         
    "D3DFMT_INDEX16",              
    "D3DFMT_INDEX32",            
};


/*
given a D3DFORMAT, returns a name of the format

*/
char *GetFormatName(D3DFORMAT p_d3dfFormat)
{
  DWORD i = 0;
  while(sg_FormatIndices[i]!=FORMAT_END_SENTINEL)
  {	
    if(sg_FormatIndices[i]==p_d3dfFormat)
      return sg_CorrespondingFormatName[i];
    i++;
  }
  return NULL;
}


/*
given a d3dformat, returns the pixel depth, e.g. the number of bits in each pixel

*/
int GetFormatPixelDepth(D3DFORMAT p_d3dfFormat)
{
  DWORD i = 0;
  while(sg_FormatIndices[i]!=FORMAT_END_SENTINEL)
  {	
    if(sg_FormatIndices[i]==p_d3dfFormat)
      return sg_CorrespondingFormatPixelDepth[i];
    i++;
  }
  return 0;
}


/*
given a DWORD corresponding to a usage bit, returns a name of usage bit

*/

char *GetUsageBitName(DWORD p_dwUsage)
{
  return sg_UsageName[p_dwUsage];
}


/*
given a DWORD corresponding to a usage bit, returns a name of usage bit

*/
char *GetResourceTypeName(D3DRESOURCETYPE p_d3drtType)
{
  return sg_ResourceTypeName[p_d3drtType];
}


/*
finds a available format for a certain type of resource (texture, cubetexture, etc) given a list of Formats, and a usage

*/
D3DFORMAT FindSupportedFormat(LPDIRECT3DDEVICE9 p_pd3dDevice, D3DFORMAT *p_d3dfFormat, DWORD p_dwNumFormats, DWORD p_dwUsage, D3DRESOURCETYPE p_d3drtType)
{
  D3DDEVICE_CREATION_PARAMETERS l_d3ddcpCreationParms;
  D3DDISPLAYMODE  l_d3ddmMode;
  
  LPDIRECT3D9 l_pd3d;
  DWORD i=0;
  
  p_pd3dDevice->GetDirect3D(&l_pd3d);
  p_pd3dDevice->GetDisplayMode(0, &l_d3ddmMode);
  p_pd3dDevice->GetCreationParameters(&l_d3ddcpCreationParms);

  for(i=0; i<p_dwNumFormats; i++)
  {
    if(SUCCEEDED(l_pd3d->CheckDeviceFormat(D3DADAPTER_DEFAULT, l_d3ddcpCreationParms.DeviceType, l_d3ddmMode.Format,   
      p_dwUsage, p_d3drtType, p_d3dfFormat[i] )) )
    {
      SAFE_RELEASE( l_pd3d );
      return(p_d3dfFormat[i]);
    }
  }
  
  char str[1024];
  sprintf_s(str, 1024, "FindSupportedFormat: No available resource of type %s, and usage type: \n", GetResourceTypeName(p_d3drtType) );
  OutputDebugString(str);
  
  for(i=0; i<MAX_USAGE_BITS; i++)
  {
    char str[1024];
    if(p_dwUsage & (1<<i))
    {
      sprintf_s(str, 1024, "%s |",GetUsageBitName(i));
      OutputDebugString(str);
    }
  }
  
  OutputDebugString("\nthat supports format types :\n");
  
  for(i=0; i<p_dwNumFormats; i++)
  {
    char str[1024];
    sprintf_s(str, 1024, "%s, ", GetFormatName(p_d3dfFormat[i]));
    OutputDebugString(str);
  }
  
  OutputDebugString("\n");
  
  SAFE_RELEASE( l_pd3d );
  return D3DFMT_UNKNOWN;
}


/*
BuildUVOffsetTexture

Purpose:
Builds UV offset Bumpmaps from a texture
  
Description:
This function takes an input texture and uses the red or luminance channel as a height value.  

To generate the uv offset map, a simple first derivative filter is applied to the height field.
  
The x filter used is  
(-1 1)
and the result is stored in the u channel

The y filter used is   
	-1
	 1  
and the result is stored in the v channel


The uv offset bumpmap is stored as a texture of type D3DFMT_V8U8 or D3DFMT_X8L8V8U8
*/

LPDIRECT3DTEXTURE9 BuildUVOffsetTexture(LPDIRECT3DDEVICE9 d3dDevice, LPDIRECT3D9 d3d, LPDIRECT3DTEXTURE9 pTextureHF)
{
  HRESULT hr;
  
  D3DLOCKED_RECT d3dlrHF;
  D3DLOCKED_RECT d3dlrBump;
  
  RECT d3dr;
  
  D3DSURFACE_DESC d3dsdHF;
  
  //pointers for data in each texture (number of bytes per pixel)
  BYTE *pBitsHF;
  BYTE *pBitsBump;
  
  //scanline width in bytes for each texture (number of bytes per pixe)
  DWORD dwPitchHF;
  DWORD dwPitchBump;
  
  //pixel depth in bytes for each texture (number of bytes per pixel)
  DWORD dwDepthHF;
  DWORD dwDepthBump;
  DWORD dwOffsetHF; //which byte in a pixel to use as the "height of the pixel"
  
  
  // Pixel formats for bumpmap and pixel shader
  D3DFORMAT d3dfHF;
  D3DFORMAT d3dfBump;
  D3DFORMAT d3dfp_BumpFormatsToTry[2] = {D3DFMT_V8U8, D3DFMT_X8L8V8U8};
  
  
  // Both have the same number of bits for width and height
  DWORD dwWidth;
  DWORD dwHeight;
  
  // Texture to be created
  LPDIRECT3DTEXTURE9 pTextureBump;
  
  // Get surface description about mip level 0
  pTextureHF->GetLevelDesc(0, &d3dsdHF);
  
  dwWidth = d3dsdHF.Width;
  dwHeight= d3dsdHF.Height;
  d3dfHF  = d3dsdHF.Format;
  
  
  if(d3dfHF == D3DFMT_A8R8G8B8 || 
     d3dfHF == D3DFMT_X8R8G8B8 ||
     d3dfHF == D3DFMT_R8G8B8   ||
     d3dfHF == D3DFMT_L8 )
  {
    dwDepthHF =GetFormatPixelDepth(d3dfHF);
    dwOffsetHF=0;
  }
  else
  {
    GLOG(("ERR: Can't accept heightfield in format sent in to BuildUVOffsetTexture"));
    return NULL;
  }

  d3dr.left=0;
  d3dr.top=0;
  d3dr.right=dwWidth;
  d3dr.bottom=dwHeight;

  //try to create uv offset map of type D3DFMT_V8U8 or D3DFMT_X8L8V8U8
  d3dfBump   =FindSupportedFormat(d3dDevice, d3dfp_BumpFormatsToTry, 2, 0, D3DRTYPE_TEXTURE);
  dwDepthBump=GetFormatPixelDepth(d3dfBump);
  
  if(d3dfBump==D3DFMT_UNKNOWN)
    return NULL;
  else if(FAILED(hr=d3dDevice->CreateTexture(dwWidth, dwHeight, 1, 0, d3dfBump, D3DPOOL_MANAGED, &pTextureBump, NULL))) 
  {
    //PUDebugPrintf("Can't create uv offset bumpmap texture of size %d,%d\n",dwWidth,dwHeight );
    pTextureBump=NULL;
    return NULL;
  }

  //get pointer to data in incoming heightfield texture (uses first component as height)
  pTextureHF->LockRect( 0,  &d3dlrHF, &d3dr, NULL );
  dwPitchHF = (DWORD)d3dlrHF.Pitch;
  pBitsHF   = (BYTE*)d3dlrHF.pBits;
  
  
  //get pointer to data in destination bumpmap texture 
  pTextureBump->LockRect( 0,  &d3dlrBump, &d3dr, NULL );
  dwPitchBump = (DWORD)d3dlrBump.Pitch;
  pBitsBump   = (BYTE*)d3dlrBump.pBits;
  
  
  BYTE* pSrcPtr = pBitsHF;
  SBYTE* pDstPtr = (SBYTE*)pBitsBump; //ptr conversion because bumpmaps are signed data 
  
  
  for( DWORD y=0; y<dwHeight; y++ )
  {
    for( DWORD x=0; x<dwWidth; x++ )
    {
      float iDu;
      float iDv;
      
      iDu=(float)*(pSrcPtr+dwOffsetHF+(((x+1)%dwWidth)*dwDepthHF)+(dwPitchHF*y));
      iDu-=(float)*(pSrcPtr+dwOffsetHF+(x*dwDepthHF)+(dwPitchHF*y));
      
      iDv=(float)*(pSrcPtr+dwOffsetHF+(x*dwDepthHF)+(dwPitchHF*((y+1)%dwHeight)) );
      iDv-=(float)*(pSrcPtr+dwOffsetHF+(x*dwDepthHF)+(dwPitchHF*y));
      
      //uv offset maps expect a signed byte as the texture pixel data format 
      //-128 (10000000 in binary)  represents max negative displacement,  
      // 0   (00000000 in binary)  represents no displacement
      // 127 (01111111 in binary)  represents max positive disp
      *(pDstPtr+0+(x*dwDepthBump)) = (SBYTE)(iDu*0.5);
      *(pDstPtr+1+(x*dwDepthBump)) = (SBYTE)(-iDv*0.5);
      
    }
    pDstPtr += dwPitchBump;
    
  }

  //unlock texture
  pTextureBump->UnlockRect(0);
  pTextureHF->UnlockRect(0);

  D3DXFilterTexture(pTextureBump, NULL, 0,  D3DX_FILTER_BOX);

  return pTextureBump;
}


/*
BuildNormalTexture

Purpose:
This function builds a xyz normal map texture used for dot3 bump mapping.

Description:
The red or luminance value of each pixel of the input texture is used as a height value for each pixel


To generate the xyz normal map, a simple first derivative filter is applied to the height field.

The uv offset bumpmap is stored as a texture of type D3DFMT_V8U8 or D3DFMT_X8L8V8U8
*/
LPDIRECT3DTEXTURE9 BuildNormalTexture(LPDIRECT3DDEVICE9 d3dDevice, LPDIRECT3D9 d3d, LPDIRECT3DTEXTURE9 pTextureHF, FLOAT fBumpiness)
{
  HRESULT hr;
  
  D3DLOCKED_RECT d3dlrHF;
  D3DLOCKED_RECT d3dlrBump;
  
  RECT d3dr;
  
  D3DSURFACE_DESC d3dsdHF;
  
  //pointers for data in each texture (number of bytes per pixel)
  BYTE *pBitsHF;
  BYTE *pBitsBump;
  
  //scanline width in bytes for each texture (number of bytes per pixe)
  DWORD dwPitchHF;
  DWORD dwPitchBump;
  
  //pixel depth in bytes for each texture (number of bytes per pixel)
  DWORD dwDepthHF;
  DWORD dwDepthBump;
  
  DWORD dwOffsetHF; //which byte in a pixel to use as the "height of the pixel"

  D3DFORMAT d3dfp_BumpFormatsToTry[3] = {D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_R8G8B8};

  //pixel formats for bumpmap and pixel shader
  D3DFORMAT d3dfHF;
  D3DFORMAT d3dfBump;

  //both have the same number of bits for width and height
  DWORD dwWidth;
  DWORD dwHeight;
  
  //texture to be created
  LPDIRECT3DTEXTURE9 pTextureBump;
  
  //get surface description about mip level 0
  pTextureHF->GetLevelDesc(0,&d3dsdHF);
  
  dwWidth=d3dsdHF.Width;
  dwHeight=d3dsdHF.Height;
  
  d3dfHF=d3dsdHF.Format;

  if(d3dfHF == D3DFMT_A8R8G8B8 || 
     d3dfHF == D3DFMT_X8R8G8B8 ||
     d3dfHF == D3DFMT_R8G8B8   ||
     d3dfHF == D3DFMT_L8 )
  {
    dwDepthHF =GetFormatPixelDepth(d3dfHF);
    dwOffsetHF=0;
  }
  else
  {
    GLOG(("ERR: Can't accept heightfield in format sent in to BuildUVOffsetTexture"));
    return NULL;
  }
  
  d3dr.left=0;
  d3dr.top=0;
  d3dr.right=dwWidth;
  d3dr.bottom=dwHeight;
  
  
  // Try to create uv offset map of type D3DFMT_V8U8 or D3DFMT_X8L8V8U8
  d3dfBump   =FindSupportedFormat(d3dDevice, d3dfp_BumpFormatsToTry, 3, 0, D3DRTYPE_TEXTURE);
  dwDepthBump=GetFormatPixelDepth(d3dfBump);
  
  if(d3dfBump==D3DFMT_UNKNOWN)
    return NULL;
  else if(FAILED(hr=d3dDevice->CreateTexture(dwWidth, dwHeight, 0, 0, d3dfBump, D3DPOOL_MANAGED, &pTextureBump, NULL))) 
  {
    //		PUDebugPrintf("Can't create rgb offset bumpmap texture of size %d,%d\n", dwWidth, dwHeight );
    pTextureBump=NULL;
    return NULL;
  }

  //get pointer to data in incoming heightfield texture (uses first component as height)
  pTextureHF->LockRect( 0,  &d3dlrHF, &d3dr, NULL );
  dwPitchHF = (DWORD)d3dlrHF.Pitch;
  pBitsHF   = (BYTE*)d3dlrHF.pBits;
  
  //get pointer to data in destination bumpmap texture 
  pTextureBump->LockRect( 0,  &d3dlrBump, &d3dr, NULL );
  dwPitchBump = (DWORD)d3dlrBump.Pitch;
  pBitsBump   = (BYTE*)d3dlrBump.pBits;
  
  BYTE* pSrcPtr = pBitsHF;
  BYTE* pDstPtr = (BYTE*)pBitsBump; //ptr conversion because normal maps use unsigned data 

  for( DWORD y=0; y<dwHeight; y++ )
  {
    for( DWORD x=0; x<dwWidth; x++ )
    {
      D3DXVECTOR3 vx,vy,vz;
      
      float iDu;
      float iDv;
      
      iDu=(float)*(pSrcPtr+dwOffsetHF+(((x+1)%dwWidth)*dwDepthHF)+(dwPitchHF*y));
      iDu-=(float)*(pSrcPtr+dwOffsetHF+(x*dwDepthHF)+(dwPitchHF*y));
      
      iDv=(float)*(pSrcPtr+dwOffsetHF+(x*dwDepthHF)+(dwPitchHF*((y+1)%dwHeight)) );
      iDv-=(float)*(pSrcPtr+dwOffsetHF+(x*dwDepthHF)+(dwPitchHF*y));
      
      vx=D3DXVECTOR3(1.0f, 0.0f, iDu*fBumpiness);  //vector between current pixel and pixel to the right of it
      vy=D3DXVECTOR3(0.0f, 1.0f, -iDv*fBumpiness);  //vector between current pixel and pixel to the bottom of it
      
      D3DXVec3Cross(&vz,&vx,&vy);
      D3DXVec3Normalize(&vz,&vz);
      
      //normal maps expect an unsigned byte as the texture pixel data format 
      // 0   (00000000 in binary)  represents a -1.0 component value
      // 128 (10000000 in binary)  represents a  0.0 component value  
      // 255 (11111111 in binary)  represents a  1.0 component value
      
      //alpha channel gets copy of original image alpha
      if(d3dfBump==D3DFMT_A8R8G8B8)
      {
        *(pDstPtr+3+(x*dwDepthBump)) = *(pSrcPtr+3+((x%dwWidth)*dwDepthHF)+(dwPitchHF*y));
      }
      
      //red channel gets x component 
      *(pDstPtr+2+(x*dwDepthBump)) = (BYTE)((vz.x)*127.0f+128.0f);
      
      //green channel gets y component 
      *(pDstPtr+1+(x*dwDepthBump)) = (BYTE)((vz.y)*127.0f+128.0f);
      
      //blue channel gets z component 
      *(pDstPtr+0+(x*dwDepthBump)) = (BYTE)((vz.z)*127.0f+128.0f); 
    }
    pDstPtr += dwPitchBump;
    //pSrcPtr += dwPitchHF;
  }

  //unlock texture
  pTextureBump->UnlockRect( 0 );
  pTextureHF->UnlockRect( 0 );
  
  D3DXFilterTexture(pTextureBump, NULL, 0,  D3DX_FILTER_BOX);

  return pTextureBump;
}

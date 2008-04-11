/* ----------------------------------------------------------------------------
Copyright (c) 1999-2002, Lev Povalahev
All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:
  
    * Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, 
    this list of conditions and the following disclaimer in the documentation 
    and/or other materials provided with the distribution.
    * The name of the author may not be used to endorse or promote products 
    derived from this software without specific prior written permission.
    
      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
      ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
      WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
      IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
      INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
      BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
      OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
      WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
      ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
      THE POSSIBILITY OF SUCH DAMAGE.
------------------------------------------------------------------------------*/

#include "MGPch.h"
#include "ltga.h"
#include <fstream>

//--------------------------------------------------
// global functions
//--------------------------------------------------
static int TGAReadError = 0;

static void ReadData(std::ifstream &file, void *data, uint size)
{
  if (!file.is_open())
    return;
  uint a = file.tellg();
  a+= size;
  file.read((char *)data, size);
  if (a != file.tellg())
  {
    TGAReadError = 1;
  }
}

//--------------------------------------------------
LTGA::LTGA()
{
  m_loaded = false;
  m_width = 0;
  m_height = 0;
  m_pixelDepth = 0;
  m_alphaDepth = 0;
  m_type = itUndefined;
  m_pixels = 0;
}


//--------------------------------------------------
LTGA::LTGA(const string &sFile)
{
  m_loaded = false;
  m_width = 0;
  m_height = 0;
  m_pixelDepth = 0;
  m_alphaDepth = 0;
  m_type = itUndefined;
  m_pixels = 0;
  LoadFromFile(sFile);
}


//--------------------------------------------------
LTGA::~LTGA()
{
  Clear();
}


//--------------------------------------------------
bool LTGA::LoadFromFile(const string &sFile)
{
  if (m_loaded)
    Clear();
  m_loaded = false;
  
  std::ifstream file;
  file.open(sFile.c_str(), std::ios::binary);
  if (!file.is_open())
    return false;
  
  bool rle = false;
  bool truecolor = false;
  uint CurrentPixel = 0;
  byte ch_buf1, ch_buf2;
  byte buf1[1000];
  
  byte IDLength;
  byte IDColorMapType;
  byte IDImageType;
  
  ReadData(file, (char*)&IDLength, 1);
  ReadData(file, (char*)&IDColorMapType, 1);
  
  if (IDColorMapType == 1)
    return false;
  
  ReadData(file, (char*)&IDImageType, 1); 
  
  switch (IDImageType)
  {
  case 2:
    truecolor = true;
    break;
  case 3:
    m_type = itGreyscale;
    break;
  case 10:
    rle = true;
    truecolor = true;
    break;
  case 11:
    rle = true;
    m_type = itGreyscale;
    break;
  default:
    return false;
  }
  
  file.seekg(5, std::ios::cur);
  
  file.seekg(4, std::ios::cur);
  ReadData(file, (char*)&m_width, 2);
  ReadData(file, (char*)&m_height, 2);
  ReadData(file, (char*)&m_pixelDepth, 1);
  
  if (! ((m_pixelDepth == 8) || (m_pixelDepth ==  24) ||
    (m_pixelDepth == 16) || (m_pixelDepth == 32)))
    return false;
  
  ReadData(file, (char*)&ch_buf1, 1); 
  
  ch_buf2 = 15; //00001111;
  m_alphaDepth = ch_buf1 & ch_buf2;
  
  if (! ((m_alphaDepth == 0) || (m_alphaDepth == 8)))
    return false;
  
  if (truecolor)
  {
    m_type = itRGB;
    if (m_pixelDepth == 32)
      m_type = itRGBA;
  }
  
  if (m_type == itUndefined)
    return false;
  
  file.seekg(IDLength, std::ios::cur);
  
  m_pixels = NEW_ARRAY(byte, m_width*m_height*(m_pixelDepth>>3));
  
  if (!rle)
  {
    //ReadData(file, (char *)m_pixels, m_width*m_height*(m_pixelDepth>>3));
    for (int i = m_height; i > 0; i--)
    {
      ReadData(file, m_pixels + (i-1)*(m_pixelDepth>>3)*m_width, m_width*(m_pixelDepth>>3));
    }
  }
  else
  {
    while (CurrentPixel < m_width*m_height -1)
    {
      ReadData(file, (char*)&ch_buf1, 1);
      if ((ch_buf1 & 128) == 128)
      {   // this is an rle packet
        ch_buf2 = (byte)((ch_buf1 & 127) + 1);   // how many pixels are encoded using this packet
        ReadData(file, (char*)buf1, m_pixelDepth/8);
        for (uint i=CurrentPixel; i<CurrentPixel+ch_buf2; i++)
          for (uint j=0; j<m_pixelDepth/8; j++)
          {
            int y = ((m_height-i-1)*m_pixelDepth)>>3;
            m_pixels[y+j] = buf1[j];
          }
          CurrentPixel += ch_buf2;
      }
      else
      {   // this is a raw packet
        ch_buf2 = (byte)((ch_buf1 & 127) + 1);
        ReadData(file, (char*)buf1, m_pixelDepth/8*ch_buf2);
        for (uint i=CurrentPixel; i<CurrentPixel+ch_buf2; i++)
          for (uint j=0; j<m_pixelDepth/8; j++)
          {
            int y = ((m_height-i-1)*m_pixelDepth)>>3;
            m_pixels[y+j] = buf1[(i-CurrentPixel)*m_pixelDepth/8+j];
          }
          CurrentPixel += ch_buf2;
      }
    }
  }
  
  if (TGAReadError != 0)
  {
    Clear();
    return false;
  }
  m_loaded = true;
  
  // swap BGR(A) to RGB(A)
  byte temp;
  if ((m_type == itRGB) || (m_type == itRGBA))
  {
    if ((m_pixelDepth == 24) || (m_pixelDepth == 32))
      for (uint i= 0; i<m_width*m_height; i++)
      {
        int y = (i*m_pixelDepth)>>3;
        temp  = m_pixels[y];
        m_pixels[y]   = m_pixels[y+2];
        m_pixels[y+2] = temp;
      }
  }
  
  return true;
}


//--------------------------------------------------
void LTGA::Clear()
{
  if (m_pixels)
    DISPOSE_ARRAY(m_pixels);
  m_pixels = 0;
  m_loaded = false;
  m_width = 0;
  m_height = 0;
  m_pixelDepth = 0;
  m_alphaDepth = 0;
  m_type = itUndefined;
}


//--------------------------------------------------
uint LTGA::GetAlphaDepth()
{
  return m_alphaDepth;
}


//--------------------------------------------------
uint LTGA::GetImageWidth()
{
  return m_width;
}


//--------------------------------------------------
uint LTGA::GetImageHeight()
{
  return m_height;
}


//--------------------------------------------------
uint LTGA::GetPixelDepth()
{
  return m_pixelDepth;
}


//--------------------------------------------------
byte* LTGA::GetPixels()
{
  return m_pixels;
}


//--------------------------------------------------
LImageType LTGA::GetImageType()
{
  return m_type;
}

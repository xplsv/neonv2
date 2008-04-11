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

#ifndef LTGA_H
#define LTGA_H
//------------------------------------------------

#include <string>

//------------------------------------------------

typedef unsigned char byte;
typedef unsigned int uint;

enum LImageType {itUndefined, itRGB, itRGBA, itGreyscale};

//------------------------------------------------
class LTGA
{
public:
    // default constructor, does nothing
    LTGA();
    // constructs the object and loads the given tga file
    LTGA(const string &sFile);
    // the destructor, cleans up the memory
    virtual ~LTGA();
    // this method loads a tga file. It clears all the data
    // if needed.
    bool LoadFromFile(const string &sFile);
    // this method clears the data, calling it is not nessesary, since it is
    // automatically called by the destructor
    void Clear();
    // this method retursn the depth of the alpha bitplane in the image
    // it can be either 0 or 8.
    uint GetAlphaDepth();
    // this method returns the width of the image.
    uint GetImageWidth();
    // this method returns the height of the image.
    uint GetImageHeight();
    // this method returns the pixel depth of the image. i.e. color 
    // depth. Can be 0 (no image loaded), 8 or 24. 16 bit images are not supported.
    uint GetPixelDepth();
    // this function returns the pointer to the image data. This pointer can be used
    // for glTexImage2D, for example.
    byte* GetPixels();
    // this function returns an image type. Imagetype can be either itUndefined (when
    // no tga file has been loaded, itRGB (when GetAlphaDepth returns 0 and GetPixelDepth
    // returns 24, itRGBA (when GetAlphaDepth returns 8 and GetPixelDepth returns 24), or
    // itGreyscale (when GetAlphaDepth returns 0 and GetPixelDepth returns 8).
    LImageType GetImageType();
protected:
    // this is the pixel buffer -> the image
    byte *m_pixels;
    // the pixel depth of the image, including the alpha bits
    uint m_pixelDepth;
    // the depth of the alpha bitplane
    uint m_alphaDepth;
    // the image height
    uint m_height;
    // the width of the image
    uint m_width;
    // image type, either rgb, rgba or greyscale 
    LImageType m_type;
    // m_loaded is true if a file has been loaded
    bool m_loaded;
};

//------------------------------------------------
#endif // LTGA_H
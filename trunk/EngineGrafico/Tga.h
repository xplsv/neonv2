//---------------------------------------------------------------------------//
// File: Tga.h
//
//---------------------------------------------------------------------------//

#ifndef TGA_H
#define TGA_H

#pragma pack(1)

struct TGAHeader
{ 
  byte   IdLength;            // Image ID Field Length      
  byte   CmapType;            // Color Map Type             
  byte   ImageType;           // Image Type                 
  
  //-- Color Map Specification --//
  word   CmapIndex;           // First Entry Index          
  word   CmapLength;          // Color Map Length           
  byte   CmapEntrySize;       // Color Map Entry Size       
  
  //-- Image Specification --//
  word   X_Origin;            // X-origin of Image          
  word   Y_Origin;            // Y-origin of Image          
  word   ImageWidth;          // Image Width                
  word   ImageHeight;         // Image Height               
  byte   PixelDepth;          // Pixel Depth                
  byte   ImagDesc;            // Image Descriptor           
};

#pragma pack()

#endif

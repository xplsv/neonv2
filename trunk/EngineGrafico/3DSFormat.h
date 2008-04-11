//---------------------------------------------------------------------------//
// File: 3DSFormat.h
//
//---------------------------------------------------------------------------//

#ifndef _3DSFORMAT_H
#define _3DSFORMAT_H

//>------ Primary chunk

#define MAIN3DS       0x4D4D

//>------ Main Chunks

#define EDIT3DS       0x3D3D  // this is the start of the editor config
#define KEYF3DS       0xB000  // this is the start of the keyframer config

//>------ sub defines of EDIT3DS

#define EDIT_MATERIAL 0xAFFF
#define EDIT_CONFIG1  0x0100
#define EDIT_CONFIG2  0x3E3D
#define EDIT_VIEW_P1  0x7012
#define EDIT_VIEW_P2  0x7011
#define EDIT_VIEW_P3  0x7020
#define EDIT_VIEW1    0x7001
#define EDIT_BACKGR   0x1200
#define EDIT_AMBIENT  0x2100
#define EDIT_OBJECT   0x4000

//>------ sub defines of EDIT_MATERIAL
#define MAT_NAME			0xA000

//>------ sub defines of EDIT_OBJECT
#define OBJ_TRIMESH   0x4100
#define OBJ_LIGHT     0x4600
#define OBJ_CAMERA    0x4700

//>------ sub defines of OBJ_CAMERA
#define CAM_UNKNWN01  0x4710 
#define CAM_UNKNWN02  0x4720 

//>------ sub defines of OBJ_LIGHT
#define LIT_OFF       0x4620
#define LIT_SPOT      0x4610
#define LIT_UNKNWN01  0x465A

//>------ sub defines of OBJ_TRIMESH
#define TRI_VERTEXL   0x4110
#define TRI_FACEL     0x4120
#define TRI_FACEM     0x4130
#define TRI_TEXL      0x4140
#define TRI_SMOOTH    0x4150
#define TRI_LOCAL     0x4160
#define TRI_VISIBLE   0x4165

//>>------ sub defs of KEYF3DS

#define KEYF_UNKNWN01 0xB009
#define KEYF_UNKNWN02 0xB00A

// keyframer chunk ids
#define KEYF_SEG      0xB008
#define KEYF_HDR      0xB00A
#define KEYF_OBJECT   0xB002
#define KEYF_CAMERA   0xB003
#define KEYF_LIGHT    0xB004
#define KEYF_NODE     0xB010
#define KEYF_PIVOT    0xB013
#define KEYF_POS      0xB020
#define KEYF_ROT      0xB021
#define KEYF_SCL      0xB022

//>>------  these define the different color chunk types
#define COL_RGB       0x0010
#define COL_TRU       0x0011
#define COL_UNK       0x0013

// Every chunk in the 3ds file starts with this struct
#pragma pack(1)
struct T3DSChunk
{
  unsigned short id;  // The chunk's id
  unsigned long  len; // The lenght of the chunk
};
#pragma pack()

//---------------------------------------------------------------------------//
// 3DS Chunk loader/parser
//---------------------------------------------------------------------------//
class CChunker
{
  public:
   
    CChunker(const char *pData)
    {
      m_pData  = pData;
      m_iDepth = 0;
    }

    // Functions
    bool Find(int iChunk)
    {
      do
      {
        if (Enter() == iChunk)
          return true;
        Leave();
      } while (!EndOf());
      return false;
    }

    int Enter()
    {
      memcpy(&m_Chunk, m_pData, sizeof(T3DSChunk));
      m_pStackIni[m_iDepth] = m_pData;
      m_pStackEnd[m_iDepth] = m_pData + m_Chunk.len;
      m_pData+=sizeof(T3DSChunk);
      m_iDepth++;
      return ID();
    }

    void Leave()
    {
      m_iDepth--;
      m_pData = m_pStackEnd[m_iDepth];
    }

    void First()
    {
      m_iDepth--;
      m_pData = m_pStackIni[m_iDepth];
      Enter();
    }

    // Gets
    bool        EndOf() const { return (m_pData >= m_pStackEnd[m_iDepth-1]); }
    unsigned    ID   () const { return m_Chunk.id; }
    const char *Data () const { return m_pData; }

    // Read Utils
    void ReadData(void *pDst, unsigned uSize)
    {
      memcpy(pDst, m_pData, uSize);
      m_pData+=uSize;
    }

    void ReadStr(char *pDst)
    {
      int iLen = 0;
      do
      {
        iLen++;
        *pDst++ = m_pData[iLen-1];
      } while(m_pData[iLen-1] != 0);
      m_pData+=iLen;
    }

  private:

    const char *m_pData;
    const char *m_pStackIni[64];
    const char *m_pStackEnd[64];
    int         m_iDepth;
    T3DSChunk   m_Chunk;
};

#endif

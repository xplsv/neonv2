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
#include "Fichero.h"


//---------------------------------------------------------------------------//
// Open
//
//---------------------------------------------------------------------------//
bool CFichero::Open(const char *pFile, const char *pAttribs)
{
  fopen_s(&m_hFile, pFile, pAttribs);
  if (!m_hFile)
  {
    GLOG(("ERR: Can't open file %s\n", pFile));
    return (false);
  }
  // Calcular el tamaño
  SeekToEnd();
  m_iLength = GetPos();
  SeekToIni();

  return (true);
}


//---------------------------------------------------------------------------//
// Close
//
//---------------------------------------------------------------------------//
void CFichero::Close()
{
  if (m_hFile)
  {
    fclose(m_hFile);
    m_hFile = NULL;
  }
}


//---------------------------------------------------------------------------//
// Read
//
//---------------------------------------------------------------------------//
int CFichero::Read(void *pData, int iChunkSize, int iChunkNum)
{
  return fread(pData, iChunkSize, iChunkNum, m_hFile);
}


//---------------------------------------------------------------------------//
// ReadLn
//
//---------------------------------------------------------------------------//
char *CFichero::ReadLn(char *pData, int iMaxBuffer)
{
  return fgets(pData, iMaxBuffer, m_hFile);
}


//---------------------------------------------------------------------------//
// Write
//
//---------------------------------------------------------------------------//
int CFichero::Write(const void *pData, int iChunkSize, int iChunkNum)
{
  return fwrite(pData, iChunkSize, iChunkNum, m_hFile);
}


//---------------------------------------------------------------------------//
// WriteLn
//
//---------------------------------------------------------------------------//
int CFichero::WriteLn(const char *pData)
{
  return fputs(pData, m_hFile);
}


//---------------------------------------------------------------------------//
// GetPos
//
//---------------------------------------------------------------------------//
int CFichero::GetPos()
{
  __int64 pos;
  fgetpos(m_hFile, &pos);
  return ((int)pos);
}


//---------------------------------------------------------------------------//
// Seek
//
//---------------------------------------------------------------------------//
void CFichero::Seek(int iOffset, int iMode)
{
  switch (iMode)
  {
    case BEGIN:   fseek(m_hFile, iOffset, SEEK_SET);   break;
    case CURRENT: fseek(m_hFile, iOffset, SEEK_CUR);   break;
    case END:     fseek(m_hFile, iOffset, SEEK_END);   break;
  }
}


//---------------------------------------------------------------------------//
// Eof
//
//---------------------------------------------------------------------------//
bool CFichero::Eof()
{
  return (feof(m_hFile) != 0);
}

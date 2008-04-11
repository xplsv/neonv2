//---------------------------------------------------------------------------//
// File: Ase.h
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Ase.h"
#include <stdio.h>

//---------------------------------------------------------------------------//
// Init
//
//---------------------------------------------------------------------------//
bool CAseFile::Init(const char *pszFile)
{
  CFichero File;
  if (!File.Open(pszFile))
    return false;

  // Cargar todo el fichero en memoria
  m_iSize     = File.GetLength();
  m_pData     = NEW_ARRAY(char, m_iSize);
  m_pKeyValue = NULL;
  File.Read (m_pData, m_iSize, 1);
  File.Close();
  ToBegin();

  m_bOk = true;

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CAseFile::End()
{
  if (IsOk())
  {
    DISPOSE_ARRAY(m_pData);
  }
}


//---------------------------------------------------------------------------//
// IntoChild
//
//---------------------------------------------------------------------------//
bool CAseFile::IntoChild(const char *pChild)
{
  ASSERT(IsOk());

  // Puntero aux
  int i      = m_iPos;
  int field  = 0;
  int iState = 0;
  int iPosIni= -1;
  do
  {
    switch (m_pData[i++])
    {
      // Key
      case '*':
        if (!field)
        {
          if (!strncmp(&m_pData[i], pChild, strlen(pChild)))
          {
            iState = 1;
            i=i+strlen(pChild)+1;
          }
        }
        break;
      // Node start
      case '{':
        if (!field && iState == 1)
          iPosIni = i+1;
        field++;
        break;
      // Node end
      case '}':
        field--;
        if (field < 0)
          return false;
        break;
    }
  } while ((iPosIni == -1) && (i < m_iSize));

  // Si lo encontramos nos posicionamos
  if (i < m_iSize)
    m_iPos = iPosIni;

  return (i < m_iSize);
}



//---------------------------------------------------------------------------//
// OutOfChild
//
//---------------------------------------------------------------------------//
void CAseFile::OutOfChild()
{
  ASSERT(IsOk());

  // Puntero aux
  int i      = m_iPos;
  int field  = 0;
  do
  {
    switch (m_pData[i++])
    {
      case '{': field++; break;
      case '}': field--; if (field < 0) { m_iPos = i+1; return; } break;
    }
  } while (i < m_iSize);
}


//---------------------------------------------------------------------------//
// IntoKey
//
//---------------------------------------------------------------------------//
bool CAseFile::IntoKey(const char *pKey)
{
  ASSERT(IsOk());
  
  int i     = m_iPos;
  int field = 0;
  do
  {
    switch (m_pData[i++])
    {
      case '*':
        if (!strncmp(&m_pData[i], pKey, strlen(pKey)) && !field )
        {
          // Si lo encontramos nos pisicionamos
          m_iPos = (i+strlen(pKey)+1);
          return true;
        }
        break;
      case '{': field++; break;
      case '}': field--; if (field < 0) return false; break;
    }
  } while (i < m_iSize);
  return false;
}


//---------------------------------------------------------------------------//
// CountKeys
//
//---------------------------------------------------------------------------//
int CAseFile::CountKeys(const char *pKey)
{
  ASSERT(IsOk());
  
  int iKeys = 0;
  int field = 0;
  int i     = m_iPos;
  do
  {
    switch (m_pData[i++])
    {
      case '*': if (!strncmp(&m_pData[i], pKey, strlen(pKey)) && !field) iKeys++; break;
      case '{': field++; break;
      case '}': field--; if (field < 0) return iKeys; break;
    }
  } while (i < m_iSize);
  return iKeys;
}

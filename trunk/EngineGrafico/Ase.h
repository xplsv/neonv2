//---------------------------------------------------------------------------//
// File: Ase.h
//
//---------------------------------------------------------------------------//

#ifndef ASE_H
#define ASE_H

class CAseFile
{
  public:
                CAseFile   () { m_bOk = false; }
               ~CAseFile   () { End(); }

    bool        Init       (const char *pszFile);
    void        End        ();
    bool        IsOk       () const { return m_bOk; }

    void        ToBegin    () { m_iPos = 0; }
    bool        IntoChild  (const char *pChild);
    void        OutOfChild ();
    bool        IntoKey    (const char *pKey);
    int         CountKeys  (const char *pKey);
    const char *GetData    () const { return &m_pData[m_iPos]; }
    int         GetPos     () const { return m_iPos; }
    void        SetPos     (int iPos) { m_iPos = iPos; }
  
  private:
    bool        m_bOk;
    char       *m_pData;
    char       *m_pKeyValue;
    int         m_iPos;
    int         m_iSize;
};

#endif

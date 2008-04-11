//---------------------------------------------------------------------------//
// File: GECamara3D.h
//
//---------------------------------------------------------------------------//

#ifndef GECAMARA3D_H
#define GECAMARA3D_H

class CPath;
class CFichero;

class CCamara3D
{
  public:
                    CCamara3D   () { m_bOk = false; }
                   ~CCamara3D   () { End(); }

    bool            Init        (CFichero *pFile, float fFrameEnd);
    void            End         ();
    bool            IsOk        () const { return m_bOk; }

    void            SetTime     (float fTime);
    void            Run         (float fRunTime);

    void            ResetFov    () { m_bOverrideFov = false; }
    void            SetFov      (float fFov) { m_bOverrideFov = true; m_fFov = fFov; }
    float           GetFov      () const { return  m_fFov; }
    TVector3 const *GetSrc      () const { return &m_vSrc; }
    TVector3 const *GetTgt      () const { return &m_vTgt; }
    char     const *GetNombre   () const { return  m_pNombre; }

  private:

    void            BorraVars   ();
    void            LiberaVars  ();

  private:          

    bool            m_bOk;
    char           *m_pNombre;
    bool            m_bFovStatic;
    bool            m_bSrcStatic;
    bool            m_bTgtStatic;
    bool            m_bOverrideFov;
    float           m_fFov;
    TVector3        m_vSrc;
    TVector3        m_vTgt;
    CPath          *m_pPathSrc;
    CPath          *m_pPathTgt;
    CPath          *m_pPathFov;
};

#endif

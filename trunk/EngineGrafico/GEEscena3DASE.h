//---------------------------------------------------------------------------//
// File: GEEscena3D.h
//
//---------------------------------------------------------------------------//

#ifndef GEESCENA3D_H
#define GEESCENA3D_H

#include "Lista.h"

class  CDisplayDevice;
class  CMaterial;
class  CObjeto3D;
class  CCamara3D;
class  CLuz3D;
class  CAseFile;

struct TMaterial
{
  int        iRef;
  int        iID;
  CMaterial *pMat;
};

class CEscena3D
{
  public:
    enum
    {
      GLOW        = 1<<0, // Excluyentes
      GLOW_GLOBAL = 1<<1, //
      FOG         = 1<<2,
      WAVE_TIME   = 1<<3,
      WAVE_XFORM  = 1<<4,
      
    };

  public:
                        CEscena3D    () { m_bOk = false; }
                       ~CEscena3D    () { End(); }

    bool                Init         (const char *pszFile);
    bool                InitASE      (const char *pszFile);
    void                End          ();
    bool                IsOk         () const { return m_bOk; }

    void                Reset        (float fTime);

    void                SetTime      (float fTime);
    void                SetCamara    (const char *pszCam);
    void                SetCamara    (unsigned iCam) { m_iCamaraActiva = iCam; }
    void                NextCamara   ();
    void                PrevCamara   (); 
    CCamara3D          *GetCamara    (unsigned iCam);

    void                Run          (float fRunTime);
    void                Draw         (CDisplayDevice *pDD);

  private:

    void                LoadMaterials(CAseFile *pFile);
    void                LoadMaterial (CAseFile *pFile, int iRef, int iID);
    void                BorraVars    ();
    void                LiberaVars   ();

  private:

    bool                m_bOk;
    float               m_fTime;
    float               m_fFirstFrame;
    float               m_fLastFrame;
    float               m_fTicksPerFrame;
    float               m_fFrameSpeed;
    float               m_fFPS;
    unsigned            m_iCamaraActiva;
    // Lista de componentes
    CLista<TMaterial *> m_ListaMateriales;
    CLista<CObjeto3D *> m_ListaObjetos;
    CLista<CCamara3D *> m_ListaCamaras;
    CLista<CLuz3D    *> m_ListaLuces;
};

#endif

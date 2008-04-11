 //---------------------------------------------------------------------------//
// File: FILTER.h
//
//---------------------------------------------------------------------------//

#ifndef _FILTER_H
#define _FILTER_H

class  CDisplayDevice;
class  COptions;
struct TCtrlVar;
struct TRenderTgt;

class CFilter
{
  public:
                          CFilter         () { m_Ok = false; }
    virtual              ~CFilter         () { End(); }

    virtual bool          Init            (COptions &aOptions) = 0;
    virtual void          End             ()       { m_Ok = false; }
            bool          IsOk            () const { return m_Ok; }

    virtual void          Run             (float fTime);
    virtual bool          Apply           (CDisplayDevice *pDD, TRenderTgt *pRenderTgt, int iTexTgt) = 0;

    virtual TCtrlVar     *GetVarCtrls     ();
    virtual void          SetVar          (int iVar, void *pData) { }
    virtual void         *GetVar          (int iVar) { return NULL; }

  protected:           

    bool                  m_Ok;
    float                 m_fTime;
    float                 m_fTimeAnt;
    float                 m_fTimeFrame;
};

#endif

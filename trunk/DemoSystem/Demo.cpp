//---------------------------------------------------------------------------//
// File: Demo.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Xml.h"
#include "Demo.h"
#include "Effect.h"
#include "Command.h"
#include "Lista.h"
#include "AviDecoder.h"
#include "Sound.h"

//---------------------------------------------------------------------------//
// Load
//
//---------------------------------------------------------------------------//
bool CDemo::Load(const char *pszFile)
{
  // XML Document
  /*
  TiXmlDocument XMLDoc(pszFile);
  if (XMLDoc.LoadFile() && XMLDoc.FirstChild("script"))
  {
    TiXmlNode *pParent = XMLDoc.FirstChild("script");

    //---------------------------------------------------------------------------
    // Cargar la lista de efectos
    m_ListaEfectos.Init();
    TiXmlNode *pNode = pParent->FirstChild("effects");
    if (pNode)
    {
      TiXmlNode *pIter = NULL;
      while (pIter = pNode->IterateChildren("effect", pIter))
      {
        // Crear la clase del efecto e inicializar
        TiXmlElement *pElem = pIter->ToElement();
        CEffect      *pFX   = CreaEfecto(pElem->Attribute("fxclass"));
        if (pFX && pFX->Init(pElem))
          m_ListaEfectos.Inserta(pFX);
        else
        {
          if (pFX)
            GLOG(("ERR: Can't load FXClass %s", pElem->Attribute("fxclass")));
          else
            GLOG(("ERR: Can't found FXClass %s", pElem->Attribute("fxclass")));
        }
      };
    }

    // Todo correcto
    Reset();
    m_bOk = true;
  }
  @ TODO
  */


  if (!IsOk())
    LiberaVars();

  return (IsOk());
}


//---------------------------------------------------------------------------//
// End
//
//---------------------------------------------------------------------------//
void CDemo::End()
{
  if (IsOk())
  {
    LiberaVars();
    m_bOk = false;
  }
}


//---------------------------------------------------------------------------//
// LiberaVars
//
//---------------------------------------------------------------------------//
void CDemo::LiberaVars()
{
  // Vaciar lista de effects
  VaciaListaConDispose(m_ListaEfectos);
  m_ListaEfectos.End();
  // Vaciar lista de commands
  VaciaListaConDispose(m_ListaComandos);
  m_ListaComandos.End();
}


//---------------------------------------------------------------------------//
// GetEffectByName
//
//---------------------------------------------------------------------------//
CEffect *CDemo::GetEffectByName(const char *pszName)
{
  if (!pszName) return NULL;
  CListaIter<CEffect *> Iter(m_ListaEfectos);
  // Vaciar lista de effectos
  while (!Iter.EsFinal())
  {
    CEffect *pFX = Iter;
    if (!Stricmp(pFX->GetName(), pszName))
      return pFX;
    Iter++;
  }
  return NULL;
}


//---------------------------------------------------------------------------//
// Reset
//
//---------------------------------------------------------------------------//
void CDemo::Reset()
{
  // Tiempo 0
  m_fTime = 0.f;
  
  // Marcar los comandos como no procesados
  CListaIter<TCommand *> IterComm(m_ListaComandos);
  while (!IterComm.EsFinal())
  {
    TCommand *pComm = IterComm;
    pComm->OnReset();
    IterComm++;
  }

  // Resetear todos los efectos
  CListaIter<CEffect *> Iter(m_ListaEfectos);
  while (!Iter.EsFinal())
  {
    CEffect *pFX = Iter;
    pFX->OnReset();
    Iter++;
  }
}


//---------------------------------------------------------------------------//
// SetTime
//
// Pone la demo en el tiempo absoluto fTime (random access)
//---------------------------------------------------------------------------//
void CDemo::SetTime(float fTime)
{
  Reset();
  Run  (fTime);
}


//---------------------------------------------------------------------------//
// Run
//
// Ejecuta el siguiente frame de la demo (continuos playback)
//---------------------------------------------------------------------------//
void CDemo::Run(float fRunTime)
{
  // Ejecutar la lista de efectos activos
  CListaIter<CEffect *> Iter(m_ListaEfectos);
  while (!Iter.EsFinal())
  {
    CEffect *pFX = Iter;
    if (pFX->IsEnabled())
      pFX->Run(fRunTime);
    Iter++;
  }

  // Guardamos el time actual
  m_fTime+=fRunTime;
}


//---------------------------------------------------------------------------//
// Draw
//
//---------------------------------------------------------------------------//
void CDemo::Draw(CDisplayDevice *pDD)
{
  pDD->GetD3DDevice()->BeginScene();
  CListaIter<CEffect *> Iter(m_ListaEfectos);
  while (!Iter.EsFinal())
  {
    CEffect *pFX = Iter;
    if (pFX->IsEnabled())
      pFX->Draw(pDD);
    Iter++;
  }
  pDD->GetD3DDevice()->EndScene();
}

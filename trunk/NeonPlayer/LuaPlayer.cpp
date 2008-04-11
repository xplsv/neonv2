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
#include "Event.h"
#include "LuaPlayer.h"
#include "LuaFile.h"
#include "VJController.h"
#include "VJLoader.h"
#include "AppWindow.h"
#include "AppOptions.h"
#include "Xml.h"

CLuaPlayer *g_LuaPlayer;

//----------------------------------------------------------------------------
// Init
//
//----------------------------------------------------------------------------
bool CLuaPlayer::Init(HINSTANCE hInst)
{
  g_LuaPlayer = this;

  m_VJController = NULL;
  m_VJController = NEW CVJController;
  if (!m_VJController->Init(NULL, 10, TModoGrafico(IAppOptions::GetInstance()->Get("Device", 0),
                                                   IAppOptions::GetInstance()->Get("Width", 640),
                                                   IAppOptions::GetInstance()->Get("Height", 480),
                                                   32,
                                                   IAppOptions::GetInstance()->Get("Refresh", 60),
                                                   IAppOptions::GetInstance()->Get("Fullscreen", 60) != 0,
                                                   false),
                                      TRect       (IAppOptions::GetInstance()->Get("RLeft", 0),
                                                   IAppOptions::GetInstance()->Get("RTop", 0),
                                                   IAppOptions::GetInstance()->Get("RWidth", 0),
                                                   IAppOptions::GetInstance()->Get("RHeight", 0)),
                                      TVectorI2   (IAppOptions::GetInstance()->Get("CustTex1W", 0),
                                                   IAppOptions::GetInstance()->Get("CustTex1H", 0)),
                                      TVectorI2   (IAppOptions::GetInstance()->Get("CustTex2W", 0),
                                                   IAppOptions::GetInstance()->Get("CustTex2H", 0)),
                                                   false,
                                                   true))
  {
    DISPOSE(m_VJController);
    return false;
  }

  m_Events.Init();  
  m_LuaFile = NEW CLuaFile;
  m_Ok = m_LuaFile->Init("demo.lua");
  DISPOSE(m_LuaFile);
  if (!m_Ok)
  {
    DISPOSE(m_VJController);
    return false;
  }

  return true;
}


//----------------------------------------------------------------------------
// End
//
//----------------------------------------------------------------------------
void CLuaPlayer::End()
{
  DISPOSE(m_VJController);
}


//----------------------------------------------------------------------------
// Update
//
//----------------------------------------------------------------------------
bool CLuaPlayer::Update()
{
  float fTime = m_VJController->GetTime() + m_StartTime;

  // Eventosssss
  if (!m_EventIter.EsFinal())
  {
    // Procesar los eventos pendientes
    TEvent *pEvent = m_EventIter;
    while (fTime > pEvent->fTime && pEvent->fTime >= 0.f)
    {
      ExecEventImm(pEvent, fTime);
      m_EventIter++;
      if (!m_EventIter.EsFinal())
        pEvent = m_EventIter;
      else
        break;
    }
  }
  // Procesar los eventos con duracion
  CListaIter<TEvent *> IterEvent(m_EventsRunning);
  while (!IterEvent.EsFinal())
  {
    TEvent *pEvent = IterEvent;
    // Value
    ExecEventRun(pEvent, fTime);
    IterEvent++;
    if (!IterEvent.EsFinal())
      pEvent = IterEvent;
    else
      break;
  }
  // Borrar los eventos con duracion caducados
  IterEvent.Init(m_EventsRunning);
  while (!IterEvent.EsFinal())
  {
    TEvent *pEvent = IterEvent;
    if (fTime > (pEvent->fDuration + pEvent->fTime))
    {
      IterEvent++;
      m_EventsRunning.Saca(pEvent);
      DISPOSE(pEvent);
    }
    else
      IterEvent++;
  }

  // Ruunnnnnnnnn
  m_VJController->Update();

  return (fTime <= m_Duration);
}


//----------------------------------------------------------------------------
// Draw
//
//----------------------------------------------------------------------------
void CLuaPlayer::Draw()
{
  m_VJController->Draw();
}
  

//----------------------------------------------------------------------------
// DemoInit
//
//----------------------------------------------------------------------------
void CLuaPlayer::DemoInit(float fDuration, float fStartTime, int iEffects, bool bEnableFPS)
{
  m_Duration  = fDuration;
  m_StartTime = fStartTime;
  m_VJLoader  = NEW CVJLoader;
  m_VJLoader->Init(iEffects+2);
}


//----------------------------------------------------------------------------
// DemoStart
//
//----------------------------------------------------------------------------
void CLuaPlayer::DemoStart()
{
  // Sort events
  m_EventsSorted.Init();
  while (m_Events.Cuenta() > 0)
  {
    TEvent *pEvent = NULL;
    CListaIter<TEvent *> Iter(m_Events);
    while (!Iter.EsFinal())
    {
      TEvent *pNewEvent = Iter;
      if (!pEvent || (pNewEvent->fTime < pEvent->fTime))
        pEvent = pNewEvent;
      Iter++;
    }
    Iter.End();
    m_Events.Saca(pEvent);
    m_EventsSorted.Inserta(pEvent);
  }
  // 
  m_VJController->GetSound()->Play(m_StartTime);
  m_VJController->Reset();
  m_EventIter.Init(m_EventsSorted);
}


//----------------------------------------------------------------------------
// LoadMusic
//
//----------------------------------------------------------------------------
void CLuaPlayer::LoadMusic(const string &sFile)
{
  m_VJController->GetSound()->Load(sFile);
  m_VJLoader->Next();
  m_VJLoader->Draw(&g_DisplayDevice);
  g_AppWindow.ProcessMessages();
}


//----------------------------------------------------------------------------
// GetValue
//
//----------------------------------------------------------------------------
float GetValue(CNodeFile::CNode *pNode, const string &sHijo, float fDef)
{
  bool bRes = false;
  CNodeFile::CNode *pHijo = pNode->FirstNode(sHijo.c_str());
  if (pHijo)
    return SafeFloat(pHijo->ToElement(), "value", fDef);
  return fDef;
}

int GetValue(CNodeFile::CNode *pNode, const string &sHijo, int iDef)
{
  bool bRes = false;
  CNodeFile::CNode *pHijo = pNode->FirstNode(sHijo.c_str());
  if (pHijo)
    return SafeInteger(pHijo->ToElement(), "value", iDef);
  return iDef;
}
 
const char *GetValue(CNodeFile::CNode *pNode, const string &sHijo, const char *sDef)
{
  bool bRes = false;
  CNodeFile::CNode *pHijo = pNode->FirstNode(sHijo.c_str());
  if (pHijo)
    return SafeString(pHijo->ToElement(), "value", sDef);
  return sDef;
}


//----------------------------------------------------------------------------
// LoadVars
//
//----------------------------------------------------------------------------
void CLuaPlayer::LoadVars(CNodeFile::CNode *pNode, int iFX, int iScope, int iObj)
{
  // FX Vars
  int iVar = 0;
  CNodeFile::CNode *pNodeFXVars = pNode->FirstNode("var");
  while (pNodeFXVars)
  {
    TCtrlVar *pCtrlVar = m_VJController->FXGetVarCtrls(iFX, iScope);
    float vf;
    int   vi;
    bool  vb;
    // Rezamos para que sea consecutivo (deberia serlo)
    switch (pCtrlVar[iVar].iType)
    {
      case TCtrlVar::SLIDER:
        vf = SafeFloat(pNodeFXVars->ToElement(), "value", 0.f) / 200.f;
        m_VJController->FXSetVar(iFX, iScope, iObj, iVar, &vf);
        break;
      case TCtrlVar::CHECK_BOX:
        vb = SafeBool(pNodeFXVars->ToElement(), "value", false);
        m_VJController->FXSetVar(iFX, iScope, iObj, iVar, &vb);
        break;
      case TCtrlVar::COMBO_BOX:
        vi = SafeInteger(pNodeFXVars->ToElement(), "value", 0);
        m_VJController->FXSetVar(iFX, iScope, iObj, iVar, &vi);
        break;
    }
    pNodeFXVars = pNodeFXVars->NextNode("var");
    iVar++;
  }
}


//----------------------------------------------------------------------------
// LoadVars
//
//----------------------------------------------------------------------------
void CLuaPlayer::LoadVars(CNodeFile::CNode *pNode, int iFX, int iFilter)
{
  // FX Vars
  int iVar = 0;
  CNodeFile::CNode *pNodeFXVars = pNode->FirstNode("var");
  while (pNodeFXVars)
  {
    TCtrlVar *pCtrlVar = m_VJController->FXGetFilterVarCtrls(iFX, iFilter);
    float vf;
    int   vi;
    bool  vb;
    // Rezamos para que sea consecutivo (deberia serlo)
    switch (pCtrlVar[iVar].iType)
    {
      case TCtrlVar::SLIDER:
        vf = SafeFloat(pNodeFXVars->ToElement(), "value", 0.f) / 200.f;
        m_VJController->FXSetFilterVar(iFX, iFilter, iVar, &vf);
        break;
      case TCtrlVar::CHECK_BOX:
        vb = SafeBool(pNodeFXVars->ToElement(), "value", false);
        m_VJController->FXSetFilterVar(iFX, iFilter, iVar, &vb);
        break;
      case TCtrlVar::COMBO_BOX:
        vi = SafeInteger(pNodeFXVars->ToElement(), "value", 0);
        m_VJController->FXSetFilterVar(iFX, iFilter, iVar, &vi);
        break;
    }
    pNodeFXVars = pNodeFXVars->NextNode("var");
    iVar++;
  }
}


//----------------------------------------------------------------------------
// LoadFX
//
//----------------------------------------------------------------------------
int CLuaPlayer::LoadFX(const string &sFile)
{
  CNodeFile NodeFile;
  if (!NodeFile.LoadFromFile(sFile))
  {
    GLOG(("ERR: Can't load file %s\n", sFile.c_str()));
    return false;
  }
  // Has nodes?
  CNodeFile::CNode *pNode = NodeFile.FirstNode("layer");
  if (!pNode) return false;

  // Effect file?
  CNodeFile::CNode *pNodeFX = pNode->FirstNode("effect");
  if (!pNodeFX) return false;

  // Load FX data
  const string &sFXClass  = pNode->AttrAsString("fxclass");
  const string &sFXDir    = pNode->AsString    ("fxdir");
  const string &sFXFile   = pNode->AsString    ("fxfile");

  PushDir("Effects/" + sFXDir);
  int iFX = m_VJController->FXLoad(sFXClass, sFXFile, NULL);
  PopDir();

  // Cargar las variables iniciales (filtros, vars, etc) 
  if (iFX != -1)
  {
    // Common vars
    CNodeFile::CNode *pNodeCommon = pNode->FirstNode("effect");
    m_VJController->FXSetSpeed  (iFX, GetValue(pNodeCommon, "speed",   200.f) * 1.f/200.f);
    m_VJController->FXSetAlpha  (iFX, GetValue(pNodeCommon, "alpha",   200.f) * 1.f/200.f);
    m_VJController->FXSetBlend  (iFX, GetValue(pNodeCommon, "blend",   0));
    m_VJController->FXSetQuality(iFX, GetValue(pNodeCommon, "quality", 0));
    m_VJController->FXSetFade   (iFX, GetValue(pNodeCommon, "fade",    250.f));

    CNodeFile::CNode *pNodeVars = pNode->FirstNode("vars");
    if (pNodeVars)
    {
      // Common Vars
      LoadVars(pNodeVars, iFX, -1, 0);

      // Object Vars
      CNodeFile::CNode *pNodeObjVars = pNodeVars->FirstNode("objvars");
      if (pNodeObjVars)
      {
        // Recorrer todos los objvar
        CNodeFile::CNode *pNodeObjVar = pNodeObjVars->FirstNode("objvar");
        while (pNodeObjVar)
        {
          int iScope = pNodeObjVar->AttrAsInt("scope", -1);
          int iObj   = pNodeObjVar->AttrAsInt("obj",   -1);
          LoadVars(pNodeObjVar, iFX, iScope, iObj);
        }
      }
    }

    // Filter Vars
    CNodeFile::CNode *pNodeFilter = pNode->FirstNode("filter");
    while (pNodeFilter)
    {
      int cell = pNodeFilter->AttrAsInt("cell");
      const string &sName = pNodeFilter->AttrAsString("name");
      // Try to load
      int iFilter = m_VJController->FXAddFilter(iFX, cell, sName);
      if (iFilter != -1)
        LoadVars(pNodeFilter, iFX, iFilter);
      pNodeFilter = pNodeFilter->NextNode("filter");
    }
  }
  m_VJLoader->Next();
  m_VJLoader->Draw(&g_DisplayDevice);
  g_AppWindow.ProcessMessages();
  return (iFX);
}


//----------------------------------------------------------------------------
// AddEvent
//
//----------------------------------------------------------------------------
void CLuaPlayer::AddEvent(TEvent *pEvent)
{
  m_Events.Inserta(pEvent);
}


//----------------------------------------------------------------------------
// ExecEventImm
//
//----------------------------------------------------------------------------
int GetFFVarName(TCtrlVar *pVars, const char *pVar)
{
  int i = 0;
  for (;;)
  {
    if (pVars[i].iType == TCtrlVar::INVALID)
      break;
    else if (!Stricmp(pVars[i].pszName, pVar))
      return i;
    i++;
  }
  return -1;
}

void CLuaPlayer::ExecEventImm(TEvent *pEvent, float fTime)
{
  // Get var ID from name
  if (pEvent->iEvent == EV_FF_SETVAR)
  {
    int iIDFilter = m_VJController->FXGetFilterID(pEvent->iFX, pEvent->iFilterSlot);
    pEvent->iSetVar = GetFFVarName(m_VJController->FXGetFilterVarCtrls(pEvent->iFX, iIDFilter), pEvent->pSetVar);
  }

  // Exec Event
  if (pEvent->fDuration > 0.f)
  {
    m_EventsRunning.Inserta(pEvent);
  }
  else
  {
    switch (pEvent->iEvent)
    {
      case EV_FX_ATTACH:     m_VJController->FXAttach      (pEvent->iFX, pEvent->iSetVarInt); break;
      case EV_FX_UNATTACH:   m_VJController->FXUnattach    (pEvent->iFX); break;
      case EV_FX_SETSPEED:   m_VJController->FXSetSpeed    (pEvent->iFX, pEvent->fSetVarFloat); break;
      case EV_FX_SETALPHA:   m_VJController->FXSetAlpha    (pEvent->iFX, pEvent->fSetVarFloat); break;
      case EV_FX_SETBLEND:   m_VJController->FXSetBlend    (pEvent->iFX, pEvent->iSetVarInt);   break;
      case EV_FX_SETTIME:    m_VJController->FXSetTime     (pEvent->iFX, pEvent->fSetVarFloat); break;
      case EV_FX_SETQUALITY: m_VJController->FXSetQuality  (pEvent->iFX, pEvent->iSetVarInt);   break;
      case EV_FX_SETFADE:    m_VJController->FXSetFade     (pEvent->iFX, pEvent->fSetVarFloat); break;
      case EV_FX_SETVAR:
        switch (pEvent->iSetVarType)
        {
          case SET_VAR_INT:   m_VJController->FXSetVar(pEvent->iFX, pEvent->iSetVarScope, pEvent->iSetVarObject, pEvent->iSetVar, &pEvent->iSetVarInt);   break;
          case SET_VAR_FLOAT: m_VJController->FXSetVar(pEvent->iFX, pEvent->iSetVarScope, pEvent->iSetVarObject, pEvent->iSetVar, &pEvent->fSetVarFloat); break;
          case SET_VAR_BOOL:  m_VJController->FXSetVar(pEvent->iFX, pEvent->iSetVarScope, pEvent->iSetVarObject, pEvent->iSetVar, &pEvent->bSetVarBool);  break;
        }
        break;
      case EV_FX_ADDFILTER:  m_VJController->FXAddFilter   (pEvent->iFX, pEvent->iFilterSlot, pEvent->pSetFilter); break;
      case EV_FX_DELFILTER:  m_VJController->FXRemoveFilter(pEvent->iFX, pEvent->iFilterSlot); break;
      case EV_FX_SWAPFILTER: m_VJController->FXSwapFilter  (pEvent->iFX, pEvent->iFilterSlot, pEvent->iSetVarInt); break;
      case EV_FF_SETVAR:    
        {
          int iIDFilter = m_VJController->FXGetFilterID(pEvent->iFX, pEvent->iFilterSlot);
          switch (pEvent->iSetVarType)
          {
            case SET_VAR_INT:   m_VJController->FXSetFilterVar(pEvent->iFX, iIDFilter, pEvent->iSetVar, &pEvent->iSetVarInt);   break;
            case SET_VAR_FLOAT: m_VJController->FXSetFilterVar(pEvent->iFX, iIDFilter, pEvent->iSetVar, &pEvent->fSetVarFloat); break;
            case SET_VAR_BOOL:  m_VJController->FXSetFilterVar(pEvent->iFX, iIDFilter, pEvent->iSetVar, &pEvent->bSetVarBool);  break;
          }
          break;
        }
    }
  }
}


//----------------------------------------------------------------------------
// ExecEventRun
//
//----------------------------------------------------------------------------
void CLuaPlayer::ExecEventRun(TEvent *pEvent, float fTime)
{
  switch (pEvent->iEvent)
  {
    case EV_FX_SETSPEED: m_VJController->FXSetSpeed    (pEvent->iFX, pEvent->GetValueF(fTime)); break;
    case EV_FX_SETALPHA: m_VJController->FXSetAlpha    (pEvent->iFX, pEvent->GetValueF(fTime)); break;
    case EV_FX_SETTIME:  m_VJController->FXSetTime     (pEvent->iFX, pEvent->GetValueF(fTime)); break;
    case EV_FX_SETVAR:   m_VJController->FXSetVar      (pEvent->iFX, pEvent->iSetVarScope, pEvent->iSetVarObject, pEvent->iSetVar, pEvent->GetValuePF(fTime)); break;
    case EV_FF_SETVAR:
      {
        int iIDFilter = m_VJController->FXGetFilterID(pEvent->iFX, pEvent->iFilterSlot);
        m_VJController->FXSetFilterVar(pEvent->iFX, iIDFilter, pEvent->iSetVar, pEvent->GetValuePF(fTime));
      }
      break;
  }
}



//---------------------------------------------------------------------------//
// Helpers
//
//---------------------------------------------------------------------------//
int CLuaPlayer::GetVarScope(int iFX, const char *pScope)
{
  vector<string> Scopes;
  m_VJController->FXGetVarScopes(iFX, Scopes);    
  for (unsigned i = 0; i < Scopes.size(); i++)
  {
    if (!Stricmp(Scopes[i], pScope))
      return i;
  }
  return -1;
}


int CLuaPlayer::GetVarObject(int iFX, int iScope, const char *pObject)
{
  vector<string> Objects;
  m_VJController->FXGetVarObjects(iFX, Objects, iScope);
  for (unsigned i = 0; i < Objects.size(); i++)
  {
    if (!Stricmp(Objects[i], pObject))
      return i;
  }
  return -1;
}


int CLuaPlayer::GetVarName(int iFX, int iScope, const char *pVar)
{
  TCtrlVar *pVars = m_VJController->FXGetVarCtrls(iFX);
  int i = 0;
  for (;;)
  {
    if (pVars[i].iType == TCtrlVar::INVALID)
      break;
    else if (!Stricmp(pVars[i].pszName, pVar))
      return i;
    i++;
  }
  return -1;
}
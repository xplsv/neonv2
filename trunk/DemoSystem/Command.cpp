//---------------------------------------------------------------------------//
// File: Command.cpp
//
//---------------------------------------------------------------------------//

#include "Base.h"
#include "Xml.h"
#include "Command.h"
#include "Filter.h"


//---------------------------------------------------------------------------//
// Init
//---------------------------------------------------------------------------//
TCommand::TCommand(TiXmlElement *pElem)
{
  bFree    = true;
  pName    = AllocCopyString(SafeString(pElem, "name", "Unnamed"));
  pComm    = AllocCopyString(SafeString(pElem, "command", "Unknown"));
  fTime    = SafeFloat(pElem->ToElement(), "time", -1.0f); // -1.f para los comandos del VJ en tiempo real
  // Argumentos
  apArgs   = NULL;
  iNumArgs = GetNumSameChilds(pElem, "arg");
  if (iNumArgs)
  {
    apArgs = NEW_ARRAY(char *, iNumArgs);
    TiXmlNode *pIter = NULL;
    for (int i = 0; i < iNumArgs; i++)
    {
      apArgs[i] = NULL;
      pIter = pElem->IterateChildren("arg", pIter);
      if (pIter->FirstChild())
        apArgs[i] = AllocCopyString(pIter->FirstChild()->Value());
    }
  }
}


//---------------------------------------------------------------------------//
// Init
//---------------------------------------------------------------------------//
TCommand::TCommand(const char *_pName, const char *_pComm, const char **_ppArgs, int _iArgs)
{
  bFree    = true;
  pName    = _pName ? AllocCopyString(_pName) : NULL;
  pComm    = AllocCopyString(_pComm);
  fTime    = -1.f;
  // Argumentos
  apArgs   = NULL;
  iNumArgs = _iArgs;
  if (iNumArgs)
  {
    apArgs = NEW_ARRAY(char *, iNumArgs);
    TiXmlNode *pIter = NULL;
    for (int i = 0; i < iNumArgs; i++)
    {
      apArgs[i] = NULL;
      if (_ppArgs && _ppArgs[i])
        apArgs[i] = AllocCopyString(_ppArgs[i]);
    }
  }
}


//---------------------------------------------------------------------------//
// TCommand destructor
//---------------------------------------------------------------------------//
TCommand::~TCommand()
{
  if (bFree)
  {
    for (int i = 0; i < iNumArgs; i++)
      DISPOSE_ARRAY(apArgs[i]);
    DISPOSE_ARRAY(apArgs);
    DISPOSE_ARRAY(pName);
    DISPOSE_ARRAY(pComm);
  }
}

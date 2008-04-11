  //---------------------------------------------------------------------------//
// File: Command.h
//
//---------------------------------------------------------------------------//

#ifndef COMMAND_H
#define COMMAND_H

class TiXmlElement;

// Estructura de un comando
struct TCommand
{
           TCommand    () { bFree = false; iNumArgs = 0; apArgs = NULL; };
           TCommand    (TiXmlElement *pElem);
           TCommand    (const char *pName, const char *pComm, const char **ppArgs, int iArgs);
          ~TCommand    ();
  float    fTime;      // Tiempo en el que se lanza
  char    *pName;      // Nombre del comando
  char    *pComm;      // Comando a ejecutar
  int      iNumArgs;   // \_
  char   **apArgs;     // /  Argumentos
  bool     bFree;      // Flag liberar memoria
};

#endif

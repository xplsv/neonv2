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

#ifndef LISTA_H
#define LISTA_H

#define CLLP  CLista
#define CLLPI CListaIter

template <class T> class CListaIter;

template <class T> class CLista
{
  public:
             CLista          () { m_bOk = false; Init(); }
            ~CLista          () { End(); }
  
    bool     Init            ();
    void     End             ();
    bool     IsOk            () const { return m_bOk; }

    void     Inserta         (const T &Obj) { InsertaUltimo(Obj); }
    void     InsertaUltimo   (const T &Obj);
    void     InsertaPrimero  (const T &Obj);

    bool     Saca            (const T &Obj);
    T        SacaPrimero     ();
    T        SacaUltimo      ();
    void     Vacia           ();
    void     VaciaConDispose ();

    T        CogePrimero     ();
    T        CogeUltimo      ();

    unsigned Cuenta          () const { return m_Cuenta; }
    bool     EstaInsertado   (const T &Obj);

  private:
    bool     m_bOk;

    struct TNodo
    {
      TNodo  *pPrev;
      TNodo  *pNext;
      T       Obj;
    };

    TNodo    *m_pPrimero;
    TNodo    *m_pUltimo;
    unsigned  m_Cuenta;
    unsigned  m_refc;

    void      BorraVars();
    void      LiberaVars();

    void      addref()      { m_refc++; }
    void      releaseref()  { m_refc--; }

    friend class CListaIter<T>; 
};

template <class T> class CListaIter
{
public:
          CListaIter      (CLista<T> &lista) { m_bOk = false; Init(lista); }
          CListaIter      () { m_bOk = false; }
          ~CListaIter     () { End(); }

  bool    Init            (CLista<T> &lista);
  void    End             ();
  bool    IsOk            () const { return m_bOk; }

          operator T      () { return m_pNodoActual->Obj; }
  void    operator++      (int) { m_pNodoActual = m_pNodoActual->pNext; }
  bool    EsFinal         () { if(m_pNodoActual == NULL) return true; else return false; }
  
private:
  bool                m_bOk;
  CLista<T>          *m_pLista;

  typename CLista<T>::TNodo   *m_pNodoActual;

  void  BorraVars     ();
  void  LiberaVars    ();
};

template <class T>bool CLista<T>::Init()
{
  End();
  BorraVars();

  m_bOk = true;
  
  return true;
}

template <class T> void CLista<T>::End()
{
  if(IsOk())
  {
    LiberaVars();
    m_bOk = false;
  }
}

template <class T> void CLista<T>::BorraVars()
{
  m_pPrimero = m_pUltimo = NULL;
  m_Cuenta = 0;
  m_refc = 0;
}

template <class T> void CLista<T>::LiberaVars()
{
  //ASSERT(m_refc == 0);

  TNodo *pNodo = m_pPrimero;
  while(pNodo)
  {
    TNodo *pFree = pNodo;
    pNodo = pNodo->pNext;
    delete pFree;
  }
}

template <class T> void CLista<T>::InsertaPrimero(const T &Obj)
{
  //ASSERT(m_refc == 0);

  TNodo *pNodo = NEW(TNodo);
  pNodo->Obj = Obj;
  pNodo->pPrev = NULL;
  pNodo->pNext = m_pPrimero;
  if(m_pPrimero == NULL)
    m_pUltimo = pNodo;
  else
    m_pPrimero->pPrev = pNodo;
  m_pPrimero = pNodo;
  m_Cuenta++;
}

template <class T> void CLista<T>::InsertaUltimo(const T &Obj)
{
  //ASSERT(m_refc == 0);

  TNodo *pNodo = NEW(TNodo);
  pNodo->Obj = Obj;
  pNodo->pPrev = m_pUltimo;
  pNodo->pNext = NULL;
  if(m_pUltimo == NULL)
    m_pPrimero = pNodo;
  else
    m_pUltimo->pNext = pNodo;
  m_pUltimo = pNodo;
  m_Cuenta++;
}

template <class T> bool CLista<T>::Saca(const T &Obj)
{
  //ASSERT(m_refc == 0);

  if(m_Cuenta == 0)
    return false;

  TNodo *pIter = m_pPrimero;

  if(pIter == m_pUltimo)
  {
    if(pIter->Obj == Obj)
    {
      DISPOSE(pIter);
      m_pPrimero = NULL;
      m_pUltimo = NULL;
      m_Cuenta = 0;
    }
  }

  while(pIter)
  {
    if(pIter->Obj == Obj)
    { //encontrado, lo sacamos
      if(pIter == m_pPrimero)
      {
        m_pPrimero = m_pPrimero->pNext;
        m_pPrimero->pPrev = NULL;
      }
      else
      {
        if(pIter == m_pUltimo)
        {
          m_pUltimo = m_pUltimo->pPrev;
          m_pUltimo->pNext = NULL;
        }
        else
        {
          pIter->pNext->pPrev = pIter->pPrev;
          pIter->pPrev->pNext = pIter->pNext;
        }
      }
      DISPOSE(pIter);
      m_Cuenta--;
      return true;
    }
    else
      pIter = pIter->pNext;
  }

  return false;
}

template <class T> T CLista<T>::SacaPrimero()
{
  //ASSERT(m_refc == 0);

  T RetVal;

  if(m_Cuenta)
  {
    RetVal = m_pPrimero->Obj;
    TNodo *pNodo = m_pPrimero;
    m_pPrimero = m_pPrimero->pNext;
    m_Cuenta--;
    if(m_Cuenta == 0)
      m_pUltimo = NULL;
    else
      m_pPrimero->pPrev = NULL;
    DISPOSE(pNodo);
  }

  return RetVal;
}

template <class T> T CLista<T>::SacaUltimo()
{
  //ASSERT(m_refc == 0);

  T RetVal;

  if(m_Cuenta)
  {
    RetVal = m_pUltimo->Obj;
    TNodo *pNodo = m_pUltimo;
    m_pUltimo = m_pUltimo->pPrev;
    m_Cuenta--;
    if(m_Cuenta == 0)
      m_pPrimero = NULL;
    else
      m_pUltimo->pNext = NULL;
    DISPOSE(pNodo);
  }

  return RetVal;
}

template <class T> void CLista<T>::Vacia()
{
  while(Cuenta())
    T t = SacaPrimero();
}

template <class T> void CLista<T>::VaciaConDispose()
{
  while(Cuenta())
  {
    T t = SacaPrimero();
    DISPOSE(t);
  }
}

template <class T> T CLista<T>::CogePrimero()
{
  T RetVal;

  if(m_Cuenta)
    RetVal = m_pPrimero->Obj;

  return RetVal;
}

template <class T> T CLista<T>::CogeUltimo()
{
  T RetVal;

  if(m_Cuenta)
    RetVal = m_pUltimo->Obj;

  return RetVal;
}

template <class T> bool CLista<T>::EstaInsertado(const T &Obj)
{
  TNodo *pIter = m_pPrimero;

  while(pIter)
  {
    if(pIter->Obj == Obj)
      return true;
    pIter = pIter->pNext;
  }

  return false;
}

template <class T> bool CListaIter<T>::Init(CLista<T> &lista)
{
  End();
  BorraVars();

  m_pLista = &lista;
  m_pNodoActual = (CLista<T>::TNodo *)m_pLista->m_pPrimero;
  m_bOk = true;
  m_pLista->addref();

  return true;
}

template <class T> void CListaIter<T>::End()
{
  if(IsOk())
  {
    LiberaVars();
    m_bOk = false;
  }
}

template <class T> void CListaIter<T>::BorraVars()
{
  m_pLista = NULL;
  m_pNodoActual = NULL;
  m_bOk = false;
}

template <class T> void CListaIter<T>::LiberaVars()
{
  m_pLista->releaseref();
}

#endif

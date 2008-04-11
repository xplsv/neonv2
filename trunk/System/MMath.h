//---------------------------------------------------------------------------//
// File: MMath.h
//
//---------------------------------------------------------------------------//

#ifndef _MMATH_H
#define _MMATH_H

inline int Round(double x)
{
  int i;
  static const float f = 0.5f;
  __asm
  {
    fld    x
    fadd   st,st(0)
    fadd   f
    fistp  i
    sar    i,1
  }
  return (i);
}

inline int Floor(double x)
{
  int i;
  static const float f = -0.5f;
  __asm
  {
    fld    x
    fadd   st,st(0)
    fadd   f
    fistp  i
    sar    i,1
  }
  return (i);
}

inline int Ceil(double x)
{
  int i;
  static const float f = -0.5f;
  __asm
  {
    fld    x
    fadd   st,st(0)
    fsubr  f
    fistp  i
    sar    i,1
  }
  return (-i);
}

#endif
